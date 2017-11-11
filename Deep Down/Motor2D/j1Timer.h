#ifndef _j1TIMER_H_
#define _j1TIMER_H_

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at;
};

#endif //_j1TIMER_H_