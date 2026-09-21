#pragma once

#include "borealis/file_select.hpp"

namespace borealis::file_select::detail {

void complete(Callback callback, Result result);
Result result_from_file_list(const char* const* fileList, const char* error);
std::string fallback_display_name(std::string_view location);

void open_macos_folder(FolderOptions options, Callback callback);
void open_ios_file(FileOptions options, Callback callback);
void open_android_folder(FolderOptions options, Callback callback);
std::string android_display_name(std::string_view location);

}  // namespace borealis::file_select::detail
