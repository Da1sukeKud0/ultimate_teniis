/*
 * model.h
 */
#ifndef MODEL_H_
#define MODEL_H_
#include <time.h>
#include "input.h"

class Model {

public:
	int i;
	void serve(void);
	void initModel(void);
	void stepPlayer(int fd);
	void ballmovement();
	void preAction();
	void postAction();
	void scorecalc(int);
	void gameset(int);
	void gameend();
};

#endif /* MODEL_H_ */
