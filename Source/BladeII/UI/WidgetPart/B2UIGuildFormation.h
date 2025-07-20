// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIFormationPopup.h"
#include "B2UIGuildBattleReady.h"
#include "B2UIGuildFormation.generated.h"

UCLASS()
class BLADEII_API UB2UIGuildFormation : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;


	void InitSlotCharacter(int32 SlotNumber, int32 InEntryCode, int32 InLv);
	void SetSelectType(int32 InSlotNumber, EGuildEntrySelectType InSelectType);
	void SetTeamNumber(int32 InTeamNumber);
	void SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
	void SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
	void SetCombat(int32 InCombat);

	int32 GetSlotCode(int32 InSlotNumber);
private:
	TArray<TWeakObjectPtr<class UB2UIGuildFormationSlot>>		CharacterSlotArr;
	TWeakObjectPtr<UTextBlock>									TB_Combat;
	TWeakObjectPtr<UTextBlock>									TB_Team;


private:
	int32 TeamNumber;

public:
	const static int32 GuildFormationOne = 4;
	const static int32 GuildFormationMax = 8;
	const static int32 GuildMercenaryBaseID = 10000;
};


UCLASS()
class BLADEII_API UB2UIGuildFormationSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

	void SetSlotCharacter(int32 InSlotNumber ,int32 InEntryCode , int32 InLv);
	void SetTextTeamNumber(int32 InTeamNumber, int32 InSlotNumber);
	void SetSelectType(EGuildEntrySelectType InSelectType);

	int32 GetSlotNumber() { return SlotNumber; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<FLinearColor>			CharacterIMGColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInstance*>		CharacterIMG;


	UFUNCTION(BlueprintImplementableEvent)
		void SetSelectAnimation();

	UFUNCTION(BlueprintImplementableEvent)
		void LineHidden();

	UFUNCTION(BlueprintImplementableEvent)
		void LineSetting(int32 InCharacterSvr);

	UFUNCTION(BlueprintImplementableEvent)
		void SelectPlaySound();

	UFUNCTION(BlueprintImplementableEvent)
		void ChangePlaySound();

	UFUNCTION()
		void OnClickSlot();

	FORCEINLINE int32 GetSlotCode() { return GuildEntryCode; }

	void SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
	void SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
protected:
	TWeakObjectPtr<UImage>				IMG_Character;
	TWeakObjectPtr<UB2Button>			BTN_Select;
	TWeakObjectPtr<UTextBlock>			TB_Lv;

	TWeakObjectPtr<UPanelWidget>		P_Select;
	TWeakObjectPtr<UPanelWidget>		P_SelectPossible;

private:
	int32 GuildEntryType;
	int32 GuildEntryCode;
	int32 SlotNumber;

	EGuildEntrySelectType SelectType;

	FGuildEntrySlotDelegate SlotDelegate;
	FGuildEntrySlotDelegate SwapDelegate;

};


UCLASS()
class BLADEII_API UB2UIGuildFormationMercenarySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

public:

	void SetSlotCharacter(int32 InEntryCode);
	void SetTextNumber(int32 InNumber);
	void SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
	void SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate);
	void SetSelectType(EGuildEntrySelectType InSelectType);

	FORCEINLINE int32 GetMercenaryCount() { return MercenaryCount;}
	FORCEINLINE int32 GetGuildEntryCode() { return GuildEntryCode; }


	UFUNCTION()
		void OnClickSlot();

	UFUNCTION(BlueprintImplementableEvent)
		void SelectPlaySound();

	UFUNCTION(BlueprintImplementableEvent)
		void ChangePlaySound();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<FLinearColor>			CharacterIMGColor;

protected:
	TWeakObjectPtr<UB2Button>		BTN_Select;
	TWeakObjectPtr<UImage>			IMG_Character;
	TWeakObjectPtr<UTextBlock>		TB_Lv;
	TWeakObjectPtr<UTextBlock>		TB_Number;

	TWeakObjectPtr<UPanelWidget>		P_Select;
	TWeakObjectPtr<UPanelWidget>		P_SelectPossible;


	FGuildEntrySlotDelegate SlotDelegate;
	FGuildEntrySlotDelegate SwapDelegate;

	int32 MercenaryCount;

	EGuildEntrySelectType SelectType;

	int32 GuildEntryCode;
};
