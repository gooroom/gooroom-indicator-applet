/* 
 *  Copyright (C) 2015-2021 Gooroom <gooroom@gooroom.kr>
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

#ifndef __GOOROOM_INDICATOR_APPLET_H__
#define __GOOROOM_INDICATOR_APPLET_H__

G_BEGIN_DECLS

#include <libgnome-panel/gp-applet.h>

#define GOOROOM_TYPE_INDICATOR_APPLET            (gooroom_indicator_applet_get_type ())
#define GOOROOM_INDICATOR_APPLET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOOROOM_TYPE_INDICATOR_APPLET, GooroomIndicatorApplet))
#define GOOROOM_INDICATOR_APPLET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GOOROOM_TYPE_INDICATOR_APPLET, GooroomIndicatorAppletClass))
#define GOOROOM_IS_INDICATOR_APPLET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOOROOM_TYPE_INDICATOR_APPLET))
#define GOOROOM_IS_INDICATOR_APPLET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GOOROOM_TYPE_INDICATOR_APPLET))
#define GOOROOM_INDICATOR_APPLET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GOOROOM_TYPE_INDICATOR_APPLET, GooroomIndicatorAppletClass))

typedef struct _GooroomIndicatorApplet        GooroomIndicatorApplet;
typedef struct _GooroomIndicatorAppletClass   GooroomIndicatorAppletClass;
typedef struct _GooroomIndicatorAppletPrivate GooroomIndicatorAppletPrivate;

struct _GooroomIndicatorApplet {
    GpApplet                       parent;
    GooroomIndicatorAppletPrivate *priv;
};

struct _GooroomIndicatorAppletClass {
    GpAppletClass parent_class;
};

GType gooroom_indicator_applet_get_type (void) G_GNUC_CONST;


G_END_DECLS

#endif /* !__GOOROOM_INDICATOR_APPLET_H__ */
