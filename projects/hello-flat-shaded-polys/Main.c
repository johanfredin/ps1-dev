#include "libgpu.h"
#include "libetc.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define NUM_BUFFERS 2
#define OT_LEN 8
#define PRIM_BUFF_32K 32768
#define NULL 0

DISPENV disp_env[NUM_BUFFERS];
DRAWENV draw_env[NUM_BUFFERS];

// Ordering table
u_long ot[NUM_BUFFERS][OT_LEN];

// Current buffer
u_char db;

// Primitive buffer (from LameGuy tutorial):
/*
 A primitive buffer is simply a global array of char elements used as a buffer to define primitive packets to.
 It is also a lot less wasteful than defining multiple arrays for different primitive types as all primitive
 types can be defined in just a single global buffer. The reason it is recommended that primitive packets
 must be defined in a global buffer is because the primitives must exist in memory until the GPU gets around
 to processing it. If you were to define primitives as a local variable in a function and register it to an
 ordering table, that primitive has most likely been overwritten by other things (since locals are generally
 temporary variables), resulting in a corrupted ordering table which would result to a GPU lock up or crash.
 A primitive buffer can be defined as such, 32KB buffer for primitives should be plenty.
 */
char prim_buff[NUM_BUFFERS][PRIM_BUFF_32K];
/*
 The next_prim variable will be used to keep track where the next primitive should be written to.
 Therefore, this variable must be set to the first primitive buffer and reset in your display function.
 */
char *next_prim = NULL;

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

    next_prim = prim_buff[0];
}

TILE *init_tile(short x, short y, u_short w, u_short h, u_char r, u_char g, u_char  b) {
    TILE *tile = (TILE *) next_prim;
    setTile(tile);
    setXY0(tile, x, y);
    setWH(tile, w, h);
    setRGB0(tile, r, g, b);
    addPrim(ot[db], tile);
    next_prim += sizeof(TILE);
    return tile;
}

void display() {
    // Wait for GPU to finish drawing and V-Blank
    DrawSync(0);
    VSync(0);

    // Apply environments
    PutDispEnv(&disp_env[db]);
    PutDrawEnv(&draw_env[db]);

    // Enable display
    SetDispMask(1);

    // Draw the ordering table
    DrawOTag(&ot[db][OT_LEN - 1]);

    // Flip current buffer
    db = !db;
    next_prim = prim_buff[db];  // Reset the next primitive pointer
}

int main() {
    init();

    while (1) {
        // Clear ot
        ClearOTagR(ot[db], OT_LEN);
        init_tile(60, 70, 32, 32, 110, 255, 0);
        init_tile(50, 50, 32, 32, 0, 255, 255);

        FntPrint("Hello Flat Shaded TILE %d", sizeof(TILE));
        FntFlush(-1);

        display();
    }
}




