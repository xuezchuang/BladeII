// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildMatchPopup.generated.h"


DECLARE_DELEGATE(FGuildMatchPopup)

UENUM(BlueprintType)
enum class EGuildBattleRequestType : uint8
{
	Request,
	Requesting,
};


UCLASS()
class BLADEII_API UB2UIGuildMatchPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetGuildSchedule();
	void SetVisibilityPopup(bool InValue);

public:
	void OpenPopup(int32 GuildState, int32 GuildPosition);
	void SetDelegate(const FGuildMatchPopup& Delegate);

protected:
	UFUNCTION()
	void OnClickBTN_Confirm();
	UFUNCTION()
	void OnClickBTN_Cancel();

private:
	TWeakObjectPtr<UB2Button>		BTN_Confirm;
	TWeakObjectPtr<UB2Button>		BTN_Cancel;
	TWeakObjectPtr<UB2Button>		BTN_Cancel2;
	TWeakObjectPtr<UCanvasPanel>	CP_GuildMatchRequest;
	TWeakObjectPtr<UCanvasPanel>	CP_ImpossibleMsg;
	TWeakObjectPtr<UCanvasPanel>	CP_GuildMatch;
	TWeakObjectPtr<UCanvasPanel>	CP_Btns;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_GuildMatch;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_GuildMemberCount;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_GuildStateAccout;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_ErrMsg;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_ImpossibleMsg;

	TWeakObjectPtr<UTextBlock>		TB_BtnPositive;
	TWeakObjectPtr<class UB2UIGuildSchedule>			UIP_GuildSchedule;

	FGuildMatchPopup				GuildMatchPopupDelegate;

};
