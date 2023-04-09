#ifndef PSX_SPIKE_TILED_H
#define PSX_SPIKE_TILED_H

#include "JSONParser.h"

/**
 * Make sure that passed in key=&quot;name&quot;<br>
 * Make sure that passed in val=&quot;value&quot;
 * @param key key to check
 * @param val value to check
 */
#define TILED_VALIDATE_PROP(key, val)                                                                                               \
    if (STR_NEQ(key, "name")) {                                                                                                     \
        logr_log(ERROR, "Tiled.h", "TILED_VALIDATE_PROP", "property key='name' expected here, instead was=%s, exiting...", key);    \
        exit(1);                                                                                                                    \
    }                                                                                                                               \
    if (STR_NEQ(val, "value")) {                                                                                                    \
        logr_log(ERROR, "Tiled.h", "TILED_VALIDATE_PROP", "property key='value' expected here, instead was='%s', exiting...", val); \
        exit(1);                                                                                                                    \
    }

typedef struct ObjectLayer_Bounds {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y;
    struct ObjectLayer_Bounds *next;
} ObjectLayer_Bounds;

typedef struct ObjectLayer_Teleport {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y;
    short dest_x, dest_y;
    u_char dest_frame;
    struct ObjectLayer_Teleport *next;
} ObjectLayer_Teleport;

typedef struct ObjectLayer_Dialog {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y;
    char *text;
    u_short max_chars;
    u_char n_lines;
    struct ObjectLayer_Dialog *next;
} ObjectLayer_Dialog;

typedef struct Layer_Data {
    u_short id;
    struct Layer_Data *next;
} Layer_Data;

typedef struct Tile_Set {
    u_short firstgid;
    char* source;
    struct Tile_Set *next;
} Tile_Set;

typedef struct Tile_Layer {
    Layer_Data *data;
    u_short height, width, id, x, y, prio, active_sprites_cnt;
    char *name, *type, *layer_type;
    u_char visible;
    struct Tile_Layer *next;
} Tile_Layer;

typedef struct Tile_Map {
    // Map width in n_tiles (not in pixels)
    u_short width;
    // Map height in n_tiles (not in pixels)
    u_short height;
    // Map tile width in pixels
    u_short tile_width;
    // Map tile height in pixels
    u_short tile_height;
    // Pointer to linked list of tile layers
    Tile_Layer *layers;
    // Pointer to linked list of bounds
    ObjectLayer_Bounds *bounds;
    // Pointer to linked list of teleports
    ObjectLayer_Teleport *teleports;
    // Pointer to linked list of dialogs
    ObjectLayer_Dialog  *dialogs;
    // Pointer to linked list of tilesets
    Tile_Set *tile_sets;

    u_char bounds_cnt, layers_cnt, teleports_cnt, tilesets_cnt, dialogs_cnt;
} Tile_Map;


/**
 * Populates a new Tile_Map type from JSON data received. This function allocates
 * memory and must be either freed manually or by calling #tiled_free
 * @param root root entry in the JSON Data linked list
 * @return a Tile_Map struct allocated on the heap.
 */
Tile_Map *tiled_populate_from_json(JSON_Data *root);

void tiled_print_map(u_char level, Tile_Map *map);

/**
 * Frees all the allocated memory in linked list passed in.
 * @param tm entry of linked list
 */
void tiled_free(Tile_Map *tm);

#endif //PSX_SPIKE_TILED_H
