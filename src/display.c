#include "display.h"
#include <stdio.h>

static gboolean draw_board(GtkWidget *drawing_area, cairo_t *cr,
                           GtkWidget *data);

GtkWidget *displayControl() {
  GtkWidget *canvas, *board, *pieces, *whitePlayerDetails, *blackPlayerDetails, *coords, *arrows,
      *buttonsArray;
  cairo_t *cr;

  canvas = gtk_vbox_new(0, 0);

  board = gtk_drawing_area_new();
  gtk_widget_set_size_request(board, BOARD_SIZE_X, BOARD_SIZE_Y);
  makeBoard(board);

    whitePlayerDetails = gtk_hbox_new(0,0);
    blackPlayerDetails = gtk_hbox_new(0,0);
  makePlayerDetails(whitePlayerDetails);
  makePlayerDetails(blackPlayerDetails);
  gtk_box_pack_end(GTK_BOX(canvas), whitePlayerDetails, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), board, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), blackPlayerDetails, 1, 1, 0);

  makePieces(pieces);
  makeCoordinates(coords);
  makeArrows(arrows);
  makeControlButtonsArray(buttonsArray);

  return canvas;
}

void makeBoard(GtkWidget *board) {
  g_signal_connect(board, "draw", G_CALLBACK(draw_board), board);
}

void makePieces(GtkWidget *pieces) {}

void makePlayerDetails(GtkWidget *playerDetails) {
  GtkWidget *name, *rating, *time;
    name = gtk_label_new("Name");
    rating = gtk_label_new("Rating");
    time = gtk_label_new("Time");

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
  return FALSE;
}