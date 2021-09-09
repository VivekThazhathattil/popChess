#include "board.h"

sizes_t getSquareSizes(GtkWidget *board) {
  sizes_t squareSize;
  //    squareSize.x = gtk_widget_get_allocated_width(board) / NUM_SQUARES_X;
  //    squareSize.y = gtk_widget_get_allocated_height(board) / NUM_SQUARES_Y;
  squareSize.x = BOARD_SIZE_X / NUM_SQUARES_X;
  squareSize.y = BOARD_SIZE_Y / NUM_SQUARES_Y;
  return squareSize;
}

void getPiecePositions(char *fen, piece_info_t *pieceInfo) {
  uint pieceId = 0;
  uint rank = 7, file = 0; // rank: 0 -> 7, file: 0 -> 7
  for (uint i = 0; fen[i] != '\0' && fen[i] != ' ';
       ++i) { // looping from black's end to white's end
    if (fen[i] == '/') {
      rank--;
      file = 0;
    } else if (fen[i] > '0' && fen[i] < '9') {
      file += fen[i] - '0';
    } else if (fen[i] > 'a' && fen[i] < 'z') {
      pieceInfo[pieceId].type = fen[i];
      pieceInfo[pieceId].x = file;
      pieceInfo[pieceId].y = rank;
      pieceInfo[pieceId].id = pieceId;
      pieceInfo[pieceId].color = 'b';
      ++file;
      ++pieceId;
    } else if (fen[i] > 'A' && fen[i] < 'Z') {
      pieceInfo[pieceId].type = fen[i];
      pieceInfo[pieceId].x = file;
      pieceInfo[pieceId].y = rank;
      pieceInfo[pieceId].id = pieceId;
      pieceInfo[pieceId].color = 'w';
      ++file;
      ++pieceId;
    }
  }
  for (uint i = 0; i < pieceId; ++i)
    pieceInfo[i].totalCount = pieceId;
}

uint getPieceType(char c){
  if(c == 'p')
    return 0;
  else if(c == 'b') 
    return 1;
  else if(c == 'n')
    return 2;
  else if(c == 'q') 
    return 3;
  else if(c == 'k') 
    return 4;
  else return 5;
}