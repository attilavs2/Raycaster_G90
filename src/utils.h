#ifndef utils
#define utils

#include <stdlib.h>

#include "fixed.h"
#include "map.h"

int cmpfunc(const void *, const void *);

fixed_t raycast(ShooterMap*, fixed_t, fixed_t, fixed_t, fixed_t, fixed_t, char);

#endif /* sprites.h */