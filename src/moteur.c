#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "fixed.h"

#include "moteur.h"
#include "sprites.h"
#include "map_test.h"

// moteur.c :
// ici se trouvent tout ce qui concerne les graphismes, mouvement et collisions
//
//

void move(){
  fixed_t moveSpeed = 0x4CCE;
  fixed_t rotSpeed = 0xFFFF;
  fixed_t c_rotSpeed = 0xFFFF;
  fixed_t s_rotSpeed = 0x0474;

  extern char map_test[map_w][map_h];
  extern fixed_t planeX;
  extern fixed_t planeY;
  extern fixed_t dirX;
  extern fixed_t dirY;
  extern fixed_t posX;
  extern fixed_t posY;

  fixed_t oldDirX;
  fixed_t oldPlaneX;
  int xtemp1;
  int ytemp1;
  int xtemp2;
  int ytemp2;

  pollevent();

	if (keydown(KEY_UP)) {
		xtemp1 = f2int(posX + fmul(dirX, moveSpeed));
    ytemp1 = f2int(posY);
		xtemp2 = f2int(posX);
		ytemp2 = f2int(posY + fmul(dirY, moveSpeed));
    if(map_test[xtemp1][ytemp1] == 0) {
      posX += fmul(dirX, moveSpeed);
    }
    if(map_test[xtemp2][ytemp2] == 0) {
      posY += fmul(dirY, moveSpeed);
    }
  }
  //move backwards if no wall behind you
  if (keydown(KEY_DOWN)) {
    xtemp1 = f2int(posX - fmul(dirX, moveSpeed));
		ytemp1 = f2int(posY);
		xtemp2 = f2int(posX);
		ytemp2 = f2int(posY - fmul(dirY, moveSpeed));
    if(map_test[xtemp1][ytemp1] == 0) {
      posX -= fmul(dirX, moveSpeed);
    }
    if(map_test[xtemp2][ytemp2] == 0) {
      posY -= fmul(dirY, moveSpeed);
    }
  }
  //rotate to the rightdouble sin_rotspeed;
  if (keydown(KEY_RIGHT)) {
    //both camera direction and camera plane must be rotated
	  oldDirX = dirX;
    dirX = (fmul(dirX, c_rotSpeed)+1) - (fmul(dirY, -s_rotSpeed)+1); 
 	  dirY = (fmul(oldDirX, -s_rotSpeed)+1) + (fmul(dirY, c_rotSpeed)+1);
   	oldPlaneX = planeX;
   	planeX = (fmul(planeX, c_rotSpeed)+1) - (fmul(planeY, -s_rotSpeed)+1);
    planeY = (fmul(oldPlaneX, -s_rotSpeed)+1) + (fmul(planeY, c_rotSpeed)+1);
  }
  //rotate to the left
  if (keydown(KEY_LEFT)) {
    //both camera direction and camera plane must be rotated
    oldDirX = dirX;
    dirX = (fmul(dirX, c_rotSpeed)-1) - (fmul(dirY, s_rotSpeed)-1);
    dirY = (fmul(oldDirX, s_rotSpeed)+1) + (fmul(dirY, c_rotSpeed)+1);
    oldPlaneX = planeX;
    planeX = (fmul(planeX, c_rotSpeed)-1) - (fmul(planeY, s_rotSpeed) - 1);
    planeY = (fmul(oldPlaneX, s_rotSpeed)+1) + (fmul(planeY, c_rotSpeed) + 1);
  }
}
void load_map(int map_id){

}

void draw_background(int background_id){
    switch (background_id){
      default: {
        break;
      }
      case 0: {
        break;
      }
      case 1: {
        drect( 0, 0, viewport_w, floor(viewport_h*0.5), 0x6d39);  //~ Bleu ciel
        break;
      }
      case 2: {
        drect( 0, 0, viewport_w, floor(viewport_h*0.5), 0xc1c7);  //Rouge "enfer"
        break;
      }
    }
}
void draw_walls(){
    extern fixed_t posX;
    extern fixed_t posY;
    extern fixed_t dirX;
    extern fixed_t dirY;
    extern fixed_t planeX;
    extern fixed_t planeY;
    extern char map_test[map_w][map_h];

    unsigned short color;
    fixed_t cameraX;
    fixed_t rayDirX;
    fixed_t rayDirY;
    fixed_t sideDistX;//length of ray from current position to next x or y-side
    fixed_t sideDistY;
    fixed_t deltaDistX;
    fixed_t deltaDistY;
    fixed_t perpWallDist;
    int x;
    int mapX;
    int mapY;
    int stepX; //what direction to step in x or y-direction (either +1 or -1)
    int stepY;
    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
    int lineHeight;
    int drawStart;
    int drawEnd;

    for(x = 0; x < viewport_w; x++) {
    
      //calculate ray position and direction
      cameraX = fdiv(fix(x*2), fix(viewport_w)) - 0xFFFF; //x-coordinate in camera space
      
      rayDirX = dirX + fmul(planeX, cameraX);
      rayDirY = dirY + fmul(planeY, cameraX);
      

      //which box of the map we're in
      mapX = f2int(posX);
      mapY = f2int(posY);
      // length of ray from one x or y-side to next x or y-side
      // these are derived as:
      // deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
      // deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
      // which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
      // where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
      // unlike (dirX, dirY) is not 1, however this does not matter, only the
      // ratio between deltaDistX and deltaDistY matters, due to the way the DDA
      // stepping further below works. So the values can be computed as below.
      // Division through zero is prevented, even though technically that's not
      // needed in C++ with IEEE 754 floating point values. 
      // Fcalva : removed the 0 div prevention
      deltaDistX = abs(fdiv(0xFFFF, rayDirX));
      deltaDistY = abs(fdiv(0xFFFF, rayDirY));
      
      //calculate step and initial sideDist
      if(rayDirX < 0) 
      {
        stepX = -1; //true
        sideDistX = fmul(posX - fix(mapX), deltaDistX);
      }
      else
      {
        stepX = 1;
        sideDistX = fmul( fix(mapX + 1) - posX, deltaDistX);
      }
      if(rayDirY < 0)
      {
        stepY = -1; //true
        sideDistY = fmul(posY - fix(mapY), deltaDistY);
      }
      else
      {
        stepY = 1;
        sideDistY = fmul( fix(mapY + 1) - posY, deltaDistY);
      }
      //perform DDA
      while(true) {
        if (sideDistX >= max_dist || sideDistY >= max_dist || mapX < 0 || mapY < 0) {
          hit = 0;
          break;
        }
        else if (map_test[mapX][mapY] != 0) { //Check if ray has hit a wall
          hit = 1;
          break;
        }
        //jump to next map square, either in x-direction, or in y-direction
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
      //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
      //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
      //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
      //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
      //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
      //steps, but we subtract deltaDist once because one step more into the wall was taken above.
      if (side == 0) {
        perpWallDist = (sideDistX - deltaDistX);
      } 
      else {
        perpWallDist = (sideDistY - deltaDistY);
      }
      /*
      dprint(1,10,C_BLACK,"a : %d", a);
      
      dprint(1,50,C_BLACK,"sideDistX : %d", (int)(sideDistX*1000));
      dprint(1,60,C_BLACK,"sideDistY : %d", (int)(sideDistY*1000));
      dprint(1,70,C_BLACK,"perpWallDist : %d", (int)(perpWallDist*1000));
      */
      //Calculate height of line to draw on screen
      lineHeight = f2int(fdiv(fix(viewport_h), perpWallDist));
      //plante avant ici

      //calculate lowest and highest pixel to fill in current stripe
      drawStart = floor((-lineHeight * 0.5) + (viewport_h * 0.5));
      if(drawStart < 0) {
        drawStart = 0;
      }
      drawEnd = floor((lineHeight * 0.5) + (viewport_h * 0.5));
      if(drawEnd >= viewport_h) {
        drawEnd = viewport_h - 1;
      }
      //give x and y sides different brightness
      if (side == 1) {
        color += 4;
      }
      //choose wall color
      switch(map_test[mapX][mapY])
      {
        case 1:   color = C_RED;     break; //red
        case 2:   color = C_GREEN;   break; //green
        case 3:   color = C_BLUE;    break; //blue
        case 4:   color = C_WHITE;   break; //white
        case 5:   color = 0x8000;    break; //rouge foncé
        case 6:   color = 0x0400;    break; //vert foncé
        case 7:   color = 0x0010;    break; //bleu foncé
        case 8:   color = C_LIGHT;   break; //gris clair
        default:  color = C_DARK;    break; //gris foncé
      }

      //draw the pixels of the stripe as a vertical line
      drect(x, drawStart, x, drawEnd, color);
    }
}



// Je garde le bout en dessous pour des raisons (date d'avant la V 0.1.1)
//Problèmes :
//-L'affichage est très mauvais, a revoir
//-Le mouvement de la caméra ne marche pas
//
//A rajouter d'ici le 27 :
//-Que ça marche (logique)
// -collisions
// -Sol
// -Gestion de plusieurs maps
// -Interaction (Portes/boutons)
// -Textures
// -Sprites
// -PNJs ( a voir selon le théme)
// -Extérieur/intérieur (très probablement mais a voir selon le thème)
//La suite dépend surtout de ce qui aura été fait et du thème
//Avec le thème, maintenant il faudrait :
//-collisions
// -sol
// -Des bouts de map reliés entre eux
// -Sprites (ennemis + ambiance dans un second temps)
// -map générées aléatoirement
// -textures
// -Gameplay
// -PNJs
// -Sans de undertale (me demandez pas pourquoi, c'est un mec sur discord qui a propos�)
//
//================================================
// au 03/06 :
// A faire là tout de suite :
// - Bien réparer correctement les murs
// - Sprites
// Plan :
// -Sol
// -Ennemis
// -Maps
// -Textures
//h
