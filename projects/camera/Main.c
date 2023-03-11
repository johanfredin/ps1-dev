#include "../../lib/header/GPUBase.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GameObject.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/CollisionHandler.h"
#include "../../lib/header/FntLogger.h"

// Constants
#define TBX_SPEED 3
#define NUM_PLAYERS 1
#define NUM_BOUNDS 6

Player *player;
Camera camera;
Controller g_ctrl;
GsBOXF bounds[NUM_BOUNDS]; // Use GsBOXF so we can render them simply
RECT r_bounds[NUM_BOUNDS];
RECT clipping_area = {0, 0, 320, 240};

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

void init_bounds() {
    GsBOXF r1 = {1, 100, 100, 50, 50, 255, 0, 0};
    GsBOXF r2 = {1, 175, 100, 100, 1, 155, 10, 0};
    GsBOXF r3 = {1, 200, 50, 1, 60, 215, 0, 10};
    GsBOXF r4 = {1, 200, 220, 1, 1, 195, 0, 50};
    GsBOXF r5 = {1, 450, 350, 20, 80, 10, 255, 50};
    GsBOXF r6 = {1, 350, 450, 100, 10, 50, 155, 20};
    RECT rect_1 = {100, 100, 50, 50};
    RECT rect_2 = {175, 100, 100, 1};
    RECT rect_3 = {200, 50, 1, 60};
    RECT rect_4 = {200, 220, 1, 1};
    RECT rect_5 = {450, 350, 20, 80};
    RECT rect_6 = {350, 450, 100, 10};
    bounds[0] = r1;
    bounds[1] = r2;
    bounds[2] = r3;
    bounds[3] = r4;
    bounds[4] = r5;
    bounds[5] = r6;
    r_bounds[0] = rect_1;
    r_bounds[1] = rect_2;
    r_bounds[2] = rect_3;
    r_bounds[3] = rect_4;
    r_bounds[4] = rect_5;
    r_bounds[5] = rect_6;
}

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

void draw_bounds() {
    u_char i;
    for (i = 0; i < NUM_BOUNDS; i++) {
        GPUB_GS_SORT_BOX_FILL(&bounds[i]);
    }
}

void init_players() {
    // Load hero sprites from disc
    CdrData *asset = cdr_read_file("PLAYER.TIM");
    GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
    GameObject *go;
    Animation *anim;

    logr_log(INFO, "Main.c", "init_players", "******************************************");
    logr_log(INFO, "Main.c", "init_players", "* FETCHING PLAYER ASSETS                  *");
    logr_log(INFO, "Main.c", "init_players", "******************************************");

    asmg_load_sprite(sprite, asset, 126, 128, BIT_DEPTH_8);
    go = gobj_init(sprite, 16, 16, TBX_SPEED, TBX_SPEED, 100, GOBJ_TYPE_PLAYER);
    anim = gobj_anim_init(0, 0, 3, 3);
    player = gobj_player_init(anim, go, 0);
}

void update() {
    gobj_player_tick(player, &g_ctrl);
    gobj_anim_tick(player->anim, player->gobj);
    chdlr_collision_resolve(player->gobj, r_bounds, NUM_BOUNDS);
    gobj_camera_tick(&camera);

    // set clipping area
    clipping_area.x = camera.x;
    clipping_area.y = camera.y;
    GsSetClip2(&clipping_area);
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

    // Call draw-sync anc vsync and swap buffers
    gpub_display_sync();
    gpub_sort_clear(gpub_curr_ot);

    // Draw the ot
    gpub_draw_ot(gpub_curr_ot, (short) -camera.x, (short) -camera.y);
    FNT_PRINT_CAM(&camera);
}

int main() {
    MEM_INIT_HEAP_3();
    CDR_INIT();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    init_ots();
    init_players();

    CDR_CLOSE();
    CTRL_INIT_P1(&g_ctrl);
    init_bounds();
    gobj_camera_init(&camera, player->gobj);
    while (1) {
        update();
        draw();
    }
}
