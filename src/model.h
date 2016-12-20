/*
 * model.h
 */
#ifndef MODEL_H_
#define MODEL_H_
#include <time.h>
#include "input.h"

class Model {
public:
	int flag;
	void initModel(void);
	void ballmovement(void);
	void preAction();
	void stepPlayer(int fd);
	void postAction();
	void scorecalc();
	void Avemode();
	void gameset(int i);
};

#endif /* MODEL_H_ */
