#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildMemberSlot.generated.h"
DECLARE_DELEGATE_FourParams(FGuildMemberChangeList,int32, int32, int32, int32)
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMemberSlot : public UB2UIWidget
{
	GENERATED_BODY()
public:
	UB2UIGuildMemberSlot(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void OnClickedSenderBTNArea() override;
	virtual void OnPressedSenderBTNArea() override;
	virtual void OnReleasedSenderBTNArea() override;
public:
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }
	void SetRank(int32 Rank);
	void SetGuildMember(b2network::B2GuildMemberPtr GuildMemberInfo,int32 Rank);

	UFUNCTION()
		void OnClickSelect();
	UFUNCTION()
		void OnClickCharacterDetail();

	void SetChangeDelegate(FGuildMemberChangeList Delegate);
	void SetLogOutTime(int64 LogOutTime);

protected:
	
	TWeakObjectPtr<class UB2UIGuildInfoMemberPart>	UIP_GuildMemberPart;
	TWeakObjectPtr<class UB2UIGuildInfoPart3>		UIP_GuildInfoPart3;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_ContactState;
	TWeakObjectPtr<UB2Button>						BTN_Click;
	TWeakObjectPtr<UB2Button>						BTN_CharacterDetail;

	TWeakObjectPtr<UWidgetSwitcher>					WS_RankNumber;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_Rank;

	TWeakObjectPtr<UTextBlock>						TB_SupportTitle;
	TWeakObjectPtr<UTextBlock>						TB_SupportCount;
private:
	int64 GuildID;
	int64 MemberID;
	int32 SlotIndex;
	int32 SlotRank;

private:
	FGuildMemberChangeList MemeberChangeListDelegate;
};