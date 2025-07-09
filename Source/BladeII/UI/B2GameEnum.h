#pragma once
#include "B2GameEnum.generated.h"

//============================================
// 서비스
//============================================
enum class EServiceState : uint8
{
	Serviced,			//정상 서비스중.
	NotServiced,		//점검 중.
	ForcedAppUpdate,	//강제 앱 업데이트 필요.
	ForcedPatch,		//강제 리소스 업데이트 필요.
	RecommendPatch,		//권장 리소스 업데이트 필요.
	UnknownError,		//알수없는 에러(주로 AGW Response 실패).
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
// 마켓, 플렛폼.
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

