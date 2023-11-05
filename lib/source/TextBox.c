#include "../header/GPUBase.h"
#include "../header/TextBox.h"
#include "../header/MemUtils.h"

#include <libetc.h>

#define SET_TILE_POS(t, tx, ty, tw, th) \
    setXY0(t, tx, ty);                  \
    (t)->w = tw;                        \
    (t)->h = th

void tbx_reset_dgl_box(DlgBox *box) {
    box->started = 0;
    txt_dlg_reset(box->dlg);
}

void add_canvas_to_dlg_box(DlgBox *box, short x, short y, short w, short h, VerticalGoraudColor *color) {
    SetPolyG4(&box->canvas);
    setRGB0(&box->canvas, color->r, color->g, color->b);
    setRGB1(&box->canvas, color->r, color->g, color->b);
    setRGB2(&box->canvas, color->r + color->ch_r, color->g + color->ch_g, color->b + color->ch_b);
    setRGB3(&box->canvas, color->r + color->ch_r, color->g + color->ch_g, color->b + color->ch_b);
    setXYWH(&box->canvas, x, y, w, h);
}

void add_border_to_box(DlgBox *box, u_char idx, Color *color) {
    SetTile(&box->border[idx]);
    setRGB0(&box->border[idx], color->r, color->g, color->b);
}

void tbx_draw(DlgBox *box) {
    if (box->complete || !box->started) {
        return;
    }

    GPUB_GS_SORT_POLY(&box->canvas);
    for (u_short i = 0; i < 8; i++) {
        GPUB_GS_SORT_POLY(&box->border[i]);
    }

    txt_dlg_draw(box->dlg);
}

void tbx_tick(DlgBox *box, DPad *ctrl) {
    short x = box->x, y = box->y;
    short w = box->w, h = box->h;

    if (box->complete) {
        return;
    }

    if (DPAD_BTN_INTERACT(ctrl)) {
        box->started = 1;
    }

    if (box->started) {
        if (txt_dlg_complete(box->dlg)) {
            // Roll up dialog box when all dialog text is done
            if (h > 0) {
                box->dlg->visible = 0;
                h -= TBX_SPEED;
            }
            if (h <= 0) {
                h = 0;
                box->complete = 1;
            }
        } else {
            // Roll dialog box all the way down to target height
            if (h < box->target_h) {
                h += TBX_SPEED;
            }

            if (h >= box->target_h) {
                h = box->target_h;
                txt_dlg_tick(box->dlg);
                if (DPAD_BTN_INTERACT(ctrl)) {
                    txt_next_msg(box->dlg, 1);
                }
            }
        }
    }

    setXYWH(&box->canvas, x, y, w, h);
    SET_TILE_POS(&box->border[0], x, y - 2, w, 1);                   // top - outer
    SET_TILE_POS(&box->border[1], x - 1, y - 1, w + 2, 1);           // top - inner
    SET_TILE_POS(&box->border[2], x - 1, y, 1, h);                   // left - outer
    SET_TILE_POS(&box->border[3], x - 2, y, 1, h);                   // left - inner
    SET_TILE_POS(&box->border[4], x, h + y + 1, w, 1);               // bottom - outer
    SET_TILE_POS(&box->border[5], x - 1, h + y, w + 2, 1);           // bottom - inner
    SET_TILE_POS(&box->border[6], w + x, y, 1, h);                   // right - outer
    SET_TILE_POS(&box->border[7], w + x + 1, y, 1, h);               // right - inner
    box->w = w;
    box->h = h;
}

DlgBox *tbx_init_dlg_box(short x, short y, short w, short h, short target_w, short target_h, VerticalGoraudColor *color,
                         Dialog *content) {
    Color white = {255, 255, 255};
    Color grey = {155, 155, 155};
    u_char i = 0;
    DlgBox *box = MEM_MALLOC_3(DlgBox);

    add_canvas_to_dlg_box(box, x, y, w, h, color);

    // Border
    add_border_to_box(box, i++, &grey);
    add_border_to_box(box, i++, &white);
    add_border_to_box(box, i++, &white);
    add_border_to_box(box, i++, &grey);
    add_border_to_box(box, i++, &grey);
    add_border_to_box(box, i++, &white);
    add_border_to_box(box, i++, &white);
    add_border_to_box(box, i, &grey);

    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
    box->target_w = target_w;
    box->target_h = target_h;
    box->dlg = content;
    box->started = 0;
    box->complete = 0;
    return box;
}

