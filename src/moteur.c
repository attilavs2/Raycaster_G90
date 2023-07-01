#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
#include "sprites.h"
#include "map_test.h"

// Je garde le bout en dessous pour des raisons
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
//

void load_map(int){

}

void draw_background(int){
    
}
void draw_walls(){
    extern float posX;
    extern float posY;
    extern float dirX;
    extern float dirY;
    extern float planeX;
    extern float planeY;
    extern char map_test[map_w][map_h];

    unsigned short color;
    float cameraX;
    float rayDirX;
    float rayDirY;
    float sideDistX;//length of ray from current position to next x or y-side
    float sideDistY;
    float deltaDistX;
    float deltaDistY;
    float perpWallDist;
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
      cameraX = 2 * x / viewport_w - 1; //x-coordinate in camera space
      rayDirX = dirX + planeX * cameraX;
      rayDirY = dirY + planeY * cameraX;
      //which box of the map we're in
      mapX = posX;
      mapY = posY;

      //length of ray from one x or y-side to next x or y-side
      //these are derived as:
      //deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
      //deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
      //which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
      //where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
      //unlike (dirX, dirY) is not 1, however this does not matter, only the
      //ratio between deltaDistX and deltaDistY matters, due to the way the DDA
      //stepping further below works. So the values can be computed as below.
      // Division through zero is prevented, even though technically that's not
      // needed in C++ with IEEE 754 floating point values.
      deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
      deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

      //calculate step and initial sideDist
      if(rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if(rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }
      //perform DDA
      while(hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if(sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if(map_test[mapX][mapY] > 0) hit = 1;
      }
      //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
      //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
      //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
      //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
      //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
      //steps, but we subtract deltaDist once because one step more into the wall was taken above.
      if(side == 0) perpWallDist = (sideDistX - deltaDistX);
      else          perpWallDist = (sideDistY - deltaDistY);

      //Calculate height of line to draw on screen
      lineHeight = floor(viewport_h / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      drawStart = floor(-lineHeight / 2 + viewport_h / 2);
      if(drawStart < 0) drawStart = 0;
      drawEnd = floor(lineHeight / 2 + viewport_h / 2);
      if(drawEnd >= viewport_h) drawEnd = viewport_h - 1;

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
        default:  color = 0xffc0;    break; //yellow
      }

      //draw the pixels of the stripe as a vertical line
      dline(x, drawStart, x, drawEnd, color);
    }
}
