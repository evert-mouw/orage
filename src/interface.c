/*
 * YOU CAN EDIT THIS FILE - it *was* generated by Glade,
 * but now it's generated by hand.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfcegui4/netk-trayicon.h>
#include <libxfcegui4/libxfcegui4.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "xfcalendar-icon-inline.h"
#include "xfce_trayicon.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_XFCalendar (void)
{
  GtkWidget *XFCalendar;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *menuitem4;
  GtkWidget *menuitem4_menu;
  GtkWidget *close1;
  GtkWidget *separator1;
  GtkWidget *separator2;
  GtkWidget *quit1;
  GtkWidget *menuitem7;
  GtkWidget *menuitem7_menu;
  /* */
  GtkWidget *menuitemSet;
  GtkWidget *menuitemSet_menu;
  GtkWidget *preferences;
  GtkWidget *selectToday;
  /* */
  GtkWidget *about1;
  GtkWidget *calendar1;
  GtkAccelGroup *accel_group;
  GdkPixbuf *xfcalendar_logo;

  xfcalendar_logo = inline_icon_at_size(xfcalendar_icon, 48, 48);
  accel_group = gtk_accel_group_new ();

  XFCalendar = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (XFCalendar), _("XFCalendar"));
  gtk_window_set_position (GTK_WINDOW (XFCalendar), GTK_WIN_POS_NONE);
  gtk_window_set_resizable (GTK_WINDOW (XFCalendar), FALSE);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (XFCalendar), TRUE);
  gtk_window_set_icon(GTK_WINDOW (XFCalendar), xfcalendar_logo);
  g_object_unref(xfcalendar_logo);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (XFCalendar), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  menuitem4 = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (menuitem4);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

  menuitem4_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menuitem4_menu);

  close1 = gtk_menu_item_new_with_mnemonic(_("Close window"));
  gtk_widget_show(close1);
  gtk_container_add(GTK_CONTAINER(menuitem4_menu), close1);

  separator1 = gtk_separator_menu_item_new();
  gtk_widget_show(separator1);
  gtk_container_add (GTK_CONTAINER (menuitem4_menu), separator1);

  quit1 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (quit1);
  gtk_container_add (GTK_CONTAINER (menuitem4_menu), quit1);

  /* */
  menuitemSet = gtk_menu_item_new_with_mnemonic(_("Settings"));
  gtk_widget_show(menuitemSet);
  gtk_container_add(GTK_CONTAINER (menubar1), menuitemSet);

  menuitemSet_menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitemSet), menuitemSet_menu);

  preferences = gtk_menu_item_new_with_mnemonic(_("Preferences"));
  gtk_widget_show(preferences);
  gtk_container_add(GTK_CONTAINER(menuitemSet_menu), preferences);
   
  separator2 = gtk_separator_menu_item_new();
  gtk_widget_show(separator2);
  gtk_container_add (GTK_CONTAINER (menuitemSet_menu), separator2);
  
  selectToday = gtk_menu_item_new_with_mnemonic(_("Select Today"));
  gtk_widget_show(selectToday);
  gtk_container_add(GTK_CONTAINER(menuitemSet_menu), selectToday);
  
  /* */
  menuitem7 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem7);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem7);

  menuitem7_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem7), menuitem7_menu);

  about1 = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (menuitem7_menu), about1);

  calendar1 = gtk_calendar_new ();
  gtk_widget_show (calendar1);
  gtk_box_pack_start (GTK_BOX (vbox1), calendar1, TRUE, TRUE, 0);
  gtk_calendar_display_options (GTK_CALENDAR (calendar1),
                                GTK_CALENDAR_SHOW_HEADING
                                | GTK_CALENDAR_SHOW_DAY_NAMES
                                | GTK_CALENDAR_SHOW_WEEK_NUMBERS);

  g_signal_connect ((gpointer) XFCalendar, "delete_event",
                    G_CALLBACK (on_XFCalendar_delete_event),
                    GTK_WIDGET(XFCalendar));
  g_signal_connect ((gpointer) close1, "activate",
                    G_CALLBACK (on_close1_activate),
                    GTK_WIDGET(XFCalendar));
  g_signal_connect ((gpointer) quit1, "activate",
                    G_CALLBACK (on_quit1_activate),
                    GTK_WIDGET(XFCalendar));
  g_signal_connect ((gpointer) about1, "activate",
                    G_CALLBACK (on_about1_activate),
                    NULL);
  g_signal_connect((gpointer) preferences, "activate",
		   G_CALLBACK(on_preferences_activate),
		   NULL);
  g_signal_connect((gpointer) selectToday, "activate",
		   G_CALLBACK(on_selectToday_activate),
		   NULL);
  g_signal_connect((gpointer) calendar1, "scroll_event",
		   G_CALLBACK (on_calendar1_scroll),
		   NULL);
  g_signal_connect ((gpointer) calendar1, "day_selected_double_click",
                    G_CALLBACK (on_calendar1_day_selected_double_click),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (XFCalendar, XFCalendar, "XFCalendar");
  GLADE_HOOKUP_OBJECT (XFCalendar, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (XFCalendar, menubar1, "menubar1");
  GLADE_HOOKUP_OBJECT (XFCalendar, menuitem4, "menuitem4");
  GLADE_HOOKUP_OBJECT (XFCalendar, menuitem4_menu, "menuitem4_menu");
  GLADE_HOOKUP_OBJECT (XFCalendar, close1, "close1");
  GLADE_HOOKUP_OBJECT (XFCalendar, quit1, "quit1");
  /* */
  GLADE_HOOKUP_OBJECT(XFCalendar, menuitemSet, "menuitemSet");
  GLADE_HOOKUP_OBJECT(XFCalendar, menuitemSet_menu, "menuitemSet_menu");
  GLADE_HOOKUP_OBJECT(XFCalendar, preferences, "preferences");
  GLADE_HOOKUP_OBJECT(XFCalendar, selectToday, "selectToday");
  /* */
  GLADE_HOOKUP_OBJECT (XFCalendar, menuitem7, "menuitem7");
  GLADE_HOOKUP_OBJECT (XFCalendar, menuitem7_menu, "menuitem7_menu");
  GLADE_HOOKUP_OBJECT (XFCalendar, about1, "about1");
  GLADE_HOOKUP_OBJECT (XFCalendar, calendar1, "calendar1");

  gtk_window_add_accel_group (GTK_WINDOW (XFCalendar), accel_group);

  return XFCalendar;
}

GtkWidget*
create_wAppointment (void)
{
  GtkWidget *wAppointment;
  GtkWidget *vbox2;
  GtkWidget *handlebox1;
  GtkWidget *toolbar1;
  GtkWidget *tmp_toolbar_icon;
  GtkWidget *btSave;
  GtkWidget *btClose;
  GtkWidget *btDelete;
  GtkWidget *btPrevious;
  GtkWidget *btToday;
  GtkWidget *btNext;
  GtkWidget *scrolledwindow1;
  GtkWidget *textview1;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  wAppointment = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (wAppointment, 300, 250);
  gtk_window_set_title (GTK_WINDOW (wAppointment), _("Appointment"));
  gtk_window_set_position (GTK_WINDOW (wAppointment), GTK_WIN_POS_NONE);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (wAppointment), TRUE);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (wAppointment), vbox2);

  handlebox1 = gtk_handle_box_new ();
  gtk_widget_show (handlebox1);
  gtk_box_pack_start (GTK_BOX (vbox2), handlebox1, FALSE, FALSE, 0);

  toolbar1 = gtk_toolbar_new ();
  gtk_widget_show (toolbar1);
  gtk_container_add (GTK_CONTAINER (handlebox1), toolbar1);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_ICONS);

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-save", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btSave = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "btSave",
                                _("Save (Ctrl+s)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btSave);
  gtk_widget_add_accelerator(btSave, "clicked", accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  gtk_toolbar_append_space(GTK_TOOLBAR (toolbar1));

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-go-back", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btPrevious = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "btPrevious",
                                _("Previous day (Ctrl+p)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btPrevious);
  gtk_widget_add_accelerator(btPrevious, "clicked", accel_group, GDK_p, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-home", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btToday = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "btToday",
                                _("Today (Alt+Home)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btToday);
  gtk_widget_add_accelerator(btToday, "clicked", accel_group, GDK_Home, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator(btToday, "clicked", accel_group, GDK_h, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-go-forward", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btNext = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "btNext",
                                _("Next day (Ctrl+n)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btNext);
  gtk_widget_add_accelerator(btNext, "clicked", accel_group, GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  gtk_toolbar_append_space(GTK_TOOLBAR (toolbar1));

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-close", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btClose = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "btClose",
                                _("Close (Ctrl+w)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btClose);
  gtk_widget_add_accelerator(btClose, "clicked", accel_group, GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  tmp_toolbar_icon = gtk_image_new_from_stock ("gtk-clear", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1)));
  btDelete = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Clear"),
                                _("Clear (Ctrl+l)"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (toolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (btDelete);
  gtk_widget_add_accelerator(btDelete, "clicked", accel_group, GDK_l, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  textview1 = gtk_text_view_new ();
  gtk_widget_show (textview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), textview1);

  g_signal_connect ((gpointer) wAppointment, "delete_event",
                    G_CALLBACK (on_wAppointment_delete_event),
                    NULL);
  g_signal_connect ((gpointer) btSave, "clicked",
                    G_CALLBACK (on_btSave_clicked),
                    NULL);
  g_signal_connect ((gpointer) btPrevious, "clicked",
                    G_CALLBACK (on_btPrevious_clicked),
                    NULL);
  g_signal_connect ((gpointer) btToday, "clicked",
                    G_CALLBACK (on_btToday_clicked),
                    NULL);
  g_signal_connect ((gpointer) btNext, "clicked",
                    G_CALLBACK (on_btNext_clicked),
                    NULL);
  g_signal_connect ((gpointer) btClose, "clicked",
                    G_CALLBACK (on_btClose_clicked),
                    NULL);
  g_signal_connect ((gpointer) btDelete, "clicked",
                    G_CALLBACK (on_btDelete_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (wAppointment, wAppointment, "wAppointment");
  GLADE_HOOKUP_OBJECT (wAppointment, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (wAppointment, handlebox1, "handlebox1");
  GLADE_HOOKUP_OBJECT (wAppointment, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (wAppointment, btSave, "btSave");
  GLADE_HOOKUP_OBJECT (wAppointment, btPrevious, "btPrevious");
  GLADE_HOOKUP_OBJECT (wAppointment, btToday, "btToday");
  GLADE_HOOKUP_OBJECT (wAppointment, btNext, "btNext");
  GLADE_HOOKUP_OBJECT (wAppointment, btClose, "btClose");
  GLADE_HOOKUP_OBJECT (wAppointment, btDelete, "btDelete");
  GLADE_HOOKUP_OBJECT (wAppointment, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (wAppointment, textview1, "textview1");

  gtk_window_add_accel_group (GTK_WINDOW (wAppointment), accel_group);

  return wAppointment;
}

GtkWidget*
create_wClearWarn (void)
{
  GtkWidget *wClearWarn;
  GtkWidget *dialog_vbox2;
  GtkWidget *hbox1;
  GtkWidget *image1;
  GtkWidget *lbClearWarn;
  GtkWidget *dialog_action_area2;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton2;

  wClearWarn = gtk_dialog_new ();
  gtk_widget_set_size_request (wClearWarn, 250, 120);
  gtk_window_set_title (GTK_WINDOW (wClearWarn), _("Warning"));
  gtk_window_set_position (GTK_WINDOW (wClearWarn), GTK_WIN_POS_CENTER);
  gtk_window_set_modal (GTK_WINDOW (wClearWarn), TRUE);
  gtk_window_set_resizable (GTK_WINDOW (wClearWarn), FALSE);

  dialog_vbox2 = GTK_DIALOG (wClearWarn)->vbox;
  gtk_widget_show (dialog_vbox2);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), hbox1, TRUE, TRUE, 0);

  image1 = gtk_image_new_from_stock ("gtk-dialog-warning", GTK_ICON_SIZE_DIALOG);
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (hbox1), image1, TRUE, TRUE, 0);

  lbClearWarn = gtk_label_new (_("You will remove all information \nassociated with this date."));
  gtk_widget_show (lbClearWarn);
  gtk_box_pack_start (GTK_BOX (hbox1), lbClearWarn, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (lbClearWarn), GTK_JUSTIFY_LEFT);

  dialog_action_area2 = GTK_DIALOG (wClearWarn)->action_area;
  gtk_widget_show (dialog_action_area2);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area2), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (wClearWarn), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton2 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton2);
  gtk_dialog_add_action_widget (GTK_DIALOG (wClearWarn), okbutton2, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton2, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) cancelbutton1, "clicked",
                    G_CALLBACK (on_cancelbutton1_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (wClearWarn, wClearWarn, "wClearWarn");
  GLADE_HOOKUP_OBJECT_NO_REF (wClearWarn, dialog_vbox2, "dialog_vbox2");
  GLADE_HOOKUP_OBJECT (wClearWarn, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (wClearWarn, image1, "image1");
  GLADE_HOOKUP_OBJECT (wClearWarn, lbClearWarn, "lbClearWarn");
  GLADE_HOOKUP_OBJECT_NO_REF (wClearWarn, dialog_action_area2, "dialog_action_area2");
  GLADE_HOOKUP_OBJECT (wClearWarn, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (wClearWarn, okbutton2, "okbutton2");

  return wClearWarn;
}

XfceTrayIcon*
create_TrayIcon (GtkWidget *window)
{
  XfceTrayIcon *trayIcon = NULL;
  //GtkWidget *trayIcon = NULL;
  GtkWidget *menuItem;
  GtkWidget *trayMenu;
  GdkPixbuf *pixbuf;

  /*
   * Create the tray icon popup menu
   */
  trayMenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_mnemonic(_("Today"));
  gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menuItem), gtk_image_new_from_stock(GTK_STOCK_HOME, GTK_ICON_SIZE_MENU));
  g_signal_connect(menuItem, "activate", G_CALLBACK(on_Today_activate),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show_all(menuItem);
  menuItem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);
  
  menuItem = gtk_menu_item_new_with_label(_("Preferences"));
  g_signal_connect(menuItem, "activate", G_CALLBACK(on_preferences_activate),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);
  menuItem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);

  menuItem = gtk_menu_item_new_with_label(_("About XFCalendar"));
  g_signal_connect(menuItem, "activate", G_CALLBACK(on_about1_activate),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);
  menuItem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);
  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
  g_signal_connect(menuItem, "activate", G_CALLBACK(gtk_main_quit), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(trayMenu), menuItem);
  gtk_widget_show(menuItem);

  /*
   * Create the tray icon
   */
  pixbuf = inline_icon_at_size(xfcalendar_icon, 16, 16);
  trayIcon = xfce_tray_icon_new_with_menu_from_pixbuf(trayMenu, pixbuf);
  g_object_unref(pixbuf);
  g_signal_connect_swapped(G_OBJECT(trayIcon), "clicked",
			   G_CALLBACK(toggle_visible_cb), window);
  return trayIcon;
}
