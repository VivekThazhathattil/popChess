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

#include "control.h"
#include "utils.h"
#include <gtk/gtk.h>

typedef struct activateData{
	int argc;
	char **argv;
} activate_data_t;

static void activate(GtkApplication* app, gpointer userData){
	activate_data_t *dat = (activate_data_t *) userData;
	int argc = dat->argc;
	char **argv = dat->argv;
  if (run(app, argc, argv) == 0)
    printf("Error encountered while running the program. Exiting...");

	free(dat);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	app = gtk_application_new("com.github.popChess", G_APPLICATION_FLAGS_NONE);
	activate_data_t *ad = (activate_data_t *) malloc(sizeof(activate_data_t));
	ad->argc = argc;
	ad->argv = argv;
	g_signal_connect(app, "activate", G_CALLBACK(activate), ad);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

  return status;
}
