#include <sys/types.h>
#include <libgpu.h>
#include <libetc.h>
#include <libgs.h>
#include "../../lib/header/CdReader.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/MemUtils.h"

#define MODE_NTSC 0
#define SCREEN_W 320
#define SCREEN_H 240
#define OT_LEN 8

DISPENV disp_env[2];
DRAWENV draw_env[2];
u_long ot[2][OT_LEN];
u_char curr_buffer = 0;

u_short pri = OT_LEN - 1;

GsSPRITE cappy_sprite;

void init() {
    // Reset gpu and enable interrupts
    ResetGraph(0);

    // Configure the DISPENVs for NTSC mode
    SetDefDispEnv(&disp_env[0], 0, 0, SCREEN_W, SCREEN_H);
    SetDefDispEnv(&disp_env[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);

    // Configure the pair of DRAWENVs for the DISPENVs
    SetDefDrawEnv(&draw_env[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw_env[1], 0, 0, SCREEN_W, SCREEN_H);

    // Set the video mode (default is NTSC so not required here)
    SetVideoMode(MODE_NTSC);
    SetDispMask(1);
    PutDispEnv(&disp_env[curr_buffer]);
    PutDrawEnv(&draw_env[curr_buffer]);
    FntLoad(960, 0);
    FntOpen(20, 20, SCREEN_W, 50, 0, 256);
    MEM_INIT_HEAP_3();
    CDR_INIT();
    CdrData cappy_data = *cdr_read_file("PLAYER.TIM");
    asmg_load_sprite(&cappy_sprite, &cappy_data, 50, 50, BIT_DEPTH_4);
    ClearOTag(ot[0], 256);
    ClearOTag(ot[1], 256);
}

void tick() {

}

void draw() {
    FntPrint("Hello LibGPU");
    AddPrim(&cappy_sprite, &ot[curr_buffer][pri--]);
    DrawOTag(ot[curr_buffer]);
    PutDispEnv(&disp_env[curr_buffer]);
    PutDrawEnv(&draw_env[curr_buffer]);
    FntFlush(-1);
    DrawSync(0);
    VSync(0);
    curr_buffer = curr_buffer % 2 ? 0 : 1;
    pri = OT_LEN -1;
}



int main() {
    init();

    while(1) {
        tick();
        draw();
    }
}




