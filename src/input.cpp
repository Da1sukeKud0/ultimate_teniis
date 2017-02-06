/*
 * input.cpp
 * InputDataをパケットに詰め込むpack関数と、パケットからメンバに取り込むreceive関数
 * 各種入力デバイスからのイベントを、InputDataへ反映させる方法を定める関数
 *dayo
 */

//キーボード上との対応をcaseで取る
#include "input.h"
#include "view.h"
#include "mysmartphone.h"
#include <iostream>
void Input::receiveInput(char *tmp, InputData &data) { // サーバとして、クライアントの入力を受け取る
	data = *(InputData *) tmp;
}

char *Input::packInput(int &len) {
	char *tmp = buffer;
	*(InputData *) tmp = input;
	tmp += sizeof(InputData);
	clearInput();
	len = tmp - buffer;
	if (len > max_inputlen) {
		std::cout << "Input size exceeds max_msglen." << std::endl;
	}
	return buffer;
}

void Input::clearInput(void) {
	input.x = input.y = input.score1 = input.score2 = -1;
//	input.ax=input.ay=input.az=0;
}

void Input::set_input(int argx, int argy) {
	input.x = argx;
	input.y = argy;
}

void Input::set_SmaphoInput(int argx, int argy) {
	ViewManager &vmr = ViewManager::getInstance();
	MySmartphone &smapho = MySmartphone::getInstance();
	set_input(vmr.get_width() * argx / smapho.get_width(),
			vmr.get_height() * argy / smapho.get_height());
}

void Input::set_angle(double argax, double argay, double argaz) {
	input.ax = argax;
	input.ay = argay;
	input.az = argaz;
}

void Input::set_key(GdkEventKey* k) {
#ifdef INPUT_VERBOSE
	std::cout << "Pressed " << k->keyval << std::endl;
#endif
	switch (k->keyval) {
	case GDK_KEY_Up:
		input.up = 1;
		break;
	case GDK_KEY_Down:
		input.down = 1;
		break;
	case GDK_KEY_Left:
		input.left = 1;
		break;
	case GDK_KEY_Right:
		input.right = 1;
		break;
	case GDK_KEY_f:
		input.w = 1;
		break;
	case GDK_KEY_e: //追記1129
		input.score1 = 1; //+=1を=1に変更　1227
		break;
	case GDK_KEY_d: //追記1129
		input.score2 = 1; //+=1を=1に変更　1227
		break;
	case GDK_KEY_w: //追記1129
		input.a = 1; //+=1を=1に変更　1227
		break;
	case GDK_KEY_a: //追記1129
		input.f = 1; //+=1を=1に変更　1227
		break;
	case GDK_KEY_space:
		input.space = 1;
		break;
	case GDK_KEY_1:
		input.key1 = 1;
		break;
	case GDK_KEY_2:
		input.key2 = 1;
		break;
	case GDK_KEY_3:
		input.key3 = 1;
		break;
	case GDK_KEY_4:
		input.key4 = 1;
		break;
	default:
		if (GDK_KEY_A <= k->keyval && k->keyval <= GDK_KEY_z) {
			input.key = k->keyval;
		}
		break;
	}
	return;
}

void Input::reset_key(GdkEventKey* k) {
#ifdef INPUT_VERBOSE
	std::cout << "Released " << k->keyval << std::endl;
#endif
	switch (k->keyval) {
	case GDK_KEY_Up:
		input.up = 0;
		break;
	case GDK_KEY_Down:
		input.down = 0;
		break;
	case GDK_KEY_Left:
		input.left = 0;
		break;
	case GDK_KEY_Right:
		input.right = 0;
		break;
	case GDK_KEY_f:
		input.w = 0;
		break;
	case GDK_KEY_space:
		input.space = 0;
		break;

	case GDK_KEY_e: //追記1129
		input.score1 = 0; //+=0を=0に変更 1227
		break;
	case GDK_KEY_d: //追記1129
		input.score2 = 0; //+=0を=0に変更 1227
		break;

	case GDK_KEY_w: //追記1129
		input.a = 0; //+=1を=1に変更　1227
		break;
	case GDK_KEY_a: //追記1129
		input.f = 0; //+=1を=1に変更　1227
		break;
	case GDK_KEY_1:
		input.key1 = 0;
		break;
	case GDK_KEY_2:
		input.key2 = 0;
		break;
	case GDK_KEY_3:
		input.key3 = 0;
		break;
	case GDK_KEY_4:
		input.key4 = 0;
		break;
	default:
		if (GDK_KEY_A <= k->keyval && k->keyval <= GDK_KEY_z) {
			input.key = 0;
		}
		break;
	}
	return;
}
