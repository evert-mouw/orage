/* appointment.c
 *
 * Copyright (C) 2004-2005 Mickaël Graf <korbinus at xfce.org>
 * Copyright (C) 2005 Juha Kautto <juha at xfce.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define _XOPEN_SOURCE /* glibc2 needs this */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include <libxfce4util/libxfce4util.h>
#include <libxfcegui4/netk-trayicon.h>
#include <libxfcegui4/libxfcegui4.h>
#include <libxfce4mcs/mcs-client.h>

#include "mainbox.h"
#include "appointment.h"
#include "event-list.h"
#include "ical-code.h"
#include "functions.h"

#define AVAILABILITY_ARRAY_DIM 2
#define RECURRENCY_ARRAY_DIM 5
#define ALARM_ARRAY_DIM 11
#define FILETYPE_SIZE 38


void delete_xfical_from_appt_win (appt_win *apptw);

gboolean ical_to_year_month_day_hours_minutes(char *ical, int *year, int *month, int *day, int *hours, int *minutes){
    int i, j;
    char cyear[5], cmonth[3], cday[3], chours[3], cminutes[3];

    if(strlen(ical) >= 8){
       for (i = 0, j = 0; i < 4; i++){
            cyear[j++] = ical[i];
        }
        cyear[4] = '\0';

        cmonth[0] = ical[4];
        cmonth[1] = ical[5];
        cmonth[2] = '\0';

        cday[0] = ical[6];
        cday[1] = ical[7];
        cday[2] = '\0';

        *year = atoi(cyear);
        *month = atoi(cmonth);
        *day = atoi(cday);

        if(strlen(ical) >= 13){
            chours[0] = ical[9];
            chours[1] = ical[10];
            chours[2] = '\0';

            cminutes[0] = ical[11];
            cminutes[1] = ical[12];
            cminutes[2] = '\0';

            *hours = atoi(chours);
            *minutes = atoi(cminutes);

        }
        else{
            *hours = -1;
            *minutes = -1;
        }

        return TRUE;
    }
    else{
        g_warning("ical string uncomplete\n");
        return FALSE;
    }
}

void year_month_day_to_display(int year, int month, int day, char *string_to_display){
    const char *date_format;
    struct tm *d;

    d = (struct tm *)malloc(1*sizeof(struct tm));

    date_format = _("%m/%d/%Y");
    d->tm_mday = day;
    d->tm_mon = month - 1;
    d->tm_year = year - 1900;

    strftime(string_to_display, 11, date_format, (const struct tm *) d);

    free(d);
}

void
on_appTitle_entry_changed_cb(GtkEditable *entry, gpointer user_data)
{
    gchar *title, *application_name;
    const gchar *appointment_name;

    appt_win *apptw = (appt_win *)user_data;

    appointment_name = gtk_entry_get_text((GtkEntry *)apptw->appTitle_entry);
    application_name = _("Orage");

    if(strlen((char *)appointment_name) > 0)
        title = g_strdup_printf("%s - %s", appointment_name, application_name);
    else
        title = g_strdup_printf("%s", application_name);

    gtk_window_set_title (GTK_WINDOW (apptw->appWindow), (const gchar *)title);

    g_free(title);
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
appSoundRepeat_checkbutton_clicked_cb(GtkCheckButton *checkbutton, gpointer user_data)
{
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_appNote_buffer_changed_cb(GtkTextBuffer *buffer, gpointer user_data){
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_appLocation_entry_changed_cb(GtkEditable *entry, gpointer user_data){
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_appSound_entry_changed_cb(GtkEditable *entry, gpointer user_data){
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_appTime_comboboxentry_changed_cb(GtkEditable *entry, gpointer user_data){
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_app_combobox_changed_cb(GtkComboBox *cb, gpointer user_data){
    appt_win *apptw = (appt_win *)user_data;
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);
}

void
on_appSound_button_clicked_cb(GtkButton *button, gpointer user_data)
{
    GtkWidget *file_chooser;
	XfceFileFilter *filter;

    gchar *appSound_entry_filename;

    const gchar * filetype[FILETYPE_SIZE] = {"*.aiff", "*.al", "*.alsa", "*.au", "*.auto", "*.avr",
                                             "*.cdr", "*.cvs", "*.dat", "*.vms", "*.gsm", "*.hcom", 
                                             "*.la", "*.lu", "*.maud", "*.mp3", "*.nul", "*.ogg", "*.ossdsp",
                                             "*.prc", "*.raw", "*.sb", "*.sf", "*.sl", "*.smp", "*.sndt", 
                                             "*.sph", "*.8svx", "*.sw", "*.txw", "*.ub", "*.ul", "*.uw",
                                             "*.voc", "*.vorbis", "*.vox", "*.wav", "*.wve"};

    register int i;

    appt_win *apptw = (appt_win *)user_data;
    appSound_entry_filename = g_strdup(gtk_entry_get_text((GtkEntry *)apptw->appSound_entry));

    file_chooser = xfce_file_chooser_new (_("Select a file..."),
                                            GTK_WINDOW (apptw->appWindow),
                                            XFCE_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

    filter = xfce_file_filter_new ();
	xfce_file_filter_set_name(filter, _("Sound Files"));
    for (i = 0; i < FILETYPE_SIZE; i++){
        xfce_file_filter_add_pattern(filter, filetype[i]);
    }
	xfce_file_chooser_add_filter(XFCE_FILE_CHOOSER(file_chooser), filter);
	filter = xfce_file_filter_new ();
	xfce_file_filter_set_name(filter, _("All Files"));
	xfce_file_filter_add_pattern(filter, "*");
	xfce_file_chooser_add_filter(XFCE_FILE_CHOOSER(file_chooser), filter);

    xfce_file_chooser_add_shortcut_folder(XFCE_FILE_CHOOSER(file_chooser), PACKAGE_DATA_DIR "/xfcalendar/sounds", NULL);

    if(strlen(appSound_entry_filename) > 0){
        g_warning("File: %s\n", appSound_entry_filename);
        xfce_file_chooser_set_filename(XFCE_FILE_CHOOSER(file_chooser), 
                                       (const gchar *) appSound_entry_filename);
    }
    else
        xfce_file_chooser_set_current_folder(XFCE_FILE_CHOOSER(file_chooser), PACKAGE_DATA_DIR "/xfcalendar/sounds");

	if(gtk_dialog_run(GTK_DIALOG(file_chooser)) == GTK_RESPONSE_ACCEPT) {
        gchar *sound_file;
		sound_file = xfce_file_chooser_get_filename(XFCE_FILE_CHOOSER(file_chooser));

        if(sound_file){
			gtk_entry_set_text(GTK_ENTRY(apptw->appSound_entry), sound_file);
			gtk_editable_set_position(GTK_EDITABLE(apptw->appSound_entry), -1);
        }
    }

    gtk_widget_destroy(file_chooser);
    g_free(appSound_entry_filename);
}

void
on_appAllDay_clicked_cb(GtkCheckButton *checkbutton, gpointer user_data)
{
    gboolean check_status;
    appt_win *apptw = (appt_win *)user_data;

    check_status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(apptw->appAllDay_checkbutton));

    if(check_status){
        gtk_widget_set_sensitive(apptw->appStartTime_comboboxentry, FALSE);
        gtk_widget_set_sensitive(apptw->appEndTime_comboboxentry, FALSE);
    } else {
        gtk_widget_set_sensitive(apptw->appStartTime_comboboxentry, TRUE);
        gtk_widget_set_sensitive(apptw->appEndTime_comboboxentry, TRUE);
    }
    apptw->appointment_changed = TRUE;
    gtk_widget_set_sensitive(apptw->appRevert, TRUE);    
}

gboolean appWindow_check_and_close(appt_win *apptw){
    gint result;
    if(apptw->appointment_changed == TRUE){
        result = xfce_message_dialog(GTK_WINDOW(apptw->appWindow),
                                     _("Warning"),
                                     GTK_STOCK_DIALOG_WARNING,
                                     _("The appointment information has been modified."),
                                     _("Do you want to continue?"),
                                     GTK_STOCK_YES,
                                     GTK_RESPONSE_ACCEPT,
                                     GTK_STOCK_NO,
                                     GTK_RESPONSE_CANCEL,
                                     NULL);

        if(result == GTK_RESPONSE_ACCEPT){
            gtk_widget_destroy(apptw->appWindow);
            g_free(apptw);
        }
    }
    else{
        gtk_widget_destroy(apptw->appWindow);
        g_free(apptw);
    }
    return TRUE;
}

gboolean
on_appWindow_delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data){
    return appWindow_check_and_close((appt_win *) user_data);
}

/*
 * fill_appt
 * This function fill an appointment with the content of an appointment window
 */
void
fill_appt(appt_type *appt, appt_win *apptw)
{
    GtkTextIter start, end;
    const char *date_format, *time_format;
    struct tm current_t;
    char *returned_by_strptime;

    /*Get the title */
    appt->title = (gchar *) gtk_entry_get_text((GtkEntry *)apptw->appTitle_entry);
    /* Get the location */
    appt->location = (gchar *) gtk_entry_get_text((GtkEntry *)apptw->appLocation_entry);

    date_format = _("%m/%d/%Y");
    time_format = "%H:%M";

    /* Get the start date and time */
    current_t.tm_hour = 0;
    current_t.tm_min = 0;

    returned_by_strptime = strptime(gtk_button_get_label(GTK_BUTTON(apptw->appStartDate_button)), date_format, &current_t);
    returned_by_strptime = strptime(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appStartTime_comboboxentry)->child)), time_format, &current_t);

    g_sprintf(appt->starttime, XFICAL_APP_TIME_FORMAT
            , current_t.tm_year + 1900, current_t.tm_mon + 1, current_t.tm_mday
            , current_t.tm_hour, current_t.tm_min, 0);

    /* Get the end date and time */
    current_t.tm_hour = 0;
    current_t.tm_min = 0;

    returned_by_strptime = strptime(gtk_button_get_label(GTK_BUTTON(apptw->appEndDate_button)), date_format, &current_t);
    returned_by_strptime = strptime(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appEndTime_comboboxentry)->child)), time_format, &current_t);
    g_sprintf(appt->endtime, XFICAL_APP_TIME_FORMAT
            , current_t.tm_year + 1900, current_t.tm_mon + 1, current_t.tm_mday
            , current_t.tm_hour, current_t.tm_min, 0);

    /* Get when the reminder when the reminder will show up */
    appt->alarmtime = gtk_combo_box_get_active((GtkComboBox *)apptw->appAlarm_combobox);

    /* Get the recurrence */
    appt->freq = gtk_combo_box_get_active((GtkComboBox *)apptw->appRecurrency_cb);
    /* Get the availability */
    appt->availability = gtk_combo_box_get_active((GtkComboBox *)apptw->appAvailability_cb);
    /* Get the notes */
    gtk_text_buffer_get_bounds(gtk_text_view_get_buffer((GtkTextView *)apptw->appNote_textview), 
                                &start, 
                                &end);

    appt->note = gtk_text_iter_get_text(&start, &end);
    /* Get if the appointment is for the all day */
    appt->allDay = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(apptw->appAllDay_checkbutton));
    /* Get if the alarm will repeat until someone shuts it off */
    appt->alarmrepeat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(apptw->appSoundRepeat_checkbutton));
    /* Which sound file will be played */
    appt->sound = (gchar *) gtk_entry_get_text((GtkEntry *)apptw->appSound_entry);

}

gboolean xfcalendar_validate_time(gchar *str){
    /* I tried to use regexec here but it was such a hassle that 
     * I decided to go the hard way :(
     */
    char chh[3], cmm[3];
    int hh, mm;

    if (strlen((char *)str) == 0) {
        return TRUE;
    }

    if (strlen((char *)str) != 5) {
        g_warning ("Wrong length for %s", str);
        return FALSE;
    }

    if (str[2] != ':') {
        g_warning ("Wrong separator for %s", str);
        return FALSE;
    }

    chh[0] = str[0];
    chh[1] = str[1];
    chh[2] = '\0';

    cmm[0] = str[3];
    cmm[1] = str[4];
    cmm[2] = '\0';

    if (!isdigit(chh[0]) ||
        !isdigit(chh[1]) ||
        !isdigit(cmm[0]) ||
        !isdigit(cmm[1])) {
        g_warning("Incorrect numbers %s", str);
        return FALSE;
    }

    hh = atoi (chh);
    mm = atoi (cmm);

    if (!(hh >= 0 && hh < 24) ||
        !(mm >= 0 && mm < 60)) {
        g_warning("Time values out of range %d and %d", hh, mm);
        return FALSE;
    }

    return TRUE;

}

gboolean xfcalendar_validate_datetime (GtkWidget *parent, gchar *startdatetime, gchar *enddatetime, 
                            gchar *starttime, gchar *endtime){
    gint result;
    if(g_ascii_strcasecmp(enddatetime, startdatetime) < 0){
        result = xfce_message_dialog(GTK_WINDOW(parent),
                                     _("Warning"),
                                     GTK_STOCK_DIALOG_WARNING,
                                     _("The end of this appointment is earlier than the beginning."),
                                     NULL,
                                     GTK_STOCK_OK,
                                     GTK_RESPONSE_ACCEPT,
                                     NULL);
        return FALSE;
    }
    else if(!xfcalendar_validate_time(starttime) || !xfcalendar_validate_time(endtime)){
        result = xfce_message_dialog(GTK_WINDOW(parent),
                                     _("Warning"),
                                     GTK_STOCK_DIALOG_WARNING,
                                     _("A time value is wrong."),
                                     _("Time values must be written 'hh:mm', for instance '09:36' or '15:23'."),
                                     GTK_STOCK_OK,
                                     GTK_RESPONSE_ACCEPT,
                                     NULL);
        return FALSE;
    }
    else
        return TRUE;
}

void
on_appFileClose_menu_activate_cb(GtkMenuItem *menuitem, gpointer user_data)
{
    appWindow_check_and_close((appt_win *) user_data);
}

void save_xfical_from_appt_win (appt_win *apptw){
    gchar *starttime, *endtime;

    gboolean ok = FALSE;
    appt_type *appt = xfical_app_alloc();

    starttime = g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appStartTime_comboboxentry)->child)));
    endtime = g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appEndTime_comboboxentry)->child)));

    fill_appt(appt, apptw);

    if(xfcalendar_validate_datetime(apptw->appWindow, appt->starttime, appt->endtime, starttime, endtime)){
        /* Here we try to save the event... */
        if (xfical_file_open()){
            if (apptw->add_appointment) {
                apptw->xf_uid =  g_strdup(xfical_app_add(appt));
                apptw->add_appointment = FALSE;
                ok = TRUE;
                g_message("New ical uid: %s \n", apptw->xf_uid);
                gtk_widget_set_sensitive(apptw->appDuplicate, TRUE);
            }
            else {
                ok = xfical_app_mod(apptw->xf_uid, appt);
                g_message("Modified :%s \n", apptw->xf_uid);
            }
            apptw->appointment_changed = FALSE;
            gtk_widget_set_sensitive(apptw->appRevert, FALSE);
            xfical_file_close();
        }

        if (ok && apptw->eventlist != NULL) {
            recreate_eventlist_win((eventlist_win *)apptw->eventlist);
        }
        apptw->appointment_new = FALSE;
    }

    g_free(starttime);
    g_free(endtime);
    g_free(appt);
}

void
on_appFileSave_menu_activate_cb (GtkMenuItem *menuitem, gpointer user_data){
    save_xfical_from_appt_win ((appt_win *)user_data);
}

void
on_appSave_clicked_cb (GtkButton *button, gpointer user_data)
{
    save_xfical_from_appt_win((appt_win *)user_data);
}

void
save_xfical_from_appt_win_and_close (appt_win *apptw){
    gchar *starttime, *endtime;

    gboolean ok = FALSE;
    appt_type *appt = xfical_app_alloc();
    gchar *new_uid;

    starttime = g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appStartTime_comboboxentry)->child)));
    endtime = g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_BIN(apptw->appEndTime_comboboxentry)->child)));

    fill_appt(appt, apptw);

    if(xfcalendar_validate_datetime(apptw->appWindow, appt->starttime, appt->endtime, starttime, endtime)){
        /* Here we try to save the event... */
        if (xfical_file_open()){
            if (apptw->add_appointment) {
                new_uid = xfical_app_add(appt);
                ok = TRUE;
                g_message("New ical uid: %s \n", new_uid);
            }
            else {
                ok = xfical_app_mod(apptw->xf_uid, appt);
                g_message("Modified :%s \n", apptw->xf_uid);
            }
            xfical_file_close();
        }

        if (ok && apptw->eventlist != NULL) {
            recreate_eventlist_win((eventlist_win *)apptw->eventlist);
        }
        gtk_widget_destroy(apptw->appWindow);
        g_free(apptw);
    }

    g_free(starttime);
    g_free(endtime);
    g_free(appt);
}

void 
on_appFileSaveClose_menu_activate_cb (GtkMenuItem *menuitem, gpointer user_data){
    save_xfical_from_appt_win_and_close ((appt_win *)user_data);
}
void
on_appSaveClose_clicked_cb(GtkButton *button, gpointer user_data)
{
    save_xfical_from_appt_win_and_close ((appt_win *)user_data);
}

void
on_appDelete_clicked_cb(GtkButton *button, gpointer user_data)
{
    delete_xfical_from_appt_win ((appt_win *) user_data);
}

void
on_appFileDelete_menu_activate_cb (GtkMenuItem *menuitem, gpointer user_data){
    delete_xfical_from_appt_win ((appt_win *) user_data);
}

void delete_xfical_from_appt_win (appt_win *apptw){
    gint result;

    result = xfce_message_dialog(GTK_WINDOW(apptw->appWindow),
                         _("Warning"),
                         GTK_STOCK_DIALOG_WARNING,
                         _("This appointment will be permanently removed."),
                         _("Do you want to continue?"),
                         GTK_STOCK_YES,
                         GTK_RESPONSE_ACCEPT,
                         GTK_STOCK_NO,
                         GTK_RESPONSE_REJECT,
                         NULL);
                                 
    if (result == GTK_RESPONSE_ACCEPT){
        if (!apptw->add_appointment)
            if (xfical_file_open()){
                xfical_app_del(apptw->xf_uid);
                xfical_file_close();
                g_message("Removed ical uid: %s \n", apptw->xf_uid);
            }

        if (apptw->eventlist != NULL)
            recreate_eventlist_win((eventlist_win *)apptw->eventlist);

        gtk_widget_destroy(apptw->appWindow);
        g_free(apptw);
    }
}

void duplicate_xfical_from_appt_win (appt_win *apptw){
    gint x, y;
    appt_win *app;
    app = create_appt_win("COPY", apptw->xf_uid, apptw->eventlist);
    gtk_window_get_position(GTK_WINDOW(apptw->appWindow), &x, &y);
    gtk_window_move(GTK_WINDOW(app->appWindow), x+20, y+20);
    gtk_widget_show(app->appWindow);
}

void
on_appFileDuplicate_menu_activate_cb (GtkMenuItem *menuitem, gpointer user_data){
    duplicate_xfical_from_appt_win ((appt_win *)user_data);
}

void
on_appDuplicate_clicked_cb (GtkButton *button, gpointer user_data)
{
    duplicate_xfical_from_appt_win ((appt_win *)user_data);
}

void
revert_xfical_to_last_saved (appt_win *apptw){
    if(!apptw->appointment_new){
        fill_appt_window(apptw, "UPDATE", apptw->xf_uid);
    }
    else{
        fill_appt_window(apptw, "NEW", apptw->chosen_date);
    }
}

void
on_appFileRevert_menu_activate_cb (GtkMenuItem *menuitem, gpointer user_data){
    revert_xfical_to_last_saved ((appt_win *) user_data);
}

void
on_appRevert_clicked_cb(GtkWidget *button, gpointer *user_data)
{
    revert_xfical_to_last_saved ((appt_win *) user_data);
}

void
on_appStartEndDate_clicked_cb (GtkWidget *button, gpointer *user_data)
{
    GtkWidget *selDate_Window_dialog;
    GtkWidget *selDate_Calendar_calendar;
    gint result;
    guint year, month, day;
    const char *date_format;
    char *date_to_display; 
    struct tm *t;
    struct tm current_t;
    char *returned_by_strptime;
    time_t tt;

    appt_win *apptw = (appt_win *)user_data;

    date_to_display = (char *)malloc(11);
    if(!date_to_display){
        g_warning("Memory allocation failure!\n");
    }

    date_format = _("%m/%d/%Y");
    returned_by_strptime = strptime(gtk_button_get_label(GTK_BUTTON(button)), date_format, &current_t);

    selDate_Window_dialog = gtk_dialog_new_with_buttons(NULL, GTK_WINDOW(apptw->appWindow),
                                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                        _("Today"), 
                                                        1,
                                                        GTK_STOCK_OK,
                                                        GTK_RESPONSE_ACCEPT,
                                                        NULL);

    selDate_Calendar_calendar = gtk_calendar_new();
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(selDate_Window_dialog)->vbox), selDate_Calendar_calendar);
    xfcalendar_select_date (GTK_CALENDAR (selDate_Calendar_calendar), current_t.tm_year+1900, 
                            current_t.tm_mon, current_t.tm_mday);
    gtk_widget_show_all(selDate_Window_dialog);

    result = gtk_dialog_run(GTK_DIALOG(selDate_Window_dialog));
    switch(result){
        case GTK_RESPONSE_ACCEPT:
            gtk_calendar_get_date(GTK_CALENDAR(selDate_Calendar_calendar), &year, &month, &day);
            year_month_day_to_display(year, month + 1, day, date_to_display);
            break;
        case 1:
            tt=time(NULL);
            t=localtime(&tt);
            year_month_day_to_display(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, date_to_display);
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            date_to_display = (char *)g_strdup(gtk_button_get_label(GTK_BUTTON(button)));
            break;
    }
    if (g_ascii_strcasecmp((gchar *)date_to_display, (gchar *)g_strdup(gtk_button_get_label(GTK_BUTTON(button)))) != 0) {
        apptw->appointment_changed = TRUE;
        gtk_widget_set_sensitive(apptw->appRevert, TRUE);
    }
    gtk_button_set_label(GTK_BUTTON(button), (const gchar *)date_to_display);
    free(date_to_display);
    gtk_widget_destroy(selDate_Window_dialog);
}

void fill_appt_window(appt_win *appt_w, char *action, char *par)
{
    char *startdate_to_display, *enddate_to_display,
         *starttime_to_display, *endtime_to_display;
    int year, month, day, hours, minutes;
    GtkTextBuffer *tb;
    appt_type *appt_data=NULL;
    struct tm *t;
    time_t tt;
    gchar *today;

    if (strcmp(action, "NEW") == 0) {
        appt_w->add_appointment = TRUE;
        appt_w->appointment_new = TRUE;
        appt_w->chosen_date = g_strdup((const gchar *)par);
        appt_data = xfical_app_alloc();
  /* par contains XFICAL_APP_DATE_FORMAT (yyyymmdd) date for new appointment */
        tt=time(NULL);
        t=localtime(&tt);
        today = (gchar *)malloc(9);
        g_sprintf (today, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
        /* If we're today, we propose an appointment the next half-hour */
        if (strcmp (appt_w->chosen_date, today) == 0) {
            if(t->tm_min <= 30){
                g_sprintf(appt_data->starttime,"%sT%02d%02d00"
                            , par, t->tm_hour, 30);
                g_sprintf(appt_data->endtime,"%sT%02d%02d00"
                            , par, t->tm_hour + 1, 00);
            }
            else{
                g_sprintf(appt_data->starttime,"%sT%02d%02d00"
                            , par, t->tm_hour + 1, 00);
                g_sprintf(appt_data->endtime,"%sT%02d%02d00"
                            , par, t->tm_hour + 1, 30);
            }
        }
        /* otherwise we suggest it at 09:00 in the morning. */
        else {
            g_sprintf(appt_data->starttime,"%sT090000", par);
            g_sprintf(appt_data->endtime,"%sT093000", par);
        }
        gtk_widget_set_sensitive(appt_w->appDuplicate, FALSE);
        g_message("Building NEW ical uid\n");
    }
    else if ((strcmp(action, "UPDATE") == 0) || (strcmp(action, "COPY") == 0)){
        g_message("%s uid: %s \n", action, par);
        appt_w->appointment_new = FALSE;
        /* If we're making a copy here, appt_w->add_appointment must become TRUE... */
        appt_w->add_appointment = (strcmp(action, "COPY") == 0);
        /* but the button for duplication must be inactivated in the new appointment */
        gtk_widget_set_sensitive(appt_w->appDuplicate, !appt_w->add_appointment);
        /* par contains ical uid */
        if (!xfical_file_open()) {
            g_message("ical file open failed\n");
            return;
        }
        if ((appt_data = xfical_app_get(par)) == NULL) {
            g_message("appointment not found\n");
            xfical_file_close();
            return;
        }
    }
    else
        g_error("unknown parameter\n");

	appt_w->xf_uid = g_strdup(appt_data->uid);

    gtk_window_set_title (GTK_WINDOW (appt_w->appWindow), _("New appointment - Orage"));
    gtk_entry_set_text(GTK_ENTRY(appt_w->appTitle_entry), (appt_data->title ? appt_data->title : ""));
    gtk_entry_set_text(GTK_ENTRY(appt_w->appLocation_entry), (appt_data->location ? appt_data->location : ""));
    if (strlen(appt_data->starttime) > 6 ) {
        g_message("starttime: %s\n", appt_data->starttime);
        ical_to_year_month_day_hours_minutes(appt_data->starttime, &year, &month, &day, &hours, &minutes);

        startdate_to_display = (char *)malloc(11);
        if(!startdate_to_display){
            g_warning("Memory allocation failure!\n");
        }
        year_month_day_to_display(year, month, day, startdate_to_display);
        gtk_button_set_label(GTK_BUTTON(appt_w->appStartDate_button), (const gchar *)startdate_to_display);
        free(startdate_to_display);

        starttime_to_display = (char *)malloc(6);
        if(!starttime_to_display){
            g_warning("Memory allocation failure!\n");
        }

        if(hours > -1 && minutes > -1){
            sprintf(starttime_to_display, "%02d:%02d", hours, minutes);
            gtk_entry_set_text(GTK_ENTRY(GTK_BIN(appt_w->appStartTime_comboboxentry)->child), (const gchar *)starttime_to_display);
        }
        free(starttime_to_display);
    }
    if (strlen( appt_data->endtime) > 6 ) {
        g_message("endtime: %s\n", appt_data->endtime);
        ical_to_year_month_day_hours_minutes(appt_data->endtime, &year, &month, &day, &hours, &minutes);

        enddate_to_display = (char *)malloc(11);
        if(!enddate_to_display){
            g_warning("Memory allocation failure!\n");
        }
        year_month_day_to_display(year, month, day, enddate_to_display);
        gtk_button_set_label(GTK_BUTTON(appt_w->appEndDate_button), (const gchar *)enddate_to_display);
        free(enddate_to_display);

        endtime_to_display = (char *)malloc(6);
        if(!endtime_to_display){
            g_warning("Memory allocation failure!\n");
        }

        if(hours > -1 && minutes > -1){
            sprintf(endtime_to_display, "%02d:%02d", hours, minutes);
            gtk_entry_set_text(GTK_ENTRY(GTK_BIN(appt_w->appEndTime_comboboxentry)->child), (const gchar *)endtime_to_display);
        }
        free(endtime_to_display);
    }
    gtk_entry_set_text(GTK_ENTRY(appt_w->appSound_entry), (appt_data->sound ? appt_data->sound : ""));
    if (appt_data->alarmtime != -1){
      gtk_combo_box_set_active(GTK_COMBO_BOX(appt_w->appAlarm_combobox)
                   , appt_data->alarmtime);
    }
	gtk_combo_box_set_active(GTK_COMBO_BOX(appt_w->appRecurrency_cb)
				   , appt_data->freq);
	if (appt_data->availability != -1){
	  gtk_combo_box_set_active(GTK_COMBO_BOX(appt_w->appAvailability_cb)
				   , appt_data->availability);
	}
	if (appt_data->note){
	  tb = gtk_text_view_get_buffer((GtkTextView *)appt_w->appNote_textview);
	  gtk_text_buffer_set_text(tb, (const gchar *) appt_data->note, -1);
	}
    else{
	  tb = gtk_text_view_get_buffer((GtkTextView *)appt_w->appNote_textview);
	  gtk_text_buffer_set_text(tb, "", -1);
    }
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(appt_w->appAllDay_checkbutton), appt_data->allDay);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(appt_w->appSoundRepeat_checkbutton), appt_data->alarmrepeat);

    if (strcmp(action, "NEW") == 0) {
        g_free(appt_data);
	}
    else
        xfical_file_close();
    appt_w->appointment_changed = FALSE;
    gtk_widget_set_sensitive(appt_w->appRevert, FALSE);
}

appt_win 
*create_appt_win(char *action, char *par, gpointer el)
{
    eventlist_win *event_list;

    register int i = 0;

    GtkWidget 
              *menu_separator,
              *toolbar_separator;

    char *availability_array[AVAILABILITY_ARRAY_DIM] = {_("Free"), _("Busy")},
         *recurrency_array[RECURRENCY_ARRAY_DIM] = {_("None"), _("Daily"), _("Weekly"), _("Monthly"), _("Yearly")},
         *alarm_array[ALARM_ARRAY_DIM] = {_("None"), _("5 minutes"), _("15 minutes"), _("30 minutes"),
                                          _("45 minutes"), _("1 hour"), _("2 hours"), _("4 hours"),
                                          _("8 hours"), _("1 day"), _("2 days")};

    appt_win *apptw = g_new(appt_win, 1);

    apptw->xf_uid = NULL;

    apptw->eventlist = el;                /* Keep memory of the parent, if any */
    event_list = (eventlist_win *) el;

    apptw->appWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(apptw->appWindow), 450, 325);

    apptw->appTooltips = gtk_tooltips_new();

    apptw->appAccelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group (GTK_WINDOW(apptw->appWindow), apptw->appAccelgroup);

    if (apptw->eventlist != NULL) {
      gtk_window_set_destroy_with_parent(GTK_WINDOW(apptw->appWindow), TRUE);
    }
    
    apptw->appVBox1 = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (apptw->appWindow), apptw->appVBox1);

    /* Menu bar */
    apptw->appMenubar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (apptw->appVBox1),
                        apptw->appMenubar,
                        FALSE,
                        FALSE,
                        0);

    /* File menu stuff */
    apptw->appFile_menu = xfcalendar_menu_new(_("_File"), apptw->appMenubar);

    apptw->appFileSave_menuitem = xfcalendar_image_menu_item_new_from_stock ("gtk-save", apptw->appFile_menu, apptw->appAccelgroup);

    apptw->appFileSaveClose_menuitem = xfcalendar_menu_item_new_with_mnemonic (_("Sav_e and close"), apptw->appFile_menu);
    gtk_widget_add_accelerator(apptw->appFileSaveClose_menuitem, "activate", apptw->appAccelgroup, GDK_w, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);

    menu_separator = xfcalendar_separator_menu_item_new (apptw->appFile_menu);

    apptw->appFileRevert_menuitem = xfcalendar_image_menu_item_new_from_stock ("gtk-revert-to-saved", apptw->appFile_menu, apptw->appAccelgroup);

    apptw->appFileDuplicate_menuitem = xfcalendar_menu_item_new_with_mnemonic (_("D_uplicate"), apptw->appFile_menu);
    gtk_widget_add_accelerator(apptw->appFileDuplicate_menuitem, "activate", apptw->appAccelgroup, GDK_d, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    menu_separator = xfcalendar_separator_menu_item_new(apptw->appFile_menu);

    apptw->appFileDelete_menuitem = xfcalendar_image_menu_item_new_from_stock ("gtk-delete", apptw->appFile_menu, apptw->appAccelgroup);

    menu_separator = xfcalendar_separator_menu_item_new(apptw->appFile_menu);

    apptw->appFileClose_menuitem = xfcalendar_image_menu_item_new_from_stock ("gtk-close", apptw->appFile_menu, apptw->appAccelgroup);

    /* Handle box and toolbar */
    apptw->appHandleBox = gtk_handle_box_new();
    gtk_box_pack_start (GTK_BOX (apptw->appVBox1), apptw->appHandleBox, FALSE, FALSE, 0);

    apptw->appToolbar = gtk_toolbar_new();
    gtk_container_add (GTK_CONTAINER (apptw->appHandleBox), apptw->appToolbar);
    gtk_toolbar_set_tooltips (GTK_TOOLBAR(apptw->appToolbar), TRUE);

    /* Add buttons to the toolbar */
    apptw->appSave = xfcalendar_toolbar_append_button (apptw->appToolbar, "gtk-save", apptw->appTooltips, _("Save"), i++);

    apptw->appSaveClose = xfcalendar_toolbar_append_button (apptw->appToolbar, "gtk-close", apptw->appTooltips, _("Save and close"), i++);
    gtk_tool_button_set_label (GTK_TOOL_BUTTON (apptw->appSaveClose), _("Save and close"));
    gtk_tool_item_set_is_important (GTK_TOOL_ITEM(apptw->appSaveClose), TRUE);

    toolbar_separator = xfcalendar_toolbar_append_separator (apptw->appToolbar, i++);

    apptw->appRevert = xfcalendar_toolbar_append_button (apptw->appToolbar, "gtk-revert-to-saved", apptw->appTooltips, _("Revert"), i++);

    apptw->appDuplicate = xfcalendar_toolbar_append_button (apptw->appToolbar, "gtk-copy", apptw->appTooltips, _("Duplicate"), i++);
    gtk_tool_button_set_label (GTK_TOOL_BUTTON (apptw->appDuplicate), _("Duplicate"));

    toolbar_separator = xfcalendar_toolbar_append_separator (apptw->appToolbar, i++);

    apptw->appDelete = xfcalendar_toolbar_append_button (apptw->appToolbar, "gtk-delete", apptw->appTooltips, _("Delete"), i++);


    gtk_widget_set_sensitive(apptw->appRevert, FALSE);

    apptw->appNotebook = gtk_notebook_new();
    gtk_container_add (GTK_CONTAINER (apptw->appVBox1), apptw->appNotebook);
    gtk_container_set_border_width(GTK_CONTAINER (apptw->appNotebook), 5);

    /* Here begins the General tab */
    apptw->appGeneral_notebook_page = xfce_framebox_new(NULL, FALSE);
    apptw->appGeneral_tab_label = gtk_label_new(_("General"));

    gtk_notebook_append_page(GTK_NOTEBOOK(apptw->appNotebook)
                            , apptw->appGeneral_notebook_page
                            , apptw->appGeneral_tab_label);

    apptw->appTableGeneral = xfcalendar_table_new (8, 2);
    xfce_framebox_add(XFCE_FRAMEBOX(apptw->appGeneral_notebook_page), apptw->appTableGeneral);

    apptw->appTitle_label = gtk_label_new (_("Title "));
    apptw->appTitle_entry = gtk_entry_new ();
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appTitle_label, apptw->appTitle_entry, 0,
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                   (GtkAttachOptions) (0));

    apptw->appLocation_label = gtk_label_new (_("Location"));
    apptw->appLocation_entry = gtk_entry_new ();
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appLocation_label, apptw->appLocation_entry, 1,
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                   (GtkAttachOptions) (0));

    apptw->appAllDay_checkbutton = gtk_check_button_new_with_mnemonic (_("All day event"));
    xfcalendar_table_add_row (apptw->appTableGeneral, NULL, apptw->appAllDay_checkbutton, 2,
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                   (GtkAttachOptions) (0));

    apptw->appStart = gtk_label_new (_("Start"));
    apptw->appStartDate_button = gtk_button_new();
    apptw->appStartTime_comboboxentry = gtk_combo_box_entry_new_text();
    apptw->appStartTime_hbox = xfcalendar_datetime_hbox_new (apptw->appStartDate_button, apptw->appStartTime_comboboxentry);
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appStart, apptw->appStartTime_hbox, 3,
                      (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                      (GtkAttachOptions) (GTK_SHRINK | GTK_FILL));

    apptw->appEnd = gtk_label_new (_("End"));
    apptw->appEndDate_button = gtk_button_new();
    apptw->appEndTime_comboboxentry = gtk_combo_box_entry_new_text();
    apptw->appEndTime_hbox = xfcalendar_datetime_hbox_new (apptw->appEndDate_button, apptw->appEndTime_comboboxentry);
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appEnd, apptw->appEndTime_hbox, 4,
                      (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                      (GtkAttachOptions) (GTK_SHRINK | GTK_FILL));

    apptw->appRecurrency = gtk_label_new (_("Recurrence"));
    apptw->appRecurrency_cb = gtk_combo_box_new_text ();
    xfcalendar_combo_box_append_array(apptw->appRecurrency_cb, recurrency_array, RECURRENCY_ARRAY_DIM);
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appRecurrency, apptw->appRecurrency_cb, 5,
                   (GtkAttachOptions) (GTK_FILL),
                   (GtkAttachOptions) (GTK_FILL));

    apptw->appAvailability = gtk_label_new (_("Availability"));
    apptw->appAvailability_cb = gtk_combo_box_new_text ();
    xfcalendar_combo_box_append_array(apptw->appAvailability_cb, availability_array, AVAILABILITY_ARRAY_DIM);
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appAvailability, apptw->appAvailability_cb, 6,
                   (GtkAttachOptions) (GTK_FILL),
                   (GtkAttachOptions) (GTK_FILL));

    apptw->appNote = gtk_label_new (_("Note"));
    apptw->appNote_Scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (apptw->appNote_Scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (apptw->appNote_Scrolledwindow), GTK_SHADOW_IN);

    apptw->appNote_buffer = gtk_text_buffer_new(NULL);
    apptw->appNote_textview = gtk_text_view_new_with_buffer(GTK_TEXT_BUFFER(apptw->appNote_buffer));
/*
    apptw->appNote_textview = gtk_text_view_new ();
*/
    gtk_container_add (GTK_CONTAINER (apptw->appNote_Scrolledwindow), apptw->appNote_textview);
    xfcalendar_table_add_row (apptw->appTableGeneral, apptw->appNote, apptw->appNote_Scrolledwindow, 7,
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL));

    /*
    apptw->appPrivate = gtk_label_new (_("Private"));
    gtk_widget_show (apptw->appPrivate);
    gtk_table_attach (GTK_TABLE (apptw->appTableGeneral), apptw->appPrivate, 0, 1, 5, 6,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (apptw->appPrivate), 0, 0.5);

    apptw->appPrivate_check = gtk_check_button_new_with_mnemonic ("");
    gtk_widget_show (apptw->appPrivate_check);
    gtk_table_attach (GTK_TABLE (apptw->appTableGeneral), apptw->appPrivate_check, 1, 2, 5, 6,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    */

    /* Here begins the Alarm tab */
    apptw->appAlarm_notebook_page = xfce_framebox_new(NULL, FALSE);
    apptw->appAlarm_tab_label = gtk_label_new(_("Alarm"));

    gtk_notebook_append_page(GTK_NOTEBOOK(apptw->appNotebook)
                            , apptw->appAlarm_notebook_page
                            , apptw->appAlarm_tab_label);

    apptw->appTableAlarm = xfcalendar_table_new (3, 2);
    xfce_framebox_add(XFCE_FRAMEBOX(apptw->appAlarm_notebook_page), apptw->appTableAlarm);

    apptw->appAlarm = gtk_label_new (_("Alarm"));
    apptw->appAlarm_combobox = gtk_combo_box_new_text ();
    xfcalendar_combo_box_append_array(apptw->appAlarm_combobox, alarm_array, ALARM_ARRAY_DIM);
    xfcalendar_table_add_row (apptw->appTableAlarm, apptw->appAlarm, apptw->appAlarm_combobox, 0,
                   (GtkAttachOptions) (GTK_FILL),
                   (GtkAttachOptions) (GTK_FILL));

    apptw->appSound_label = gtk_label_new (_("Sound"));
    apptw->appSound_hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_set_spacing(GTK_BOX (apptw->appSound_hbox), 6);
    apptw->appSound_entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (apptw->appSound_hbox), apptw->appSound_entry, TRUE, TRUE, 0);
    apptw->appSound_button = gtk_button_new_from_stock("gtk-find");
    gtk_box_pack_start (GTK_BOX (apptw->appSound_hbox), apptw->appSound_button, FALSE, TRUE, 0);
    xfcalendar_table_add_row (apptw->appTableAlarm, apptw->appSound_label, apptw->appSound_hbox, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL));

    apptw->appSoundRepeat_checkbutton = gtk_check_button_new_with_mnemonic (_("Repeat alarm sound"));
    xfcalendar_table_add_row (apptw->appTableAlarm, NULL, apptw->appSoundRepeat_checkbutton, 2,
                   (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                   (GtkAttachOptions) (0));
    /* */

    g_signal_connect((gpointer) apptw->appFileSave_menuitem, "activate",
            G_CALLBACK (on_appFileSave_menu_activate_cb),
            apptw);

    g_signal_connect((gpointer) apptw->appFileSaveClose_menuitem, "activate",
            G_CALLBACK (on_appFileSaveClose_menu_activate_cb),
            apptw);

    g_signal_connect((gpointer) apptw->appFileDuplicate_menuitem, "activate",
            G_CALLBACK (on_appFileDuplicate_menu_activate_cb),
            apptw);

    g_signal_connect((gpointer) apptw->appFileRevert_menuitem, "activate",
            G_CALLBACK (on_appFileRevert_menu_activate_cb),
            apptw);

    g_signal_connect((gpointer) apptw->appFileDelete_menuitem, "activate",
            G_CALLBACK (on_appFileDelete_menu_activate_cb),
            apptw);

    g_signal_connect((gpointer) apptw->appFileClose_menuitem, "activate",
            G_CALLBACK (on_appFileClose_menu_activate_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appAllDay_checkbutton, "clicked",
            G_CALLBACK (on_appAllDay_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appSound_button, "clicked",
            G_CALLBACK (on_appSound_button_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appSave, "clicked",
            G_CALLBACK (on_appSave_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appSaveClose, "clicked",
            G_CALLBACK (on_appSaveClose_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appDelete, "clicked",
            G_CALLBACK (on_appDelete_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appDuplicate, "clicked",
            G_CALLBACK (on_appDuplicate_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appRevert, "clicked",
            G_CALLBACK (on_appRevert_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appStartDate_button, "clicked",
            G_CALLBACK (on_appStartEndDate_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appEndDate_button, "clicked",
            G_CALLBACK (on_appStartEndDate_clicked_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appWindow, "delete-event",
            G_CALLBACK (on_appWindow_delete_event_cb),
            apptw);
    /* Take care of the title entry to build the appointment window title 
     * Beware: we are not using apptw->appTitle_entry as a GtkEntry here 
     * but as an interface GtkEditable instead.
     */
    g_signal_connect_after ((gpointer) apptw->appTitle_entry, "changed",
            G_CALLBACK (on_appTitle_entry_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appLocation_entry, "changed",
            G_CALLBACK (on_appLocation_entry_changed_cb),
            apptw);

    g_signal_connect ((gpointer) apptw->appSoundRepeat_checkbutton, "clicked",
            G_CALLBACK (appSoundRepeat_checkbutton_clicked_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appStartTime_comboboxentry, "changed",
            G_CALLBACK (on_appTime_comboboxentry_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appEndTime_comboboxentry, "changed",
            G_CALLBACK (on_appTime_comboboxentry_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appRecurrency_cb, "changed",
            G_CALLBACK (on_app_combobox_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appAvailability_cb, "changed",
            G_CALLBACK (on_app_combobox_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appAlarm_combobox, "changed",
            G_CALLBACK (on_app_combobox_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appNote_buffer, "changed",
            G_CALLBACK (on_appNote_buffer_changed_cb),
            apptw);

    g_signal_connect_after ((gpointer) apptw->appSound_entry, "changed",
            G_CALLBACK (on_appSound_entry_changed_cb),
            apptw);

    fill_appt_window(apptw, action, par);
    apptw->appointment_changed = FALSE;
    gtk_widget_show_all(apptw->appWindow);

    return apptw;
}
