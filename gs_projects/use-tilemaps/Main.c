#include "../../lib/header/AssetManager.h"
#include "../../lib/header/Frames.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/Map.h"
#include "../../lib/header/GPUBase.h"

#define NUM_PLAYERS 1
#define NUM_OTS 2
#define OT_MAIN &gpub_ot[gpub_current_buffer][0]

const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

GsOT orderingTable[GPUB_NUM_BUFFERS][NUM_OTS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][NUM_OTS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS][NUM_OTS];

Player *player;
Camera camera;

void init_ots() {
    for (int i = 0; i < GPUB_NUM_BUFFERS; i++) {
        gpub_init_ot(&gpub_ot[i][0], &orderingTable[i][0], minorOrderingTable[i][0], GPUB_OT_LENGTH_STD);
        gpub_init_ot(&gpub_ot[i][1], &orderingTable[i][1], minorOrderingTable[i][1], GPUB_OT_LENGTH_SM);
    }
}

void init_player() {
     // Load hero sprites from disc
    CdrData *asset = cdr_read_file("PLAYER.TIM");
    GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);

    asmg_load_sprite(sprite, asset, 126, 128, BIT_DEPTH_8);
    GameObject *go = gobj_init(sprite, 16, 16, 4, 4, 100, GOBJ_TYPE_PLAYER);
    player = gobj_player_init(NULL, go, 0);

    MEM_FREE_3_AND_NULL(asset);
}

void update() {
    map_tick(player, &camera);
    gobj_camera_tick(&camera);
}

void draw() {
    gpub_clear_display();
    gpub_clear_ot(OT_MAIN);

    // Draw world
    gpub_curr_ot = OT_MAIN;
    map_ot_sort_bg_layers();
    gobj_player_draw(player);
    map_ot_sort_fg_layers();

    gpub_display_sync();
    gpub_sort_clear(OT_MAIN);
    gpub_draw_ot(OT_MAIN, -camera.x, -camera.y);
}

int main() {
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    MEM_INIT_HEAP_3();

    init_ots();
    CDR_INIT();
    init_player();
    gobj_camera_init(&camera, player->gobj);
    map_init(1, player, &camera);
    CDR_CLOSE();

    while (1) {
        update();
        draw();
    }
}
