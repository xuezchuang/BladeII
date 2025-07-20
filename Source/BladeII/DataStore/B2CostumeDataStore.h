// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonStruct.h"
#include "EventSubsystem.h"
#include "B2UIEnum.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChangedCostumeVisible, EPCClass, ECostumeEquipPlace, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedVisibleMode, bool)

struct StaticCostumeEquipContainer
{
	StaticCostumeEquipContainer();
	~StaticCostumeEquipContainer() {}

	TArray<ECostumeEquipPlace> CostumeEquipArray;
	TArray<EPCClass> PCClassArray;
};

class FCostumeVisibleCache
{
public:
	FCostumeVisibleCache();
	~FCostumeVisibleCache() {};

	void SetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip, bool bVisible);
	bool GetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip);

	void AllVisibleChange(EPCClass InPCClass, bool bVisible);

	FOnChangedCostumeVisible OnChagneCostumeVisible;

private:
	TMap<EPCClass, TMap<ECostumeEquipPlace, bool>> CostumeVisibleMap;
};

class BLADEII_API FCostumeDataStore
{
public:
	FCostumeDataStore();
	~FCostumeDataStore();

	static const StaticCostumeEquipContainer& GetCostumeEquipArray();

public:
	//Use only reference variable.
	FCostumeDataStore(FCostumeDataStore const&) = delete;
	FCostumeDataStore& operator=(FCostumeDataStore const&) = delete;

public:
	void OnStartUpModule();
	void OnShutDownModule();

public:
	bool GetIsOpenCostume() { return IsOpenCostume; }
	const ECostumePage& GetCachedCostumePageType() { return CachedCostumePageType; }

public:
	void SetMasterData(const FB2MasterDatas& InMasterDatas);
	void SetIsOpenCostume(const bool& IsOpen);
	void SetCachedCostumePageType(const ECostumePage& eCostumePageType);

	//visible
	FORCEINLINE const TArray<int32>& GetPreviewCostumes() { return PreviewCostumeRefIDList; };

	void ClearPreview();
	void SetLocalVisibleMode(EPCClass ChangeClass = EPCClass::EPC_End);
	void SetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip, bool bVisible);
	bool GetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip);

	void AllVisibleChange(EPCClass InPCClass, bool bVisible);
	FORCEINLINE bool IsPreviewMode() { return bPreview; }

	FOnChangedCostumeVisible OnChangeCostumeVisible;
	FOnChangedVisibleMode OnChangedVisibleMode;

private:
	void SubscribeEvents();
	void UnSubscribeEvents();
	
	//visible
	void ChangeVisibleMode(bool InPreview, EPCClass InPCClass);
	void SetPreviewMode(int32 ProductID);
	EItemEquipPlace GetRandomCostumeAnim();

private: //Dynamic Data


private: //Static Data


private:
	TArray<std::shared_ptr<issue>>	Issues;
	bool							IsOpenCostume;
	ECostumePage					CachedCostumePageType;


	//visible
	bool								  bPreview;
	//Request 줄이는 용도
	std::shared_ptr<FCostumeVisibleCache> LocalVisibleCache;
	//상점 미리보기 용도
	std::shared_ptr<FCostumeVisibleCache> PreviewVisibleCache;

	std::shared_ptr<FCostumeVisibleCache> CurrentVisibleCache;

	TArray<int32>						  PreviewCostumeRefIDList;
};
