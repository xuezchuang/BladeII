#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildInfoPart.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoPart1 : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetGuildName(FText GuildName);
	void SetGuildMasterName(FText GuildMasterName);
	void SetGuildLv(int32 InLv);
protected:
	TWeakObjectPtr<class UB2UIGuildMark>			UIP_GuildMark;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildMasterName;
	TWeakObjectPtr<UTextBlock>						TB_GuildLv;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoPart2 : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void SetCombat(int32 Combat);
	void SetGuildNumber(int32 MemberCount, int32 MaxMemberCount);

protected:
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildMember;
	TWeakObjectPtr<UTextBlock>						TB_CombatPower;

	TWeakObjectPtr<UTextBlock>						TB_Combat;
	TWeakObjectPtr<UB2RichTextBlock>						RTB_GuildMember_text;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoPart3 : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildScore(int32 Score);
protected:
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildScore;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoPart4 : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetGuildInfo(FText GuildName, int32 AttackCount, int32 AttackMaxCount, int32 Point, int32 MaxPoint);

protected:
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Record;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Point;

	TWeakObjectPtr<class UB2UIGuildMark>			UIP_GuildMark;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoPart5 : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetGuildInfo(FText GuildName, int32 Point, int32 GuildExp);

protected:
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Record;
	TWeakObjectPtr<UProgressBar>					PB_Point;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Point;
	TWeakObjectPtr<UTextBlock>						TB_GuildExp;

	TWeakObjectPtr<class UB2UIGuildMark>			UIP_GuildMark;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoMedal : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBarDivided")
		void OnMedal();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBarDivided")
		void OffMedal();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBarDivided")
		void MedalShot();

protected:
	TWeakObjectPtr<UB2Image>				IMG_OnMedal;
	TWeakObjectPtr<UB2Image>				IMG_OffMedal;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoMedalPart : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBarDivided")
		void EventMedalSetting(int32 Count);

	void SetMedalCount(int32 MedalCount);
	void SetGuildName(bool InMyGuild,int32 Number, FString GuildName);
	void SetIsAttack(bool IsAttack);
	void SettextAttackNumber(FText InText);

	virtual void UpdateStaticText() override;

protected:
	TArray<TWeakObjectPtr<UB2UIGuildInfoMedal>>		GuildMadalArr;
	TWeakObjectPtr<UB2UIGuildInfoMedal>				UIP_GuildMedal1;
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildNumber;

	TWeakObjectPtr<UPanelWidget>					O_AttackInfo;
	TWeakObjectPtr<UPanelWidget>					O_NoAttack;

	TWeakObjectPtr<UTextBlock>						TB_AtteckNumber;
	TWeakObjectPtr<UTextBlock>						TB_AttackComplet;

	int32 RemainMedalCount;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoMedalCastlePart : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	void SetMedalCount(int32 MedalCount);
	void SetGuildName(bool bMy, int32 Number, FString GuildName);
	void PlayMedalShot();

protected:
	TWeakObjectPtr<UB2UIGuildInfoMedal>				UIP_GuildMedal1;
	TWeakObjectPtr<UB2RichTextBlock>				TB_MedalCount;
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock>				TB_GuildNumber;
	TWeakObjectPtr<UB2Image>						IMG_MyGuild;
	TWeakObjectPtr<UB2Image>						IMG_OtherGuild;

}; 
DECLARE_DELEGATE_OneParam(FGuildMapquickSlot, int32);
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMapSlot: public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void OnSelectMap();
	void OffSelectMap();
	void SetMyCastlePos(bool MyPos);
	void SetMapIndex(int32 Index);
	void SetDelegate(FGuildMapquickSlot MapSlotdelegate);

public:
	UFUNCTION()
		void OnClickSelect();

protected:
	TWeakObjectPtr<UB2Image>			IMG_Select;
	TWeakObjectPtr<UB2Image>			IMG_Disable;
	TWeakObjectPtr<UB2Image>			IMG_Me;
	TWeakObjectPtr<UB2Button>			BTN_Click;

private:
	FGuildMapquickSlot					QuickSlotDelegate;
	int32								SlotIndex;

};
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildBattleSituationSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildMark(bool MyGuild, int32 Mark, int32 MarkColor, int32 BG, int32 BGIndex);
	void SetSituationSlot(int32 MyNumber, FString MyName, int32 OtherNumber, FString OtherName, int32 MedalCount, bool MyAttack, int64 Time);
	void SetMyGuildName(int32 Number, FString Name);
	void SetOtherGuildName(int32 Number, FString Name);
	void SetMedal(int32 Count);
	void SetAttack(bool MyAttack);
	void SetTime(int64 Time);

protected:
	TWeakObjectPtr<class UB2UIGuildMark>	UIP_MyGuildMark;
	TWeakObjectPtr<class UB2UIGuildMark >	UIP_OtherGuildMark;
	TWeakObjectPtr<UTextBlock>				TB_MyPowerIndex;
	TWeakObjectPtr<UTextBlock>				TB_OtherPowerIndex;
	TWeakObjectPtr<UB2RichTextBlock>		TB_MyName;
	TWeakObjectPtr<UB2RichTextBlock>		TB_OtherName;
	TWeakObjectPtr<UB2Image>				IMG_MyAttack;
	TWeakObjectPtr<UB2Image>				IMG_OtherAttack;
	TWeakObjectPtr<UB2RichTextBlock>		TB_Time;

	TArray<TWeakObjectPtr<UB2UIGuildInfoMedal>>			GuildMedalArr;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildSeasonRewardSlot : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetRewardSlot(FText Rank, int32 RewardId, int32 Count);
	void SetRank(FText Rank);
	void SetRewardInfo(int32 RewardId, int32 Count);
	void SetIncludeRank(bool bRank);


protected:
	TWeakObjectPtr<UB2RichTextBlock>			TB_Rank;
	TWeakObjectPtr<UB2RichTextBlock>			TB_ItemNumber;
	TWeakObjectPtr<UB2Image>					IMG_Item;
	TWeakObjectPtr<UB2Image>					IMG_Select;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInviteSlot : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	void SetGuildInfo(const FString& NickName, int32 Combat, EPCClass PCClass, int32 Lv, int32 Rank);
	void SetGuildState(int32 Stats, int64 CharAccountId);

	UFUNCTION()
		void OnClickSurch();
	UFUNCTION()
		void OnClickCharacterDetail();
protected:
	TWeakObjectPtr<class UB2UIGuildInfoMemberPart>		UIP_GuildMemberPart;
	TWeakObjectPtr<UB2Button>							BTN_Invite;
	TWeakObjectPtr<UB2Button>							BTN_CharacterDetail;
	TWeakObjectPtr<UB2RichTextBlock>					TB_Invite;

	int64 AccountId;
	int64 DeliveryInviteGuildTicket;
};