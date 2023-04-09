#include "../header/CollisionHandler.h"

u_char chdlr_overlaps(RECT *r1, RECT *r2) {
    return (r1->x < r2->x + r2->w) &
           (r1->x + r1->w > r2->x) &
           (r1->y < r2->y + r2->h) &
           (r1->y + r1->h > r2->y);
}

void chdlr_collision_resolve(GameObject *gobj, RECT *level_bounds, u_char blocks_amount) {
    for (u_char i = 0; i < blocks_amount; i++) {
        RECT rect = level_bounds[i];
        RECT gobj_bounds = *gobj->bounds;
        if (chdlr_overlaps(&gobj_bounds, &rect)) {
            short old_x = gobj->old_x;
            short old_y = gobj->old_y;
            short right = (gobj->sprite->x + gobj_bounds.w);
            short bottom = (gobj->sprite->y + gobj_bounds.h);
            short oldBottom = (gobj->old_y + gobj_bounds.h);
            short oldRight = (gobj->old_x + gobj_bounds.w);
            short rightBottom = (rect.y + rect.h);

            if (right >= rect.x & oldRight <= rect.x) {                                          // From right
                gobj->sprite->x = (rect.x - gobj->bounds->w);
            } else if (gobj->sprite->x <= (rect.x + rect.w) & old_x >= (rect.x + rect.w)) {      // From left
                gobj->sprite->x = (rect.x + rect.w);
            } else if (bottom >= rect.y & oldBottom <= rect.y) {                                 // From above
                gobj->sprite->y = (rect.y - gobj->bounds->h);
            } else if (gobj->sprite->y <= rightBottom & old_y >= rightBottom) {                  // From below
                gobj->sprite->y = rightBottom;
            }

            // Update the bounds to match the corrected sprite positions so prevent jitter with camera
            gobj->bounds->x = gobj->sprite->x;
            gobj->bounds->y = gobj->sprite->y;
        }
    }
}
