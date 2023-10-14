#include "libgpu.h"
#include "libetc.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define NUM_BUFFERS 2

DISPENV disp_env[NUM_BUFFERS];
DRAWENV draw_env[NUM_BUFFERS];
int db;


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

int main() {
    init();

    while(1) {
        FntPrint("Hello LibGPU, curr buff = %d", db);
        FntFlush(-1);

        display();
    }
}




