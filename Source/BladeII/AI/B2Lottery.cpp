
#include "B2Lottery.h"
//#include "BladeII.h"

Lottery::~Lottery()
{
	Balls.Empty();
}

void Lottery::PushBall(uint8 key, float percent)
{
	if (AlreadyHas(key))
	{
		return;
	}

	auto LastIndex = Balls.Num();

	Range		r;
	r.start		= LastIndex ? Balls[LastIndex-1].range.end : 0.0f;
	r.end		= r.start + percent;

	KeyRangePair	kr;
	kr.key		= key;
	kr.range	= r;

	Balls.Add(kr);
}

uint8 Lottery::PopBall() const
{
	auto	Random = FMath::FRand();

	for (auto p : Balls)
	{
		if (p.range.end < Random)
			continue;

		return p.key;
	}

	BII_CHECK(false);
	return 0;
}

bool Lottery::AlreadyHas(uint8 key) const
{
	for (auto p : Balls)
	{
		if (p.key != key)
			continue;

		return true;
	}

	return false;
}