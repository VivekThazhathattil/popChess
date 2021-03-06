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

#include "board.h"

sizes_t getSquareSizes(GtkWidget *board) {
  IGNORE(board);
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
      pieceInfo[pieceId].type = fen[i] - 'A' + 'a';
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

uint getPieceType(char c) {
  if (c == 'p')
    return 0;
  else if (c == 'n')
    return 1;
  else if (c == 'b')
    return 2;
  else if (c == 'r')
    return 3;
  else if (c == 'q')
    return 4;
  else
    return 5;
}

// move: Eg: 'a5b6' 'c1c8'
uint getCoordinatesFromMove(char *move, uint *x1, uint *y1, uint *x2,
                            uint *y2) {
  // printf("%s\n", move);
  if (strlen(move) > 4)
    return 0;
  char *pch;
  char files[] = "abcdefgh";
  char ranks[] = "12345678";
  pch = strchr(files, move[0]);
  if (pch == NULL)
    return 0;
  *x1 = pch - files;
  pch = strchr(ranks, move[1]);
  if (pch == NULL)
    return 0;
  *y1 = pch - ranks;
  pch = strchr(files, move[2]);
  if (pch == NULL)
    return 0;
  *x2 = pch - files;
  pch = strchr(ranks, move[3]);
  if (pch == NULL)
    return 0;
  *y2 = pch - ranks;
  return 1;
}