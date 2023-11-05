#ifndef PSX_MAP_H
#define PSX_MAP_H

#include "GameObject.h"

/**
 * Allocates heap for passed in collision block
 */
#define MAP_MALLOC_COLLISION_BLOCK(cb, bounds_cnt)           \
    cb = MEM_MALLOC_3(CollisionBlock);                       \
    cb->bounds = MEM_CALLOC_3(bounds_cnt, RECT)

extern DPad g_ctrl;

void map_init(u_char level, Player *player, Camera *camera);
void map_tick();

void map_ot_sort_bg_layers();
void map_ot_sort_fg_layers();
void map_ot_sort_hud_layer();

#endif // PSX_MAP_H