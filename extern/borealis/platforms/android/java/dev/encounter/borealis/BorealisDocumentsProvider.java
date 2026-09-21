package dev.encounter.borealis;

import android.content.ContentResolver;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.CancellationSignal;
import android.os.ParcelFileDescriptor;
import android.provider.DocumentsContract;
import android.provider.DocumentsContract.Document;
import android.provider.DocumentsContract.Root;
import android.provider.DocumentsProvider;
import android.webkit.MimeTypeMap;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

/** Exposes the active internal application-data directory to Android's Files UI. */
public class BorealisDocumentsProvider extends DocumentsProvider {
    private static final String ROOT_DOCUMENT_ID = "root";
    private static final String LOCATION_DESCRIPTOR_NAME = "data_location.json";
    private static final String DIRECTORY_MIME_TYPE = Document.MIME_TYPE_DIR;

    private static final String[] DEFAULT_ROOT_PROJECTION = new String[] {
        Root.COLUMN_ROOT_ID,
        Root.COLUMN_FLAGS,
        Root.COLUMN_TITLE,
        Root.COLUMN_DOCUMENT_ID,
        Root.COLUMN_ICON,
        Root.COLUMN_AVAILABLE_BYTES,
        Root.COLUMN_SUMMARY
    };

    private static final String[] DEFAULT_DOCUMENT_PROJECTION = new String[] {
        Document.COLUMN_DOCUMENT_ID,
        Document.COLUMN_DISPLAY_NAME,
        Document.COLUMN_FLAGS,
        Document.COLUMN_MIME_TYPE,
        Document.COLUMN_LAST_MODIFIED,
        Document.COLUMN_SIZE
    };

    @Override
    public boolean onCreate() {
        return true;
    }

    @Override
    public Cursor queryRoots(String[] projection) throws FileNotFoundException {
        MatrixCursor result = new MatrixCursor(resolveRootProjection(projection));
        if (!isInternalDataPathActive()) {
            return result;
        }

        File root = getRootDirectory();
        ApplicationInfo application = getContext().getApplicationInfo();
        CharSequence label = application.loadLabel(getContext().getPackageManager());
        String title = label != null ? label.toString() : getContext().getPackageName();
        MatrixCursor.RowBuilder row = result.newRow();
        row.add(Root.COLUMN_ROOT_ID, getContext().getPackageName());
        row.add(Root.COLUMN_FLAGS,
            Root.FLAG_LOCAL_ONLY | Root.FLAG_SUPPORTS_CREATE | Root.FLAG_SUPPORTS_IS_CHILD);
        row.add(Root.COLUMN_TITLE, title);
        row.add(Root.COLUMN_DOCUMENT_ID, ROOT_DOCUMENT_ID);
        row.add(Root.COLUMN_ICON, application.icon);
        row.add(Root.COLUMN_AVAILABLE_BYTES, root.getFreeSpace());
        row.add(Root.COLUMN_SUMMARY, title + " files");
        return result;
    }

    @Override
    public Cursor queryDocument(String documentId, String[] projection)
        throws FileNotFoundException
    {
        MatrixCursor result = new MatrixCursor(resolveDocumentProjection(projection));
        includeDocument(result, documentId, getFileForDocumentId(documentId));
        return result;
    }

    @Override
    public Cursor queryChildDocuments(
        String parentDocumentId, String[] projection, String sortOrder)
        throws FileNotFoundException
    {
        return queryChildDocumentsInternal(parentDocumentId, projection);
    }

    @Override
    public Cursor queryChildDocuments(
        String parentDocumentId, String[] projection, Bundle queryArgs)
        throws FileNotFoundException
    {
        return queryChildDocumentsInternal(parentDocumentId, projection);
    }

    private Cursor queryChildDocumentsInternal(String parentDocumentId, String[] projection)
        throws FileNotFoundException
    {
        MatrixCursor result = new MatrixCursor(resolveDocumentProjection(projection));
        File parent = getFileForDocumentId(parentDocumentId);
        File[] files = parent.listFiles();
        result.setNotificationUri(
            getContext().getContentResolver(), getChildDocumentsUri(parentDocumentId));
        if (files != null) {
            for (File file : files) {
                includeDocument(result, getDocumentIdForFile(file), file);
            }
        }
        return result;
    }

    @Override
    public boolean isChildDocument(String parentDocumentId, String documentId) {
        try {
            return isInside(
                getFileForDocumentId(parentDocumentId), getFileForDocumentId(documentId));
        } catch (FileNotFoundException e) {
            return false;
        }
    }

    @Override
    public String createDocument(String parentDocumentId, String mimeType, String displayName)
        throws FileNotFoundException
    {
        File parent = getFileForDocumentId(parentDocumentId);
        if (!parent.isDirectory()) {
            throw new FileNotFoundException("Parent is not a directory: " + parentDocumentId);
        }

        File file = buildUniqueFile(parent, sanitizeDisplayName(displayName));
        boolean created;
        if (DIRECTORY_MIME_TYPE.equals(mimeType)) {
            created = file.mkdir();
        } else {
            try {
                created = file.createNewFile();
            } catch (IOException e) {
                throw asFileNotFound("Unable to create document", e);
            }
        }
        if (!created) {
            throw new FileNotFoundException("Unable to create document: " + displayName);
        }

        notifyChildrenChanged(parentDocumentId);
        return getDocumentIdForFile(file);
    }

    @Override
    public String renameDocument(String documentId, String displayName)
        throws FileNotFoundException
    {
        if (ROOT_DOCUMENT_ID.equals(documentId)) {
            throw new FileNotFoundException("Cannot rename root document");
        }
        File file = getFileForDocumentId(documentId);
        File target = buildUniqueFile(file.getParentFile(), sanitizeDisplayName(displayName));
        String parentDocumentId = getDocumentIdForFile(file.getParentFile());
        if (!file.renameTo(target)) {
            throw new FileNotFoundException("Unable to rename document: " + documentId);
        }
        notifyDocumentChanged(documentId);
        notifyDocumentChanged(getDocumentIdForFile(target));
        notifyChildrenChanged(parentDocumentId);
        return getDocumentIdForFile(target);
    }

    @Override
    public void deleteDocument(String documentId) throws FileNotFoundException {
        if (ROOT_DOCUMENT_ID.equals(documentId)) {
            throw new FileNotFoundException("Cannot delete root document");
        }
        File file = getFileForDocumentId(documentId);
        String parentDocumentId = getDocumentIdForFile(file.getParentFile());
        deleteRecursively(file);
        notifyDocumentChanged(documentId);
        notifyChildrenChanged(parentDocumentId);
    }

    @Override
    public ParcelFileDescriptor openDocument(
        String documentId, String mode, CancellationSignal signal) throws FileNotFoundException
    {
        return ParcelFileDescriptor.open(getFileForDocumentId(documentId), modeToParcelMode(mode));
    }

    @Override
    public AssetFileDescriptor openDocumentThumbnail(
        String documentId, android.graphics.Point sizeHint, CancellationSignal signal)
        throws FileNotFoundException
    {
        throw new FileNotFoundException("Thumbnails are not supported");
    }

    private void includeDocument(MatrixCursor result, String documentId, File file)
        throws FileNotFoundException
    {
        MatrixCursor.RowBuilder row = result.newRow();
        boolean isDirectory = file.isDirectory();
        String displayName = ROOT_DOCUMENT_ID.equals(documentId)
            ? String.valueOf(getContext().getApplicationInfo().loadLabel(
                getContext().getPackageManager()))
            : file.getName();

        int flags = Document.FLAG_SUPPORTS_DELETE | Document.FLAG_SUPPORTS_RENAME;
        if (isDirectory) {
            flags |= Document.FLAG_DIR_SUPPORTS_CREATE;
        } else if (file.canWrite()) {
            flags |= Document.FLAG_SUPPORTS_WRITE;
        }
        if (ROOT_DOCUMENT_ID.equals(documentId)) {
            flags &= ~(Document.FLAG_SUPPORTS_DELETE | Document.FLAG_SUPPORTS_RENAME);
        }

        row.add(Document.COLUMN_DOCUMENT_ID, documentId);
        row.add(Document.COLUMN_DISPLAY_NAME, displayName);
        row.add(Document.COLUMN_FLAGS, flags);
        row.add(Document.COLUMN_MIME_TYPE,
            isDirectory ? DIRECTORY_MIME_TYPE : getMimeType(file));
        row.add(Document.COLUMN_LAST_MODIFIED, file.lastModified());
        row.add(Document.COLUMN_SIZE, isDirectory ? null : file.length());
    }

    private File getRootDirectory() throws FileNotFoundException {
        if (!isInternalDataPathActive()) {
            throw new FileNotFoundException(
                "DocumentsProvider is disabled while an external data path is configured");
        }
        File root = getContext().getFilesDir();
        if (root == null) {
            throw new FileNotFoundException("Application files directory is unavailable");
        }
        return root;
    }

    private File getFileForDocumentId(String documentId) throws FileNotFoundException {
        File root = getRootDirectory();
        if (ROOT_DOCUMENT_ID.equals(documentId)) {
            return root;
        }
        if (!documentId.startsWith(ROOT_DOCUMENT_ID + "/")) {
            throw new FileNotFoundException("Invalid document id: " + documentId);
        }

        File file = new File(root, documentId.substring(ROOT_DOCUMENT_ID.length() + 1));
        if (!isInside(root, file)) {
            throw new FileNotFoundException("Document escapes application files: " + documentId);
        }
        if (!file.exists()) {
            throw new FileNotFoundException("Document does not exist: " + documentId);
        }
        return file;
    }

    private String getDocumentIdForFile(File file) throws FileNotFoundException {
        File root = getRootDirectory();
        if (sameFile(root, file)) {
            return ROOT_DOCUMENT_ID;
        }
        if (!isInside(root, file)) {
            throw new FileNotFoundException("File escapes application files: " + file);
        }
        String rootPath = canonicalPath(root);
        String filePath = canonicalPath(file);
        return ROOT_DOCUMENT_ID + "/" + filePath.substring(rootPath.length() + 1);
    }

    private boolean isInternalDataPathActive() {
        if (getContext() == null || getContext().getFilesDir() == null) {
            return false;
        }
        File descriptor = new File(getContext().getFilesDir(), LOCATION_DESCRIPTOR_NAME);
        if (!descriptor.isFile()) {
            return true;
        }
        try {
            JSONObject json = new JSONObject(readText(descriptor));
            return "default".equals(json.optString("mode", "default"));
        } catch (IOException | JSONException e) {
            return true;
        }
    }

    private String authority() {
        return getContext().getPackageName() + ".documents";
    }

    private Uri getChildDocumentsUri(String parentDocumentId) {
        return DocumentsContract.buildChildDocumentsUri(authority(), parentDocumentId);
    }

    private void notifyChildrenChanged(String parentDocumentId) {
        ContentResolver resolver = getContext().getContentResolver();
        resolver.notifyChange(getChildDocumentsUri(parentDocumentId), null, false);
    }

    private void notifyDocumentChanged(String documentId) {
        ContentResolver resolver = getContext().getContentResolver();
        resolver.notifyChange(
            DocumentsContract.buildDocumentUri(authority(), documentId), null, false);
    }

    private static String readText(File file) throws IOException {
        try (FileInputStream input = new FileInputStream(file);
             ByteArrayOutputStream output = new ByteArrayOutputStream())
        {
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = input.read(buffer)) != -1) {
                output.write(buffer, 0, bytesRead);
            }
            return output.toString(StandardCharsets.UTF_8.name());
        }
    }

    private static String[] resolveRootProjection(String[] projection) {
        return projection != null ? projection : DEFAULT_ROOT_PROJECTION;
    }

    private static String[] resolveDocumentProjection(String[] projection) {
        return projection != null ? projection : DEFAULT_DOCUMENT_PROJECTION;
    }

    private static String sanitizeDisplayName(String displayName) throws FileNotFoundException {
        if (displayName == null) {
            throw new FileNotFoundException("Document name is empty");
        }
        String sanitized = displayName.trim();
        if (sanitized.isEmpty() || ".".equals(sanitized) || "..".equals(sanitized) ||
            sanitized.contains("/") || sanitized.contains("\\"))
        {
            throw new FileNotFoundException("Invalid document name: " + displayName);
        }
        return sanitized;
    }

    private static File buildUniqueFile(File parent, String displayName) {
        File file = new File(parent, displayName);
        if (!file.exists()) {
            return file;
        }
        int dot = displayName.lastIndexOf('.');
        String baseName = dot > 0 ? displayName.substring(0, dot) : displayName;
        String extension = dot > 0 ? displayName.substring(dot) : "";
        for (int i = 1; i < 100; ++i) {
            file = new File(parent, baseName + " (" + i + ")" + extension);
            if (!file.exists()) {
                return file;
            }
        }
        return new File(parent, baseName + " (" + System.currentTimeMillis() + ")" + extension);
    }

    private static int modeToParcelMode(String mode) {
        if ("r".equals(mode)) {
            return ParcelFileDescriptor.MODE_READ_ONLY;
        }
        if ("w".equals(mode) || "wt".equals(mode)) {
            return ParcelFileDescriptor.MODE_WRITE_ONLY |
                ParcelFileDescriptor.MODE_CREATE | ParcelFileDescriptor.MODE_TRUNCATE;
        }
        if ("wa".equals(mode)) {
            return ParcelFileDescriptor.MODE_WRITE_ONLY |
                ParcelFileDescriptor.MODE_CREATE | ParcelFileDescriptor.MODE_APPEND;
        }
        if ("rw".equals(mode)) {
            return ParcelFileDescriptor.MODE_READ_WRITE | ParcelFileDescriptor.MODE_CREATE;
        }
        if ("rwt".equals(mode)) {
            return ParcelFileDescriptor.MODE_READ_WRITE |
                ParcelFileDescriptor.MODE_CREATE | ParcelFileDescriptor.MODE_TRUNCATE;
        }
        return ParcelFileDescriptor.MODE_READ_ONLY;
    }

    private static String getMimeType(File file) {
        int dot = file.getName().lastIndexOf('.');
        if (dot >= 0) {
            String extension = file.getName().substring(dot + 1).toLowerCase();
            String mimeType = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension);
            if (mimeType != null) {
                return mimeType;
            }
        }
        return "application/octet-stream";
    }

    private static void deleteRecursively(File file) throws FileNotFoundException {
        if (file.isDirectory()) {
            File[] children = file.listFiles();
            if (children != null) {
                for (File child : children) {
                    deleteRecursively(child);
                }
            }
        }
        if (!file.delete()) {
            throw new FileNotFoundException("Unable to delete document: " + file);
        }
    }

    private static boolean isInside(File parent, File child) {
        try {
            String parentPath = canonicalPath(parent);
            String childPath = canonicalPath(child);
            return childPath.equals(parentPath) || childPath.startsWith(parentPath + File.separator);
        } catch (FileNotFoundException e) {
            return false;
        }
    }

    private static boolean sameFile(File first, File second) {
        try {
            return canonicalPath(first).equals(canonicalPath(second));
        } catch (FileNotFoundException e) {
            return false;
        }
    }

    private static String canonicalPath(File file) throws FileNotFoundException {
        try {
            return file.getCanonicalPath();
        } catch (IOException e) {
            throw asFileNotFound("Unable to resolve path", e);
        }
    }

    private static FileNotFoundException asFileNotFound(String message, IOException cause) {
        FileNotFoundException exception =
            new FileNotFoundException(message + ": " + cause.getMessage());
        exception.initCause(cause);
        return exception;
    }
}
