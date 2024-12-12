#include "chadware.h"
#include <ncurses.h>
#include "chadgraphics.h"

char i14toh(int32_t n, int32_t start_point) {
	n -= start_point;
	if (n > 15)
		n %= 16;
	switch(n) {
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		case 10:
			return 'a';
			break;
		case 11:
			return 'b';
			break;
		case 12:
			return 'c';
			break;
		case 13:
			return 'd';
			break;
		case 14:
			return 'e';
			break;
		case 15:
			return 'f';
			break;
	}
}

void p_natural_block(int32_t tile_id, int8_t n_anim, int x, int y, int level) {
	char block_character;
	

	if (tile_id >= 1 && tile_id <= 16) { // grass
		init_pair(1, COLOR_WHITE, COLOR_GREEN);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '~';
	}
	if (tile_id >= 17 && tile_id <= 32) { // dirt
		init_pair(1, COLOR_BLACK, COLOR_WHITE);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '+';
	}
	if (tile_id >= 33 && tile_id <= 48) { // stone
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '@';
	}
	if (tile_id >= 49 && tile_id <= 64) { // sand
		init_pair(1, COLOR_WHITE, COLOR_YELLOW);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '#';
	}
	if (tile_id >= 65 && tile_id <= 80) { // sandstone
		init_pair(1, COLOR_BLACK, 11 /* LIGHT_YELLOW */);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '[';
	}
	if (tile_id >= 81 && tile_id <= 96) { // water 
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '=';
	}
	if (tile_id >= 97 && tile_id <= 112) { // lava 
		init_pair(1, COLOR_WHITE, 9 /*LIGHT_RED*/);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '=';
	}
	if (tile_id >= 113 && tile_id <= 128) { // clay
		init_pair(1, COLOR_WHITE, 8 /*LIGHT_BLACK or GRAY*/);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '0';
	}
	if (tile_id >= 129 && tile_id <= 144) { // gravel
		init_pair(1, COLOR_BLACK, 8 /*LIGHT_BLACK or GRAY*/);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '&';
	} 
	if (tile_id >= 145 && tile_id <= 160) { // ice
		init_pair(1, 8 /*LIGHT_BLACK or GRAY*/, 12 /*LIGHT_BLUE*/);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '$';
	}
	if (tile_id >= 161 && tile_id <= 176) { // snow
		init_pair(1, 8 /*LIGHT_BLACK or GRAY*/, COLOR_WHITE);
		wattr_set(main_win, A_BOLD, 1, NULL);
		block_character = '@';
	}

	if (n_anim > 50) {
	mvwaddch(main_win, y, x, i14toh(tile_id + level, 1));
	}
	else {
		mvwaddch(main_win, y, x, block_character);
	}
	wattr_set(main_win, A_NORMAL, 0, NULL);
}
