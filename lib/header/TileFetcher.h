#ifndef PSX_TILEFETCHER_H
#define PSX_TILEFETCHER_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include "Frames.h"
#include "Tiled.h"

void tf_add_layers_to_frame(Frame *frame, FR_TileSet *tile_sets, u_char n_tilesets, Tile_Map *map);

#endif //PSX_TILEFETCHER_H
