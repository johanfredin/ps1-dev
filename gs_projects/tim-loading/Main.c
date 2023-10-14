#include "../../lib/header/Logger.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/AssetManager.h"

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];
GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

GsSPRITE sprite_alola_8, sprite_raichu_4, sprite_trees_16, sprite_tileset_8, region;
CdrData *data_alola, *data_raichu, *data_trees, *data_tileset;

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

void init_sprites() {
    data_alola = cdr_read_file("ALOLA_8.TIM");
    data_raichu = cdr_read_file("RAICHU_4.TIM");
    data_trees = cdr_read_file("TREES_16.TIM");
    data_tileset = cdr_read_file("TS8_TL.TIM");

    asmg_load_sprite(&sprite_alola_8, data_alola, 0, 0, BIT_DEPTH_8);
    asmg_load_sprite(&sprite_tileset_8, data_tileset, 0, 0, BIT_DEPTH_8);
    asmg_load_sprite(&sprite_raichu_4, data_raichu, 100, 100, BIT_DEPTH_4);
    asmg_load_sprite_region(&sprite_tileset_8, &region, 280, 55, 32, 16, 16, 16);
    asmg_load_16_bit_sprite(&sprite_trees_16, data_trees, 120, 10);
}

void draw() {
    // Clear display
    gpub_clear_display();

    // Assign and clear current ot
    gpub_curr_ot = &gpub_ot[gpub_current_buffer];
    gpub_clear_ot(gpub_curr_ot);

    GPUB_GS_SORT_FAST_OBJ(&sprite_tileset_8);
    GPUB_GS_SORT_FAST_OBJ(&sprite_alola_8);     // Draw 8 bit sprite
    GPUB_GS_SORT_FAST_OBJ(&sprite_raichu_4);    // Draw 4 bit sprite
    GPUB_GS_SORT_FAST_OBJ(&region);             // Draw 4 bit sprite
    GPUB_GS_SORT_FAST_OBJ(&sprite_trees_16);    // Draw 16 bit sprite

    // Call drawsync anc vsync and swap buffers
    gpub_display_sync();
    gpub_sort_clear(gpub_curr_ot);

    // Draw the ot
    gpub_draw_ot(gpub_curr_ot, 0, 0);
}

int main() {
    Color bg_color = {20, 90, 80};
    MEM_INIT_HEAP_3();
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    init_ots();
    gpub_set_bg_color(&bg_color);

    CDR_INIT();
    init_sprites();
    CDR_CLOSE();

    while (1) {
        draw();
        FntPrint("TIM loading test");
    }
}
