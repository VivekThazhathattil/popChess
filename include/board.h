#ifndef BOARD_H_
#define BOARD_H_

#include "utils.h"
#include <gtk/gtk.h>

#define NUM_SQUARES_X 8
#define NUM_SQUARES_Y 8
#define BOARD_SIZE_X 600
#define BOARD_SIZE_Y 600

sizes_t getSquareSizes(GtkWidget *board);

#endif // include guard