#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2UIEnum.h"
#include "B2UITMFormation.generated.h"

DECLARE_DELEGATE_OneParam(FTeamMatchEntryCharacter, EPCClass);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITMFormation : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetStartPos(ETMFormation Pos);
	void UIDocSetCharacter();
	void SetSlotCharacter(EPCClass PCClass);
	void NoneSelectCharacterSlot();
	
	FTeamMatchEntryCharacter GetCharacterSlotUpdateDelegate(){ return CharacterSlotUpdateDelegate; }

	bool TeamDocPCClassTest(class UB2UIDocTeamMatch* TeamUIDoc);

	FORCEINLINE bool IsChangeAnimation() { return bIsChangeAnimation; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager);

private:
	TWeakObjectPtr<UCanvasPanel>			Formation1;
	TWeakObjectPtr<UCanvasPanel>			Formation2;
	TWeakObjectPtr<UCanvasPanel>			Formation3;

	TWeakObjectPtr<UImage>			IMG_EmptySlot1;
	TWeakObjectPtr<UImage>			IMG_Character1;
	TWeakObjectPtr<UImage>			IMG_EmptySlot2;
	TWeakObjectPtr<UImage>			IMG_Character2;
	TWeakObjectPtr<UImage>			IMG_EmptySlot3;
	TWeakObjectPtr<UImage>			IMG_Character3;

	TWeakObjectPtr<UButton>			BTN_Formation1;
	TWeakObjectPtr<UButton>			BTN_Formation2;
	TWeakObjectPtr<UButton>			BTN_Formation3;

	TWeakObjectPtr<UImage>			IMG_FormationCircle1;
	TWeakObjectPtr<UImage>			IMG_FormationCircle2;
	TWeakObjectPtr<UImage>			IMG_FormationCircle3;

	TWeakObjectPtr<UTextBlock>		X_FirstCharFomationNum;
	TWeakObjectPtr<UTextBlock>		X_SecondCharFomationNum;
	TWeakObjectPtr<UTextBlock>		X_ThirdCharFomationNum;
	
	TArray<UImage*>					CircleImageArr;
	TArray<UImage*>					CharacterImageArr;

	ETMFormation					BeforeFormation;
	ETMFormation					AfterFormation;
	EPCClass						ChoiceClass;
public:
	FTeamMatchEntryCharacter		CharacterSlotUpdateDelegate;

private:
	void SetFomationNum(ETMFormation Formation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInterface*>		CharacterTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FSlateColor					WhiteColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FSlateColor					DarkColor;
	

	UFUNCTION()
		void OnClickFormation1();
	UFUNCTION()
		void OnClickFormation2();
	UFUNCTION()
		void OnClickFormation3();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void StartPosSettingEvent();

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void StartPlayAnimation(UWidgetAnimation* ANIM_Formation_1_1, UWidgetAnimation* ANIM_Formation_1_2
		, UWidgetAnimation* ANIM_Formation_2_1, UWidgetAnimation* ANIM_Formation_2_2
		, UWidgetAnimation* ANIM_Formation_3_1, UWidgetAnimation* ANIM_Formation_3_2);

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void StartPosSetting(UWidgetAnimation* NormalPosAni, UWidgetAnimation* AttPosAni
		, UWidgetAnimation* DefPosAni);

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void SetSlotState(int32 SelectIndex);

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void AnimationComplete();

	void SetPlayAnimation(ETMFormation beforeFormation, ETMFormation afterFormation);
	void CancelFormation();

private:
	bool bIsChangeAnimation;

};
