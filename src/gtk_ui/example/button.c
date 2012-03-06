/* 本例的主要目的是在窗口中显示一个按钮,
 * 并且单击按钮退出程序
 */
#include<gtk/gtk.h>

void cb_button(GtkWidget *widget, gpointer data)
{// 按钮"button"的回调函数
	gtk_main_quit();
}

int main(int argc, char *argv[])
{
	GtkWidget *main_window; //主窗口对象
	GtkWidget *button;
	//将要放置到主窗口中的按钮对
	gtk_init(&argc, &argv);
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window),"Hello,ubuntu2!");
	//设置窗口的默认大小(宽 200,高度 50)
	gtk_window_set_default_size(GTK_WINDOW(main_window),200,50);
	button = gtk_button_new_with_label("退出程序");
	gtk_container_add(GTK_CONTAINER(main_window), button);
	//为"button"连接“单击事件”要调用的回调函数
	g_signal_connect(G_OBJECT(button), "clicked",G_CALLBACK(cb_button),NULL);
	g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(cb_button), NULL);

	gtk_widget_show(button);
	gtk_widget_show(main_window);
	//上边的两句可以合为 gtk_widget_show_all(window)
	gtk_main();
	return 0;
}

