/*
 * scene.h
 *
 *  Created on: 2016/10/18
 *      Author: sdjikken
 */
/*
 * scene.h
 * 表示するデータを定義するSceneクラス
 */
#ifndef SCENE_H_
#define SCENE_H_
#include <gtkmm.h>

const int max_scenelen = 1000;

struct Dot {
	int x, y, visible;
};

const int max_dots = 5;
struct Player { // 要素数を可変にしたい場合は、packSceneとreceiveSceneを注意深く書き直す必要がある
	char name[20];
	int x, y;
	double ax, ay, az;
	Dot dots[max_dots];
	int curDots;
};

typedef std::map<int, Player> Players;

class Scene {
public:
	char c[2];  //ここから　viewに送られ表示される要素　個人：Player構造体　全体:tm,p,id
	struct tm tm;
	Players p;
	int id;     //ここまで
	void receiveScene(char *);
	char *packScene(int &len);
	bool valid;
	void init();
	Scene(){init();};
private:
	char buffer[max_scenelen];
};

#endif

