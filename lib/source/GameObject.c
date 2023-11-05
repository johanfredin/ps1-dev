#include "../header/GameObject.h"
#include "../header/GPUBase.h"
#include "../header/MemUtils.h"
#include "../header/Logger.h"
#include "../header/AssetManager.h"
#include "../header/SoundEffect.h"

#include <libetc.h>

GameObject *gobj_init(GsSPRITE *sprite, short w, short h, short x_speed, short y_speed, u_char health, u_char type) {
    GameObject *gobj = MEM_MALLOC_3(GameObject);
    gobj->w = w;
    gobj->h = h;
    gobj->sprite = sprite;
    gobj->sprite->w = w; // Set width portion of image to render
    gobj->sprite->h = h; // Set height portion of image to render
    gobj->x_speed = x_speed;
    gobj->y_speed = y_speed;
    gobj->health = health;
    gobj->spawn_x = sprite->x;
    gobj->spawn_y = sprite->y;
    gobj->type = type;
    gobj->heading = 0;
    gobj->can_move = 1;
    gobj->bounds = MEM_MALLOC_3(RECT);
    gobj->bounds->x = gobj->sprite->x;
    gobj->bounds->y = gobj->sprite->y;
    gobj->bounds->w = gobj->sprite->w;
    gobj->bounds->h = gobj->sprite->h;
    LOGR_LOG_GOBJ(DEBUG, gobj);
    return gobj;
}

void gobj_draw(GameObject *game_object) {
    GPUB_GS_SORT_FAST_OBJ(game_object->sprite);
}

void gobj_tick(GameObject *game_object) {
    game_object->sprite->x = (game_object->sprite->x + game_object->x_speed);
    game_object->sprite->y = (game_object->sprite->y + game_object->y_speed);
}

Player *gobj_player_init(Animation *anim, GameObject *gobj, u_char p_num) {
    Player *p = MEM_MALLOC_3(Player);
    p->anim = anim;
    p->gobj = gobj;
    p->p_num = p_num;
    return p;
}

void gobj_player_draw(Player *p) {
    GPUB_GS_SORT_FAST_OBJ(p->gobj->sprite);
}

void gobj_draw_bounds(GameObject *gobj) {
    RECT *bounds = gobj->bounds;
    GsBOXF boxf = {1, bounds->x, bounds->y, bounds->w, bounds->h, 100, 0, 0};
    GPUB_GS_SORT_BOX_FILL(&boxf);
}

void gobj_player_tick(Player *p, DPad *ctrl) {
    if (p->gobj->can_move) {
        // Set old pos
        p->gobj->old_x = p->gobj->sprite->x;
        p->gobj->old_y = p->gobj->sprite->y;
        p->gobj->x_speed = p->gobj->y_speed = 0;

        DPAD_READ_INPUT(ctrl);
        p->gobj->heading = 0;
        if (DPAD_DPAD_DOWN(ctrl)) {
            p->gobj->y_speed = GOBJ_P_VEL_Y;
            p->gobj->heading = GOBJ_HEADING_DOWN;
        }
        if (DPAD_DPAD_UP(ctrl)) {
            p->gobj->y_speed = -GOBJ_P_VEL_Y;
            p->gobj->heading = GOBJ_HEADING_UP;
        }
        if (DPAD_DPAD_LEFT(ctrl)) {
            p->gobj->x_speed = -GOBJ_P_VEL_X;
            p->gobj->heading = GOBJ_HEADING_LEFT;
        }
        if (DPAD_DPAD_RIGHT(ctrl)) {
            p->gobj->x_speed = GOBJ_P_VEL_X;
            p->gobj->heading = GOBJ_HEADING_RIGHT;
        }

        // Update bounds
        p->gobj->sprite->x += p->gobj->x_speed;
        p->gobj->sprite->y += p->gobj->y_speed;
        p->gobj->bounds->x = p->gobj->sprite->x;
        p->gobj->bounds->y = p->gobj->sprite->y;
    }
}

Animation *gobj_anim_init(u_short curr_u, u_short curr_v, u_char key_frames, u_short ticks_per_frame) {
    Animation *anim = MEM_MALLOC_3(Animation);
    anim->curr_u = curr_u;
    anim->curr_v = curr_v;
    anim->key_frames = key_frames;
    anim->ticks_per_frame = ticks_per_frame;
    anim->acc_ticks = 0;
    return anim;
}

void gobj_anim_tick(Animation *anim, GameObject *gobj) {
    u_short u = anim->curr_u; // current x offset
    u_short v = anim->curr_v; // current y offset
    u_short sprite_w = gobj->w;
    u_short sprite_h = gobj->h;

    if (GOBJ_IS_NOT_MOVING(gobj)) {
        // Set x offset and acquired ticks to 0 when not moving
        gobj->sprite->u = 0;
        anim->acc_ticks = 0;
        anim->curr_u = 0;
        return;
    }

    // Handle y offset
    if (gobj->heading & GOBJ_HEADING_LEFT) {
        v = sprite_h;       // 2nd row in image-sheet
    } else if (gobj->heading & GOBJ_HEADING_RIGHT) {
        v = sprite_h << 1;  // 3rd row in image-sheet
    } else if (gobj->heading & GOBJ_HEADING_UP) {
        v = sprite_h * 3;   // 4th row in image-sheet
    } else if (gobj->heading & GOBJ_HEADING_DOWN) {
        v = 0;              // 1st row in image-sheet
    }

    gobj->sprite->v = v;
    anim->curr_v = v;

    if (GOBJ_ANIM_CYCLE_COMPLETE(anim)) {
        anim->acc_ticks = 0;
        if (u >= ((anim->key_frames * sprite_w) - sprite_w)) { // If x offset is at last horizontal frame, reset it to 0
            u = 0;
        } else {
            u += sprite_w; // Move to next frame when not at the end of horizontal sheet.
        }
        if (u & sprite_w) {
            ASMG_AUDIO_PLAY(SFX_WALK);
        }
    } else {
        anim->acc_ticks++;
    }
    anim->curr_u = u;
    gobj->sprite->u = u;
}

void gobj_camera_init(Camera *cam, GameObject *obj_in_focus) {
    cam->x = cam->y = 0;
    cam->viewport_w = gpub_screen_w;
    cam->viewport_h = gpub_screen_h;
    cam->map_w = gpub_screen_w << 1;
    cam->map_h = gpub_screen_h << 1;
    cam->obj_in_focus = obj_in_focus;
}

void gobj_camera_tick(Camera *cam) {
    u_short obj_x = cam->obj_in_focus->bounds->x + (cam->obj_in_focus->bounds->w >> 1);
    u_short obj_y = cam->obj_in_focus->bounds->y + (cam->obj_in_focus->bounds->h >> 1);

    // If nothing to focus on then don't do anything
    if (cam->obj_in_focus == NULL) {
        return;
    }

    // Set game object in the middle of the viewport
    short x = (obj_x - (cam->viewport_w >> 1));
    short y = (obj_y - (cam->viewport_h >> 1));

    // Handle clamping
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x + cam->viewport_w > cam->map_w) {
        x = (cam->map_w - cam->viewport_w);
    }
    if (y + cam->viewport_h > cam->map_h) {
        y = (cam->map_h - cam->viewport_h);
    }
    cam->x = x;
    cam->y = y;
}