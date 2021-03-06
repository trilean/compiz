/*
 * Copyright © 2011 Canonical Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Authored by:
 *     Sam Spilsbury <sam.spilsbury@canonical.com>
 */

#include "gtk-window-decorator.h"
#include "gwd-settings.h"

typedef struct _decor_frame_type_info
{
    create_frame_proc create_func;
    destroy_frame_proc destroy_func;
} decor_frame_type_info_t;

GHashTable    *frame_info_table;
GHashTable    *frames_table;

static void
decor_frame_refresh (decor_frame_t *frame)
{
    decor_shadow_options_t active_o, inactive_o;
    decor_shadow_info_t *info;

    gwd_decor_frame_ref (frame);

    frame_update_titlebar_font (frame);

    if (strcmp (frame->type, "switcher") != 0 && strcmp (frame->type, "bare") != 0)
        gwd_theme_update_border_extents (gwd_theme, frame);

    gwd_theme_get_shadow (gwd_theme, frame, &active_o, TRUE);
    gwd_theme_get_shadow (gwd_theme, frame, &inactive_o, FALSE);

    info = malloc (sizeof (decor_shadow_info_t));

    if (!info)
	return;

    info->frame = frame;
    info->state = 0;

    frame_update_shadow (frame, info, &active_o, &inactive_o);

    gwd_decor_frame_unref (frame);

    free (info);
    info = NULL;
}

decor_frame_t *
gwd_get_decor_frame (const gchar *frame_name)
{
    decor_frame_t *frame = g_hash_table_lookup (frames_table, frame_name);

    if (!frame)
    {
	/* Frame not found, look up frame type in the frame types
	 * hash table and create a new one */

	decor_frame_type_info_t *info = g_hash_table_lookup (frame_info_table, frame_name);

	if (!info)
	    g_critical ("Could not find frame info %s in frame type table", frame_name);

	frame = (*info->create_func) (frame_name);

	if (!frame)
	    g_critical ("Could not allocate frame %s", frame_name);

	g_hash_table_insert (frames_table, frame->type, frame);

	gwd_decor_frame_ref (frame);

	decor_frame_refresh (frame);
    }
    else
	gwd_decor_frame_ref (frame);

    return frame;
}

decor_frame_t *
gwd_decor_frame_ref (decor_frame_t *frame)
{
    ++frame->refcount;
    return frame;
}

decor_frame_t *
gwd_decor_frame_unref (decor_frame_t *frame)
{
    --frame->refcount;

    if (frame->refcount == 0)
    {
	decor_frame_type_info_t *info = g_hash_table_lookup (frame_info_table, frame->type);

	if (!info)
	    g_critical ("Couldn't find %s in frame info table", frame->type);

	if(!g_hash_table_remove (frames_table, frame->type))
	    g_critical ("Could not remove frame type %s from hash_table!", frame->type);

	(*info->destroy_func) (frame);
    }
    return frame;
}

gboolean
gwd_decor_frame_add_type (const gchar	     *name,
			  create_frame_proc  create_func,
			  destroy_frame_proc destroy_func)
{
    decor_frame_type_info_t *frame_type = malloc (sizeof (decor_frame_type_info_t));

    if (!frame_type)
	return FALSE;

    frame_type->create_func = create_func;
    frame_type->destroy_func = destroy_func;

    g_hash_table_insert (frame_info_table, strdup (name), frame_type);

    return TRUE;
}

void
gwd_decor_frame_remove_type (const gchar	     *name)
{
    g_hash_table_remove (frame_info_table, name);
}

void
gwd_frames_foreach (GHFunc   foreach_func,
		    gpointer user_data)
{
    g_hash_table_foreach (frames_table, foreach_func, user_data);
}

void
gwd_process_frames (GHFunc	foreach_func,
		    const gchar	*frame_keys[],
		    gint	frame_keys_num,
		    gpointer	user_data)
{
    gint   i = 0;

    for (; i < frame_keys_num; ++i)
    {
	gpointer frame = g_hash_table_lookup (frames_table, frame_keys[i]);

	if (!frame)
	    continue;

	(*foreach_func) ((gpointer) frame_keys[i], frame, user_data);
    }
}

void
destroy_frame_type (gpointer data)
{
    decor_frame_type_info_t *info = (decor_frame_type_info_t *) data;

    if (info)
    {
	/* TODO: Destroy all frames with this type using
	 * the frame destroy function */
    }

    free (info);
}

decor_frame_t *
decor_frame_new (const gchar *type)
{
    decor_frame_t *frame = malloc (sizeof (decor_frame_t));

    if (!frame)
    {
	g_critical ("Couldn't allocate frame!");
	return NULL;
    }

    frame->type = strdup (type);
    frame->refcount = 0;
    frame->border_shadow_active = NULL;
    frame->border_shadow_inactive = NULL;
    frame->max_border_shadow_active = NULL;
    frame->max_border_shadow_inactive = NULL;

    frame->pango_context = NULL;

    return frame;
}

void
decor_frame_destroy (decor_frame_t *frame)
{
    Display *xdisplay = gdk_x11_get_default_xdisplay ();

    if (frame->border_shadow_active)
	decor_shadow_destroy (xdisplay, frame->border_shadow_active);

    if (frame->border_shadow_inactive)
	decor_shadow_destroy (xdisplay, frame->border_shadow_inactive);

    if (frame->max_border_shadow_active)
	decor_shadow_destroy (xdisplay, frame->max_border_shadow_active);

    if (frame->max_border_shadow_inactive)
	decor_shadow_destroy (xdisplay, frame->max_border_shadow_inactive);

    if (frame->pango_context)
	g_object_unref (G_OBJECT (frame->pango_context));

    if (frame)
	free (frame->type);

    free (frame);
}

void
initialize_decorations ()
{
    frame_info_table = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, destroy_frame_type);

    gwd_decor_frame_add_type ("normal", create_normal_frame, destroy_normal_frame);
    gwd_decor_frame_add_type ("dialog", create_normal_frame, destroy_normal_frame);
    gwd_decor_frame_add_type ("modal_dialog", create_normal_frame, destroy_normal_frame);
    gwd_decor_frame_add_type ("menu", create_normal_frame, destroy_normal_frame);
    gwd_decor_frame_add_type ("utility", create_normal_frame, destroy_normal_frame);
    gwd_decor_frame_add_type ("switcher", create_switcher_frame, destroy_switcher_frame);
    gwd_decor_frame_add_type ("bare", create_bare_frame, destroy_bare_frame);

    frames_table = g_hash_table_new (g_str_hash, g_str_equal);
}
