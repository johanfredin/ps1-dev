#include <libspu.h>
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/GameObject.h"
#include "../../lib/header/Map.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/SoundEffect.h"


// Constants
#define NUM_PLAYERS 1
#define NUM_OTS 2
#define OT_MAIN &gpub_ot[gpub_current_buffer][0]
#define OT_HUD &gpub_ot[gpub_current_buffer][1]

// Voice channels
#define SFX_WALK SPU_00CH
#define SFX_HPUP SPU_01CH


const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

GsOT orderingTable[GPUB_NUM_BUFFERS][NUM_OTS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][NUM_OTS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS][NUM_OTS];

Player *player;
Camera camera;
CdrData **sfx_files;
RECT clipping_area = {0, 0, 320, 240};

void init_players(u_char numPlayers);
void init_ots();
void update();
void draw();

int main() {
    MEM_INIT_HEAP_3();
    asmg_audio_init();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    init_ots();
    CDR_INIT();
    sfx_load();
    init_players(NUM_PLAYERS);
    gobj_camera_init(&camera, player->gobj);
    map_init(1, player, &camera);
//    CDR_CLOSE();


    ASMG_AUDIO_PLAY(SFX_HPUP);

    while (1) {
        update();
        draw();
    }
}

void init_players(u_char numPlayers) {
    // Load hero sprites from disc
    CdrData *asset = cdr_read_file("PLAYER.TIM");
    logr_log(INFO, "Main.c", "init_players", "******************************************");
    logr_log(INFO, "Main.c", "init_players", "* FETCHING PLAYER ASSETS, NUM-PLAYERS=%d *", numPlayers);
    logr_log(INFO, "Main.c", "init_players", "******************************************");

    for (int i = 0; i < numPlayers; i++) {
        GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
        asmg_load_sprite(sprite, asset, 126, 128, BIT_DEPTH_8);
        GameObject *go = gobj_init(sprite, 16, 16, 0, 0, 100, GOBJ_TYPE_PLAYER);
        Animation *anim = gobj_anim_init(0, 0, 3, 3);
        player = gobj_player_init(anim, go, 0);
    }
}

void sfx_load() {
    sfx_files = MEM_CALLOC_3_PTRS(2, CdrData);
    sfx_files[0] = cdr_read_file("STEP.VAG");
    sfx_files[1] = cdr_read_file("HPUP.VAG");

    asmg_transfer_vag_to_spu(sfx_files[0], SFX_WALK);
    asmg_transfer_vag_to_spu(sfx_files[1], SFX_HPUP);

    CDR_DATA_FREE(sfx_files[0]);
    CDR_DATA_FREE(sfx_files[1]);
    MEM_FREE_3_AND_NULL(sfx_files);
}

void init_ots() {
    for (int i = 0; i < GPUB_NUM_BUFFERS; i++) {
        gpub_init_ot(&gpub_ot[i][0], &orderingTable[i][0], minorOrderingTable[i][0], GPUB_OT_LENGTH_STD);
        gpub_init_ot(&gpub_ot[i][1], &orderingTable[i][1], minorOrderingTable[i][1], GPUB_OT_LENGTH_SM);
    }
}

void update() {
    map_tick(player, &camera);
    gobj_camera_tick(&camera);

    // set clipping area
    clipping_area.x = camera.x;
    clipping_area.y = camera.y;
    GsSetClip2(&clipping_area);
}

void draw() {
    gpub_clear_display();
    gpub_clear_ot(OT_MAIN);
    gpub_clear_ot(OT_HUD);

    // Draw world
    gpub_curr_ot = OT_MAIN;
    map_ot_sort_bg_layers();
    gobj_player_draw(player);
    map_ot_sort_fg_layers();

    // Draw hud and dialogs
    gpub_curr_ot = OT_HUD;
    map_ot_sort_hud_layer();

    gpub_display_sync();
    gpub_sort_clear(OT_MAIN);
    gpub_draw_ot(OT_MAIN, -camera.x, -camera.y);
    gpub_draw_ot(OT_HUD, 0, 0);
}
