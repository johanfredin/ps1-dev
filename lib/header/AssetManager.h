#ifndef PSX_ASSET_MANAGER_H
#define PSX_ASSET_MANAGER_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include "CdReader.h"
#include "Tiled.h"

#define ASMG_SOUND_MALLOC_MAX 0xA
#define ASMG_VAG_HEADER_BYTE_SIZE 0x30
#define ASMG_BLEND_NORMAL 128

#define ASMG_SPRITE_ATTR_16_BIT (2 << 24)

#define ASMG_SPRITE_ATTR_CLUT(gs_image) ((gs_image)->pmode & 3) << 24

#define ASMG_HAS_CLUT(gs_image) ((gs_image)->pmode & 3) < 2

#define ASMG_FREE_VRAM_ASSET(asset)             \
    MEM_FREE_3_AND_NULL((asset)->frame_buffer); \
    MEM_FREE_3_AND_NULL((asset)->clut);         \
    MEM_FREE_3_AND_NULL(asset)

#define ASMG_AUDIO_PLAY(voice_channel) SpuSetKey(SpuOn, voice_channel)

#define ASMG_AUDIO_STOP(voice_channel) SpuSetKey(SpuOff, voice_channel)

#define ASMG_AUDIO_FREE(spu_address) SpuFree(spu_address)

#define ASMG_SET_BLEND(p) (p)->r = (p)->g = (p) -> b = ASMG_BLEND_NORMAL

#define ASMG_SET_RGB(p, cr, cg, cb) (p)->r = cr; (p)->g = cg; (p)->b = cb

/**
 * All VAGs have a 48 byte header, which must be removed for playback. This header should not be removed
 * before converting VAGs to VABs on the Mac or PC; otherwise, improper conversion will occur.
 */
#define ASMG_AUDIO_SKIP_VAG_HEADER(vag) ((vag) += ASMG_VAG_HEADER_BYTE_SIZE)

typedef enum BitDepthMul {
    BIT_DEPTH_4 = 4,
    BIT_DEPTH_8 = 1,
} BitDepthMul;

void asmg_load_tim_data(GsIMAGE *tim_data, CdrData *cdr_data);
void asmg_load_sprite(GsSPRITE *sprite, CdrData *data, u_short x, u_short y, BitDepthMul multiplier);
void asmg_load_16_bit_sprite(GsSPRITE *sprite, CdrData *data, u_short x, u_short y);
void asmg_load_sprite_region(GsSPRITE *base, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h);
void asmg_audio_init();
void asmg_transfer_vag_to_spu(CdrData *cdr_data, u_long voice_channel);

#endif // PSX_ASSET_MANAGER_H
