#include "../../lib/header/Map.h"
#include "../../lib/header/AssetManager.h"
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/MemUtils.h"
#include "../../lib/header/TileFetcher.h"
#include "../../lib/header/StrUtils.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/CollisionHandler.h"

#include <libetc.h>

#define MAP_START_FRAME 0
#define MAP_DIALOGS_IN_FRAME(dlg_cnt) dlg_cnt | 0

#define TRACK_WORLD 2
#define TRACK_INDOORS 3
#define CDDA_TRACKS 2

Frame *g_map_frames;
FR_TileSet **g_map_tile_sets;
CdrData **g_map_cdr_data_assets;
Font *g_fnt;
VerticalGoraudColor *g_canvas_clr;
DPad g_ctrl;

Player *g_player;
Camera *g_camera;

u_char g_assets_cnt = 0, g_map_tilesets_count = 0;
u_char g_frame_cnt;
u_char g_current_frame = MAP_START_FRAME;

CdrDATrack tracks[CDDA_TRACKS] = {
        {TRACK_WORLD,   0},
        {TRACK_INDOORS, 0}
};

void init_frame(Frame *frame, char *json_map_file, CdrDATrack *audio_track);

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
    load_level_assets_from_cd(level);
    load_tilesets();

    // Load font and dialog bg color
    g_fnt = txt_fnt_init("FONT.TIM", 8, 8, TXT_FNT_PADDING_DEFAULT);
    g_canvas_clr = MEM_MALLOC_3(VerticalGoraudColor);
    TBX_INIT_VERT_GORAUD_COLOR(g_canvas_clr, 10, 10, 40, 0, 0, 200 - 40);

    g_map_frames = MEM_CALLOC_3(4, Frame);
    init_frame(&g_map_frames[g_frame_cnt++], "L1_WRLD.JSON", &tracks[0]);
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN1.JSON", &tracks[1]);
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN2.JSON", &tracks[1]);
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN3.JSON", &tracks[1]);

    // Init controller
    DPAD_INIT(&g_ctrl);

    // Cleanup
    for (u_char i = 0; i < g_assets_cnt; i++) {
        CDR_DATA_FREE(g_map_cdr_data_assets[i]);
    }
    MEM_FREE_3_AND_NULL(g_map_cdr_data_assets);
    g_camera = camera;
    g_player = player;

    // Start initial track
    cdr_da_play(&tracks[0]);
}

void load_level_assets_from_cd(u_char level) {
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "*********************************");
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "* ADDING ASSETS FOR LEVEL NR %d *", level);
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "*********************************");
    if (level == 1) {
        g_map_cdr_data_assets = MEM_CALLOC_3_PTRS(9, CdrData);
        // Load tile set images
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TR.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BR.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_IN.TIM");

        // Load tile map json files
        g_assets_cnt += g_map_tilesets_count;
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("L1_WRLD.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN1.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN2.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN3.JSON");
    }
    logr_log(DEBUG, "Map.c", "load_level_assets_from_cd", "%d assets loaded", g_assets_cnt);
}

/**
 * Populates our global array of FR_TileSet structs from the tileset CdrData assets.
 * Retrieve the GS_SPRITE and the asset name.
 */
void load_tilesets() {
    g_map_tile_sets = MEM_CALLOC_3_PTRS(g_map_tilesets_count, FR_TileSet);
    for (u_char i = 0; i < g_map_tilesets_count; i++) {
        GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
        g_map_tile_sets[i] = frames_malloc_fr_tileset();
        asmg_load_sprite(sprite, g_map_cdr_data_assets[i], 0, 0, BIT_DEPTH_8);
        g_map_tile_sets[i]->sprite = sprite;
        g_map_tile_sets[i]->source = g_map_cdr_data_assets[i]->name;
        logr_log(TRACE, "Map.c", "load_tilesets", "tileset=%s loaded", g_map_cdr_data_assets[i]->name);
    }
    logr_log(DEBUG, "Map.c", "load_tilesets", "%d tilesets loaded", g_map_tilesets_count);
}

void init_frame(Frame *frame, char *json_map_file, CdrDATrack *audio_track) {
    // Parse json file into tile map
    CdrData *json_cdr_data = cdr_find_data_entry(json_map_file, g_map_cdr_data_assets, g_assets_cnt);
    logr_log(INFO, "Map.c", "init_frame", "JSON file=%s retrieved", json_map_file);
    u_long *content = json_cdr_data->file;
    JSON_Data *json_map_data = jsonp_parse((char *) content);
    Tile_Map *tile_map = tiled_populate_from_json(json_map_data);
    tiled_print_map(DEBUG, tile_map);
    frame->fr_tilesets = transfer_to_frame_tileset(tile_map);

    // Calc potential x and/or y offsets (e.g frame is smaller than screen w and/or h)
    u_short frame_w = tile_map->width * tile_map->tile_width;
    u_short frame_h = tile_map->height * tile_map->tile_height;

    u_char offset_x = 0, offset_y = 0;
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
    frame_init_teleports(tile_map, frame);
    frame_init_dialogs(tile_map, frame);

    // Set frame audio track
    frame->audio_track = audio_track;

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
    for (
            i = 0, curr_b = tile_map->bounds;
            curr_b != NULL;
            i++, curr_b = curr_b->next
            ) {
        u_short x = curr_b->x + frame->offset_x;
        u_short y = curr_b->y + frame->offset_y;
        collision_blocks->bounds[i] = get_rect(x, y, curr_b->width, curr_b->height);
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;
}

void frame_init_teleports(Tile_Map *tile_map, Frame *frame) {
    int i;
    ObjectLayer_Teleport *curr_t;
    Teleport *teleports;
    u_char teleports_cnt = tile_map->teleports_cnt;
    teleports = MEM_CALLOC_3(teleports_cnt, Teleport);
    for (
            i = 0, curr_t = tile_map->teleports;
            curr_t != NULL;
            i++, curr_t = curr_t->next
            ) {
        u_short x = curr_t->x + frame->offset_x;
        u_short y = curr_t->y + frame->offset_y;
        teleports[i].origin = get_rect(x, y, curr_t->width, curr_t->height);
        teleports[i].dest_x = curr_t->dest_x;
        teleports[i].dest_y = curr_t->dest_y;
        teleports[i].dest_frame = curr_t->dest_frame;
    }
    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;
}

void frame_init_dialogs(Tile_Map *tile_map, Frame *frame) {
    u_char dialogs_cnt = tile_map->dialogs_cnt;
    if (MAP_DIALOGS_IN_FRAME(dialogs_cnt)) {  // Load dialogs from tile map if present
        int i;
        ObjectLayer_Dialog *curr_d;
        FR_Dialog *dialogs;
        dialogs = MEM_CALLOC_3(dialogs_cnt, FR_Dialog);
        dialogs->content = NULL;
        for (
                i = 0, curr_d = tile_map->dialogs;
                curr_d != NULL;
                i++, curr_d = curr_d->next
                ) {
            char **strs = (char **) MEM_MALLOC_3_STRS(strlen(curr_d->text));
            char *token = strtok(curr_d->text, ";");
            u_short x = curr_d->x + frame->offset_x;
            u_short y = curr_d->y + frame->offset_y;
            short dlg_x = 20, dlg_y = 40;
            short dlg_w, dlg_target_h;
            u_char n_messages;
            for (n_messages = 0; token != NULL; n_messages++) {
                size_t n = strlen(token) + 1;
                strs[n_messages] = (char *) MEM_MALLOC_3_STRS(n);
                memcpy(strs[n_messages], token, n);
                token = strtok(NULL, ";");
            }

            dialogs[i].bounds = get_rect(x, y, curr_d->width, curr_d->height);
            dialogs[i].n_lines = curr_d->n_lines;
            dialogs[i].max_chars = curr_d->max_chars;
            dialogs[i].initialized = 0;

            // Create a dialog struct from acquired properties in tiled map editor.
            dlg_w = ((curr_d->max_chars * (g_fnt->cw + g_fnt->padding)) + (g_fnt->padding * 2));
            dlg_target_h = ((curr_d->n_lines * (g_fnt->ch + g_fnt->padding)) + (g_fnt->padding * 4));

            // Now we have the message tokens, lets create a dialog
            Dialog *dialog = txt_dlg_init(
                    strs, NULL, n_messages, g_fnt, 3,
                    dlg_x + (g_fnt->padding * 2), dlg_y + (g_fnt->padding * 2), 0
            );
            DlgBox *dlgBox = tbx_init_dlg_box(dlg_x, dlg_y, dlg_w, 0, dlg_w, dlg_target_h, g_canvas_clr, dialog);

            // Assign to array
            dialogs[i].content = dlgBox;
            logr_log(INFO, "Map.c", "init_frame", "Dialog n_lines=%d, max_chars=%d, n_messages=%d",
                     dialogs[i].n_lines, dialogs[i].max_chars, n_messages);

            frame->dialogs = dialogs;
            frame->d_amount = dialogs_cnt;

            // remove allocated strings, no longer needed
            MEM_FREE_STRS(strs, n_messages);
        }
    } else {
        frame->dialogs = NULL; // set to NULL since there can be frames without dialogs
    }
}

FR_TileSet *transfer_to_frame_tileset(Tile_Map *map) {
    FR_TileSet *fr_tile_sets = MEM_CALLOC_3(map->tilesets_cnt, FR_TileSet);
    u_char match_cnt = 0;

    // Iterate tilesets that the tile map is using
    for (Tile_Set *curr_ts = map->tile_sets; curr_ts != NULL; curr_ts = curr_ts->next) {
        // Iterate the map FR_Tileset array to look for a matching image
        for (u_char i = 0; i < g_map_tilesets_count; i++) {
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
    Frame *frame = &g_map_frames[g_current_frame];
    if (frame->bg_layers != NULL) {
        for (SpriteLayer *curr_sl = frame->bg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            for (u_short i = 0; i < curr_sl->sprites_cnt; i++) {
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
    Frame *frame = &g_map_frames[g_current_frame];
    if (frame->fg_layers != NULL) {
        for (SpriteLayer *curr_sl = frame->fg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            for (u_short i = 0; i < curr_sl->sprites_cnt; i++) {
                if (CHDLR_SPRITE_IN_VIEW(curr_sl->sprites[i], g_camera)) {
                    GPUB_GS_SORT_FAST_OBJ(curr_sl->sprites[i]);
                }
            }
        }
    }
}

void map_ot_sort_hud_layer() {
    Frame *frame = &g_map_frames[g_current_frame];
    if (frame->dialogs != NULL) {
        for (u_char i = 0; i < frame->d_amount; i++) {
            tbx_draw(frame->dialogs[i].content);
        }
    }
}

void map_tick() {
    Frame *frame = &g_map_frames[g_current_frame];
    gobj_player_tick(g_player, &g_ctrl);
    handle_teleport_collision(g_player->gobj, frame);
    handle_block_collision(g_player->gobj, frame);
    handle_dialog_collision(g_player->gobj, frame);
    gobj_anim_tick(g_player->anim, g_player->gobj);

    // Update potential game objects in frame (other than g_player)
    if (frame->game_object != NULL) {
        gobj_tick(frame->game_object);
        handle_block_collision(frame->game_object, frame);
        handle_teleport_collision(frame->game_object, frame);
    }
}

void handle_block_collision(GameObject *gobj, Frame *frame) {
    CollisionBlock *blocks = frame->collision_blocks;
    switch (gobj->type) {
        case GOBJ_TYPE_PLAYER:
            chdlr_collision_resolve(gobj, blocks->bounds, blocks->amount);
            break;
        case GOBJ_TYPE_NPC:
            for (u_short i = 0; i < blocks->amount; i++) {
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

void handle_teleport_collision(GameObject *gobj, Frame *frame) {
    Teleport *teleports = frame->teleports;
    for (u_char i = 0; i < frame->t_amount; i++) {
        Teleport t = teleports[i];
        if (chdlr_overlaps(gobj->bounds, &t.origin)) {
            g_current_frame = t.dest_frame; // Update curr frame to dest frame so that we can get its offsets (if any)
            if (t.dest_x > 0) {
                gobj->sprite->x = (t.dest_x + g_map_frames[g_current_frame].offset_x);
            }
            if (t.dest_y > 0) {
                gobj->sprite->y = (t.dest_y + g_map_frames[g_current_frame].offset_y);
            }
            g_camera->map_w = g_map_frames[g_current_frame].width;
            g_camera->map_h = g_map_frames[g_current_frame].height;

            // Swap audio track
            CdrDATrack *current_track = frame->audio_track;
            CdrDATrack *next_track = g_map_frames[t.dest_frame].audio_track;
            cdr_da_swap(current_track, next_track);

            if (g_camera->map_w <= g_camera->viewport_w) {
                g_camera->map_w = gpub_screen_w;
                g_camera->viewport_w = gpub_screen_w;
                g_camera->x = 0;
            }
            if (g_camera->map_h <= g_camera->viewport_h) {
                g_camera->map_h = gpub_screen_h;
                g_camera->viewport_h = gpub_screen_h;
                g_camera->y = 0;
            }
        }
    }
}

void handle_dialog_collision(GameObject *gobj, Frame *frame) {
    if (frame->dialogs != NULL) {
        FR_Dialog *dialogs = frame->dialogs;
        for (u_char i = 0; i < frame->d_amount; i++) {
            FR_Dialog *d = &dialogs[i];
            if (chdlr_overlaps(gobj->bounds, &d->bounds)) {
                DPAD_READ_INPUT(&g_ctrl);
                if (DPAD_BTN_INTERACT(&g_ctrl)) {
                    d->content->complete = 0;
                    d->initialized = 1;
                    d->content->started = 1;
                    d->content->dlg->visible = 1;
                    gobj->can_move = 0;
                }
                if (d->content->complete) {
                    gobj->can_move = 1;
                    d->initialized = 0;
                    tbx_reset_dgl_box(d->content);
                }
            }
            if (d->initialized) {
                tbx_tick(d->content, &g_ctrl);
            }
        }
    }
}