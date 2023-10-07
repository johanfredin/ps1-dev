//
// Created by johan on 2022-01-30.
//
#include "../header/MemUtils.h"
#include "../header/Controller.h"
#include <malloc.h>

Controller *ctrl_init(int id) {
    Controller *ctrl = MEM_MALLOC_3(Controller);
    ctrl->id = id;
    ctrl->curr_btn = 0;
    return ctrl;
}
