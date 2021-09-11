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

#include "display.h"
#include <stdio.h>

// TODO: remove global vars; use a struct ptr;
GtkWidget *wNameWidget, *wRatingWidget, *wTimeWidget;
GtkWidget *bNameWidget, *bRatingWidget, *bTimeWidget;
GtkWidget *boardRef;
RsvgHandle *piece_images[2][6];
uint fenActive;
piece_info_t *pInf;

// bad practice: remove duplicates
void updateAllLabelTexts(lichess_data_t *liDat) {
  updateLabelTexts(wNameWidget, liDat->white.name);
  updateLabelTexts(bNameWidget, liDat->black.name);
  updateLabelTexts(wRatingWidget, liDat->white.rating);
  updateLabelTexts(bRatingWidget, liDat->black.rating);
}
static gboolean draw_board(GtkWidget *drawing_area, cairo_t *cr,
                           GtkWidget *data);

GtkWidget *displayControl() {
  GtkWidget *canvas, *arrows, *buttonsArray, *coords, *board, *pieces,
      *whitePlayerDetails, *blackPlayerDetails;
  cairo_t *cr;
  fenActive = 0;

  char *piece_svgs = "pieces/merida/";
  GError *err = NULL;
  load_svgs(piece_svgs, &err);
  canvas = gtk_vbox_new(0, 0);
  board = gtk_drawing_area_new();
  boardRef = board;
  gtk_widget_set_size_request(board, BOARD_SIZE_X, BOARD_SIZE_Y);
  makeBoard(board);

  wNameWidget = gtk_label_new("-");
  bNameWidget = gtk_label_new("-");
  wRatingWidget = gtk_label_new("-");
  bRatingWidget = gtk_label_new("-");
  wTimeWidget = gtk_label_new("-");
  bTimeWidget = gtk_label_new("-");

  whitePlayerDetails = gtk_hbox_new(0, 0);
  blackPlayerDetails = gtk_hbox_new(0, 0);

  makePlayerDetails(whitePlayerDetails, wNameWidget, wRatingWidget, wTimeWidget,
                    NULL, NULL, NULL);
  makePlayerDetails(blackPlayerDetails, bNameWidget, bRatingWidget, bTimeWidget,
                    NULL, NULL, NULL);
  gtk_box_pack_end(GTK_BOX(canvas), whitePlayerDetails, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), board, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), blackPlayerDetails, 1, 1, 0);

  char piecesDir[] = "pieces/merida/w_q.svg";
  //makePieces(pieces, piecesDir);
  //gtk_box_pack_end(GTK_BOX(canvas), pieces, 1, 1, 0);
  makeCoordinates(coords);
  makeArrows(arrows);
  makeControlButtonsArray(buttonsArray);

  return canvas;
}

void makeBoard(GtkWidget *board) {
  g_signal_connect(board, "draw", G_CALLBACK(draw_board), board);
}

void makePieces(GtkWidget *pieces, char *piecesDir) {
  GError *error = NULL;
  GdkPixbuf *pix;
  if ((pix = gdk_pixbuf_new_from_file_at_scale(piecesDir, 200, 200, TRUE,
                                               &error)) == NULL) {
    g_printerr("Error loading file: #%d %s\n", error->code, error->message);
    g_error_free(error);
    exit(1);
  }
  pieces = gtk_image_new_from_pixbuf(pix);
}

void updateLabelTexts(GtkWidget *label, char *text) {
  gtk_label_set_text(GTK_LABEL(label), (text == NULL) ? "-" : text);
}
void makePlayerDetails(GtkWidget *playerDetails, GtkWidget *name,
                       GtkWidget *rating, GtkWidget *time, char *nameStr,
                       char *titleStr, char *ratingStr) {
  //  name = (nameStr == NULL) ? gtk_label_new("Name") : nameStr;
  //  rating = (ratingStr == NULL) ? gtk_label_new("Rating") : ratingStr;
  //  time = gtk_label_new("Time"); // TODO: fix this as well, along with
  //  ratingStr
  updateLabelTexts(name, nameStr);
  updateLabelTexts(rating, ratingStr);
  updateLabelTexts(time, NULL);
  gtk_box_pack_start(GTK_BOX(playerDetails), name, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(playerDetails), rating, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(playerDetails), time, 1, 1, 0);
}

void makeCoordinates(GtkWidget *coords) {}

void makeArrows(GtkWidget *arrows) {}

void makeControlButtonsArray(GtkWidget *buttonsArray) {}

static gboolean draw_board(GtkWidget *drawing_area, cairo_t *cr,
                           GtkWidget *data) {
  sizes_t ss = getSquareSizes(data);
  for (uint i = 0; i < NUM_SQUARES_Y; ++i) {
    for (uint j = 0; j < NUM_SQUARES_X; ++j) {
      if ((i + j) % 2 == 0) {
        cairo_set_source_rgb(cr, 240.0 / 255, 217.0 / 255, 181.0 / 255);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
      } else {
        cairo_set_source_rgb(cr, 181.0 / 255, 136.0 / 255, 99.0 / 255);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
      }
    }
  }
  // draw pieces
  if (fenActive > 0) {
    double scale = 0.005 * DEFAULT_BOARD_SIZE / DEFAULT_SQUARE_SIZE;
    RsvgHandle *piece_image;
    for (uint i = 0; i < pInf[0].totalCount; ++i) {
      cairo_scale(cr, scale, scale);
      cairo_move_to(cr, pInf[i].x * DEFAULT_SQUARE_SIZE, (7 - pInf[i].y) * DEFAULT_SQUARE_SIZE );
      printf("PINF: %s, %s \n", pInf[i].color, pInf[i].type);
      piece_image = piece_images[(pInf[i].color == 'b' ? 0 : 1)][getPieceType(pInf[i].type)];
      rsvg_handle_render_cairo(piece_image, cr);
      cairo_move_to(cr, 0, 0);
      cairo_scale(cr, 1 / scale, 1 / scale);
    }
  }
  //fenActive = 0;
  return FALSE;
}

void load_svgs(char *dir, GError **err) {
  uint len = strlen(dir) + 8; // e.g.: "w_k.svg\0"
  char str[len];
  char piece_letters[] = "pnbrqk";
  char side_letters[] = "bw";

  for (uint i = 0; i < 2; i++) {
    char side = side_letters[i];

    for (uint j = 0; piece_letters[j] != '\0'; j++) {
      sprintf(str, "%s%c_%c.svg", dir, side, piece_letters[j]);

      piece_images[i][j] = rsvg_handle_new_from_file(str, err);
      if (*err != NULL)
        return;
    }
  }
}

void showPieces(piece_info_t *pieceInfo) {
  fenActive = 1;
  pInf = pieceInfo;
  gtk_widget_queue_draw(boardRef);
}
