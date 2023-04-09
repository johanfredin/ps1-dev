#ifndef PSX_FRAMES_H
#define PSX_FRAMES_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include "GameObject.h"
#include "TextBox.h"

typedef struct Teleport {
    RECT origin;
    u_short dest_x, dest_y;
    // frame array index of frame to change to when colliding with this teleport
    u_char dest_frame;
} Teleport;

typedef struct CollisionBlock {
    // The actual physical bounds that we will collide with in the frame
    RECT *bounds;
    // The amount of blocks on one frame
    u_char amount;
} CollisionBlock;

typedef struct FR_Dialog {
    // If we collide with these bounds and press interact button then dialog can play (if not playing already)
    RECT bounds;
    u_short max_chars;
    u_char n_lines: 7;
    u_char initialized: 1;
    DlgBox *content;
} FR_Dialog;

typedef struct SpriteLayer {
    GsSPRITE **sprites;
    u_short sprites_cnt;
    u_short prio;
    struct SpriteLayer *next;
} SpriteLayer;

typedef struct FR_TileSet {
    char *source;
    GsSPRITE *sprite;
    u_short start_id;
    struct FR_TileSet *next;
} FR_TileSet;

typedef struct Frame {
    SpriteLayer *bg_layers, *fg_layers;
    u_char t_amount, d_amount, offset_x, offset_y;
    u_short width, height;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    FR_Dialog *dialogs;
    GameObject *game_object;
    FR_TileSet *fr_tilesets;
    CdrDATrack *audio_track;
} Frame;

FR_TileSet *frames_malloc_fr_tileset();
void frames_insert_sl_sorted(SpriteLayer **root, SpriteLayer *new_layer);

#endif
