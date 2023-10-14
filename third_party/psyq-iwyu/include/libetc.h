/* $PSLibId: Run-time Library Release 4.7$ */
#ifndef _LIBETC_H_
#define _LIBETC_H_

/*
 *  (C) Copyright 1993/1994 Sony Corporation,Tokyo,Japan. All Rights Reserved
 *
 * 			libetc.h: Pad Interface
 */

#include <types.h>

extern int PadIdentifier;
/*
 * PAD I/O (SIO Pad)
 */
#define PADLup     (1<<12)
#define PADLdown   (1<<14)
#define PADLleft   (1<<15)
#define PADLright  (1<<13)
#define PADRup     (1<< 4)
#define PADRdown   (1<< 6)
#define PADRleft   (1<< 7)
#define PADRright  (1<< 5)
#define PADi       (1<< 9)
#define PADj       (1<<10)
#define PADk       (1<< 8)
#define PADl       (1<< 3)
#define PADm       (1<< 1)
#define PADn       (1<< 2)
#define PADo       (1<< 0)
#define PADh       (1<<11)
#define PADL1      PADn
#define PADL2      PADo
#define PADR1      PADl
#define PADR2      PADm
#define PADstart   PADh
#define PADselect  PADk

#define MOUSEleft  (1<<3)
#define MOUSEright (1<<2)

/*
 * PAD utility macro: _PAD(x,y)
 *   x: controller ID (0 or 1)
 *   y: PAD assign macro
 *
 * Example: _PAD(0,PADstart) ... PADstart of controller 1
 *          _PAD(1,PADLup)   ... PADLup of controller 2
 */
#define _PAD(x, y) ((y)<<((x)<<4))

/* scratch pad address 0x1f800000 - 0x1f800400 */
#define getScratchAddr(offset)  ((u_long *)(0x1f800000+(offset)*4))

/*
 * Video Mode:	NTSC/PAL
 */
#define MODE_NTSC 0
#define MODE_PAL 1

/*
 * Prototypes
 */
#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int CheckCallback(void);
/**
 * Initialize a controller (for prototyping only).
 * Initializes the controller. Since this function supports only the 16-button controller, it should be used for
 * prototyping purposes only.
 * @param mode Always pass 0
 */
void PadInit(int mode);

/**
 * Initializes all system callbacks. Sets all callback functions to 0 (unregistered), and after securing the interrupt
 * context stack, sets up the environment for accepting interrupts.
 * ResetCallback() must be called after program boot, before any other processing is performed.
 * The environment initialized by ResetCallback() remains valid until StopCallback() is called.
 * It is acceptable to continuously call ResetCallback() without StopCallback(). However, the second and
 * subsequent calls are ignored.
 * @return
 */
int ResetCallback(void);

int RestartCallback(void);

int StopCallback(void);

/**
 * Wait for the next vertical blank, or return the vertical blank counter value.
 * @param mode <ul>
 * <li>0: Blocks until vertical sync is generated</li>
 * <li>1: Returns time elapsed from the point VSync() processing is last completed when mode=1or n in horizontal sync units</li>
 * <li>n (n>1): Blocks from the point VSync() processing is last completed when mode=1 or n until n number of vertical syncs are generated.</li>
 * <li>-n (n>0): Returns absolute time after program boot in vertical sync interval units.</li>
 * </ul>
 * @return
 */
int VSync(int mode);

int VSyncCallback(void (*f)(void));

long GetVideoMode(void);

/**
 * Declare current video signaling system.<br>
 * Declares the video signaling system indicated by mode to the libraries (MODE_NTSC(0) for NTSC and
 * MODE_PAL(1) for PAL).<br>
 * Related libraries will conform to the actions of the declared video signaling system environment.
 * Should be called in advance of all library functions.
 * @param mode Video signaling system mode
 * @return Previously-set video signaling system mode.
 */
long SetVideoMode(long mode);

/**
 * Read data from the controller (for prototyping only)
 * @param id Controller ID (unused)
 * @return Controller button status. High 2 bytes are pad 2, low 2 bytes are pad 1.
 */
u_long PadRead(int id);

void PadStop(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif /* _LIBETC_H_ */


