/*
 * edit.c
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "defines.h"
#include "tools.h"
#include "edit.h"

char *newalarm;

static GtkWidget *wid_status;
static GtkWidget *wid_time;
static GtkWidget *wid_date;
static GtkWidget *wid_mesg;

void set_data(GtkWidget *widget UNUSED, GtkWidget *data UNUSED) {
    const char *wstat = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(wid_status)->entry));
    const char *wtime = gtk_entry_get_text(GTK_ENTRY(wid_time));
    const char *wdate = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(wid_date)->entry));
    const char *wmesg = gtk_entry_get_text(GTK_ENTRY(wid_mesg));
    int         has_day = (strcmp(wdate, "ALL") != 0);
    int         has_msg = (strcmp(wdate, "") != 0);
    int         len = strlen(wstat) + 1 + strlen(wtime) + 1;
   
    len += has_day ? strlen(wdate) + 1 : 0;
    len += has_msg ? strlen(wmesg) + 1 : 0;
    newalarm = xmalloc(len);

    sprintf(newalarm, "%s@%s%s%s%s%s",
            wstat,
            wtime,
            has_day ? "-" : "",
            has_day ? wdate : "",
            has_msg ? "." : "",
            has_msg ? wmesg : "");
}


void edit_dialog(const char *title,
                 int on,
                 const char *atime,
                 const char *adate,
                 const char *amesg,
                 void (*f_ok)(void)) {

    GtkWidget *dialog;
    GtkWidget *bouton;
    GtkWidget *label;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GList     *items = NULL;

    FREE(newalarm);
    dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    g_signal_connect(dialog, "delete_event",
                     G_CALLBACK(gtk_widget_destroy), NULL);
    g_signal_connect(dialog, "destroy",
                     G_CALLBACK(gtk_widget_destroy), NULL);

    bouton = gtk_button_new_with_label("Ok");
    g_signal_connect(bouton, "clicked", G_CALLBACK(set_data), NULL);
    g_signal_connect(bouton, "clicked", G_CALLBACK(f_ok),     NULL);
    g_signal_connect_swapped(bouton, "clicked",
                             G_CALLBACK(gtk_widget_destroy), dialog);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dialog))),
                       bouton, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_grab_default(GTK_WIDGET(bouton));
    gtk_widget_show(bouton);

    bouton = gtk_button_new_with_label("Cancel");
    g_signal_connect_swapped(bouton, "clicked",
                             G_CALLBACK(gtk_widget_destroy), dialog);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dialog))),
                       bouton, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_show(bouton);

    gtk_container_set_border_width(GTK_CONTAINER
                                   (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), 6);

    hbox = gtk_hbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox);
    gtk_widget_show(hbox);

    /* STATUS */
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Status");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    items = g_list_append(items, "On");
    items = g_list_append(items, "Off");
    wid_status = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(wid_status), items);
    gtk_combo_set_value_in_list(GTK_COMBO(wid_status), TRUE, FALSE);
    gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(wid_status)->entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), wid_status, FALSE, TRUE, 6);
    gtk_widget_show(wid_status);
    if (on)
        gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(wid_status)->entry), "On");
    else
        gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(wid_status)->entry), "Off");


    /* HOUR */
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Hour");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_time = gtk_entry_new();
    if (atime) gtk_entry_set_text(GTK_ENTRY(wid_time), atime);
    gtk_box_pack_start(GTK_BOX(vbox), wid_time, FALSE, TRUE, 6);
    gtk_widget_show(wid_time);


    /* DAY */
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Day");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    items = NULL;
    items = g_list_append(items, "ALL");
    items = g_list_append(items, "1");
    items = g_list_append(items, "2");
    items = g_list_append(items, "3");
    items = g_list_append(items, "4");
    items = g_list_append(items, "5");
    items = g_list_append(items, "6");
    items = g_list_append(items, "7");
    wid_date = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(wid_date), items);
    gtk_combo_set_value_in_list(GTK_COMBO(wid_date), TRUE, TRUE);
    gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(wid_date)->entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), wid_date, FALSE, TRUE, 6);
    gtk_widget_show(wid_date);
    if (adate)
        gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(wid_date)->entry), adate);

    
    /* MESSAGE */
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Message");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_mesg = gtk_entry_new();
    if (amesg) gtk_entry_set_text(GTK_ENTRY(wid_mesg), amesg);
    gtk_box_pack_start(GTK_BOX(vbox), wid_mesg, FALSE, TRUE, 6);
    gtk_widget_show(wid_mesg);



    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_widget_show(dialog);
}

