#pragma once

UENUM()
enum class BTTASK_MODE : uint8
{
	EBM_MOVE		=	0,
	EBM_ATTACK,
	EBM_DEFENSE,
	EBM_RETURNHOME
};

UENUM()
enum class ATTACK_MODE : uint8
{
	EAM_NORMAL		=	0,
	EAM_SKILL,
	EAM_TAG
};