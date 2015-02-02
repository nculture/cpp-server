#ifndef BASEDEF_TASKS_H
#define BASEDEF_TASKS_H

#include "commondef.h"
#include "task.h"

namespace basedef {

class Tasks
{
public:
	Tasks();
	~Tasks();

	void add(Task* pTask);
	bool cancel(Task* pTask);
	void process();

private:
	typedef std::vector<basedef::Task*> Container;
	Container container_;
};

}

#endif