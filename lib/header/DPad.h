/*
 * Wrapper for handling basic dpad controls. Uses player 1 only currently.
 * This lib uses only the standard libetc functions and does not work with anything else than
 * the digital pad
 */
#ifndef PSX_DEV_DPAD_H
#define PSX_DEV_DPAD_H

#define DPAD_PLAYER_1 1
#define DPAD_P1_INTERACT PADstart

#define DPAD_INIT(ctrl)              \
    (ctrl)->id = 1;                  \
    (ctrl)->curr_btn = 0

typedef struct DPad {
    int id;
    unsigned int curr_btn;
} DPad;

/**
 * Allocates space for Controller on the heap and sets id to passed in param.
 * @return ptr to new controller
 */
DPad *dpad_init();

#define DPAD_READ_INPUT(ctrl) (ctrl)->curr_btn = PadRead(DPAD_PLAYER_1)
#define DPAD_DPAD_UP(ctrl) ((ctrl)->curr_btn & PADLup)
#define DPAD_DPAD_DOWN(ctrl) ((ctrl)->curr_btn & PADLdown)
#define DPAD_DPAD_LEFT(ctrl) ((ctrl)->curr_btn & PADLleft)
#define DPAD_DPAD_RIGHT(ctrl) ((ctrl)->curr_btn & PADLright)
#define DPAD_BTN_INTERACT(ctrl) ((ctrl)->curr_btn & DPAD_P1_INTERACT)


#endif //PSX_DEV_DPAD_H
