#ifndef utils
#define utils

#include <stdlib.h>

#include "fixed.h"
#include "map.h"

int cmpfunc(const void *, const void *);

fixed_t raycast(ShooterMap*, fixed_t, fixed_t, fixed_t, fixed_t, fixed_t, char);

void *bsearch (const void *key, const void *base0, size_t nmemb, size_t size,
         int (*compar)(const void *, const void *));
#endif /* sprites.h */