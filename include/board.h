/*
 *    This file is part of popChess.
 *
 *    popChess is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    popChess is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with popChess.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "utils.h"
#include <gtk/gtk.h>
#include <string.h>

#define NUM_SQUARES_X 8
#define NUM_SQUARES_Y 8
#define BOARD_SIZE_X 600
#define BOARD_SIZE_Y 600
#define DEFAULT_BOARD_SIZE 600
#define DEFAULT_SQUARE_SIZE 75

sizes_t getSquareSizes(GtkWidget *);
void getPiecePositions(char *, piece_info_t *);
uint getPieceType(char);
uint getCoordinatesFromMove(char*, uint*, uint*, uint*, uint*);

#endif // include guard
