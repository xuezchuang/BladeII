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

	EPCClass CharacterType;				// ĳ���� Ÿ��
	int32 FactoryID;					// ��ü ���̺� ����ũ �ڵ�
	int32 MakeCount;					// ���� Ƚ��
	int32 MainItemRefID;				// ���� ������ ���� ������
	int32 MainItemCount;				// ���� ������ ���� �������� �ʿ� ����
	EItemForgeSlot SlotType;			// ���̺� �÷� ����
	TArray<TargetData> ResourceItem;	// ��ü ���̺� ������
	TArray<FB2Item> HoldList;			// ���� ���� ������ ����Ʈ

public:

	void SetDataSet(EItemForgeSlot SType) {
		SlotType = SType;
	}
	// ������ ����
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

	ItemForgeSettingDatas ItemForgeDataSet;		// ���ۼ� ���� ������ ��

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2DynItemIcon_ItemForge> result;

	TWeakObjectPtr<class UHorizontalBox> HB_Items;
	TWeakObjectPtr<class UScrollBox> SB_ItemTargets;
	TWeakObjectPtr<class UB2Button>				BTN_Select;
	TWeakObjectPtr<UTextBlock>					TB_Confirm;
	class FLocalCharacterData*					CachedCharacterStore;	// �ӽ� ����� ĳ���� ������ �����
private:
	FB2Item										SelectedItem;			// ���õ� ������
	int32										Itemforge_SelectTicket;	// SelectedItem ������ ���� Event System

	bool										bSubscribed;			// Event System ���� ����
	//TArray<std::shared_ptr<issue>>				Issues;					// Event System �Լ� ����

	class UB2UIItemForgeIngredientList* ItemForgeIngredientUI;			// ���� UI Ŭ����

	int32 requiredItemEnhanceLevel;
	int32 requiredItemLevel;
public:
	virtual void Init() override;		// B2UI �⺻ Init
	// ItemForgePopup�� �ʿ��� ��� ������ ����
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
	// B2UI �⺻ ���
	virtual void CacheAssets()override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	
	
};
