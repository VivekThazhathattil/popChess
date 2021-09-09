#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "board.h"
#include "utils.h"
#include <gtk/gtk.h>

GtkWidget *displayControl();
void makeBoard(GtkWidget *);
void makePieces(GtkWidget *, char *);
void updateCanvas(GtkWidget *, char *, char *, char *, char *, char *, char *);
void makePlayerDetails(GtkWidget *, GtkWidget *, GtkWidget *, GtkWidget *,
                       char *, char *, char *);
void makeCoordinates(GtkWidget *);
void makeArrows(GtkWidget *);
void makeControlButtonsArray(GtkWidget *);
void updateAllLabelTexts(lichess_data_t*);
void updateLabelTexts(GtkWidget* , char* );

#endif // include guard
