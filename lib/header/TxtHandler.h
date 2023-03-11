//
// Created on 12/12/2021.
//

#ifndef PSX_DEV_TXTHANDLER_H
#define PSX_DEV_TXTHANDLER_H

#include "../header/AssetManager.h"

#define TXT_FNT_PADDING_DEFAULT 2

typedef struct Font {
    // The image representing the whole font image
    GsSPRITE *fnt_sprite;
    // Character width of font image
    u_char cw;
    // Character height of font image
    u_char ch;
    // Padding between characters
    u_char padding;
} Font;

typedef struct Message {
    GsSPRITE *fnt_sprites;
    u_short strlen;
    u_short acc_chars;
    u_char active: 4;
    u_char acc_ticks: 4;
} Message;

typedef struct Dialog {
    Message *messages;
    char *id;
    u_char n_messages;
    u_char x;
    u_char y;
    u_char ticks_per_frame: 7;
    u_char visible: 1;
} Dialog;

/**
 * Load a font from cd and store on the heap.
 * @param name name of the font file e.g my_font.tim
 * @param cw Character width of font image
 * @param ch Character height of font image
 * @param padding Padding between characters
 * @return pointer to font from cd
 */
Font *txt_fnt_init(char *name, u_char cw, u_char ch, u_char padding);

Dialog *txt_dlg_init(char **strs, char *id, u_char n_messages, Font *fnt, u_short ticks_per_frame, u_char x, u_char y, u_char visible);
void *txt_msg_init(Message *msg, Font *font, u_char x, u_char y, char *str, u_char make_static, u_char active);
void txt_dlg_reset(Dialog *dlg);
void txt_msg_draw(Message *msg);
void txt_dlg_draw(Dialog *dlg);
void txt_dlg_tick(Dialog *dlg);
void txt_next_msg(Dialog *dlg, u_char can_skip);
u_char txt_dlg_complete(Dialog *dlg);

#endif //PSX_DEV_TXTHANDLER_H
