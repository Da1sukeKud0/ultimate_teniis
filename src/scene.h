/*
 * scene.h
 * 表示するデータを定義するSceneクラス
 */
#ifndef SCENE_H_
#define SCENE_H_
#include <gtkmm.h>
const int max_scenelen = 1000;

const int max_dots = 5;
struct Player { // 要素数を可変にしたい場合は、packSceneとreceiveSceneを注意深く書き直す必要がある
	double x, y;
};

struct Player2 {
	double x, y;
};

struct score {
	int sx, sy ,setx ,sety;
};

struct game {
	int charactorselect, change, service, getpoint, getset1, getset2, gameset,
			cha1, cha2, cha3, cha4,serve,flag,win;
};

struct ball {
	double x, y;
	double vx, vy;
};

struct ballshadow {
	double x, y;
	double vx, vy;
};

struct imageball {
	double x, y;
	double vx, vy;
};

struct imageballshadow {
	double x, y;
	double vx, vy;
};

struct imageplayer {
	double x, y;
	double vx, vy;
};

struct imageplayer2 {
	double x, y;
	double vx, vy;
};

typedef std::map<int, Player> Players;

class Scene {
public:
	char c[2];
	struct tm tm;
	game g;
	Players p;
	Player2 mp2;
	Player mp;
	score s;
	ball b;
	ballshadow bs;
	imageball ib;
	imageballshadow ibs;
	imageplayer ip;
	imageplayer2 ip2;
	int id;
	void receiveScene(char *);
	char *packScene(int &len);
	bool valid;
	void init();
	Scene() {
		init();
	}
	;

private:
	char buffer[max_scenelen];
};

#endif
