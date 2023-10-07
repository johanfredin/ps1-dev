#ifndef _LIBSND_H_
#define _LIBSND_H_

/*****************************************************************
 *
 * $RCSfile: libsnd.h,v $
 *
 * 	Copyright (C) 1994 by Sony Computer Entertainment Inc.
 *				          All Rights Reserved.
 *
 *	Sony Computer Entertainment Inc. Development Department
 *
 *****************************************************************/
/*
 * $PSLibId: Run-time Library Release 4.7$
 */

#include <sys/types.h>

/*
 * Macro 
 */
#define SSPLAY_INFINITY  0
#define SS_NOTICK     0x1000
#define SS_NOTICK0     0
#define SS_TICK60     1
#define SS_TICK240     2
#define SS_TICK120     3
#define SS_TICK50     4
#define SS_TICKVSYNC     5
#define SS_TICKMODE_MAX  6
#define SSPLAY_PAUSE     0
#define SSPLAY_PLAY      1
#define SS_SOFF          0
#define SS_SON           1
#define SS_MIX           0
#define SS_REV           1
#define SS_SERIAL_A      0
#define SS_SERIAL_B      1
#define SS_MUTE_OFF      0
#define SS_MUTE_ON       1

#define SS_IMEDIATE        0
#define SS_IMMEDIATE        0
#define SS_WAIT_COMPLETED   1

#define SS_REV_TYPE_OFF        0
#define SS_REV_TYPE_ROOM       1
#define SS_REV_TYPE_STUDIO_A   2
#define SS_REV_TYPE_STUDIO_B   3
#define SS_REV_TYPE_STUDIO_C   4
#define SS_REV_TYPE_HALL       5
#define SS_REV_TYPE_SPACE      6
#define SS_REV_TYPE_ECHO       7
#define SS_REV_TYPE_DELAY      8
#define SS_REV_TYPE_PIPE       9
#define SSSKIP_TICK    0
#define SSSKIP_NOTE4   1
#define SSSKIP_NOTE8   2
#define SSSKIP_BAR     3

#define SS_SEQ_TABSIZ     176

#define SND_VOLL  1
#define SND_VOLR  2
#define SND_ADSR1 4
#define SND_ADSR2 8
#define SND_ADDR  16
#define SND_PITCH 32

#ifndef NULL
#define NULL 0
#endif


/*
 * Vag & Vab Structure 
 */
typedef struct VabHdr {         /* VAB Bank Headdings */

    long form;          /* always 'VABp' */
    long ver;           /* VAB file version number */
    long id;            /* VAB id */
    unsigned long fsize;         /* VAB file size */
    unsigned short reserved0;     /* system reserved */
    unsigned short ps;            /* # of the programs in this bank */
    unsigned short ts;            /* # of the tones in this bank */
    unsigned short vs;            /* # of the vags in this bank */
    unsigned char mvol;          /* master volume for this bank */
    unsigned char pan;           /* master panning for this bank */
    unsigned char attr1;         /* bank attributes1 */
    unsigned char attr2;         /* bank attributes2 */
    unsigned long reserved1;     /* system reserved */

} VabHdr;            /* 32 byte */


typedef struct ProgAtr {        /* Program Headdings */

    unsigned char tones;          /* # of tones */
    unsigned char mvol;           /* program volume */
    unsigned char prior;          /* program priority */
    unsigned char mode;           /* program mode */
    unsigned char mpan;           /* program pan */
    char reserved0;      /* system reserved */
    short attr;           /* program attribute */
    unsigned long reserved1;      /* system reserved */
    unsigned long reserved2;      /* system reserved */

} ProgAtr;            /* 16 byte */


typedef struct VagAtr {         /* VAG Tone Headdings */

    unsigned char prior;         /* tone priority */
    unsigned char mode;          /* play mode */
    unsigned char vol;           /* tone volume*/
    unsigned char pan;           /* tone panning */
    unsigned char center;        /* center note */
    unsigned char shift;         /* center note fine tune */
    unsigned char min;           /* minimam note limit */
    unsigned char max;           /* maximam note limit */
    unsigned char vibW;          /* vibrate depth */
    unsigned char vibT;          /* vibrate duration */
    unsigned char porW;          /* portamento depth */
    unsigned char porT;          /* portamento duration */
    unsigned char pbmin;         /* under pitch bend max */
    unsigned char pbmax;         /* upper pitch bend max */
    unsigned char reserved1;     /* system reserved */
    unsigned char reserved2;     /* system reserved */
    unsigned short adsr1;         /* adsr1 */
    unsigned short adsr2;         /* adsr2 */
    short prog;          /* parent program*/
    short vag;           /* vag reference */
    short reserved[4];   /* system reserved */

} VagAtr;            /* 32 byte */


/*
 * Volume Structure
 */
typedef struct {
    unsigned short left;    /* L Channel */
    unsigned short right;   /* R Channel */
} SndVolume;

typedef struct SndVolume2 {
    short left;
    short right;
} SndVolume2;

typedef struct SndRegisterAttr {
    SndVolume2 volume;
    short pitch;
    short mask;
    short addr;
    short adsr1;
    short adsr2;
} SndRegisterAttr;

typedef struct SndVoiceStats {
    short vagId;
    short vabId;
    unsigned short pitch;
    short note;
    short tone;
    short prog_num;
    short prog_actual;
    short vol;
    short pan;
} SndVoiceStats;

/*
 * CallBack
 */
typedef void (*SsMarkCallbackProc)(short, short, short);


/*  
 * Prototype 
 */
#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
* Sets up a VAB header list in main memory so that it can be used by the Sound Library. A VAB ID may be
* specified to be used for opening, or if vabid is set to -1, the function allocates an empty VAB ID (0 - 15) if
* there is one. Execute SsVabTransBody() to transfer the VAB body to the SPU RAM, and
* SsVabTransCompleted() to confirm completion of the transfer.
* This function reserves an area in SPU RAM for the VAB body in multiples of 64 bytes. Therefore, this area
* can be larger than the actual VAB body by up to 48 bytes (since the body is in sections of 16 bytes).
* Since SsVabOpenHead(), SsVabFakeHead() and SsVabOpenHeadSticky() use and alter the system
* reserved area of the header list, the reserved area cannot be rewritten after header list recognition.
* @param addr Start address of VAB header (.VH) in main memory
* @param vabid VAB ID
* @return VAB identification number.
* Returns -1 if unsuccessful. See error codes under SsVabOpenHeadSticky() for details. Also returns error if
* there isn’t enough room in SPU RAM for the VAB.
*/
extern short SsVabOpenHead(unsigned char *addr, short vabid);

extern short SsVabOpenHeadSticky(unsigned char *, short, unsigned long);

/**
 * After SsVabOpenHead() recognizes a header list, this function starts transferring sound source data (VAB
 * body) in main memory to SPU local memory. Data is transferred in 64-byte units. Use
 * SsVabTransCompleted() to confirm transfer completion.
 * The starting address (addr ) in the sound buffer into which VabBody(.VB) is transferred must be in the range
 * 0x1010-0x7ffff. It must take into account the size of the .VB, so that data is not transferred into the reverb
 * work area.
 * @param addr VAB data start address
 * @param vabid
 * @return VAB identifying number. Returns -1 if unsuccessful
 */
extern short SsVabTransBody(unsigned char *addr, short vabid);

extern short SsVabTransBodyPartly(unsigned char *, unsigned long, short);

extern short SsVabTransfer(unsigned char *, unsigned char *, short, short);

/**
 * Get VAB data transfer state.<br>
 * Determines whether data transfer to SPU local memory has terminated.
 * If immediateFlag is SS_IMMEDIATE, the function returns the transfer state immediately. If immediateFlag is
 * SS_WAIT_COMPLETED, the function loops until transfer is completed.
 * @param immediateFlag Transfer status recognition flag
 * @return 1 if the transfer has been completed, 0 if the transfer is ongoing.
 */
extern short SsVabTransCompleted(short immediateFlag);

extern void SsVabClose(short);

/**
 * Initializes the sound system, clearing the sound local memory.
 */
extern void SsInit(void);

extern void SsInitHot(void);

/**
 * Specify the area of a SEQ/SEP data attribute table.<br>
 * Specifies the area of a SEQ/SEP data attribute table. The library uses this area to analyze SEQ/SEP data,
 * then saves it and plays it back.
 * s_max specifies the maximum number of times SEQ/SEP data may be opened. The upper limit is 32. Once
 * s_max is reached, unused SEQ/SEP data must be closed with SsSeqClose()/SsSepClose() before more
 * data can be opened. t_max specifies the number of SEQ included in the largest SEP data. Set t_max to 1
 * to handle only SEQ data and not use SEP data. The upper limit of t_max is 16.
 * You must preserve the area for the table by using global variables or functions like malloc() (auto variables
 * cannot be used).
 * Use the following to find the size:
 * (SS_SEQ_TABSIZ x s_max x t_max)
 * where the constant SS_SEQ_TABSIZ is declared in libsnd.h. (note that its value may vary from version to
 * version).
 * SsSetTableSize() should be called immediately after SsInit(). Both functions should be called only once;
 * what happens when multiple calls are made is unclear.
 * @param table Pointer to SEQ/SEP data attribute table area variable
 * @param s_max Maximum times to open SEQ/SEP data (up to 32)
 * @param t_max Number of SEQ included in SEP
 */
extern void SsSetTableSize(char *table, short s_max, short t_max);

/**
 * Sets the resolution of a tick. Call this function only once before calling SsSeqOpen(), SsSepOpen() or
 * SsStart() for the first time. When it is called multiple times, correct operation cannot be guaranteed.
 * The tick mode does not depend on the speed or resolution specified by SEQ/SEP data, and merely
 * specifies the resolution inside the sound system.
 * The effects of SS_TICK50, SS_TICK60, and SS_TICKVSYNC differ according to the specification of
 * SetVideoMode() (see the individual entries below).
 * tick_mode may be specified with the following values:
 * <ul>
 * <li>SS_TICK50 1/50 second</li>
 * <li>SS_TICK60 1/60 second</li>
 * <li>SS_TICKVSYNC VSync Resolution (1/50 PAL, 1/60 NTSC)</li>
 * <li>SS_TICK120 1/120 second</li>
 * <li>SS_TICK240 1/240 second</li>
 * <li>SS_NOTICK 1/60 second*</li>
 * <li>Any resolution (60-240) 1/tick_mode seconds</li>
 * <li>Any resolution | SS_NOTICK 1/tick_mode seconds*</li>
 * </ul>
 * *SsSeqCalledTbyT() is called automatically every tick, except when tick_mode is SS_NOTICK or (any
 * resolution | SS_NOTICK). In those cases, the program must call SsSeqCalledTbyT() at the specified timing.
 * “Any resolution” means that you specify a value between 60-240, and the resolution is 1/tick_mode.
 * Example: tick_mode = 65 | SS_NOTICK sets up a resolution of 1/65th second.
 * The OS Root Counter RCntCNT3 is used to generate VSYNC timing. Therefore, if you use SS_TICK50
 * with MODE_PAL (specified in SetVideoMode()), or SS_TICK60 with MODE_NTSC, or SS_TICKVSYNC, you
 * shouldn’t use RCntCNT3 for any other timing resolution.
 * The OS Root Counter RCntCNT2 is used for all other tick modes. Therefore, you shouldn’t use RCntCNT2
 * for any other timing resolution.
 * @param tick_mode Tick mode (see table for values)
 */
extern void SsSetTickMode(long tick_mode);

extern int SsSetTickCallback(void (*cb)());

/**
 * Start the sound system.
 * Starts the sound system.
 * When SsSetTickMode() is used to set a mode that calls SsSeqCalledTbyT() automatically, this function
 * causes SsSeqCalledTbyT() to be called each tick.
 */
extern void SsStart(void);

/**
 * Start the sound system (VSyncCallback version).
 * When SsSetTickMode() is used to set a mode that calls SsSeqCalledTbyT() automatically, this function
 * causes SsSeqCalledTbyT() to be called each tick.
 * SsStart2() must be used when the tick mode is equal to the TV’s sync rate (e.g. SS_TICK60 on NTSC or
 * SS_TICK50 on PAL). In may also be used with other modes.
 */
extern void SsStart2(void);

extern void SsEnd(void);

extern void SsQuit(void);

/**
 * Interpret SEQ/SEP data and carry out playback.<br>
 * At each Tick this function is called; it interprets SEQ/SEP data and carries out playback. The tick rate is set
 * by SsSetTickMode(), but this merely regulates the internal sound system, without depending either on the
 * speed or resolution determined by SEQ/SEP data.
 * When SsSetTickMode() is called with tick_mode SS_NOTICK, this function must be called by the program
 * (usually with vertical sync timing). Otherwise, the sound processing code automatically calls this function at
 * the given resolution.
 */
extern void SsSeqCalledTbyT(void);

/**
 * Open SEQ data.<br>
 * Designates an SEQ number for the SEQ data to allow playback.
 * Before calling this function, you must have confirmed with SsVabTransCompleted() that the VAB data from
 * the vab_id has completed being transferred to the sound buffer.
 * @param addr Pointer to start address of SEQ data in the main storage
 * @param vab_id VAB id
 * @return SEQ access number. This value is passed to other SEQ routines such as SsSeqPlay().
 * If you try to open more than 32 SEP data (combined with SEQ data) at the same time, -1 is returned.
 */
extern short SsSeqOpen(unsigned long *addr, short vab_id);

/**
 * Read (play) SEQ data.<br>
 * Selects either immediate SEQ data reading (play_mode = SSPLAY_PLAY) or sets a pause state at the start
 * of SEQ data (play_mode = SSPLAY_PAUSE). Specify the number of times to repeat the music by l_count,
 * using SSPLAY_INFINITY for unlimited play.
 * @param seq_access_num SEQ access number
 * @param play_mode Performance mode
 * @param l_count Number of repeats of the music
 */
extern void SsSeqPlay(short seq_access_num, char play_mode, short l_count);

extern void SsSeqPlayPtoP(short, short, unsigned char *, unsigned char *, char, short);

extern void SsSeqPause(short);

extern void SsSeqReplay(short);

extern int SsSeqSkip(short, short, char, short);

extern void SsSeqStop(short);

/**
 * Sets the main volume of music with seq_access_num to values specified for voll and volr (from 0 to 127).
 * @param seq_access_num SEQ access number
 * @param voll Left channel’s main volume value
 * @param volr Right channel’s main volume value
 */
extern void SsSeqSetVol(short seq_access_num, short voll, short volr);

extern void SsSeqSetNext(short, short);

extern void SsSeqSetCrescendo(short, short, long);

/**
 * Lowers main volume of SEQ data with seq_access_num by the vol value in v_time. If each voice volume is
 * the minimum value (0), or if vol is a negative number, there is no effect. It is recommended that v_time be
 * set to an integer multiple of vol.
 * @param short seq_access_num SEQ access number
 * @param short vol Volume value (0-127)
 * @param long v_time Time (in ticks)
 */
extern void SsSeqSetDecrescendo(short seq_access_num, short vol, long v_time);

extern void SsSeqSetAccelerando(short, long, long);

extern void SsSeqSetRitardando(short, long, long);

extern void SsSeqClose(short);

/**
 * Open SEP data.<br>
 * Analyzes the SEP data located in the main memory, and returns a SEP access number. Up to 32 pieces of
 * SEP data can be opened simultaneously when combined with the number of open SEQ data.
 * @param addr Pointer to starting address of SEP data within the main memory
 * @param vab_id VAB id
 * @param seq_num Number of SEQs contained in SEP
 * @return SEP access number: an internal SEP data management table number that has the same characteristics as
 * the SEQ access number.
 */
extern short SsSepOpen(unsigned long *addr, short vab_id, short seq_num);

/**
 * Begins to read (play) SEQ data specified by the SEP data seq_num specified by seq_access_num, if
 * play_mode = SSPLAY_PLAY. If play_mode = SSPLAY_PAUSE, makes a pause state. For infinite play
 * repetition, specify SSPLAY_INFINITY in l_count.<br>
 * Example:
 * <ul>
 * <li>sep1 = SsSepOpen (addr, vab_id, 4);  Open SEP data containing four pieces of SEQ data</li>
 * SsSepPlay (sep1, 2, SSPLAY_PLAY, 2); Immediately play 3rd SEQ data of opened SEP data twice
 * </ul>
 * @param sep_access_num SEP access number
 * @param seq_num SEP data SEQ number
 * @param play_mode Play mode
 * @param l_count Song repetition count
 */
extern void SsSepPlay(short sep_access_num, short seq_num, char play_mode, short l_count);

extern void SsSepPause(short, short);

extern void SsSepReplay(short, short);

extern void SsSepStop(short, short);

/**
 * Sets the L and R channels for the main volume of the seq_num-th SEQ data of SEP data with
 * sep_access_num to specified values (between 0 and 127).
 * @param sep_access_num SEP access number
 * @param seq_num SEQ number inside SEP data
 * @param voll L channel main volume value
 * @param volr R channel main volume value
 */
extern void SsSepSetVol(short sep_access_num, short seq_num, short voll, short volr);

extern void SsSepSetCrescendo(short, short, short, long);

extern void SsSepSetDecrescendo(short, short, short, long);

extern void SsSepSetAccelerando(short, short, long, long);

extern void SsSepSetRitardando(short, short, long, long);

extern void SsSepClose(short);

extern long SsVoKeyOn(long, long, unsigned short, unsigned short);

extern long SsVoKeyOff(long, long);

/**
 * Set main volume value.
 * Sets the master volume for the sound system to voll and volr (values range from 0 to 127).
 * You must call this function before playing sequence (SEQ, SEP) data.
 * @param voll L channel volume
 * @param volr R channel volume
 */
extern void SsSetMVol(short voll, short volr);

extern void SsGetMVol(SndVolume *);

extern void SsSetRVol(short, short);

extern void SsGetRVol(SndVolume *);

extern void SsSetMute(char);

extern char SsGetMute(void);

extern void SsSetSerialAttr(char, char, char);

extern char SsGetSerialAttr(char, char);

extern void SsSetSerialVol(char, short, short);

extern void SsGetSerialVol(char, SndVolume *);

extern void SsSetNck(short);

extern short SsGetNck(void);

extern void SsSetNoiseOn(short, short);

extern void SsSetNoiseOff(void);

extern void SsSetMono(void);

extern void SsSetStereo(void);

extern void SsSetTempo(short, short, short);

extern void SsSetLoop(short, short, short);

extern short SsIsEos(short, short);

extern void SsPlayBack(short, short, short);

extern void SsSetMarkCallback(short, short, SsMarkCallbackProc);

extern char SsSetReservedVoice(char);

extern short SsUtKeyOn(short, short, short, short, short, short, short);

extern short SsUtKeyOff(short, short, short, short, short);

extern short SsUtKeyOnV(short voice, short vabId, short prog, short tone,
                        short note, short fine, short voll, short volr);

extern short SsUtKeyOffV(short voice);

extern short SsUtPitchBend(short, short, short, short, short);

extern short SsUtChangePitch(short, short, short, short, short,
                             short, short);

extern short SsUtChangeADSR(short, short, short, short,
                            unsigned short, unsigned short);

extern short SsUtSetVabHdr(short, VabHdr *);

extern short SsUtGetVabHdr(short, VabHdr *);

extern short SsUtSetProgAtr(short, short, ProgAtr *);

extern short SsUtGetProgAtr(short, short, ProgAtr *);

extern short SsUtSetVagAtr(short, short, short, VagAtr *);

extern short SsUtGetVagAtr(short, short, short, VagAtr *);

extern short SsUtSetDetVVol(short, short, short);

extern short SsUtGetDetVVol(short, short *, short *);

extern short SsUtSetVVol(short, short, short);

extern short SsUtGetVVol(short, short *, short *);

extern short SsUtAutoVol(short, short, short, short);

extern short SsUtAutoPan(short, short, short, short);

extern void SsUtReverbOn(void);

extern void SsUtReverbOff(void);

extern short SsUtSetReverbType(short);

extern short SsUtGetReverbType(void);

extern void SsUtSetReverbDepth(short, short);

extern void SsUtSetReverbFeedback(short);

extern void SsUtSetReverbDelay(short);

extern void SsUtAllKeyOff(short);

extern void SsSetAutoKeyOffMode(short mode);

extern void SsUtFlush(void);

extern short SsVabFakeHead(unsigned char *, short, unsigned long);

extern short SsVabFakeBody(short);

extern unsigned long SsUtGetVBaddrInSB(short);

extern long SsUtGetVagAddr(short vabId, short vagId);

extern unsigned long SsUtGetVagAddrFromTone(short vabId, short progId,
                                            short toneId);

extern void SsSetNext(short, short, short, short);

extern void SsSeqGetVol(short, short, short *, short *);

extern void SsChannelMute(short, short, long);

extern short SsSeqOpenJ(unsigned long *, short);

extern short SsSepOpenJ(unsigned long *, short, short);

extern unsigned char *SsGetCurrentPoint(short, short);

extern int SsSetCurrentPoint(short, short, unsigned char *);

extern long SsGetChannelMute(short, short);

extern void SsSetVoiceMask(unsigned long);

extern unsigned long SsGetVoiceMask(void);

extern void SsQueueRegisters(long, SndRegisterAttr *);

extern void SsQueueKeyOn(long);

extern void SsQueueReverb(long, long);

extern short SsGetActualProgFromProg(short, short);

extern void SsSetVoiceSettings(long, SndVoiceStats *);

extern unsigned short SsPitchFromNote(short, short, unsigned char,
                                      unsigned char);

extern short SsVoiceCheck(long, long, short);

extern char SsBlockVoiceAllocation(void);

extern char SsUnBlockVoiceAllocation(void);

extern long SsAllocateVoices(unsigned char, unsigned char);


#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

/*
 * for function table
 */

#define CC_NUMBER     0
#define CC_BANKCHANGE 1
#define CC_DATAENTRY  2
#define CC_MAINVOL    3
#define CC_PANPOT     4
#define CC_EXPRESSION 5
#define CC_DAMPER     6
#define CC_NRPN1      7
#define CC_NRPN2      8
#define CC_RPN1       9
#define CC_RPN2       10
#define CC_EXTERNAL   11
#define CC_RESETALL   12

#define DE_PRIORITY    0
#define DE_MODE    1
#define DE_LIMITL    2
#define DE_LIMITH    3
#define DE_ADSR_AR_L    4
#define DE_ADSR_AR_E    5
#define DE_ADSR_DR    6
#define DE_ADSR_SL    7
#define DE_ADSR_SR_L    8
#define DE_ADSR_SR_E    9
#define DE_ADSR_RR_L    10
#define DE_ADSR_RR_E    11
#define DE_ADSR_SR    12
#define DE_VIB_TIME    13
#define DE_PORTA_DEPTH    14
#define DE_REV_TYPE    15
#define DE_REV_DEPTH    16
#define DE_ECHO_FB    17
#define DE_ECHO_DELAY    18
#define DE_DELAY    19

typedef struct {
    void (*noteon)();

    void (*programchange)();

    void (*pitchbend)();

    void (*metaevent)();

    void (*control[13])();

    void (*ccentry[20])();
} _SsFCALL;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void _SsNoteOn(short, short, unsigned char, unsigned char);

extern void _SsSetProgramChange(short, short, unsigned char);

extern void _SsGetMetaEvent(short, short, unsigned char);

extern void _SsSetPitchBend(short, short);

extern void _SsSetControlChange(short, short, unsigned char);

extern void _SsContBankChange(short, short);

extern void _SsContDataEntry(short, short, unsigned char);

extern void _SsContMainVol(short, short, unsigned char);

extern void _SsContPanpot(short, short, unsigned char);

extern void _SsContExpression(short, short, unsigned char);

extern void _SsContDamper(short, short, unsigned char);

extern void _SsContExternal(short, short, unsigned char);

extern void _SsContNrpn1(short, short, unsigned char);

extern void _SsContNrpn2(short, short, unsigned char);

extern void _SsContRpn1(short, short, unsigned char);

extern void _SsContRpn2(short, short, unsigned char);

extern void _SsContResetAll(short, short);

extern void _SsSetNrpnVabAttr0(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr1(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr2(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr3(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr4(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr5(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr6(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr7(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr8(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr9(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr10(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr11(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr12(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr13(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr14(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr15(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr16(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr17(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr18(short, short, short, VagAtr, short, unsigned char);

extern void _SsSetNrpnVabAttr19(short, short, short, VagAtr, short, unsigned char);

extern void dmy_nothing1(short, short, unsigned char, unsigned char);

extern void dmy_SsNoteOn(short, short, unsigned char, unsigned char);

extern void dmy_SsSetProgramChange(short, short, unsigned char);

extern void dmy_SsGetMetaEvent(short, short, unsigned char);

extern void dmy_SsSetPitchBend(short, short);

extern void dmy_SsSetControlChange(short, short, unsigned char);

extern void dmy_SsContBankChange(short, short);

extern void dmy_SsContDataEntry(short, short, unsigned char);

extern void dmy_SsContMainVol(short, short, unsigned char);

extern void dmy_SsContPanpot(short, short, unsigned char);

extern void dmy_SsContExpression(short, short, unsigned char);

extern void dmy_SsContDamper(short, short, unsigned char);

extern void dmy_SsContExternal(short, short, unsigned char);

extern void dmy_SsContNrpn1(short, short, unsigned char);

extern void dmy_SsContNrpn2(short, short, unsigned char);

extern void dmy_SsContRpn1(short, short, unsigned char);

extern void dmy_SsContRpn2(short, short, unsigned char);

extern void dmy_SsContResetAll(short, short);

extern void dmy_SsSetNrpnVabAttr0(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr1(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr2(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr3(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr4(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr5(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr6(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr7(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr8(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr9(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr10(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr11(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr12(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr13(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr14(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr15(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr16(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr17(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr18(short, short, short, VagAtr, short, unsigned char);

extern void dmy_SsSetNrpnVabAttr19(short, short, short, VagAtr, short, unsigned char);

extern _SsFCALL SsFCALL;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif


#if 0
jt_SsInit ()
{
    SsFCALL.noteon                   = (void (*)())_SsNoteOn;
    SsFCALL.programchange            = (void (*)())_SsSetProgramChange;
    SsFCALL.metaevent                = (void (*)())_SsGetMetaEvent;
    SsFCALL.pitchbend                = (void (*)())_SsSetPitchBend;
        SsFCALL.control [CC_NUMBER]      = (void (*)())_SsSetControlChange;
    SsFCALL.control [CC_BANKCHANGE]  = (void (*)())_SsContBankChange;
    SsFCALL.control [CC_MAINVOL]     = (void (*)())_SsContMainVol;
    SsFCALL.control [CC_PANPOT]      = (void (*)())_SsContPanpot;
    SsFCALL.control [CC_EXPRESSION]  = (void (*)())_SsContExpression;
    SsFCALL.control [CC_DAMPER]      = (void (*)())_SsContDamper;
    SsFCALL.control [CC_NRPN1]       = (void (*)())_SsContNrpn1;
    SsFCALL.control [CC_NRPN2]       = (void (*)())_SsContNrpn2;
    SsFCALL.control [CC_RPN1]        = (void (*)())_SsContRpn1;
    SsFCALL.control [CC_RPN2]        = (void (*)())_SsContRpn2;
    SsFCALL.control [CC_EXTERNAL]    = (void (*)())_SsContExternal;
    SsFCALL.control [CC_RESETALL]    = (void (*)())_SsContResetAll;
    SsFCALL.control [CC_DATAENTRY]   = (void (*)())_SsContDataEntry;
    SsFCALL.ccentry [DE_PRIORITY]	 = (void (*)())_SsSetNrpnVabAttr0;
    SsFCALL.ccentry [DE_MODE]        = (void (*)())_SsSetNrpnVabAttr1;
    SsFCALL.ccentry [DE_LIMITL]      = (void (*)())_SsSetNrpnVabAttr2;
    SsFCALL.ccentry [DE_LIMITH]      = (void (*)())_SsSetNrpnVabAttr3;
    SsFCALL.ccentry [DE_ADSR_AR_L]   = (void (*)())_SsSetNrpnVabAttr4;
    SsFCALL.ccentry [DE_ADSR_AR_E]   = (void (*)())_SsSetNrpnVabAttr5;
    SsFCALL.ccentry [DE_ADSR_DR]     = (void (*)())_SsSetNrpnVabAttr6;
    SsFCALL.ccentry [DE_ADSR_SL]     = (void (*)())_SsSetNrpnVabAttr7;
    SsFCALL.ccentry [DE_ADSR_SR_L]   = (void (*)())_SsSetNrpnVabAttr8;
    SsFCALL.ccentry [DE_ADSR_SR_E]   = (void (*)())_SsSetNrpnVabAttr9;
    SsFCALL.ccentry [DE_ADSR_RR_L]   = (void (*)())_SsSetNrpnVabAttr10;
    SsFCALL.ccentry [DE_ADSR_RR_E]   = (void (*)())_SsSetNrpnVabAttr11;
    SsFCALL.ccentry [DE_ADSR_SR]     = (void (*)())_SsSetNrpnVabAttr12;
    SsFCALL.ccentry [DE_VIB_TIME]    = (void (*)())_SsSetNrpnVabAttr13;
    SsFCALL.ccentry [DE_PORTA_DEPTH] = (void (*)())_SsSetNrpnVabAttr14;
    SsFCALL.ccentry [DE_REV_TYPE]    = (void (*)())_SsSetNrpnVabAttr15;
    SsFCALL.ccentry [DE_REV_DEPTH]   = (void (*)())_SsSetNrpnVabAttr16;
    SsFCALL.ccentry [DE_ECHO_FB]     = (void (*)())_SsSetNrpnVabAttr17;
    SsFCALL.ccentry [DE_ECHO_DELAY]  = (void (*)())_SsSetNrpnVabAttr18;
    SsFCALL.ccentry [DE_DELAY]       = (void (*)())_SsSetNrpnVabAttr19;
}
#endif

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
#endif /* _LIBSND_H_ */
/* DON'T ADD STUFF AFTER THIS */
