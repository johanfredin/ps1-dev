/**
 * Plays 2 VAG files using LibSPU, more low level and tedious.
 */

#include <libspu.h>
#include "../../lib/header/Logger.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/SoundEffect.h"

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

CdrData **vags;

void init_ptrs() {
    vags = MEM_CALLOC_3_PTRS(2, CdrData);
    vags[0] = cdr_read_file("STEP.VAG");
    vags[1] = cdr_read_file("HPUP.VAG");
    asmg_audio_init();
    asmg_transfer_vag_to_spu(vags[0], SFX_WALK);
    asmg_transfer_vag_to_spu(vags[1], SFX_HPUP);
}

int main() {
    MEM_INIT_HEAP_3();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);

    CDR_INIT();
    init_ptrs();
    CDR_CLOSE();
    ASMG_AUDIO_PLAY(SFX_WALK);
    ASMG_AUDIO_PLAY(SFX_HPUP);

    while (1) {
        FntPrint("Pay sounds example");

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

