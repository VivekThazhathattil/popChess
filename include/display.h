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

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <board.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <utils.h>
#ifndef RSVG_CAIRO_H_
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>
#endif

typedef struct boardInfo {
  GtkWidget *widget;
  int fenActive;
  piece_info_t *piece_info;
} board_info_t;

typedef struct displayOutput {
  GtkWidget *canvas;
  board_info_t *board_info;
} display_output_t;

GtkWidget *displayControl();
void makeBoard(GtkWidget *);
void makePieces(GtkWidget *, char *);
void updateCanvas(GtkWidget *, char *, char *, char *, char *, char *, char *);
void makePlayerDetails(GtkWidget *, GtkWidget *, GtkWidget *, GtkWidget *,
                       char *, char *, char *);
void makeCoordinates(GtkWidget *);
void makeArrows(GtkWidget *);
void makeControlButtonsArray(GtkWidget *);
void updateAllLabelTexts(lichess_data_t *);
void updateLabelTexts(GtkWidget *, char *);
void load_svgs(char *, GError **);
void showPieces(piece_info_t *);
void freeBoardInfo(board_info_t *);
void freeDisplayOutput(display_output_t *);

#endif // include guard
