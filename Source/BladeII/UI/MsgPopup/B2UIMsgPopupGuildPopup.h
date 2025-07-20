#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIMsgPopupGuildPopup.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupGuildPopup : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	void SetGuildMark(int32 GuildImage, int32 GuildColor, int32 GuildBG, int32 GuildBGColor);
	void SetGuildName(const FString& GuildName);

	void AddHandler(const FMsgPopupOnClick& Handler);

	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickConfirm();

protected:
	TWeakObjectPtr<class UB2UIGuildMark>			UIP_LargeGuildMark;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_GuildName;
	TWeakObjectPtr<UB2Button>						BTN_Confirm;

	TWeakObjectPtr<UB2RichTextBlock>				TB_Content;
	TWeakObjectPtr<UTextBlock>						TB_Notice;
	TWeakObjectPtr<UTextBlock>						TB_BtnPositive;

	FMsgPopupOnClick								ButtonDelegate;
};