#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "board.h"
#include "utils.h"
#include <gtk/gtk.h>

GtkWidget *displayControl();
void makeBoard(GtkWidget *board);
void makePieces(GtkWidget *pieces);
void makePlayerDetails(
    GtkWidget *playerDetails); // includes player names, ratings, clock
void makeCoordinates(GtkWidget *coords);
void makeArrows(GtkWidget *arrows);
void makeControlButtonsArray(GtkWidget *buttonsArray);

#endif // include guard
