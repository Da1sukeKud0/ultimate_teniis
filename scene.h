/*
 * scene.h
 * 表示するデータを定義するSceneクラス
 */
#ifndef SCENE_H_
#define SCENE_H_
#include <gtkmm.h>

const int max_scenelen = 1000;


struct Player { // 要素数を可変にしたい場合は、packSceneとreceiveSceneを注意深く書き直す必要がある

	char name;
	int x, y; //キャラの座標
	int ballspeed; //キャラ固有の打球球速
	int runspeed; //キャラの移動速度
	int spin; //キャラ固有のスピン量
	int technique; //必殺技に関する設定

	int metor = 0; //maxで必殺技使用可

};

struct background{
	void score(){
		int a=15,b=30,c=40;
		char d;
	}
};

struct ball{
	int x,y; //球の位置
	int vx,vy; //球速
	int sy; //球の位置(影)　高さの表現
	int svy; //球の位置(影)の速度
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
