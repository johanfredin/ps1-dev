#include "../../lib/header/Logger.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/TxtHandler.h"
#include "../../lib/header/TextBox.h"

const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];
DlgBox *dlg_box_fixed;
DPad g_ctrl;

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

void init_dialogs() {
    VerticalGoraudColor canvas_color, fixed_color;
    Font *fnt = txt_fnt_init("FONT.TIM", 8, 8, 2);
    TBX_INIT_VERT_GORAUD_COLOR(&canvas_color, 10, 10, 40, 0, 0, 200 - 40);
    TBX_INIT_VERT_GORAUD_COLOR(&fixed_color, 50, 50, 10, 100, 100, 200);

    char *animated_messages[2] = {
            "Animated Message works!",
            "Johan is cool"
    };

    Dialog *dlg = txt_dlg_init(animated_messages, "yolo", 2, fnt, 5, 30, 70, 1);
    dlg_box_fixed = tbx_init_dlg_box(20, 60, 260, 0, 260, 130, &canvas_color, dlg);
}

void update() {
    DPAD_READ_INPUT(&g_ctrl);
    tbx_tick(dlg_box_fixed, &g_ctrl);
    FntPrint("Current btn=%d, complete=%d\n", g_ctrl.curr_btn, dlg_box_fixed->complete);
    if (dlg_box_fixed->complete) {
        dlg_box_fixed->complete = 0;
        tbx_reset_dgl_box(dlg_box_fixed);
    }
}

void draw() {
    // Clear display
    gpub_clear_display();
    gpub_curr_ot = &gpub_ot[gpub_current_buffer];
    gpub_clear_ot(gpub_curr_ot);

    FntPrint("Dialog box test project\n");
    tbx_draw(dlg_box_fixed);

    // Call draw-sync anc vsync and swap buffers
    gpub_display_sync();
    gpub_sort_clear(gpub_curr_ot);

    // Draw the ot
    gpub_draw_ot(gpub_curr_ot, 0, 0);
}

int main() {
    Color bg_color = {90, 70, 20};
    MEM_INIT_HEAP_3();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    DPAD_INIT(&g_ctrl);

    init_ots();
    gpub_set_bg_color(&bg_color);
    CDR_INIT();
    init_dialogs();
    CDR_CLOSE();
    while (1) {
        update();
        draw();
    }
}

