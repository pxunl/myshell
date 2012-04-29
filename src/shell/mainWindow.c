/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  mainWindow.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Tue, March 6, 2012 12:19:02 HKT
 * Description  :  2012-3-6
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "shell.h"


GtkTextIter   iter;
GtkTextIter   start_iter;
GtkTextIter   end_iter;
GtkTextBuffer *buffer;
static gchar  g_buf_cmd[512];
static gint   count = 0;
static void destroy(GtkWidget *widget, gpointer data);

static gboolean virtual_keyboard_drive(GtkWidget *widget, GdkEventKey *event, gpointer data);

int main(int argc, char *argv[])
{
	GtkWidget 		*main_window;
	GtkTextView     *text_view;
	GtkWidget 		*scrolled_window;
	GtkWidget 		*v_box;
	GtkWidget 		*menu_vbox;
	GtkWidget 		*menubar;
	GtkWidget 		*filemenu;
	GtkWidget 		*new;
	GtkWidget 		*file;
	GtkWidget 		*quit;
	GtkWidget 		*sep;
	GtkWidget 		*option;
	GtkWidget 		*edit;
	GtkWidget 		*help;

	GtkAccelGroup *accel_group = NULL;
	
	gtk_init(&argc, &argv);

	memset(g_buf_cmd, '\0', sizeof(g_buf_cmd));

	//Create main window
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	//bing the "destroy event" signal to close the main window
	gtk_signal_connect(GTK_OBJECT(main_window), "destroy", 
			G_CALLBACK(destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "Myshell");
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 730, 533);
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 5);
	GTK_WINDOW(main_window)->allow_shrink = TRUE;

	//vbox for menubar
	menu_vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(main_window), menu_vbox);

	menubar = gtk_menu_bar_new();
	filemenu = gtk_menu_new();

	//create accelrators
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(main_window), accel_group);

	//add menu into menubar
	file = gtk_menu_item_new_with_mnemonic("_File");
	edit = gtk_menu_item_new_with_mnemonic("_Edit");
	option = gtk_menu_item_new_with_mnemonic("_Option");
	help = gtk_menu_item_new_with_mnemonic("_Help");
	new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	/*open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);*/
	quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

	//separator the menu
	sep = gtk_separator_menu_item_new();

	//add CTRL+N to create a new window
	/*gtk_widget_add_accelerator(new, "avtivate", accel_group,*/
			/*GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);*/

	//add CTRL+Q to close this window
	gtk_widget_add_accelerator(quit, "activate", accel_group,
		GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	//add submenu to filemenu
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

	//show menu
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), option);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
	gtk_box_pack_start(GTK_BOX(menu_vbox), menubar, FALSE, FALSE, 3);

	// bing the signal quit to close the window
	g_signal_connect(G_OBJECT(quit), "activate", 
			G_CALLBACK(destroy), NULL);

	//////////////////////////////////////////////
	/*text_view for editing or showing information*/
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	text_view = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(menu_vbox), scrolled_window, TRUE, TRUE, 5);

	buffer = gtk_text_view_get_buffer(text_view);

	gtk_text_buffer_get_end_iter(buffer, &iter);
	
	gtk_text_buffer_insert(buffer, &iter, "pxunl@lnuxp.#", -1);
	/*gtk_text_buffer_get_start_iter(buffer, &start_iter);*/

	gtk_signal_connect(G_OBJECT(text_view), "key-press-event", G_CALLBACK(virtual_keyboard_drive), NULL);
		
	//////////////////////////////////////////////
	gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
	v_box = gtk_vbox_new(FALSE, 5);
	/*gtk_container_add(GTK_CONTAINER(main_window), v_box);*/
	gtk_widget_show_all(main_window);
	gtk_main();
	
	return 0;
}

//typedef  void* gpointer
//close the main window and exit programg
static void destroy(GtkWidget *widget, gpointer data)
{
	g_print("\nended...\n");
	gtk_main_quit();
}

static gboolean virtual_keyboard_drive(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	if (event->keyval == GDK_Return)
	{
		g_buf_cmd[count] = '\0';
		g_print("  --|| %s ||--  ", g_buf_cmd);
		Shell_Main(g_buf_cmd);
		count = 0;
		memset(g_buf_cmd, '\0', sizeof(g_buf_cmd));
	}
	// ignore signal "Alt"
	else if (event->keyval == GDK_Alt_L)
	{
		return FALSE;
	}
	else 
	{
		g_buf_cmd[count++] = event->keyval;
	}
	
	return FALSE;
}
