#include "../header/GPUBase.h"
#include "../header/Logger.h"

PACKET GPUOutputPacket[GPUB_NUM_BUFFERS][GPUB_PACKET_MAX];

u_long __ramsize = 0x00200000;        // force 2 megabytes of RAM NOLINT(bugprone-reserved-identifier)
u_long __stacksize = 0x00004000;        // force 16 kilobytes of stack NOLINT(bugprone-reserved-identifier)

u_char gpub_current_buffer = 0;
Color bg_color = {0, 0, 0};

GPUB_OT *gpub_curr_ot;

u_short w, h;
u_char gpub_fps;

void gpub_init_screen(u_char region_mode, u_char fps) {
    SetVideoMode(region_mode);
    SetDispMask(1);  // 1=MASK on
    ResetGraph(0);   // Initialise drawing engine, 0=Complete reset

    // Clear VRAM
    RECT rect;
    setRECT(&rect, 0, 0, GPUB_FRAME_BUFFER_WIDTH, GPUB_FRAME_BUFFER_HEIGHT);
    ClearImage2(&rect, bg_color.r, bg_color.g, bg_color.b);         // Clear Frame Buffer at high speed (interlaced mode)
    DrawSync(0);                                                           // Waits for ClearImage2 function to finish

    // Resets libgpu and initializes the graphic system. libpgu settings are maintained by
    // the global variables GsDISPENV and GsDRAWENV, we want libGPU to handle offsets (I think :S)
    GsInitGraph(gpub_screen_w, gpub_screen_h, GsOFSGPU, 0, 0);

    // Define double buffers
    GsDefDispBuff(0, 0, 0, gpub_screen_h);

    PadInit(GPUB_MODE_NTSC);

    gpub_fps = fps;
    logr_log(INFO, "GPUBase.c", "gpub_init_screen", "Screen initialized, w=%d, h=%d, REGION_MODE=%d, FPS Mode=%d", w, h, region_mode, gpub_fps);
}

void gpub_init_ot(GPUB_OT *gpub_ot, GsOT *ot, GsOT_TAG *tag, u_short length) {
    gpub_ot->ot = ot;
    gpub_ot->ot->length = length;
    gpub_ot->ot->org = tag;
    GsClearOt(0, 0, gpub_ot->ot);
}

void gpub_clear_display() {
#ifdef USE_FNT_PRINT
    FntFlush(-1);
#endif
    gpub_current_buffer = GsGetActiveBuff();
    GsSetWorkBase((PACKET *) GPUOutputPacket[gpub_current_buffer]);
}

void gpub_clear_ot(GPUB_OT *gpub_ot) {
    GsClearOt(0, 0, gpub_ot->ot);
    gpub_ot->prio = (1 << gpub_ot->ot->length) - 1;
}

void gpub_sort_clear(GPUB_OT *ot_to_clear) {
    GsSortClear(bg_color.r, bg_color.g, bg_color.b, ot_to_clear->ot);
}

void gpub_display_sync() {
    DrawSync(0);
    VSync(gpub_fps);
    GsSwapDispBuff();
}

void gpub_draw_ot(GPUB_OT *gpub_ot, short offset_x, short offset_y) {
    GsSetOffset(offset_x, offset_y);
    GsSetDrawBuffClip();
    GsDrawOt(gpub_ot->ot);
}

void gpub_set_bg_color(Color *color) {
    bg_color.r = color->r;
    bg_color.g = color->g;
    bg_color.b = color->b;
}
