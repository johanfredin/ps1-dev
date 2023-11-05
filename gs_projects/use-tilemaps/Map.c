#include "../../lib/header/Map.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/TileFetcher.h"
#include "../../lib/header/StrUtils.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/CollisionHandler.h"

#define MAP_START_FRAME 0
#define MAP_DIALOGS_IN_FRAME(dlg_cnt) dlg_cnt | 0

Frame *g_map_frames;
FR_TileSet **g_map_tile_sets;
CdrData **g_map_cdr_data_assets;
DPad g_ctrl;

Player *g_player;
Camera *g_camera;

u_char g_assets_cnt = 0, g_map_tilesets_count = 0;
u_char g_frame_cnt;
u_char g_current_frame = MAP_START_FRAME;

void init_frame(Frame *frame, char *json_map_file);
void frame_init_collision_blocks(Tile_Map *tile_map, Frame *frame);
void frame_init_teleports(Tile_Map *tile_map, Frame *frame);
void frame_init_dialogs(Tile_Map *tile_map, Frame *frame);
RECT get_rect(short x, short y, short w, short h);
void handle_block_collision(GameObject *gobj, Frame *frame);
void handle_teleport_collision(GameObject *gobj, Frame *frame);
void handle_dialog_collision(GameObject *gobj, Frame *frame);
void load_level_assets_from_cd(u_char level);

/**
 * Transfer data from the Tile_Set struct parsed from the map editor
 * to our frame specific FR_Tile_Set struct. We want separation of
 * concern between the two so that the Tiled version can be
 * freed afterwards
 * @param map map to transfer data from
 */
FR_TileSet *transfer_to_frame_tileset(Tile_Map *map);

void load_tilesets();

void map_init(u_char level, Player *player, Camera *camera) {
    u_char i;
    load_level_assets_from_cd(level);
    load_tilesets();

    g_map_frames = MEM_CALLOC_3(1, Frame);
    init_frame(&g_map_frames[g_frame_cnt++], "L1_WRLD.JSON");

    // Init controller
    DPAD_INIT(&g_ctrl);

    // Cleanup
    for (i = 0; i < g_assets_cnt; i++) {
        CDR_DATA_FREE(g_map_cdr_data_assets[i]);
    }
    MEM_FREE_3_AND_NULL(g_map_cdr_data_assets);
    g_camera = camera;
    g_player = player;
}

void load_level_assets_from_cd(u_char level) {
    if (level == 1) {
        g_map_cdr_data_assets = MEM_CALLOC_3_PTRS(5, CdrData);
        // Load tile set images
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TR.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BR.TIM");

        // Load tile map json files
        g_assets_cnt += g_map_tilesets_count;
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("L1_WRLD.JSON");
    }
    logr_log(DEBUG, "Map.c", "load_level_assets_from_cd", "%d assets loaded", g_assets_cnt);
}

/**
 * Populates our global array of FR_TileSet structs from the tileset CdrData assets.
 * Retrieve the GS_SPRITE and the asset name.
 */
void load_tilesets() {
    u_char i;
    g_map_tile_sets = MEM_CALLOC_3_PTRS(g_map_tilesets_count, FR_TileSet);
    for (i = 0; i < g_map_tilesets_count; i++) {
        GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
        g_map_tile_sets[i] = frames_malloc_fr_tileset();
        asmg_load_sprite(sprite, g_map_cdr_data_assets[i], 0, 0, BIT_DEPTH_8);
        g_map_tile_sets[i]->sprite = sprite;
        g_map_tile_sets[i]->source = g_map_cdr_data_assets[i]->name;
        logr_log(TRACE, "Map.c", "load_tilesets", "tileset=%s loaded", g_map_cdr_data_assets[i]->name);
    }
    logr_log(DEBUG, "Map.c", "load_tilesets", "%d tilesets loaded", g_map_tilesets_count);
}

void init_frame(Frame *frame, char *json_map_file) {
    // Declarations --------------------------
    CdrData *json_cdr_data;
    u_long *content;
    JSON_Data *json_map_data;
    Tile_Map *tile_map;

    // Map coords
    u_short frame_w, frame_h;
    u_char offset_x = 0, offset_y = 0;

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, g_map_cdr_data_assets, g_assets_cnt);
    logr_log(INFO, "Map.c", "init_frame", "JSON file=%s retrieved", json_map_file);
    content = json_cdr_data->file;
    json_map_data = jsonp_parse((char *) content);
    tile_map = tiled_populate_from_json(json_map_data);
    tiled_print_map(DEBUG, tile_map);
    frame->fr_tilesets = transfer_to_frame_tileset(tile_map);

    // Calc potential x and/or y offsets (e.g frame is smaller than screen w and/or h)
    frame_w = tile_map->width * tile_map->tile_width;
    frame_h = tile_map->height * tile_map->tile_height;

    if (frame_w < gpub_screen_w) {
        offset_x = (gpub_screen_w - frame_w) / 2;
    }
    if (frame_h < gpub_screen_h) {
        offset_y = (gpub_screen_h - frame_h) / 2;
    }

    frame->width = frame_w;
    frame->height = frame_h;

    // Give potential offsets to frame.
    frame->offset_x = offset_x;
    frame->offset_y = offset_y;
    logr_log(DEBUG, "Map.c", "init_frame", "frame offset_x=%d, offset_y=%d", offset_x, offset_y);

    // Load tilesets (frame may consist of multiple tilesets)
    tf_add_layers_to_frame(frame, frame->fr_tilesets, g_map_tilesets_count, tile_map);

    // Add collision blocks, teleports and dialogs from tile map
    frame_init_collision_blocks(tile_map, frame);

    // Housekeeping
    jsonp_free(json_map_data);
    tiled_free(tile_map);
}

void frame_init_collision_blocks(Tile_Map *tile_map, Frame *frame) {
    int i;
    CollisionBlock *collision_blocks;
    ObjectLayer_Bounds *curr_b;
    u_char blocks_cnt = tile_map->bounds_cnt;
    MAP_MALLOC_COLLISION_BLOCK(collision_blocks, blocks_cnt);
    for (i = 0, curr_b = tile_map->bounds; curr_b != NULL; i++, curr_b = curr_b->next) {
        u_short x = curr_b->x + frame->offset_x;
        u_short y = curr_b->y + frame->offset_y;
        collision_blocks->bounds[i] = get_rect((short) x, (short) y, (short) curr_b->width, (short) curr_b->height);
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;
}

FR_TileSet *transfer_to_frame_tileset(Tile_Map *map) {
    Tile_Set *curr_ts;
    FR_TileSet *fr_tile_sets = MEM_CALLOC_3(map->tilesets_cnt, FR_TileSet);
    u_char match_cnt = 0;

    // Iterate tilesets that the tile map is using
    for (curr_ts = map->tile_sets; curr_ts != NULL; curr_ts = curr_ts->next) {
        u_char i;

        // Iterate the map FR_Tileset array to look for a matching image
        for (i = 0; i < g_map_tilesets_count; i++) {
            u_char count;
            char *source = curr_ts->source;
            char substr[16];
            FR_TileSet *map_fr_tile_set = g_map_tile_sets[i];
            /*
             * Compare source in current map tileset with the one for the image
             * We do this by making a lower case comparison and checking if the
             * full source path in our tiled json file contains the current image name (minus the .tim suffix)
             */
            char *tim_name = strcpy(MEM_CALLOC_3(strlen(map_fr_tile_set->source) + 1, char), map_fr_tile_set->source);
            STR_TO_LOWERCASE(tim_name);

            STR_READ_UNTIL(tim_name, substr, '.', count);
            MEM_FREE_3_AND_NULL(tim_name);

            if (STR_CONTAINS(source, substr)) {
                // Match found, assign it to frame tilesets
                map_fr_tile_set->start_id = curr_ts->firstgid;
                fr_tile_sets[match_cnt++] = *map_fr_tile_set;
                break;
            }
        }
    }

    // Verify the expected amount of tilesets were transferred
    if (match_cnt != map->tilesets_cnt) {
        logr_log(ERROR, "Map.c", "transfer_to_frame_tileset",
                 "Incorrect number of matching tilesets between tile map and frame, match_cnt=%d, tileset_cnt=%d, shutting down...",
                 match_cnt, map->tilesets_cnt);
        exit(1);
    }

    return fr_tile_sets;
}

RECT get_rect(short x, short y, short w, short h) {
    RECT r = {x, y, w, h};
    logr_log(TRACE, "Map.c", "get_rect", "Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}

void map_ot_sort_bg_layers() {
    SpriteLayer *curr_sl;
    Frame *frame = &g_map_frames[g_current_frame];

    if (frame->bg_layers != NULL) {
        for (curr_sl = frame->bg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                if (CHDLR_SPRITE_IN_VIEW(curr_sl->sprites[i], g_camera)) {
                    GPUB_GS_SORT_FAST_OBJ(curr_sl->sprites[i]);
                }
            }
        }
    }

    if (frame->game_object != NULL) {
        gobj_draw(frame->game_object);
    }
}

void map_ot_sort_fg_layers() {
    SpriteLayer *curr_sl;
    Frame *frame = &g_map_frames[g_current_frame];

    if (frame->fg_layers != NULL) {
        for (curr_sl = frame->fg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                if (CHDLR_SPRITE_IN_VIEW(curr_sl->sprites[i], g_camera)) {
                    GPUB_GS_SORT_FAST_OBJ(curr_sl->sprites[i]);
                }
            }
        }
    }
}

void map_tick() {
    Frame *frame = &g_map_frames[g_current_frame];
    gobj_player_tick(g_player, &g_ctrl);
    handle_block_collision(g_player->gobj, frame);
}

void handle_block_collision(GameObject *gobj, Frame *frame) {
    CollisionBlock *blocks = frame->collision_blocks;
    u_short i;
    switch (gobj->type) {
        case GOBJ_TYPE_PLAYER:
            chdlr_collision_resolve(gobj, blocks->bounds, blocks->amount);
            break;
        case GOBJ_TYPE_NPC:
            for (i = 0; i < blocks->amount; i++) {
                if (chdlr_overlaps(gobj->bounds, blocks[i].bounds)) {
                    if (gobj->heading & (GOBJ_HEADING_RIGHT | GOBJ_HEADING_LEFT)) {
                        GOBJ_SWITCH_X_DIR(gobj);
                    } else {
                        GOBJ_SWITCH_Y_DIR(gobj);
                    }
                }
            }
            break;
    }
}