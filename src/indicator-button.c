/*  
 *  Copyright (C) 2015-2019 Gooroom <gooroom@gooroom.kr>
 *  Copyright (c) 2012-2013 Andrzej <ndrwrdck@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 *  This file implements an indicator button class corresponding to
 *  a single indicator object entry.
 *
 */



#include <glib.h>
#include <gtk/gtk.h>

#include <libayatana-indicator/indicator-object.h>

#include "indicator-button.h"

struct _XfceIndicatorButton
{
  GtkToggleButton       __parent__;

  IndicatorObject      *io;
  const gchar          *io_name;
  IndicatorObjectEntry *entry;

  GtkMenu              *menu;

  GtkWidget            *box;
  GtkWidget            *icon;
  GtkWidget            *label;

  gulong                deactivate_id;
};

struct _XfceIndicatorButtonClass
{
  GtkToggleButtonClass __parent__;
};


G_DEFINE_TYPE (XfceIndicatorButton, xfce_indicator_button, GTK_TYPE_TOGGLE_BUTTON)




//static gboolean
//xfce_indicator_button_query_tooltip (GtkWidget  *widget,
//                                     gint        x,
//                                     gint        y,
//                                     gboolean    keyboard_mode,
//                                     GtkTooltip *tooltip,
//                                     gpointer    user_data)
//{
//	XfceIndicatorButton *button = XFCE_INDICATOR_BUTTON (widget);
//
//	if (button->entry && button->entry->accessible_desc) {
//		gtk_tooltip_set_text (tooltip, button->entry->accessible_desc);
//
//		return TRUE;
//	}
//
//	return FALSE;
//}

static void
xfce_indicator_button_icon_changed (GtkImage            *icon,
                                    GParamSpec          *pspec,
                                    XfceIndicatorButton *button)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_IMAGE (icon));

	gtk_widget_queue_resize (GTK_WIDGET (button));
}

static void
xfce_indicator_button_label_changed (GtkLabel            *label,
                                     GParamSpec          *pspec,
                                     XfceIndicatorButton *button)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_LABEL (label));

	gtk_widget_queue_resize (GTK_WIDGET (button));
}

static void
xfce_indicator_button_menu_deactivate (XfceIndicatorButton *button,
                                       GtkMenu             *menu)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_MENU (menu));

	if (button->deactivate_id) {
		g_signal_handler_disconnect (menu, button->deactivate_id);
		button->deactivate_id = 0;
	}

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), FALSE);
}

static gboolean
xfce_indicator_button_button_press (GtkWidget      *widget,
                                    GdkEventButton *event)
{
	GdkRectangle rect;
	GtkAllocation alloc;
	XfceIndicatorButton *button = XFCE_INDICATOR_BUTTON (widget);

	gtk_widget_get_allocation (widget, &alloc);

	if(event->button == 1 && button->menu != NULL) /* left click only */
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),TRUE);
		button->deactivate_id = g_signal_connect_swapped
			(G_OBJECT (button->menu), "deactivate",
			 G_CALLBACK (xfce_indicator_button_menu_deactivate), button);
		gtk_menu_reposition (GTK_MENU (button->menu));

		rect.x = alloc.x;
		rect.y = alloc.y;
		rect.width = alloc.width;
		rect.height = alloc.height;

        gtk_menu_popup_at_rect (GTK_MENU (button->menu),
                                gtk_widget_get_window (widget),
                                &rect,
                                GDK_GRAVITY_NORTH_WEST,
                                GDK_GRAVITY_NORTH_WEST,
                                NULL);

		return TRUE;
	}

	return FALSE;
}

static gboolean
xfce_indicator_button_button_release (GtkWidget      *widget,
                                      GdkEventButton *event)
{
	XfceIndicatorButton *button = XFCE_INDICATOR_BUTTON (widget);

	if(event->button == 2) /* middle button */
	{
		g_signal_emit_by_name (button->io, INDICATOR_OBJECT_SIGNAL_SECONDARY_ACTIVATE, 
				button->entry, event->time);
		return TRUE;
	}

	return FALSE;
}

static void
xfce_indicator_button_init (XfceIndicatorButton *button)
{
	gtk_widget_set_can_focus(GTK_WIDGET(button), FALSE);
	gtk_widget_set_can_default (GTK_WIDGET (button), FALSE);
	gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
	gtk_button_set_use_underline (GTK_BUTTON (button),TRUE);
	gtk_widget_set_focus_on_click (GTK_WIDGET (button), FALSE);
	gtk_widget_set_name (GTK_WIDGET (button), "gooroom-indicator-applet-button");

	button->io = NULL;
	button->entry = NULL;
	button->menu = NULL;
	button->icon = NULL;
	button->label = NULL;
	button->deactivate_id = 0;

	gtk_widget_set_halign (GTK_WIDGET (button), GTK_ALIGN_FILL);
	gtk_widget_set_valign (GTK_WIDGET (button), GTK_ALIGN_FILL);
}

static void
xfce_indicator_button_class_init (XfceIndicatorButtonClass *klass)
{
  GtkWidgetClass    *widget_class;

  widget_class = GTK_WIDGET_CLASS (klass);
  widget_class->button_press_event = xfce_indicator_button_button_press;
  widget_class->button_release_event = xfce_indicator_button_button_release;
}

GtkWidget *
xfce_indicator_button_new (IndicatorObject        *io,
                           const gchar            *io_name,
                           IndicatorObjectEntry   *entry)
{
	XfceIndicatorButton *button = g_object_new (XFCE_TYPE_INDICATOR_BUTTON, NULL);

	button->io = io;
	button->io_name = io_name;
	button->entry = entry;

	button->box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_container_add (GTK_CONTAINER (button), button->box);
	gtk_widget_show (button->box);

//	g_object_set (G_OBJECT (button), "has-tooltip", TRUE, NULL);
//	g_signal_connect (button, "query-tooltip",
//			G_CALLBACK (xfce_indicator_button_query_tooltip), NULL);

	return GTK_WIDGET (button);
}

void
xfce_indicator_button_set_label (XfceIndicatorButton *button,
                                 GtkLabel            *label)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_LABEL (label));

	if (button->label != NULL)
		gtk_container_remove (GTK_CONTAINER (button->box), button->label);

	button->label = GTK_WIDGET (label);
	g_signal_connect (G_OBJECT (button->label), "notify::label",
			G_CALLBACK (xfce_indicator_button_label_changed), button);
	g_signal_connect (G_OBJECT (button->label), "notify::visible",
			G_CALLBACK (xfce_indicator_button_label_changed), button);

	gtk_box_pack_start (GTK_BOX (button->box), button->label, TRUE, TRUE, 0);
	gtk_widget_show (button->label);
}

void
xfce_indicator_button_set_image (XfceIndicatorButton *button,
                                 GtkImage            *image)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_IMAGE (image));

	if (button->icon != NULL) 
		gtk_container_remove (GTK_CONTAINER (button->box), button->icon);

	button->icon = GTK_WIDGET (image);
	g_signal_connect (G_OBJECT (button->icon), "notify::pixbuf",
			G_CALLBACK (xfce_indicator_button_icon_changed), button);

	gtk_box_pack_start (GTK_BOX (button->box), button->icon, TRUE, TRUE, 0);
	gtk_widget_show (button->icon);
}

void
xfce_indicator_button_set_menu (XfceIndicatorButton *button,
                                GtkMenu             *menu)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));
	g_return_if_fail (GTK_IS_MENU (menu));

	if (button->menu != NULL) {
		gtk_menu_detach (button->menu);
		gtk_menu_popdown (button->menu);
		button->menu = NULL;
	}

	button->menu = menu;
	gtk_menu_attach_to_widget (menu, GTK_WIDGET (button), NULL);
}

IndicatorObjectEntry *
xfce_indicator_button_get_entry (XfceIndicatorButton *button)
{
	g_return_val_if_fail (XFCE_IS_INDICATOR_BUTTON (button), NULL);

	return button->entry;
}

IndicatorObject *
xfce_indicator_button_get_io (XfceIndicatorButton *button)
{
	g_return_val_if_fail (XFCE_IS_INDICATOR_BUTTON (button), NULL);

	return button->io;
}

const gchar *
xfce_indicator_button_get_io_name (XfceIndicatorButton *button)
{
	g_return_val_if_fail (XFCE_IS_INDICATOR_BUTTON (button), NULL);

	return button->io_name;
}

void
xfce_indicator_button_destroy (XfceIndicatorButton *button)
{
	g_return_if_fail (XFCE_IS_INDICATOR_BUTTON (button));

	if (button->menu != NULL) {
		gtk_menu_detach (button->menu);
		gtk_menu_popdown (button->menu);
		button->menu = NULL;
	}

	gtk_widget_destroy (GTK_WIDGET (button));
}
