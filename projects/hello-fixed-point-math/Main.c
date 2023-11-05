#include <libgpu.h>
#include <libetc.h>
#include <libapi.h>
#include "../../lib/header/Logger.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define NUM_BUFFERS 2

#define PAD_1 0
#define PAD_2 1

#define NUM_CONTROLLERS 2
#define PAD_BUFF_SIZE 34

#define PAD_STAT_CONNECTED 0

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

#define PAD_TYPE_DIGITAL 0x4
#define PAD_TYPE_ANALOG 0x5
#define PAD_TYPE_DUAL_SHOCK 0x7

#define OT_LEN 8

#define POLY_SPEED ONE + (ONE >> 2) // 1.25

#define PRIM_BUFF_32K 32768

typedef struct PadType {
    unsigned char stat;
    unsigned char len: 4;
    unsigned char type: 4;
    unsigned short curr_btn;
    unsigned char rs_x, rs_y;  // right analog stick x,y
    unsigned char ls_x, ls_y;  // left analog stick x,y
} PadType;

DISPENV disp_env[NUM_BUFFERS];
DRAWENV draw_env[NUM_BUFFERS];

u_char db;

// Ordering table
u_long ot[NUM_BUFFERS][OT_LEN];
char prim_buff[NUM_BUFFERS][PRIM_BUFF_32K];
char *next_prim = 0;

POLY_F4 flat_poly;
TILE_16 *tile16;

/*
 * (From LameGuys tutorials)
 * Define an array of two 34 byte elements.
 * This array will be used as a buffer for storing incoming controller
 * data which requires at least about 34 bytes per controller port.
 * It may sound a bit excessive for storing input data, but this is necessary
 * in case a multitap is connected to one of the ports, as a multitap will
 * return up to 34 bytes from all four controllers connected to the tap.
 */
unsigned char pad_buff[NUM_CONTROLLERS][PAD_BUFF_SIZE];

void init() {
    // Reset gpu and enable interrupts
    ResetGraph(0);

    // Set the video mode (default is NTSC so not required here)
    SetVideoMode(MODE_NTSC);

    // Configure the DISPENVs for NTSC mode
    SetDefDispEnv(&disp_env[0], 0, 0, SCREEN_W, SCREEN_H);
    SetDefDispEnv(&disp_env[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);

    // Configure the pair of DRAWENVs for the DISPENVs
    SetDefDrawEnv(&draw_env[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw_env[1], 0, 0, SCREEN_W, SCREEN_H);

    // Specifies the clear color of the DRAWENV
    setRGB0(&draw_env[0], 63, 0, 127);
    setRGB0(&draw_env[1], 63, 0, 127);

    // Enable background clear
    draw_env[0].isbg = 1;
    draw_env[1].isbg = 1;

    // Apply environments
    PutDispEnv(&disp_env[0]);
    PutDrawEnv(&draw_env[0]);

    db = 0;

    FntLoad(960, 0);
    FntOpen(20, 20, SCREEN_W, 50, 0, 256);
}

void init_controllers() {
    // Initialize the pads
    InitPAD((char *) pad_buff[PAD_1], PAD_BUFF_SIZE, (char *) pad_buff[PAD_2], PAD_BUFF_SIZE);

    // Begin polling
    StartPAD();

    // TO avoid VSYNC timeout error
    ChangeClearPAD(1);
}

void display() {
    // Wait for GPU to finish drawing and V-Blank
    DrawSync(0);
    VSync(0);

    // Flip current buffer
    db = !db;

    // Apply environments
    PutDispEnv(&disp_env[db]);
    PutDrawEnv(&draw_env[db]);

    // Enable display
    SetDispMask(1);
}

void init_pad(PadType *pad, u_char *buffer) {
    // First byte in buffer determines if the controller is connected or not (0 = yes, 1 = no)
    if (buffer[0] == PAD_STAT_CONNECTED) {
        pad->stat = PAD_STAT_CONNECTED;

        /*
        * The following byte is the controller ID, denoting both the controller type and the number of bytes the controller has sent to the console.
         * The controller type is stored in the upper 4 bits of the ID byte while the data byte is at the lower 4 bits.
        */
        pad->type = buffer[1] >> 4;
        pad->len = buffer[1] & 0x0F;
        logr_log(INFO, "Main.c", "init_pad", "Controller connected, type=%d", pad->type);
    } else {
        logr_log(INFO, "Main.c", "init_pad", "Controller not connected");
    }

}

void check_pad(PadType *pad) {
    // Get the id
    char *current_button = "";
    u_char is_connected = pad->stat == PAD_STAT_CONNECTED;
    if (is_connected) {
        char *type = "";
        switch (pad->type) {
            case PAD_TYPE_DIGITAL:
                type = "Digital";
                break;
            case PAD_TYPE_ANALOG:
                type = "Analog";
                FntPrint("L3 (x,y), R3=(x, y),(%d,%d), (%d, %d)\n", pad->ls_x, pad->ls_y, pad->rs_x, pad->rs_y);
                break;
            case PAD_TYPE_DUAL_SHOCK:
                type = "Dual Shock";
                FntPrint("L3 (x,y), R3=(x, y),(%d,%d), (%d, %d)\n", pad->ls_x, pad->ls_y, pad->rs_x, pad->rs_y);
                break;
            default:
                type = "UNKNOWN";
        }
        FntPrint("Controller type=%s\n", type);

        if (!(pad->curr_btn & PAD_UP)) {
            current_button = "UP";
        }
        if (!(pad->curr_btn & PAD_DOWN)) {
            current_button = "DOWN";
        }
        if (!(pad->curr_btn & PAD_LEFT)) {
            current_button = "LEFT";
        }
        if (!(pad->curr_btn & PAD_RIGHT)) {
            current_button = "RIGHT";
        }

    }

    FntPrint("Connected=%d\nButton pressed=%s", is_connected, current_button);
}

void init_poly() {
    tile16 = (TILE_16 *) next_prim;
    setTile(tile16);
    setXY0(tile16, ONE * disp_env[0].disp.w >> 1, ONE * disp_env[0].disp.h >> 1);
    setRGB0(tile16, 128, 128, 128);
    addPrim(ot[db], tile16);

}

int main() {
    init();
    init_controllers();
    init_poly();

    PadType *p1 = (PadType *) pad_buff[PAD_1];
    PadType *p2 = (PadType *) pad_buff[PAD_2];

    init_pad(p1, pad_buff[PAD_1]);
    init_pad(p2, pad_buff[PAD_2]);
    while (1) {
        FntPrint("Hello Controllers!\n");
        check_pad(p1);
        DrawOTag(ot[db]);
        FntFlush(-1);
        display();
    }
}




