#include "../../lib/header/GPUBase.h"

GsOT orderingTable[GPUB_NUM_BUFFERS];
GsOT_TAG minorOrderingTable[GPUB_NUM_BUFFERS][1 << GPUB_OT_LENGTH_STD];

GPUB_OT gpub_ot[GPUB_NUM_BUFFERS];

// Define screen width and height, required by GPUBase
const u_short gpub_screen_w = 320;
const u_short gpub_screen_h = 240;

// Declare shapes
POLY_F3 poly_f3;  			// Flat shaded triangle
POLY_G3 poly_g3;			// Gouraud shaded triangle

POLY_F4 poly_f4;			// Flat shaded rectangle
POLY_G4 poly_g4;			// Gouraud shaded rectangle

TILE tile;					// Tile where width and height can be set
TILE_1 tile1;				// Fixed 1 pixel size tile
TILE_8 tile8;			    // Tile with fixed w/h of 8 pixels
TILE_16 tile16;		        // Tile with fixed w/h of 16 pixels

u_short poly_cnt = 0;

u_long ot[255];

void init_ots() {
    gpub_init_ot(&gpub_ot[0], &orderingTable[0], minorOrderingTable[0], GPUB_OT_LENGTH_STD);
    gpub_init_ot(&gpub_ot[1], &orderingTable[1], minorOrderingTable[1], GPUB_OT_LENGTH_STD);
}

void addPrimes() {
    ClearOTag(ot, 256);
	AddPrim(&ot[poly_cnt++], &poly_f3);
	AddPrim(&ot[poly_cnt++], &poly_g3);
	AddPrim(&ot[poly_cnt++], &tile1);
	AddPrim(&ot[poly_cnt++], &tile);
	AddPrim(&ot[poly_cnt++], &tile8);
	AddPrim(&ot[poly_cnt++], &tile16);
	AddPrim(&ot[poly_cnt++], &poly_f4);
	AddPrim(&ot[poly_cnt++], &poly_g4);
}

void initPolys() {
	// Init poly f3
	SetPolyF3(&poly_f3);
	setRGB0(&poly_f3, 255, 132, 45);
	setXY3(&poly_f3, 10, 50, 50, 50, 10, 90);
	

	// Init gouraud triangle
	SetPolyG3(&poly_g3);
	setRGB0(&poly_g3, 0, 0, 0);
	setRGB1(&poly_g3, 200, 10, 100);
	setRGB2(&poly_g3, 100, 110, 10);
	setXY3(&poly_g3, 60, 50, 20, 90, 60, 90);
	

	// Init 1 pixel tile
	SetTile1(&tile1);
	setXY0(&tile1, 70, 90);
	setRGB0(&tile1, 255, 0, 0);
	

	// Init solid tile
	SetTile(&tile);
	setXY0(&tile, 90, 90);
	setRGB0(&tile, 255, 255, 255);
	setWH(&tile, 50, 50);

	// Init tile 8
	SetTile8(&tile8);
	setXY0(&tile8, 120, 70);
	setRGB0(&tile8, 123, 123, 123);
	

	// Init tile 16
	SetTile16(&tile16);
	setXY0(&tile16, 150, 70);
	setRGB0(&tile16, 211, 50, 55);


	// Init flat rect
	SetPolyF4(&poly_f4);
	setSemiTrans(&poly_f4, 1);
	setRGB0(&poly_f4, 213, 12, 45);
	setXY4(&poly_f4, 40, 120, 80, 120, 40, 160, 80, 160);
	

	// Init gouraud rect
	SetPolyG4(&poly_g4);
	setRGB0(&poly_g4, 10, 10, 10);
	setRGB1(&poly_g4, 0, 255, 30);
	setRGB2(&poly_g4, 0, 0, 255);
	setRGB3(&poly_g4, 255, 0, 0);
	setXY4(&poly_g4, 40, 200, 80, 200, 40, 240, 80, 240);
	
}

void draw() {
    // Clear display
    gpub_clear_display();

    // Assign and clear current ot
    gpub_curr_ot = &gpub_ot[gpub_current_buffer];
    gpub_clear_ot(gpub_curr_ot);

    // Sort game objects into current ot
    DrawOTag(ot);

    // Call drawsync anc vsync and swap buffers
    gpub_display_sync();
    gpub_sort_clear(gpub_curr_ot);

    // Draw the ot
    gpub_draw_ot(gpub_curr_ot, 0, 0);
}

int main() {
    Color bg_color = {25, 0, 50};
    gpub_init_screen(GPUB_MODE_NTSC, GPUB_FPS_60);
    init_ots();
    GPUB_INIT_DEBUG_FNT_DEFAULT();
    gpub_set_bg_color(&bg_color);
    initPolys();
    addPrimes();

    while (1) {
        FntPrint("Draw Primes in ot demo\n");
        draw();
    }
}
