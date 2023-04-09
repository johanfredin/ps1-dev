/**
 * Helper lib for streaming data from the cd rom
 */
#ifndef PSX_CD_READER_H
#define PSX_CD_READER_H

#include <libds.h>
#include <stdlib.h>

#define CDR_SECTOR 0x800

#define CDR_FILE_NOT_FOUND NULL
#define CDR_INVALID_READ (-1)
#define CDR_DA_MODE_REPEAT 2
#define CDR_DA_STOP 0

typedef struct CdrData {
    char *name;
    u_long *file;
    u_int sectors;
} CdrData;

typedef struct CdDATrack {
    u_char track: 7;
    u_char is_playing: 1;
} CdDATrack;

#define CDR_INIT()                                                                                   \
    if(DsInit() != 1) {                                                                              \
        logr_log(ERROR, "CdReader.c", "cdr_init", "LIBDS not initialized properly, terminating..."); \
        exit(1);                                                                                     \
    }                                                                                                \
    logr_log(INFO, "CdReader.c", "cdr_init", "LIBDS initialized")


#define CDR_CLOSE() DsClose()

#define CDR_DATA_FREE(cdr_data)             \
    MEM_FREE_3_AND_NULL((cdr_data)->file);  \
    MEM_FREE_3_AND_NULL(cdr_data)

CdrData *cdr_create_data_entry(char *name);
CdrData *cdr_find_data_entry(char *name, CdrData **assets, unsigned char assets_cnt);

/**
 * Read contents of the cd. CdOpen should be called prior to this and CdClose after
 * @param file_path location of file on cd
 * @return allocated CdrData struct
 */
CdrData *cdr_read_file(char *file_path);

void cdr_da_play(CdDATrack *track);
void cdr_da_stop(CdDATrack *track);

#endif // PSX_CD_READER_H
