#include <stdlib.h>
#include "../header/TileFetcher.h"
#include "../header/MemUtils.h"
#include "../header/StrUtils.h"
#include "../header/Logger.h"

SpriteLayer *init_sprite_layer();
u_short to_tm_u_coord(u_short id, u_short tile_w);
u_short to_tm_v_coord(u_short id, u_short tile_h);
GsSPRITE *map_tile(u_short id, u_short x, u_short y, FR_TileSet *tile_sets, u_char n_tilesets, Tile_Map *map);

void tf_add_layers_to_frame(Frame *frame, FR_TileSet *tile_sets, u_char n_tilesets, Tile_Map *map) {
    SpriteLayer *root_bg_layer = NULL;
    SpriteLayer *root_fg_layer = NULL;

    logr_log(INFO, "TileFetcher.c", "tl_get_layers", "Entered function");

    // Iterate map tile layers
    for (Tile_Layer *tl_curr = map->layers; tl_curr != NULL; tl_curr = tl_curr->next) {
        u_short curr_col = 0, tiles_cnt = 0, rows_cnt = 0, active_tiles_cnt = 0;
        SpriteLayer *sl = init_sprite_layer();
        GsSPRITE **layer_sprites = MEM_CALLOC_3_PTRS(tl_curr->active_sprites_cnt, GsSPRITE);

        if (tl_curr->layer_type == NULL) {
            logr_log(ERROR, "TileFetcher.c", "tf_add_layers_to_frame", "Layer type must not be NULL, exiting...");
            exit(1);
        }

        logr_log(INFO, "TileFetcher.c", "tl_get_layers", "FETCHING LAYER %s, active_sprites_count=%d", tl_curr->name, tl_curr->active_sprites_cnt);

        // Iterate the layer data to get tileset position for each tile
        for (Layer_Data *ld_curr = tl_curr->data; ld_curr != NULL; ld_curr = ld_curr->next) {
            u_short id = ld_curr->id;

            // Check if time for new row
            if (curr_col >= map->width) {
                curr_col = 0;
                rows_cnt += 1;
            }

            if (id == 0) {
                logr_log(TRACE, "TileFetcher.c", "tl_get_layers", "skipping null tile at index=%d", tiles_cnt);
            } else {
                GsSPRITE *sprite;
                u_short x = frame->offset_x + (curr_col * map->tile_width);
                u_short y = frame->offset_y + (rows_cnt * map->tile_height);

                --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed

                sprite = map_tile(id, x, y, tile_sets, n_tilesets, map);
                if (sprite == NULL) {
                    logr_log(ERROR, "TileFetcher.c", "tl_get_layers", "Id of col=%d at layer=%s exceeds max dimensions for tilesets, terminating...", id, tl_curr->layer_type);
                    exit(1);
                }
                layer_sprites[active_tiles_cnt++] = sprite;
            }
            tiles_cnt++;
            curr_col++;
        }

        sl->sprites = layer_sprites;
        sl->sprites_cnt = active_tiles_cnt;
        sl->prio = tl_curr->prio;
        if (STR_EQ(tl_curr->layer_type, "bg")) {
            frames_insert_sl_sorted(&root_bg_layer, sl);
            logr_log(DEBUG, "TileFetcher.c", "tf_add_layers_to_frame", "Bg layer with prio=%d added to frame", sl->prio);
        } else if (STR_EQ(tl_curr->layer_type, "fg")) {
            frames_insert_sl_sorted(&root_fg_layer, sl);
            logr_log(DEBUG, "TileFetcher.c", "tf_add_layers_to_frame", "Fg layer with prio=%d added to frame", sl->prio);
        } else {
            logr_log(ERROR, "TileFetcher.c", "tf_add_layers_to_frame", "layer type %d not a valid type, must be either bg or fg, exiting...", tl_curr->layer_type);
            exit(1);
        }
    }

    // Finally, give layers to frame
    frame->bg_layers = root_bg_layer;
    frame->fg_layers = root_fg_layer;
}

GsSPRITE *map_tile(u_short id, u_short x, u_short y, FR_TileSet *tile_sets, u_char n_tilesets, Tile_Map *map) {
    // Iterate our fr_tilesets
    for(u_char i = 0; i < n_tilesets; i++) {
        FR_TileSet *tf_tileset = &tile_sets[i];
        GsSPRITE *base = tf_tileset->sprite;

        u_short ts_tw = map->tile_width;
        u_short ts_th = map->tile_height;
        u_short ts_start_id = tf_tileset->start_id - 1; // Same as before, ids are 1 indexed in json but actually 0 indexed in Tiled editor, 0 in json means null, and we have already covered that before
        u_short max_id = ts_start_id + (ts_tw * ts_th);

        if(id >= ts_start_id && id <= max_id) {
            GsSPRITE *tile_sprite = MEM_MALLOC_3(GsSPRITE);
            u_short adapted_id = id - ts_start_id;     // We need to subtract the start id from the tileset, so it maps correctly within the tileset image
            u_short u = to_tm_u_coord(adapted_id, map->tile_width);
            u_short v = to_tm_v_coord(adapted_id, map->tile_height);

            asmg_load_sprite_region(base, tile_sprite, x, y, u, v, map->tile_width, map->tile_height);
            LOGR_LOG_GS_OBJ(DEBUG, tile_sprite);
            return tile_sprite;
        }
    }

    return NULL; // Return NULL if we have exceeded all tilesets and still not within range
}

u_short to_tm_u_coord(u_short id, u_short tile_w) {
    while (id >= tile_w) {
        id -= tile_w;
    }
    return id * tile_w;
}

u_short to_tm_v_coord(u_short id, u_short tile_h) {
    u_short v = 0;
    while (id >= tile_h) {
        v++;
        id -= tile_h;
    }
    return v * tile_h;
}

SpriteLayer *init_sprite_layer() {
    SpriteLayer *sl = MEM_MALLOC_3(SpriteLayer);
    sl->sprites = NULL;
    sl->next = NULL;
    sl->sprites_cnt = 0;
    sl->prio = 0;
    return sl;
}
