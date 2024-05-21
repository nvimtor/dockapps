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


static void set_list(void) {

    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(alarmlist));
    GtkListStore *store = GTK_LIST_STORE(model);
    GtkTreeIter   iter;
    Alarm        *alrm = alarms;

    gtk_list_store_clear(store);

    while (alrm) {

        gtk_list_store_append(store, &iter);
        gtk_list_store_set   (store, &iter,
                              COL_STATUS,  alrm->on   ? "ON"       : "OFF",
                              COL_HOUR,    alrm->time ? alrm->time : "ERROR",
                              COL_DAY,     alrm->date ? alrm->date : "ALL",
                              COL_MESSAGE, alrm->msg  ? alrm->msg  : "",
                              COL_ALARM,   alrm,
                              -1);

        alrm = alrm->next;

    }

    clear_alarmlist_selection();

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


void do_add(void) {
    alrm_add(&alarms, newalarm);
    set_list();
    FREE(newalarm);
}


void add_alarm(void) {
    edit_dialog("Add alarm...", TRUE, "12:00", NULL, NULL, do_add);
}


static Alarm *
get_alarm(GtkTreeModel **modelp, GtkTreeIter *iterp) {

    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    Alarm *alrm = NULL;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(alarmlist));
    if (gtk_tree_selection_get_selected(selection, &model, &iter))
        gtk_tree_model_get(model, &iter, COL_ALARM, (gpointer) &alrm, -1);

    if (modelp)
        *modelp = model;

    if (iterp)
        *iterp = iter;

    return alrm;

}


void do_change(void) {
    Alarm *alrm;
    char  *time = NULL, *date = NULL, *ison = NULL, *mesg = NULL, *at;
    char  *tokstr = xstrdup(newalarm);
    char  *toksav = tokstr;

    alrm = get_alarm(NULL, NULL);
    if (!alrm)
        return;

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

    alrm = get_alarm(NULL, NULL);
    if (!alrm)
        return;

    edit_dialog("Edit alarm...", alrm->on, alrm->time, alrm->date, alrm->msg, do_change);
}


void switch_onoff(void) {
    Alarm *alrm;
    GtkTreeModel *model;
    GtkTreeIter iter;

    /* get selected default box */
    alrm = get_alarm(&model, &iter);
    if (!alrm)
        return;
    alrm->on = !alrm->on;
    /* set text in both rows */
    gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                       COL_STATUS, alrm->on ? "ON" : "OFF",
                       -1);
}


void remove_alarm(void) {
    Alarm *alrm, *prev;

    alrm = get_alarm(NULL, NULL);
    if (!alrm)
        return;

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

