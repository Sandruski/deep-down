#ifndef __PATH_H__
#define __PATH_H__

#include "p2Point.h"
#define MAX_STEPS 25

struct Animation;

struct Step
{
	uint frames = 1;
	fPoint speed;
	Animation* animation = nullptr;
};

class Path
{
public:
	bool loop = true;
	Step steps[MAX_STEPS];
	fPoint accumulated_speed = { 0.0f, 0.0f };
	fPoint hi = { 0.0f, 0.0f };

	uint count = 0;
public:
	uint current_frame = 0;
	uint last_step = 0;
	int hi1 = 0;
	int path1_finished = 0;
public:

	void PushBack(fPoint speed, uint frames, Animation* animation = nullptr)
	{
		steps[last_step].animation = animation;
		steps[last_step].frames = frames;
		steps[last_step++].speed = speed;
	}

	iPoint GetCurrentPosition(Animation** current_animation = nullptr)
	{
		current_frame += 1;
		count = 0;
		uint i = 0;
		bool need_loop = true;
		for (; i < last_step; ++i)
		{
			count += steps[i].frames;
			if (current_animation != nullptr)
				*current_animation = steps[i].animation;
			if (count >= current_frame)
			{
				accumulated_speed += steps[i].speed;
				need_loop = false;
				hi = steps[i].speed;
				hi1 = count;

				break;
			}
			if (count == 100) {
				path1_finished = count;
			}
		}

		if (need_loop && loop)
			current_frame = 0;

		return iPoint((int)accumulated_speed.x, (int)accumulated_speed.y);
	}

	fPoint GetCurrentPositionfl(Animation** current_animation = nullptr)
	{
		current_frame += 1;
		count = 0;
		uint i = 0;
		bool need_loop = true;
		for (; i < last_step; ++i)
		{
			count += steps[i].frames;
			if (current_animation != nullptr)
				*current_animation = steps[i].animation;
			if (count >= current_frame)
			{
				accumulated_speed += steps[i].speed;
				need_loop = false;
				hi = steps[i].speed;
				hi1 = count;

				break;
			}
			if (count == 100) {
				path1_finished = count;
			}
		}

		if (need_loop && loop)
			current_frame = 0;

		return fPoint(accumulated_speed.x, accumulated_speed.y);
	}

	bool Path1Finished() {
		if (path1_finished == 100) {
			loop = true;
		}
		return loop;
	}

	fPoint GetCurrentPositionf()
	{
		return hi;
	}

	int GetCurrentStep() const
	{
		return hi1;
	}

	void ChangeNextPositionf(fPoint speed) {
		steps[hi1 + 1].speed = speed;
	}

	void Reset()
	{
		current_frame = 0;
	}
};

#endif // __PATH_H__
