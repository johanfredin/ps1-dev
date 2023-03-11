// font is mig68000, available for free with attribution, made by Zingot
// Games, https://www.zingot.com/ & https://zingot.itch.io/fontpack

static const unsigned char s_systemFont[] = {
    // offsets to luts and bitstream
    0x5a, 0x87,

    // binary tree
    0x04, 0x46, 0x06, 0x3e, 0x08, 0x34, 0x0a, 0x2e, 0x0c, 0x16, 0x00, 0x0e, 0x10, 0xfb, 0x12, 0x14, 0xff, 0xfe, 0xfd,
    0xfc, 0x18, 0x2a, 0x1a, 0x22, 0x1c, 0xf6, 0x1e, 0x20, 0xfa, 0xf9, 0xf8, 0xf7, 0x24, 0x26, 0xf5, 0xf4, 0x28, 0xf1,
    0xf3, 0xf2, 0xf0, 0x2c, 0xef, 0xee, 0xed, 0x30, 0xec, 0x32, 0xeb, 0xea, 0xe9, 0x36, 0x38, 0xe4, 0xe8, 0x3a, 0x3c,
    0xe5, 0xe7, 0xe6, 0xe3, 0x40, 0xe2, 0x42, 0x44, 0xdf, 0xe1, 0xe0, 0xde, 0x48, 0xdd, 0x4a, 0x4c, 0x4e, 0xdc, 0xdb,
    0x50, 0xd4, 0x52, 0xd5, 0x54, 0xd6, 0xda, 0x56, 0xd9, 0x58, 0xd8, 0xd7,

    // lut
    0x33, 0x73, 0xdc, 0x3b, 0x67, 0x03, 0x6e, 0x6f, 0x3f, 0x56, 0x78, 0x0f, 0x37, 0x1f, 0x35, 0x08, 0x76, 0xc0, 0x1b,
    0x3e, 0x7c, 0x6b, 0x70, 0x7e, 0x1c, 0x7b, 0x77, 0x0e, 0x63, 0x18, 0x06, 0x7f, 0x38, 0x60, 0x00, 0x66, 0x30, 0x0c,
    0xc3, 0x6c, 0xde, 0xff, 0x1e, 0x36, 0x3c,

    // bitstream
    0xe9, 0x7d, 0x95, 0xab, 0x73, 0xab, 0x82, 0xfb, 0xfa, 0xdf, 0x25, 0x97, 0xfe, 0xaf, 0xea, 0xa7, 0xf7, 0x55, 0x98,
    0x5c, 0xa6, 0x2a, 0x38, 0x62, 0x88, 0x5c, 0x72, 0x89, 0xa8, 0x2a, 0x4a, 0xef, 0x8b, 0xed, 0x91, 0xa4, 0xa9, 0xf2,
    0x7b, 0x6e, 0xdf, 0xae, 0xdf, 0x24, 0x9f, 0x2d, 0xbd, 0x2f, 0x81, 0x7e, 0x24, 0xc6, 0x9d, 0x2a, 0xbf, 0xe7, 0xf6,
    0xed, 0xfa, 0x4d, 0xc2, 0xc7, 0x5e, 0xca, 0x31, 0x68, 0x5d, 0xe6, 0x9e, 0xaf, 0xea, 0x4c, 0xde, 0xbf, 0xc8, 0xfc,
    0x26, 0xb9, 0xc4, 0xf8, 0x94, 0x63, 0x50, 0xdd, 0x7b, 0xcf, 0x57, 0x75, 0x60, 0xc2, 0xfb, 0x97, 0xcc, 0x6f, 0x12,
    0x43, 0xae, 0x4f, 0xbd, 0x08, 0x1e, 0x64, 0xf7, 0x3c, 0x4e, 0x51, 0x22, 0x1d, 0x17, 0xf0, 0xe8, 0x7c, 0x2a, 0x76,
    0xf8, 0x93, 0x7a, 0x31, 0x3d, 0xd6, 0x3d, 0x8f, 0x53, 0x94, 0x48, 0xae, 0x0b, 0x78, 0x3a, 0x1f, 0x2b, 0x76, 0xf8,
    0x93, 0x72, 0xfc, 0x85, 0x75, 0xcf, 0x57, 0x75, 0x21, 0x90, 0xbc, 0xe3, 0xa7, 0xf9, 0x62, 0xc8, 0x55, 0xca, 0xf1,
    0x8f, 0x60, 0xdd, 0xf3, 0x55, 0xdd, 0x89, 0xf1, 0x8e, 0x9f, 0xe6, 0xcb, 0x25, 0x46, 0xd5, 0x4b, 0xb0, 0xc5, 0xb8,
    0x93, 0x92, 0x4d, 0x77, 0x67, 0xd3, 0x8e, 0x84, 0x8f, 0x5d, 0xf5, 0x22, 0x88, 0x97, 0x34, 0x29, 0xd9, 0xc4, 0xde,
    0xd9, 0xb4, 0x23, 0xf9, 0xac, 0xd4, 0x2b, 0x10, 0x8f, 0xe5, 0x32, 0xa5, 0x04, 0x26, 0x11, 0x1d, 0x22, 0x11, 0x23,
    0x55, 0x4a, 0xbd, 0x02, 0x40, 0x76, 0x6e, 0x29, 0x81, 0xbf, 0xe8, 0x9f, 0xff, 0xf5, 0x19, 0xa9, 0x52, 0x55, 0x55,
    0x15, 0x53, 0x55, 0x55, 0x55, 0x4c, 0x55, 0x55, 0x55, 0xd5, 0xad, 0xaa, 0xaa, 0xaa, 0x5b, 0x85, 0x3e, 0x82, 0x7c,
    0x12, 0xfd, 0x7d, 0xcf, 0x8d, 0xf3, 0x8f, 0x9e, 0xe8, 0xd3, 0xc6, 0x1f, 0x1e, 0x9c, 0x0f, 0xfe, 0xd0, 0x73, 0x88,
    0x30, 0x44, 0x44, 0xfb, 0x9e, 0x1b, 0xe7, 0x44, 0x10, 0x11, 0xd1, 0xc6, 0x1f, 0x1e, 0x9c, 0x0f, 0xfe, 0xd0, 0xe3,
    0x1b, 0x1b, 0x66, 0xb6, 0xc9, 0x03, 0xc6, 0x0a, 0x7a, 0x03, 0x36, 0x6d, 0x7c, 0x3b, 0xd7, 0x0e, 0x13, 0xdf, 0x6c,
    0x66, 0x9b, 0x3c, 0x60, 0xac, 0x10, 0x6e, 0xc0, 0xa6, 0x8d, 0x6f, 0xe7, 0xda, 0x61, 0x62, 0xd9, 0x6c, 0x66, 0x36,
    0x79, 0x78, 0x73, 0x9c, 0x0d, 0x98, 0x69, 0xe3, 0xff, 0xeb, 0xde, 0xf3, 0xc6, 0xb2, 0xd9, 0xcc, 0x6c, 0xf2, 0xf0,
    0xe6, 0xb0, 0x6c, 0xc0, 0xd8, 0xda, 0xf8, 0xff, 0xba, 0xf7, 0xbc, 0xe1, 0x08, 0xd9, 0x9e, 0x4f, 0x18, 0xc9, 0x43,
    0x31, 0x1c, 0x88, 0x11, 0x80, 0x18, 0x6d, 0x38, 0xe9, 0x5e, 0x1d, 0x1e, 0x8e, 0x90, 0xed, 0xf9, 0x84, 0x91, 0x3c,
    0x14, 0xc3, 0xf1, 0x21, 0x00, 0xb9, 0xda, 0x70, 0xd2, 0xbd, 0x3a, 0x3c, 0x96, 0xcd, 0x66, 0xb6, 0xc9, 0xc3, 0x1b,
    0xe7, 0x33, 0xe0, 0xc5, 0x6b, 0x73, 0xbe, 0xec, 0x9e, 0x53, 0x2c, 0x9b, 0xcd, 0x6c, 0x93, 0x87, 0x37, 0xce, 0x67,
    0xc0, 0xeb, 0xb5, 0x39, 0x5f, 0x76, 0xcf, 0x29, 0xf0, 0x66, 0x33, 0xdb, 0x34, 0xc0, 0x38, 0x9f, 0x81, 0xda, 0xb4,
    0xcf, 0x3a, 0x81, 0xaf, 0x3f, 0x05, 0xde, 0xd8, 0x30, 0xb3, 0x4d, 0x03, 0x8c, 0xf3, 0x19, 0xa8, 0x4d, 0xfb, 0xac,
    0x13, 0xf8, 0xfa, 0x53, 0xd0, 0x8c, 0x30, 0x44, 0x46, 0xfb, 0xc4, 0x89, 0x73, 0x32, 0x67, 0x24, 0x12, 0x7f, 0x90,
    0xf3, 0xe1, 0x7e, 0x3d, 0x87, 0x20, 0x9f, 0x64, 0xd8, 0xfe, 0xe7, 0xc4, 0xf9, 0x37, 0xda, 0xd7, 0x17, 0x7f, 0x90,
    0xf3, 0xe1, 0x7e, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x4e, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0xe5, 0x28, 0xcb, 0xe7, 0x66, 0x3a, 0xa6, 0xaa, 0xea, 0x56, 0x15, 0x5e, 0xec, 0x52, 0x96, 0xef, 0x67, 0x3a, 0xa6,
    0xaa, 0xea, 0x56, 0x15, 0xa4, 0x9e, 0xa5, 0x2c, 0xdf, 0xcd, 0xca, 0x54, 0x55, 0xdd, 0xaa, 0x92, 0x54, 0xca, 0xf2,
    0xdd, 0xac, 0x4c, 0x55, 0xd5, 0xad, 0x2a, 0x49, 0xe5, 0x7e, 0xf4, 0xb1, 0x8f, 0x30, 0xa4, 0x33, 0x41, 0x45, 0x1f,
    0x61, 0xe8, 0xa3, 0x8d, 0xf3, 0x91, 0xa4, 0x3a, 0x18, 0x21, 0x22, 0x44, 0xa4, 0x33, 0xb1, 0x12, 0x11, 0x11, 0x42,
    0x1b, 0xe7, 0x23, 0x49, 0x95, 0x9f, 0x6d, 0x77, 0xd3, 0x79, 0xe5, 0x6c, 0xdb, 0xec, 0x6e, 0xfc, 0xdd, 0x61, 0x0b,
    0x5f, 0xf9, 0xd9, 0x76, 0x37, 0x9d, 0x57, 0xce, 0xb6, 0xcd, 0xee, 0xc6, 0xdf, 0x1d, 0xb6, 0xf0, 0x15, 0x36, 0x1b,
    0xdd, 0x4d, 0xe7, 0x29, 0xdc, 0xb6, 0x19, 0xba, 0x1b, 0x8e, 0x99, 0x24, 0x55, 0x34, 0x1b, 0xdd, 0x4d, 0xe7, 0x29,
    0xdc, 0xb6, 0x19, 0x76, 0x37, 0x1c, 0x33, 0x49, 0xaa, 0x36, 0x9b, 0x5d, 0x9a, 0xce, 0x2b, 0xbe, 0x11, 0x99, 0xbf,
    0xfb, 0xce, 0xfb, 0xaf, 0xa4, 0x6a, 0xb3, 0xd9, 0xc5, 0xa6, 0xf3, 0x8a, 0x6f, 0x08, 0x33, 0x7f, 0xf7, 0x9d, 0xf7,
    0x5f, 0x49, 0x15, 0x11, 0x22, 0x74, 0xfd, 0xdc, 0x33, 0x97, 0x8f, 0xcc, 0x1b, 0x7d, 0x12, 0x2b, 0x17, 0x41, 0xea,
    0x59, 0x61, 0xe8, 0x63, 0xff, 0xfa, 0x9d, 0xb3, 0xc3, 0xf7, 0xcd, 0x1b, 0x72, 0x31, 0xbd, 0x5c, 0x84, 0x17, 0xbb,
    0xaa, 0x2a, 0xac, 0x50, 0x55, 0xe6, 0xab, 0xaa, 0x5b, 0x55, 0x55, 0xd5, 0xaf, 0x67, 0x55, 0xe6, 0xab, 0xaa, 0x5b,
    0x55};
