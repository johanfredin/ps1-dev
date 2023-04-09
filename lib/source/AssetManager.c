#include <stdlib.h>

#include <libspu.h>

#include "../header/AssetManager.h"
#include "../header/GPUBase.h"
#include "../header/Logger.h"

/*
 * Get the vram tim_data size using bitplane info (because vram position is in 16bits mode only)
 * From the great tutorials from orion!
 */
#define ImageToVRamSize(size, mode) ((size) / (1 << (2 - ((mode) & 3))))
#define VRamToImageSize(size, mode) ((size) * (1 << (2 - ((mode) & 3))))

SpuCommonAttr spu_common_attr;
SpuVoiceAttr spu_voice_attr;
long vag_spu_addr;
char rec[SPU_MALLOC_RECSIZ * (ASMG_SOUND_MALLOC_MAX + 1)];

void asmg_load_tim_data(GsIMAGE *tim_data, CdrData *cdr_data) {
    RECT rect;

    GsGetTimInfo(cdr_data->file + 1, tim_data);    // + 1 unsigned long to skip header (4 bytes)
    rect.x = tim_data->px;
    rect.y = tim_data->py;
    rect.w = tim_data->pw;
    rect.h = tim_data->ph;
    LoadImage(&rect, tim_data->pixel);
    LOGR_LOG_FRAME_BUFFER(INFO, &rect);

    // If color bits < 16 then we need to load the CLUT
    if (ASMG_HAS_CLUT(tim_data)) {
        rect.x = tim_data->cx;
        rect.y = tim_data->cy;
        rect.w = tim_data->cw;
        rect.h = tim_data->ch;
        LoadImage(&rect, tim_data->clut);
        LOGR_LOG_CLUT(INFO, &rect);
    }
}

void asmg_load_sprite(GsSPRITE *sprite, CdrData *data, u_short x, u_short y, BitDepthMul multiplier) {
    GsIMAGE tim_data;
    asmg_load_tim_data(&tim_data, data);

    u_short vrx = (tim_data.px + ImageToVRamSize(0, tim_data.pmode)) & 0x3C0;

    sprite->x = x;
    sprite->y = y;
    sprite->attribute = ASMG_SPRITE_ATTR_CLUT(&tim_data);
    sprite->w = tim_data.pw << multiplier;
    sprite->h = tim_data.ph;
    sprite->tpage = GetTPage((int) tim_data.pmode, 1, tim_data.px, tim_data.py);
    sprite->u = (VRamToImageSize(tim_data.px - vrx, tim_data.pmode)) & 255;
    sprite->v = tim_data.py & 255;
    sprite->cx = tim_data.cx;
    sprite->cy = tim_data.cy;
    ASMG_SET_BLEND(sprite);
    sprite->mx = 0;
    sprite->my = 0;
    sprite->scalex = GPUB_ROT_ONE;
    sprite->scaley = GPUB_ROT_ONE;
    sprite->rotate = 0;
    LOGR_LOG_SPRITE(INFO, sprite);
}

void asmg_load_16_bit_sprite(GsSPRITE *sprite, CdrData *data, u_short x, u_short y) {
    GsIMAGE tim_data;
    asmg_load_tim_data(&tim_data, data);

    sprite->attribute = ASMG_SPRITE_ATTR_16_BIT;
    sprite->x = x;
    sprite->y = y;
    sprite->w = tim_data.pw;
    sprite->h = tim_data.ph;
    sprite->tpage = GetTPage((int) tim_data.pmode, 0, tim_data.px, tim_data.py);
    sprite->u = 0;
    sprite->v = 0;
    ASMG_SET_BLEND(sprite);
    sprite->mx = 0;
    sprite->my = 0;
    sprite->scalex = GPUB_ROT_ONE;
    sprite->scaley = GPUB_ROT_ONE;
    sprite->rotate = 0;
}

void asmg_load_sprite_region(GsSPRITE *base, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h) {
    region->attribute = base->attribute;
    region->x = x;
    region->y = y;
    region->w = w;
    region->h = h;
    region->tpage = base->tpage;
    region->u = u;
    region->v = v;
    region->cx = base->cx;
    region->cy = base->cy;
    ASMG_SET_RGB(region, base->r, base->g, base->g);
    region->rotate = base->rotate;
    region->mx = base->mx;
    region->my = base->my;
    region->scalex = base->scalex;
    region->scaley = base->scaley;
}

void asmg_audio_init() {
    SpuInit();
    SpuInitMalloc(ASMG_SOUND_MALLOC_MAX, rec);
    spu_common_attr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR;
    spu_common_attr.mvol.left = 0x3fff;     // Master left vol
    spu_common_attr.mvol.right = 0x3fff;    // Master right vol
    spu_voice_attr.mask = 0xFFFFFF;
    SpuSetCommonAttr(&spu_common_attr);
    SpuSetVoiceAttr(&spu_voice_attr);
    logr_log(INFO, "AssetManager.c", "asmg_audio_init", "SPU initialized");
}

void asmg_transfer_vag_to_spu(CdrData *cdr_data, u_long voice_channel) {
    SpuSetTransferMode(SpuTransByDMA);                                   // set transfer mode to DMA
    vag_spu_addr = SpuMalloc((long)(cdr_data->sectors) * CDR_SECTOR);     // allocate SPU memory for sound
    if(vag_spu_addr == -1) {
        logr_log(ERROR, "AssetManager.c", "asmg_transfer_vag_to_spu", "Could not allocate audio file %s, shutting down", cdr_data->name);
        exit(1);
    }

    SpuSetTransferStartAddr(vag_spu_addr);                           // set transfer starting address to malloced area
    u_char *vag_file = (u_char *) cdr_data->file;
    ASMG_AUDIO_SKIP_VAG_HEADER(vag_file);
    SpuWrite(vag_file, cdr_data->sectors * CDR_SECTOR);         // perform actual transfer
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);                       // wait for dma to complete
    spu_voice_attr.mask = (
            SPU_VOICE_VOLL |
            SPU_VOICE_VOLR |
            SPU_VOICE_PITCH |
            SPU_VOICE_WDSA |
            SPU_VOICE_ADSR_AMODE |
            SPU_VOICE_ADSR_SMODE |
            SPU_VOICE_ADSR_RMODE |
            SPU_VOICE_ADSR_AR |
            SPU_VOICE_ADSR_DR |
            SPU_VOICE_ADSR_SR |
            SPU_VOICE_ADSR_RR |
            SPU_VOICE_ADSR_SL
    );
    spu_voice_attr.voice = voice_channel;
    spu_voice_attr.volume.left = 0x1fff;
    spu_voice_attr.volume.right = 0x1fff;
    spu_voice_attr.pitch = 0x1000;
    spu_voice_attr.addr = vag_spu_addr;
    spu_voice_attr.a_mode = SPU_VOICE_LINEARIncN;
    spu_voice_attr.s_mode = SPU_VOICE_LINEARIncN;
    spu_voice_attr.r_mode = SPU_VOICE_LINEARIncN;
    spu_voice_attr.ar = 0x0;
    spu_voice_attr.dr = 0x0;
    spu_voice_attr.sr = 0x0;
    spu_voice_attr.rr = 0x0;
    spu_voice_attr.sl = 0xf;
    SpuSetVoiceAttr(&spu_voice_attr);
}

