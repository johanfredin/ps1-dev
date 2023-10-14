/* $PSLibId: Run-time Library Release 4.7$ */
#ifndef _LIBGPU_H_
#define _LIBGPU_H_
/*
 *  (C) Copyright 1993-1995 Sony Corporation,Tokyo,Japan. All Rights Reserved
 *
 * 		libgpu.h: Graphic Primitive Structures Database
 *
 * Primitive list:
 *
 *      Name     |Size*1|Shade  |Vertex |Texture| Function
 *      ---------+------+-------+-------+-------+------------------------
 *      POLY_F3  | 5	|Flat   |   3   |OFF    | Flat Triangle
 *      POLY_FT3 | 8	|Flat   |   3   |ON     | Flat Textured Triangle
 *      POLY_G3  | 7	|Gouraud|   3   |OFF    | Gouraud Triangle
 *      POLY_GT3 |10	|Gouraud|   3   |ON     | Gouraud Textured Triangle
 *      POLY_F4  | 6	|Flat   |   4   |OFF    | Flat Quadrangle
 *      POLY_FT4 |10	|Flat   |   4   |ON     | Flat Textured Quadrangle
 *      POLY_G4  | 9	|Gouraud|   4   |OFF    | Gouraud Quadrangle
 *      POLY_GT4 |13	|Gouraud|   4   |ON     | Gouraud Textured Quadrangle
 *      ---------+------+-------+-------+-------+------------------------
 *      LINE_F2  | 4	|Flat   |   2   | -     | unconnected Flat Line 
 *      LINE_G2  | 5	|Gouraud|   2   | -     | unconnected Gouraud Line 
 *      LINE_F3  | 6	|Flat	|   3	| -     | 3-connected Flat Line
 *      LINE_G3  | 8	|Gouraud|   3	| -     | 3-connected Gouraud Line
 *      LINE_F4  | 7	|Flat	|   4	| -    	| 4-connected Flat Line
 *      LINE_G4  |10	|Gouraud|   4	| -    	| 4-connected Gouraud Line
 *      ---------+------+-------+-------+-------+------------------------
 *      SPRT	 | 5	|Flat	|   1   |ON     | free size Sprite
 *      SPRT_16	 | 4	|Flat	|   1   |ON     | 16x16 Sprite
 *      SPRT_8	 | 4	|Flat	|   1   |ON     | 8x8 Sprite
 *      ---------+------+-------+-------+-------+------------------------
 *      TILE	 | 4	|Flat	|   1   |OFF    | free size Sprite
 *      TILE_16	 | 3	|Flat	|   1   |OFF    | 16x16 Sprite
 *      TILE_8	 | 3	|Flat	|   1   |OFF    | 8x8 Sprite
 *      TILE_1	 | 3	|Flat	|   1   |OFF    | 1x1 Sprite
 *      ---------+------+-------+-------+-------+------------------------
 *      DR_TWIN	 | 3	|   -	|   -   | -     | Texture Window
 *      DR_AREA	 | 3	|   -	|   -   | -     | Drawing Area
 *      DR_OFFSET| 3	|   -	|   -   | -     | Drawing Offset
 *      DR_MODE  | 3	|   -	|   -   | -     | Drawing Mode
 *      DR_ENV   |16	|   -	|   -	| -     | Drawing Environment
 *      DR_MOVE  | 6	|   -	|   -	| -     | MoveImage
 *      DR_LOAD  |17	|   -	|   -	| -     | LoadImage
 *      DR_TPAGE | 2    |   -   |   -   | -     | Drawing TPage
 *      DR_STP   | 3    |   -   |   -   | -     | Drawing STP
 *
 *	*1: in long-word
 *
 * Texture Attributes:
 *	abr: ambient rate
 *		abr	0	1	2	3
 *		-------------------------------------
 *		Front	0.5	1.0	0.5	-1.0
 *		Back	0.5	1.0	1.0	 1.0
 *
 *	tp: texture mode
 *		 tp	0	1	2	
 *		 -----------------------------
 *		 depth	4bit	8bit	16bit
 *		 color	CLUT	CLUT	DIRECT
 */

#include <libgte.h>
#include <types.h>

/*
 *	Externals
 */
extern int (*GPU_printf)(char *fmt, ...);    /* printf() object */

/*
 *	Time-out Cycle
 */
#define WAIT_TIME    0x800000

/*
 *	General Macros
 */
#define limitRange(x, l, h)    ((x)=((x)<(l)?(l):(x)>(h)?(h):(x)))

/*
 *	Set/Add Vector/Rectangle Attributes
 */
#define setVector(v, _x, _y, _z) \
    (v)->vx = _x, (v)->vy = _y, (v)->vz = _z

#define applyVector(v, _x, _y, _z, op) \
    (v)->vx op _x, (v)->vy op _y, (v)->vz op _z

#define copyVector(v0, v1) \
    (v0)->vx = (v1)->vx, (v0)->vy = (v1)->vy, (v0)->vz = (v1)->vz

#define addVector(v0, v1) \
    (v0)->vx += (v1)->vx,    \
    (v0)->vy += (v1)->vy,    \
    (v0)->vz += (v1)->vz

#define dumpVector(str, v)    \
    GPU_printf("%s=(%d,%d,%d)\n", str, (v)->vx, (v)->vy, (v)->vz)

#define dumpMatrix(x)    \
    GPU_printf("\t%5d,%5d,%5d\n",(x)->m[0][0],(x)->m[0][1],(x)->m[0][2]),\
    GPU_printf("\t%5d,%5d,%5d\n",(x)->m[1][0],(x)->m[1][1],(x)->m[1][2]),\
    GPU_printf("\t%5d,%5d,%5d\n",(x)->m[2][0],(x)->m[2][1],(x)->m[2][2])

#define setRECT(r, _x, _y, _w, _h) \
    (r)->x = (_x),(r)->y = (_y),(r)->w = (_w),(r)->h = (_h)

/*
 *	Set Primitive Attributes
 */
#define setTPage(p, tp, abr, x, y) \
    ((p)->tpage = getTPage(tp,abr,x,y))

#define setClut(p, x, y) \
    ((p)->clut = getClut(x,y))

/*
 * Set Primitive Colors
 */
#define setRGB0(p, _r0, _g0, _b0)                        \
    (p)->r0 = _r0,(p)->g0 = _g0,(p)->b0 = _b0

#define setRGB1(p, _r1, _g1, _b1)                        \
    (p)->r1 = _r1,(p)->g1 = _g1,(p)->b1 = _b1

#define setRGB2(p, _r2, _g2, _b2)                        \
    (p)->r2 = _r2,(p)->g2 = _g2,(p)->b2 = _b2

#define setRGB3(p, _r3, _g3, _b3)                        \
    (p)->r3 = _r3,(p)->g3 = _g3,(p)->b3 = _b3

/*
 * Set Primitive Screen Points
 */
#define setXY0(p, _x0, _y0)                        \
    (p)->x0 = (_x0), (p)->y0 = (_y0)                \

#define setXY2(p, _x0, _y0, _x1, _y1)                    \
    (p)->x0 = (_x0), (p)->y0 = (_y0),                \
    (p)->x1 = (_x1), (p)->y1 = (_y1)

#define setXY3(p, _x0, _y0, _x1, _y1, _x2, _y2)                \
    (p)->x0 = (_x0), (p)->y0 = (_y0),                \
    (p)->x1 = (_x1), (p)->y1 = (_y1),                \
    (p)->x2 = (_x2), (p)->y2 = (_y2)

#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3)            \
    (p)->x0 = (_x0), (p)->y0 = (_y0),                \
    (p)->x1 = (_x1), (p)->y1 = (_y1),                \
    (p)->x2 = (_x2), (p)->y2 = (_y2),                \
    (p)->x3 = (_x3), (p)->y3 = (_y3)

#define setXYWH(p, _x0, _y0, _w, _h)                    \
    (p)->x0 = (_x0),      (p)->y0 = (_y0),                \
    (p)->x1 = (_x0)+(_w), (p)->y1 = (_y0),                \
    (p)->x2 = (_x0),      (p)->y2 = (_y0)+(_h),            \
    (p)->x3 = (_x0)+(_w), (p)->y3 = (_y0)+(_h)

/*
 * Set Primitive Width/Height
 */
#define setWH(p, _w, _h)    (p)->w = _w, (p)->h = _h

/*
 * Set Primitive Texture Points
 */
#define setUV0(p, _u0, _v0)                        \
    (p)->u0 = (_u0), (p)->v0 = (_v0)                \

#define setUV3(p, _u0, _v0, _u1, _v1, _u2, _v2)                \
    (p)->u0 = (_u0), (p)->v0 = (_v0),                \
    (p)->u1 = (_u1), (p)->v1 = (_v1),                \
    (p)->u2 = (_u2), (p)->v2 = (_v2)

#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3)            \
    (p)->u0 = (_u0), (p)->v0 = (_v0),                \
    (p)->u1 = (_u1), (p)->v1 = (_v1),                \
    (p)->u2 = (_u2), (p)->v2 = (_v2),                \
    (p)->u3 = (_u3), (p)->v3 = (_v3)

#define setUVWH(p, _u0, _v0, _w, _h)                    \
    (p)->u0 = (_u0),      (p)->v0 = (_v0),                \
    (p)->u1 = (_u0)+(_w), (p)->v1 = (_v0),                \
    (p)->u2 = (_u0),      (p)->v2 = (_v0)+(_h),            \
    (p)->u3 = (_u0)+(_w), (p)->v3 = (_v0)+(_h)


/*
 * Dump Primivie Parameters
 */
#define dumpRECT(r)    \
    GPU_printf("(%d,%d)-(%d,%d)\n", (r)->x,(r)->y,(r)->w,(r)->h)

#define dumpWH(p)    GPU_printf("(%d,%d)\n", (p)->w,  (p)->h )
#define dumpXY0(p)    GPU_printf("(%d,%d)\n", (p)->x0, (p)->y0)
#define dumpUV0(p)    GPU_printf("(%d,%d)\n", (p)->u0, (p)->v0)

#define dumpXY2(p)                            \
    GPU_printf("(%d,%d)-(%d,%d)\n",                    \
    (p)->x0, (p)->y0, (p)->x1, (p)->y1)

#define dumpXY3(p)                            \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)\n",                \
    (p)->x0, (p)->y0, (p)->x1, (p)->y1,                \
    (p)->x2, (p)->y2)

#define dumpUV3(p)                            \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)\n",                \
    (p)->u0, (p)->v0, (p)->u1, (p)->v1,                \
    (p)->u2, (p)->v2)

#define dumpXY4(p)                            \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)-(%d,%d)\n",            \
    (p)->x0, (p)->y0, (p)->x1, (p)->y1,                \
    (p)->x2, (p)->y2, (p)->x3, (p)->y3)

#define dumpUV4(p)                            \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)-(%d,%d)\n",            \
    (p)->u0, (p)->v0, (p)->u1, (p)->v1,                \
    (p)->u2, (p)->v2, (p)->u3, (p)->v3)

#define dumpRGB0(p)                            \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r0, (p)->g0, (p)->b0)

#define dumpRGB1(p)                            \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r1, (p)->g1, (p)->b1)

#define dumpRGB2(p)                            \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r2, (p)->g2, (p)->b2)

#define dumpRGB3(p)                            \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r3, (p)->g3, (p)->b3)

/*
 * Primitive Handling Macros
 */
#define setlen(p, _len)    (((P_TAG *)(p))->len  = (u_char)(_len))
#define setaddr(p, _addr)    (((P_TAG *)(p))->addr = (u_long)(_addr))
#define setcode(p, _code)    (((P_TAG *)(p))->code = (u_char)(_code))

#define getlen(p)            (u_char)(((P_TAG *)(p))->len)
#define getcode(p)        (u_char)(((P_TAG *)(p))->code)
#define getaddr(p)        (u_long)(((P_TAG *)(p))->addr)

/**
 * Get pointer to next primitive in primitive list.
 * @param p Pointer to start address of a primitive
 * @return Pointer to the next primitive.
 */
#define nextPrim(p)        (void *)((((P_TAG *)(p))->addr)|0x80000000)
#define isendprim(p)        ((((P_TAG *)(p))->addr)==0xffffff)

#define addPrim(ot, p)        setaddr(p, getaddr(ot)), setaddr(ot, p)
#define addPrims(ot, p0, p1)    setaddr(p1, getaddr(ot)),setaddr(ot, p0)

#define catPrim(p0, p1)        setaddr(p0, p1)
#define termPrim(p)        setaddr(p, 0xffffffff)

#define setSemiTrans(p, abe) \
    ((abe)?setcode(p, getcode(p)|0x02):setcode(p, getcode(p)&~0x02))

#define setShadeTex(p, tge) \
    ((tge)?setcode(p, getcode(p)|0x01):setcode(p, getcode(p)&~0x01))

/**
 * Calculates the texture page ID, and returns it.
 * The semitransparent rate is also effective for polygons on which texture mapping is not performed.
 * The texture page address is limited to a multiple of 64 in the X direction and a multiple of 256 in the Y  direction.
 * @param tp texture pointer
 */
#define getTPage(tp, abr, x, y)                    \
     ((((tp)&0x3)<<7)|(((abr)&0x3)<<5)|(((y)&0x100)>>4)|(((x)&0x3ff)>>6)| \
     (((y)&0x200)<<2))

#define getClut(x, y) \
    (((y)<<6)|(((x)>>4)&0x3f))

#define dumpTPage(tpage)                        \
    GPU_printf("tpage: (%d,%d,%d,%d)\n",                \
               ((tpage)>>7)&0x003,((tpage)>>5)&0x003,    \
               ((tpage)<<6)&0x7c0,                \
               (((tpage)<<4)&0x100)+(((tpage)>>2)&0x200))

#define dumpClut(clut) \
    GPU_printf("clut: (%d,%d)\n", (clut&0x3f)<<4, (clut>>6))

#define _get_mode(dfe, dtd, tpage)    \
        ((0xe1000000)|((dtd)?0x0200:0)| \
        ((dfe)?0x0400:0)|((tpage)&0x9ff))

#define setDrawTPage(p, dfe, dtd, tpage)    \
    setlen(p, 1),    \
    ((u_long *)(p))[1] = _get_mode(dfe, dtd, tpage)

#define _get_tw(tw)    \
        (tw ? ((0xe2000000)|((((tw)->y&0xff)>>3)<<15)| \
        ((((tw)->x&0xff)>>3)<<10)|(((~((tw)->h-1)&0xff)>>3)<<5)| \
        (((~((tw)->w-1)&0xff)>>3))) : 0)

#define setTexWindow(p, tw)            \
    setlen(p, 2),                \
    ((u_long *)(p))[1] = _get_tw(tw),    \
    ((u_long *)(p))[2] = 0

#define _get_len(rect)    \
        (((screen_rect)->w*(screen_rect)->h+1)/2+4)

#define setDrawLoad(pt, rect)                    \
    (_get_len(screen_rect) <= 16) ? (                \
        (setlen(pt, _get_len(screen_rect))),            \
        ((pt)->code[0] = 0xa0000000),            \
        ((pt)->code[1] = *((u_long *)&(screen_rect)->x)),    \
        ((pt)->code[2] = *((u_long *)&(screen_rect)->w)),    \
        ((pt)->p[_get_len(screen_rect)-4] = 0x01000000)    \
    ) : ( \
        (setlen(pt,0)) \
    )

#define setDrawStp(p, pbw)                \
        setlen(p, 2),                    \
        ((u_long *)p)[1] = 0xe6000000|(pbw?0x01:0),    \
        ((u_long *)p)[2] = 0

#define setDrawMode(p, dfe, dtd, tpage, tw)        \
        setlen(p, 2),                    \
        ((u_long *)p)[1] = _get_mode(dfe, dtd, tpage),    \
        ((u_long *)p)[2] = _get_tw((RECT *)tw)


/*	Primitive 	Lentgh		Code				*/
/*--------------------------------------------------------------------	*/
/*									*/
#define setPolyF3(p)    setlen(p, 4),  setcode(p, 0x20)
#define setPolyFT3(p)    setlen(p, 7),  setcode(p, 0x24)
#define setPolyG3(p)    setlen(p, 6),  setcode(p, 0x30)
#define setPolyGT3(p)    setlen(p, 9),  setcode(p, 0x34)
#define setPolyF4(p)    setlen(p, 5),  setcode(p, 0x28)
#define setPolyFT4(p)    setlen(p, 9),  setcode(p, 0x2c)
#define setPolyG4(p)    setlen(p, 8),  setcode(p, 0x38)
#define setPolyGT4(p)    setlen(p, 12), setcode(p, 0x3c)

#define setSprt8(p)    setlen(p, 3),  setcode(p, 0x74)
#define setSprt16(p)    setlen(p, 3),  setcode(p, 0x7c)
#define setSprt(p)    setlen(p, 4),  setcode(p, 0x64)

#define setTile1(p)    setlen(p, 2),  setcode(p, 0x68)
#define setTile8(p)    setlen(p, 2),  setcode(p, 0x70)
#define setTile16(p)    setlen(p, 2),  setcode(p, 0x78)
#define setTile(p)    setlen(p, 3),  setcode(p, 0x60)
#define setLineF2(p)    setlen(p, 3),  setcode(p, 0x40)
#define setLineG2(p)    setlen(p, 4),  setcode(p, 0x50)
#define setLineF3(p)    setlen(p, 5),  setcode(p, 0x48),(p)->pad = 0x55555555
#define setLineG3(p)    setlen(p, 7),  setcode(p, 0x58),(p)->pad = 0x55555555, \
            (p)->p2 = 0
#define setLineF4(p)    setlen(p, 6),  setcode(p, 0x4c),(p)->pad = 0x55555555
#define setLineG4(p)    setlen(p, 9),  setcode(p, 0x5c),(p)->pad = 0x55555555, \
            (p)->p2 = 0, (p)->p3 = 0

/*
 * Rectangle:
 */
typedef struct {
    short x, y;        /* offset point on VRAM */
    short w, h;        /* width and height */
} RECT;

typedef struct {
    int x, y;        /* offset point on VRAM */
    int w, h;        /* width and height */
} RECT32;

/*
 * Environment 
 */
typedef struct {
    u_long tag;
    u_long code[15];
} DR_ENV;                /* Packed Drawing Environment */

typedef struct {
    /**
     * Drawing area. Drawing is restricted to the area specified by clip. It must be
     * within the area area (0, 0) - (1023, 511).
     */
    RECT clip;        /* clip area */
    /**
     * The offsets ofs[0] and ofs[1] are added to the X and Y values, respectively, of
     * all primitives before drawing. Note: Addresses after adding offsets are
     * wrapped around at (-1024, -1024) - (1023, 1023).
     */
    short ofs[2];        /* drawing offset */
    /**
     * Texture window. Specifies a rectangle inside the texture page, to be used
     * for drawing textures.
     */
    RECT tw;        /* texture window */
    /** Initial value of texture page */
    u_short tpage;        /* texture page */
    /** Dithering processing flag. 0: off; 1: on */
    u_char dtd;        /* dither flag (0:off, 1:on) */
    /**
     * 0: drawing to display area is blocked<br>
     * 1: drawing to display area is permitted
     */
    u_char dfe;        /* flag to draw on display area (0:off 1:on) */
    /**
     * Enable to auto-clear
     * 0: Does not clear drawing area when drawing environment is set.
     * 1: Paints entire clip area with brightness values (r0, g0, b0) when drawing
     * environment is set.
     */
    u_char isbg;        /* enable to auto-clear */
    /** Background color. Valid only when isbg is 1. */
    u_char r0, g0, b0;    /* initital background color */
    // Reserved
    DR_ENV dr_env;        /* reserved */
} DRAWENV;

typedef struct {
    RECT disp;        /* display area */
    RECT screen;        /* display start point */
    /* fix contributed by StiNKz */
    /* SDK loads these bytes as a single word, */
    /* which could cause crashes due to alignment issues */
    union {
        u_long _data;
        struct {
            u_char isinter;    /* interlace 0: off 1: on */
            u_char isrgb24;    /* RGB24 bit mode */
            u_char pad0, pad1;    /* reserved */
        };
    };
} DISPENV;

/*
 * Polygon Primitive Definitions
 */
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    u_char r0, g0, b0, code;
} P_TAG;

typedef struct {
    u_char r0, g0, b0, code;
} P_CODE;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
} POLY_F3;                /* Flat Triangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;                /* Flat Quadrangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
    short x1, y1;
    u_char u1, v1;
    u_short tpage;
    short x2, y2;
    u_char u2, v2;
    u_short pad1;
} POLY_FT3;                /* Flat Textured Triangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
    short x1, y1;
    u_char u1, v1;
    u_short tpage;
    short x2, y2;
    u_char u2, v2;
    u_short pad1;
    short x3, y3;
    u_char u3, v3;
    u_short pad2;
} POLY_FT4;                /* Flat Textured Quadrangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, pad1;
    short x1, y1;
    u_char r2, g2, b2, pad2;
    short x2, y2;
} POLY_G3;                /* Gouraud Triangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, pad1;
    short x1, y1;
    u_char r2, g2, b2, pad2;
    short x2, y2;
    u_char r3, g3, b3, pad3;
    short x3, y3;
} POLY_G4;                /* Gouraud Quadrangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char u1, v1;
    u_short tpage;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_char u2, v2;
    u_short pad2;
} POLY_GT3;                /* Gouraud Textured Triangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char u1, v1;
    u_short tpage;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_char u2, v2;
    u_short pad2;
    u_char r3, g3, b3, p3;
    short x3, y3;
    u_char u3, v3;
    u_short pad3;
} POLY_GT4;                /* Gouraud Textured Quadrangle */

/*
 * Line Primitive Definitions
 */
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
} LINE_F2;                /* Unconnected Flat Line */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, p1;
    short x1, y1;
} LINE_G2;                /* Unconnected Gouraud Line */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    u_long pad;
} LINE_F3;                /* 2 connected Flat Line */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_long pad;
} LINE_G3;                /* 2 connected Gouraud Line */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
    u_long pad;
} LINE_F4;                /* 3 connected Flat Line Quadrangle */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_char r3, g3, b3, p3;
    short x3, y3;
    u_long pad;
} LINE_G4;                /* 3 connected Gouraud Line */

/**
 * Sprite of any desired size.<br>
 * Sprite Primitive Definitions<br>
 * <strong>u_long</strong> *tag; Pointer to next primitive (reserved)<br>
 * <strong>u_char</strong> r0, g0, b0; RGB color values for sprite<br>
 * <strong>u_char</strong> code; Primitive code (reserved)<br>
 * <strong>short</strong> x0, y0; Position of sprite (top left coordinate)<br>
 * <strong>u_char</strong> u0, v0; Position of sprite texture within the texture page (top left coordinate). u0<br>
 * <strong>should</strong> be an even number.<br>
 * <strong>u_short</strong> clut; CLUT ID used (for 4-bit/8-bit mode only)<br>
 * <strong>short</strong> w, h; Width and height of sprite. w is an even number
 */
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
    short w, h;
} SPRT;                    /* free size Sprite */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
} SPRT_16;                /* 16x16 Sprite */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0;
    u_short clut;
} SPRT_8;                /* 8x8 Sprite */

/*
 * Tile Primitive Definitions
 */
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;                    /* free size Tile */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
} TILE_16;                /* 16x16 Tile */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
} TILE_8;                /* 8x8 Tile */

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
} TILE_1;                /* 1x1 Tile */

/*
 *  Special Primitive Definitions
 */
typedef struct {
    u_long tag;
    u_long code[2];
} DR_MODE;                /* Drawing Mode */

typedef struct {
    u_long tag;
    u_long code[2];
} DR_TWIN;                /* Texture Window */

typedef struct {
    u_long tag;
    u_long code[2];
} DR_AREA;                /* Drawing Area */

/**
 * Drawing offset modification primitive.
 * Changes the drawing offset parameters of the current drawing environment while a primitive list is being
 * drawn. See the ofs member of the DRAWENV structure for more information. Use SetDrawOffset() to
 * specify the parameters to be used.
 */
typedef struct {
    // Pointer to the next primitive in primitive list
    u_long tag;
    // New drawing offset information specified by SetDrawOffset()
    u_long code[2];
} DR_OFFSET;                /* Drawing Offset */

typedef struct {            /* MoveImage */
    u_long tag;
    u_long code[5];
} DR_MOVE;

/**
 * Load Image primitive.
 * typedef struct {<br>
 *  u_long *tag; Pointer to next primitive (reserved)<br>
 *  u_long code[3]; Primitive ID<br>
 *  u_long p[13]; Transfer data<br>
 * } DR_LOAD;
 * Transfers data below array p to the frame buffer. As with LoadImage(), semitransparent/ transparent color
 * control is not performed. Also, there is no dependence on the drawing environment.
 * Maximum data transfer amount is 12 words (24 pixels).
 */
typedef struct {            /* LoadImage */
    u_long tag;
    u_long code[3];
    u_long p[13];
} DR_LOAD;

typedef struct {
    u_long tag;
    u_long code[1];
} DR_TPAGE;                /* Drawing TPage */

typedef struct {
    u_long tag;
    u_long code[2];
} DR_STP;                               /* Drawing STP */

/*
 *	Font Stream Parameters
 */
#define FNT_MAX_ID    8    /* max number of stream ID */
#define FNT_MAX_SPRT    1024    /* max number of sprites in all streams */

/*
 *	Multi-purpose Sony-TMD primitive
 */
typedef struct {
    u_long id;
    u_char r0, g0, b0, p0;        /* Color of vertex 0 */
    u_char r1, g1, b1, p1;        /* Color of vertex 1 */
    u_char r2, g2, b2, p2;        /* Color of vertex 2 */
    u_char r3, g3, b3, p3;        /* Color of vertex 3 */
    u_short tpage, clut;        /* texture page ID, clut ID */
    u_char u0, v0, u1, v1;        /* texture corner point */
    u_char u2, v2, u3, v3;

    /* independent vertex model */
    SVECTOR x0, x1, x2, x3;        /* 3D corner point */
    SVECTOR n0, n1, n2, n3;        /* 3D corner normal vector */

    /* Common vertex model */
    SVECTOR *v_ofs;            /* offset to vertex database */
    SVECTOR *n_ofs;            /* offset to normal database */

    u_short vert0, vert1;        /* index of vertex */
    u_short vert2, vert3;
    u_short norm0, norm1;        /* index of normal */
    u_short norm2, norm3;


} TMD_PRIM;

/**
 * TIM data header information is acquired by ReadTIM().
 * crect and caddr are assigned a value of zero for TIM having no CLUT.
 */
typedef struct {
    /* pixel mode */
    u_long mode;
    /* CLUT rectangle on frame buffer */
    RECT *crect;
    /* CLUT address on main memory */
    u_long *caddr;
    /* texture image rectangle on frame buffer */
    RECT *prect;
    /* texture image address on main memory */
    u_long *paddr;
} TIM_IMAGE;

/*
 * Prototypes
 */
#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#ifndef _FNTPRINT_
#define _FNTPRINT_
extern int FntPrint(...);
#endif /* _FNTPRINT_ */
#ifndef _KANJIFNTPRINT_
#define _KANJIFNTPRINT_
extern int KanjiFntPrint(...);
#endif /* _KANJIFNTPRINT_ */
#else
#ifndef _FNTPRINT_
#define _FNTPRINT_

extern int FntPrint(void *, ...);

#endif /* _FNTPRINT_ */
#ifndef _KANJIFNTPRINT_
#define _KANJIFNTPRINT_

extern int KanjiFntPrint();

#endif /* _KANJIFNTPRINT_ */
#endif

extern DISPENV *GetDispEnv(DISPENV *env);

/**
 * Sets a display environment according to information specified by env.
 * @param env Pointer to display environment start address
 * @return A pointer to the display environment set; on failure, returns 0.
 */
extern DISPENV *PutDispEnv(DISPENV *env);

/**
 * Sets the members of a DISPENV (display environment) structure. The new display area is specified using
 * the coordinates within the frame buffer of the top left corner, along with the width and height, of the desired
 * rectangle.<p>
 * This function does not actually change the display environment. It merely sets the members of the specified
 * structure as desired. Use PutDispEnv() with this structure to change the actual environment.<br>
 * <strong>Note</strong>: While the screen width and height are set to (0, 0), internally they are processed as (256, 240).
 * @param env Pointer to display environment
 * @param x Upper left corner of display area
 * @param y Upper left corner of display area
 * @param w Width of the display area
 * @param h Height of the display area
 * @return Pointer to the display environment set.
 */
extern DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);

extern DRAWENV *GetDrawEnv(DRAWENV *env);

/**
 * Set the drawing environment.<br>
 * The basic drawing parameters (such as the drawing offset and the drawing clip area) are set according to
 * the values specified in env.
 * The drawing environment is effective until the next time PutDrawEnv() is executed, or until the DR_ENV
 * primitive is executed.
 * @param env Pointer to drawing environment start address
 * @return A pointer to the drawing environment set. On failure, returns 0.
 */
extern DRAWENV *PutDrawEnv(DRAWENV *env);

/**
 * Set standard drawing environment structure.<br>
 * Sets the drawing area members of a DRAWENV (drawing environment) structure. The new drawing area is
 * specified using the coordinates within the frame buffer of the top left corner, along with the width and
 * height, of the desired rectangle.<br>
 * <br>
 * This function does not actually change the drawing environment. It merely sets the members of the
 * specified structure as desired. Use PutDrawEnv() with this structure to change the actual environment.
 * @param env Pointer to drawing environment
 * @param x Upper left corner of drawing area x pos
 * @param y Upper left corner of drawing area y pos
 * @param w Width of drawing area
 * @param h Height of drawing area
 * @return The starting pointer of the drawing environment set.
 */
extern DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);

/**
 * Produce a TIM header<br>
 * Sets the members of the TIM_IMAGE structure pointed to by timimg according to the data specified by the
 * most recent OpenTIM() call.
 * @param timimg TIM_IMAGE structure pointer
 * @return The timimg start address, if succesful; 0 if TIM analysis fails.
 */
extern TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg);

extern TMD_PRIM *ReadTMD(TMD_PRIM *tmdprim);

extern int CheckPrim(char *s, u_long *p);

extern int ClearImage(RECT *rect, u_char r, u_char g, u_char b);

extern int ClearImage2(RECT *rect, u_char r, u_char g, u_char b);

/**
 * Waits for drawing to terminate.<br>
 * If DrawSync(0) is used, and execution of the primitive list takes an exceptionally long time (approximately
 * longer than 8 Vsync) to complete, a timeout is generated and the GPU is reset. Reasons why this might
 * occur include an exceptionally long primitive list, or one that renders exceptionally large numbers of pixels.
 * Another possibility is that the primitive list has been corrupted in some way. To avoid this, the application
 * can use a loop such as:<br>
 * while(DrawSync(1));<br>
 * <br>
 * The following routines use the GPU queue, and therefore their termination can be detected using
 * DrawSync(), or by setting a callback with DrawSyncCallback(): ClearImage(), ClearImage2(), DrawOTag(),
 * DrawOTagEnv(), LoadImage(), MoveImage(), PutDrawEnv(), StoreImage().
 * @param mode <ul><li>0: Wait for termination of all non-blocking functions registered in the queue</li><li>1: Return the number of positions in the current queue</li></ul>
 * @return Number of positions in the execution queue.
 */
extern int DrawSync(int mode);

/**
 * Opens the stream for on-screen printing. After this, character strings up to n characters long can be drawn
 * in the (x, y)- (x+w, y+h) rectangular area of the frame buffer, using FntPrint(). If isbg is 1, the background is
 * cleared when a character string is drawn.
 * Up to 8 streams can be opened at once. However, once a stream is opened, it cannot be closed until the
 * next time FntLoad() is called.
 * n specifies the maximum number of characters. Up to 1024 characters can be specified together in 8
 * streams.
 * @param x Display start x location
 * @param y Display start y location
 * @param w Display area w
 * @param h Display area h
 * @param isbg Automatic clearing of background
 * 0: Don’t clear background to (0, 0, 0) when display is performed
 * 1: Clear background to (0, 0, 0) when display is performed
 * @param n Maximum number of characters
 * @return The stream ID
 */
extern int FntOpen(int x, int y, int w, int h, int isbg, int n);

extern int GetGraphDebug(void);

extern int GetTimSize(u_char *sjis);

extern int IsEndPrim(void *p);

extern int KanjiFntOpen(int x, int y, int w, int h, int dx, int dy, int cx, int cy, int isbg, int n);

extern void KanjiFntClose(void);

extern int Krom2Tim(u_char *sjis, u_long *taddr, int dx, int dy, int cdx, int cdy, u_int fg, u_int bg);

/**
 * Transfer data to a frame buffer.<br>
 * Transfers the contents of memory from the address p to the rectangular area in the frame buffer specified
 * by recp.<br>
 * Because LoadImage() is a non-blocking function, transmission termination must be detected by DrawSync()
 * or by installing a callback routine with DrawSyncCallback().<br>
 * The source and destination areas are not affected by the drawing environment (clip, offset). The destination
 * area must be located within a drawable area (0, 0) - (1023, 511). See the description of the DR_LOAD primitive.
 * @param rect Pointer to destination rectangular area.
 * @param p Pointer to main memory address of source of transmission
 * @return Position of this command in the libgpu command queue.
 */
extern int LoadImage(RECT *rect, u_long *p);

extern int MargePrim(void *p0, void *p1);

extern int MoveImage(RECT *rect, int x, int y);

/**
 * Opens a TIM in main memory. The information in the opened TIM can then be read using ReadTIM().
 * Only one TIM can be opened at a time. An opened TIM is not closed until the next time OpenTIM() is called.
 * @param addr Pointer to main memory address to which the TIM has been loaded
 * @return 0 on success; any other value indicates failure.
 */
extern int OpenTIM(u_long *addr);

extern int OpenTMD(u_long *tmd, int obj_no);

/**
 * Resets the graphic system according to mode:
 * <ul>
 * <li>0: Complete reset. The drawing environment and display environment are initialized.</li>
 * <li>1: Cancels the current drawing and flushes the command buffer.</li>
 * <li>3: Initializes the drawing engine while preserving the current display environment (i.e. the screen is not cleared or the screen mode changed)</li>
 * </ul>
 * @param mode Reset mode
 * @return
 */
extern int ResetGraph(int mode);

extern int SetGraphDebug(int level);

extern int StoreImage(RECT *rect, u_long *p);

/**
 * Walks the array specified by ot and sets each element to be a pointer to the following element, except the
 * last, which is set to a pointer to a special terminator value which the PlayStation® uses to recognize the
 * end of a primitive list. n specifies the number entries in the array.
 * To execute the OT initialized by ClearOTag(), call DrawOTag(ot).
 * @param ot OT starting pointer
 * @param n Number of entries in OT
 * @return
 */
extern u_long *ClearOTag(u_long *ot, int n);

extern u_long *ClearOTagR(u_long *ot, int n);

/**
 * Draws the contents of the print stream into the frame buffer. It initializes and then draws a sprite primitive
 * list corresponding to the characters specified in the print stream.
 * When id is -1, the print stream ID which was set in SetDumpFnt() is used (0 if print stream ID was not set).
 * After the drawing has been done, the print stream contents are also flushed.
 * @param id Print stream ID
 * @return The starting pointer of the primitive list used to perform the drawing.
 */
extern u_long *FntFlush(int id);

extern u_long *KanjiFntFlush(int id);

extern u_long DrawSyncCallback(void (*func)(void));

extern u_short GetClut(int x, int y);

/**
 * Calculate value of member tpage in a primitive
 * @param tp Texture mode<ul><li>0: 4bitCLUT</li><li>1: 8bitCLUT</li><li>2: 16bitDirect</li></ul>
 * @param abr Semitransparency rate<ul><li>0: 0.5 x Back + 0.5 x Forward</li><li>1: 1.0 x Back + 1.0 x Forward</li><li>2: 1.0 x Back - 1.0 x Forward</li><li>3: 1.0 x Back + 0.25 x Forward</li></ul>
 * @param x Texture page address
 * @param y Texture page address
 * @return Texture page ID
 */
extern u_short GetTPage(int tp, int abr, int x, int y);

/**
 * Load 256-color CLUT<br>
 * Loads 256 entries of texture color data (CLUT) from main memory address clut into the frame buffer (x,y)
 * and calculates the ID of the loaded texture CLUT.
 * 256 palette entries are always transmitted, even in 4-bit mode.
 * @param clut Pointer to CLUT data start address
 * @param x Destination coordinate x in frame buffer
 * @param y Destination coordinate y in frame buffer
 * @return The CLUT ID for the loaded CLUT.
 */
extern u_short LoadClut(u_long *clut, int x, int y);

/**
 * Load 16-color CLUT<br>
 * Loads 16 entries of texture color data (CLUT) from main memory address clut into the frame buffer (x,y) and
 * calculates the ID of the loaded texture CLUT.
 * LoadClut2() transmits 16 palette entries whereas LoadClut() transmits 256 palette entries.
 * LoadClut2() internally invokes LoadImage().
 * @param clut Pointer to CLUT data start address
 * @param x Destination coordinate x in frame buffer
 * @param y Destination coordinate y in frame buffer
 * @return The CLUT ID for the loaded CLUT.
 */
extern u_short LoadClut2(u_long *clut, int x, int y);

/**
 * Loads a texture page
 * Loads a texture pattern from the memory area starting at the address pix into the frame buffer area starting
 * at the address (x, y), and calculates the texture page ID for the loaded texture pattern.<br>
 * The texture pattern size w represents the number of pixels, not the actual size of the transfer area in the
 * frame buffer.<br>
 * LoadTPage() calls LoadImage() internally.
 * @param pix Pointer to texture pattern start address
 * @param tp Bit depth (0 = 4-bit; 1 = 8-bit; 2 = 16-bit)
 * @param abr Semitransparency rate
 * @param x Destination frame buffer address x pos
 * @param y Destination frame buffer address y pos
 * @param w Destination frame buffer address width
 * @param h Destination frame buffer address height
 * @return Texture page ID for the loaded texture pattern
 */
extern u_short LoadTPage(u_long *pix, int tp, int abr, int x, int y, int w, int h);

/**
 * Get pointer to next primitive in primitive list.
 * @param p Pointer to start address of a primitive
 * @return Pointer to the next primitive.
 */
extern void *NextPrim(void *p);

/**
 * Register a primitive to the OT.<br>
 * Registers a primitive beginning with the address *p to the OT entry *ot in OT table. ot is an ordering table or
 * pointer to another primitive.
 * A primitive may be added to a primitive list only once in the same frame. Attempting to add it multiple times
 * in the same frame results in a corrupted list.
 * @param ot OT entry
 * @param p Start address of primitive to be registered
 */
extern void AddPrim(void *ot, void *p);

/**
 * Collectively register primitives to the OT.<br>
 * Registers primitives beginning with p0 and ending with p1 to the *ot entry in the OT.
 * The primitive list is a list of primitives connected by AddPrim() or created by the local ordering table.
 * @param ot OT entry
 * @param p0 Start address of primitive list
 * @param p1 End address of primitive list
 */
extern void AddPrims(void *ot, void *p0, void *p1);

extern void CatPrim(void *p0, void *p1);

/**
 * Executes the GPU primitives in the linked list ot.
 * DrawOTag() is non-blocking. To detect when execution of the primitive list is complete, use DrawSync() or
 * install a callback routine with DrawSyncCallback().
 * @param p Pointer to a linked list of GPU primitives
 */
extern void DrawOTag(u_long *p);

extern void DrawOTagIO(u_long *p);

extern void DrawOTagEnv(u_long *p, DRAWENV *env);

extern void DrawPrim(void *p);

extern void DumpClut(u_short clut);

extern void DumpDispEnv(DISPENV *env);

extern void DumpDrawEnv(DRAWENV *env);

extern void DumpOTag(u_long *p);

extern void DumpTPage(u_short tpage);

/**
 * Transmits the built-in text font used for debugging text output to the frame buffer. It loads the basic font
 * pattern (4-bit, 256x128) and initializes all the print streams.
 * FntLoad() must always be executed before FntOpen() and FntFlush(). The font area must not clash with the
 * frame buffer area used by the application. Font data is located at the upper left of the texture page for
 * FntFlush(). Font data is treated as a RECT (0,0,32,32) area consisting of 128 characters, each 128 x 32. As
 * this is similar to the texture page area, tx is restricted to a multiple of 64 and ty is restricted to a multiple of
 * 256.
 * Loads the Clut to location (tx, ty+128).
 * @param tx frame buffer x pos
 * @param ty frame buffer y pos
 */
extern void FntLoad(int tx, int ty);

/**
 * Puts display mask into the status specified by mask. mask =0: not displayed on screen; mask = 1;
 * displayed on screen.
 * @param mask Display mask
 */
extern void SetDispMask(int mask);

extern void SetDrawArea(DR_AREA *p, RECT *r);

extern void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env);

extern void SetDrawLoad(DR_LOAD *p, RECT *rect);

extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw);

extern void SetDrawTPage(DR_TPAGE *p, int dfe, int dtd, int tpage);

extern void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y);

/**
 * Initializes a DR_OFFSET primitive. By using AddPrim() to insert a DR_OFFSET primitive into your primitive
 * list, it is possible to change part of your drawing environment in the middle of drawing.
 * @param p Pointer to drawing offset setting primitive
 * @param ofs Pointer to drawing offset
 */
extern void SetDrawOffset(DR_OFFSET *p, u_short *ofs);

extern void SetDrawStp(DR_STP *p, int pbw);

/**
 * Sets the print stream for debug printing. The output of the debug printing functions can then be carried out
 * in relation to the stream specified in id.
 * The actual display is executed by FntFlush().
 * @param id Print stream ID
 */
extern void SetDumpFnt(int id);

extern void SetLineF2(LINE_F2 *p);

extern void SetLineF3(LINE_F3 *p);

extern void SetLineF4(LINE_F4 *p);

extern void SetLineG2(LINE_G2 *p);

extern void SetLineG3(LINE_G3 *p);

extern void SetLineG4(LINE_G4 *p);

extern void SetPolyF3(POLY_F3 *p);

extern void SetPolyF4(POLY_F4 *p);

extern void SetPolyFT3(POLY_FT3 *p);

extern void SetPolyFT4(POLY_FT4 *p);

extern void SetPolyG3(POLY_G3 *p);

extern void SetPolyG4(POLY_G4 *p);

extern void SetPolyGT3(POLY_GT3 *p);

extern void SetPolyGT4(POLY_GT4 *p);

extern void SetSemiTrans(void *p, int abe);

extern void SetShadeTex(void *p, int tge);

extern void SetSprt(SPRT *p);

extern void SetSprt16(SPRT_16 *p);

extern void SetSprt8(SPRT_8 *p);

extern void SetTexWindow(DR_TWIN *p, RECT *tw);

extern void SetTile(TILE *p);

extern void SetTile1(TILE_1 *p);

extern void SetTile16(TILE_16 *p);

extern void SetTile8(TILE_8 *p);

extern void TermPrim(void *p);

extern u_long *BreakDraw(void);

extern void ContinueDraw(u_long *insaddr, u_long *contaddr);

extern int IsIdleGPU(int max_count);

extern int GetODE(void);

extern int LoadImage2(RECT *rect, u_long *p);

extern int StoreImage2(RECT *rect, u_long *p);

extern int MoveImage2(RECT *rect, int x, int y);

extern int DrawOTag2(u_long *p);

extern void GetDrawMode(DR_MODE *p);

extern void GetTexWindow(DR_TWIN *p);

extern void GetDrawArea(DR_AREA *p);

extern void GetDrawOffset(DR_OFFSET *p);

extern void GetDrawEnv2(DR_ENV *p);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif /* _LIBGPU_H_ */
