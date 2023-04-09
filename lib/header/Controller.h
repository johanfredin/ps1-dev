/*
 * Wrapper for handling basic dpad controls. Uses player 1 only currently.
 * PadInit must have been called prior to using any functions or macros in here.
 */

#ifndef PSX_DEV_CONTROLLER_H
#define PSX_DEV_CONTROLLER_H

#define CTRL_PLAYER_1 1
#define CTRL_PLAYER_2 2

#define CTRL_P1_ACTION PADR1
#define CTRL_P1_INTERACT PADstart

#define CTRL_INIT(ctrl, ctrl_id)     \
    (ctrl)->id = ctrl_id;            \
    (ctrl)->curr_btn = 0

#define CTRL_INIT_P1(ctrl) CTRL_INIT(ctrl, CTRL_PLAYER_1)

typedef struct Controller {
    int id;
    unsigned int curr_btn;
} Controller;

/**
 * Allocates space for Controller on the heap and sets id to passed in param.
 * @param id controller id (1=Player 1)
 * @return ptr to new controller
 */
Controller *ctrl_init(int id);

#define CTRL_READ_INPUT(ctrl) (ctrl)->curr_btn = PadRead(CTRL_PLAYER_1)
#define CTRL_DPAD_UP(ctrl) ((ctrl)->curr_btn & PADLup)
#define CTRL_DPAD_DOWN(ctrl) ((ctrl)->curr_btn & PADLdown)
#define CTRL_DPAD_LEFT(ctrl) ((ctrl)->curr_btn & PADLleft)
#define CTRL_DPAD_RIGHT(ctrl) ((ctrl)->curr_btn & PADLright)
#define CTRL_BTN_INTERACT(ctrl) ((ctrl)->curr_btn & CTRL_P1_INTERACT)

#define CTRL_BTN_ACTION(ctrl) ((ctrl)->curr_btn & CTRL_P1_ACTION)
#define CTRL_IS_MOVING(ctrl) ((PADLup | PADLdown | PADLright | PADLleft) & (ctrl)->curr_btn)

#define CTRL_IS_BTN(ctrl, btn) ((ctrl)->curr_btn & (btn))

#endif //PSX_DEV_CONTROLLER_H
