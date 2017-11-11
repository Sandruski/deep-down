#ifndef _j1PERFTIMER_H_
#define _j1PERFTIMER_H_

#include "p2Defs.h"

class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //_j1PERFTIMER_H_