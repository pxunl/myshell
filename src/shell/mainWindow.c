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
#include <unistd.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "shell.h"
#include "valuelib.h"

#define BUF_SIZ   2048

static GtkTextIter   iter;
static GtkTextBuffer *buffer;
static GtkTextIter   start_iter;
static GtkTextIter   end_iter;
static gchar  g_buf_cmd[512];
static gint   count = 0;
static gint   apipe[2] = {0,1};
static gchar  buf[BUFSIZ];
static guint  clear_count = 0;

extern char **environ;
void initialize();

static void destroy(GtkWidget *widget, gpointer data);
static gboolean virtual_keyboard_drive(GtkWidget *widget, GdkEventKey *event, gpointer data);

int main(int argc, char *argv[])
{
	GtkWidget 		*main_window;
	GtkWidget 	    *text_view;
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
	GtkAccelGroup   *accel_group = NULL;
	
	gtk_init(&argc, &argv);
	initialize();
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
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

	sep = gtk_separator_menu_item_new();

	//add CTRL+N to create a new window
	/*gtk_widget_add_accelerator(new, "avtivate", accel_group,*/
			/*GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);*/

	//CTRL+Q to close this window
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

	g_signal_connect(G_OBJECT(quit), "activate", 
			G_CALLBACK(destroy), NULL);

	//////////////////////////////////////////////
	/*text_view for editing or showing information*/
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	text_view = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(menu_vbox), scrolled_window, TRUE, TRUE, 5);
	buffer = gtk_text_view_get_buffer((GtkTextView *)text_view);
	
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert(buffer, &iter, "pxunl@lnuxp.#", -1);

	//alway listenning key-press-event to catching command line from gtk_text_view
	gtk_signal_connect((GtkObject *)text_view, "key-press-event", G_CALLBACK(virtual_keyboard_drive), NULL);
		
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

/**
 * @virtual_keyboard_drive : process keys from keyboard in gtk_text_view,
 * 							 call function Shell_Main and give command_string to it when pressing 'enter'
 * @data: NULL
 * @return FALSE Always!
 */
static gboolean virtual_keyboard_drive(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	
	glong len = 0;
	guint key = event->keyval;
	if (key == GDK_Return)
	{
		g_buf_cmd[count] = '\0';

		/* *debug */
		/*g_print("  --|| %s ||-- \n ", g_buf_cmd);*/
		/*g_print("  --|| %d ||--  ",count);*/
	
		Shell_Main(g_buf_cmd);

		/* get  object information from stdout*/
		if ((len = read(apipe[0], buf, BUF_SIZ)) == -1)
		{
			g_print("error,when reading from pipe\n");
			exit(1);
		}

		/* initialize buffers */
		count = 0;
		memset(g_buf_cmd, '\0', sizeof(g_buf_cmd));
		gtk_text_buffer_get_end_iter(buffer, &iter);
		gtk_text_buffer_insert(buffer, &iter, buf, -1);
		memset(buf, '\0', BUF_SIZ);

		/* clear screan */
		if ((clear_count++) > 10) 
		{
			gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);
			gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
			clear_count = 0;
		}
		gtk_text_buffer_get_end_iter(buffer, &iter);
		gtk_text_buffer_insert(buffer, &iter, "\npxunl@lnuxp.#", -1);
	}

	/*delete backward */
	else if (key == GDK_BackSpace) 
	{
		if (count > 0) 
		{
			g_buf_cmd[count-1] = '\0';
			count--;
		}
		return FALSE;
	}
	
	/* ignore some keys signals */
	else if ((key == GDK_Left) 
			|| (key == GDK_Alt_L)
			|| (key == GDK_Right)
			|| (key == GDK_Down)
			|| (key == GDK_Up)
			|| (key == GDK_Tab)
			|| (key == GDK_Shift_L)
			|| (key == GDK_Shift_R)
			|| (key == GDK_Caps_Lock)
			|| (key == GDK_Control_L))
	{
		return FALSE;
	}
	else 
	{
		g_buf_cmd[count++] = key;
	}
	
	return FALSE;
}

/**
 * @purpose:initialize shell
 * @return:nothing
 */
void initialize()
{
	memset(buf, '\0', BUF_SIZ);
	memset(g_buf_cmd, '\0', sizeof(g_buf_cmd));
	Env_To_Table(environ);
	if (pipe(apipe) == -1) 
	{
		g_print("couldn't make pipe");
		exit(1);
	}

	/* dup stdout to apipe[1] to get ouput for gtk_text_view */
	close(1);
	dup(apipe[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

