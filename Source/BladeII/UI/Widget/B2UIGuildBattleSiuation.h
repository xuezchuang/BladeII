// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildBattleSiuation.generated.h"

UENUM(BlueprintType)
enum class EUIGuildBattleSiuation : uint8
{
	Total			UMETA(DisplayName = "Total"),
	MyGuild			UMETA(DisplayName = "MyGuild"),
	TargetGuild		UMETA(DisplayName = "TargetGuild"),
};

UCLASS()
class BLADEII_API UB2UIGuildBattleSiuation : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void OnOpen(bool RightNow = false);

	void SetGuildInfo();
	void MyGuildMatchList();
	void OtherGuildMatchList();
	void TotalGuildMatchList();
	void SetDivisionText();
protected:
	UFUNCTION()
		void OnClickBTN_Division();
	UFUNCTION()
		void OnClickBTN_Division1();
	UFUNCTION()
		void OnClickBTN_Division2();
	UFUNCTION()
		void OnClickClose();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSubclassOf<class UB2UIGuildBattleSituationSlot>		SituationSlot;

protected:
	TWeakObjectPtr<class UB2UIGuildInfoPart4> UIP_LeftBattleGuildInfo;
	TWeakObjectPtr<class UB2UIGuildInfoPart4> UIP_RightBattleGuildInfo;
	TWeakObjectPtr<UB2RichTextBlock>	TB_Time;
	TWeakObjectPtr<UB2Button>			BTN_Division;
	TWeakObjectPtr<UB2Button>			BTN_Close;
	TWeakObjectPtr<UVerticalBox>		VB_List;
	TWeakObjectPtr<UCanvasPanel>		CP_Tap;

	TWeakObjectPtr<UB2Button>			BTN_Division1;
	TWeakObjectPtr<UB2Button>			BTN_Division2;

	TWeakObjectPtr<UB2RichTextBlock>	TB_Division;
	TWeakObjectPtr<UB2RichTextBlock>	TB_Division1;
	TWeakObjectPtr<UB2RichTextBlock>	TB_Division2;


	EUIGuildBattleSiuation				GuildBattleType;

};
