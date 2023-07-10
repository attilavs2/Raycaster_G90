#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <libprof.h>

#define USB

#ifdef USB
#include <gint/usb-ff-bulk.h>
#include <gint/usb.h>
#endif

#include "fixed.h"

#include "moteur.h"
#include "map_test.h"
#include "sprites.h"

//====== Raycaster (faut trouver un nom) =====
// Toute la base du raycaster vient d'ici : https://lodev.org/cgtutor/raycasting.html
// Grands remerciments !
// Github : https://github.com/attilavs2/Raycaster_prealpha
// Tout le code de cette version est en GPL3
// Game design : Fcalva (aka. fklv, Fcalva#6860, attilavs2)
// Programmation : Fcalva
// Art : Fcalva
// 
// 
// TODO :
// - réparer l'affichage
// - sprites
// - gameplay
#ifndef FXCG50
#error Ce code est pour FXCG50/G90+E uniquement, enlevez ce message a vos riques et périls
#endif

#ifdef USB
void USB_capture() {
	if (usb_is_open()) {
		usb_fxlink_screenshot(false);
	}
}
#endif



//#define debug //pour afficher les infos de debug

extern char map_test[map_w][map_h];

char exit_game = 0;
char disp_frame_time = 0;
int frame_time_timer = 1;
int capture_timer = 1;

fixed_t posX;
fixed_t posY;
fixed_t dirX;
fixed_t dirY;
fixed_t planeX;
fixed_t planeY;

int frame_time = 0;

//prof_init();

int main(){
	dclear(C_WHITE);
	dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();

	posX = startpos_x; 
	posY = startpos_y;  //x and y start position
  	dirX = start_dirX;
	dirY = start_dirY; //initial direction vector
 	planeX = fix(0); 
	planeY = fix(0.66); //the 2d raycaster version of camera plan
	
	prof_init();

	#ifdef USB
    usb_interface_t const *interfaces[] = {&usb_ff_bulk, NULL};
    usb_open(interfaces, GINT_CALL_NULL);
    #endif

	//autres trucs de chargement



	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.2.2");
	dtext( 97, 120, C_BLACK, "Edition FPS BRRRRRRRRRRRRRRRRRR");
	dtext(105, 150, C_BLACK, "Appuyez sur une touche");


	dupdate();
	getkey();
	
	test_sprite(0xFFFF);
	dupdate();
	getkey();
	return 1;
	
	/*
	while (exit_game == 0) {
		prof_t frame = prof_make();
		prof_enter(frame);

		dclear(C_LIGHT);

		draw_background(0);

		draw_walls();
		
		keys_get();

		if (disp_frame_time == 1) {
			dprint( 1, 10, C_BLACK, "Frame time : %d ms", frame_time);
		}
		
		#ifdef debug
		dprint( 1, 20, C_BLACK, "planeX : %d", planeX);
		dprint( 1, 30, C_BLACK, "planeY : %d", planeY);
		dprint( 1, 40, C_BLACK, "dirX : %d", dirX);
		dprint( 1, 50, C_BLACK, "dirY : %d", dirY);
		#endif

		dupdate();
		prof_leave(frame);
		frame_time = (int)prof_time(frame)/1000;
	}*/

	prof_quit();
	usb_close();
	return 1;
}

void keys_get(){
	move();
	pollevent();
	if (keydown(KEY_F1) && frame_time_timer <= 0) {
		if (disp_frame_time == 0) {
			disp_frame_time = 1;
			frame_time_timer = 10;
		}
		else {
			disp_frame_time = 0;
			frame_time_timer = 10;
		}
	}
	frame_time_timer--;
	if (keydown(KEY_F6)) {
		exit_game = 1;
	}

	#ifdef USB
	if (keydown(KEY_0) && keydown(KEY_EXE) && capture_timer <= 0) {
		USB_capture();
		capture_timer = 10;
	}
	capture_timer--;
	#endif
}