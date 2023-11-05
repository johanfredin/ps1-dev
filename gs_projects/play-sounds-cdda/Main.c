/**
 * Plays sequence data using LibSND, remake of the psyq sample project that loads data from a cd instead.
 */

#include "../../lib/header/GPUBase.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/DPad.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/CdReader.h"

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

Color snd_bg_color = {120, 50, 100};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    CdrDATrack tracks[2] = {
            {2, 0},
            {3, 0}
    };
    CdrDATrack current = tracks[0];
    DPad controller;
    MEM_INIT_HEAP_3();
    CDR_INIT();
    DPAD_INIT(&controller);
    gpub_set_bg_color(&snd_bg_color);
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    GPUB_INIT_DEBUG_FNT(960, 512, 5, 10, 256, 256, 1, 512);
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);

    // Init sound lib
    while (1) {
        // Check controller input
        DPAD_READ_INPUT(&controller);
        if (DPAD_DPAD_LEFT(&controller)) {
            cdr_da_stop(&current);
            current = tracks[0];
            cdr_da_play(&current);
        } else if (DPAD_DPAD_RIGHT(&controller)) {
            cdr_da_stop(&current);
            current = tracks[1];
            cdr_da_play(&current);
        } else if(DPAD_DPAD_DOWN(&controller)) {
            cdr_da_stop(&current);
        }


        // Draw display
        FntPrint("CDDA example\n");
        FntPrint("<- to play track 2\n");
        FntPrint("-> to play track 3\n");
        FntPrint("down to stop playback\n");
        FntPrint("Current track=%d, is_playing=%d\n", current.track, current.is_playing);
        FntPrint("Current key=%d\n", controller.curr_btn);

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

#pragma clang diagnostic pop

