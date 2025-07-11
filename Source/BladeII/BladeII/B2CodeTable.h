 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Common/CommonStruct.h"

/**
 *
 */
 //[@AKI, 180716] 더 좋은 위치가 있다면 변경해도 됨
namespace eLogSendToFlag
{
	const int32 LOG_SENDTO_NONE = 0;
	const int32 LOG_SENDTO_GAMESERVER = 1;
	const int32 LOG_SENDTO_KOREA_KAKAOBIS = 2; //1 << 1;
	const int32 LOG_SENDTO_GLOBAL = 4; //1 << 2;
	const int32 LOG_SENDTO_TOTAL = 4 + 2 + 1; //(1 << 2) + (1 << 1) + 1;
};

class BLADEII_API B2CodeTable
{
	//Function
public:
	B2CodeTable();
	~B2CodeTable();

protected:
private:

	//Variable
public:
protected:
private:
};

// Kakao중심으로 잡아서 Base가 4개의 필드를 가지고 있음
class B2ActionLogSender
{
	//Function
public:
	B2ActionLogSender()
	{
		m_stCategory.Empty();
		m_stAction.Empty();
		m_stLabel.Empty();
		m_stActionAttr1.Empty();
	}
	virtual ~B2ActionLogSender() {};

	virtual B2ActionLogSender& setCategory(const FString& _stCategory) { m_stCategory = _stCategory;		return Me(); };
	virtual B2ActionLogSender& setAction(const FString& _stAction) { m_stAction = _stAction;		return Me(); };
	virtual B2ActionLogSender& setLabel(const FString& _stLabel) { m_stLabel = _stLabel;			return Me(); };
	virtual B2ActionLogSender& setActionAttr1(const FString& _stActionAttr1) { m_stActionAttr1 = _stActionAttr1;	return Me(); };

	virtual void LogSender(const int32 _LogAddresseeType);
protected:
	B2ActionLogSender& Me()
	{
		return static_cast<B2ActionLogSender&>(*this);
	}
	FString MakePreHeader();
	//FString ConvertJsonToFString(TSharedPtr<class FJsonObject> _jsonObj);
private:

	//Variable
public:
protected:
	FString m_stCategory;
	FString m_stAction;
	FString m_stLabel;
	FString m_stActionAttr1;
private:
};


template <class T>
struct B2ActionLogType
{
	//Function
public:
	static FString GetName(const T& enumType) { return FString(TEXT("default_NONE")); };
protected:
private:

	//Variable
public:
protected:
private:
};

template<>
struct B2ActionLogType<EItemInvenType>
{
	//Function
public:
	static FString GetName(const EItemInvenType& _inInvenType)
	{
		switch (_inInvenType)
		{
		case EItemInvenType::EIIVT_Weapon:				return FString(TEXT("WEAPON"));;
		case EItemInvenType::EIIVT_Protection:			return FString(TEXT("ARMOR"));
		case EItemInvenType::EIIVT_Accessory:			return FString(TEXT("ACCESSORY"));
		case EItemInvenType::EIIVT_Consumables:			return FString(TEXT("CONSUMABLE"));
		case EItemInvenType::EIIVT_United:				return FString(TEXT("UNITED"));
		default:										return FString(TEXT("Inven_NONE"));
		}

	};
protected:
private:

	//Variable
public:
protected:
private:

};

template<>
struct B2ActionLogType<EStageDifficulty>
{
	//Function
public:
	static FString GetName(const EStageDifficulty& _InStageDifficultType)
	{
		switch (_InStageDifficultType)
		{
		case EStageDifficulty::ESD_Normal:			return FString(TEXT("NORMAL"));
		case EStageDifficulty::ESD_Hard:			return FString(TEXT("HARD"));
		case EStageDifficulty::ESD_VeryHard:		return FString(TEXT("VERY_HARD"));
		case EStageDifficulty::ESD_Hell:			return FString(TEXT("HELL"));
		default:									return FString(TEXT("StageDifficult_NONE"));
		}
	};
protected:
private:

	//Variable
public:
protected:
private:
};

template<>
struct B2ActionLogType<EStoreItemType>
{
	//Function
public:
	static FString GetName(const EStoreItemType& _InStoreItemType)
	{
		switch (_InStoreItemType)
		{
		case EStoreItemType::BladePoint:		return FString(TEXT("BLADEPOINT"));
		case EStoreItemType::Gem:				return FString(TEXT("GEM"));
		case EStoreItemType::Gold:				return FString(TEXT("GOLD"));
		case EStoreItemType::Item:				return FString(TEXT("ITEM"));
		case EStoreItemType::GeneralLottery:	return FString(TEXT("GENERAL_LOTTERY"));
		case EStoreItemType::SelectiveLottery:	return FString(TEXT("SELECTIVE_LOTTERY"));
		case EStoreItemType::Package:			return FString(TEXT("PACKAGE"));
		case EStoreItemType::DarkCrystal:		return FString(TEXT("DARK_CRYSTAL"));
		default:								return FString(TEXT("StoreItem_NONE"));
		}
	};
protected:
private:

	//Variable
public:
protected:
private:
};

template<>
struct B2ActionLogType<ECollectionType>
{
	//Function
public:
	static FString GetName(const ECollectionType& _InCollectionType)
	{
		switch (_InCollectionType)
		{
		case ECollectionType::SetItem:			return FString(TEXT("SET"));
		case ECollectionType::Weapon:			return FString(TEXT("WEAPON"));
		case ECollectionType::Armor:			return FString(TEXT("ARMOR"));
		case ECollectionType::Accessory:		return FString(TEXT("ACCESSORY"));
		case ECollectionType::Wing:				return FString(TEXT("WING"));
		default:								return FString(TEXT("CollectionType_NONE"));
		}
	};
protected:
private:

	//Variable
public:
protected:
private:
};

template<>
struct B2ActionLogType<EB2GameMode>
{
	//Function
public:
	static FString GetName(const EB2GameMode& _InGameModeType)
	{
		FString typeName(TEXT("GameModeType_"));
		switch (_InGameModeType)
		{
		case EB2GameMode::Scenario: 		return FString(TEXT("SCENARIO"));
		case EB2GameMode::CounterDungeon:	return FString(TEXT("COUNTERDUNGEON"));
		case EB2GameMode::HeroTower:		return FString(TEXT("HERO_TOWER"));
		case EB2GameMode::PVP_Tag:			return FString(TEXT("PVP_TAG"));
		case EB2GameMode::PVP_Team:			return FString(TEXT("PVP_TEAM"));
		case EB2GameMode::Raid:				return FString(TEXT("RAID"));
		case EB2GameMode::Control:			return FString(TEXT("ASSAULT"));
		case EB2GameMode::Tutorial:			return FString(TEXT("TUTORIAL"));
		case EB2GameMode::Lobby:			return FString(TEXT("LOBBY"));
		case EB2GameMode::Guild:			return FString(TEXT("GUILD"));
		default:							return FString(TEXT("GameModeType_NONE"));
		}
	};
protected:
private:

	//Variable
public:
protected:
private:

};


template<>
struct B2ActionLogType<EPCClass>
{
	//Function
public:
	static FString GetName(const EPCClass& _InPCClass)
	{
		switch (_InPCClass)
		{
		case EPCClass::EPC_Assassin:		return FString(TEXT("ASSASSIN"));
		case EPCClass::EPC_Fighter:			return FString(TEXT("FIGHTER"));
		case EPCClass::EPC_Gladiator:		return FString(TEXT("GLADIATOR"));
		case EPCClass::EPC_Wizard:			return FString(TEXT("WIZARD"));
		default:							return FString(TEXT("COMMON"));
		}
	};
protected:
private:

	//Variable
public:
protected:
private:
};
