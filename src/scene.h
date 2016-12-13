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

struct Player2{
	double x,y;
};

struct score {//追記1129
	int sx, sy;
};

struct game {
	int change,service,getpoint;
};

struct ball{
	double x, y;
	double vx, vy;
};

struct ballshadow{
	int x,y;
	int ax,ay;
};

struct imageball{

};

struct imageballshadow{

};

struct imageplayer{

};

struct imageplayer2{

};

typedef std::map<int, Player> Players;

class Scene {
public:
	char c[2];
	struct tm tm;
	game pic;
	Players p;
	Player2 mp2;
	Player mp;
	score s;
	ball b;
	ballshadow bs;
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
