/*
    Musical Spectrum plugin for the DeaDBeeF audio player

    Copyright (C) 2014 Christian Boxdörfer <christian.boxdoerfer@posteo.de>

    Based on DeaDBeeFs stock spectrum.
    Copyright (c) 2009-2014 Alexey Yakovenko <waker@users.sourceforge.net>
    Copyright (c) 2011 William Pitcock <nenolod@dereferenced.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <fcntl.h>
#include <gtk/gtk.h>

#include "support.h"
#include "config.h"
#include "config_dialog.h"

#define     STR_GRADIENT_VERTICAL "Vertical"
#define     STR_GRADIENT_HORIZONTAL "Horizontal"
#define     STR_ALIGNMENT_LEFT "Left"
#define     STR_ALIGNMENT_RIGHT "Right"
#define     STR_ALIGNMENT_CENTER "Center"
#define     STR_WINDOW_BLACKMANN_HARRIS "Blackmann-Harris"
#define     STR_WINDOW_HANNING "Hanning"

void
on_button_config (GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *spectrum_properties;
    GtkWidget *config_dialog;
    GtkWidget *vbox01;
    GtkWidget *vbox02;
    GtkWidget *vbox03;
    GtkWidget *vbox04;
    GtkWidget *hbox01;
    GtkWidget *hbox02;
    GtkWidget *valign_01;
    GtkWidget *valign_02;
    GtkWidget *valign_03;
    GtkWidget *color_label;
    GtkWidget *color_frame;
    GtkWidget *color_bg_label;
    GtkWidget *color_bg;
    GtkWidget *color_vgrid_label;
    GtkWidget *color_vgrid;
    GtkWidget *color_hgrid_label;
    GtkWidget *color_hgrid;
    GtkWidget *hseparator_01;
    GtkWidget *color_gradients[MAX_NUM_COLORS];
    GtkWidget *num_colors_label;
    GtkWidget *num_colors;
    GtkWidget *hbox03;
    GtkWidget *db_range_label0;
    GtkWidget *db_range;
    GtkWidget *hgrid;
    GtkWidget *vgrid;
    GtkWidget *bar_mode;
    GtkWidget *hbox04;
    GtkWidget *window_label;
    GtkWidget *window;
    GtkWidget *hbox05;
    GtkWidget *gradient_orientation_label;
    GtkWidget *gradient_orientation;
    GtkWidget *hbox06;
    GtkWidget *alignment_label;
    GtkWidget *alignment;
    GtkWidget *dialog_action_area13;
    GtkWidget *applybutton1;
    GtkWidget *cancelbutton1;
    GtkWidget *okbutton1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    spectrum_properties = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (spectrum_properties), "Spectrum Properties");
    gtk_window_set_type_hint (GTK_WINDOW (spectrum_properties), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_window_set_resizable (GTK_WINDOW (spectrum_properties), FALSE);

    config_dialog = gtk_dialog_get_content_area (GTK_DIALOG (spectrum_properties));
    gtk_widget_show (config_dialog);

    hbox01 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox01);
    gtk_box_pack_start (GTK_BOX (config_dialog), hbox01, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox01), 12);

    color_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (color_label),"<b>Colors</b>");
    gtk_widget_show (color_label);

    color_frame = gtk_frame_new ("Colors");
    gtk_frame_set_label_widget ((GtkFrame *)color_frame, color_label);
    gtk_frame_set_shadow_type ((GtkFrame *)color_frame, GTK_SHADOW_IN);
    gtk_widget_show (color_frame);
    gtk_box_pack_start (GTK_BOX (hbox01), color_frame, TRUE, FALSE, 0);

    vbox01 = gtk_vbox_new (FALSE, 8);
    gtk_widget_show (vbox01);
    gtk_container_add (GTK_CONTAINER (color_frame), vbox01);
    gtk_container_set_border_width (GTK_CONTAINER (vbox01), 12);

    hbox02 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox02);
    gtk_box_pack_start (GTK_BOX (vbox01), hbox02, TRUE, TRUE, 0);

    vbox03 = gtk_vbox_new (FALSE, 8);
    gtk_widget_show (vbox03);
    gtk_box_pack_start (GTK_BOX (hbox02), vbox03, TRUE, TRUE, 0);

    vbox04 = gtk_vbox_new (FALSE, 8);
    gtk_widget_show (vbox04);
    gtk_box_pack_start (GTK_BOX (hbox02), vbox04, TRUE, TRUE, 0);

    valign_01 = gtk_alignment_new(0, 1, 0, 1);
    gtk_container_add(GTK_CONTAINER(vbox03), valign_01);
    gtk_widget_show (valign_01);

    color_bg_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (color_bg_label),"Background:");
    gtk_widget_show (color_bg_label);
    gtk_container_add(GTK_CONTAINER(valign_01), color_bg_label);

    color_bg = gtk_color_button_new ();
    gtk_color_button_set_use_alpha ((GtkColorButton *)color_bg, TRUE);
    gtk_widget_show (color_bg);
    gtk_box_pack_start (GTK_BOX (vbox04), color_bg, TRUE, TRUE, 0);

    valign_02 = gtk_alignment_new(0, 1, 0, 1);
    gtk_container_add(GTK_CONTAINER(vbox03), valign_02);
    gtk_widget_show (valign_02);

    color_vgrid_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (color_vgrid_label),"Vertical grid:");
    gtk_widget_show (color_vgrid_label);
    gtk_container_add(GTK_CONTAINER(valign_02), color_vgrid_label);

    color_vgrid = gtk_color_button_new ();
    gtk_color_button_set_use_alpha ((GtkColorButton *)color_vgrid, TRUE);
    gtk_widget_show (color_vgrid);
    gtk_box_pack_start (GTK_BOX (vbox04), color_vgrid, TRUE, TRUE, 0);

    valign_03 = gtk_alignment_new(0, 1, 0, 1);
    gtk_container_add(GTK_CONTAINER(vbox03), valign_03);
    gtk_widget_show (valign_03);

    color_hgrid_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (color_hgrid_label),"Horizontal grid:");
    gtk_widget_show (color_hgrid_label);
    gtk_container_add(GTK_CONTAINER(valign_03), color_hgrid_label);

    color_hgrid = gtk_color_button_new ();
    gtk_color_button_set_use_alpha ((GtkColorButton *)color_hgrid, TRUE);
    gtk_widget_show (color_hgrid);
    gtk_box_pack_start (GTK_BOX (vbox04), color_hgrid, TRUE, TRUE, 0);

    hseparator_01 = gtk_hseparator_new ();
    gtk_widget_show (hseparator_01);
    gtk_box_pack_start (GTK_BOX (vbox01), hseparator_01, TRUE, TRUE, 0);

    num_colors_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (num_colors_label),"Number of colors:");
    gtk_widget_show (num_colors_label);
    gtk_box_pack_start (GTK_BOX (vbox01), num_colors_label, FALSE, FALSE, 0);

    num_colors = gtk_spin_button_new_with_range (1,MAX_NUM_COLORS,1);
    gtk_widget_show (num_colors);
    gtk_box_pack_start (GTK_BOX (vbox01), num_colors, FALSE, FALSE, 0);

    for (int i = 0; i < MAX_NUM_COLORS; i++) {
        color_gradients[i] = gtk_color_button_new ();
        gtk_color_button_set_use_alpha ((GtkColorButton *)color_gradients[i], TRUE);
        if (i < CONFIG_NUM_COLORS) {
            gtk_widget_show (color_gradients[i]);
        }
        else {
            gtk_widget_hide (color_gradients[i]);
        }
        gtk_box_pack_start (GTK_BOX (vbox01), color_gradients[i], TRUE, FALSE, 0);
        gtk_widget_set_size_request (color_gradients[i], -1, 30);
        gtk_color_button_set_color (GTK_COLOR_BUTTON (color_gradients[i]), &(CONFIG_GRADIENT_COLORS[i]));
    }

    vbox02 = gtk_vbox_new (FALSE, 8);
    gtk_widget_show (vbox02);
    gtk_box_pack_start (GTK_BOX (hbox01), vbox02, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox02), 12);

    hbox03 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox03);
    gtk_box_pack_start (GTK_BOX (vbox02), hbox03, FALSE, FALSE, 0);

    db_range_label0 = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (db_range_label0),"dB range:");
    gtk_widget_show (db_range_label0);
    gtk_box_pack_start (GTK_BOX (hbox03), db_range_label0, FALSE, TRUE, 0);

    db_range = gtk_spin_button_new_with_range (50,120,10);
    gtk_widget_show (db_range);
    gtk_box_pack_start (GTK_BOX (hbox03), db_range, TRUE, TRUE, 0);

    hbox04 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox04);
    gtk_box_pack_start (GTK_BOX (vbox02), hbox04, FALSE, FALSE, 0);

    window_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (window_label),"Window function:");
    gtk_widget_show (window_label);
    gtk_box_pack_start (GTK_BOX (hbox04), window_label, FALSE, TRUE, 0);

    window = gtk_combo_box_text_new ();
    gtk_widget_show (window);
    gtk_box_pack_start (GTK_BOX (hbox04), window, TRUE, TRUE, 0);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(window), STR_WINDOW_BLACKMANN_HARRIS);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(window), STR_WINDOW_HANNING);

    hbox05 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox05);
    gtk_box_pack_start (GTK_BOX (vbox02), hbox05, FALSE, FALSE, 0);

    gradient_orientation_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (gradient_orientation_label),"Gradient orientation:");
    gtk_widget_show (gradient_orientation_label);
    gtk_box_pack_start (GTK_BOX (hbox05), gradient_orientation_label, FALSE, TRUE, 0);

    gradient_orientation = gtk_combo_box_text_new ();
    gtk_widget_show (gradient_orientation);
    gtk_box_pack_start (GTK_BOX (hbox05), gradient_orientation, TRUE, TRUE, 0);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(gradient_orientation), STR_GRADIENT_VERTICAL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gradient_orientation), STR_GRADIENT_HORIZONTAL);

    hbox06 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox06);
    gtk_box_pack_start (GTK_BOX (vbox02), hbox06, FALSE, FALSE, 0);

    alignment_label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (alignment_label),"Alignment:");
    gtk_widget_show (alignment_label);
    gtk_box_pack_start (GTK_BOX (hbox06), alignment_label, FALSE, TRUE, 0);

    alignment = gtk_combo_box_text_new ();
    gtk_widget_show (alignment);
    gtk_box_pack_start (GTK_BOX (hbox06), alignment, TRUE, TRUE, 0);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(alignment), STR_ALIGNMENT_LEFT);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(alignment), STR_ALIGNMENT_RIGHT);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(alignment), STR_ALIGNMENT_CENTER);

    hgrid = gtk_check_button_new_with_label ("Horizontal grid");
    gtk_widget_show (hgrid);
    gtk_box_pack_start (GTK_BOX (vbox02), hgrid, FALSE, FALSE, 0);

    vgrid = gtk_check_button_new_with_label ("Vertical grid");
    gtk_widget_show (vgrid);
    gtk_box_pack_start (GTK_BOX (vbox02), vgrid, FALSE, FALSE, 0);

    bar_mode = gtk_check_button_new_with_label ("Bar mode");
    gtk_widget_show (bar_mode);
    gtk_box_pack_start (GTK_BOX (vbox02), bar_mode, FALSE, FALSE, 0);

    dialog_action_area13 = gtk_dialog_get_action_area (GTK_DIALOG (spectrum_properties));
    gtk_widget_show (dialog_action_area13);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area13), GTK_BUTTONBOX_END);

    applybutton1 = gtk_button_new_from_stock ("gtk-apply");
    gtk_widget_show (applybutton1);
    gtk_dialog_add_action_widget (GTK_DIALOG (spectrum_properties), applybutton1, GTK_RESPONSE_APPLY);
    gtk_widget_set_can_default (applybutton1, TRUE);

    cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
    gtk_widget_show (cancelbutton1);
    gtk_dialog_add_action_widget (GTK_DIALOG (spectrum_properties), cancelbutton1, GTK_RESPONSE_CANCEL);
    gtk_widget_set_can_default (cancelbutton1, TRUE);

    okbutton1 = gtk_button_new_from_stock ("gtk-ok");
    gtk_widget_show (okbutton1);
    gtk_dialog_add_action_widget (GTK_DIALOG (spectrum_properties), okbutton1, GTK_RESPONSE_OK);
    gtk_widget_set_can_default (okbutton1, TRUE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (hgrid), CONFIG_ENABLE_HGRID);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (vgrid), CONFIG_ENABLE_VGRID);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (bar_mode), CONFIG_ENABLE_BAR_MODE);
    gtk_combo_box_set_active (GTK_COMBO_BOX (window), CONFIG_WINDOW);
    gtk_combo_box_set_active (GTK_COMBO_BOX (gradient_orientation), CONFIG_GRADIENT_ORIENTATION);
    gtk_combo_box_set_active (GTK_COMBO_BOX (alignment), CONFIG_ALIGNMENT);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (num_colors), CONFIG_NUM_COLORS);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (db_range), CONFIG_DB_RANGE);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (color_bg), &CONFIG_COLOR_BG);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (color_vgrid), &CONFIG_COLOR_VGRID);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (color_hgrid), &CONFIG_COLOR_HGRID);

    char text[100];
    for (;;) {
        int response = gtk_dialog_run (GTK_DIALOG (spectrum_properties));
        if (response == GTK_RESPONSE_OK || response == GTK_RESPONSE_APPLY) {
            gtk_color_button_get_color (GTK_COLOR_BUTTON (color_bg), &CONFIG_COLOR_BG);
            gtk_color_button_get_color (GTK_COLOR_BUTTON (color_vgrid), &CONFIG_COLOR_VGRID);
            gtk_color_button_get_color (GTK_COLOR_BUTTON (color_hgrid), &CONFIG_COLOR_HGRID);
            for (int i = 0; i < CONFIG_NUM_COLORS; i++) {
                gtk_color_button_get_color (GTK_COLOR_BUTTON (color_gradients[i]), &CONFIG_GRADIENT_COLORS[i]);
            }
            CONFIG_ENABLE_HGRID = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (hgrid));
            CONFIG_ENABLE_VGRID = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (vgrid));
            CONFIG_ENABLE_BAR_MODE = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (bar_mode));
            CONFIG_DB_RANGE = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (db_range));
            CONFIG_NUM_COLORS = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (num_colors));
            for (int i = 0; i < MAX_NUM_COLORS && color_gradients[i]; i++) {
                if (i < CONFIG_NUM_COLORS) {
                    gtk_widget_show (color_gradients[i]);
                }
                else if (color_gradients[i]) {
                    gtk_widget_hide (color_gradients[i]);
                }
            }

            snprintf (text, sizeof (text), "%s", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (gradient_orientation)));
            if (strcmp (text, STR_GRADIENT_VERTICAL) == 0) {
                CONFIG_GRADIENT_ORIENTATION = 0;
            }
            else if (strcmp (text, STR_GRADIENT_HORIZONTAL) == 0) {
                CONFIG_GRADIENT_ORIENTATION = 1;
            }
            else {
                CONFIG_GRADIENT_ORIENTATION = -1;
            }

            snprintf (text, sizeof (text), "%s", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (alignment)));
            if (strcmp (text, STR_ALIGNMENT_LEFT) == 0) {
                CONFIG_ALIGNMENT = LEFT;
            }
            else if (strcmp (text, STR_ALIGNMENT_RIGHT) == 0) {
                CONFIG_ALIGNMENT = RIGHT;
            }
            else if (strcmp (text, STR_ALIGNMENT_CENTER) == 0) {
                CONFIG_ALIGNMENT = CENTER;
            }
            else {
                CONFIG_ALIGNMENT = -1;
            }

            snprintf (text, sizeof (text), "%s", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (window)));
            if (strcmp (text, STR_WINDOW_BLACKMANN_HARRIS) == 0) {
                CONFIG_WINDOW = BLACKMAN_HARRIS;
            }
            else if (strcmp (text, STR_WINDOW_HANNING) == 0) {
                CONFIG_WINDOW = HANNING;
            }
            else {
                CONFIG_GRADIENT_ORIENTATION = -1;
            }

            save_config ();
            deadbeef->sendmessage (DB_EV_CONFIGCHANGED, 0, 0, 0);
        }
        if (response == GTK_RESPONSE_APPLY) {
            continue;
        }
        break;
    }
    gtk_widget_destroy (spectrum_properties);
#pragma GCC diagnostic pop
    return;
}

