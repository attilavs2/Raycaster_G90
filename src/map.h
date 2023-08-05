#include "fixed.h"
#ifndef map_test_M
#define map_test_M

#define map_w 128
#define map_h 128
//#define startpos_x (fixed_t) 1370352//exprimé en cases<<16
//#define startpos_y (fixed_t) 1817445

typedef struct{
    //size
    int w, h;

    //Start info
    fixed_t startpos_x; fixed_t startpos_y;
    fixed_t startdir_x; fixed_t startdir_y;
    fixed_t startplane_x;   fixed_t startplane_y;

    //Floor layer data (Unused for now)
    floor[w][h];
    //Wall layer data
    wall[w][h];
    //Sprite layer data
    sprites[w][h];
    //Mobs layer data (spawn data , unused)
    mobs[w][h];

} ShooterMap;

extern ShooterMap ShooterLevel0;

#endif /* map_test.h */