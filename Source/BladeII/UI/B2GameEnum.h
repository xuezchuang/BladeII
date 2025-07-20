#pragma once
#include "B2GameEnum.generated.h"

//============================================
// 辑厚胶
//============================================
enum class EServiceState : uint8
{
	Serviced,			//沥惑 辑厚胶吝.
	NotServiced,		//痢八 吝.
	ForcedAppUpdate,	//碍力 聚 诀单捞飘 鞘夸.
	ForcedPatch,		//碍力 府家胶 诀单捞飘 鞘夸.
	RecommendPatch,		//鼻厘 府家胶 诀单捞飘 鞘夸.
	UnknownError,		//舅荐绝绰 俊矾(林肺 AGW Response 角菩).
};

UENUM(BlueprintType)
enum class EOSCode : uint8
{
	None,
	PC,
	IOS,
	AOS,
	Max
};

UENUM(BlueprintType)
enum class EAccountType : uint8
{
	None,
	Guest,
	Google,
	Apple,
	Facebook,
	Max
};

//============================================
// 付南, 敲房汽.
//============================================
UENUM(BlueprintType)
enum class EMarketType : uint8
{
	None,
	AppStore = 11,
	GoogleStore,
	TStore,
	Max
};

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	None,
	Google = 3,
	Facebook,
	GameCenter,
	Max
};

UENUM(BlueprintType)
enum class EPlatformIDPCode : uint8
{
	None,
	Facebook,
	Google,
	Guest,
	Kakao,
	Max
};

