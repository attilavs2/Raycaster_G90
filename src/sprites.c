#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>

#include "fixed.h"

#include "map_test.h"
#include "sprites.h"
#include "moteur.h"

// 2 maps de sprites : Les sprites de la map (tonneaux, torches, cadavres...)
// Et les sprites de PNJs, donc d'ennemis et amicaux si jamais il y en a
// 
//

//char lsprites[32][32]; //les pnjs (ennemis)

void draw_sprites() {
	// 1 - lister les sprites en render distance
	// 2 - vérifier la distance/si c'est derrière un mur
	// 3 - trier par distance
	// 4 - passage a la moulinette de la perspective de la taille
	// 5 - vérifier les recouvrement éventuels
	// 6 - dessiner en commençant par derrière
}
void test_sprite(fixed_t scale){
	extern bopti_image_t demo_player;
	struct image_linear_map temp_lin_map;

	image_scale(&demo_player , scale, scale, &temp_lin_map);
	image_linear_alloc(&demo_player , &temp_lin_map);
	dimage(100,100,&demo_player);
}