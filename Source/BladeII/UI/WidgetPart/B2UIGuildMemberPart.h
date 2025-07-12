#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildMemberPart.generated.h"

UENUM(BlueprintType)
enum class EGuildChangeType : uint8
{
	APPOINT		UMETA(DisplayName = "Appoint"),
	DISMISS		UMETA(DisplayName = "Dismiss"),
	KICK		UMETA(DisplayName = "Kick"),
	PROFILE 	UMETA(DisplayName = "Profile"),
	OBTAIN UMETA(DisplayName = "Obtain"),
	HANDOVER UMETA(DisplayName = "HandOver")
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoMemberPart : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetNickName(const FString& NickName);
	void SetCombat(int32 Combat);
	void SetCharacterType(EPCClass PCClass);
	void SetCharacterLv(int32 Lv);
	void SetRank(int32 Rank);


UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelectItem")
	void SetHeroImage_BP(int32 CharType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInstance*>				RankImage;

protected:
	TWeakObjectPtr<UB2Image>			IMG_Character;
	TWeakObjectPtr<UB2Image>			IMG_Rank;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_NickName;
	TWeakObjectPtr<UTextBlock>			TB_CombatPower;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Lv;

};

DECLARE_DELEGATE(FGuildUesrClick)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoMemberPart1 : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetSlotText(FText Text);
	void SetDelegate(FGuildUesrClick Memberdelegate);
	void SetGuildChangeType(EGuildChangeType Type, int64 ID);

	FText GetMemberName();

	UFUNCTION()
		void OnClickSelect();

private:
	FGuildUesrClick			GuildUserDelegate;
	EGuildChangeType		ChangeType;
	int64					MemberID;
protected:
	TWeakObjectPtr<UB2Button>			BTN_Select;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Text;
};