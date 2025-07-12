// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildMapEntryDirect.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildMapEntryDirect : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetMyGuildInfo(b2network::B2GuildBattleGuildPtr InGuildInfo);
	void SetOtherGuildInfo(b2network::B2GuildBattleGuildPtr InGuildInfo);
	void SetMyGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetMyGuildName(FText NameText);
	void SetMyGuildMemeber(int32 GuildMember);
	void SetMyGuildLv(int32 Lv);


	void SetOtherGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetOtherGuildName(FText OtherNameText);
	void SetOtherGuildMemeber(int32 GuildMember);
	void SetOtherGuildLv(int32 Lv);

	UFUNCTION(BlueprintImplementableEvent)
		void BasePlayAnimation();

	UFUNCTION(BlueprintImplementableEvent)
		void SpecialPlayAnimation();

private:
	TWeakObjectPtr<class UB2UIGuildMark> UIP_MyGuildMark;
	TWeakObjectPtr<UB2RichTextBlock>	TB_MyGuildName;
	TWeakObjectPtr<UTextBlock>			TB_MyGuildLv;
	TWeakObjectPtr<UTextBlock>			TB_MyGuildMember;
	

	TWeakObjectPtr<class UB2UIGuildMark> UIP_OtherGuildMark;
	TWeakObjectPtr<UB2RichTextBlock>	TB_OtherGuildName;
	TWeakObjectPtr<UTextBlock>			TB_OtherGuildLv;
	TWeakObjectPtr<UTextBlock>			TB_OtherGuildMember;

	TWeakObjectPtr<UPanelWidget>		O_MyDirect;
	TWeakObjectPtr<UPanelWidget>		O_OtherDirect;

	TWeakObjectPtr<class UB2UIGuildInfoPart4>	UIP_LeftGuildInfo;
	TWeakObjectPtr<class UB2UIGuildInfoPart4>	UIP_RightGuildInfo;

	
};
