/*
 *  Copyright (C) 2015-2017 Gooroom <gooroom@gooroom.kr>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include <gtk/gtk.h>

#include <panel-applet.h>

#include "indicator-button.h"
#include "gooroom-indicator-applet.h"

#include <libayatana-ido/libayatana-ido.h>
#include <libayatana-indicator/indicator-ng.h>
#include <libayatana-indicator/indicator-object.h>



struct _GooroomIndicatorAppletPrivate
{
	GtkWidget *buttonbox;
};


G_DEFINE_TYPE_WITH_PRIVATE (GooroomIndicatorApplet, gooroom_indicator_applet, PANEL_TYPE_APPLET)



static gboolean
contain_blacklist (const gchar *module_name)
{
	gboolean ret = FALSE;
	GSettingsSchema *schema;
	GSettingsSchemaSource *schema_source;
	const gchar *schema_id = "apps.gooroom-indicator-applet";

	g_return_val_if_fail (module_name != NULL, FALSE);

	schema_source = g_settings_schema_source_get_default ();
	schema = g_settings_schema_source_lookup (schema_source, schema_id, FALSE);
	if (g_settings_schema_has_key (schema, "blacklist")) {
		GSettings *gsettings = g_settings_new (schema_id);
		gchar **blacklist = g_settings_get_strv (gsettings, "blacklist");

		if (g_strv_contains ((const gchar * const *) blacklist, module_name))
			ret = TRUE;

		g_strfreev (blacklist);
		g_object_unref (gsettings);
	}

	g_settings_schema_unref (schema);

	return ret;
}

static void
entry_removed (IndicatorObject      *io,
               IndicatorObjectEntry *entry,
               gpointer              data)
{
	GList *children, *l = NULL;

	const gchar *io_name = g_object_get_data (G_OBJECT (io), "io-name");
	g_debug ("Entry removed for io=%s", io_name);

	GooroomIndicatorApplet *applet = GOOROOM_INDICATOR_APPLET (data);
	GooroomIndicatorAppletPrivate *priv = applet->priv;

	children = gtk_container_get_children (GTK_CONTAINER (priv->buttonbox));
	for (l = children; l; l = l->next) {
		XfceIndicatorButton *child = XFCE_INDICATOR_BUTTON (l->data);
		if (child && (xfce_indicator_button_get_entry (child) == entry)) {
			xfce_indicator_button_destroy (child);
			break;
		}
	}

	g_list_free (children);
}

static void
entry_added (IndicatorObject      *io,
             IndicatorObjectEntry *entry,
             gpointer              data)
{
	GtkWidget *button;
	const gchar *io_name;

	GooroomIndicatorApplet *applet = GOOROOM_INDICATOR_APPLET (data);
	GooroomIndicatorAppletPrivate *priv = applet->priv;

	io_name = g_object_get_data (G_OBJECT (io), "io-name");
	g_debug ("Entry added for io=%s", io_name);

	if (contain_blacklist (entry->name_hint))
		return;

	button = xfce_indicator_button_new (io, io_name, entry);

	if (entry->image != NULL)
		xfce_indicator_button_set_image (XFCE_INDICATOR_BUTTON (button), entry->image);

	if (entry->label != NULL)
		xfce_indicator_button_set_label (XFCE_INDICATOR_BUTTON (button), entry->label);

	if (entry->menu != NULL)
		xfce_indicator_button_set_menu (XFCE_INDICATOR_BUTTON (button), entry->menu);

	gtk_container_add (GTK_CONTAINER (priv->buttonbox), button);
	gtk_widget_show (button);
}

static gboolean
load_indicator (GooroomIndicatorApplet *applet,
                IndicatorObject        *io,
                const gchar            *name)
{
	GList *entries, *entry;
	IndicatorObjectEntry *entrydata;

	GooroomIndicatorAppletPrivate *priv = applet->priv;

	g_return_val_if_fail (name != NULL, FALSE);
	g_debug ("Load indicator: %s", name);

	g_object_set_data (G_OBJECT (io), "io-name", g_strdup (name));

	/* Connect to its signals */
	g_signal_connect (G_OBJECT (io), INDICATOR_OBJECT_SIGNAL_ENTRY_ADDED,
			G_CALLBACK (entry_added), applet);
	g_signal_connect (G_OBJECT (io), INDICATOR_OBJECT_SIGNAL_ENTRY_REMOVED,
			G_CALLBACK (entry_removed), applet);

	/* Work on the entries */
	entries = indicator_object_get_entries (io);
	entry = NULL;

	for (entry = entries; entry; entry = entry->next) {
		entrydata = (IndicatorObjectEntry *)entry->data;
		entry_added (io, entrydata, applet);
	}

	g_list_free (entries);

	return TRUE;
}

static gboolean
load_module (GooroomIndicatorApplet *applet, const gchar *name)
{
	gchar *path;
	IndicatorObject *io;

	if (!name || !g_str_has_suffix (name, G_MODULE_SUFFIX))
		return FALSE;

	g_debug ("Loading Module: %s", name);

	/* Build the object for the module */
	path = g_build_filename (INDICATOR_DIR, name, NULL);
	io = indicator_object_new_from_file (path);
	g_free (path);

	return load_indicator (applet, io, name);
}

static gint
load_modules (GooroomIndicatorApplet *applet)
{
	gint num = 0;
	GDir *dir = NULL;
	const gchar *io_name;

	dir = g_dir_open (INDICATOR_DIR, 0, NULL);

	while ((io_name = g_dir_read_name (dir)) != NULL) {
		g_debug ("Loading module: %s", io_name);
		if (load_module (applet, io_name)) {
			num++;
		}
	}

	g_dir_close (dir);

	return num;
}

static void
gooroom_indicator_applet_init (GooroomIndicatorApplet *applet)
{
	GtkCssProvider *provider = NULL;
	GooroomIndicatorAppletPrivate *priv;

	/* Initialize i18n */
	bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

	priv = applet->priv = gooroom_indicator_applet_get_instance_private (applet);

	panel_applet_set_flags (PANEL_APPLET (applet), PANEL_APPLET_EXPAND_MINOR);
	gtk_container_set_border_width (GTK_CONTAINER (applet), 0);

	/* Init some theme/icon stuff */
	gtk_icon_theme_append_search_path (gtk_icon_theme_get_default(), INDICATOR_ICONS_DIR);

	priv->buttonbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_container_add (GTK_CONTAINER (applet), priv->buttonbox);
	gtk_widget_show (priv->buttonbox);

	load_modules (applet);
}

static void
gooroom_indicator_applet_finalize (GObject *object)
{
	GooroomIndicatorApplet *applet = GOOROOM_INDICATOR_APPLET (object);
	GooroomIndicatorAppletPrivate *priv = applet->priv;

	G_OBJECT_CLASS (gooroom_indicator_applet_parent_class)->finalize (object);
}

static void
gooroom_indicator_applet_class_init (GooroomIndicatorAppletClass *class)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (class);

	object_class->finalize = gooroom_indicator_applet_finalize;
}

static gboolean
gooroom_indicator_applet_fill (GooroomIndicatorApplet *applet)
{
	g_return_val_if_fail (PANEL_IS_APPLET (applet), FALSE);

	gtk_widget_show_all (GTK_WIDGET (applet));

	return TRUE;
}

static gboolean
gooroom_indicator_applet_factory (PanelApplet *applet,
                                    const gchar *iid,
                                    gpointer     data)
{
	gboolean retval = FALSE;

	if (!g_strcmp0 (iid, "GooroomIndicatorApplet"))
		retval = gooroom_indicator_applet_fill (GOOROOM_INDICATOR_APPLET (applet));

	return retval;
}

PANEL_APPLET_IN_PROCESS_FACTORY ("GooroomIndicatorAppletFactory",
                                 GOOROOM_TYPE_INDICATOR_APPLET,
                                 gooroom_indicator_applet_factory,
                                 NULL)
