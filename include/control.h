#ifndef CONTROL_H_
#define CONTROL_H_

#include <gtk/gtk.h>
#include <stdio.h>

#include <board.h>
#include <display.h>
#include <get_feed.h>
#include <parse_feed.h>
#include <utils.h>

int run(int argc, char *args[]);
int setWindowProps(GtkWidget *win);
void showWindow(GtkWidget *win);

#endif // include guard
