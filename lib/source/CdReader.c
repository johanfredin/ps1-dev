#include <sys/types.h>

#include "../header/CdReader.h"
#include "../header/Logger.h"
#include "../header/MemUtils.h"
#include "../header/StrUtils.h"

CdrData *cdr_create_data_entry(char *name) {
    CdrData *asset = MEM_MALLOC_3(CdrData);
    asset->name = name;
    asset->file = NULL;
    asset->sectors = 0;
    return asset;
}

void cdr_init_cdr_data(CdrData *cdr_data, char *name) {
    cdr_data->name = name;
    cdr_data->file = NULL;
    cdr_data->sectors = 0;
}

CdrData *cdr_find_data_entry(char* name, CdrData **assets, u_char assets_cnt) {
    for(u_char i = 0; i < assets_cnt; i++) {
        if(STR_EQ(assets[i]->name, name)) {
            logr_log(TRACE, "CdReader.c", "cdr_find_data_entry", "Name=%s, found at index=%d", assets[i]->name, i);
            return assets[i];
        }
    }
    logr_log(ERROR, "CdReader.c", "cdr_find_data_entry", "No CdrData with name='%s' in passed in array, terminating...", name);
    exit(1);
}

void cdr_read_from_disc(CdrData *cdr_data) {
    char file_path_raw[16];

    // Get raw file path
    strcpy(file_path_raw, "\\");
    strcat(file_path_raw, cdr_data->name);
    strcat(file_path_raw, ";1");
    logr_log(DEBUG, "CdReader.c", "cdr_read_file", "Loading file from CD: %s", file_path_raw);

    // Search for file on disc
    DslFILE temp_file_info;
    const DslFILE *res = DsSearchFile(&temp_file_info, file_path_raw);
    if (res == CDR_FILE_NOT_FOUND) {
        logr_log(ERROR, "CdReader.c", "cdr_read_file", "File=%s not found, terminating...", cdr_data->name);
        exit(1);
    }
    if ((long) res == CDR_INVALID_READ) {
        logr_log(ERROR, "CdReader.c", "cdr_read_file", "CDR Read error for file=%s, terminating...", cdr_data->name);
        exit(1);
    }

    // Read the file if it was found
    if (temp_file_info.size > 0) {
        logr_log(TRACE, "CdReader.c", "cdr_read_file", "file found");
        logr_log(TRACE, "CdReader.c", "cdr_read_file", "file size: %lu", temp_file_info.size);
        const u_long sectors_size = temp_file_info.size + (CDR_SECTOR % temp_file_info.size);
        const u_long sectors_needed = (sectors_size + CDR_SECTOR - 1) / CDR_SECTOR;
        logr_log(TRACE, "CdReader.c", "cdr_read_file", "file buffer size needed: %d", sectors_size);
        logr_log(TRACE, "CdReader.c", "cdr_read_file", "sectors needed: %d", sectors_needed);
        cdr_data->file = MEM_MALLOC_3_CUS_SIZE(u_long, sectors_size + CDR_SECTOR);
        DsRead(&temp_file_info.pos, (int) sectors_needed, cdr_data->file, DslModeSpeed);

        while (DsReadSync(NULL)) {
            // Wait for null...
        }

        cdr_data->sectors = sectors_needed;
        logr_log(INFO, "CdReader.c", "cdr_read_file", "File %s loaded from CD", cdr_data->name);
    } else {
        logr_log(ERROR, "CdReader.c", "cdr_read_file", "File %s file not found on CD, terminating...", cdr_data->name);
        exit(1);
    }
}

CdrData *cdr_read_file(char *file_path) {
    CdrData *cdr_data = cdr_create_data_entry(file_path);
    cdr_read_from_disc(cdr_data);
    return cdr_data;
}

void cdr_da_play(CdrDATrack *track) {
#if DA_ENABLED
    if (CDR_DA_CANT_PLAY(track)) {
        return;
    }

    int t[] = {track->track, 0};
    DsPlay(CDR_DA_MODE_REPEAT, t, 0);
    track->is_playing = 1;
#endif
}

void cdr_da_stop(CdrDATrack *track) {
#if DA_ENABLED
    if (CDR_DA_CANT_STOP(track)) {
        return;
    }

    int t[] = {track->track, 0};
    track->is_playing = DsPlay(CDR_DA_STOP, t, 0);
    track->is_playing = 0;
#endif
}

void cdr_da_swap(CdrDATrack *from, CdrDATrack *to) {
#if DA_ENABLED
    const u_char from_track_nr = from->track;
    const u_char to_track_nr = to->track;

    if (from_track_nr == to_track_nr) {
        // If the track we want to swap to is the same track, do nothing
        return;
    }

    cdr_da_stop(from);
    cdr_da_play(to);
#endif
}
