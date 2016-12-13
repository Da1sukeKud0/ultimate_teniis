/*
 * input.h
 * 受け付ける入力データを定義するInputDataと、関連するInputクラス
 */

//キーボード上の入力に対応する変数の定義
#ifndef INPUT_H_
#define INPUT_H_

// #define INPUT_VERBOSE
#include <vector>
#include <map>
#include <gtkmm.h>

const int max_inputlen = 100;

struct InputData { // 要素数を可変にしたい場合は、packInputとreceiveInputを注意深く書き直す必要がある
	int up, down, left, right;
	int x, y;
	int key;
	int space;
	int score;
	double ax, ay, az;
	int w,w0,w1;
};

class Input {
public:
	static Input& getInstance() {
		static Input instance;
		return instance;
	}
	void receiveInput(char *, InputData &data);
	char *packInput(int &);
	void checkInput(void);
	void clearInput(void);
	void set_key(GdkEventKey* k);
	void reset_key(GdkEventKey* k);
	void set_input(int argx, int argy);
	void set_SmaphoInput(int argx, int argy);
	void set_angle(double, double, double);
	//void player_move(double movex,double movey); //追記
	InputData input;
private:
	Input(){
		clearInput();
	}
	Input(Input&);
	void operator =(Input&);
	char buffer[max_inputlen];
};

#endif
