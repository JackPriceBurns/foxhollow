#ifndef MAIN_OBJSEQ_CONTROL_H_
#define MAIN_OBJSEQ_CONTROL_H_

/*
 * Sequence callbacks write these bits into ObjSeqState/ObjSeqState
 * byte +0x90. ObjSeq_update consumes the paired set/clear requests for three
 * per-slot latch arrays plus the saved-frame restart request. The high bit is
 * also observed by an object callback before ObjSeq_update clears the byte.
 */
#define OBJSEQ_CONTROL_SET_LATCH_B 0x01
#define OBJSEQ_CONTROL_CLEAR_LATCH_B 0x02
#define OBJSEQ_CONTROL_SET_LATCH_A 0x04
#define OBJSEQ_CONTROL_CLEAR_LATCH_A 0x08
#define OBJSEQ_CONTROL_SET_STATE_LATCH 0x10
#define OBJSEQ_CONTROL_CLEAR_STATE_LATCH 0x20
#define OBJSEQ_CONTROL_RESTART_AT_SAVED_FRAME 0x40
#define OBJSEQ_CONTROL_SUPPRESS_MESSAGES 0x80 /* suppress the callback's queued-message relay */

#endif /* MAIN_OBJSEQ_CONTROL_H_ */
