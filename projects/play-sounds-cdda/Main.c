/**
 * Plays sequence data using LibSND, remake of the psyq sample project that loads data from a cd instead.
 */

#include <libspu.h>
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"
#include <libcd.h>
#include <stdio.h>

#define NEXT_EMPTY_VAB_ID (-1)

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

Color snd_bg_color = {120, 50, 100};

void print_loc(int i, CdlLOC *loc) {
    printf("-------------------------Pos at i=%d sector=%d, track=%d, min=%d, sec=%d\n", i, loc->sector, loc->track,
           loc->minute, loc->second);
}

void retrieve_cdda_audio() {
    CdlLOC loc[100];
    int data[2];
    int n_tocs;
    SpuInit();
    CdInit();
    SpuSetCommonCDMix(SPU_ON);
    SpuSetCommonCDVolume(0x3FFF, 0x3FFF);

    // Get CD TOC
    if ((n_tocs = CdGetToc(loc)) == 0) {
        printf("No TOC found\n");
    }

    printf("n_toc=%d\n", n_tocs);
//    print_loc(0, &loc[0]);
//    print_loc(1, &loc[1]);
//    print_loc(2, &loc[2]);

    // Prevent out of bounds pos
    for (int i = 1; i < 3; i++) {
        CdIntToPos(CdPosToInt(&loc[i]) - 74, &loc[i]);
        print_loc(i, &loc[i]);
    }

    data[0] = CdPosToInt(&loc[0]);
    data[1] = 0;
    u_char param = CdlModeRept | CdlModeDA;
    CdControl(CdlSetmode, &param, 0);
//    CdPlay(2, &data[0], 0);
    // Play second track
    CdControl(CdlPlay, (u_char *) &loc[1], 0);
}

int main() {
    MEM_INIT_HEAP_3();
    gpub_set_bg_color(&snd_bg_color);
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    GPUB_INIT_DEBUG_FNT(960, 512, 5, 10, 256, 256, 1, 512);
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);

    // Init sound lib
    retrieve_cdda_audio();
    while (1) {
        FntPrint("LibSND sounds example\n");
        FntPrint("X to play a sequence\n");
        FntPrint("Y to load a sep\n");
        FntPrint("Start to start CDDA playback\n");
        FntPrint("Select to stop playback\n");
        FntPrint("Current key=%d\n", 0);

        // Clear display
        gpub_clear_display();

        // Assign and clear current ot
        gpub_curr_ot = &gpub_ot[gpub_current_buffer];
        gpub_clear_ot(gpub_curr_ot);

        // Call drawsync anc vsync and swap buffers
        gpub_display_sync();
        gpub_sort_clear(gpub_curr_ot);

        // Draw the ot
        gpub_draw_ot(gpub_curr_ot, 0, 0);
    }
}

