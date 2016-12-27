/*
 * model.h
 */
#ifndef MODEL_H_
#define MODEL_H_
#include <time.h>
#include "input.h"

class Model {
public:
	void serve(void);
	void initModel(void);
	void stepPlayer(int fd);
	void ballmovement();
	void preAction();
	void postAction();
	void scorecalc();
	void Avemode();
	void gameset(int i);
};

#endif /* MODEL_H_ */
