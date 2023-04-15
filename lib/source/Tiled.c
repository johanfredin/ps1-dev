#include <stdlib.h>
#include "../header/Tiled.h"
#include "../header/Logger.h"
#include "../header/StrUtils.h"
#include "../header/MemUtils.h"

void add_tile_layers_to_map(Tile_Map *tm, JSON_Data *jobj_root);
void add_data_to_layer(Tile_Layer *layer, JSON_Data *root);
void add_additional_properties_to_layer(Tile_Layer *layer, JSON_Data *root);
void add_tile_sets_to_map(Tile_Map *tm, JSON_Data *root);
void add_object_layers_to_map(Tile_Map *tm, JSON_Data *root);
void add_teleport_layers_to_map(Tile_Map *tm, JSON_Data *root);
void add_dialog_layers_to_map(Tile_Map *tm, JSON_Data *root);
Tile_Map *malloc_tile_map();


Tile_Map *tiled_populate_from_json(JSON_Data *root) {
    if (root == NULL) {
        logr_log(ERROR, "Tiled.c", "tiled_populate_from_json", "root is NULL");
        exit(1);
    }
    Tile_Map *tm = malloc_tile_map();
    for (JSON_Data *curr = root; curr != NULL; curr = curr->next) {
        char *key = curr->key;
        void *value = curr->value;
        if (STR_EQ(key, "height")) {
            tm->height = *(int *) value;
        } else if (STR_EQ(key, "width")) {
            tm->width = *(int *) value;
        } else if (STR_EQ(key, "tileheight")) {
            tm->tile_height = *(int *) value;
        } else if (STR_EQ(key, "tilewidth")) {
            tm->tile_width = *(int *) value;
        } else if (STR_EQ(key, "layers")) {
            add_tile_layers_to_map(tm, (JSON_Data *) value);
        } else if (STR_EQ(key, "tilesets")) {
            add_tile_sets_to_map(tm, (JSON_Data *) value);
        }
    }
    return tm;
}

Tile_Map *malloc_tile_map() {
    Tile_Map *tm = MEM_MALLOC_3(Tile_Map);
    tm->width = tm->height = tm->tile_width = tm->tile_height = 0;
    tm->layers = NULL;
    tm->bounds = NULL;
    tm->teleports = NULL;
    tm->dialogs = NULL;
    tm->bounds_cnt = 0;
    tm->teleports_cnt = 0;
    tm->dialogs_cnt = 0;
    return tm;
}

void tiled_free(Tile_Map *tm) {
    while (tm->layers != NULL) {
        Tile_Layer *aux = tm->layers;
        tm->layers = tm->layers->next;
        MEM_FREE_LINKED_LIST(aux->data, Layer_Data)
        MEM_FREE_3_AND_NULL(aux);
    }
    MEM_FREE_3_AND_NULL(tm->layers);
    MEM_FREE_LINKED_LIST(tm->bounds, ObjectLayer_Bounds)
    MEM_FREE_LINKED_LIST(tm->teleports, ObjectLayer_Teleport)
    MEM_FREE_LINKED_LIST(tm->dialogs, ObjectLayer_Dialog)
    MEM_FREE_LINKED_LIST(tm->tile_sets, Tile_Set)
    MEM_FREE_3_AND_NULL(tm);
}

void tiled_print_map(u_char level, Tile_Map *map) {
    ObjectLayer_Bounds *bounds_layer;
    ObjectLayer_Teleport *teleports_layer;
    ObjectLayer_Dialog *dialog_layer;
    logr_log(level, "Tiled.c", "tiled_print_map", "Map parsed from JSON");
    logr_log(level, "Tiled.c", "tiled_print_map", "-------------------- ");
    logr_log(level, "Tiled.c", "tiled_print_map", "{ ");
    logr_log(level, "Tiled.c", "tiled_print_map", "  bounds_cnt=%d", map->bounds_cnt);
    logr_log(level, "Tiled.c", "tiled_print_map", "  teleports_cnt=%d", map->teleports_cnt);
    logr_log(level, "Tiled.c", "tiled_print_map", "  tilesets_cnt=%d", map->tilesets_cnt);
    logr_log(level, "Tiled.c", "tiled_print_map", "  width=%d ", map->width);
    logr_log(level, "Tiled.c", "tiled_print_map", "  height=%d ", map->height);
    logr_log(level, "Tiled.c", "tiled_print_map", "  tile_width=%d ", map->tile_width);
    logr_log(level, "Tiled.c", "tiled_print_map", "  tile_height=%d ", map->tile_height);
    logr_log(level, "Tiled.c", "tiled_print_map", "  layers_cnt=%d ", map->layers_cnt);

    logr_log(level, "Tiled.c", "tiled_print_map", "  layers=[ ");
    for (Tile_Layer *tile_layer = map->layers; tile_layer != NULL; tile_layer = tile_layer->next) {
        logr_log(level, "Tiled.c", "tiled_print_map", "    { ");
        logr_log(level, "Tiled.c", "tiled_print_map", "      name=%s ", tile_layer->name);
        logr_log(level, "Tiled.c", "tiled_print_map", "      type=%s ", tile_layer->type);
        logr_log(level, "Tiled.c", "tiled_print_map", "      id=%d ", tile_layer->id);
        logr_log(level, "Tiled.c", "tiled_print_map", "      x=%d ", tile_layer->x);
        logr_log(level, "Tiled.c", "tiled_print_map", "      y=%d ", tile_layer->y);
        logr_log(level, "Tiled.c", "tiled_print_map", "      width=%d ", tile_layer->width);
        logr_log(level, "Tiled.c", "tiled_print_map", "      height=%d ", tile_layer->height);
        logr_log(level, "Tiled.c", "tiled_print_map", "      visible=%d ", tile_layer->visible);
        logr_log(level, "Tiled.c", "tiled_print_map", "      layer_type=%s ", tile_layer->layer_type);
        logr_log(level, "Tiled.c", "tiled_print_map", "      prio=%d ", tile_layer->prio);
        logr_log(level, "Tiled.c", "tiled_print_map", "      active_sprites_cnt=%d ", tile_layer->active_sprites_cnt);
        logr_log(level, "Tiled.c", "tiled_print_map", "    } ");
    }
    logr_log(level, "Tiled.c", "tiled_print_map", "  ] ");
    logr_log(level, "Tiled.c", "tiled_print_map", "  bounds=[ ");
    for (bounds_layer = map->bounds; bounds_layer != NULL; bounds_layer = bounds_layer->next) {
        logr_log(level, "Tiled.c", "tiled_print_map", "    { ");
        logr_log(level, "Tiled.c", "tiled_print_map", "      id=%d ", bounds_layer->id);
        logr_log(level, "Tiled.c", "tiled_print_map", "      visible=%d ", bounds_layer->visible);
        logr_log(level, "Tiled.c", "tiled_print_map", "      x=%d ", bounds_layer->x);
        logr_log(level, "Tiled.c", "tiled_print_map", "      y=%d ", bounds_layer->y);
        logr_log(level, "Tiled.c", "tiled_print_map", "      width=%d ", bounds_layer->width);
        logr_log(level, "Tiled.c", "tiled_print_map", "      height=%d ", bounds_layer->height);
        logr_log(level, "Tiled.c", "tiled_print_map", "      visible=%d ", bounds_layer->visible);
        logr_log(level, "Tiled.c", "tiled_print_map", "    } ");
    }
    logr_log(level, "Tiled.c", "tiled_print_map", "  ] ");
    logr_log(level, "Tiled.c", "tiled_print_map", "  teleports=[ ");
    for (teleports_layer = map->teleports; teleports_layer != NULL; teleports_layer = teleports_layer->next) {
        logr_log(level, "Tiled.c", "tiled_print_map", "    { ");
        logr_log(level, "Tiled.c", "tiled_print_map", "      id=%d ", teleports_layer->id);
        logr_log(level, "Tiled.c", "tiled_print_map", "      visible=%d ", teleports_layer->visible);
        logr_log(level, "Tiled.c", "tiled_print_map", "      x=%d ", teleports_layer->x);
        logr_log(level, "Tiled.c", "tiled_print_map", "      y=%d ", teleports_layer->y);
        logr_log(level, "Tiled.c", "tiled_print_map", "      width=%d ", teleports_layer->width);
        logr_log(level, "Tiled.c", "tiled_print_map", "      height=%d ", teleports_layer->height);
        logr_log(level, "Tiled.c", "tiled_print_map", "      dest_frame=%d ", teleports_layer->dest_frame);
        logr_log(level, "Tiled.c", "tiled_print_map", "      dest_x=%d ", teleports_layer->dest_x);
        logr_log(level, "Tiled.c", "tiled_print_map", "      dest_y=%d ", teleports_layer->dest_y);
        logr_log(level, "Tiled.c", "tiled_print_map", "    } ");
    }
    logr_log(level, "Tiled.c", "tiled_print_map", "  ] ");
    logr_log(level, "Tiled.c", "tiled_print_map", "  dialogs=[ ");
    for (dialog_layer = map->dialogs; dialog_layer != NULL; dialog_layer = dialog_layer->next) {
        logr_log(level, "Tiled.c", "tiled_print_map", "    { ");
        logr_log(level, "Tiled.c", "tiled_print_map", "      id=%d ", dialog_layer->id);
        logr_log(level, "Tiled.c", "tiled_print_map", "      visible=%d ", dialog_layer->visible);
        logr_log(level, "Tiled.c", "tiled_print_map", "      x=%d ", dialog_layer->x);
        logr_log(level, "Tiled.c", "tiled_print_map", "      y=%d ", dialog_layer->y);
        logr_log(level, "Tiled.c", "tiled_print_map", "      width=%d ", dialog_layer->width);
        logr_log(level, "Tiled.c", "tiled_print_map", "      height=%d ", dialog_layer->height);
        logr_log(level, "Tiled.c", "tiled_print_map", "      text=%s ", dialog_layer->text);
        logr_log(level, "Tiled.c", "tiled_print_map", "      max_chars=%d ", dialog_layer->max_chars);
        logr_log(level, "Tiled.c", "tiled_print_map", "      n_lines=%d ", dialog_layer->n_lines);
        logr_log(level, "Tiled.c", "tiled_print_map", "    } ");
    }
    logr_log(level, "Tiled.c", "tiled_print_map", "  ] ");
    logr_log(level, "Tiled.c", "tiled_print_map", "  tile_sets=[ ");
    for (Tile_Set *tile_set = map->tile_sets; tile_set != NULL; tile_set = tile_set->next) {
        logr_log(level, "Tiled.c", "tiled_print_map", "    { ");
        logr_log(level, "Tiled.c", "tiled_print_map", "       firstgid: %d", tile_set->firstgid);
        logr_log(level, "Tiled.c", "tiled_print_map", "       source: %s", tile_set->source);
        logr_log(level, "Tiled.c", "tiled_print_map", "    } ");
    }
    logr_log(level, "Tiled.c", "tiled_print_map", "  ] ");

    logr_log(level, "Tiled.c", "tiled_print_map", "} ");
}

void add_tile_layers_to_map(Tile_Map *tm, JSON_Data *jobj_root) {
    Tile_Layer *tl_root = MEM_MALLOC_3(Tile_Layer);
    u_char layers_cnt = 1;  // We have already encountered a layer if we're in here so at least we know there is one.
    JSON_Data *jobj_curr;
    Tile_Layer *tl_curr, *tl_prev;

    for (jobj_curr = jobj_root, tl_curr = tl_root; jobj_curr != NULL; jobj_curr = jobj_curr->next) {
        JSON_Data *curr_layer;
        Tile_Layer *next = NULL;
        for (curr_layer = (JSON_Data *) jobj_curr->value; curr_layer != NULL; curr_layer = curr_layer->next) {
            char *key = curr_layer->key;
            void *value = curr_layer->value;

            if (STR_EQ(key, "data")) {
                add_data_to_layer(tl_curr, (JSON_Data *) value);
            } else if (STR_EQ(key, "height")) {
                tl_curr->height = *(u_short *) value;
            } else if (STR_EQ(key, "id")) {
                tl_curr->id = *(u_short *) value;
            } else if (STR_EQ(key, "name")) {
                u_char is_bounds = STR_EQ((char *) value, "bounds");
                u_char is_teleports = STR_EQ((char *) value, "teleports");
                u_char is_dialogs = STR_EQ((char *) value, "dialogs");
                if (is_bounds | is_teleports | is_dialogs) {
                    // FF until we reach objects array
                    while (STR_NEQ(curr_layer->key, "objects")) {
                        curr_layer = curr_layer->next;
                    }
                }
                // Value should now be at objects root
                value = curr_layer->value;  // Sync
                if (is_bounds) {
                    add_object_layers_to_map(tm, (JSON_Data *) value);
                    MEM_FREE_3_AND_NULL(tl_curr);
                    tl_curr = tl_prev;
                } else if (is_teleports) {
                    add_teleport_layers_to_map(tm, (JSON_Data *) value);
                    MEM_FREE_3_AND_NULL(tl_curr);
                    tl_curr = tl_prev;
                } else if (is_dialogs) {
                    add_dialog_layers_to_map(tm, (JSON_Data *) value);
                    MEM_FREE_3_AND_NULL(tl_curr);
                    tl_curr = tl_prev;
                } else {
                    tl_curr->name = (char *) value;
                }
            } else if (STR_EQ(key, "type")) {
                tl_curr->type = (char *) value;
            } else if (STR_EQ(key, "visible")) {
                tl_curr->visible = *(u_char *) value;
            } else if (STR_EQ(key, "width")) {
                tl_curr->width = *(u_short *) value;
            } else if (STR_EQ(key, "x")) {
                tl_curr->x = *(u_short *) value;
            } else if (STR_EQ(key, "y")) {
                tl_curr->y = *(u_short *) value;
            } else if (STR_EQ(key, "properties")) {
                add_additional_properties_to_layer(tl_curr, (JSON_Data *) value);
            }
        }

        if (jobj_curr->next != NULL) {
            layers_cnt++;
            next = MEM_MALLOC_3(Tile_Layer);
        }
        tl_curr->next = next;
        tl_prev = tl_curr;
        tl_curr = next;

    }
    tm->layers = tl_root;
    tm->layers_cnt = layers_cnt;
}

void add_object_layers_to_map(Tile_Map *tm, JSON_Data *root) {
    u_char objects_cnt;
    JSON_Data *curr_obj_layer; // Our current json object being iterated
    ObjectLayer_Bounds *ol_root = MEM_MALLOC_3(ObjectLayer_Bounds);
    ObjectLayer_Bounds *ol_curr = ol_root;
    for (curr_obj_layer = root, objects_cnt = 0;
         curr_obj_layer != NULL; curr_obj_layer = curr_obj_layer->next, objects_cnt++) {    // Iterate objects
        JSON_Data *entry_root = (JSON_Data *) curr_obj_layer->value;
        for (JSON_Data *entry_curr = entry_root;
             entry_curr != NULL; entry_curr = entry_curr->next) { // Iterate object properties
            char *key = entry_curr->key;
            void *value = entry_curr->value;

            if (STR_EQ(key, "height")) {
                ol_curr->height = *(u_int *) value;
            } else if (STR_EQ(key, "id")) {
                ol_curr->id = *(u_int *) value;
            } else if (STR_EQ(key, "visible")) {
                ol_curr->visible = *(u_char *) value;
            } else if (STR_EQ(key, "width")) {
                ol_curr->width = *(u_int *) value;
            } else if (STR_EQ(key, "x")) {
                ol_curr->x = *(u_int *) value;
            } else if (STR_EQ(key, "y")) {
                ol_curr->y = *(u_int *) value;
            }
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr_obj_layer, ol_curr, ObjectLayer_Bounds)
    }
    tm->bounds = ol_root;
    tm->bounds_cnt = objects_cnt;
}

void add_teleport_layers_to_map(Tile_Map *tm, JSON_Data *root) {
    u_char objects_cnt;
    ObjectLayer_Teleport *ol_root, *ol_curr; // Our object sprite_layers linked list
    JSON_Data *curr_obj_layer; // Our current json object being iterated
    ol_root = MEM_MALLOC_3(ObjectLayer_Teleport);
    ol_curr = ol_root;
    for (
            curr_obj_layer = root, objects_cnt = 0;
            curr_obj_layer != NULL;
            curr_obj_layer = curr_obj_layer->next, objects_cnt++
    ) {
        JSON_Data *entry_root = (JSON_Data *) curr_obj_layer->value;
        JSON_Data *entry_curr;
        for (entry_curr = entry_root; entry_curr != NULL; entry_curr = entry_curr->next) { // Iterate object properties
            char *key = entry_curr->key;
            void *value = entry_curr->value;

            if (STR_EQ(key, "height")) {
                ol_curr->height = *(u_int *) value;
            } else if (STR_EQ(key, "id")) {
                ol_curr->id = *(u_int *) value;
            } else if (STR_EQ(key, "visible")) {
                ol_curr->visible = *(u_char *) value;
            } else if (STR_EQ(key, "width")) {
                ol_curr->width = *(u_int *) value;
            } else if (STR_EQ(key, "x")) {
                ol_curr->x = *(u_int *) value;
            } else if (STR_EQ(key, "y")) {
                ol_curr->y = *(u_int *) value;
            } else if (STR_EQ(key, "properties")) {
                JSON_Data *props_root = (JSON_Data *) entry_curr->value;

                // Init dest properties to 0 to prevent garbage
                ol_curr->dest_x = 0;
                ol_curr->dest_y = 0;
                ol_curr->dest_frame = 0;

                for (JSON_Data *props_curr = props_root; props_curr != NULL; props_curr = props_curr->next) {
                    JSON_Data *teleport_property_obj = (JSON_Data *) props_curr->value;
                    char *prop_name = (char *) teleport_property_obj->value;
                    short prop_value = *(short *) teleport_property_obj->next->next->value;

                    TILED_VALIDATE_PROP(teleport_property_obj->key, teleport_property_obj->next->next->key)

                    if (STR_EQ(prop_name, "dest_frame")) {
                        ol_curr->dest_frame = prop_value;
                    } else if (STR_EQ(prop_name, "dest_x")) {
                        ol_curr->dest_x = prop_value;
                    } else if (STR_EQ(prop_name, "dest_y")) {
                        ol_curr->dest_y = prop_value;
                    }
                }
            }
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr_obj_layer, ol_curr, ObjectLayer_Teleport)
    }
    tm->teleports = ol_root;
    tm->teleports_cnt = objects_cnt;
}

void add_dialog_layers_to_map(Tile_Map *tm, JSON_Data *root) {
    u_char objects_cnt;
    JSON_Data *curr_obj_layer; // Our current json object being iterated
    ObjectLayer_Dialog *ol_root = MEM_MALLOC_3(ObjectLayer_Dialog);
    ObjectLayer_Dialog *ol_curr = ol_root;
    for (curr_obj_layer = root, objects_cnt = 0;
         curr_obj_layer != NULL;
         curr_obj_layer = curr_obj_layer->next,
         objects_cnt++
    ) {
        JSON_Data *entry_root = (JSON_Data *) curr_obj_layer->value;
        for (JSON_Data *entry_curr = entry_root; entry_curr != NULL; entry_curr = entry_curr->next) { // Iterate object properties
            char *key = entry_curr->key;
            void *value = entry_curr->value;

            if (STR_EQ(key, "height")) {
                ol_curr->height = *(u_int *) value;
            } else if (STR_EQ(key, "id")) {
                ol_curr->id = *(u_int *) value;
            } else if (STR_EQ(key, "visible")) {
                ol_curr->visible = *(u_char *) value;
            } else if (STR_EQ(key, "width")) {
                ol_curr->width = *(u_int *) value;
            } else if (STR_EQ(key, "x")) {
                ol_curr->x = *(u_int *) value;
            } else if (STR_EQ(key, "y")) {
                ol_curr->y = *(u_int *) value;
            } else if (STR_EQ(key, "properties")) {
                // Init dest properties to prevent garbage
                ol_curr->text = NULL;
                ol_curr->max_chars = 0;
                ol_curr->n_lines = 1;

                JSON_Data *props_root = (JSON_Data *) entry_curr->value;
                for (JSON_Data *props_curr = props_root; props_curr != NULL; props_curr = props_curr->next) {
                    JSON_Data *teleport_property_obj = (JSON_Data *) props_curr->value;
                    char *prop_name = (char *) teleport_property_obj->value;
                    void *prop_value = teleport_property_obj->next->next->value;

                    TILED_VALIDATE_PROP(teleport_property_obj->key, teleport_property_obj->next->next->key)

                    if (STR_EQ(prop_name, "max_chars")) {
                        ol_curr->max_chars = *(u_short *) prop_value;
                    } else if (STR_EQ(prop_name, "n_lines")) {
                        ol_curr->n_lines = *(u_char *) prop_value;
                    } else if (STR_EQ(prop_name, "text")) {
                        ol_curr->text = (char *) prop_value;
                    }
                }
            }
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr_obj_layer, ol_curr, ObjectLayer_Dialog)
    }
    tm->dialogs = ol_root;
    tm->dialogs_cnt = objects_cnt;
}

void add_additional_properties_to_layer(Tile_Layer *layer, JSON_Data *root) {
    // Iterate properties array
    for (JSON_Data *curr = root; curr != NULL; curr = curr->next) {
        JSON_Data *curr_props = (JSON_Data *) curr->value;

        // Fetch the 2 properties needed in json object
        char *prop_name = (char *) curr_props->value;
        void *prop_value = curr_props->next->next->value;

        // Validate that the property prop_value is actually 2 lanes down
        TILED_VALIDATE_PROP(curr_props->key, curr_props->next->next->key)

        if (STR_EQ(prop_name, "layer_type")) {
            layer->layer_type = (char *) prop_value;
        } else if (STR_EQ(prop_name, "prio")) {
            layer->prio = *(u_short *) prop_value;
        }
    }
}

void add_data_to_layer(Tile_Layer *layer, JSON_Data *root) {
    Layer_Data *data_root = MEM_MALLOC_3(Layer_Data);
    u_short active_sprites_cnt = 0;
    Layer_Data *data = data_root;
    for (JSON_Data *curr = root; curr != NULL; curr = curr->next) {
        data->id = *(u_short *) curr->value;
        if (data->id > 0) {
            active_sprites_cnt++;
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr, data, Layer_Data)
    }
    layer->active_sprites_cnt = active_sprites_cnt;
    layer->data = data_root;
}

void add_tile_sets_to_map(Tile_Map *tm, JSON_Data *root) {
    u_char tilesets_cnt;
    JSON_Data *json_curr;
    Tile_Set *ts_root = MEM_MALLOC_3(Tile_Set);
    Tile_Set *ts_curr = ts_root;
    for (
            json_curr = root, tilesets_cnt = 0;
            json_curr != NULL;
            json_curr = json_curr->next, tilesets_cnt++
    ) {
        for (JSON_Data *json_tileset = (JSON_Data *) json_curr->value; json_tileset != NULL; json_tileset = json_tileset->next) {
            if (STR_EQ(json_tileset->key, "firstgid")) {
                ts_curr->firstgid = *(u_short *) json_tileset->value;
            } else if (STR_EQ(json_tileset->key, "source")) {
                ts_curr->source = (char *) json_tileset->value;
            }
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(json_curr, ts_curr, Tile_Set)
    }
    tm->tilesets_cnt = tilesets_cnt;
    tm->tile_sets = ts_root;
}