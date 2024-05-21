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

    const char *wstat = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(wid_status));
    const char *wtime = gtk_entry_get_text(GTK_ENTRY(wid_time));
    const char *wdate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(wid_date));
    const char *wmesg = gtk_entry_get_text(GTK_ENTRY(wid_mesg));

    int has_day = (strcmp(wdate, "ALL") != 0);
    int has_msg = (strcmp(wdate, "") != 0);
    int len = strlen(wstat) + 1 + strlen(wtime) + 1;
   
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


static gboolean set_active_date(GtkTreeModel *model, GtkTreePath *path,
				GtkTreeIter *iter, gpointer user_data)
{
    const char *date1 = user_data;
    const char *date2;

    (void) path;

    gtk_tree_model_get(model, iter, 0, (gpointer) &date2, -1);
    if (strcmp(date1, date2) == 0) {
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(wid_date), iter);
	return TRUE;
    }

    return FALSE;
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
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog), bouton, GTK_RESPONSE_OK);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_grab_default(GTK_WIDGET(bouton));
    gtk_widget_show(bouton);

    bouton = gtk_button_new_with_label("Cancel");
    g_signal_connect_swapped(bouton, "clicked",
                             G_CALLBACK(gtk_widget_destroy), dialog);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog), bouton, GTK_RESPONSE_CANCEL);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_show(bouton);

    gtk_container_set_border_width(GTK_CONTAINER
                                   (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), 6);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox);
    gtk_widget_show(hbox);

    /* STATUS */
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Status");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_status = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_status), "On");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_status), "Off");
    gtk_box_pack_start(GTK_BOX(vbox), wid_status, FALSE, TRUE, 6);
    gtk_widget_show(wid_status);
    gtk_combo_box_set_active(GTK_COMBO_BOX(wid_status), on ? 0 : 1);

    /* HOUR */
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Hour");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_time = gtk_entry_new();
    if (atime) gtk_entry_set_text(GTK_ENTRY(wid_time), atime);
    gtk_box_pack_start(GTK_BOX(vbox), wid_time, FALSE, TRUE, 6);
    gtk_widget_show(wid_time);


    /* DAY */
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Day");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_date = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "ALL");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "4");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "5");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "6");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wid_date), "7");
    gtk_box_pack_start(GTK_BOX(vbox), wid_date, FALSE, TRUE, 6);
    gtk_widget_show(wid_date);
    gtk_combo_box_set_active(GTK_COMBO_BOX(wid_date), 0);
    if (adate)
	gtk_tree_model_foreach(gtk_combo_box_get_model(GTK_COMBO_BOX(wid_date)),
			       set_active_date, (void *) adate);
    
    /* MESSAGE */
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), vbox);
    gtk_widget_show(vbox);

    label = gtk_label_new("Message");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5f);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
    gtk_widget_show(label);

    wid_mesg = gtk_entry_new();
    if (amesg) gtk_entry_set_text(GTK_ENTRY(wid_mesg), amesg);
    gtk_box_pack_start(GTK_BOX(vbox), wid_mesg, FALSE, TRUE, 6);
    gtk_widget_show(wid_mesg);



    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_widget_show(dialog);
}

