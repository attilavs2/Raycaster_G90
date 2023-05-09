#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"

dclear(C_WHITE);
extern int player_x;
extern int player_y;
//extern int player_z;
extern int player_dir;
compute_table();
load_map();
int main(void)
{
	draw_background();
	draw_walls();
	if (keydown ( 0x86 )){
		//flemme d'implémanter le mouvement, verrai quand ça compile
	}
	if (keydown( 0x85 )) {
		player_dir -= 5;
	}
	if (keydown( 0x76 )) {
		player_dir += 5;
	}
	if (keydown( 0x74 )) {
		return 1;
	}
}