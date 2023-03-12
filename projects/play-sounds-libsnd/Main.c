/**
 * Plays sequence data using LibSND, remake of the psyq sample project that loads data from a cd instead.
 */

#include <libsnd.h>
#include "../../lib/header/GPUBase.h"
#include "../../lib/header/Logger.h"
#include "../../lib/header/CdReader.h"
#include "../../lib/header/MemUtils.h"

#define NEXT_EMPTY_VAB_ID -1

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

Color snd_bg_color = {120, 50, 100};

short seq1; /* SEQ data id */
short sep1; /* SEP data id */
short vab1; /* VAB data id */

char seq_table[SS_SEQ_TABSIZ * 2 * 3];

short transfer_vab_header(u_char *start_addr, short id) {
    short vab_id = SsVabOpenHead(start_addr, id);
    if(vab_id == -1) {
        logr_log(ERROR, "Main.c", "transfer_vab_header", "Could not open vab header");
        exit(1);
    }
    return vab_id;
}

short transfer_vab_body(u_char *start_addr, short id) {
    short vab_id = SsVabTransBody(start_addr, id);
    if(vab_id == -1) {
        logr_log(ERROR, "Main.c", "transfer_vab_body", "Could not open vab body");
        exit(1);
    }
    return vab_id;
}

void retrieve_sound_data() {
    CdrData *cdr_seq1, *cdr_sep1, *cdr_vab1, *cdr_vh1;

    CDR_INIT();
    cdr_seq1 = cdr_read_file("SINFONIE.SEQ");
    cdr_sep1 = cdr_read_file("SAMPLE.SEP");
    cdr_vab1 = cdr_read_file("SIMPLE.VB");
    cdr_vh1 = cdr_read_file("SIMPLE.VH");

    vab1 = transfer_vab_header((u_char*) cdr_vh1->file, NEXT_EMPTY_VAB_ID);
    vab1 = transfer_vab_body((u_char *) cdr_vab1->file, vab1);

    // Wait for transfer to complete
    SsVabTransCompleted(SS_WAIT_COMPLETED);

    seq1 = SsSeqOpen(cdr_seq1->file, vab1);
    sep1 = SsSepOpen(cdr_sep1->file, vab1, 3);
    SsSetMVol(127, 127);
    SsSeqSetVol(seq1, 127, 127);
    SsSepSetVol(sep1, 1, 80, 80);
    SsSepSetVol(sep1, 2, 127, 127);
    SsSepSetVol(sep1, 3, 127, 127);

    SsSeqPlay(seq1, SSPLAY_PLAY, 3);

    SsSepPlay(sep1, 2, SSPLAY_PLAY, 2);
    SsSeqSetDecrescendo(seq1, 50, 200);

    SsSepPlay(sep1, 1, SSPLAY_PLAY, 1);

    CDR_DATA_FREE(cdr_seq1);
    CDR_DATA_FREE(cdr_sep1);
    CDR_DATA_FREE(cdr_vab1);
    CDR_DATA_FREE(cdr_vh1);
}

int main() {
    MEM_INIT_HEAP_3();
    gpub_set_bg_color(&snd_bg_color);
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    GPUB_INIT_DEBUG_FNT(960, 512, 5, 10, 256, 128, 1, 512);
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);

    // Init sound lib
    SsInit();
    SsSetTableSize(seq_table, 2, 3);
    SsSetTickMode(SS_TICK60);
    retrieve_sound_data();
    while (1) {
        FntPrint("LibSND sounds example\n");
        FntPrint("X to play a sequence\n");
        FntPrint("Y to load a sep\n");
        FntPrint("Current key=%d\n", 0);

        // Clear display
        gpub_clear_display();

        // Assign and clear current ot
        gpub_curr_ot = &gpub_ot[gpub_current_buffer];
        gpub_clear_ot(gpub_curr_ot);

        // Call drawsync anc vsync and swap buffers
        gpub_display_sync();
        gpub_sort_clear(gpub_curr_ot);

        SsSeqCalledTbyT();

        // Draw the ot
        gpub_draw_ot(gpub_curr_ot, 0, 0);
    }
}

