#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>

#include "fixed.h"

#include "map.h"
#include "sprites.h"
#include "moteur.h"
#include "utils.h"

// 2 maps de sprites : Les sprites de la map (tonneaux, torches, cadavres...) qui vont être dans
// map_test.c dans le futur proche et dans les données tiled plus tard
// Et les sprites de PNJs, donc d'ennemis et amicaux si jamais il y en a, qui vont être 
// répertoriés dans un tableau de structs, leur place dans le tableau étant leur ID
// le mob_index sert a garder une position plus facile a vérifier des mobs, le nombre  dans la matrice 
// étant leur ID
//

char mob_index[map_w][map_h]; //les pnjs (ennemis)

void draw_sprites(image_t *frame_buffer, image_t *sprite, ShooterMap *ShooterLevel) {
	extern fixed_t posX;
    extern fixed_t posY;
    extern fixed_t dirX;
    extern fixed_t dirY;
    extern fixed_t planeX;
    extern fixed_t planeY;
	
	int i, j;

	struct image_linear_map temp;

	int sprite_list[max_sprite_search][4];
	int sprite_list2[max_sprite_search][4];
	int dist_list[max_sprite_search];
	char list_counter = 0;

	fixed_t sprite_ratio;
	fixed_t sprite_dirX;
	fixed_t sprite_dirY;

	int mapX = ffloor(posX);
	int mapY = ffloor(posY);

	for(i = -12; i <= 12; i++){
		for(j = -12; j <= 12; j++){
			if (ShooterLevel->sprites[mapX + i][mapY + j] == 0) break;
			
			if (list_counter > max_sprite_search - 1) break;

			int sprite_dist = i*i + j*j;
			//if(sprite_dist >= sprite_max_dist*sprite_max_dist) break;

			//if(raycast(ShooterLevel, posX, posY, sprite_dirX, sprite_dirY, sprite_dist, 2) != -1) break;

			sprite_list[list_counter][0] = sprite_dist;
			sprite_list[list_counter][1] = ShooterLevel->sprites[mapX + i][mapY + j];
			sprite_list[list_counter][2] = i;
			sprite_list[list_counter][3] = j;
			list_counter++;
		}
		if (list_counter > max_sprite_search - 1) break;
	}

	if(list_counter == 0) return;

	for(i = 0; i < list_counter; i++){
		dist_list[i] = sprite_list[i][0];
	}

	qsort(&dist_list[0], list_counter, sizeof(int), cmpfunc);

	if(list_counter > max_sprite_display) list_counter = max_sprite_display;

	for(i = 0; i < list_counter; i++){
		int *b = (int*)bsearch(&dist_list[i], &sprite_list, list_counter, 4 * sizeof(int), cmpfunc);
		int c = (int)(&sprite_list[0][0] - b) / sizeof(int);
		sprite_list2[i][0] = dist_list[i];
		sprite_list2[i][1] = sprite_list[c][1];
		sprite_list2[i][2] = sprite_list[c][2];
		sprite_list2[i][3] = sprite_list[c][3]; 
	}

	for(i = list_counter; i >= 0; i--) {

		int spriteX = mapX + sprite_list[i][2];
		int spriteY = mapY + sprite_list[i][3];

		if (sprite_list2[i][2] >= sprite_list2[i][3]){
			sprite_ratio = abs(fdiv(fix(sprite_list2[i][3]), fix(sprite_list2[i][2])));
			sprite_dirX = sprite_ratio * sprite_list2[i][2] / abs(sprite_list2[i][2]); //pour avoir le signe correct
			sprite_dirY = (1 - sprite_ratio) * sprite_list2[i][3] / abs(sprite_list2[i][3]);
		}
		else {
			sprite_ratio = abs(fdiv(fix(sprite_list2[i][2]), fix(sprite_list2[i][3])));
			sprite_dirX = (1 - sprite_ratio) * sprite_list2[i][2] / abs(sprite_list2[i][2]);
			sprite_dirY = sprite_ratio * sprite_list2[i][3] / abs(sprite_list2[i][3]); //pour avoir le signe correct
		}

		sprite_dirX = sprite_dirX - dirX;
		sprite_dirY = sprite_dirY - dirY;

		fixed_t inv_d = fdiv(0xFFFF, (fmul(planeX, dirY) - fmul(dirX, planeY)));
		fixed_t transX = inv_d * (fmul(dirY, spriteX) - fmul(dirX, spriteY));
		fixed_t transY = inv_d * (fmul(-planeY, spriteX) + fmul(planeX, spriteY));
		int screen_x = (int)(viewport_w * 0.5) * ffloor(0xFFFF + fdiv(transX, transY));
		//if(screen_x <= 0 || screen_x >= viewport_w) break;

		int spriteHeight = f2int(fdiv(fix(viewport_h), fix(sqrt(sprite_list[i][0])))); //Taille en px de la ligne
      	if (spriteHeight < 1) spriteHeight = 1;
      	fixed_t spriteSize = fix(spriteHeight) / 64; //taille proportionelle de la ligne a la tex
		
		image_scale(sprite, spriteSize, spriteSize, &temp);
      	image_linear(sprite, image_at(frame_buffer, screen_x, (int)(viewport_h * 0.5 - spriteHeight * 0.5)), &temp);
	}
}