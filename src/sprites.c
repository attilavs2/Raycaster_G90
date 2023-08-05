s#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>

#include "fixed.h"

#include "map.h"
#include "sprites.h"
#include "moteur.h"

// 2 maps de sprites : Les sprites de la map (tonneaux, torches, cadavres...) qui vont être dans
// map_test.c dans le futur proche et dans les données tiled plus tard
// Et les sprites de PNJs, donc d'ennemis et amicaux si jamais il y en a, qui vont être 
// répertoriés dans un tableau de structs, leur place dans le tableau étant leur ID
// le mob_index sert a garder une position plus facile a vérifier des mobs, le nombre  dans la matrice 
// étant leur ID
//
extern ShooterMap ShooterLevel0

char mob_index[map_w][map_h] = ; //les pnjs (ennemis)

void draw_sprites(image_t *frame_buffer, image_t *sprite) {
	extern fixed_t posX;
    extern fixed_t posY;
    extern fixed_t dirX;
    extern fixed_t dirY;
    extern fixed_t planeX;
    extern fixed_t planeY;
	
	int i, j;

	struct image_linear_map temp;

	fixed_t sprite_ratio;
	fixed_t sprite_dirX;
	fixed_t sprite_dirY;
	
	// 1 - lister les sprites en render distance
	// 2 - vérifier la distance/si c'est derrière un mur
	// 3 - trier par distance
	// 4 - passage a la moulinette de la perspective de la taille
	// 5 - dessiner en commençant par derrière

	int mapX = ffloor(posX);
	int mapY = ffloor(posY);
	//approche bourrine

	for(i = -12; i <= 12; i++){
		for(j = -12; j <= 12; j++){
			if (mob_index[mapX + i][mapY + j] == 0) break;
			
			if (i >= j){
				sprite_ratio = abs(fdiv(fix(j), fix(i)));
				sprite_dirX = sprite_ratio * i / abs(i); //pour avoir le signe correct
				sprite_dirY = (1 - sprite_ratio) * j / abs(j);
			}
			else{
				sprite_ratio = abs(fdiv(fix(i), fix(j)));
				sprite_dirX = (1 - sprite_ratio) * i / abs(i);
				sprite_dirY = sprite_ratio * j / abs(j); //pour avoir le signe correct
			}

			fixed_t sprite_dist = fix((float)sqrt(i*i + j*j));
			if(sprite_dist >= fix(sprite_max_dist)) break;

			if(raycast(posX, posY, sprite_dirX, sprite_dirY, sprite_dist, 2) != -1) break;
			
			sprite_dirX = sprite_dirX - dirX; //Relative direction to the center of screen
			sprite_dirY = sprite_dirY - dirY;

			fixed_t inv_d = 0xFFFF / (fmul(planeX, dirY) - fmul(dirX, planeY));
			fixed_t transX = inv_d * (fmul(dirY, spriteX) - fmul(dirX, spriteY));
			fixed_t transY = inv_d * (fmul(-planeY, spriteX) + fmul(planeX, spriteY));
			int screen_x = (int)(viewport_w * 0.5) * ffloor(0xFFFF + fdvi(transX, transY));

			int spriteHeight = f2int(fdiv(fix(viewport_h), sprite_dist)); //Taille en px de la ligne
      		if (spriteHeight < 1) spriteHeight = 1;
      		fixed_t spriteSize = fix(spriteHeight) / 64; //taille proportionelle de la ligne a la tex

			image_scale(sprite, spriteSize, spriteSize, &temp);
      		image_linear(sprite, image_at(frame_buffer, screen_x, (int)(viewport_h * 0.5 - spriteHeight * 0.5)), &temp); 
		}
	} 
}