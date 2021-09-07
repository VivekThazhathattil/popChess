#include "board.h"

sizes_t getSquareSizes(GtkWidget *board) {
  sizes_t squareSize;
  //    squareSize.x = gtk_widget_get_allocated_width(board) / NUM_SQUARES_X;
  //    squareSize.y = gtk_widget_get_allocated_height(board) / NUM_SQUARES_Y;
  squareSize.x = BOARD_SIZE_X / NUM_SQUARES_X;
  squareSize.y = BOARD_SIZE_Y / NUM_SQUARES_Y;
  return squareSize;
}