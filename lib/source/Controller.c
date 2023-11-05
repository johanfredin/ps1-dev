//
// Created by johan on 2023-11-05.
//
#include "../header/Controller.h"
#include <libapi.h>

/*
 * (From LameGuys tutorials)
 * Define an array of two 34 byte elements.
 * This array will be used as a buffer for storing incoming controller
 * data which requires at least about 34 bytes per controller port.
 * It may sound a bit excessive for storing input data, but this is necessary
 * in case a multitap is connected to one of the ports, as a multitap will
 * return up to 34 bytes from all four controllers connected to the tap.
 */
unsigned char pad_buff[NUM_CONTROLLERS][PAD_BUFF_SIZE];

void ctrl_init() {
    // Initialize the pads
    InitPAD((char *) pad_buff[CTRL_PAD_1], PAD_BUFF_SIZE, (char *) pad_buff[CTRL_PAD_2], PAD_BUFF_SIZE);

    // Begin polling
    StartPAD();

    // TO avoid VSYNC timeout error
    ChangeClearPAD(1);

}

Controller *ctrl_read(unsigned char id) {
    return (Controller *) pad_buff[id];
}
