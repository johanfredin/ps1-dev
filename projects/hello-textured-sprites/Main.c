#include "libgpu.h"
#include "libetc.h"
#include "../../lib/header/CdReader.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"

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
u_short db;

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

SPRT cappy_8bit;
SPRT crash_16bit;
TIM_IMAGE tim_crash;
TIM_IMAGE tim_cappy;

void init_system() {
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
    setRGB0(&draw_env[0], 0, 0, 0);
    setRGB0(&draw_env[1], 0, 0, 0);

    // Enable background clear
    draw_env[0].isbg = 1;
    draw_env[1].isbg = 1;

    // Apply environments
    PutDispEnv(&disp_env[0]);
    PutDrawEnv(&draw_env[0]);

    db = 0;

    FntLoad(960, 0);
    FntOpen(100, 1, 180, 20, 1, 256);

    next_prim = prim_buff[0];

    // Enable display
    SetDispMask(1);
}

void load_tim_data(TIM_IMAGE *tim_data, CdrData *cdr_data) {
    // Read TIM information
    OpenTIM(cdr_data->file);
    ReadTIM(tim_data);

    // Upload pixel data to framebuffer
    LoadImage(tim_data->prect, tim_data->paddr);
    DrawSync(0);
    logr_log(INFO, "Main.c", "load_tim_data",
             "Texture loaded from %s {x, y, w, h}=%d, %d", cdr_data, tim_data->prect->x, tim_data->prect->y,
             tim_data->prect->w, tim_data->prect->h);

    // Upload CLUT to framebuffer if present
    if (tim_data->mode & 0x8) {
        LoadImage(tim_data->crect, tim_data->caddr);
        DrawSync(0);
        logr_log(INFO, "Main.c", "load_tim_data",
                 "Clut added for %s {x, y}=%d, %d", cdr_data, tim_data->crect->x, tim_data->crect->y);
    }
}

void set_sprite(TIM_IMAGE *tim, SPRT *sprite) {
    setSprt(sprite);
    addPrim(ot, sprite);                  // Sort the primitive and advance

    // Set sprite size
    short w = sprite->w = tim->prect->w << (2 - tim->mode & 0x3);
    short h = sprite->h = tim->prect->h;
    setWH(sprite, w, h);

    // Get CLUT values (if sprite not 16 bit)
    if (tim->mode & 0x8) {
        sprite->clut = getClut(tim->crect->x, tim->crect->y);
    }

    // Set UV offset
    u_short sprite_u = ((tim->prect->x & 0x3f) << (2 - tim->mode & 0x3));
    u_short sprite_v = (tim->prect->y & 0xff);
    setUV0(sprite, sprite_u, sprite_v);
    setRGB0(sprite, 128, 128, 128);

    logr_log(INFO, "Main.c", "set_sprite",
             "Sprite initialized {x, y, w, h, u, v}=%d, %d, %d, %d, %d, %d",
             sprite->x0, sprite->y0, sprite->w, sprite->h, sprite->u0, sprite->v0
    );

}

void sort_sprite(int x, int y, SPRT *sprite, TIM_IMAGE *tim, u_short u, u_short v) {
    setSprt(sprite);
    setXY0(sprite, x, y);                 // Set position
    setUV0(sprite, u, v);

    addPrim(ot[db], sprite);                  // Sort the primitive and advance
    next_prim += sizeof(SPRT);


    DR_TPAGE *dr_tpage = (DR_TPAGE *) next_prim;             // Sort the texture page value
    int t_page = getTPage(tim->mode & 0x3, 0, tim->prect->x, tim->prect->y);
    setDrawTPage(dr_tpage, 0, 1,  t_page);
    addPrim(ot[db], dr_tpage);
    next_prim += sizeof(DR_TPAGE);        // Return new primitive pointer (set to nextpri)
}

void display() {
    // Wait for GPU to finish drawing and V-Blank
    DrawSync(0);
    VSync(0);

    // Apply environments
    PutDispEnv(&disp_env[db]);
    PutDrawEnv(&draw_env[db]);

    // Draw the ordering table
    DrawOTag(&ot[db][OT_LEN - 1]);

    // Flip current buffer
    db = !db;
    next_prim = prim_buff[db];  // Reset the next primitive pointer
}

int main() {
    init_system();

    MEM_INIT_HEAP_3();
    CDR_INIT();
    // Acquire crash and cappy tims from cd
    CdrData *data_crash = cdr_read_file("CRASH.TIM");
    CdrData *data_cappy = cdr_read_file("CAPPY.TIM");
    CDR_CLOSE();

    // Acquire tim data
    load_tim_data(&tim_crash, data_crash);
    load_tim_data(&tim_cappy, data_cappy);

    set_sprite(&tim_crash, &crash_16bit);
    set_sprite(&tim_cappy, &cappy_8bit);

    PutDrawEnv(&draw_env[!db]);

    CDR_DATA_FREE(data_crash);
    CDR_DATA_FREE(data_cappy);

    // Cappy is a sprite sheet, we want one frame only
    setWH(&cappy_8bit, 64, 64);

    while (1) {
        // Clear ot
        ClearOTagR(ot[db], OT_LEN);

        FntPrint("                  Hello Textured sprites\n");

        sort_sprite(256, 32, &cappy_8bit, &tim_cappy, 0, 0);
        sort_sprite(0, 0, &crash_16bit, &tim_crash, 0, 0);

        FntFlush(-1);
        display();
    }
}





