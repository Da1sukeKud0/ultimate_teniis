/*
 * main.cpp
 * 使用するライブラリを初期化し、ユーザインタフェース定義を読み込み、制御をgtkmmへと移す
 */
#include <iostream>
#include <gtkmm.h>
#include "view.h"
#include "manager.h"

#define UI_FILE "glade.ui"

int main(int argc, char *argv[]) {
	Gtk::Window *w;
	Gtk::Main kit(argc, argv);
#ifdef USE_OPENGL
	gdk_gl_init(&argc, &argv);
#ifdef USE_OPENGLUT
	glutInit(&argc, argv);
#endif
#endif
	Manager &mgr = Manager::getInstance();
	ViewManager &vmr=ViewManager::getInstance();
	mgr.init_status();
	Glib::RefPtr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_file(UI_FILE);
	} catch (const Glib::FileError &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	w=vmr.init(builder);
	kit.run(*w);
	return 0;
}
