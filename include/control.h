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

#ifndef CONTROL_H_
#define CONTROL_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include <board.h>
#include <display.h>
#include <get_feed.h>
#include <json.h>
#include <parse_feed.h>
#include <utils.h>

int run(GtkApplication *app, int argc, char *args[]);
int setWindowProps(GtkWidget *win);
void showWindow(GtkWidget *win);
void triggerFENReceived(char *fenFeed);
void freeLichessData();

#endif // include guard
