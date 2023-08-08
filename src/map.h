#include "fixed.h"
#ifndef map_test_M
#define map_test_M

#define map_w 128
#define map_h 128

typedef struct{
    //size (Redundant for now as it's hardcoded to 128x128)
    //int w, h;

    //Start info
    fixed_t startpos_x; fixed_t startpos_y;
    fixed_t startdir_x; fixed_t startdir_y;
    fixed_t startplane_x;  fixed_t startplane_y;

    //Floor layer data (Unused for now)
    unsigned char floor[map_w][map_h];
    //Wall layer data
    unsigned char wall[map_w][map_h];
    //Sprite layer data
    unsigned char sprites[map_w][map_h];
    //Mobs layer data (spawn data , unused)
    unsigned char mobs[map_w][map_h];

} ShooterMap;

#endif /* map_test.h */