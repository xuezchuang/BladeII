// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIEnum.h"
#include <memory>
#include "B2UIItemForgePopup.generated.h"

/**
 * 
 */

struct TargetData 
{
	int32 RefID;
	int32 Count;

	TargetData(int32 RefId, int32 count)
	{
		RefID = RefId;
		Count = count;
	}
};

class ItemForgeSettingDatas 
{
public:
	ItemForgeSettingDatas() {}
	ItemForgeSettingDatas(EItemForgeSlot SType, int32 FID):
		FactoryID(FID),SlotType(SType){}

	EPCClass CharacterType;				// 캐릭터 타입
	int32 FactoryID;					// 대체 테이블 유니크 코드
	int32 MakeCount;					// 제작 횟수
	int32 MainItemRefID;				// 선택 가능한 메인 아이템
	int32 MainItemCount;				// 선택 가능한 메인 아이템의 필요 개수
	EItemForgeSlot SlotType;			// 테이블 컬럼 선택
	TArray<TargetData> ResourceItem;	// 대체 테이블 데이터
	TArray<FB2Item> HoldList;			// 유저 보유 아이템 리스트

public:

	void SetDataSet(EItemForgeSlot SType) {
		SlotType = SType;
	}
	// 데이터 세팅
	void SetDataSet(EPCClass CharType,int32 FID, EItemForgeSlot SType,
		int32 Makecount, int32 MainItem, int32 MainItemcount) {
		CharacterType = CharType;
		SetDataSet(SType);
		FactoryID = FID;
		MakeCount = Makecount;
		MainItemRefID = MainItem;
		MainItemCount = MainItemcount;
	}
};


UCLASS()
class BLADEII_API UB2UIItemForgePopup : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	
	TWeakObjectPtr<UTextBlock>	TB_Title;

	TWeakObjectPtr<class UB2ScrollBox>	SB_Item;


	TSubclassOf<class UB2DynItemIcon_ItemForge>   TabButtonBP;


	TWeakObjectPtr<class UB2Button>	BTN_Cancel;

	ItemForgeSettingDatas ItemForgeDataSet;		// 제작소 관련 데이터 셋

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2DynItemIcon_ItemForge> result;

	TWeakObjectPtr<class UHorizontalBox> HB_Items;
	TWeakObjectPtr<class UScrollBox> SB_ItemTargets;
	TWeakObjectPtr<class UB2Button>				BTN_Select;
	TWeakObjectPtr<UTextBlock>					TB_Confirm;
	class FLocalCharacterData*					CachedCharacterStore;	// 임시 저장된 캐릭터 데이터 저장소
private:
	FB2Item										SelectedItem;			// 선택된 아이템
	int32										Itemforge_SelectTicket;	// SelectedItem 세팅을 위한 Event System

	bool										bSubscribed;			// Event System 세팅 여부
	//TArray<std::shared_ptr<issue>>				Issues;					// Event System 함수 펑터

	class UB2UIItemForgeIngredientList* ItemForgeIngredientUI;			// 상위 UI 클래스

	int32 requiredItemEnhanceLevel;
	int32 requiredItemLevel;
public:
	virtual void Init() override;		// B2UI 기본 Init
	// ItemForgePopup에 필요한 모든 데이터 세팅
	void InitializeForgePopup(EPCClass CharType, int32 FID, EItemForgeSlot SType,int32 Makecount,int32 MainItem ,int32 MainItemCount ,class UB2UIItemForgeIngredientList* MainUI, int32 enhanceLevel=0, int32 itemLevel=0);
	// EventSystem Sub,UnSub
	void SubscribeEvent();
	void UnSubscribeEvent();
public:
	UFUNCTION()
	void OnClickBTN_Cancel();
	UFUNCTION()
	void OnClickBTNSelect();
protected:
	// B2UI 기본 기능
	virtual void CacheAssets()override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	
	
};
