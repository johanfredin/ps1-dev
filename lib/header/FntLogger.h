#ifndef PSX_DEV_FNTLOGGER_H
#define PSX_DEV_FNTLOGGER_H

#define FNT_PRINT_ANIMATION(anim) FntPrint("Acc=%d, curr_u=%d, heading=%d moving=%d\n", anim->acc_ticks, anim->curr_u, gobj->heading, GOBJ_IS_MOVING(gobj))

#define FNT_PRINT_GOBJ_COORDS(gobj)                                 \
    FntPrint("x=%d, y=%d\n", gobj->sprite->x, gobj->sprite->y);     \
    FntPrint("heading=%d\n", gobj->heading)

#define FNT_PRINT_BLOCKS(curr_frame_n, frame_ptr)                               \
    FntPrint("Current frame=%d\n", curr_frame_n);                               \
    FntPrint("Blocks in frame=%d\n", frame_ptr->collision_blocks->amount);      \
    FntPrint("Teleports in frame=%d\n", frame->t_amount)

#define FNT_PRINT_BOX(box) FntPrint("box x=%d, y=%d, w=%d, h=%d, tw=%d, th=%d\n", (box)->x, (box)->y, (box)->w, (box)->h, (box)->target_w, (box)->target_h)

#define FNT_PRINT_CTRL(ctrl) FntPrint("ctrl: id=%d, curr_btn=%d\n", ctrl->id, ctrl->curr_btn)

#define FNT_PRINT_DLG_BOX(dlg) FntPrint("Dialog started=%d, complete=%d\n, w=%d, h=%d\n", box->started, box->complete, box->w, box->h)

#define FNT_PRINT_PLAYER(player) \
FntPrint("px=%d, py=%d, vel_x=%d, vel_y=%d, heading=%d\n", (player)->gobj->sprite->x, (player)->gobj->sprite->y, (player)->gobj->x_speed, (player)->gobj->y_speed, (player)->gobj->heading)

#define FNT_PRINT_CAM(cam)                                                                              \
FntPrint("cam_x=%d, cam_y=%d, cam_w=%d, cam_h=%d\n", (cam)->x, (cam)->y, (cam)->viewport_w, (cam)->viewport_h)

#define FNT_PRINT_FRAME(frame)                                                                              \
FntPrint("frame_w=%d, frame_h=%d\n", (frame)->width, (frame)->height)

#endif //PSX_DEV_FNTLOGGER_H
