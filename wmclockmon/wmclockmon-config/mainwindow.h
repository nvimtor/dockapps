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
extern GtkWidget *wid_clk;
extern GtkWidget *wid_itm;
extern GtkWidget *wid_bin;
extern GtkWidget *wid_locale;
extern GtkWidget *wid_showcal;
extern GtkWidget *wid_calalrms;

typedef enum {
     COL_STATUS = 0,
     COL_HOUR,
     COL_DAY,
     COL_MESSAGE,
     COL_ALARM,
     NUM_COLS
} AlarmListColumn;

void clear_alarmlist_selection(void);

void create_mainwindow(void);

#endif
