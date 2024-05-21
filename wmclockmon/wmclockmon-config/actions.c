/*
 * actions.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <ctype.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include "defines.h"
#include "variables.h"
#include "mainwindow.h"
#include "tools.h"
#include "edit.h"
#include "actions.h"

void quit_app(void) {
    FREE(command);
    FREE(config_file);
    FREE(light_color);
    FREE(msgcmd);
    FREE(style_dir);
    FREE(style_name);
    free_alrm(&alarms);
    gtk_main_quit();
}


void set_list() {
    Alarm *alrm = alarms;
    gchar *text_line[4];
    int    row = 0, r = 0;

    gtk_clist_clear(GTK_CLIST(alarmlist));
    selected_row = -1;
    gtk_clist_freeze(GTK_CLIST(alarmlist));
    while (alrm) {
        text_line[0] = alrm->on ? "ON" : "OFF";
        text_line[1] = alrm->time ? alrm->time : "ERROR";
        text_line[2] = alrm->date ? alrm->date : "ALL";
        text_line[3] = alrm->msg ? alrm->msg : "";
        row = gtk_clist_append(GTK_CLIST(alarmlist), text_line);
        gtk_clist_set_row_data(GTK_CLIST(alarmlist), row, (gpointer)alrm);
        alrm = alrm->next;
        r++;
    }
    gtk_clist_thaw(GTK_CLIST(alarmlist));
    list_unsel_cb(GTK_CLIST(alarmlist), 0, 0, NULL, NULL);
}

void set_values(void) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_backlight), backlight);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_blink),
            switch_authorized);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_h12), h12);
    if (time_mode == 1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_itm), 1);
    else if (time_mode == 2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_bin), 1);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_clk), 1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_locale), use_locale);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_showcal), showcal);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid_calalrms), calalrms);
    if (light_color) gtk_entry_set_text(GTK_ENTRY(wid_color), light_color);
    if (command)     gtk_entry_set_text(GTK_ENTRY(wid_command), command);
    if (msgcmd)      gtk_entry_set_text(GTK_ENTRY(wid_msgcmd), msgcmd);
    if (style_name)  gtk_entry_set_text(GTK_ENTRY(wid_stylename), style_name);
    if (style_dir)   gtk_entry_set_text(GTK_ENTRY(wid_styledir), style_dir);

    set_list();
}


void save_datas(void) {
    FREE(style_dir);
    FREE(style_name);
    FREE(light_color);
    FREE(command);
    FREE(msgcmd);

    style_dir         = xstrdup(gtk_entry_get_text(GTK_ENTRY(wid_styledir)));
    style_name        = xstrdup(gtk_entry_get_text(GTK_ENTRY(wid_stylename)));
    light_color       = xstrdup(gtk_entry_get_text(GTK_ENTRY(wid_color)));
    command           = xstrdup(gtk_entry_get_text(GTK_ENTRY(wid_command)));
    msgcmd            = xstrdup(gtk_entry_get_text(GTK_ENTRY(wid_msgcmd)));
    backlight         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_backlight));
    switch_authorized = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_blink));
    h12               = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_h12));
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_itm)))
        time_mode = 1;
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_bin)))
        time_mode = 2;
    else
        time_mode = 0;
    use_locale        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_locale));
    showcal           = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_showcal));
    calalrms          = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_calalrms));

    save_cfgfile();
}


void do_add(GtkWidget *widget UNUSED, void *data UNUSED) {
    alrm_add(&alarms, newalarm);
    set_list();
    FREE(newalarm);
}


void add_alarm(void) {
    edit_dialog("Add alarm...", TRUE, "12:00", NULL, NULL, do_add);
}


void do_change(GtkWidget *widget UNUSED, void *data UNUSED) {
    Alarm *alrm;
    char  *time = NULL, *date = NULL, *ison = NULL, *mesg = NULL, *at;
    char  *tokstr = xstrdup(newalarm);
    char  *toksav = tokstr;

    alrm = (Alarm *)gtk_clist_get_row_data(GTK_CLIST(alarmlist), selected_row);

    at = strchr(newalarm, '@');
    if (at) ison = strtok(tokstr, "@");
    time = strtok(at ? NULL : tokstr, "-.");
    if (strchr(newalarm, '-')) date = strtok(NULL, ".");
    mesg = strtok(NULL, "\n\0");

    FREE(alrm->entry);
    FREE(alrm->time);
    FREE(alrm->date);
    FREE(alrm->msg);
    alrm->entry = xstrdup(newalarm);
    alrm->time  = time ? xstrdup(time) : NULL;
    alrm->date  = date ? xstrdup(date) : NULL;
    alrm->on    = ison ? getbool(ison) : TRUE;
    alrm->msg   = mesg ? xstrdup(mesg) : NULL;
    set_list();
    FREE(newalarm);
    FREE(toksav);
}



void edit_entry(void) {
    Alarm *alrm;

    alrm = (Alarm *)gtk_clist_get_row_data(GTK_CLIST(alarmlist), selected_row);
    edit_dialog("Edit alarm...", alrm->on, alrm->time, alrm->date, alrm->msg, do_change);
}


void switch_onoff(void) {
    Alarm *alrm;

    /* get selected default box */
    alrm = (Alarm *)gtk_clist_get_row_data(GTK_CLIST(alarmlist), selected_row);
    alrm->on = !alrm->on;
    /* set text in both rows */
    gtk_clist_set_text(GTK_CLIST(alarmlist), selected_row, 0,
            alrm->on ? "ON" : "OFF");
    
}


void remove_alarm(void) {
    Alarm *alrm, *prev;

    alrm = (Alarm *)gtk_clist_get_row_data(GTK_CLIST(alarmlist), selected_row);
    prev = alarms;
    while (prev && (prev->next != alrm)) {
        prev = prev->next;
    }
    if (alrm == alarms) {
        alarms = alrm->next;
    } else if (prev != NULL) {
        prev->next = alrm->next;
    } else {
        fprintf(stderr, "Error when removing alarm...\n");
    }
    FREE(alrm->entry);
    FREE(alrm->time);
    FREE(alrm->date);
    FREE(alrm->msg);
    FREE(alrm);
    set_list();
}

