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

#endif // include guard