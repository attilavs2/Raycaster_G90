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