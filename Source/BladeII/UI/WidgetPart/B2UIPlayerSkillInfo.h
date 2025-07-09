#pragma once

#include "B2UIWidgetBase.h"
#include "B2SkillInfo.h"
#include "Styling/SlateTypes.h"
#include "B2UIPlayerSkillInfo.generated.h"

USTRUCT()
struct FSkillBadgeImage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ESkillType SkillType;
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* BadgeImage;
	UPROPERTY(EditAnywhere)
	FSlateColor FontColor;

	static class UMaterialInstance* GetSkillBadgeImage(const TArray<FSkillBadgeImage>& SkillBadgeImages, ESkillType InSkillType);
	static const FSlateColor& GetSkillBadgeFontColor(const TArray<FSkillBadgeImage>& SkillBadgeImages, ESkillType InSkillType);
};

USTRUCT()
struct FLearnLevelFontColor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 LearnLevel;
	UPROPERTY(EditAnywhere)
	FSlateColor FontColor;

	static FSlateColor DefaultColor;
	static const FSlateColor& GetFontColor(const TArray<FLearnLevelFontColor>& LearnLevelFontColors, int32 LearnLevel);
};

DECLARE_DELEGATE(FSkillInfoOnClick)
DECLARE_DELEGATE(FSkillLevelUpOnClick)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPlayerSkillInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;	
	
	void SetButtonStyle(const FButtonStyle& InButtonStyle);

	void SetSkillLevel(int32 InSkillLevel,bool bIsRegist = false);
	void SetSkillMaxLevel(int32 InSkillMaxLevel);
	void SetSkillType(ESkillType InSkillType);
	void SetSkillId(int32 InSkillId);
	void SetSkillLevelupGold(int32 InSkillId);
	void SetSkillLevelupPoint(int32 InSkillId, int32 SkillPointAmount);

	void UpdateLock(int32 CharacterLevel);
	void UpdatePassiveSkillLock();
	void UpdateEnableLevelUp(bool bEnoughSkillPoint);

	FORCEINLINE int32 GetSkillId(){ return SkillId; };

	void SetSkillIconHandler(const FSkillInfoOnClick& ClickHandler);
	void SetSkillLevelUpHandler(const FSkillLevelUpOnClick& ClickHandler);
	void SetSelected(bool bSet);
	void SetSuggested(bool bSet);
	void SetRegistred(bool bSet);

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	int32 GetSkillLv() { return SkillLevel; }
	int32 GetSkillMaxLv() { return SkillLevelMax; }

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySkillEnhance_BP();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	int32 GetLevelUpGold(int32 InSkillID);
	
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FSkillBadgeImage> SkillBadgeImages;

	UPROPERTY(EditAnywhere)
	TArray<FLearnLevelFontColor> LearnLevelFontColors;

	UPROPERTY(EditAnywhere)
	FSlateColor SkillLevelColor;

	UPROPERTY(EditAnywhere)
	FSlateColor SkillDisableColor;

	UPROPERTY(EditAnywhere)
	FSlateColor SkillEnableColor;

	UPROPERTY(EditAnywhere)
	FSlateColor SkillNotEnoughConditionColor;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIFloatingText> FloatTextClass;

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySuggest_BP();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayLearnSkill_BP();



	UFUNCTION(BlueprintImplementableEvent)
		void PlayEnableAddSP(bool bPlay);
	
private:
	UFUNCTION()
	void OnClickBtnAddSkillPoint();

	UFUNCTION()
	void OnClickBtnSkillIcon();

	void SetSkillName(FText InSkillName);

	void OnDealyAddSkillPoint();
	void OnDelaySkillIcon();

private:
	TWeakObjectPtr<UButton> BTN_SkillIcon;
	TWeakObjectPtr<UButton> BTN_AddSkillPoint;
	TWeakObjectPtr<UImage> IMG_AddSkillPoint;
	TWeakObjectPtr<UImage> IMG_EnableAddSP;
	TWeakObjectPtr<UImage> FX_EnableAddSP_01;
	TWeakObjectPtr<UImage> FX_EnableAddSP_02;

	TWeakObjectPtr<UTextBlock> TB_SkillName;
	TWeakObjectPtr<UHorizontalBox>	HB_SkillLevel;
	TWeakObjectPtr<UTextBlock> TB_SkillLevel;
	TWeakObjectPtr<UTextBlock> TB_SkillRestrict;
	TWeakObjectPtr<UImage> IMG_SkillLevel_N;
	TWeakObjectPtr<UTextBlock> TB_SkillMaxLevel;
	TWeakObjectPtr<UTextBlock> TB_SkillType;
	TWeakObjectPtr<UImage> IMG_Skill_Badge;
	
	TWeakObjectPtr<UImage> IMG_Registred;
	TWeakObjectPtr<UImage> IMG_RegistredCheck;
	TWeakObjectPtr<UTextBlock> TB_Registred;

	TWeakObjectPtr<UImage> IMG_Selected;
	TWeakObjectPtr<UImage> IMG_Suggest_Select01;
	TWeakObjectPtr<UImage> IMG_Suggest_Select02;

	TWeakObjectPtr<UImage> IMG_OpenSkillBG;
	TWeakObjectPtr<UImage> IMG_Learned;	
	TWeakObjectPtr<UImage> IMG_NotLearn;
	TWeakObjectPtr<UImage> IMG_SkillIcon;
	TWeakObjectPtr<UImage> Add_SkillFrame;
	TWeakObjectPtr<UImage> IMG_LockImage;

	TWeakObjectPtr<UOverlay> Ov_Lock;
	TWeakObjectPtr<UOverlay> OV_SkillLevel;
	TWeakObjectPtr<UOverlay> SkillIconBase;
	TWeakObjectPtr<UOverlay> Skill_Badge;
	TWeakObjectPtr<UOverlay> Ov_SkillAddedOptions;
	TWeakObjectPtr<UOverlay> OV_MaxText;
	TArray<TWeakObjectPtr<class UB2UIFloatingText>> SkillAddedOptions;

	TWeakObjectPtr<UPanelWidget> PN_NeedSkillPoint;
	TWeakObjectPtr<UTextBlock> TB_NeedSkillPoint;

	TWeakObjectPtr<UPanelWidget> PN_NeedLevelupGold;
	TWeakObjectPtr<UTextBlock> TB_NeedLevelupGold;

	TWeakObjectPtr<UTextBlock> TB_ConditionUpdate;

	bool bLockLimitLevel;
	bool bEnableLevelUp;
	int32 SkillId;
	int32 SkillLevel;
	int32 SkillLevelMax;
	bool bSuggested;
	float ElapsedAutoAddPoint;

	bool IsWatingLevelup;

	FTimerHandle DelayCallbackTH;

	FSkillInfoOnClick SkillIconClickHandler;
	FSkillLevelUpOnClick  SkillLevelUpClickHandler;
};