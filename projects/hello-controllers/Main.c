#include <libgpu.h>
#include <libetc.h>
#include "../../lib/header/Controller.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define NUM_BUFFERS 2

DISPENV disp_env[NUM_BUFFERS];
DRAWENV draw_env[NUM_BUFFERS];

u_char db;

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

//void init_pad(PadType *pad, u_char *buffer) {
//    // First byte in buffer determines if the controller is connected or not (0 = yes, 1 = no)
//    if (buffer[0] == CTRL_STAT_CONNECTED) {
//        pad->stat = CTRL_STAT_CONNECTED;
//
//        /*
//        * The following byte is the controller ID, denoting both the controller type and the number of bytes the controller has sent to the console.
//         * The controller type is stored in the upper 4 bits of the ID byte while the data byte is at the lower 4 bits.
//        */
//        pad->type = buffer[1] >> 4;
//        pad->len = buffer[1] & 0x0F;
//        logr_log(INFO, "Main.c", "init_pad", "Controller connected, type=%d", pad->type);
//    } else {
//        logr_log(INFO, "Main.c", "init_pad", "Controller not connected");
//    }
//
//}

void check_pad(Controller *pad) {
    // Get the id
    char *current_button = "";
    u_char is_connected = CTRL_IS_CONNECTED(pad);
    if (is_connected) {
        char *type = "";
        switch (pad->type) {
            case CTRL_TYPE_DIGITAL:
                type = "Digital";
                break;
            case CTRL_TYPE_ANALOG:
                type = "Analog";
                FntPrint("Sticks stat L3 (x,y), R3=(x, y),(%d,%d), (%d, %d)\n", pad->ls_x, pad->ls_y, pad->rs_x,
                         pad->rs_y);
                break;
            case CTRL_TYPE_DUAL_SHOCK:
                type = "Dual Shock";
                FntPrint("Sticks stat L3 (x,y), R3=(x, y),(%d,%d), (%d, %d)\n", pad->ls_x, pad->ls_y, pad->rs_x,
                         pad->rs_y);
                break;
            default:
                type = "UNKNOWN";
        }
        FntPrint("Controller type=%s\n", type);


        if (CTRL_IS_BTN_UP(pad)) {
            current_button = "UP";
        }
        if (CTRL_IS_BTN_DOWN(pad)) {
            current_button = "DOWN";
        }
        if (CTRL_IS_BTN_LEFT(pad)) {
            current_button = "LEFT";
        }
        if (CTRL_IS_BTN_RIGHT(pad)) {
            current_button = "RIGHT";
        }
        if (CTRL_IS_BTN_CROSS(pad)) {
            current_button = "X";
        }
        if (CTRL_IS_BTN_SQUARE(pad)) {
            current_button = "[]";
        }
        if (CTRL_IS_BTN_CIRCLE(pad)) {
            current_button = "()";
        }
        if (CTRL_IS_BTN_TRIANGLE(pad)) {
            current_button = "/\\";
        }
        if (CTRL_IS_BTN_START(pad)) {
            current_button = "START";
        }
        if (CTRL_IS_BTN_SELECT(pad)) {
            current_button = "SELECT";
        }
        if (CTRL_IS_BTN_R1(pad)) {
            current_button = "R1";
        }
        if (CTRL_IS_BTN_R2(pad)) {
            current_button = "R2";
        }
        if (CTRL_IS_BTN_L1(pad)) {
            current_button = "L1";
        }
        if (CTRL_IS_BTN_L2(pad)) {
            current_button = "L2";
        }
        if (CTRL_IS_BTN_R3(pad)) {
            current_button = "R3";
        }
        if (CTRL_IS_BTN_L3(pad)) {
            current_button = "L3";
        }
    }
    FntPrint("Connected=%d\nButton pressed=%s", is_connected, current_button);
}

int main() {
    init();
    ctrl_init();

    Controller *p1 = ctrl_read(CTRL_PAD_1);
    Controller *p2 = ctrl_read(CTRL_PAD_2);

    while (1) {
        FntPrint("Hello Controllers!\n");
        check_pad(p1);
        FntFlush(-1);
        display();
    }
}




