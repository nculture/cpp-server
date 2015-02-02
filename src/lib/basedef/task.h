#ifndef BASEDEF_TASK_H
#define BASEDEF_TASK_H

#include "commondef.h"

namespace basedef {

class Task
{
public:
	virtual ~Task() {}
	virtual bool process() = 0;
};

}

#endif