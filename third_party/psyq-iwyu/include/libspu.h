#ifndef _LIBSPU_H_
#define _LIBSPU_H_

/*****************************************************************
 * -*- c -*-
 * $RCSfile: libspu.h,v $
 *
 * Copyright (c) 1993, 1994, 1995, 1996 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * This file is part of ``PlayStation(R)'' Programmer Tool /
 * Runtime Library.
 *
 * R & D Division, Sony Computer Entertainment Inc.
 *
 * $Id: libspu.h,v 1.80 1997/06/24 11:14:04 kaol Exp $
 *
 *****************************************************************/
/*
 * $PSLibId: Run-time Library Release 4.7$
 */

/* ----------------------------------------------------------------
 *	CONSTANT 
 * ---------------------------------------------------------------- */

#define SPU_SUCCESS    0
#define SPU_INVALID_ARGS (-3)
#define SPU_DIAG    (-2)
#define SPU_CHECK    (-1)
#define SPU_OFF        0
#define SPU_ON        1
#define SPU_CLEAR    2
#define SPU_RESET    3
#define SPU_DONT_CARE    4
#define SPU_ALL        0
#define SPU_CDONLY    5
#define SPU_VOICEONLY    6
#define SPU_CONT    7
#define SPU_BIT        8
#define SPU_NULL    0
/* Macros below will be obsoleted. */
#define SpuDiag        SPU_DIAG
#define SpuCheck    SPU_CHECK
#define SpuOff        SPU_OFF
#define SpuOn        SPU_ON
#define SpuClear    SPU_CLEAR
#define SpuReset    SPU_RESET
#define SpuDontCare    SPU_DONT_CARE
#define SpuALL        SPU_ALL
#define SpuCDOnly    SPU_CDONLY
#define SpuVoiceOnly    SPU_VOICEONLY
#define SpuCont        SPU_CONT
#define SpuNull        SPU_NULL

#define SPU_OFF_ENV_ON    2
#define SPU_ON_ENV_OFF    3
/* Macros below will be obsoleted. */
#define SpuOffEnvOn    SPU_OFF_ENV_ON
#define SpuOnEnvOff    SPU_ON_ENV_OFF

#define SPU_ERROR    (-1)
/* Macros below will be obsoleted. */
#define SpuError    SPU_ERROR

#define SPU_TRANSFER_BY_DMA 0L
#define SPU_TRANSFER_BY_IO 1L
/* Macros below will be obsoleted. */
#define SpuTransferByDMA SPU_TRANSFER_BY_DMA
#define SpuTransferByIO  SPU_TRANSFER_BY_IO
#define SpuTransByDMA SpuTransferByDMA
#define SpuTransByIO SpuTransferByIO

#define SPU_TRANSFER_WAIT 1
#define SPU_TRANSFER_PEEK 0
#define SPU_TRANSFER_GLANCE SPU_TRANSFER_PEEK

/*
 * Voice designate
 */

#ifndef __SPU_VOICE
#define __SPU_VOICE

#define SPU_00CH (0x1L<< 0)
#define SPU_01CH (0x1L<< 1)
#define SPU_02CH (0x1L<< 2)
#define SPU_03CH (0x1L<< 3)
#define SPU_04CH (0x1L<< 4)
#define SPU_05CH (0x1L<< 5)
#define SPU_06CH (0x1L<< 6)
#define SPU_07CH (0x1L<< 7)
#define SPU_08CH (0x1L<< 8)
#define SPU_09CH (0x1L<< 9)
#define SPU_10CH (0x1L<<10)
#define SPU_11CH (0x1L<<11)
#define SPU_12CH (0x1L<<12)
#define SPU_13CH (0x1L<<13)
#define SPU_14CH (0x1L<<14)
#define SPU_15CH (0x1L<<15)
#define SPU_16CH (0x1L<<16)
#define SPU_17CH (0x1L<<17)
#define SPU_18CH (0x1L<<18)
#define SPU_19CH (0x1L<<19)

#define SPU_20CH (0x1L<<20)
#define SPU_21CH (0x1L<<21)
#define SPU_22CH (0x1L<<22)
#define SPU_23CH (0x1L<<23)

#define SPU_0CH SPU_00CH
#define SPU_1CH SPU_01CH
#define SPU_2CH SPU_02CH
#define SPU_3CH SPU_03CH
#define SPU_4CH SPU_04CH
#define SPU_5CH SPU_05CH
#define SPU_6CH SPU_06CH
#define SPU_7CH SPU_07CH
#define SPU_8CH SPU_08CH
#define SPU_9CH SPU_09CH

#define SPU_ALLCH (SPU_00CH | SPU_01CH | SPU_02CH | SPU_03CH | SPU_04CH | \
           SPU_05CH | SPU_06CH | SPU_07CH | SPU_08CH | SPU_09CH | \
           SPU_10CH | SPU_11CH | SPU_12CH | SPU_13CH | SPU_14CH | \
           SPU_15CH | SPU_16CH | SPU_17CH | SPU_18CH | SPU_19CH | \
           SPU_20CH | SPU_21CH | SPU_22CH | SPU_23CH)

#define SPU_KEYCH(x) (0x1L<<(x))
#define SPU_VOICECH(x) SPU_KEYCH(x)

#endif /* __SPU_VOICE */

/* for Voice setting */

#define    SPU_VOICE_VOLL        (0x01 <<  0) /* volume (left) */
#define    SPU_VOICE_VOLR        (0x01 <<  1) /* volume (right) */
#define    SPU_VOICE_VOLMODEL    (0x01 <<  2) /* volume mode (left) */
#define    SPU_VOICE_VOLMODER    (0x01 <<  3) /* volume mode (right) */
#define    SPU_VOICE_PITCH        (0x01 <<  4) /* tone (pitch setting) */
#define    SPU_VOICE_NOTE        (0x01 <<  5) /* tone (note setting)  */
#define    SPU_VOICE_SAMPLE_NOTE    (0x01 <<  6) /* waveform data sample note */
#define    SPU_VOICE_WDSA        (0x01 <<  7) /* waveform data start address */
#define    SPU_VOICE_ADSR_AMODE    (0x01 <<  8) /* ADSR Attack rate mode */
#define    SPU_VOICE_ADSR_SMODE    (0x01 <<  9) /* ADSR Sustain rate mode */
#define    SPU_VOICE_ADSR_RMODE    (0x01 << 10) /* ADSR Release rate mode */
#define    SPU_VOICE_ADSR_AR    (0x01 << 11) /* ADSR Attack rate         */
#define    SPU_VOICE_ADSR_DR    (0x01 << 12) /* ADSR Decay rate          */
#define    SPU_VOICE_ADSR_SR    (0x01 << 13) /* ADSR Sustain rate        */
#define    SPU_VOICE_ADSR_RR    (0x01 << 14) /* ADSR Release rate        */
#define    SPU_VOICE_ADSR_SL    (0x01 << 15) /* ADSR Sustain level       */
#define    SPU_VOICE_LSAX        (0x01 << 16) /* start address for loop */
#define    SPU_VOICE_ADSR_ADSR1    (0x01 << 17) /* ADSR adsr1 for `VagAtr'  */
#define    SPU_VOICE_ADSR_ADSR2    (0x01 << 18) /* ADSR adsr2 for `VagAtr'  */

#define    SPU_VOICE_DIRECT    0
#define    SPU_VOICE_LINEARIncN    1
#define    SPU_VOICE_LINEARIncR    2
#define    SPU_VOICE_LINEARDecN    3
#define    SPU_VOICE_LINEARDecR    4
#define    SPU_VOICE_EXPIncN    5
#define    SPU_VOICE_EXPIncR    6
#define    SPU_VOICE_EXPDec    7
#define    SPU_VOICE_EXPDecN    SPU_VOICE_EXPDec
#define    SPU_VOICE_EXPDecR    SPU_VOICE_EXPDec

#define    SPU_DECODED_FIRSTHALF    0
#define    SPU_DECODED_SECONDHALF    1
#define    SPU_DECODE_FIRSTHALF    SPU_DECODED_FIRSTHALF
#define    SPU_DECODE_SECONDHALF    SPU_DECODED_SECONDHALF


#define    SPU_COMMON_MVOLL        (0x01 <<  0) /* master volume (left) */
#define    SPU_COMMON_MVOLR        (0x01 <<  1) /* master volume (right) */
#define    SPU_COMMON_MVOLMODEL        (0x01 <<  2) /* master volume mode (left) */
#define    SPU_COMMON_MVOLMODER        (0x01 <<  3) /* master volume mode (right) */
#define    SPU_COMMON_RVOLL        (0x01 <<  4) /* reverb volume (left) */
#define    SPU_COMMON_RVOLR        (0x01 <<  5) /* reverb volume (right) */
#define    SPU_COMMON_CDVOLL        (0x01 <<  6) /* CD input volume (left) */
#define    SPU_COMMON_CDVOLR        (0x01 <<  7) /* CD input volume (right) */
#define    SPU_COMMON_CDREV        (0x01 <<  8) /* CD input reverb on/off */
#define    SPU_COMMON_CDMIX        (0x01 <<  9) /* CD input on/off */
#define    SPU_COMMON_EXTVOLL        (0x01 << 10) /* external digital input volume (left) */
#define    SPU_COMMON_EXTVOLR        (0x01 << 11) /* external digital input volume (right) */
#define    SPU_COMMON_EXTREV        (0x01 << 12) /* external digital input reverb on/off */
#define    SPU_COMMON_EXTMIX        (0x01 << 13) /* external digital input on/off */

/* for Reverb setting */

#define    SPU_REV_MODE        (0x01 <<  0) /* mode setting */
#define    SPU_REV_DEPTHL        (0x01 <<  1) /* reverb depth (left) */
#define    SPU_REV_DEPTHR        (0x01 <<  2) /* reverb depth (right) */
#define    SPU_REV_DELAYTIME    (0x01 <<  3) /* Delay Time  (ECHO, DELAY only) */
#define    SPU_REV_FEEDBACK    (0x01 <<  4) /* Feedback    (ECHO only) */

#define SPU_REV_MODE_CHECK    (-1)
#define SPU_REV_MODE_OFF    0
#define SPU_REV_MODE_ROOM    1
#define SPU_REV_MODE_STUDIO_A    2
#define SPU_REV_MODE_STUDIO_B    3
#define SPU_REV_MODE_STUDIO_C    4
#define SPU_REV_MODE_HALL    5
#define SPU_REV_MODE_SPACE    6
#define SPU_REV_MODE_ECHO    7
#define SPU_REV_MODE_DELAY    8
#define SPU_REV_MODE_PIPE    9
#define SPU_REV_MODE_MAX    10

#define SPU_REV_MODE_CLEAR_WA    0x100

/* ----------------------------------------------------------------
 *	Event flushing
 * ---------------------------------------------------------------- */

#define SPU_EVENT_KEY      (0x01 << 0)
#define SPU_EVENT_PITCHLFO (0x01 << 1)
#define SPU_EVENT_NOISE    (0x01 << 2)
#define SPU_EVENT_REVERB   (0x01 << 3)

#define SPU_EVENT_ALL 0

/* ----------------------------------------------------------------
 *	Structure
 * ---------------------------------------------------------------- */

typedef struct {
    short left;           /* Lch */
    short right;       /* Rch */
} SpuVolume;

typedef struct {
    unsigned long voice;        /* set voice:
                            SpuSetVoiceAttr: each voice is a bit array
                            SpuGetVoiceAttr: voice is a bit value */
    unsigned long mask;        /* settings attribute bit (invalid with Get) */
    SpuVolume volume;        /* volume                         */
    SpuVolume volmode;    /* volume mode                    */
    SpuVolume volumex;    /* current volume (invalid with Set) */
    unsigned short pitch;        /* tone (pitch setting) */
    unsigned short note;        /* tone (note setting) */
    unsigned short sample_note;    /* tone (note setting) */
    short envx;        /* current envelope value (invalid with Set) */
    unsigned long addr;        /* waveform data start address */
    unsigned long loop_addr;    /* loop start address */
    long a_mode;        /* Attack rate mode			*/
    long s_mode;        /* Sustain rate mode			*/
    long r_mode;        /* Release rate mode			*/
    unsigned short ar;        /* Attack rate				*/
    unsigned short dr;        /* Decay rate				*/
    unsigned short sr;        /* Sustain rate				*/
    unsigned short rr;        /* Release rate				*/
    unsigned short sl;        /* Sustain level			*/
    unsigned short adsr1;        /* adsr1 for `VagAtr' */
    unsigned short adsr2;        /* adsr2 for `VagAtr' */
} SpuVoiceAttr;

typedef struct {
    short voiceNum;        /* voice number */
    short pad;            /* padding */
    SpuVoiceAttr attr;        /* voice attribute */
} SpuLVoiceAttr;

typedef struct {
    unsigned long mask;      /* settings mask */

    long mode;      /* reverb mode */
    SpuVolume depth;      /* reverb depth */
    long delay;      /* Delay Time  (ECHO, DELAY only)   */
    long feedback; /* Feedback    (ECHO only)          */
} SpuReverbAttr;

#define SPU_DECODEDDATA_SIZE 0x200
#define SPU_DECODEDATA_SIZE SPU_DECODEDDATA_SIZE
typedef struct {
    short cd_left[SPU_DECODEDDATA_SIZE];
    short cd_right[SPU_DECODEDDATA_SIZE];
    short voice1[SPU_DECODEDDATA_SIZE];
    short voice3[SPU_DECODEDDATA_SIZE];
} SpuDecodedData;
typedef SpuDecodedData SpuDecodeData;

typedef struct {
    SpuVolume volume;          /* volume       */
    long reverb;          /* reverb on/off */
    long mix;          /* mixing on/off */
} SpuExtAttr;

/**
 * Used when setting/checking common attributes. The members needed for setting are set as bit values in
 * mask.
 */
typedef struct {
    /* settings mask */
    unsigned long mask;
    /* master volume */
    SpuVolume mvol;
    /* master volume mode */
    SpuVolume mvolmode;
    /* current master volume */
    SpuVolume mvolx;
    /* CD input attributes */
    SpuExtAttr cd;
    /* external digital input attributes */
    SpuExtAttr ext;
} SpuCommonAttr;

#ifndef __SPU_IRQCALLBACK_PROC
#define __SPU_IRQCALLBACK_PROC

typedef void (*SpuIRQCallbackProc)(void);

#endif /* __SPU_IRQCALLBACK_PROC */
#ifndef __SPU_TRANSFERCALLBACK_PROC
#define __SPU_TRANSFERCALLBACK_PROC

typedef void (*SpuTransferCallbackProc)(void);

#endif /* __SPU_TRANSFERCALLBACK_PROC */

/* ----------------------------------------------------------------
 *	for SPU Malloc (used in SpuInitMalloc())
 * ---------------------------------------------------------------- */

#define SPU_MALLOC_RECSIZ 8

/* ----------------------------------------------------------------
 *	User specifiable global environment
 * ---------------------------------------------------------------- */

typedef struct {
    unsigned long mask;
    unsigned long queueing;
} SpuEnv;

#define SPU_ENV_EVENT_QUEUEING            (0x01 << 0)

/* ----------------------------------------------------------------
 *	prototype declaration
 * ---------------------------------------------------------------- */

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
* Initializes SPU. Called only once within the program. After initialization, the state of the SPU is:
* <ul>
* <li>Master volume is 0 for both L/R</li>
* <li>Reverb is off; reverb work area is not reserved</li>
* <li>Reverb depth and volume are 0 for both L/R</li>
* <li>Sound buffer transfer mode is DMA transfer</li>
* <li>For all voices: Key off</li>
* <li>For all voices: Pitch LFO, noise, reverb functions not set</li>
* <li>CD input volume is 0 for both L/R</li>
* <li>External digital input volume is 0 for both L/R</li>
* <li>DMA transfer initialization set</li>
</ul>
The status of the sound buffer is indeterminate after initialization.
*/
extern void SpuInit(void);

extern void SpuInitHot(void);

extern void SpuStart(void);

extern void SpuQuit(void);

extern long SpuSetMute(long on_off);

extern long SpuGetMute(void);

extern void SpuSetEnv(SpuEnv *env);

extern long SpuSetNoiseClock(long n_clock);

extern long SpuGetNoiseClock(void);

extern unsigned long SpuSetNoiseVoice(long on_off, unsigned long voice_bit);

extern unsigned long SpuGetNoiseVoice(void);

extern long SpuSetReverb(long on_off);

extern long SpuGetReverb(void);

extern long SpuSetReverbModeParam(SpuReverbAttr *attr);

extern void SpuGetReverbModeParam(SpuReverbAttr *attr);

extern long SpuSetReverbDepth(SpuReverbAttr *attr);

extern long SpuReserveReverbWorkArea(long on_off);

extern long SpuIsReverbWorkAreaReserved(long on_off);

extern unsigned long SpuSetReverbVoice(long on_off, unsigned long voice_bit);

extern unsigned long SpuGetReverbVoice(void);

extern long SpuClearReverbWorkArea(long mode);

/**
 * Transfers size bytes of data from main memory addr to the sound buffer
 * The main memory address addr storing the transfer data must be a global variable or an address in a heap
 * area that was allocated by a function such as malloc(). It can’t address a variable on the stack declared in
 * a function.
 * SpuWrite() does not perform sound buffer memory management, so real waveform data cannot be used if
 * the user does not transfer to addresses which avoid the following areas.
 * <ul>
 * <li>SPU decoded data transfer area: 0x0000-0xfff</li>
 * <li>System reserved area: 0x1000-0x100f</li>
 * <li>Addresses after the reverb work area offset (start) address</li>
 * </ul>
 * After calling, either call SpuIsTransferCompleted() to confirm transfer completion or set the DMA transfer
 * completion Callback function in advance using SpuSetTransferCallback().
 * Due to the limitations of the DMA transfer hardware, transfers are always performed in 64 byte units. When
 * specifying values which are not multiples of 64 as secondary arguments, since the portion of the value
 * which is a multiple of 64 is transferred, it’s possible to damage the data in the SPU memory.
 * @param addr Pointer to transfer data start address in main memory
 * @param size Transfer data size (in bytes)
 * @return Transferred data size. If size is larger than 512 KB, the actual transferred size is returned.
 * If the transfer mode is SPU_TRANSFER_BY_DMA and size is not a multiple of 64, the return value will be
 * incorrect.
 */
extern unsigned long SpuWrite(unsigned char *addr, unsigned long size);

extern unsigned long SpuWrite0(unsigned long size);

extern unsigned long SpuRead(unsigned char *addr, unsigned long size);

/**
 * Sets the mode for transferring data from main memory to the sound buffer. The mode values can be:
 * <ul>
 * <li>SPU_TRANSFER_BY_DMA: DMA transfer; can do other processing during transfer (default value).</li>
 * <li>SPU_TRANSFER_BY_IO: I/O transfer. Uses CPU; cannot do other processing during transfer.</li>
 * </ul>
 * <strong>Note:</strong> These specifications are valid only when transferring data from main memory to the sound buffer.
 * DMA transfer is always used when transferring data from the sound buffer to main memory.
 * When a transfer is done without first calling this function, the transfer mode is the previously set value.
 * @param mode
 * @return
 */
extern long SpuSetTransferMode(long mode);

#define SpuSetTransMode(mode) SpuSetTransferMode((mode))

extern long SpuGetTransferMode(void);

#define SpuGetTransMode() SpuGetTransferMode()

/**
 * Sets a starting address in the sound buffer, specified in addr, for transferring data to and from main
 * memory. addr must be a byte value that is
 * <ul>
 * <li>Divisible by 8. If it is not divisible by 8, it is increased to the next value divisible by 8.</li>
 * <li>Between 0x1010 - 0x7ffff for transfers to the sound buffer.</li>
 * <li>Between 0 - 0x0fff for transfers from the sound buffer. See SpuReadDecodedData().</li>
 * </ul>
 * Note: 0x1000 - 0x100f is reserved for the system.
 * @param addr Sound buffer transfer destination/transfer source start address
 * @return Start address value. If the address specified is smaller than 0x1010 or greater than 512 KB, 0 is returned.
 */
extern unsigned long SpuSetTransferStartAddr(unsigned long addr);

#define SpuSetTransStartAddr(addr) SpuSetTransferStartAddr((addr))

extern unsigned long SpuGetTransferStartAddr(void);

#define SpuGetTransStartAddr() SpuGetTransferStartAddr()

extern unsigned long SpuWritePartly(unsigned char *addr, unsigned long size);

/**
 * Checks whether transfer is completed or waits for completion, depending on value of flag:
 * <ul>
 * <li>SPU_TRANSFER_WAIT Wait until transfer ends</li>
 * <li>SPU_TRANSFER_PEEK Check whether transfer has ended and return result</li>
 * <li>SPU_TRANSFER_GLANCE Same as SPU_TRANSFER_PEEK</li>
 * </ul
 * This function doesn’t work (and returns 1) when a callback function is set with SpuSetTransferCallback()
 * and started at the completion of DMA transfer.
 * @param flag Check flag
 * @return 1 : transfer completed; 0 : transfer not completed.
 * If flag = SPU_TRANSFER_WAIT, waits until transfer ends and always returns 1. If transfer mode is
 * SPU_TRANSFER_BY_IO, 1 is returned immediately.
 */
extern long SpuIsTransferCompleted(long flag);

extern SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func);

extern long SpuReadDecodedData(SpuDecodedData *d_data, long flag);

#define SpuReadDecodeData(d_data, flag) SpuReadDecodedData((d_data), (flag))

extern long SpuSetIRQ(long on_off);

extern long SpuGetIRQ(void);

extern unsigned long SpuSetIRQAddr(unsigned long);

extern unsigned long SpuGetIRQAddr(void);

extern SpuIRQCallbackProc SpuSetIRQCallback(SpuIRQCallbackProc);

extern void SpuSetVoiceAttr(SpuVoiceAttr *arg);

extern void SpuGetVoiceAttr(SpuVoiceAttr *arg);

extern void SpuSetKey(long on_off, unsigned long voice_bit);

extern void SpuSetKeyOnWithAttr(SpuVoiceAttr *attr);

/**
 * Checks key on/key off and envelope status for a voice specified in voice_bit with one of the values
 * SPU_0CH … SPU_23CH. (If multiple bits are set, the smallest voice number set is selected.)
 * @param voice_bit Checked voice
 * @return If successful, the current key on/key off status and envelope status of the specified voice are returned. (See
 * the table below.) If the specified voice is incorrect, -1 is returned.
 */
extern long SpuGetKeyStatus(unsigned long voice_bit);

extern void SpuGetAllKeysStatus(char *status);

extern unsigned long SpuFlush(unsigned long ev);

extern unsigned long SpuSetPitchLFOVoice(long on_off, unsigned long voice_bit);

extern unsigned long SpuGetPitchLFOVoice(void);

/**
 * Sets sound system attributes. Specify the attributes (members of attr) by ORing together the terms shown
 * below in attr.mask. If attr.mask is 0, all attributes are set.
 * The individual parameters that can be set are:<ul>
 * <li>Master Volume (attr.mvol ) and Master Volume Mode (attr.mvolmode)
 * Left and right are set independently. The volume range obtainable and the various modes are the
 * same as the settings for each voice; see Table 15–7 under SpuSetVoiceAttr().</li>
 * <li>CD Input Volume (attr.cd.volume)
 * Set independently for left and right in the range -0x8000 - 0x7fff. If volume is negative, the phase is
 * inverted.</li>
 * <li>CD Input Reverb On/Off (attr.cd.reverb)
 * SPU_ON = reverb on; SPU_OFF = reverb off</li>
 * <li>CD Input Mixing On/Off (attr.cd.mix)
 * SPU_ON = mixing on; SPU_OFF = mixing off. CD input is not output unless mixing is on.</li>
 * <li>External Digital Input Volume (attr.ext.volume)
 * Set independently for left and right in the range -0x8000 - 0x7fff. If volume is negative, the phase is
 * inverted.</li>
 * <li>External Digital Input Reverb On/Off (attr.ext.reverb)
 * SPU_ON = reverb on; SPU_OFF = reverb off.</li>
 * <li>External Digital Input Mixing On/Off (attr.ext.mix)
 * SPU_ON = mixing on; SPU_OFF = mixing off. External digital input is not output unless mixing is on.</li>
 * @param attr Pointer to attributes common to all voices
 */
extern void SpuSetCommonAttr(SpuCommonAttr *attr);

extern void SpuGetCommonAttr(SpuCommonAttr *attr);

/**
 * Initializes memory management for the sound buffer. You specify num as the maximum number of memory<br>
 * blocks that will be allocated, and an area pointed to by top to hold a memory management table, which
 * stores information about each block.<br>
 * The size of the area pointed to by top must be:<br>
 * (SPU_MALLOC_RECSIZ • (num + 1)) bytes
 * @param num Maximum number of times memory is allocated
 * @param top Pointer to the start address of the memory management table
 * @return The number of memory management blocks specified.
 */
extern long SpuInitMalloc(long num, char *top);

/**
 * Allocate an area of size bytes in the sound buffer.
 * Failure occurs if:
 * <ul>
 * <li>The requested size cannot be continuously allocated.</li>
 * <li>The only area that satisfies the requested size is part or all of a reverb work area already allocated by SpuReserveReverbWorkArea().</li>
 * </ul>
 * @param size Size of area allocated (in bytes)
 * @return The starting address of the allocated area, if successful; -1 if unsuccessful.
 */
extern long SpuMalloc(long size);

extern long SpuMallocWithStartAddr(unsigned long addr, long size);

extern void SpuFree(unsigned long addr);

extern long SpuRGetAllKeysStatus(long min_, long max_, char *status);

extern long SpuRSetVoiceAttr(long min_, long max_, SpuVoiceAttr *arg);

extern void SpuNSetVoiceAttr(int vNum, SpuVoiceAttr *arg);

extern void SpuNGetVoiceAttr(int vNum, SpuVoiceAttr *arg);

extern void SpuLSetVoiceAttr(int num, SpuLVoiceAttr *argList);

extern void SpuSetVoiceVolume(int vNum, short volL, short volR);

extern void SpuSetVoiceVolumeAttr(int vNum, short volL, short volR,
                                  short volModeL, short volModeR);

extern void SpuSetVoicePitch(int vNum, unsigned short pitch);

extern void SpuSetVoiceNote(int vNum, unsigned short note);

extern void SpuSetVoiceSampleNote(int vNum, unsigned short sampleNote);

extern void SpuSetVoiceStartAddr(int vNum, unsigned long startAddr);

extern void SpuSetVoiceLoopStartAddr(int vNum, unsigned long lsa);

extern void SpuSetVoiceAR(int vNum, unsigned short AR);

extern void SpuSetVoiceDR(int vNum, unsigned short DR);

extern void SpuSetVoiceSR(int vNum, unsigned short SR);

extern void SpuSetVoiceRR(int vNum, unsigned short RR);

extern void SpuSetVoiceSL(int vNum, unsigned short SL);

extern void SpuSetVoiceARAttr(int vNum, unsigned short AR, long ARmode);

extern void SpuSetVoiceSRAttr(int vNum, unsigned short SR, long SRmode);

extern void SpuSetVoiceRRAttr(int vNum, unsigned short RR, long RRmode);

extern void SpuSetVoiceADSR(int vNum, unsigned short AR, unsigned short DR,
                            unsigned short SR, unsigned short RR,
                            unsigned short SL);

extern void SpuSetVoiceADSRAttr(int vNum,
                                unsigned short AR, unsigned short DR,
                                unsigned short SR, unsigned short RR,
                                unsigned short SL,
                                long ARmode, long SRmode, long RRmode);

extern void SpuGetVoiceVolume(int vNum, short *volL, short *volR);

extern void SpuGetVoiceVolumeAttr(int vNum, short *volL, short *volR,
                                  short *volModeL, short *volModeR);

extern void SpuGetVoiceVolumeX(int vNum, short *volXL, short *volXR);

extern void SpuGetVoicePitch(int vNum, unsigned short *pitch);

extern void SpuGetVoiceNote(int vNum, unsigned short *note);

extern void SpuGetVoiceSampleNote(int vNum, unsigned short *sampleNote);

extern void SpuGetVoiceEnvelope(int vNum, short *envx);

extern void SpuGetVoiceStartAddr(int vNum, unsigned long *startAddr);

extern void SpuGetVoiceLoopStartAddr(int vNum, unsigned long *loopStartAddr);

extern void SpuGetVoiceAR(int vNum, unsigned short *AR);

extern void SpuGetVoiceDR(int vNum, unsigned short *DR);

extern void SpuGetVoiceSR(int vNum, unsigned short *SR);

extern void SpuGetVoiceRR(int vNum, unsigned short *RR);

extern void SpuGetVoiceSL(int vNum, unsigned short *SL);

extern void SpuGetVoiceARAttr(int vNum, unsigned short *AR, long *ARmode);

extern void SpuGetVoiceSRAttr(int vNum, unsigned short *SR, long *SRmode);

extern void SpuGetVoiceRRAttr(int vNum, unsigned short *RR, long *RRmode);

extern void SpuGetVoiceADSR(int vNum,
                            unsigned short *AR, unsigned short *DR,
                            unsigned short *SR, unsigned short *RR,
                            unsigned short *SL);

extern void SpuGetVoiceADSRAttr(int vNum,
                                unsigned short *AR, unsigned short *DR,
                                unsigned short *SR, unsigned short *RR,
                                unsigned short *SL,
                                long *ARmode, long *SRmode, long *RRmode);

extern void SpuGetVoiceEnvelopeAttr(int vNum, long *keyStat, short *envx);

extern void SpuSetCommonMasterVolume(short mvol_left, short mvol_right);

extern void SpuSetCommonMasterVolumeAttr(short mvol_left, short mvol_right,
                                         short mvolmode_left,
                                         short mvolmode_right);

extern void SpuSetCommonCDMix(long cd_mix);

extern void SpuSetCommonCDVolume(short cd_left, short cd_right);

extern void SpuSetCommonCDReverb(long cd_reverb);

extern void SpuGetCommonMasterVolume(short *mvol_left, short *mvol_right);

extern void SpuGetCommonMasterVolumeX(short *mvolx_left, short *mvolx_right);

extern void SpuGetCommonMasterVolumeAttr(short *mvol_left, short *mvol_right,
                                         short *mvolmode_left,
                                         short *mvolmode_right);

extern void SpuGetCommonCDMix(long *cd_mix);

extern void SpuGetCommonCDVolume(short *cd_left, short *cd_right);

extern void SpuGetCommonCDReverb(long *cd_reverb);

extern long SpuSetReverbModeType(long mode);

extern void SpuSetReverbModeDepth(short depth_left, short depth_right);

extern void SpuSetReverbModeDelayTime(long delay);

extern void SpuSetReverbModeFeedback(long feedback);

extern void SpuGetReverbModeType(long *mode);

extern void SpuGetReverbModeDepth(short *depth_left, short *depth_right);

extern void SpuGetReverbModeDelayTime(long *delay);

extern void SpuGetReverbModeFeedback(long *feedback);

extern void SpuSetESA(long revAddr);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

/* ---------------------------------------------------------------- */

#define SPU_ST_NOT_AVAILABLE 0
#define SPU_ST_ACCEPT  1

#define SPU_ST_ERROR (-1)
#define SPU_ST_INVALID_ARGUMENT (-2)
#define SPU_ST_WRONG_STATUS (-3)

#define SPU_ST_STOP     2
#define SPU_ST_IDLE     3
#define SPU_ST_PREPARE  4
#define SPU_ST_START    5
#define SPU_ST_PLAY     6
#define SPU_ST_TRANSFER 7
#define SPU_ST_FINAL    8


/* VAG's header size */
#define SPU_ST_VAG_HEADER_SIZE 0x30

typedef struct {
    char status;        /* stream status */
    char pad1;            /* padding */
    char pad2;            /* padding */
    char pad3;            /* padding */
    long last_size;        /* the size of last transferring
				   (last_size <= (size / 2)) */
    unsigned long buf_addr;    /* The start address of stream buffer */
    unsigned long data_addr;    /* The start address of SPU streaming
				   data in main memory */
} SpuStVoiceAttr;

typedef struct {
    long size;            /* The size of stream buffer */
    long low_priority;        /* transfer priority */
    SpuStVoiceAttr voice[24];
} SpuStEnv;

#ifndef __SPU_ST_TRANSFERCALLBACK_PROC
#define __SPU_ST_TRANSFERCALLBACK_PROC

typedef void (*SpuStCallbackProc)(unsigned long, long);

#endif /* __SPU_TRANSFERCALLBACK_PROC */

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
* Initializes SPU streaming. Called only once in an executed program
* @param mode Not used under the current specificcation, Pass in 0
* @return Pointer to the SPU streaming environment structure SpuStEnv.
*/
extern SpuStEnv *SpuStInit(long);

extern long SpuStQuit(void);

extern long SpuStGetStatus(void);

extern unsigned long SpuStGetVoiceStatus(void);

/**
 * Prepares for a stream in SPU streaming, and provides instructions for starting it.
 * The voices for the stream are set in voice_bit by ORing the appropriate values SPU_0CH … SPU_23CH.<br>
 * flag values are:
 * <ul>
 * <li>SPU_ST_PREPARE = Preparation
 * Prepares the stream according to the attributes of the SpuStEnv structure returned by SpuStInit().
 * After preparation, the callback function set by SpuStSetPreparationFinishedCallback() is called.</li>
 * <li>SPU_ST_PLAY = Start
 * The stream is started according to the attributes of the SpuStEnv structure returned by SpuStInit().
 * If streaming status is SPU_ST_PREPARE, the voice is keyed on. If the status is SPU_ST_TRANSFER,
 * the transfer waits until processing for the current streams is transferred to the latter part of the stream
 * buffer.
 * When one transfer to the stream buffer for all streams is completed, the callback function set by
 * SpuStSetTransferFinishedCallback() is called, and the attributes for the next transfer for each stream
 * are set.
 * When a stream is completed, the callback function set by SpuStSetStreamFinishedCallback() is called
 * (just before the next transfer if other streams are processed.)</li>
 * <ul>
 * @param flag Stream state flag
 * @param voice_bit Streaming voices
 * @return SPU_ST_ACCEPT Processing is accepted.<br>
 * SPU_ST_NOT_AVAILABLE SPU streaming is not available. SpuStInit()
 * has not been called.<br>
 * SPU_ST_INVALID_ARGUMENTS The value of the arguments is not in the
 * specification.<br>
 * SPU_ST_WRONG_STATUS SpuStTransfer() not accepted. The causes
 * are:
 * <ul>
 * <li>The current status is SPU_ST_FINAL.</li>
 * <li>flag is SPU_ST_PREPARE, and the current status is SPU_ST_PREPARE.</li>
 * <li>flag is SPU_ST_PLAY, and the current status is SPU_ST_IDLE.</li>
 * </ul>
 */
extern long SpuStTransfer(long flag, unsigned long voice_bit);

extern SpuStCallbackProc SpuStSetPreparationFinishedCallback(SpuStCallbackProc func);

extern SpuStCallbackProc SpuStSetTransferFinishedCallback(SpuStCallbackProc func);

extern SpuStCallbackProc SpuStSetStreamFinishedCallback(SpuStCallbackProc func);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
#endif /* _LIBSPU_H_ */
/* DON'T ADD STUFF AFTER THIS */
