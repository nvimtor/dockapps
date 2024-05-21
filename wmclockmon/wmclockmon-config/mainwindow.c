/*
 * Create the main window.
 */

#include <gtk/gtk.h>
#include "../config.h"
#include "defines.h"
#include "variables.h"
#include "mainwindow.h"
#include "actions.h"


GtkWidget *application;
GtkWidget *alarmlist;
GtkWidget *wid_backlight;
GtkWidget *wid_blink;
GtkWidget *wid_styledir;
GtkWidget *wid_stylename;
GtkWidget *wid_color;
GtkWidget *wid_command;
GtkWidget *wid_msgcmd;
GtkWidget *wid_h12;
GtkWidget *wid_clk;
GtkWidget *wid_itm;
GtkWidget *wid_bin;
GtkWidget *wid_locale;
GtkWidget *wid_showcal;
GtkWidget *wid_calalrms;

static const gchar *const clist_titles[] = {" Status ", "  Hour  ", " Day ", " Message "};

static GtkWidget *b_edit;
static GtkWidget *b_set;
static GtkWidget *b_remove;

void clear_alarmlist_selection(void) {
    gtk_widget_set_sensitive(b_edit,   FALSE);
    gtk_widget_set_sensitive(b_set,    FALSE);
    gtk_widget_set_sensitive(b_remove, FALSE);
}

static void
alarmlist_selection_changed(void)
{
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(alarmlist));
    gboolean selected = gtk_tree_selection_get_selected(selection, NULL, NULL);

    gtk_widget_set_sensitive(b_edit,   selected);
    gtk_widget_set_sensitive(b_set,    selected);
    gtk_widget_set_sensitive(b_remove, selected);
}

static GtkWidget *
create_alarmlist (void)
{
    GtkTreeView *view;
    GtkTreeSelection *selection;
    GtkCellRenderer *renderer;
    GtkTreeModel *model;

    view = GTK_TREE_VIEW(gtk_tree_view_new());

    selection = gtk_tree_view_get_selection(view);
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(view, -1,
                                                clist_titles[COL_STATUS],
                                                renderer, "text", COL_STATUS,
                                                NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(view, -1,
                                                clist_titles[COL_HOUR],
                                                renderer, "text", COL_HOUR,
                                                NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(view, -1,
                                                clist_titles[COL_DAY],
                                                renderer, "text", COL_DAY,
                                                NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(view, -1,
                                                clist_titles[COL_MESSAGE],
                                                renderer, "text", COL_MESSAGE,
                                                NULL);

    model = GTK_TREE_MODEL(gtk_list_store_new(NUM_COLS,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_POINTER));

    gtk_tree_view_set_model(view, model);

    g_object_unref (model);

    return GTK_WIDGET(view);
}

void create_mainwindow(void) {
    GtkWidget *main_vbox;
    GtkWidget *buttons_hbox;
    GtkWidget *left_vbox;
    GtkWidget *right_vbox;
    GtkWidget *bouton;
    GtkWidget *scrolled_window;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *frame;
    GtkWidget *table;
    GtkWidget *notebook;

    /*** FENÊTRE PRINCIPALE ***/
    application = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(application), PACKAGE" Config");
    /*-- Connexion aux signaux --*/
    g_signal_connect(application, "delete_event", G_CALLBACK(quit_app), NULL);
    g_signal_connect(application, "destroy",      G_CALLBACK(quit_app), NULL);
    /*-- Taille de la fenêtre --*/
    gtk_widget_set_size_request(GTK_WIDGET(application), WIN_WIDTH, WIN_HEIGHT);
    gtk_widget_realize(application);

    /*** Zone principale de placement des widgets***/
    main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 1);
    gtk_container_add(GTK_CONTAINER(application), main_vbox);
    gtk_widget_show(main_vbox);



    /*** WIDGETS DE DONNÉES ***/
    notebook = gtk_notebook_new();

    /*-- Zones des données simples --*/
    left_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);


    /*--- Frame 1 ---*/
    frame = gtk_frame_new(" Startup ");
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(left_vbox), frame, FALSE, TRUE, 1);
    gtk_widget_show(frame);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(frame), hbox);
    gtk_widget_show(hbox);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 1);
    gtk_widget_show(vbox);

    wid_backlight = gtk_check_button_new_with_label("Back (or LED) light");
    gtk_box_pack_start(GTK_BOX(vbox), wid_backlight, FALSE, TRUE, 1);
    gtk_widget_show(wid_backlight);

    wid_blink = gtk_check_button_new_with_label("Blink");
    gtk_box_pack_start(GTK_BOX(vbox), wid_blink, FALSE, TRUE, 1);
    gtk_widget_show(wid_blink);

    wid_h12 = gtk_check_button_new_with_label("12h clock mode");
    gtk_box_pack_start(GTK_BOX(vbox), wid_h12, FALSE, TRUE, 1);
    gtk_widget_show(wid_h12);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 1);
    gtk_widget_show(vbox);

    wid_locale = gtk_check_button_new_with_label("Use current locale");
    gtk_box_pack_start(GTK_BOX(vbox), wid_locale, FALSE, TRUE, 1);
    gtk_widget_show(wid_locale);

    wid_showcal = gtk_check_button_new_with_label("Show calendar");
    gtk_box_pack_start(GTK_BOX(vbox), wid_showcal, FALSE, TRUE, 1);
    gtk_widget_show(wid_showcal);

    wid_calalrms = gtk_check_button_new_with_label("Load calendar alarms");
    gtk_box_pack_start(GTK_BOX(vbox), wid_calalrms, FALSE, TRUE, 1);
    gtk_widget_show(wid_calalrms);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 1);
    gtk_widget_show(vbox);

    wid_clk = gtk_radio_button_new_with_label(NULL, "Normal clock");
    gtk_box_pack_start(GTK_BOX(vbox), wid_clk, FALSE, TRUE, 1);
    gtk_widget_show(wid_clk);

    wid_itm = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wid_clk), "Internet time");
    gtk_box_pack_start(GTK_BOX(vbox), wid_itm, FALSE, TRUE, 1);
    gtk_widget_show(wid_itm);

    wid_bin = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wid_clk), "Binary clock");
    gtk_box_pack_start(GTK_BOX(vbox), wid_bin, FALSE, TRUE, 1);
    gtk_widget_show(wid_bin);

    /*--- Frame 2 ---*/
    frame = gtk_frame_new(" Look ");
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(left_vbox), frame, FALSE, TRUE, 1);
    gtk_widget_show(frame);
    
    table = gtk_table_new(2, 3, FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(table), 2);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    label = gtk_label_new("Style directory : ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
    gtk_widget_show(label);
    
    wid_styledir = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), wid_styledir, 1, 2, 0, 1);
    gtk_widget_show(wid_styledir);

    label = gtk_label_new("Style : ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);
    gtk_widget_show(label);
    
    wid_stylename = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), wid_stylename, 1, 2, 1, 2);
    gtk_widget_show(wid_stylename);

    label = gtk_label_new("Color : ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);
    gtk_widget_show(label);
    
    wid_color = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), wid_color, 1, 2, 2, 3);
    gtk_widget_show(wid_color);

    /*--- Frame 3 ---*/
    frame = gtk_frame_new(" Commands ");
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(left_vbox), frame, FALSE, FALSE, 1);
    gtk_widget_show(frame);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    gtk_widget_show(vbox);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 1);
    gtk_widget_show(hbox);

    label = gtk_label_new("Command : ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 1);
    gtk_widget_show(label);
    
    wid_command = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), wid_command, TRUE, TRUE, 1);
    gtk_widget_show(wid_command);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 1);
    gtk_widget_show(hbox);

    label = gtk_label_new("Message command : ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 1);
    gtk_widget_show(label);
    
    wid_msgcmd = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), wid_msgcmd, TRUE, TRUE, 1);
    gtk_widget_show(wid_msgcmd);


    label = gtk_label_new(" Misc options ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), left_vbox, label);
    gtk_widget_show(label);
    gtk_widget_show(left_vbox);


    /*-- Liste des alarmes --*/
    right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

    alarmlist = create_alarmlist();
    g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(alarmlist)),
		     "changed", G_CALLBACK(alarmlist_selection_changed), NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), alarmlist);
    gtk_widget_show(alarmlist);

    gtk_box_pack_start(GTK_BOX(right_vbox), scrolled_window, TRUE, TRUE, 1);
    gtk_widget_show(scrolled_window);

    /*--- Frame 4 ---*/
    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(right_vbox), frame, FALSE, TRUE, 1);
    gtk_widget_show(frame);

    table = gtk_table_new(2, 2, FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(table), 2);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    bouton = gtk_button_new_with_label(" Add alarm ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(add_alarm), NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), bouton, 0, 1, 0, 1);
    gtk_widget_show(bouton);

    bouton = gtk_button_new_with_label(" Edit entry ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(edit_entry), NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), bouton, 0, 1, 1, 2);
    gtk_widget_set_sensitive(bouton, FALSE);
    gtk_widget_show(bouton);
    b_edit = bouton;

    bouton = gtk_button_new_with_label(" Switch On/Off ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(switch_onoff), NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), bouton, 1, 2, 1, 2);
    gtk_widget_set_sensitive(bouton, FALSE);
    gtk_widget_show(bouton);
    b_set = bouton;

    bouton = gtk_button_new_with_label(" Remove alarm ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(remove_alarm), NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), bouton, 1, 2, 0, 1);
    gtk_widget_set_sensitive(bouton, FALSE);
    gtk_widget_show(bouton);
    b_remove = bouton;


    label = gtk_label_new(" Alarms ");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), right_vbox, label);
    gtk_widget_show(label);
    gtk_widget_show(right_vbox);


    gtk_box_pack_start(GTK_BOX(main_vbox), notebook, TRUE, TRUE, 1);
    gtk_widget_show(notebook);



    /*** BOUTONS DE SAUVEGARDE ET ANNULATION ***/
    buttons_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(main_vbox), buttons_hbox, FALSE, TRUE, 1);
    gtk_widget_show(buttons_hbox);


    bouton = gtk_button_new_with_label(" Save ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(save_datas), NULL);
    g_signal_connect(bouton, "clicked", G_CALLBACK(quit_app),   NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), bouton, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_grab_default(GTK_WIDGET(bouton));
    gtk_widget_show(bouton);

    bouton = gtk_button_new_with_label(" Cancel ");
    g_signal_connect(bouton, "clicked", G_CALLBACK(quit_app), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), bouton, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(bouton), TRUE);
    gtk_widget_show(bouton);



    /*** AFFICHAGE DE LA FENÊTRE ***/
    gtk_widget_show(application);
}
