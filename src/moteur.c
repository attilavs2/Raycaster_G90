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
  extern int frame_time;
  fixed_t moveSpeed = (int) frame_time * 0x148; //frame_time * fix(carrés/seconde/1000) dans ce cas carrés/seconde = 5
  fixed_t rotSpeed = (int) frame_time * 0x83; //frame_time * fix(radians/seconde/1000) dans ce cas radians/seconde = 2
  fixed_t c_rotSpeed = fix(cos(f2float(rotSpeed)));
  fixed_t s_rotSpeed = fix(sin(f2float(rotSpeed)));

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
  if (dirX > 0xFFFF) dirX = 0xFFFF;
	if (dirY > 0xFFFF) dirY = 0xFFFF;
	if (dirX < -0xFFFF) dirX = -0xFFFF;
	if (dirY < -0xFFFF) dirY = -0xFFFF;
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

void draw_walls(image_t *XOR_tex, image_t *frame_buffer){
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

    image_fill(frame_buffer, C_LIGHT);

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
      if (rayDirX <= 0) {
        stepX = -1; //true
        sideDistX = fmul(posX - fix(mapX), deltaDistX);
      }
      else {
        stepX = 1;
        sideDistX = fmul( fix(mapX + 1) - posX, deltaDistX);
      }

      if (rayDirY <= 0) {
        stepY = -1; //true
        sideDistY = fmul(posY - fix(mapY), deltaDistY);
      }
      else {
        stepY = 1;
        sideDistY = fmul( fix(mapY + 1) - posY, deltaDistY);
      }
      //perform DDA
      while(true) {
        //Check if the ray is out of range/bounds
        if (sideDistX >= max_dist || sideDistY >= max_dist || mapX < 0 || mapY < 0) {
          hit = 0;
          break;
        }
        //Otherwise check if ray has hit a wall
        else if (map_test[mapX][mapY] != 0) { 
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
      //texturing calculations
      //int texNum = test_map[mapX][mapY] - 1; //a voir plus tard

      //calculate value of wallX
      fixed_t wallX; //where exactly the wall was hit
      if (side == 0){
        wallX = posY + fmul(perpWallDist, rayDirY);
      } 
      else  {
        wallX = posX + fmul(perpWallDist, rayDirX);
      }         
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = f2int(fmul(wallX, 64));
      if(side == 0 && rayDirX > 0) {
        texX = 64 - texX - 1;
      }
      if(side == 1 && rayDirY < 0) {
        texX = 64 - texX - 1;
      }
      image_t texStripe = *image_sub(XOR_tex , texX, 0, 1, 64);
      struct image_linear_map temp;
      image_scale(&texStripe, 0xFFFF, fix(drawEnd - drawStart / 64), &temp);
      temp.u = x;
      temp.v = 0;
      image_linear(&texStripe, frame_buffer, &temp);
    }
    dprint(1,1,C_BLACK,"coucou 281"); dupdate(); getkey();
    dimage(0, 0, frame_buffer);
}

//Function using the same raycast logic returning distance
//Returns -1 if it didn't hit anything
// ! a tester !
fixed_t raycast(fixed_t posX, fixed_t posY, fixed_t rayDirX, fixed_t rayDirY, fixed_t dist, char type){
    extern char map_test[map_w][map_h];
    extern char mob_index[map_w][map_h];
    
    fixed_t sideDistX;//length of ray from current position to next x or y-side
    fixed_t sideDistY;
    fixed_t deltaDistX;
    fixed_t deltaDistY;
    fixed_t wallDist;

    char side;

    int mapX;
    int mapY;
    int stepX; //what direction to step in x or y-direction (either +1 or -1)
    int stepY;
    int hit = 0; //was there a wall hit?
      

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
    if (rayDirX <= 0) {
      stepX = -1; //true
      sideDistX = fmul(posX - fix(mapX), deltaDistX);
    }
    else {
      stepX = 1;
      sideDistX = fmul( fix(mapX + 1) - posX, deltaDistX);
    }

    if (rayDirY <= 0) {
      stepY = -1; //true
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
          else if (map_test[mapX][mapY] != 0 || mob_index[mapX][mapY] != 0) { 
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
          //Otherwise check if ray has hit a wall/mob
          else if (map_test[mapX][mapY] != 0) { 
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
          //Otherwise check if ray has hit a wall/mob
          else if (mob_index[mapX][mapY] != 0) { 
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
        break;
      }
      default : {
        return -2;
        break;
      }
    }
    
    //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
    //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
    //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
    //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
    //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
    //steps, but we subtract deltaDist once because one step more into the wall was taken above.
    if (hit == 0){
      wallDist = -1;
    }
    else if (side == 0) {
      wallDist = (sideDistX - deltaDistX);
    } 
    else {
      wallDist = (sideDistY - deltaDistY);
    }
    return wallDist;
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
