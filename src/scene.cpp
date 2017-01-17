/*
 * scene.cpp
 * Sceneをパケットに詰め込むpack関数と、パケットからメンバに取り込むreceive関数
 */
#include "scene.h"
#include <iostream>

void Scene::receiveScene(char *tmp) {
	int size, j;
	if (valid)
		return;
	c[0] = *(char *) tmp;
	tmp += sizeof(char);
	c[1] = *(char *) tmp;
	tmp += sizeof(char);
	id = *(char *) tmp;
	tmp += sizeof(char);
	tm = *(struct tm *) tmp;
	tmp += sizeof(struct tm);
	//追記
	mp = *(struct Player *) tmp;
	tmp += sizeof(struct Player);
	mp2 = *(struct Player2 *) tmp;
	tmp += sizeof(struct Player2);
	s = *(struct score *) tmp;
	tmp += sizeof(struct score);
	b = *(struct ball *) tmp;
	tmp += sizeof(struct ball);
	bs = *(struct ballshadow *) tmp;
	tmp += sizeof(struct ballshadow);
	ib = *(struct imageball *) tmp;
	tmp += sizeof(struct imageball);
	ibs = *(struct imageballshadow *) tmp;
	tmp += sizeof(struct imageballshadow);
	ip = *(struct imageplayer *) tmp;
	tmp += sizeof(struct imageplayer);
	ip2 = *(struct imageplayer2 *) tmp;
	tmp += sizeof(struct imageplayer2);

	p.clear(); // まずコンテナを空にして、
	size = *(char *) tmp;
	tmp += sizeof(char); // 可変長のデータの個数を受け取り
	for (int i = 0; i < size; ++i) { // その数だけ繰り返し、コンテナに格納する
		j = *(char *) tmp;
		tmp += sizeof(char);
		p[j] = *(Player *) tmp;
		tmp += sizeof(Player);

	}
	valid = true;
}

char *Scene::packScene(int &len) {
	char *tmp = buffer;
	*(char *) tmp = c[0];
	tmp += sizeof(char);
	*(char *) tmp = c[1];
	tmp += sizeof(char);
	*(char *) tmp = id;
	tmp += sizeof(char);
	*(struct tm *) tmp = tm;
	tmp += sizeof(struct tm);
	//追記
	*(struct Player *) tmp = mp;
	tmp += sizeof(struct Player);
	*(struct Player2 *) tmp = mp2 ;
	tmp += sizeof(struct Player2);
	*(struct score *) tmp = s;
	tmp += sizeof(struct score);
	*(struct ball *) tmp = b;
	tmp += sizeof(struct ball);
	*(struct ballshadow *) tmp = bs;
	tmp += sizeof(struct ballshadow);
	*(struct imageball *) tmp = ib;
	tmp += sizeof(struct imageball);
	*(struct imageballshadow *) tmp = ibs;
	tmp += sizeof(struct imageballshadow);
	*(struct imageplayer *) tmp = ip;
	tmp += sizeof(struct imageplayer);
	*(struct imageplayer2 *) tmp = ip2;
	tmp += sizeof(struct imageplayer2);

	*(char *) tmp = p.size();
	tmp += sizeof(char); // 可変長のデータは、まず個数を送る
	for (Players::iterator i = p.begin(); i != p.end(); ++i) {
		*(char *) tmp = i->first;
		tmp += sizeof(char);
		*(Player *) tmp = i->second;
		tmp += sizeof(Player);
	}
	len = tmp - buffer;
	if (len > max_scenelen) {
		std::cout << "Scene size exceeds max_msglen." << std::endl;
	}
	return buffer;
}

void Scene::init(void) {
	c[0] = c[1] = 0;
	id = 0;
	//値の初期化を行う場合この辺で定義
	p.clear();

	valid = false;
	g.flag = 0;
	g.change = 0;
	g.service = 0;
	g.getpoint = 0;
	g.getset1 = 0;
	g.getset2 = 0;
	g.gameset = 0;
	g.charactorselect = 0;
	g.cha1 = 0;
	g.cha2 = 0;
	g.cha3 = 0;
	g.cha4 = 0;
	g.win = 0;
	mp.x = 0;
	mp.y = 0;
	mp2.x = 0;
	mp2.y = 0;
	s.sx = 0; //追記1129
	s.sy = 0; //追記1129
	s.setx = 0;
	s.sety = 0;
	b.x = 0;
	b.y = 0;
	b.vx = 0;
	b.vy = 0;
	bs.x = 0;
	bs.y = 0;
	bs.vx = 0;
	bs.vy = 0;
	ib.x = 0;
	ib.y = 0;
	ib.vx = 0;
	ib.vy = 0;
	ibs.x = 0;
	ibs.y = 0;
	ibs.vx = 0;
	ibs.vy = 0;
	ip.x = 0;
	ip.y = 0;
	ip.vx = 0;
	ip.vy = 0;
	ip2.x = 0;
	ip2.y = 0;
	ip2.vx = 0;
	ip2.vy = 0;

}
