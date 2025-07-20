// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UILotteryAllReceive.generated.h"

/**
 * 
 */
UENUM()
enum class ELotteryFilterType : uint8
{
	Weapon,
	Protection,
	Accessory,
	WeaponEssence,
	ProtectionEssence,
	AccessoryEssence,
	GladiatorWeapon,
	AssassinWeapon,
	WizardWeapon,
	FighterWeapon,
	GladiatorProtection,
	AssassinProtection,
	WizardProtection,
	FighterProtection,
	GladiatorAccessory,
	AssassinAccessory,
	WizardAccessory,
	FighterAccessory,
	FixedWeaponEssence,
	FixedProtectionEssence,
	FixedAccessoryEssence,
};

USTRUCT(BlueprintType)
struct FLotteryFilteringInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ELotteryFilterType FilterType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> LotteryIDs;
};

UCLASS()
class BLADEII_API UB2UILotteryAllReceive : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetLotteryMailInfo(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailInfo);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickOK();
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnChangedMailCount(float ChangeValue);
	UFUNCTION()
		void OnClickMailCountDown();
	UFUNCTION()
		void OnClickMailCountUp();
	UFUNCTION()
		void OnClickMax();

	void SetLotteryTabs(EPCClass InPCClass);
	void SetTBCount(int32 InValue);
	void SetValueMailCountSlider(int32 InValue);
	void GetCheckedFiltering(TArray<ELotteryFilterType>& OutCheckedFiltering);

private:
	struct LotteryMailInfo
	{
		int64 MailID;// 皋老 id 急琶茄 某腐磐 馆券!
		int32 LotteryIDs;
		MailGiftType GiftType; //MailGiftType::LotteryTicket, MailGiftType::Fixed_Grade_Lottey 犬牢 啊瓷
	};

	UPROPERTY(EditAnywhere, Category = "LotteryMail")
		 int32 MaxCountLotteryMail;
	UPROPERTY(EditAnywhere, Category = "LotteryMail")
		TArray<FLotteryFilteringInfo> LotteryTicketFilteringInfos; //MailGiftType::LotteryTicket
	UPROPERTY(EditAnywhere, Category = "LotteryMail")
		TArray<FLotteryFilteringInfo> FixedGradeLotteyFilteringInfos; //MailGiftType::Fixed_Grade_Lottey

	int32 ReceiveCount;
	EPCClass SelectEPCClass;
	TArray<int64> SelectMailIDs;
	TArray<LotteryMailInfo> LotteryMainInfos;
	TMap<EPCClass, TWeakObjectPtr<class UB2UILotteryTab>> LotteryTabs;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_WeaponCheckBox;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_ProtectionCheckBox;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_AccessoryCheckBox;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_WeaponEssenceCheckBox;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_ProtectionEssenceCheckBox;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_AccessoryEssenceCheckBox;
	TWeakObjectPtr<UB2Button> BTN_OK;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_MailCountDown;
	TWeakObjectPtr<UB2Button> BTN_MailCountUp;
	TWeakObjectPtr<UB2Button> BTN_Max;
	TWeakObjectPtr<USlider> S_MailCount;
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UTextBlock> TB_Max;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Description;
	TWeakObjectPtr<UTextBlock> TB_OK;
};
