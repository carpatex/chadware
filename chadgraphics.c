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

void init_char_colors() {
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, 11);
	init_pair(6, COLOR_WHITE, COLOR_BLUE);
	init_pair(7, COLOR_WHITE, 9); 
	init_pair(8, COLOR_WHITE, 8);
	init_pair(9, COLOR_BLACK, 8); 
	init_pair(10, 8, 12);
	init_pair(11, 8, COLOR_WHITE);
}
void p_natural_block(int32_t tile_id, int8_t n_anim, int x, int y) {
	char block_character;

	if (tile_id >= 1 && tile_id <= 16) { // grass
		wattr_set(main_win, A_NORMAL, 1, NULL);
		block_character = '~';
	}
	if (tile_id >= 17 && tile_id <= 32) { // dirt
		wattr_set(main_win, A_NORMAL, 2, NULL);
		block_character = '+';
	}
	if (tile_id >= 33 && tile_id <= 48) { // stone
		wattr_set(main_win, A_NORMAL, 3, NULL);
		block_character = '@';
	}
	if (tile_id >= 49 && tile_id <= 64) { // sand
		wattr_set(main_win, A_NORMAL, 4, NULL);
		block_character = '#';
	}
	if (tile_id >= 65 && tile_id <= 80) { // sandstone
		wattr_set(main_win, A_NORMAL, 5, NULL);
		block_character = '[';
	}
	if (tile_id >= 81 && tile_id <= 96) { // water 
		wattr_set(main_win, A_NORMAL, 6, NULL);
		block_character = '=';
	}
	if (tile_id >= 97 && tile_id <= 112) { // lava 
		wattr_set(main_win, A_NORMAL, 7, NULL);
		block_character = '=';
	}
	if (tile_id >= 113 && tile_id <= 128) { // clay
		wattr_set(main_win, A_NORMAL, 8, NULL);
		block_character = '0';
	}
	if (tile_id >= 129 && tile_id <= 144) { // gravel
		wattr_set(main_win, A_NORMAL, 9, NULL);
		block_character = '&';
	} 
	if (tile_id >= 145 && tile_id <= 160) { // ice
		wattr_set(main_win, A_NORMAL, 10, NULL);
		block_character = '$';
	}
	if (tile_id >= 161 && tile_id <= 176) { // snow
		wattr_set(main_win, A_NORMAL, 11, NULL);
		block_character = '@';
	}
	if (n_anim > 50) {
		mvwaddch(main_win, y, x, i14toh((tile_id - 1) % 16, 1));
	}
	else {
		mvwaddch(main_win, y, x, block_character);
	}
	wattr_set(main_win, A_NORMAL, 0, NULL);
}
