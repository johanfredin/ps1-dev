#ifndef PSX_DEV_COLLISIONHANDLER_H
#define PSX_DEV_COLLISIONHANDLER_H

#include "GameObject.h"

#define CHDLR_SPRITE_IN_VIEW(sprite, cam)        \
(sprite)->x < (cam)->x + (cam)->viewport_w &     \
(sprite)->x + (sprite)->w > (cam)->x &           \
(sprite)->y < (cam)->y + (cam)->viewport_h &     \
(sprite)->y + (cam)->viewport_h > (cam)->y

u_char chdlr_overlaps(RECT *r1, RECT *r2);
void chdlr_collision_resolve(GameObject *gobj, RECT *level_bounds, u_char blocks_amount);

#endif //PSX_DEV_COLLISIONHANDLER_H
