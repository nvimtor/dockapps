/*
 * Create the main window.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#include "defines.h"
#include "mainwindow.h"
#include "main.h"
#include "tools.h"


#define UNIQUE 1
#define YEAR   2
#define MONTH  3

#define UNIQSTR "%04d-%02d-"
#define YEARSTR "XXXX-%02d-"
#define MONTSTR "XXXX-XX-"


GtkWidget *application;

/* Calendar part */
static GtkWidget *calendar;
static GtkWidget *closewindow;

/* Editor part */
static GtkWidget *text_buttons;
static GtkWidget *label_u;
static GtkWidget *label_y;
static GtkWidget *label_m;
static GtkWidget *button_u = NULL;
static GtkWidget *button_y = NULL;
static GtkWidget *button_m = NULL;
static GtkWidget *edit;
static GtkWidget *save;
static GtkWidget *delete;
static GtkWidget *cancel;
static int        shown = 1;
static int        dateb = 0;
static char       daystr[MAXSTRLEN + 1];


static void show_editor(void) {
    gtk_widget_hide(calendar);
    gtk_widget_hide(closewindow);
    gtk_widget_grab_default(GTK_WIDGET(cancel));
    gtk_widget_grab_focus(GTK_WIDGET(edit));
    gtk_widget_show(text_buttons);
    gtk_widget_show(edit);
    gtk_widget_show(save);
    gtk_widget_show(delete);
    gtk_widget_show(cancel);
}


static void hide_editor(void) {
    gtk_widget_hide(text_buttons);
    gtk_widget_hide(edit);
    gtk_widget_hide(save);
    gtk_widget_hide(delete);
    gtk_widget_hide(cancel);
    gtk_widget_show(calendar);
    gtk_widget_grab_default(GTK_WIDGET(closewindow));
    gtk_widget_grab_focus(GTK_WIDGET(closewindow));
    gtk_widget_show(closewindow);
}


static void toggle_displ(void) {
    switch (shown) {
        case 1:
            show_editor();
            shown = 2;
            break;
        case 2:
            hide_editor();
            shown = 1;
            break;
    }
}


static void load_file(const char *datestr) {
    FILE *file;
    char *filename = get_file(datestr);

    GtkTextIter iter;
    GtkTextBuffer *buf;

    buf = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit)));
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &iter);
    gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &iter);

    if ((file = fopen(filename, "r")) != NULL) {
        while (! feof(file)) {
            char line[MAXSTRLEN + 1];
            if (!fgets(line, MAXSTRLEN, file))
                break;
            if (line[0] != 0)
	      gtk_text_buffer_insert(buf, &iter, line, -1);
        }
        fclose(file);
    }
    FREE(filename);
}


static void toggle_button(GtkWidget *button) {
    int is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));

    if (is_active)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
}


static void toggle_buttons(int button) {
    switch (button) {
        case UNIQUE: toggle_button(button_u); break;
        case YEAR:   toggle_button(button_y); break;
        case MONTH:  toggle_button(button_m); break;
        default:     break;
    }
}


static void to_button(int button) {
    if (dateb == 0)
	    dateb = button;
    if (button != dateb) {
        int b = dateb;
        dateb = button;
        toggle_buttons(b);
    }
}


static void set_buttons_text(void) {
    guint  year, month, day;
    char datestr[MAXSTRLEN + 1];

    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    month++;
    snprintf(datestr, MAXSTRLEN, UNIQSTR"%02u", year, month, day);
    gtk_label_set_text(GTK_LABEL(label_u), datestr);
    snprintf(datestr, MAXSTRLEN, YEARSTR"%02u", month, day);
    gtk_label_set_text(GTK_LABEL(label_y), datestr);
    snprintf(datestr, MAXSTRLEN, MONTSTR"%02u", day);
    gtk_label_set_text(GTK_LABEL(label_m), datestr);
}


static void editor_flush(void) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), "", 0);
}


static void editor_fill(int which) {
    const char *dstr;
    GtkWidget *label;
    GtkTextIter iter;

    switch (which) {
    case UNIQUE: label = label_u; break;
    case YEAR:   label = label_y; break;
    case MONTH:  label = label_m; break;
    }
    dstr = gtk_label_get_text(GTK_LABEL(label));
    strcpy(daystr, dstr);
    to_button(which);
    editor_flush();
    load_file(daystr);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &iter);
    gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &iter);
}


static void check_button(int bnum, GtkWidget *button) {
    int is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));

    if (!(button_u && button_y && button_m)) return;
    if (!is_active && (dateb == bnum))
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    if (is_active && (dateb != bnum)) editor_fill(bnum);
}


static void cal_click(void) {
    set_buttons_text();
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_u), TRUE);
    editor_fill(UNIQUE);
}


static void set_text_u(void) {
    check_button(UNIQUE, button_u);
}


static void set_text_y(void) {
    check_button(YEAR, button_y);
}


static void set_text_m(void) {
    check_button(MONTH, button_m);
}


static void save_datas(void) {
    char *filename = get_file(daystr);
    int   len      = strlen(robust_home()) + strlen(DEFAULT_CONFIGDIR);
    char *dirname  = xmalloc(len + 2);
    struct stat stat_buf;

    GtkTextIter ts, te;
    gchar *tbuf;
    int tlen;

    tlen = gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))));
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &ts, &te);

    sprintf(dirname, "%s/%s", robust_home(), DEFAULT_CONFIGDIR);
    if (tlen > 0) {
        if (! ((stat(dirname, &stat_buf) == 0) && S_ISDIR(stat_buf.st_mode)))
            mkdir(dirname, 0755);

        if ((stat(dirname, &stat_buf) == 0) && S_ISDIR(stat_buf.st_mode)) {
            FILE *file = fopen(filename, "w");
            guint year, month, day;

            if (file) {
	      tbuf = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit))), &ts, &te, TRUE);
	      fprintf(file, "%s", tbuf);
	      g_free(tbuf);
	      fflush(file);
	      fclose(file);
            }
            gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
            gtk_calendar_mark_day(GTK_CALENDAR(calendar), day);
        }
    }
    FREE(filename);
}


static void delete_file(void) {
    char *filename = get_file(daystr);
    guint year, month, day;

    unlink(filename);
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    gtk_calendar_unmark_day(GTK_CALENDAR(calendar), day);
    FREE(filename);
    editor_flush();
}


static int check_day(const char *filename, const char *startstr) {
    int day = -1;

    if (strncmp(filename, startstr, strlen(startstr)) == 0) {
        char format[12];
        sprintf(format, "%s%%d", startstr);
        sscanf(filename, format, &day);
    }

    return day;
}


static void mark_days(void) {
    char          *Home = robust_home();
    DIR           *dir;
    struct dirent *dir_ent;
    char          *dirname = xmalloc(
            strlen(Home) +
            strlen(DEFAULT_CONFIGDIR) +
            3);

    gtk_calendar_clear_marks(GTK_CALENDAR(calendar));
    sprintf(dirname, "%s/%s", Home, DEFAULT_CONFIGDIR);
    if ((dir = opendir(dirname)) != NULL) {
        char startstr_u[9]; /* unique (full date) */
        char startstr_y[9]; /* yearly date */
        char startstr_m[9]; /* monthly date */
        guint year, month, day;

        gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
        month++;
        sprintf(startstr_u, UNIQSTR, year, month);
        sprintf(startstr_y, YEARSTR, month);
        sprintf(startstr_m, MONTSTR);
        while ((dir_ent = readdir(dir)) != NULL) {
            int day_u = check_day(dir_ent->d_name, startstr_u);
            int day_y = check_day(dir_ent->d_name, startstr_y);
            int day_m = check_day(dir_ent->d_name, startstr_m);
            if (day_u != -1)
                gtk_calendar_mark_day(GTK_CALENDAR(calendar), day_u);
            if (day_y != -1)
                gtk_calendar_mark_day(GTK_CALENDAR(calendar), day_y);
            if (day_m != -1)
                gtk_calendar_mark_day(GTK_CALENDAR(calendar), day_m);
        }
        closedir(dir);
    }
    FREE(dirname);
}


void create_mainwindow(void) {
    GtkWidget *main_vbox;
    GtkWidget *buttons_hbox;

    /*** FENÊTRE PRINCIPALE ***/
    application = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(application), PACKAGE" Calendar");
    /*-- Connexion aux signaux --*/
    g_signal_connect(application, "delete_event", G_CALLBACK(quit_app), NULL);
    g_signal_connect(application, "destroy",      G_CALLBACK(quit_app), NULL);
    /*-- Taille de la fenêtre --*/
    gtk_widget_set_size_request(GTK_WIDGET(application), WIN_WIDTH, WIN_HEIGHT);
    gtk_widget_realize(application);

    /*** Zone principale de placement des widgets***/
    main_vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 1);
    gtk_container_add(GTK_CONTAINER(application), main_vbox);
    gtk_widget_show(main_vbox);

    calendar = gtk_calendar_new();
    gtk_calendar_set_display_options(GTK_CALENDAR(calendar),
                                     GTK_CALENDAR_SHOW_HEADING   |
                                     GTK_CALENDAR_SHOW_DAY_NAMES);
    gtk_calendar_select_month(GTK_CALENDAR(calendar),
                              timeinfos->tm_mon, timeinfos->tm_year + 1900);
    gtk_calendar_select_day(GTK_CALENDAR(calendar), timeinfos->tm_mday);
    mark_days();
    gtk_box_pack_start(GTK_BOX(main_vbox), calendar, TRUE, TRUE, 1);
    g_signal_connect(calendar, "day-selected-double-click",
                     G_CALLBACK(cal_click), NULL);
    g_signal_connect(calendar, "day-selected-double-click",
                     G_CALLBACK(toggle_displ), NULL);
    g_signal_connect(calendar, "month-changed", G_CALLBACK(mark_days), NULL);
    gtk_widget_show(calendar);

    edit = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(edit), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(edit), GTK_WRAP_WORD_CHAR);

    gtk_box_pack_start(GTK_BOX(main_vbox), edit, TRUE, TRUE, 1);


    /*** BOUTONS DE CHANGEMENT DE TEXTE ***/
    text_buttons = gtk_hbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(main_vbox), text_buttons, FALSE, TRUE, 1);


    button_u = gtk_toggle_button_new();
    g_signal_connect(button_u, "clicked", G_CALLBACK(set_text_u), NULL);
    gtk_box_pack_start(GTK_BOX(text_buttons), button_u, TRUE, TRUE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_u), TRUE);
    gtk_widget_show(button_u);
    label_u = gtk_label_new(" Unique ");
    gtk_widget_show(label_u);
    gtk_container_add(GTK_CONTAINER(button_u), label_u);


    button_y = gtk_toggle_button_new();
    g_signal_connect(button_y, "clicked", G_CALLBACK(set_text_y), NULL);
    gtk_box_pack_start(GTK_BOX(text_buttons), button_y, TRUE, TRUE, 0);
    gtk_widget_show(button_y);
    label_y = gtk_label_new(" Yearly ");
    gtk_widget_show(label_y);
    gtk_container_add(GTK_CONTAINER(button_y), label_y);


    button_m = gtk_toggle_button_new();
    g_signal_connect(button_m, "clicked", G_CALLBACK(set_text_m), NULL);
    gtk_box_pack_start(GTK_BOX(text_buttons), button_m, TRUE, TRUE, 0);
    gtk_widget_show(button_m);
    label_m = gtk_label_new(" Monthly ");
    gtk_widget_show(label_m);
    gtk_container_add(GTK_CONTAINER(button_m), label_m);



    /*** BOUTONS DE SAUVEGARDE ET ANNULATION ***/
    buttons_hbox = gtk_hbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(main_vbox), buttons_hbox, FALSE, TRUE, 1);
    gtk_widget_show(buttons_hbox);


    closewindow = gtk_button_new_with_label(" Close ");
    g_signal_connect(closewindow, "clicked", G_CALLBACK(quit_app), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), closewindow, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(closewindow), TRUE);
    gtk_widget_grab_default(GTK_WIDGET(closewindow));
    gtk_widget_show(closewindow);


    save = gtk_button_new_with_label(" Save ");
    g_signal_connect(save, "clicked", G_CALLBACK(save_datas), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), save, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(save), TRUE);


    delete = gtk_button_new_with_label(" Delete ");
    g_signal_connect(delete, "clicked", G_CALLBACK(delete_file), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), delete, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(delete), TRUE);


    cancel = gtk_button_new_with_label(" Close ");
    g_signal_connect(cancel, "clicked", G_CALLBACK(toggle_displ), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_hbox), cancel, TRUE, TRUE, 0);
    gtk_widget_set_can_default(GTK_WIDGET(cancel), TRUE);


    /*** AFFICHAGE DE LA FENÊTRE ***/
    gtk_widget_show(application);
}
