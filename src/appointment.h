/* appointment.h
 *
 * Copyright (C) 2004 Micka�l Graf <korbinus@lunar-linux.org>
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


typedef struct
{
  gchar *title,
    *location;

  gint alarm,
    alarmTimeType,
    availability;

  gchar *note;
  gchar *uid;

        /* time format must be:
         * yyyymmdd[Thhmiss[Z]] = %04d%02d%02dT%02d%02d%02d
         * T means it has also time part
         * Z means it is in UTC format
         */
  gchar 
    starttime[17],
    endtime[17];

} appt_type;

#define XF_APP_TIME_FORMAT "%04d%02d%02dT%02d%02d%02d"
#define XF_APP_DATE_FORMAT "%04d%02d%02d"

/* typedef struct _appointment appointment; */


typedef struct
{
  GtkWidget *appWindow;
  GtkWidget *appVBox1;
  GtkWidget *appTable;
  GtkWidget *appTitle_label;
  GtkWidget *appLocation_label;
  GtkWidget *appStart;
  GtkWidget *appEnd;
  GtkWidget *appPrivate;
  GtkWidget *appAlarm;
  GtkWidget *appRecurrence;
  GtkWidget *appNote;
  GtkWidget *appAvailability;
  GtkWidget *appTitle_entry;
  GtkWidget *appLocation_entry;
  GtkWidget *appPrivate_check;
  GtkWidget *appNote_Scrolledwindow;
  GtkWidget *appNote_textview;
  GtkWidget *appAvailability_cb;
  GtkWidget *appAlarm_hbox;
  GtkObject *appAlarm_spinbutton_adj;
  GtkWidget *appAlarm_spinbutton;
  GtkWidget *appAlarm_fixed_1;
  GtkWidget *appAlarmTimeType_combobox;
  GtkWidget *appAlarm_fixed_2;
  GtkWidget *appStartTime_hbox;
  GtkObject *appStartHour_spinbutton_adj;
  GtkWidget *appStartHour_spinbutton;
  GtkWidget *appStartColumn_label;
  GtkObject *appStartMinutes_spinbutton_adj;
  GtkWidget *appStartMinutes_spinbutton;
  GtkWidget *appStartTime_fixed;
  GtkWidget *appEndTime_hbox;
  GtkObject *appEndHour_spinbutton_adj;
  GtkWidget *appEndHour_spinbutton;
  GtkWidget *appEndColumn_label;
  GtkObject *appEndMinutes_spinbutton_adj;
  GtkWidget *appEndMinutes_spinbutton;
  GtkWidget *appEndTime_fixed;
  GtkWidget *appHBox1;
  GtkWidget *appRemove;
  GtkWidget *appBottom_fixed;
  GtkWidget *appClose;

  appt_type *appt_data;
} appt_win;

void fill_appt_window(appt_win *appt, char *action, char *par);

appt_win *create_appt_win(char *action, char *par);



