/*
 * tintamp_ui.h
 *
 * Part of tintamp (the integer amplifier)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <math.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <librfn.h>
#include <libtt.h>
#include "tintamp_ui.h"

static GtkBuilder *builder;
static GtkDialog *aboutdialog;

static struct {
	const char *name;
	int ctrl;
} ui_map[] = {
		{ "adjustment_gain", TT_PREAMP_CONTROL_GAIN }
};

static void polar2cartesian(double r, double t, double *x, double *y)
{
	*x = r * cos(t);
	*y = r * sin(t);
}


void on_window_destroy (GtkWidget *object, gpointer user_data)
{
	gtk_main_quit();
}

void on_menuitem_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	gtk_main_quit();
}

void on_aboutmenuitem_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	gtk_dialog_run(aboutdialog);
	gtk_widget_hide(GTK_WIDGET(aboutdialog));
}

gboolean on_knob_draw (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	gdouble x1, x2, y1, y2;
	gdouble theta;

	GtkAdjustment *adjustment = GTK_ADJUSTMENT(data);
	gdouble lower = gtk_adjustment_get_lower(adjustment);
	gdouble upper = gtk_adjustment_get_upper(adjustment);
	gdouble value = gtk_adjustment_get_value(adjustment);

	// put is the value to be displayed (in range 0..1)
	gdouble range = (value - lower) / (upper - lower);

	guint width, height, size;
	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);
	size = MIN(width, height);

	cairo_translate(cr, (width - size) / 2.0, (height - size) / 2.0);
	cairo_scale(cr, size, size);
	cairo_translate(cr, 0.5, 0.5);

	GdkRGBA color;

	gdouble start = 0.75 * G_PI;
	gdouble end = 2.25 * G_PI;

	// set the drawing colour
	gtk_style_context_get_color (
			gtk_widget_get_style_context(widget),
			gtk_widget_get_state_flags(widget), &color);
	gdk_cairo_set_source_rgba (cr, &color);
	cairo_set_line_width(cr, 0.010);

	// draw the scale arc
	cairo_arc (cr, 0.0, 0.0, 0.45, start, end);
	cairo_stroke (cr);

	// draw the tick marks
	for (theta = start; theta <= end; theta += ((end - start) / 10)) {
		polar2cartesian(0.45, theta, &x1, &y1);
		polar2cartesian(0.5, theta, &x2, &y2);
		cairo_move_to(cr, x1, y1);
		cairo_line_to(cr, x2, y2);
		cairo_stroke(cr);
	}


	// draw the knob's pointer
	cairo_set_line_width(cr, 0.04);
	theta = start + (range * (end - start));
	polar2cartesian(0.1, theta, &x1, &y1);
	polar2cartesian(0.3, theta, &x2, &y2);
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_stroke (cr);

	cairo_set_line_width(cr, 0.03);
	cairo_arc(cr, 0.0, 0.0, 0.35, 0, 2 * G_PI);
	cairo_stroke(cr);

	return FALSE;
}

void on_knob_value_changed(GtkScaleButton *button,
		gdouble         value,
		gpointer        user_data)
{
	typedef void (ctrlfn_t)(void *, int, ttspl_t);

	// make sure the UI redraws
	GtkWidget *drawing_area = GTK_WIDGET(user_data);
	gtk_widget_queue_draw(drawing_area);

	// update the engine
	GObject *adjustment = G_OBJECT(gtk_scale_button_get_adjustment(button));
	ctrlfn_t *func = g_object_get_data(adjustment, "func");
	void *obj = g_object_get_data(adjustment, "obj");
	int ctrl = (int) (intptr_t) g_object_get_data(adjustment, "ctrl");

	if (func && obj && ctrl)
		func(obj, ctrl, TTFLOAT(value));
	else
		g_warning("Knob is not connected to anything");

}

static void fixup_scale_button(GtkBuilder *builder, char *sb_name, char *da_name)
{
	GtkScaleButton *scale_button =
			GTK_SCALE_BUTTON(gtk_builder_get_object(builder, sb_name));
	GtkWidget *drawing_area =
			GTK_WIDGET(gtk_builder_get_object(builder, da_name));
	GtkContainer *dummy_container =
			GTK_CONTAINER(gtk_widget_get_parent(drawing_area));

	// Remove the (pointless) child widget from the scale button. Strictly speaking ref'ing this
	// object creates a memory leak but although the scale button allows us to remove the child
	// widget it keeps on updating it [hardcoded in gtk_scale_button_update_icon()] even after
	// letting go of the reference.
	//
	// Leaking is better than the alternative!
	GtkWidget *pointless_image = gtk_bin_get_child(GTK_BIN(scale_button));
	g_object_ref((gpointer) pointless_image);
	gtk_container_remove(GTK_CONTAINER(scale_button), pointless_image);

	// Migrate the drawing area from its original (dummy) container into the scale button.
	gtk_container_remove(dummy_container, drawing_area);
	gtk_container_add(GTK_CONTAINER(scale_button), drawing_area);
}

void tintamp_ui_init(void)
{
	GtkWidget       *window;

	builder = gtk_builder_new ();
	gtk_builder_add_from_resource (builder, "/tintamp/tintamp.ui", NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	aboutdialog = GTK_DIALOG (gtk_builder_get_object(builder, "aboutdialog"));

	// scalebutton has to be "tricked" into supporting custom widgets
#define FIXUP(x) fixup_scale_button(builder, "scalebutton_" #x, "drawingarea_" #x)
	FIXUP(gain);
	FIXUP(treble);
	FIXUP(middle);
	FIXUP(bass);
	FIXUP(volume);
#undef FIXUP

	gtk_builder_connect_signals (builder, NULL);

	gtk_widget_show (window);
}

void tintamp_ui_setup(tt_tintamp_t *tt)
{
	for (size_t i=0; i<lengthof(ui_map); i++) {
		GObject *obj;

		obj = gtk_builder_get_object(builder, ui_map[i].name);
		g_object_set_data(obj, "func", &tt_tintamp_set_control);
		g_object_set_data(obj, "obj", tt);
		g_object_set_data(obj, "ctrl", (void *) (intptr_t) ui_map[i].ctrl);

		ttspl_t val = tt_tintamp_get_control(tt, ui_map[i].ctrl);
		gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), TTASFLOAT(val));
	}
}
