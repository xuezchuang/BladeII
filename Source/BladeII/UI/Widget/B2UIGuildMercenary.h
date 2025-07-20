// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildMercenary.generated.h"

DECLARE_DELEGATE_OneParam(FNPCSelectSlotDelegate , int32)

UENUM()
enum class ENPCSKillType : uint8
{
	Special,
	Skill,
};

UENUM()
enum class ENPCSpecialSKill : uint8
{
	Option1,
	Option2,
	Option3,
};

UENUM()
enum class ENPCIconSlotState : uint8
{
	Select,
	UnSelect,
	Lock,
};

UCLASS()
class BLADEII_API UB2UIGuildMercenarySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetSlot(b2network::B2mdGuildMercenaryPtr InNPCInfo);
	int32 GuildNPCID() { return NPCID; }

	void SetSlotState(ENPCIconSlotState InIconSlot);
	ENPCIconSlotState GetSlotState() {return SlotState;}

	UFUNCTION(BlueprintImplementableEvent)
		void InceraseExpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void LevelUpEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void SelectPlaySoundEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void LockPlaySoundEvent();


public:
	UFUNCTION()
		void OnClickNPCButton();

public:
	TWeakObjectPtr<UButton>			BTN_NPCSelect;

	TWeakObjectPtr<UPanelWidget>	O_Select;
	TWeakObjectPtr<UImage>			IMG_SelectNPCImage;

	TWeakObjectPtr<UPanelWidget>	O_UnSelect;
	TWeakObjectPtr<UImage>			IMG_UnSelectNPCImage;

	TWeakObjectPtr<UPanelWidget>	O_Lock;
	TWeakObjectPtr<UImage>			IMG_LockNPCImage;


private:
	int32							NPCID;
	ENPCIconSlotState				SlotState;
	FNPCSelectSlotDelegate			SlotDelegate;

public:
	template<typename FunctorType>
	inline void SetNPCSelectSlotDelegate(FunctorType&& InFunctor)
	{
		SlotDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

};

UCLASS()
class BLADEII_API UB2UIGuildMercenarySkillSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

public:
	void SetIDAndSkillType(int32 InID, ENPCSKillType InSkillType, int32 InLevel = 0 , ENPCSpecialSKill SpecialType = ENPCSpecialSKill::Option1);
	void SetSpecialType();
	void SetSkillType();

public:
	TWeakObjectPtr<UPanelWidget>	O_VisiblePanel;
	TWeakObjectPtr<UB2Button>		BTN_SKill;
	TWeakObjectPtr<UImage>			IMG_Skill;
	TWeakObjectPtr<UPanelWidget>	O_LockPanel;
	TWeakObjectPtr<UImage>			IMG_LockImage;
	TWeakObjectPtr<UTextBlock>		TB_LockLv;

public:
	UFUNCTION()
		void OnClickSkill();

private:
	int32 ID;
	int32 Level;
	ENPCSKillType SkillType;
	ENPCSpecialSKill SpecialType;


	int32 OptionID;
	int32 OptionValue;
};


UCLASS()
class BLADEII_API UB2UIGuildMercenary : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete();
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UpdateStaticText() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SubscribeEvent();
	void UnSubscribeEvent();

public:
	void SetMercenary();
	void SetDonatioCount();
	void SetMercenaryInfo(int32 InNPCID);
	void DonatioButtonEnbled(bool IsEnable = true);
	void DeliveryuildDonation(FB2ResponseGuildDonationPtr InGuildDonatio);
	UFUNCTION()
		void OnClickGoldDonatio();

	UFUNCTION()
		void OnClickGemDonatio();

	UFUNCTION(BlueprintImplementableEvent)
		void InceraseExpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void LevelUpEvent();

	void UpgradeTick(float InDeltaTime);

public:
	TWeakObjectPtr<UTextBlock>					TB_NPCName;
	TWeakObjectPtr<UB2RichTextBlock>			TB_NPCDesc;
	TWeakObjectPtr<UTextBlock>					TB_NPCLv;
	TWeakObjectPtr<UTextBlock>					TB_NPCLvNext;
	
	TWeakObjectPtr<UImage>						IMG_LevelMax;
	TWeakObjectPtr<UProgressBar>				PB_Exp;
	TWeakObjectPtr<UB2RichTextBlock>			TB_Exp;

	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_SpecialSkill1;
	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_SpecialSkill2;
	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_SpecialSkill3;
	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_Skill1;
	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_Skill2;
	TWeakObjectPtr<class UB2UIGuildMercenarySkillSlot>		UIP_Skill3;

	TArray<TWeakObjectPtr<class UB2UIGuildMercenarySlot>>	GuildMercenarySlotArr;


	TWeakObjectPtr<UButton>						BTN_Donatio;

	TWeakObjectPtr<UScrollBox>					SB_Mercenary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildMercenarySlot)
		TSubclassOf<UB2UIGuildMercenarySlot>		GuildMercenarySlot;


	//Donatio

	TWeakObjectPtr<UB2Button>					BTN_GoldDonatio;
	TWeakObjectPtr<UB2RichTextBlock>			TB_GoldDonatioExp;
	TWeakObjectPtr<class UB2UIStoreProductCost>	UIP_GoldPrice;


	TWeakObjectPtr<UB2Button>					BTN_GemDonatio;
	TWeakObjectPtr<UB2RichTextBlock>			TB_GemDonatioExp;
	TWeakObjectPtr<class UB2UIStoreProductCost>	UIP_GemPrice;

	TWeakObjectPtr<UTextBlock>					TB_DonatioCount;
	TWeakObjectPtr<UTextBlock>					TB_DonatioMaxCount;


	TWeakObjectPtr<UImage>						IMG_FX_bar_01;

	//static Text

	TWeakObjectPtr<class UB2RichTextBlock>		TB_DonatioDesc;
	TWeakObjectPtr<UTextBlock>					TB_Special;
	TWeakObjectPtr<UTextBlock>					TB_Skill;

	TWeakObjectPtr<UTextBlock>					TB_GoldDonatio;
	TWeakObjectPtr<UTextBlock>					TB_GemDonatio;

	TWeakObjectPtr<UTextBlock>					TB_Donatio1;
	TWeakObjectPtr<UTextBlock>					TB_Donatio2;

	UB2UIGuildMercenarySlot*					SelectSlot;


private:
	int32 NPCID;
	uint32 DeliveryuildDonationTiket;
	uint32 DeliveryuildDonatioErrorTiket;


	int32 BeforeExp;
	int32 ProgressExp;
	int32 AfterExp;
	int32 BeforeLevel;
	int32 AfterLevel;
	float UpgradeTimeRemain;
	const float UpgradeTimeMax = 0.5f;
	const float FXbarXSize = 330.f;

	bool LevelUpEffect;


};
