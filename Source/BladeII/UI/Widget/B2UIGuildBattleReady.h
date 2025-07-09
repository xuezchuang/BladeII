// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIFormationPopup.h"
#include "B2UIGuildBattleReady.generated.h"


UENUM(BlueprintType)
enum class EGuildEntrySelectType : uint8
{
	Normal,
	Select,
	SelectPossible,
	SelectImpossible,
	NonSelect,
};

UENUM(BlueprintType)
enum class EGuildEntrySlotType : uint8
{
	FormationSlot,
	MercenaryList,
};


USTRUCT()
struct FGuildSlotInfo
{
	GENERATED_BODY()

	FGuildSlotInfo(){}
	FGuildSlotInfo(EGuildEntrySlotType InSlotType, EGuildEntrySelectType InSelectType, int32 InEntryCode, int32 InSlotNumber)
	{
		SlotType = InSlotType;
		SelectType = InSelectType;
		EntryCode = InEntryCode;
		SlotNumber = InSlotNumber;
	}

	EGuildEntrySlotType		SlotType;
	EGuildEntrySelectType	SelectType;
	int32					EntryCode;
	int32					SlotNumber;
};


USTRUCT()
struct FGuildBattleEntryInfo
{
	GENERATED_BODY()	

	FGuildBattleEntryInfo(){}
	FGuildBattleEntryInfo(b2network::B2GuildBattleCharacterSlotPtr CharacterInfo)
	{
		Slot_num = CharacterInfo->slot_num;
		Entry_code = CharacterInfo->entry_code;
		Entry_type = CharacterInfo->entry_type;
		Level = CharacterInfo->level;
	}

	int32 Slot_num;
	int32 Entry_code;
	int32 Entry_type;
	int32 Level;

};

DECLARE_DELEGATE_FourParams(FGuildEntrySlotDelegate, EGuildEntrySlotType, EGuildEntrySelectType, int32,int32);

UCLASS()
class BLADEII_API UB2UIGuildBattleReady : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	UFUNCTION()
		void OnClickBTN_Upgrade();
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;


	void initializationSlot();

	void SwapSlotUpdate(EGuildEntrySlotType InSlotType, EGuildEntrySelectType InSelectType, int32 InEntryCode, int32 InSlotNumber);
	void SelectSlotUpdata(EGuildEntrySlotType InSlotType, EGuildEntrySelectType InSelectType, int32 InEntryCode, int32 InSlotNumber);
	int32 GetEntryCombat(int32 EntryCode);

	bool CheckChangeFormation();

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIGuildFormationMercenarySlot>	UIP_MercenarySlot;

private:
	virtual bool OnBackButtonProc() override;


protected:
	TWeakObjectPtr<UB2Button>						BTN_Upgrade;
	TWeakObjectPtr<class UB2UIGuildFormation>		UIP_GuildFormation1;
	TWeakObjectPtr<class UB2UIGuildFormation>		UIP_GuildFormation2;

	TWeakObjectPtr<UTextBlock>						TB_HeroExplanation;

	TWeakObjectPtr<UHorizontalBox>					X_MercenaryList;

	TArray<class UB2UIGuildFormationMercenarySlot*>		MercenaryListSlotArr;

	FGuildSlotInfo		SelectGuildSlotInfo;

	TArray<FGuildBattleEntryInfo>				EntryInfo;

	uint32 DeliverySetGuildBattleEntryTiket;
};