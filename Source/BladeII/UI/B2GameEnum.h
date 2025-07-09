#pragma once
#include "B2GameEnum.generated.h"

//============================================
// ����
//============================================
enum class EServiceState : uint8
{
	Serviced,			//���� ������.
	NotServiced,		//���� ��.
	ForcedAppUpdate,	//���� �� ������Ʈ �ʿ�.
	ForcedPatch,		//���� ���ҽ� ������Ʈ �ʿ�.
	RecommendPatch,		//���� ���ҽ� ������Ʈ �ʿ�.
	UnknownError,		//�˼����� ����(�ַ� AGW Response ����).
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
// ����, �÷���.
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

