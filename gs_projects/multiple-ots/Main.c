/*
 * Not really a HUD per se but an example of how to display text/dialog boxes/menu items in a separate OT without offset
 */

#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/GameObject.h"
#include "../../lib/header/TextBox.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"

// Constants
#define NUM_PLAYERS 1
#define NUM_BOUNDS 6
#define CAM_SPEED 6

#define NUM_OTS 2
#define OT_MAIN 0
#define OT_HUD 1

const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

GsOT orderingTable[GPUB_NUM_BUFFERS][NUM_OTS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][NUM_OTS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS][NUM_OTS];

Camera camera;
DlgBox dlg_box_fixed;
char *animated_messages[2];

DPad g_ctrl;

GsBOXF bounds[NUM_BOUNDS] = { // Use GsBOXF so we can render them simply
        {1, 100, 100, 50,  50, 255, 0,   0},
        {1, 175, 100, 100, 1,  155, 10,  0},
        {1, 200, 50,  1,   60, 215, 0,   10},
        {1, 200, 220, 1,   1,  195, 0,   50},
        {1, 450, 350, 20,  80, 10,  255, 50},
        {1, 350, 450, 100, 10, 50,  155, 20}
};
RECT r_bounds[NUM_BOUNDS] = {
        {100, 100, 50,  50},
        {175, 100, 100, 1},
        {200, 50,  1,   60},
        {200, 220, 1,   1},
        {450, 350, 20,  80},
        {350, 450, 100, 10}
};

RECT clipping_area = {0, 0, 320, 240};

void draw_bounds() {
    for (u_char i = 0; i < NUM_BOUNDS; i++) {
        GPUB_GS_SORT_BOX_FILL(&bounds[i]);
    }
}

void init_ots() {
    for (int i = 0; i < GPUB_NUM_BUFFERS; i++) {
        gpub_init_ot(&gpub_ot[i][OT_MAIN], &orderingTable[i][OT_MAIN], minorOrderingTable[i][OT_MAIN], GPUB_OT_LENGTH_STD);
        gpub_init_ot(&gpub_ot[i][OT_HUD], &orderingTable[i][OT_HUD], minorOrderingTable[i][OT_HUD], GPUB_OT_LENGTH_SM);
    }
}

int main() {
    Color bg_color = {90, 70, 20};
    VerticalGoraudColor canvas_color, fixed_color;
    MEM_INIT_HEAP_3();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    gpub_set_bg_color(&bg_color);
    init_ots();
    CDR_INIT();
    Font fnt = *txt_fnt_init("FONT.TIM", 8, 8, 2);
    CDR_CLOSE();
    asmg_audio_init();
    TBX_INIT_VERT_GORAUD_COLOR(&canvas_color, 10, 10, 40, 0, 0, 200 - 40);
    TBX_INIT_VERT_GORAUD_COLOR(&fixed_color, 50, 50, 10, 100, 100, 200);
    animated_messages[0] = "Animated Message works!";
    animated_messages[1] = "Johan is cool";
    DPAD_INIT(&g_ctrl);
    Dialog *dlg = txt_dlg_init(animated_messages, "yolo", 2, &fnt, 5, 30, 70, 1);
    dlg_box_fixed = *tbx_init_dlg_box(20, 60, 260, 0, 200, 60, &canvas_color, dlg);
    gobj_camera_init(&camera, NULL);

    while (1) {
        gpub_clear_display();
        gpub_clear_ot(&gpub_ot[gpub_current_buffer][OT_MAIN]);
        gpub_clear_ot(&gpub_ot[gpub_current_buffer][OT_HUD]);

        // Move camera depending on input
        DPAD_READ_INPUT(&g_ctrl);
        if (DPAD_DPAD_LEFT(&g_ctrl)) {
            camera.x -= CAM_SPEED;
        }
        if (DPAD_DPAD_RIGHT(&g_ctrl)) {
            camera.x += CAM_SPEED;
        }
        if (DPAD_DPAD_UP(&g_ctrl)) {
            camera.y -= CAM_SPEED;
        }
        if (DPAD_DPAD_DOWN(&g_ctrl)) {
            camera.y += CAM_SPEED;
        }

        // Update dialog
        tbx_tick(&dlg_box_fixed, &g_ctrl);

        // Draw rects and dialogs
        gpub_curr_ot = &gpub_ot[gpub_current_buffer][OT_MAIN];
        draw_bounds();

        gpub_curr_ot = &gpub_ot[gpub_current_buffer][OT_HUD];
        tbx_draw(&dlg_box_fixed);

        if (dlg_box_fixed.complete) {
            dlg_box_fixed.complete = 0;
            tbx_reset_dgl_box(&dlg_box_fixed);
        }

        gpub_display_sync();
        gpub_sort_clear(&gpub_ot[gpub_current_buffer][OT_MAIN]);
        gpub_draw_ot(&gpub_ot[gpub_current_buffer][OT_MAIN], -camera.x, -camera.y);
        gpub_draw_ot(&gpub_ot[gpub_current_buffer][OT_HUD], 0, 0);

        FntPrint("cam_x=%d, cam_y=%d, cam_w=%d, cam_h=%d\n", camera.x, camera.y, camera.viewport_w, camera.viewport_h);
    }
}
