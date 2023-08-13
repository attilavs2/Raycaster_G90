#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gint/display.h>
#include <libprof.h>
#include <gint/keyboard.h>

#include "fixed.h"

#include "moteur.h"
#include "sprites.h"
#include "map.h"

// moteur.c :
// ici se trouvent tout ce qui concerne les graphismes, chargement de map, logique, mouvement et collisions
//
//

void move(ShooterMap* ShooterLevel) {
  extern int frame_time;
  fixed_t moveSpeed = fmul(fix(frame_time), 0x148); //frame_time * fix(carrés/seconde/1000) là carrés/seconde = 5
  fixed_t rotSpeed = fmul(fix(frame_time), 0x83); //frame_time * fix(radians/seconde/1000) là radians/seconde = 2
  fixed_t c_rotSpeed = fix(cos(f2float(rotSpeed)));
  fixed_t s_rotSpeed = fix(sin(f2float(rotSpeed)));

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

    if(ShooterLevel->wall[xtemp1][ytemp1] == 0) posX += fmul(dirX, moveSpeed);
    if(ShooterLevel->wall[xtemp2][ytemp2] == 0) posY += fmul(dirY, moveSpeed);
  }
  //move backwards if no wall behind you
  if (keydown(KEY_DOWN)) {
    xtemp1 = f2int(posX - fmul(dirX, moveSpeed));
		ytemp1 = f2int(posY);
		xtemp2 = f2int(posX);
		ytemp2 = f2int(posY - fmul(dirY, moveSpeed));

    if(ShooterLevel->wall[xtemp1][ytemp1] == 0) posX -= fmul(dirX, moveSpeed);
    if(ShooterLevel->wall[xtemp2][ytemp2] == 0) posY -= fmul(dirY, moveSpeed);
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

void draw_background(int background_id, image_t *skybox, image_t *skyboxSlice0, image_t *skyboxSlice1, image_t *frame_buffer){ //a revoir
    extern fixed_t dirX;
    
    switch (background_id){
      default: {
        break;
      }
      case 0: {
        break;
      }
      case 1: {
        int slice0_X = ffloor((dirX + 0xFFFF) * 450);

        image_clear(skyboxSlice0);
        image_clear(skyboxSlice1);
        skyboxSlice0 = image_sub(skybox, slice0_X, 0, viewport_w, (int)(viewport_h * 0.5));
        image_copy(skyboxSlice0, frame_buffer, true);
        /*
        if (slice0_X + viewport_h > 899){
          int slice1_X = 899 - slice0_X + viewport_w;

          skyboxSlice0 = image_sub(skybox, slice0_X, 0, viewport_w - slice1_X, (int)(viewport_h * 0.5));
          skyboxSlice1 = image_sub(skybox, 0, 0, slice1_X, (int)(viewport_h * 0.5));

          image_copy(skyboxSlice1, image_at(frame_buffer, 0, 0), true);
          image_copy(skyboxSlice0, image_at(frame_buffer, -(slice1_X - viewport_w), 0), true);
        }
        else {
          
        }*/
        break;
      }
    }
}

void logic(image_t *frame_buffer, image_t *D_tex){
  //logique (logique)
}

void load_map(){
  dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();
  //trucs de map
}

void draw_f(image_t *floor_tex, image_t *frame_buffer){ //a refaire
    extern fixed_t posX;
    extern fixed_t posY;
    extern fixed_t dirX;
    extern fixed_t dirY;
    extern fixed_t planeX;
    extern fixed_t planeY;
    
    int x;
    int y;

    for (y = 0; y < viewport_h; y++){
      fixed_t rayDirX0 = dirX - planeX;
      fixed_t rayDirY0 = dirY - planeY;
      fixed_t rayDirX1 = dirX + planeX;
      fixed_t rayDirY1 = dirY + planeY;

      int p = y - (int) (viewport_h * 0.5);

      fixed_t posZ = fmul(fix(0.5), fix(viewport_h));

      fixed_t rowDist = fdiv(posZ, fix(p));

      fixed_t floorStepX = fdiv(fmul(rowDist, (rayDirX1 - rayDirX0)), viewport_w);
      fixed_t floorStepY = fdiv(fmul(rowDist, (rayDirY1 - rayDirY0)), viewport_w);

      fixed_t floorX = posX + fmul(rowDist, rayDirX0);
      fixed_t floorY = posY + fmul(rowDist, rayDirY0);

      for (x = 0; x < viewport_w; ++x){
        int cellX = f2int(floorX);
        int cellY = f2int(floorY);
        
        int tx = (int)ffloor(64 * (floorX - cellX)) & 63;
        int ty = (int)ffloor(64 * (floorX - cellX)) & 63;

        floorX += floorStepX;
        floorY += floorStepY;

        unsigned short color = image_get_pixel(floor_tex, tx, ty);
        //color = (color >> 1) & 0b0111111101111111;
        image_set_pixel(frame_buffer, x, viewport_h - y - 1, color);
      }
    } 

}

void draw_walls(image_t *tex_1, image_t *tex_2, image_t *tex_3, image_t *tex_4, image_t *D_tex, ShooterMap *ShooterLevel,image_t *frame_buffer){
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
    fixed_t texSize;
    int x;
    int mapX;
    int mapY;
    int stepX; //what direction to step in x or y-direction (either +1 or -1)
    int stepY;
    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
    int lineHeight;
    int texX;
    int texSample;
    int texSampleY;

    int v_offset = 0; //(int)(sin(f2int(posX + posY)) * 5); //a raffiner un peu
    fixed_t h_offset = 0; //fix(sin(f2int(posX - posY)) * 0.01);

    struct image_linear_map temp;

    image_t texStripe;

    extern int raycast_time;
    extern int draw_time;

    prof_t rayscat = prof_make();
    prof_t img_drw = prof_make();

    for(x = 0; x < viewport_w; x++) {
		  prof_enter(rayscat);

      //calculate ray position and direction
      cameraX = fdiv(fix(x*2), fix(viewport_w)) - 0xFFFF + h_offset; //x-coordinate in camera space
      
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
      if (rayDirX < 0) {
        stepX = -1; //true
        sideDistX = fmul(posX - fix(mapX), deltaDistX);
      }
      else {
        stepX = 1;
        sideDistX = fmul( fix(mapX + 1) - posX, deltaDistX);
      }

      if (rayDirY < 0) {
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
        if (sideDistX >= max_dist || sideDistY >= max_dist || mapX < 0 || mapY < 0 || mapX >= map_w || mapY >= map_h) {
          hit = 0;
          break;
        }
        //Otherwise check if ray has hit a wall
        else if (ShooterLevel->wall[mapX][mapY] != 0) { 
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

      if (side == 0) perpWallDist = (sideDistX - deltaDistX);
      else perpWallDist = (sideDistY - deltaDistY);

      //texturing calculations

      //calculate value of wallX
      fixed_t wallX; //where exactly the wall was hit
      if (side == 0) wallX = posY + fmul(perpWallDist, rayDirY); 
      else  wallX = posX + fmul(perpWallDist, rayDirX);     
      wallX -= fix(floor(f2int(wallX)));

      //x coordinate on the texture
      texX = fmul(wallX, 64);
      while(texX >= 64 || texX < 0){
        if(texX >= 64) texX -= 64;
        if(texX < 0) texX += 64;
      }
      if(side == 0 && rayDirX > 0) texX = 64 - texX - 1;
      if(side == 1 && rayDirY < 0) texX = 64 - texX - 1;

      lineHeight = f2int(fdiv(fix(viewport_h), perpWallDist)); //Taille en px de la ligne
      if (lineHeight < 1) lineHeight = 1;

      fixed_t texSize = fix(lineHeight) / 64; //taille proportionelle de la ligne a la tex
      if (texSize < 0x400) texSize = 0x400; //0x400 = 1/64 * 2^16
      if (texSize > 0x3D000) { //0x3D000 = 3.8125 * 2^16, 3.8125 = viewport_h/64
        texSample = ffloor(fdiv(fix(viewport_h), texSize));
        texSampleY  = 32 - (int)texSample * 0.5 + 1;
      }
      else {
        texSample = 64;
        texSampleY = 0; 
      }

      prof_leave(rayscat);
      prof_enter(img_drw);

      image_clear(&texStripe);

      switch(ShooterLevel->wall[mapX][mapY]){
        case 1 : texStripe = *image_sub(tex_1, texX, texSampleY, 1, texSample); break;
        case 2 : texStripe = *image_sub(tex_2, texX, texSampleY, 1, texSample); break;
        case 3 : texStripe = *image_sub(tex_3, texX, texSampleY, 1, texSample); break;
        case 4 : texStripe = *image_sub(tex_4, texX, texSampleY, 1, texSample); break;
        default : texStripe = *image_sub(D_tex, texX, texSampleY, 1, texSample); break;
      }

      image_scale(&texStripe, 0xFFFF, texSize, &temp);
      image_linear(&texStripe, image_at(frame_buffer, x, (int)(viewport_h * 0.5 - lineHeight * 0.5) + v_offset), &temp); 

      prof_leave(img_drw);
    }

    raycast_time = (int)prof_time(rayscat)/1000;
    draw_time = (int)prof_time(img_drw)/1000;
}