#ifndef PSX_GPU_BASE_H
#define PSX_GPU_BASE_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>

// Constants
#define GPUB_NUM_BUFFERS 0x2
#define GPUB_OT_LENGTH_STD 0xA
#define GPUB_OT_LENGTH_SM 0x8
#define GPUB_MODE_NTSC 0x0
#define GPUB_MODE_PAL 0x1
#define GPUB_ROT_ONE 0x1000
#define GPUB_PACKET_MAX 1000 * 0x40
#define GPUB_FRAME_BUFFER_WIDTH 0x400
#define GPUB_FRAME_BUFFER_HEIGHT 0x200

#define GPUB_FPS_30 2
#define GPUB_FPS_60 0

#define GPUB_GS_SORT_FAST_OBJ(gs_obj) GsSortFastSprite(gs_obj, gpub_curr_ot->ot, gpub_curr_ot->prio--)
#define GPUB_GS_SORT_POLY(poly) GsSortPoly(poly, gpub_curr_ot->ot, gpub_curr_ot->prio--)
#define GPUB_GS_SORT_BOX_FILL(box) GsSortBoxFill(box, gpub_curr_ot->ot, gpub_curr_ot->prio--)

#define USE_FNT_PRINT

// Initialize debug font
#define GPUB_INIT_DEBUG_FNT(tx, ty, x, y, w, h, bg, max_chars)                                                      \
    FntLoad(tx, ty);                                                                                                \
    SetDumpFnt(FntOpen(x, y, w, h, bg, max_chars)) // Sets the dumped font for use with FntPrint()

#define GPUB_INIT_DEBUG_FNT_DEFAULT() GPUB_INIT_DEBUG_FNT(960, 256, 5, 10, 320, 32, 0, 512)


typedef struct Color {
    u_char r, g, b;
} Color;

typedef struct GPUB_OT {
    GsOT *ot;
    u_short prio;
} GPUB_OT;

extern GPUB_OT *gpub_curr_ot;
extern u_char gpub_current_buffer;
extern const u_short gpub_screen_w;
extern const u_short gpub_screen_h;

// Prototypes
void gpub_init_screen(u_char region_mode, u_char fps);
void gpub_display_sync();
void gpub_set_bg_color(Color *color);
void gpub_clear_display();
void gpub_sort_clear(GPUB_OT *ot_to_clear);
void gpub_init_ot(GPUB_OT *gpub_ot, GsOT *ot, GsOT_TAG *tag, u_short length);
void gpub_clear_ot(GPUB_OT *gpub_ot);
void gpub_draw_ot(GPUB_OT *gpub_ot, short offset_x, short offset_y);

#endif // PSX_GPU_BASE_H
