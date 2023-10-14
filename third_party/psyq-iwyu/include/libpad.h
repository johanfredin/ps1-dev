#ifndef _LIBPAD_H_
#define _LIBPAD_H_

/*
 *	File:libpad.h
 *	Copyright (C) 1997 by Sony Computer Entertainment Inc.
 *			All rights Reserved
 */
/*
 * $PSLibId: Run-time Library Release 4.7$
 */

/* don't change these macros and structures which is referred in controler code */ 

#ifndef TRUE
#define TRUE		1
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef NULL
#define NULL		0
#endif


#define PadStateDiscon		0
#define PadStateFindPad		1
#define PadStateFindCTP1	2
#define PadStateFindCTP2	3
#define PadStateReqInfo		4
#define PadStateExecCmd		5
#define PadStateStable		6

#define InfoModeCurID		1
#define InfoModeCurExID		2
#define InfoModeCurExOffs	3
#define InfoModeIdTable		4

#define InfoActFunc		1
#define InfoActSub		2
#define InfoActSize		3
#define InfoActCurr		4
#define InfoActSign		5

#define PadMaxCurr		60	/* PS maximum current supply */
#define PadCurrCTP1		10	/* SCPH-1150 biblator current */


/*
 * Prototypes
 */

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/**
 * Initializes the control environment for a controller.
 * <p>
 * When using this function, other initialization routines such as PadInitMtap(), InitPAD(), InitGUN(), InitTAP(),
 * and PadInit() cannot be used.
 * <p>
 * In libpad, controller connection state is maintained by the library. If the connection state is invalid, the
 * controller isn’t be recognized. Therefore, when a controller is used by both parent and child processes,
 * each process must call PadInitDirect().
 * <p>
 * For the format of the receive buffer, see “Receive Buffer Data Format” of Chapter 13 (Controller/Peripherals
 * Library) of the Library Overview.
 * <p>
 * If a Multi Tap is not used, using this function for initialization reduces program size by about 1.6KB.
 * Meaning of analog values:
 * @param pad1 Port 1 receive results (34 bytes)
 * @param pad2 Port 2 receive results (34 bytes)
 */
void PadInitDirect(unsigned char *pad1, unsigned char *pad2);
void PadInitMtap(unsigned char *, unsigned char *);
void PadInitGun(unsigned char *, int);
int PadChkVsync(void);

/**
 * Start reading from controller.
 */
void PadStartCom(void);
void PadStopCom(void);

/**
 * In general, communication with the controller takes place once per frame (1/60th of a second). However,
 * when a lower update rate is desired (e.g. when polling for a button press), communication with the
 * controller can be temporarily disabled with this function to provide the application with greater processing
 * time.
 * <p>
 * The vertical retrace interrupt itself is not enabled or disabled, so PadEnableCom() only works between
 * PadStartCom() and PadStopCom().
 * <p>
 * Ports 0 and 1 have a default value of "enabled." Calling PadInitDirect(), PadInitMtap(), PadStartCom(), or
 * PadStopCom() don’t affect the enable/disable state set by PadEnableCom().
 * <p>
 * If communication is suspended for three seconds or more, the controller is reset. If communication is
 * subsequently restarted, the return value from PadGetState() temporarily becomes PadStateDiscon and a
 * retry is generated to refetch controller information. For this reason, the return value from PadGetState()
 * needs to be monitored so that refetched actuator information can be properly processed.
 * <p>
 * Inhibit communication with one port using PadEnableCom(). When the mouse is attached to the inhibited
 * port, the controller which was attached to the enabled port will not recognize the invalid state of the mouse.
 * Therefore, the same setting of enabling/inhibiting communication should be performed for both ports.
 * @param mode Bit 0 is used to enable/disable port 0, and bit 1 is used to enable/disable port 1. 1 = enabled; 0 = disabled
 * @return The previous enable/disable state of communication before the function was called.
 */
unsigned PadEnableCom(unsigned mode);
void PadEnableGun(unsigned char);
void PadRemoveGun(void);

/**
 * Checks that the controller is connected, determines when button-press information is valid, and determines
 * when information from the actuators is valid.
 * port represents the port number to be checked, as follows:
 * <table>
 *  <tr>
 *      <th>Type</th>
 *      <th>Port 1</th>
 *      <th>port 2</th>
 *  </tr>
 *      <td>Direct connection</td>
 *      <td>0x00</td>
 *      <td>0x10</td>
 *  </tr>
 *  </tr>
 *      <td>Multi Tap A</td>
 *      <td>0x00</td>
 *      <td>0x10</td>
 *  </tr>
 *  </tr>
 *      <td>Multi Tap B</td>
 *      <td>0x01</td>
 *      <td>0x110</td>
 *  </tr>
 * </table>
 * @param the port number to be checked
 * @return
 * <table>
 *  <tr>
 *      <th>Value</th>
 *      <th>Macro (libpad.h)</th>
 *      <th>Controller connection state</th>
 *  </tr>
 *  <tr>
 *      <td>0</td>
 *      <td>PadStateDiscon</td>
 *      <td>Controller disconnected</td>
 *  </tr>
 *  <tr>
 *      <td>1</td>
 *      <td>PadStateFindPad</td>
 *      <td>Find controller connection (checking)</td>
 *  </tr>
 *  <tr>
 *      <td>2</td>
 *      <td>PadStateFindCTP1</td>
 *      <td>Check for connection with a controller that does not support the vibration function (including SCPH-1150). (Complete the acquisition of controller information)</td>
 *  </tr>
 *  <tr>
 *      <td>4</td>
 *      <td>PadStateReqInfo</td>
 *      <td>Actuator information being retrieved (data being retrieved)</td>
 *  </tr>
 *  <tr>
 *      <td>5</td>
 *      <td>PadStateExecCmd</td>
 *      <td>Library is communicating with controller (e.g. PadSetActAlign())</td>
 *  </tr>
 *  <tr>
 *      <td>6</td>
 *      <td>PadStateStable</td>
 *      <td>Check for connection with a controller that has a vibration function (DUAL SHOCK). Retrieval of actuator information completed, or library-controller communication completed (PadSetActAlign(), etc. can be called)</td>
 *  </tr>
 * </table>
 */
int PadGetState(int port);
int PadInfoMode(int, int, int);
int PadInfoAct(int, int, int);
int PadInfoComb(int, int, int);
int PadSetActAlign(int, unsigned char *);
int PadSetMainMode(int socket, int offs, int lock);
void PadSetAct(int, unsigned char *, int);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _LIBPAD_H_ */

/* don't add stuff after this */

