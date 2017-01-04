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

//コートの角の座標設定
int rux = 0;
int ruy = 0;
int lux = 606;
int luy = 0;
int rdx = 0;
int rdy = 424;
int ldx = 606;
int ldy = 424;

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

	/*
	 //得点判定
	 if (scene.ib.y >= ) {
	 //ボール戻す
	 scorecalc(1);

	 }
	 */
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

	//以下!= (-1)から変更してscorecalcテスト中
	if (input.score1 == 1) { //追記1227
		//xの点数追加時のシミュレーション用の動作　本編では削除
		scorecalc(1);
		cout << "テスト1" << endl;
		cout << scene.s.sx << " " << input.score1 << endl;
	}
	if (input.score2 == 1) { //追記1227
		//yの点数追加時のシミュレーション用の動作　本編では削除
		scorecalc(2);
		cout << "テスト2" << endl;
		cout << scene.s.sy << " " << input.score2 << endl;
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

	}

	if (input.w == 1) {
		scene.g.service = 1;
	}
	//打った後にservice == 1になるように

	//得点後の動作 getpointは0でフラット/1でplayer1の得点/2でplayer2の得点
	/*if (scene.g.getpoint == 1 || scene.g.getpoint == 2) {
	 scene.g.service = 0;
	 scene.g.flag = 0;
	 //仕様変更によりgetpointは使用していないため必要な場合flagについてはどこかに転記
	 }
	 */
	//std::cout << scene.g.service << "," << scene.g.flag << "," <<scene.ibs.y<<","<<scene.ibs.vy<<std::endl;
	//std::cout << scene.ibs.x << "," << scene.ibs.y << std::endl;
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
void Model::scorecalc(int i) {
	Scene &scene = Manager::getInstance().scene;

	//xの得点
	switch (i) {
	case 1:
		if (scene.s.sx == 3) { //40点以降
			if (scene.s.sy == 3) {
				scene.s.sx = 4; //xがAve
				break;
			} else if (scene.s.sy == 4) {
				scene.s.sx = 3; //yのAveに追いついたので40:40に
				scene.s.sy = 3;
				break;
			} else { //yが30点未満なので勝利
				gameset(1);
				break;
			}
		}
		if (scene.s.sx == 4) { //xがAveの状態から勝利
			gameset(1);
			break;
		}
		if (scene.s.sx <= 2) { //通常得点
			scene.s.sx++;
			break;
		}
		cout << "scorecalc呼び出しテスト" << scene.s.sx << endl;
		break;

	case 2:
		if (scene.s.sy == 3) { //40点以降
			if (scene.s.sx == 3) {
				scene.s.sy = 4; //yがAve
				break;
			} else if (scene.s.sx == 4) {
				scene.s.sy = 3; //xのAveに追いついたので40:40に
				scene.s.sx = 3;
				break;
			} else { //xが30点未満なので勝利
				gameset(2);
				break;
			}
		}
		if (scene.s.sy == 4) { //yがAveの状態から勝利
			gameset(2);
			break;
		}
		if (scene.s.sy <= 2) { //通常得点
			scene.s.sy++;
			break;
		}
		cout << "scorecalc呼び出しテスト" << scene.s.sy << endl;
		break;
	}
}

void Model::gameset(int i) { //gamesetって書いちゃったけど1setとった時の動作＋2set先取完全試合終了時の操作を含む
	Scene &scene = Manager::getInstance().scene;

	switch (i) {
	case 1:
		if (scene.s.setx == 1) {
			//x勝利画面
		}
		++scene.s.setx;
		scene.s.sx = scene.s.sy = 0;
		break;

	case 2:
		if (scene.s.setx == 1) {
			//y勝利画面
		}
		++scene.s.sety;
		scene.s.sx = scene.s.sy = 0;
		break;
	}
}

