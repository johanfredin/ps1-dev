#include "../../lib/header/GPUBase.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GameObject.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/CollisionHandler.h"

// Constants
#define TBX_SPEED 3
#define NUM_PLAYERS 1
#define NUM_BOUNDS 4

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

Player *player;
DPad g_ctrl;
GsBOXF bounds[NUM_BOUNDS] = { // Use GsBOXF so we can render them simply
        {1, 100, 100, 50,  50, 255, 0,   0},
        {1, 175, 100, 100, 1,  155, 10,  0},
        {1, 200, 50,  1,   60, 215, 0,   10},
        {1, 200, 220, 1,   1,  195, 0,   50}
};
RECT r_bounds[NUM_BOUNDS] = {
        {100, 100, 50,  50},
        {175, 100, 100, 1},
        {200, 50,  1,   60},
        {200, 220, 1,   1}
};

void draw_bounds() {
    for (u_char i = 0; i < NUM_BOUNDS; i++) {
        GPUB_GS_SORT_BOX_FILL(&bounds[i]);
    }
}

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

void init_player() {
    // Load hero sprites from disc
    CdrData *asset = cdr_read_file("PLAYER.TIM");
    GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
    GameObject *go;

    logr_log(INFO, "Main.c", "init_players", "FETCHING PLAYER ASSETS");

    asmg_load_sprite(sprite, asset, 126, 128, BIT_DEPTH_8);
    go = gobj_init(sprite, 16, 16, TBX_SPEED, TBX_SPEED, 100, GOBJ_TYPE_PLAYER);

    // Skipping giving the player an animation will lead to static image, good enough for test
    player = gobj_player_init(NULL, go, 0);
}

void update() {
    gobj_player_tick(player, &g_ctrl);
    chdlr_collision_resolve(player->gobj, r_bounds, NUM_BOUNDS);
}

void draw() {
    // Clear display
    gpub_clear_display();

    // Assign and clear current ot
    gpub_curr_ot = &gpub_ot[gpub_current_buffer];
    gpub_clear_ot(gpub_curr_ot);

    // Sort game objects into current ot
    draw_bounds();
    gobj_player_draw(player);

    // Call drawsync anc vsync and swap buffers
    gpub_display_sync();
    gpub_sort_clear(gpub_curr_ot);

    // Draw the ot
    gpub_draw_ot(gpub_curr_ot, 0, 0);
}

int main() {
    MEM_INIT_HEAP_3();
    CDR_INIT();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    init_ots();
    init_player();
    CDR_CLOSE();

    DPAD_INIT(&g_ctrl);
    while (1) {
        update();
        draw();
    }
}
