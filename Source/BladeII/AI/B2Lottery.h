#pragma once

#include "B2AI.h"

struct Range
{
	float	start;
	float	end;
};

struct KeyRangePair
{
	uint8	key;
	Range	range;
};

class Lottery
{
public:
	~Lottery();

	void	PushBall(uint8, float);
	uint8	PopBall() const;

private:
	bool	AlreadyHas(uint8) const;

	TArray<KeyRangePair>	Balls;
};