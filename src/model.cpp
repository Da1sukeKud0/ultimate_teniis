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
	flag = 0;
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
		scene.mp.x += (input.right - input.left) * 2;
		scene.mp.y += (input.down - input.up) * 2;
	}

	for (int i = 0; i < max_dots; ++i) {
		scene.mp2.x += (input.right - input.left) * 2;
		scene.mp2.y += (input.down - input.up) * 2;
	}

	if (input.x != (-1)) {
		//player.dots[player.curDots].x = input.x;
		//player.dots[player.curDots].y = input.y;
		//player.dots[player.curDots].visible = 1;
		//player.curDots = (player.curDots + 1) % max_dots;

		//追記
		scene.mp.x = input.x;
		scene.mp.y = input.y;
	}

	if (input.w != (-1)) { //追記
		scene.pic.change = input.w;
	}

	if (input.score != (-1)) { //追記1129
		scene.s.sx = input.score;
	}

	if (input.key != 0) {
		scene.c[0] = input.key;
	}
	//ゲーム開始時の動作
	if (scene.pic.service == 0) {
		scene.mp.y = 364;
		if (scene.mp.x < 122) {
			scene.mp.x = 122;
		}
		if (scene.mp.x > 392) {
			scene.mp.x = 392;
		}
		//打った後にservice == 1になるように
	}

	//得点後の動作
	if (scene.pic.getpointx == 1 || scene.pic.getpointy == 1) {
		scene.pic.service = 0;

	}
}

void Model::ballmovement(void) {
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;
	if (scene.pic.change == 1) {
		if (flag == 0) {
			scene.b.vx = 1;
			flag += 1;
		} else if (flag == 1) {
			scene.b.vx *= -1;
		}
	}
	for (int i = 0; i <= 100; i++) {
		scene.b.x += scene.b.vx;
	}
}

//通常得点
void Model::scorecalc() {
	Scene &scene = Manager::getInstance().scene;
	//xの得点
	if (scene.pic.getpointx == 1) {

		if (scene.s.sx < 2) {
			scene.s.sx++; //通常得点
			scene.pic.getpointx = 0;
			flag = 0;
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
	if (scene.pic.getpointy == 1) {

		if (scene.s.sy < 2) {
			scene.s.sy++; //通常得点
			scene.pic.getpointy = 0;
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
	if (scene.s.sx == 4 || scene.s.sy == 4) { //片方Ave
		if (scene.pic.getpointx == 1) {
			gameset(1);
		}
		if (scene.pic.getpointy == 1) {
			gameset(2);
		}
	}

	if (scene.s.sx == 4 && scene.s.sy == 4) { //双方Ave
		if (scene.pic.getpointx == 1 || scene.pic.getpointy == 1) {
			scene.s.sx = 4;
			scene.s.sy = 4;
		}
	}
}

void Model::gameset(int i) {
	Scene &scene = Manager::getInstance().scene;
	if (i == 1) {
	} //xのセット＋１
	if (i == 2) {
	} //yのセット＋１

	scene.pic.getpointx = 0;
	scene.pic.getpointy = 0;
	//gameset時の変更とメッセージ
}

