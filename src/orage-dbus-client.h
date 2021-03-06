/* Generated by dbus-binding-tool; do not edit! */

#include <glib/gtypes.h>
#include <glib/gerror.h>
#include <dbus/dbus-glib.h>

G_BEGIN_DECLS

#ifndef DBUS_GLIB_CLIENT_WRAPPERS_org_xfce_calendar
#define DBUS_GLIB_CLIENT_WRAPPERS_org_xfce_calendar

static
#ifdef G_HAVE_INLINE
inline
#endif
gboolean
org_xfce_calendar_load_file (DBusGProxy *proxy, const char * IN_file, GError **error)

{
  return dbus_g_proxy_call (proxy, "LoadFile", error, G_TYPE_STRING, IN_file, G_TYPE_INVALID, G_TYPE_INVALID);
}

typedef void (*org_xfce_calendar_load_file_reply) (DBusGProxy *proxy, GError *error, gpointer userdata);

static void
org_xfce_calendar_load_file_async_callback (DBusGProxy *proxy, DBusGProxyCall *call, void *user_data)
{
  DBusGAsyncData *data = user_data;
  GError *error = NULL;
  dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
  (*(org_xfce_calendar_load_file_reply)data->cb) (proxy, error, data->userdata);
  return;
}

static
#ifdef G_HAVE_INLINE
inline
#endif
DBusGProxyCall*
org_xfce_calendar_load_file_async (DBusGProxy *proxy, const char * IN_file, org_xfce_calendar_load_file_reply callback, gpointer userdata)

{
  DBusGAsyncData *stuff;
  stuff = g_new (DBusGAsyncData, 1);
  stuff->cb = G_CALLBACK (callback);
  stuff->userdata = userdata;
  return dbus_g_proxy_begin_call (proxy, "LoadFile", org_xfce_calendar_load_file_async_callback, stuff, g_free, G_TYPE_STRING, IN_file, G_TYPE_INVALID);
}


static
#ifdef G_HAVE_INLINE
inline
#endif
gboolean
org_xfce_calendar_export_file (DBusGProxy *proxy, const char * IN_file, const gint IN_type, const char * IN_uids, GError **error)

{
  return dbus_g_proxy_call (proxy, "ExportFile", error, G_TYPE_STRING, IN_file, G_TYPE_INT, IN_type, G_TYPE_STRING, IN_uids, G_TYPE_INVALID, G_TYPE_INVALID);
}

typedef void (*org_xfce_calendar_export_file_reply) (DBusGProxy *proxy, GError *error, gpointer userdata);

static void
org_xfce_calendar_export_file_async_callback (DBusGProxy *proxy, DBusGProxyCall *call, void *user_data)
{
  DBusGAsyncData *data = user_data;
  GError *error = NULL;
  dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
  (*(org_xfce_calendar_export_file_reply)data->cb) (proxy, error, data->userdata);
  return;
}

static
#ifdef G_HAVE_INLINE
inline
#endif
DBusGProxyCall*
org_xfce_calendar_export_file_async (DBusGProxy *proxy, const char * IN_file, const int IN_type, const char * IN_uids, org_xfce_calendar_export_file_reply callback, gpointer userdata)

{
  DBusGAsyncData *stuff;
  stuff = g_new (DBusGAsyncData, 1);
  stuff->cb = G_CALLBACK (callback);
  stuff->userdata = userdata;
  return dbus_g_proxy_begin_call (proxy, "ExportFile", org_xfce_calendar_export_file_async_callback, stuff, g_free, G_TYPE_STRING, IN_file, G_TYPE_INT, IN_type,G_TYPE_STRING, IN_uids, G_TYPE_INVALID);
}


static
#ifdef G_HAVE_INLINE
inline
#endif
gboolean
org_xfce_calendar_add_foreign (DBusGProxy *proxy, const char * IN_file, const gboolean IN_mode, const char * IN_name, GError **error)

{
  return dbus_g_proxy_call (proxy, "AddForeign", error, G_TYPE_STRING, IN_file, G_TYPE_BOOLEAN, IN_mode, G_TYPE_STRING, IN_name, G_TYPE_INVALID, G_TYPE_INVALID);
}

typedef void (*org_xfce_calendar_add_foreign_reply) (DBusGProxy *proxy, GError *error, gpointer userdata);

static void
org_xfce_calendar_add_foreign_async_callback (DBusGProxy *proxy, DBusGProxyCall *call, void *user_data)
{
  DBusGAsyncData *data = user_data;
  GError *error = NULL;
  dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
  (*(org_xfce_calendar_add_foreign_reply)data->cb) (proxy, error, data->userdata);
  return;
}

static
#ifdef G_HAVE_INLINE
inline
#endif
DBusGProxyCall*
org_xfce_calendar_add_foreign_async (DBusGProxy *proxy, const char * IN_file, const gboolean IN_mode, const char * IN_name, org_xfce_calendar_add_foreign_reply callback, gpointer userdata)

{
  DBusGAsyncData *stuff;
  stuff = g_new (DBusGAsyncData, 1);
  stuff->cb = G_CALLBACK (callback);
  stuff->userdata = userdata;
  return dbus_g_proxy_begin_call (proxy, "AddForeign", org_xfce_calendar_add_foreign_async_callback, stuff, g_free, G_TYPE_STRING, IN_file, G_TYPE_BOOLEAN, IN_mode, G_TYPE_STRING, IN_file, G_TYPE_INVALID);
}


static
#ifdef G_HAVE_INLINE
inline
#endif
gboolean
org_xfce_calendar_remove_foreign (DBusGProxy *proxy, const char * IN_file, GError **error)

{
  return dbus_g_proxy_call (proxy, "RemoveForeign", error, G_TYPE_STRING, IN_file, G_TYPE_INVALID, G_TYPE_INVALID);
}

typedef void (*org_xfce_calendar_remove_foreign_reply) (DBusGProxy *proxy, GError *error, gpointer userdata);

static void
org_xfce_calendar_remove_foreign_async_callback (DBusGProxy *proxy, DBusGProxyCall *call, void *user_data)
{
  DBusGAsyncData *data = user_data;
  GError *error = NULL;
  dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
  (*(org_xfce_calendar_remove_foreign_reply)data->cb) (proxy, error, data->userdata);
  return;
}

static
#ifdef G_HAVE_INLINE
inline
#endif
DBusGProxyCall*
org_xfce_calendar_remove_foreign_async (DBusGProxy *proxy, const char * IN_file, org_xfce_calendar_remove_foreign_reply callback, gpointer userdata)

{
  DBusGAsyncData *stuff;
  stuff = g_new (DBusGAsyncData, 1);
  stuff->cb = G_CALLBACK (callback);
  stuff->userdata = userdata;
  return dbus_g_proxy_begin_call (proxy, "RemoveForeign", org_xfce_calendar_remove_foreign_async_callback, stuff, g_free, G_TYPE_STRING, IN_file, G_TYPE_INVALID);
}
#endif /* defined DBUS_GLIB_CLIENT_WRAPPERS_org_xfce_calendar */

G_END_DECLS
