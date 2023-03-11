//
// Created by johan on 2022-06-19.
//

#include "GameObject.h"

#ifndef PSX_DEV_COLLISIONHANDLER_H
#define PSX_DEV_COLLISIONHANDLER_H

#define CHDLR_OVERLAP_RECT_VS_RECT(r1, r2)        \
((r1)->x < (r2)->x + (r2)->w) &                   \
((r1)->x + (r1)->w > (r2)->x) &                   \
((r1)->y < (r2)->y + (r2)->h) &                   \
((r1)->y + (r1)->h > (r2)->y)

#define CHDLR_SPRITE_IN_VIEW(sprite, cam)        \
(sprite)->x < (cam)->x + (cam)->viewport_w &     \
(sprite)->x + (sprite)->w > (cam)->x &           \
(sprite)->y < (cam)->y + (cam)->viewport_h &     \
(sprite)->y + (cam)->viewport_h > (cam)->y

u_char chdlr_overlaps(RECT *r1, RECT *r2);
void chdlr_collision_resolve(GameObject *gobj, RECT *level_bounds, u_char blocks_amount);

#endif //PSX_DEV_COLLISIONHANDLER_H
