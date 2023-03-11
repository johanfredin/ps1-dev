#ifndef PSX_DEV_SOUNDEFFECT_H
#define PSX_DEV_SOUNDEFFECT_H

#include <libspu.h>
#include "CdReader.h"

// Voice channels
#define SFX_WALK SPU_00CH
#define SFX_HPUP SPU_01CH

void sfx_load();

#endif //PSX_DEV_SOUNDEFFECT_H
