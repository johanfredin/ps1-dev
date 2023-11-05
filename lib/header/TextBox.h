#ifndef PSX_DEV_TEXTBOX_H
#define PSX_DEV_TEXTBOX_H

#include "TxtHandler.h"
#include "DPad.h"

#define TBX_SPEED 12

#define TBX_INIT_VERT_GORAUD_COLOR(clr, clr_r, clr_g, clr_b, clr_ch_r, clr_ch_g, clr_ch_b) \
    (clr)->r = clr_r; \
    (clr)->g = clr_g; \
    (clr)->b = clr_b; \
    (clr)->ch_r = clr_ch_r; \
    (clr)->ch_g = clr_ch_g; \
    (clr)->ch_b = clr_ch_b

typedef struct VerticalGoraudColor {
    u_char r, g, b;
    short ch_r, ch_g, ch_b;
} VerticalGoraudColor;

typedef struct DlgBox {
    short x, y;
    short w, h;
    short target_w, target_h;
    Dialog *dlg;
    POLY_G4 canvas;
    TILE border[8];
    u_char started: 1;
    u_char complete: 1;
} DlgBox;

DlgBox *tbx_init_dlg_box(short x, short y, short w, short h, short target_w, short target_h, VerticalGoraudColor *color, Dialog *content);
void tbx_draw(DlgBox *box);
void tbx_tick(DlgBox *box, DPad *ctrl);
void tbx_reset_dgl_box(DlgBox *box);

#endif //PSX_DEV_TEXTBOX_H
