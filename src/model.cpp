/*
 * model.cpp
 * 入力データInputDataの内容を場面Sceneに反映させる
 * 不公平にならないよう注意が必要
 */

//inputとsceneの連携
#include <iostream>
#include "model.h"
#include "manager.h"
#include "input.h"
using namespace std;

void Model::initModel(void) {
//std::cout << "Init" << std::endl;
	Scene &scene = Manager::getInstance().scene;
	time_t t;
	t = time(NULL);
	localtime_r(&t, &scene.tm);
	scene.c[0] = 0;
	scene.c[1] = 0;
	/*for (Players::iterator i = scene.p.begin(); i != scene.p.end(); ++i) {
	 i->second.curDots = 0;
	 for (int j = 0; j < max_dots; ++j) {
	 i->second.dots[j].visible = 0;
	 }
	 }*/

}

void Model::preAction(void) { // 衝突判定など、判定のみを行う。公平のため、ここで動かしてはいけない
	time_t t;
	Scene &scene = Manager::getInstance().scene;
	t = time(NULL);
	localtime_r(&t, &scene.tm);
}

void Model::postAction(void) { // 全プレイヤーの動作を終えた後に、全体の状況を変えたい場合はここで処理する

}

void Model::stepPlayer(int fd) { // 各プレイヤーの動作を行う。公平のため、ここでは判断を行ってはいけない
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;
	InputData &input = mgr.members[fd].input;
	int id = 0;
	for (Members::iterator i = mgr.members.begin(); i != mgr.members.end();
			++i, ++id) {
		if (i->first == fd)
			break;
	}

	//Player &player = scene.p[id];

	for (int i = 0; i < max_dots; ++i) {
		//player.dots[i].x += (input.right - input.left) * 5;
		//player.dots[i].y += (input.down - input.up) * 5;
	}

	//追記
	for (int i = 0; i < max_dots; ++i) {
		scene.ip.x += (input.right - input.left) * 2;
		scene.ip.y += (input.down - input.up) * 2;
	}

	for (int i = 0; i < max_dots; ++i) {
		scene.ip2.x += (input.right - input.left) * 2;
		scene.ip2.y += (input.down - input.up) * 2;
	}

	if (input.x != (-1)) {
		//player.dots[player.curDots].x = input.x;
		//player.dots[player.curDots].y = input.y;
		//player.dots[player.curDots].visible = 1;
		//player.curDots = (player.curDots + 1) % max_dots;

		//追記
		scene.ip.x = input.x;
		scene.ip.y = input.y;
	}

	if (input.w != (-1)) { //追記
		scene.g.change = input.w;
	}

	if (input.score != (-1)) { //追記1227
		scene.g.getpoint = input.score;
		cout << scene.s.sx << " " << scene.g.getpoint << " " << input.score
				<< endl;
	}

	if (input.key != 0) {
		scene.c[0] = input.key;
	}

	//ゲーム開始時の動作
	if (scene.g.service == 0) {
		scene.ip.y = 364;
		scene.ibs.y = 364;
		if (scene.ip.x < 122) {
			scene.ip.x = 122;
			//ballの初期位置決定

		}
		if (scene.ip.x > 392) {
			scene.ip.x = 392;
			//ballの初期位置決定
		}
		scene.ibs.x = scene.ip.x - 10;
		//打った後にservice == 1になるように
		if (input.w == 1) {
			scene.g.service = 1;
		}
	}

	//得点後の動作 getpointは0でフラット/1でplayer1の得点/2でplayer2の得点
	if (scene.g.getpoint == 1 || scene.g.getpoint == 2) {
		scene.g.service = 0;
		scene.g.flag = 0;

	}
	//std::cout << scene.g.service << "," << scene.g.flag << "," <<scene.ibs.y<<","<<scene.ibs.vy<<std::endl;
}

void Model::serve(void) {

}

void Model::ballmovement() {
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;

	if (scene.g.change == 1) {

		if (scene.g.flag == 0) {

			if (scene.ip.x - 20 <= scene.ibs.x && scene.ibs.x <= scene.ip.x
					&& scene.ip.y - 10 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 10) {
				scene.ibs.vy = 1;
				scene.ibs.vx = (scene.ip.y - scene.ib.y) / 100;
				scene.g.flag += 1;
			} else if (scene.ip.x <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x + 20
					&& scene.ip.y - 10 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 10) {
				scene.ibs.vy = 1;
				scene.ibs.vx = (-(scene.ip.y - scene.ib.y)) / 100;
				scene.g.flag += 1;
			}
		} else if (scene.g.flag == 1) {
			if (scene.ip.x - 5 <= scene.ibs.x && scene.ibs.x <= scene.ip.x
					&& scene.ip.y - 10 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 10) {
				scene.ibs.vy *= -1;
				scene.ibs.vx = scene.ip.y - scene.ib.y;
			} else if (scene.ip.x <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x + 5
					&& scene.ip.y - 10 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 10) {
				scene.ibs.vy *= -1;
				scene.ibs.vx = -(scene.ip.y - scene.ib.y);
			}

		}
	}

	scene.ib.x += scene.ib.vx;

	scene.ibs.y += scene.ibs.vy;
	scene.ibs.x += scene.ibs.vx;
}

//通常得点
void Model::scorecalc() {
	Scene &scene = Manager::getInstance().scene;
	//xの得点
	if (scene.g.getpoint == 1) {

		if (scene.s.sx == 2) {
			scene.s.sx = 3; //通常得点
			scene.g.getpoint = 0;
		}
		if (scene.s.sx == 1) {
			scene.s.sx = 2; //通常得点
			scene.g.getpoint = 0;
		}
		if (scene.s.sx == 0) {
			scene.s.sx = 1; //通常得点
			scene.g.getpoint = 0;
		}

		if (scene.s.sy != 4 && scene.s.sx == 3) { //Ave外
			gameset(1);
		} //通常勝利

		if (scene.s.sy == 3 && scene.s.sy == 3) { //40vs40
			scene.s.sx = 4;
			Avemode();
		}
	}

	//yの得点
	if (scene.g.getpoint == 2) {

		if (scene.s.sy == 2) {
			scene.s.sy = 3; //通常得点
			scene.g.getpoint = 0;
		}
		if (scene.s.sy == 1) {
			scene.s.sy = 2; //通常得点
			scene.g.getpoint = 0;
		}
		if (scene.s.sy == 0) {
			scene.s.sy = 1; //通常得点
			scene.g.getpoint = 0;
		}

		if (scene.s.sx != 4 && scene.s.sy == 3) { //Ave外
			gameset(2);
		} //通常勝利

		if (scene.s.sx == 3 && scene.s.sx == 3) { //40vs40
			scene.s.sy = 4;
			Avemode();
		}
	}
}

void Model::Avemode() {
	Scene &scene = Manager::getInstance().scene;

	if (scene.s.sx == 4 && scene.s.sy == 4) { //双方Ave
		if (scene.g.getpoint == 1 || scene.g.getpoint == 2) {
			scene.s.sx = 3;
			scene.s.sy = 3;
		}
	}

	if (scene.s.sx == 4 || scene.s.sy == 4) { //片方Ave
		if (scene.g.getpoint == 1) {
			gameset(1);
		}
		if (scene.g.getpoint == 2) {
			gameset(2);
		}
	}

}

void Model::gameset(int i) { //gamesetって書いちゃったけど1setとった時の動作＋2set先取完全試合終了時の操作を含む
	Scene &scene = Manager::getInstance().scene;
	if (i == 1) { //xのセット＋１
		++scene.s.setx;

	}
	if (i == 2) { //yのセット＋１
		++scene.s.sety;
	}

	scene.g.getpoint = 0;

	//gameset時の変更とメッセージ
}

