/*
 * Create the main window.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>

extern GtkWidget *application;
extern GtkWidget *alarmlist;
extern GtkWidget *wid_backlight;
extern GtkWidget *wid_blink;
extern GtkWidget *wid_styledir;
extern GtkWidget *wid_stylename;
extern GtkWidget *wid_color;
extern GtkWidget *wid_command;
extern GtkWidget *wid_msgcmd;
extern GtkWidget *wid_h12;
extern GtkWidget *wid_itm;
extern GtkWidget *wid_locale;

extern int selected_row;

gint list_unsel_cb (GtkCList *clist,
        gint row,
        gint column,
        GdkEventButton *event,
        void *data);
void create_mainwindow();

#endif
