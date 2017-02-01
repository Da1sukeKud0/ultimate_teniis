/*
 * view.cpp
 * 画面表示を司る関数群
 */
#include <iostream>
#include <cmath>
#include "view.h"
#include "manager.h"
#include "mynetwork.h"
#include "input.h" //追記
#include <string.h>

using namespace std;

#if !GTKMM3
const int ss_divisor = 3; // frames
#endif

MyDrawingArea::MyDrawingArea(BaseObjectType* o,
		const Glib::RefPtr<Gtk::Builder>& g) :
		Gtk::DrawingArea(o) {
	Manager &mgr = Manager::getInstance();
	MySmartphone &smapho = MySmartphone::getInstance();
	mgr.scene.init();
	;
	smapho.open(8888);

#ifdef USE_OPENGL
	gl_config = gdk_gl_config_new_by_mode((GdkGLConfigMode)
			(GDK_GL_MODE_RGBA|GDK_GL_MODE_DEPTH|GDK_GL_MODE_DOUBLE));
	gtk_widget_set_gl_capability(&(o->widget), gl_config, NULL, TRUE,
			GDK_GL_RGBA_TYPE);
#endif
}

void MyDrawingArea::on_realize(void) {
	//	std::cout << "Realized" << std::endl;
	Gtk::DrawingArea::on_realize();
}

#if GTKMM3
bool MyDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cc) {
	Gtk::DrawingArea::on_draw(cc);
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;
#else
	bool MyDrawingArea::on_expose_event(GdkEventExpose* e) {
		Cairo::RefPtr<Cairo::Context> cc =
		this->get_window()->create_cairo_context();
		Gtk::DrawingArea::on_expose_event(e);
		Manager &mgr = Manager::getInstance();
		Scene &scene = mgr.scene;

#endif
	if (!scene.valid) {
#ifdef USE_OPENGL
		GdkGLContext *gl_context = gtk_widget_get_gl_context((GtkWidget *)this->gobj());
		GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable((GtkWidget *)this->gobj());
		gdk_gl_drawable_gl_begin(gl_drawable, gl_context);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (gdk_gl_drawable_is_double_buffered(gl_drawable)) {
			gdk_gl_drawable_swap_buffers(gl_drawable);
		} else {
			glFlush();
		}
		gdk_gl_drawable_gl_end(gl_drawable);
#else
		cc->set_source_rgb(0.8, 0.8, 0.8);
		cc->paint();
#endif
		return true;
	}
	//	std::cout << "Exposed" << std::endl;
	//int ls = fmin(this->get_width() * 0.5f, this->get_height() * 0.5f);
	//int lm = fmin(this->get_width() * 0.4f, this->get_height() * 0.4f);
	//int lh = fmin(this->get_width() * 0.25f, this->get_height() * 0.25f);

#ifdef USE_OPENGL
	int z=ls/30;
	GdkGLContext *gl_context = gtk_widget_get_gl_context((GtkWidget *)this->gobj());
	GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable((GtkWidget *)this->gobj());

	gdk_gl_drawable_gl_begin(gl_drawable, gl_context);
	glViewport(0, 0, this->get_width(), this->get_height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, ls/10, 5.0*ls);
	gluLookAt(0, 3.0*ls, 2.0*ls, 0.0, 0.0, 0.0, 0.0, 9.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	GLfloat position[4]= {-(GLfloat)ls, (GLfloat)ls, (GLfloat)ls, (GLfloat)1};
	GLfloat color[4];
	GLUquadricObj *q;

	glLightfv(GL_LIGHT0, GL_POSITION, position);
//	color[0]=0.0; color[1]=0.0; color[2]=0.0; color[3]=1.0;
//	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	color[0]=0.7; color[1]=0.7; color[2]=0.7; color[3]=1.0;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
//	color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=1.0;
//	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	q = gluNewQuadric();

	//	color[0]=0.2; color[1]=0.2; color[2]=0.2; color[3]=1.0;
	//	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	//	color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=1.0;
	//	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=0.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	color[0]=128.0;
	glMaterialfv(GL_FRONT, GL_SHININESS, color);
	color[0]=0; color[1]=0; color[2]=0; color[3]=1.0;
	glMaterialfv(GL_FRONT, GL_EMISSION, color);

	color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, -z/2, 0);
	glRotated(270, 1, 0, 0);
	gluDisk(q, 0, ls, 120, 10);
	glPopMatrix();

	for(int i=0; i<12; ++i) {
		glPushMatrix();
		glRotated(-30*i+180, 0.0, 1.0, 0.0);
		glTranslated(0, -z/2, lm);
		gluCylinder(q, z, z, ls-lm, 10, 10);
		glPopMatrix();
	}

	color[0]=0.0; color[1]=0.0; color[2]=1.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, z, 0);
	glRotated(-6.0*scene.tm.tm_sec+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, ls, 10, 10);
	glPopMatrix();

	color[0]=0.0; color[1]=1.0; color[2]=0.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, 2*z, 0);
	glRotated(-6.0*(scene.tm.tm_min+scene.tm.tm_sec/60.0)+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, lm, 10, 10);
	glPopMatrix();

	color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, 3*z, 0);
	glRotated(-30.0*(scene.tm.tm_hour+scene.tm.tm_min/60.0)+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, lh, 10, 10);
	glPopMatrix();

#ifdef USE_OPENGLUT
	std::string s("SD Experiments F");
	glColor3f(1, 0, 0);
	glRasterPos3i(-120, 1, 20);
	for(unsigned int i=0; i<s.length(); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s.c_str()[i]);
	}
#endif

	for(Players::iterator p=scene.p.begin(); p!=scene.p.end(); ++p) {
		color[0]=((p->first+1)&1)>0; color[1]=((p->first+1)&2)>0; color[2]=((p->first+1)&4)>0; color[3]=1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		for(int j=0; j<max_dots; ++j) {
			if(p->second.dots[j].visible==1) {
				glPushMatrix();
				glTranslated((p->second.dots[j].x-this->get_width()/2)/2, 0, (p->second.dots[j].y-this->get_height()/2)/2);
				glRotated(270, 1, 0, 0);
				gluDisk(q, z, 2*z, 120, 10);
				glPopMatrix();
			}
		}
	}

	color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glCullFace(GL_FRONT);
	glPushMatrix();
	double roll=atan2(scene.p[0].ax, scene.p[0].az)*180/M_PI;
	double pitch=atan2(scene.p[0].ay, scene.p[0].az)*180/M_PI;
	glRotated(pitch, 関連画像1.0, 0.0, 0.0);
	glRotated(roll, 0.0, 0.0, 1.0);
	gdk_gl_draw_teapot(true, lh/4);
	glPopMatrix();

	if (gdk_gl_drawable_is_double_buffered(gl_drawable)) {
		gdk_gl_drawable_swap_buffers(gl_drawable);
	} else {cc->restore();
		glFlush();
	}
	gdk_gl_drawable_gl_end(gl_drawable);

#else
#if GTKMM3
#else
	MySmartphone &smapho = MySmartphone::getInstance();
	static int frame = 0;
	if ((++frame % ss_divisor) == 0 && smapho.isConnected()
			&& mgr.get_mode() == Manager::Server && mgr.members.size() == 1) {
		Glib::RefPtr<Gdk::Pixmap> pixmap = this->get_snapshot();
		Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(
				(Glib::RefPtr<Gdk::Drawable>) pixmap, 0, 0, this->get_width(),
				this->get_height());
		pixbuf = pixbuf->scale_simple(smapho.get_width(), smapho.get_height(),
				Gdk::INTERP_NEAREST);
		smapho.sendPixbuf((Glib::RefPtr<Gdk::Pixbuf>) pixbuf);
	}
#endif
	/* 文字盤
	 cc->set_source_rgb(0.8, 0.8, 0.8);
	 cc->paint();
	 cc->set_line_width(1.0);
	 cc->set_source_rgb(0, 0, 0);
	 for(int i=0; i<12; ++i){
	 cc->move_to((double)(ls+lm*sin(2.0*M_PI*i/12)), (double)(ls-lm*cos(2.0*M_PI*i/12)));
	 cc->line_to((double)(ls+ls*sin(2.0*M_PI*i/12)), (double)(ls-ls*cos(2.0*M_PI*i/12)));
	 cc->stroke();
	 }*/

	//こっから追記
	if (scene.id == -1) {
		scene.mp.x = 300
				+ (0.5 + (scene.ip.y / 424) * 0.5) * (scene.ip.x - 300);
		scene.mp.y = scene.ip.y;
		scene.mp2.x = 300
				+ (0.5 + (scene.ip2.y / 424) * 0.5) * (scene.ip2.x - 300);
		scene.mp2.y = scene.ip2.y;
		scene.b.x = 300 + (0.5 + (scene.ib.y / 424) * 0.5) * (scene.ib.x - 300);
		scene.b.y = scene.ib.y;
		scene.bs.x = 300
				+ (0.5 + (scene.ibs.y / 424) * 0.5) * (scene.ibs.x - 300);
		scene.bs.y = scene.ibs.y;
	} else if (scene.id == 1) {
		scene.mp.x = 305 + (1 - (scene.ip.y / 424) * 0.5) * (scene.ip.x - 300);
		scene.mp.y =339 -scene.ip.y;
		scene.mp2.x = 305
				+ (1 - (scene.ip2.y / 424) * 0.5) * (scene.ip2.x - 300);
		scene.mp2.y =339 -scene.ip2.y;
		scene.b.x = 305 + (1 - (scene.ib.y / 424) * 0.5) * (scene.ib.x - 300);
		scene.b.y =339 - scene.ib.y;
		scene.bs.x = 305
				+ (1 - (scene.ibs.y / 424) * 0.5) * (scene.ibs.x - 300);
		scene.bs.y =339 -scene.ibs.y;
	} else if (scene.id == 0) {
		scene.mp.x = 300
				+ (0.5 + (scene.ip.y / 424) * 0.5) * (scene.ip.x - 300);
		scene.mp.y = scene.ip.y;
		scene.mp2.x = 300
				+ (0.5 + (scene.ip2.y / 424) * 0.5) * (scene.ip2.x - 300);
		scene.mp2.y = scene.ip2.y;
		scene.b.x = 300 + (0.5 + (scene.ib.y / 424) * 0.5) * (scene.ib.x - 300);
		scene.b.y = scene.ib.y;
		scene.bs.x = 300
				+ (0.5 + (scene.ibs.y / 424) * 0.5) * (scene.ibs.x - 300);
		scene.bs.y = scene.ibs.y;
	}

	/*scene.mp.x = 300 + (0.5 + (scene.ip.y / 424) * 0.5) * (scene.ip.x - 300);
	 scene.mp.y = scene.ip.y;
	 scene.b.x = 300 + (0.5 + (scene.ib.y / 424) * 0.5) * (scene.ib.x - 300);
	 scene.b.y = scene.ib.y;
	 scene.bs.x = 300 + (0.5 + (scene.ibs.y / 424) * 0.5) * (scene.ibs.x - 300);
	 scene.bs.y = scene.ibs.y;
	 */

	cc->save();
	Cairo::RefPtr<Cairo::Surface> court, myplayer, myplayer2, ball;
	court = Cairo::ImageSurface::create_from_png("court.png");
	cc->scale(1.0, 1.0);
	cc->set_source(court, 0, 0);
	cc->paint();
	cc->restore();
	if (scene.id == -1) {
		cc->save();
		if (scene.g.change1 == 0) {
			myplayer = Cairo::ImageSurface::create_from_png("待機1.png");
			cc->scale(1.0, 1.0);
			cc->set_source(myplayer, scene.mp.x, scene.mp.y);
			cc->paint();
		}

		if (scene.g.change1 == 1) {
			if (scene.ip.x <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 5) {
				myplayer = Cairo::ImageSurface::create_from_png("バックハンド1.png");
				cc->scale(1.0, 1.0);
				cc->set_source(myplayer, scene.mp.x, scene.mp.y);
				cc->paint();
			} else if (scene.ip.x - 5 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x) {
				myplayer = Cairo::ImageSurface::create_from_png("フォアハンド1.png");
				cc->scale(1.0, 1.0);
				cc->set_source(myplayer, scene.mp.x, scene.mp.y);
				cc->paint();
			}
		}

		cc->restore();
	} else if (scene.id == 0) {
		cc->save();
		//if (scene.g.change1 == 0) {
		myplayer = Cairo::ImageSurface::create_from_png("待機1.png");
		cc->scale(1.0, 1.0);
		cc->set_source(myplayer, scene.mp.x, scene.mp.y);
		cc->paint();
		//}

		/*if (scene.g.change1 == 1) {
		 if (scene.ip.x <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 5) {
		 myplayer = Cairo::ImageSurface::create_from_png("バックハンド1.png");
		 cc->scale(1.0, 1.0);
		 cc->set_source(myplayer, scene.mp.x, scene.mp.y);
		 cc->paint();
		 } else if (scene.ip.x - 5 <= scene.ibs.x
		 && scene.ibs.x <= scene.ip.x) {
		 myplayer = Cairo::ImageSurface::create_from_png("フォアハンド1.png");
		 cc->scale(1.0, 1.0);
		 cc->set_source(myplayer, scene.mp.x, scene.mp.y);
		 cc->paint();
		 }*/
		//}
		cc->restore();
		cc->save();
		//if (scene.g.change2 == 0) {
		myplayer = Cairo::ImageSurface::create_from_png("待機2.png");
		cc->scale(1.0, 1.0);
		cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
		cc->paint();
		//}

		/*if (scene.g.change2 == 1) {
		 if (scene.ip.x <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 5) {
		 myplayer = Cairo::ImageSurface::create_from_png("バックハンド2.png");
		 cc->scale(0.5, 0.5);
		 cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
		 cc->paint();
		 } else if (scene.ip.x - 5 <= scene.ibs.x
		 && scene.ibs.x <= scene.ip.x) {
		 myplayer = Cairo::ImageSurface::create_from_png("フォアハンド2.png");
		 cc->scale(0.5, 0.5);
		 cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
		 cc->paint();
		 }
		 }*/

		cc->restore();
	} else if (scene.id == 1) {
		cc->save();
	//	if (scene.g.change2 == 0) {
		myplayer = Cairo::ImageSurface::create_from_png("待機2.png");
		cc->scale(1.0, 1.0);
		cc->set_source(myplayer, scene.mp.x, scene.mp.y);
		cc->paint();
		//}

		/*if (scene.g.change2 == 1) {

		if (scene.ip.x <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 5) {
			myplayer = Cairo::ImageSurface::create_from_png("バックハンド2.png");
			cc->scale(1.0, 1.0);
			cc->set_source(myplayer, scene.mp.x, scene.mp.y);
			cc->paint();
		} else if (scene.ip.x - 5 <= scene.ibs.x && scene.ibs.x <= scene.ip.x) {
			myplayer = Cairo::ImageSurface::create_from_png("フォアハンド2.png");
			cc->scale(1.0, 1.0);
			cc->set_source(myplayer, scene.mp.x, scene.mp.y);
			cc->paint();
		}

		}*/


	cc->restore();
	cc->save();
	//if (scene.g.change1 == 0) {
		myplayer = Cairo::ImageSurface::create_from_png("待機1.png");
		cc->scale(1.0, 1.0);
		cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
		cc->paint();
	//}



	/*if (scene.g.change1 == 1) {
		if (scene.ip.x <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 5) {
			myplayer = Cairo::ImageSurface::create_from_png("バックハンド1.png");
			cc->scale(0.5, 0.5);
			cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
			cc->paint();
		} else if (scene.ip.x - 5 <= scene.ibs.x && scene.ibs.x <= scene.ip.x) {
			myplayer = Cairo::ImageSurface::create_from_png("フォアハンド1.png");
			cc->scale(0.5, 0.5);
			cc->set_source(myplayer, scene.mp2.x, scene.mp2.y);
			cc->paint();
		}
	}*/

	cc->restore();
}
//ゲーム開始時のキャラクター選択ぅぅ
/*if (scene.g.charactorselect == 0) {
 cc->save();
 Cairo::RefPtr<Cairo::Surface> chara1, chara2, chara3, chara4; //1枚が200*300の画像として計算
 chara1 = Cairo::ImageSurface::create_from_png("chara1.png");
 cc->scale(1.0, 1.0);
 cc->set_source(chara1, 0, 0);
 cc->paint();
 chara2 = Cairo::ImageSurface::create_from_png("chara2.png");
 cc->scale(1.0, 1.0);
 cc->set_source(chara2, 300, 0);
 cc->paint();
 chara3 = Cairo::ImageSurface::create_from_png("chara3.png");
 cc->scale(1.0, 1.0);
 cc->set_source(chara3, 0, 200);
 cc->paint();
 chara4 = Cairo::ImageSurface::create_from_png("chara4.png");
 cc->scale(1.0, 1.0);
 cc->set_source(chara4, 300, 200);
 cc->paint();
 cc->restore();
 }
 }*/

//ボールの座標計算
cc->save();
ball = Cairo::ImageSurface::create_from_png("ball.png");
cc->scale(1.0, 1.0);
cc->set_source(ball, scene.bs.x, scene.bs.y);
cc->paint();
/*if (scene.g.change == 0) {
 ball = Cairo::ImageSurface::create_from_png("ball.png");
 cc->scale(1.0, 1.0);
 cc->set_source(ball, scene.b.x, scene.b.y);
 cc->paint();
 } else if (scene.g.change == 1) {
 for (int i = 0; i <= 100; i++) {
 ball = Cairo::ImageSurface::create_from_png("ball.png");
 scene.b.x += scene.b.vx;
 scene.b.y -= scene.b.vy;
 cc->scale(1.0, 1.0);
 cc->set_source(ball, scene.b.x, scene.b.y);
 cc->paint();
 }
 }*/

cc->restore();

//得点板
cc->save();
cc->set_source_rgb(0.1, 0.1, 0.1);
cc->set_line_width(2.0);
cc->move_to(8, 8);
cc->line_to(8, 72);
cc->line_to(107, 72);
cc->line_to(107, 8);
cc->line_to(8, 8);
cc->fill();
cc->stroke();

cc->set_source_rgb(255.0, 255.0, 255.0);

cc->set_line_width(2.0);
cc->move_to(8, 8);
cc->line_to(8, 72);
cc->line_to(107, 72);
cc->line_to(107, 8);
cc->line_to(8, 8);
cc->stroke();

cc->move_to(8, 40);
cc->line_to(107, 40);
cc->stroke();

cc->move_to(41, 8);
cc->line_to(41, 72);
cc->stroke();

cc->move_to(74, 8);
cc->line_to(74, 72);
cc->stroke();

cc->set_font_size(22);
cc->move_to(10, 35);
cc->show_text(string("P1"));

cc->set_font_size(22);
cc->move_to(10, 68);
cc->show_text(string("P2"));

//得点表示
if (scene.s.sx == 0) {
	cc->set_font_size(22);
	cc->move_to(43, 35);
	cc->show_text(string("0"));
} else if (scene.s.sx == 1) {
	cc->set_font_size(22);
	cc->move_to(43, 35);
	cc->show_text(string("15"));
} else if (scene.s.sx == 2) {
	cc->set_font_size(22);
	cc->move_to(43, 35);
	cc->show_text(string("30"));
} else if (scene.s.sx == 3) {
	cc->set_font_size(22);
	cc->move_to(43, 35);
	cc->show_text(string("40"));
} else if (scene.s.sx == 4) {
	cc->set_font_size(19);
	cc->move_to(43, 35);
	cc->show_text(string("Ave"));
}

if (scene.s.sy == 0) {
	cc->set_font_size(22);
	cc->move_to(43, 68);
	cc->show_text(string("0"));
} else if (scene.s.sy == 1) {
	cc->set_font_size(22);
	cc->move_to(43, 68);
	cc->show_text(string("15"));
} else if (scene.s.sy == 2) {
	cc->set_font_size(22);
	cc->move_to(43, 68);
	cc->show_text(string("30"));
} else if (scene.s.sy == 3) {
	cc->set_font_size(22);
	cc->move_to(43, 68);
	cc->show_text(string("40"));
} else if (scene.s.sy == 4) {
	cc->set_font_size(19);
	cc->move_to(43, 68);
	cc->show_text(string("Ave"));
}

//セット数表示
if (scene.s.setx == 0) {
//cout << "関数実行テスト" << endl;
	cc->set_font_size(22);
	cc->move_to(76, 35);
	cc->show_text(string("0"));
} else if (scene.s.setx == 1) {
	cc->set_font_size(22);
	cc->move_to(76, 35);
	cc->show_text(string("1"));
} else if (scene.s.setx == 2) {
	cc->set_font_size(19);
	cc->move_to(76, 35);
	cc->show_text(string("win"));
}

if (scene.s.sety == 0) {
	cc->set_font_size(22);
	cc->move_to(76, 68);
	cc->show_text(string("0"));
} else if (scene.s.sety == 1) {
	cc->set_font_size(22);
	cc->move_to(76, 68);
	cc->show_text(string("1"));
} else if (scene.s.sety == 2) {
	cc->set_font_size(19);
	cc->move_to(76, 68);
	cc->show_text(string("win"));
}
cc->restore();

//勝利時の画面表示
cc->save();
Cairo::RefPtr<Cairo::ImageSurface> win;
if (scene.g.win == 1) {
	win = Cairo::ImageSurface::create_from_png("win1.png");
	cc->scale(1.0, 1.0);
	cc->set_source(win, 0, 100);
	cc->paint();
}
if (scene.g.win == 2) {
	win = Cairo::ImageSurface::create_from_png("win2.png");
	cc->scale(1.0, 1.0);
	cc->set_source(win, 0, 100);
	cc->paint();
}
cc->restore();

//追記終わり

/*針・点
 cc->set_font_size(16);
 cc->move_to((double)(ls+ls*sin(2.0*M_PI*scene.tm.tm_sec/60)),
 (double)(ls-ls*cos(2.0*M_PI*scene.tm.tm_sec/60)));
 cc->show_text(std::string(scene.c).c_str());
 cc->move_to(4, 20);

 branch 'master' of ssh://sdex@www.comp.sd.keio.ac.jp/share/home/sdex/2016/team4.git

 if(scene.id==-1){
 cc->show_text(std::string("Standalone"));
 }else if(scene.id==0){
 cc->show_text(std::string("Server"));
 }else{
 cc->show_text(std::string("Client #")+std::to_string(scene.id));
 }

 cc->set_line_width(3.0); // second hand
 cc->set_source_rgb(0.0, 0.0, 1.0);
 cc->move_to(ls, ls);
 cc->line_to((double)(ls+ls*sin(2.0*M_PI*scene.tm.tm_sec/60)),
 (double)(ls-ls*cos(2.0*M_PI*scene.tm.tm_sec/60)));
 cc->stroke();

 cc->set_line_width(8.0); // minute hand
 cc->set_source_rgb(0.0, 1.0, 0.0);
 cc->move_to(ls, ls);
 cc->line_to((double)(ls+lm*sin(2.0*M_PI*(scene.tm.tm_min/60.0+scene.tm.tm_sec/3600.0))),
 (double)(ls-lm*cos(2.0*M_PI*(scene.tm.tm_min/60.0+scene.tm.tm_sec/3600.0))));
 cc->stroke();

 cc->set_line_width(10.0); // hour hand
 cc->set_source_rgb(1.0, 0.0, 0.0);
 cc->move_to(ls, ls);
 cc->line_to((double)(ls+lh*sin(2.0*M_PI*(scene.tm.tm_hour/12.0+scene.tm.tm_min/720.0))),
 (double)(ls-lh*cos(2.0*M_PI*(scene.tm.tm_hour/12.0+scene.tm.tm_min/720.0))));
 cc->stroke();

 cc->set_line_width(5.0);
 cc->set_source_rgb(1.0, 0.0, 0.0);

 for(Players::iterator p=scene.p.begin(); p!=scene.p.end(); ++p){
 cc->set_source_rgb(((p->first+1)&1)>0, ((p->first+1)&2)>0, ((p->first+1)&4)>0);
 for(int j=0; j<max_dots; ++j){
 if(p->second.dots[j].visible==1){
 cc->arc((double)p->second.dots[j].x, (double)p->second.dots[j].y,
 5.0, 0.0, (double)(2.0*M_PI));
 cc->stroke();
 }
 }
 }
 */

#endif
scene.valid = false;
return true;

}

void MyDrawingArea::update() {
this->queue_draw();
}

// PressイベントとReleaseイベントの両方を見ることで
// 押し続けている状態を把握できるようにできる
bool MyDrawingArea::on_key_press_event(GdkEventKey* k) {
Input &input = Input::getInstance();
input.set_key(k);
return false;
}

bool MyDrawingArea::on_key_release_event(GdkEventKey* k) {
Input &input = Input::getInstance();
input.reset_key(k);
return true;
}

bool MyDrawingArea::on_button_press_event(GdkEventButton* event) {
Input &input = Input::getInstance();
input.set_input(event->x, event->y);
cout << event->x << "," << event->y << endl;
return true;
}

MyImageMenuItem::MyImageMenuItem(BaseObjectType* o,
	const Glib::RefPtr<Gtk::Builder>& g) :
	Gtk::ImageMenuItem(o) {
menuId = -1;
}

void MyImageMenuItem::on_activate(void) {
Gtk::ImageMenuItem::on_activate();

Manager &mgr = Manager::getInstance();
MyNetwork &net = MyNetwork::getInstance();
ViewManager &vmr = ViewManager::getInstance();

switch (menuId) {
case 0:
	vmr.menu[0]->set_sensitive(false);
	vmr.menu[1]->set_sensitive(true);
	vmr.menu[2]->set_sensitive(false);
	vmr.menu[4]->set_sensitive(false);
	mgr.set_state(Manager::Run);
	vmr.push(std::string("Run"));
	switch (mgr.get_mode()) {
	case Manager::Standalone:
		mgr.init_objects();
		mgr.startStandaloneTick(vmr.name->get_text());
		break;
	case Manager::Server:
		mgr.init_objects();
		net.runServer();
		break;
	case Manager::Client:
		net.runClient();
		break;
	}
	break;
case 1:
	vmr.menu[0]->set_sensitive(true);
	vmr.menu[1]->set_sensitive(false);
	vmr.menu[2]->set_sensitive(true);
	vmr.menu[4]->set_sensitive(true);
	mgr.set_state(Manager::Stop);
	vmr.push(std::string("Stop"));
	switch (mgr.get_mode()) {
	case Manager::Server:
		net.stopServer();
		break;
	case Manager::Client:
		net.stopClient();
		break;
	default:
		break;
	}
	break;
case 2:
	vmr.subWindow->show();
	break;
case 3:
	vmr.chooser->show();
	break;
case 4:
	net.disconnect();
	net.closeServer();
	exit(0);
}
}

MyStatusbar::MyStatusbar(BaseObjectType* o, const Glib::RefPtr<Gtk::Builder>& g) :
	Gtk::Statusbar(o) {
statusId = 0;
}

void MyStatusbar::pushTemp(std::string s) {
push(s, statusId);
sigc::slot<bool> slot = sigc::bind(sigc::mem_fun(*this, &MyStatusbar::erase),
		statusId);
Glib::signal_timeout().connect(slot, 5000);
statusId++;
}

bool MyStatusbar::erase(int i) {
pop(i);
return false;
}

void ViewManager::subCancel(void) {
chooser->hide();
}

void ViewManager::subSend(void) {
MySmartphone &smapho = MySmartphone::getInstance();
if (smapho.isConnected()) {
	smapho.sendImage((const char *) (chooser->get_filename().c_str()));
}
chooser->hide();
}

void ViewManager::subHide(void) {
Manager &mgr = Manager::getInstance();
MyNetwork &net = MyNetwork::getInstance();

if (server->get_active() && mgr.get_mode() != Manager::Server) {
	net.disconnect();
	if (net.startServer(std::atoi(sport->get_text().c_str()),
			name->get_text().c_str())) {
		mgr.set_mode(Manager::Server);
	} else {
		mgr.set_mode(Manager::Standalone);
	}
} else if (client->get_active() && mgr.get_mode() != Manager::Client) {
	net.closeServer();

	if (net.connectClient(cip->get_text().c_str(),
			std::atoi(cport->get_text().c_str()), name->get_text().c_str())) {
		mgr.set_mode(Manager::Client);
	} else {
		mgr.set_mode(Manager::Standalone);
	}
} else {
	net.closeServer();
	net.disconnect();
	mgr.set_mode(Manager::Standalone);
}
switch (mgr.get_mode()) {
case Manager::Standalone:
	standalone->set_active();
	break;
case Manager::Server:
	server->set_active();
	break;
case Manager::Client:
	client->set_active();
	break;
}
subWindow->hide();
}

Gtk::Window *ViewManager::init(Glib::RefPtr<Gtk::Builder> builder) {
builder->get_widget("window1", mainWindow);
builder->get_widget("window2", subWindow);
builder->get_widget("window3", chooser);
builder->get_widget("button1", ok);
ok->signal_clicked().connect(sigc::mem_fun0(*this, &ViewManager::subHide));
builder->get_widget("button2", ok);
ok->signal_clicked().connect(sigc::mem_fun0(*this, &ViewManager::subCancel));
builder->get_widget("button3", ok);
ok->signal_clicked().connect(sigc::mem_fun0(*this, &ViewManager::subSend));
builder->get_widget("sip", sip);
builder->get_widget("sport", sport);
builder->get_widget("cip", cip);
builder->get_widget("cport", cport);
builder->get_widget("name", name);
builder->get_widget("standalone", standalone);
builder->get_widget("server", server);
builder->get_widget("client", client);
builder->get_widget_derived("drawingarea1", drawingArea);
builder->get_widget_derived("statusbar1", statusbar);
builder->get_widget_derived("Start", menu[0]);
builder->get_widget_derived("Stop", menu[1]);
menu[1]->set_sensitive(false);
builder->get_widget_derived("SetMode", menu[2]);
builder->get_widget_derived("SendImage", menu[3]);
menu[3]->set_sensitive(false);
builder->get_widget_derived("Quit", menu[4]);
for (int i = 0; i < 5; ++i) {
	menu[i]->menuId = i;
}
return mainWindow;
}
