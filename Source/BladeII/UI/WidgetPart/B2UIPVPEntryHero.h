#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIPVPEntryCharacters.h"
#include "B2UIPVPEntryHero.generated.h"

enum class EEntryType : int8
{
	EntryType_None,
	EntryType_Select1,
	EntryType_Select2,
	EntryType_Team,
	EntryType_MainChar,
};

DECLARE_DELEGATE_OneParam(FUB2UIPVPEntryHero_OnClickBtnSelect, EPCClass);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPEntryHero : public UB2UIWidgetBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Init() override;
	void SetEntryInfo(EPCClass PCClass, EEntryType EntryType, int32 ClassLevel);
	void SetVisibilityWidgets(EEntryType EntryType);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
public:
	UFUNCTION()
	void OnClickBtnSelect();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> PortraitInfo;
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> Inactive_PortraitInfo;

private:
	TWeakObjectPtr<UOverlay> OV_MainSelect;
	TWeakObjectPtr<UOverlay> OV_SubSelect;
	TWeakObjectPtr<UOverlay> OV_TeamSelect;
	TWeakObjectPtr<UImage> IMG_Portrait_Active;
	TWeakObjectPtr<UImage> IMG_Portrait_Effec;
	TWeakObjectPtr<UImage> IMG_Portrait_De;
	TWeakObjectPtr<UTextBlock> TB_Level_De;
	TWeakObjectPtr<UTextBlock> TB_Level;
	TWeakObjectPtr<UB2Button> BTN_Select;
	TWeakObjectPtr<UVerticalBox> VB_De;
	TWeakObjectPtr<UVerticalBox> VB_Active;

public:
	FUB2UIPVPEntryHero_OnClickBtnSelect FOnClickBtnSelect;

private:
	EPCClass		CurPCClass;
	int32			CurClassLevel;
};
