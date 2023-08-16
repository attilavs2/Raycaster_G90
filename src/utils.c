#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>

#include "utils.h"
#include "map.h"
#include "moteur.h"

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
/*
 * Copyright (c) 1990 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*

@deftypefn Supplemental void* bsearch (const void *@var{key}, @
  const void *@var{base}, size_t @var{nmemb}, size_t @var{size}, @
  int (*@var{compar})(const void *, const void *))

Performs a search over an array of @var{nmemb} elements pointed to by
@var{base} for a member that matches the object pointed to by @var{key}.
The size of each member is specified by @var{size}.  The array contents
should be sorted in ascending order according to the @var{compar}
comparison function.  This routine should take two arguments pointing to
the @var{key} and to an array member, in that order, and should return an
integer less than, equal to, or greater than zero if the @var{key} object
is respectively less than, matching, or greater than the array member.

@end deftypefn

*/

/*
 * Perform a binary search.
 *
 * The code below is a bit sneaky.  After a comparison fails, we
 * divide the work in half by moving either left or right. If lim
 * is odd, moving left simply involves halving lim: e.g., when lim
 * is 5 we look at item 2, so we change lim to 2 so that we will
 * look at items 0 & 1.  If lim is even, the same applies.  If lim
 * is odd, moving right again involes halving lim, this time moving
 * the base up one item past p: e.g., when lim is 5 we change base
 * to item 3 and make lim 2 so that we will look at items 3 and 4.
 * If lim is even, however, we have to shrink it by one before
 * halving: e.g., when lim is 4, we still looked at item 2, so we
 * have to make lim 3, then halve, obtaining 1, so that we will only
 * look at item 3.
 */
void *
bsearch (const void *key, const void *base0,
         size_t nmemb, size_t size,
         int (*compar)(const void *, const void *))
{
	const char *base = (const char *) base0;
	int lim, cmp;
	const void *p;

	for (lim = nmemb; lim != 0; lim >>= 1) {
		p = base + (lim >> 1) * size;
		cmp = (*compar)(key, p);
		if (cmp == 0)
			return (void *)p;
		if (cmp > 0) {	/* key > p: move right */
			base = (const char *)p + size;
			lim--;
		} /* else move left */
	}
	return (NULL);
}

//Function using the same raycast logic returning distance (and without the same comments)
//Returns -1 if it didn't hit anything, -2 if incorrect type input
// Type 0 = Mobs & Walls; Type 1 = Mobs; Type 2 = Walls
// ! a tester !

fixed_t raycast(ShooterMap *ShooterLevel, fixed_t posX, fixed_t posY, fixed_t rayDirX, fixed_t rayDirY, fixed_t dist, char type){
    extern char mob_index[map_w][map_h];
    
    fixed_t sideDistX;
    fixed_t sideDistY;
    fixed_t deltaDistX;
    fixed_t deltaDistY;
    fixed_t wallDist;

    char side;

    int mapX;
    int mapY;
    int stepX;
    
    int stepY;
    int hit = 0;
    
    mapX = f2int(posX);
    mapY = f2int(posY);

    deltaDistX = abs(fdiv(0xFFFF, rayDirX));
    deltaDistY = abs(fdiv(0xFFFF, rayDirY));
      
    if (rayDirX <= 0) {
      stepX = -1;
      sideDistX = fmul(posX - fix(mapX), deltaDistX);
    }
    else {
      stepX = 1;
      sideDistX = fmul( fix(mapX + 1) - posX, deltaDistX);
    }

    if (rayDirY <= 0) {
      stepY = -1;
      sideDistY = fmul(posY - fix(mapY), deltaDistY);
    }
    else {
      stepY = 1;
      sideDistY = fmul( fix(mapY + 1) - posY, deltaDistY);
    }
    //perform DDA
    switch(type){
      //Walls and mobs raycast
      case 0 : {
        while(true) {
          //Check if the ray is out of range/bounds
          if (sideDistX >= dist || sideDistY >= dist || mapX < 0 || mapY < 0) {
            hit = 0;
            break;
          }
          //Otherwise check if ray has hit a wall/mob
          else if (ShooterLevel->wall[mapX][mapY] != 0 || mob_index[mapX][mapY] != 0) { 
            hit = 1;
            break;
          }
          if (sideDistX < sideDistY) {
            sideDistX += deltaDistX; 
            mapX += stepX; 
            side = 0;
          }
          else {
            sideDistY += deltaDistY;
            mapY += stepY; 
            side = 1;
          }
       
        }
        break;
      }
      //Mobs only
      case 1 : {
        while(true) {
          //Check if the ray is out of range/bounds
          if (sideDistX >= dist || sideDistY >= dist || mapX < 0 || mapY < 0) {
            hit = 0;
            break;
          }
          //Otherwise check if ray has hit a wall
          else if (ShooterLevel->wall[mapX][mapY] != 0) { 
            hit = 1;
            break;
          }
          if (sideDistX < sideDistY) {
            sideDistX += deltaDistX; 
            mapX += stepX; 
            side = 0;
          }
          else {
            sideDistY += deltaDistY;
            mapY += stepY; 
            side = 1;
          }
        }
        break;
      }
      //Walls only
      case 2 : {
        while(true) {
          //Check if the ray is out of range/bounds
          if (sideDistX >= dist || sideDistY >= dist || mapX < 0 || mapY < 0) {
            hit = 0;
            break;
          }
          //Otherwise check if ray has hit a mob
          else if (mob_index[mapX][mapY] != 0) { 
            hit = 1;
            break;
          }
          if (sideDistX < sideDistY) {
            sideDistX += deltaDistX; 
            mapX += stepX; 
            side = 0;
          }
          else {
            sideDistY += deltaDistY;
            mapY += stepY; 
            side = 1;
          }
        }
        break;
      }
      default : {
        return -2;
        break;
      }
    }
    
    if (hit == 0) wallDist = -1;
    else if (side == 0) wallDist = (sideDistX - deltaDistX);
    else wallDist = (sideDistY - deltaDistY);

    return wallDist;
}