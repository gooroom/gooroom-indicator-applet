/*
 *  Copyright (c) 2012-2013 Andrzej <ndrwrdck@gmail.com>
 *  Copyright (C) 2015-2021 Gooroom <gooroom@gooroom.kr>
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

#ifndef __INDICATOR_BUTTON_H__
#define __INDICATOR_BUTTON_H__

#include <glib.h>
#include <gtk/gtk.h>

#include <libayatana-indicator/indicator-object.h>

G_BEGIN_DECLS

GType xfce_indicator_button_get_type (void);

#define XFCE_TYPE_INDICATOR_BUTTON             (xfce_indicator_button_get_type())
#define XFCE_INDICATOR_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), XFCE_TYPE_INDICATOR_BUTTON, XfceIndicatorButton))
#define XFCE_INDICATOR_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), XFCE_TYPE_INDICATOR_BUTTON, XfceIndicatorButtonClass))
#define XFCE_IS_INDICATOR_BUTTON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), XFCE_TYPE_INDICATOR_BUTTON))
#define XFCE_IS_INDICATOR_BUTTON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), XFCE_TYPE_INDICATOR_BUTTON))
#define XFCE_INDICATOR_BUTTON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), XFCE_TYPE_INDICATOR_BUTTON, XfceIndicatorButtonClass))

typedef struct          _XfceIndicatorButton              XfceIndicatorButton;
typedef struct          _XfceIndicatorButtonClass         XfceIndicatorButtonClass;


void                    xfce_indicator_button_set_label   (XfceIndicatorButton        *button,
                                                           GtkLabel                   *label);

void                    xfce_indicator_button_set_image   (XfceIndicatorButton        *button,
                                                           GtkImage                   *image);

void                    xfce_indicator_button_set_menu    (XfceIndicatorButton        *button,
                                                           GtkMenu                    *menu);

IndicatorObjectEntry   *xfce_indicator_button_get_entry   (XfceIndicatorButton        *button);

IndicatorObject        *xfce_indicator_button_get_io      (XfceIndicatorButton        *button);

const gchar            *xfce_indicator_button_get_io_name (XfceIndicatorButton        *button);

GtkWidget              *xfce_indicator_button_new         (IndicatorObject            *io,
                                                           const gchar                *io_name,
                                                           IndicatorObjectEntry       *entry);

void                    xfce_indicator_button_destroy     (XfceIndicatorButton *button);

G_END_DECLS

#endif /* !__INDICATOR_BUTTON_H__ */
