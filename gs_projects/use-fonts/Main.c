#include "../../lib/header/Logger.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/TxtHandler.h"

#include <libetc.h>

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

char *animated_messages[3];
u_long curr_btn = 0;
u_long prev_btn = 0;

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

int main() {
    Color bg_color = {20, 70, 120};
    MEM_INIT_HEAP_3();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    gpub_set_bg_color(&bg_color);
    init_ots();
    CDR_INIT();
    Font *fnt = txt_fnt_init("FONT.TIM", 8, 8, 2);
    CDR_CLOSE();
    animated_messages[0] = "Animated Message works!";
    animated_messages[1] = "Johan is cool";
    animated_messages[2] = "Julia is cool";
    Dialog *dlg_anim = txt_dlg_init(animated_messages, "yolo", 3, fnt, 5, 50, 150, 1);

    while (1) {
        // Clear display
        gpub_clear_display();

        // Assign and clear current ot
        gpub_curr_ot = &gpub_ot[gpub_current_buffer];
        gpub_clear_ot(gpub_curr_ot);

        // Sort game objects into current ot
        prev_btn = PadRead(1);
        FntPrint("Fonts test project\ncurr=%d, prev=%d", curr_btn, prev_btn);
        txt_dlg_draw(dlg_anim);
        txt_dlg_tick(dlg_anim);
        curr_btn = PadRead(1);

        if(curr_btn & PADLup) {
            txt_next_msg(dlg_anim, 1);
        }

        // Call drawsync anc vsync and swap buffers
        gpub_display_sync();
        gpub_sort_clear(gpub_curr_ot);

        // Draw the ot
        gpub_draw_ot(gpub_curr_ot, 0, 0);
    }
}

