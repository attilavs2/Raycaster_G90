#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>
#include <libprof.h>

#define USB

#ifdef USB
#include <gint/usb-ff-bulk.h>
#include <gint/usb.h>
#endif

#include "fixed.h"

#include "moteur.h"
#include "map.h"
#include "sprites.h"

//====== ShooterGeam V 0.1 =====
// Git du moteur : https://github.com/attilavs2/Raycaster_prealpha
// 
// Tout le code est en GPL3
// Game design : Fcalva (aka. fklv, Fcalva#6860, attilavs2)
// Programmation : Fcalva
// "Art" : Fcalva
// 
// TODO :
//	- Sprites ! |~| 
//		-Refaire mieux |X|
//		-Optimiser | |
//	- Map tiled | |
// 
//

#ifndef FXCG50
#error Ce code est pour FXCG50/G90+E uniquement, enlevez ce message a vos riques et périls
#endif

#ifdef USB
void USB_capture() {
	if (usb_is_open()) usb_fxlink_screenshot(false);
}
#endif

#define debug //pour afficher les infos de debug

//extern ShooterMap ShooterLevel0;

extern image_t briques0;
extern image_t buisson0;

extern image_t zombard;

char exit_game = 0;
char disp_frame_time = 0;
char first_frame = 1;
int frame_time_timer = 1;
int capture_timer = 1;

fixed_t posX;
fixed_t posY;
fixed_t dirX;
fixed_t dirY;
fixed_t planeX;
fixed_t planeY;

int frame_time = 0;
int a;

int raycast_time = 0;
int draw_time = 0;

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
	if (keydown(KEY_F6)) exit_game = 1;

	#ifdef debug
	if (keydown(KEY_TAN)) end_screen();
	#endif

	#ifdef USB
	if (keydown(KEY_0) && keydown(KEY_EXE) && capture_timer <= 0) {
		USB_capture();
		capture_timer = 10;
	}
	capture_timer--;
	#endif
}

void main_menu(){
	dtext_opt(198, 100, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "ShooterGame", -1);
	dtext_opt(198, 120, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "De Fcalva", -1);
	dtext_opt(198, 150, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "Appuyez sur une touche", -1);

	dtext( 1, 1, C_BLACK, "Version ALPHA");

	dupdate();
	getkey();
}

void end_screen(){
	dtext_opt(198, 70, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "Vous avez battu Maze3D !", -1);
	dtext_opt(198, 90, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "Optn - Rejouer", -1);
	dtext_opt(198, 100, 0xde85, C_NONE, DTEXT_CENTER, DTEXT_TOP, "Menu - Quitter", -1);

	dupdate();

	while(true){
		a = getkey().key;
		if (a == KEY_OPTN) {
			load_map(); 
			break;
		}
	}
}

int main(){
	dclear(C_WHITE);

	//trucs de chargement

	load_map();

	image_t frame_buffer = *image_alloc(viewport_w, viewport_h, IMAGE_RGB565A);
	
	image_t sky_tex = *image_alloc(64, 64, IMAGE_RGB565);
	image_t WXOR_tex = *image_alloc(64, 64, IMAGE_RGB565);
	image_t D_tex = *image_alloc(64, 64, IMAGE_RGB565);

	int i, j;
	for (i = 0; i<= 64; i++){
		for (j = 0; j<= 64; j++){
			int c = (int)floor((float)(i ^ j) * 0.5);
			unsigned short color = (c << 8) + (c << 4) + c;
    		image_set_pixel(&WXOR_tex, i, j, color);
		}
	}
	image_fill(&D_tex, 0x4228);
	image_fill(&sky_tex, 0x9dbd);
	
	prof_init();

	#ifdef USB
    usb_interface_t const *interfaces[] = {&usb_ff_bulk, NULL};
    usb_open(interfaces, GINT_CALL_NULL);
    #endif

	posX = fix(13); posY = fix(111);
	dirX = 0xFFFF; dirY = 0x0;
	planeX = 0x0; 
	planeY = fix(-0.66);
	
	
	while (exit_game == 0) {
		prof_t frame = prof_make();
		prof_enter(frame);
		
		drect(0, 0, 395, 112, 0x9dbd);
		drect(0,112, 395, 243, 0xc4c9);

		image_clear(&frame_buffer);

		//draw_sprites(&frame_buffer, &zombard, &ShooterLevel0);

		//draw_walls(&buisson0, &briques0, &sky_tex, &WXOR_tex, &D_tex, &ShooterLevel0, &frame_buffer);

		dimage(0, 0, &frame_buffer);

		if(first_frame == 1) main_menu();
		
		keys_get();

		//logic(&frame_buffer, &D_tex);

		if (disp_frame_time == 1) dprint( 1, 1, C_BLACK, "Frame time : %d ms", frame_time);
		
		#ifdef debug
		dprint( 1, 10, C_BLACK, "planeX : %d", planeX);
		dprint( 1, 20, C_BLACK, "planeY : %d", planeY);
		dprint( 1, 30, C_BLACK, "dirX : %d", dirX);
		dprint( 1, 40, C_BLACK, "dirY : %d", dirY);
		dprint( 1, 50, C_BLACK, "posX : %d", posX);
		dprint( 1, 60, C_BLACK, "posY : %d", posY);
		#endif

		dprint(1,10,C_BLACK, "Raycast time : %d ms", raycast_time);
		dprint(1,20,C_BLACK, "Draw time : %d ms", draw_time);

		dupdate();
		prof_leave(frame);
		frame_time = (int)prof_time(frame)/1000;
		first_frame = 0;
	}

	prof_quit();
	usb_close();

	free(&sky_tex);
	free(&WXOR_tex);
	free(&D_tex);
	free(&frame_buffer);

	return 1;
}