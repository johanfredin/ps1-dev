#include "../header/Frames.h"
#include "../header/MemUtils.h"

void insert_beginning(SpriteLayer **root, SpriteLayer *new_layer) {
    new_layer->next = *root;
    *root = new_layer;
}

void insert_after(SpriteLayer *layer, SpriteLayer *new_layer) {
    new_layer->next = layer->next;
    layer->next = new_layer;
}

void frames_insert_sl_sorted(SpriteLayer **root, SpriteLayer *new_layer) {
    SpriteLayer *curr;
    if (*root == NULL || (*root)->prio >= new_layer->prio) {
        insert_beginning(root, new_layer);
        return;
    }

    for (curr = *root; curr->next != NULL; curr = curr->next) {
        if (curr->next->prio >= new_layer->prio) {
            break;
        }
    }
    insert_after(curr, new_layer);
}

FR_TileSet *frames_malloc_fr_tileset() {
    FR_TileSet *ts = MEM_MALLOC_3(FR_TileSet);
    ts->source = NULL;
    ts->sprite = NULL;
    ts->start_id = 0;
    return ts;
}