#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2LobbyUI_ItemHeroChangeMain.generated.h"

/**
*
*/
USTRUCT()
struct FHeroItemChangeCharacterSlot
{
	GENERATED_BODY()

	TWeakObjectPtr<UOverlay>		O_CharacterOverlay;
	TWeakObjectPtr<UB2Button>		BTN_Character;

	TWeakObjectPtr<UOverlay>		O_CharacterOnOverlay;
	TWeakObjectPtr<UTextBlock>		TB_CharacterName;
	TWeakObjectPtr<UTextBlock>		TB_CharacterName2;

	TWeakObjectPtr<UOverlay>		O_CharacterInActive;
};

class issue;
using issue_ptr = std::shared_ptr<issue>;

UCLASS()
class BLADEII_API UB2LobbyUI_ItemHeroChangeMain : public UB2UIWidget, public IB2UIBackWidget
{

	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	//virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void OnOpen(bool RightNow = false) override;


	virtual void OnClose(bool RightNow = false) override;

	void InitBase();
	void SubscribeEvents();
	void UnSubscribeEvents();

	virtual void UpdateStaticText() override;

	virtual void CloseWidgetDelegate() override;


	UFUNCTION()
		void OnClickClose();

	UFUNCTION()
		void OnClickGladiator();
	UFUNCTION()
		void OnClickAssasin();
	UFUNCTION()
		void OnClickWizard();
	UFUNCTION()
		void OnClickFighter();

	UFUNCTION()
		void OnClickChangeItem();

	UFUNCTION(BlueprintCallable)
		void ResultFunction();


	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void ResultStartAnimation();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sfx_Sound")
		USoundBase*					Sfx_Sound;
public:
	void DestroyTargetItemIcon();

	void CloseAndQuitItemOpMode();
	void UpdateItemIcons();

	void SetCharacterSlotState(EPCClass InClass, bool InState);
	void SetInActive(EPCClass InClass);
	void SetCostInfo(int32 InDarkCrystalCost, TArray<FB2Item>	InChangeItem);
	void SetCostText(int32 InCost);

	void IngredItemUpdate(EPCClass InPCClass);
	void ServerResultItemHeroChangeMain(const FB2ResponseChangeItemCharacterTypePtr& InSeverPtr);

	void ChangeButtonActive();

private:
	int32 DarkCrystalCost;
	TArray<FB2Item>		ChangeItem;

	uint32 ChangeItemCharacterTypeTicket;

	EPCClass ChoicePCClass;

protected:
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
		class UB2LobbyInventory* CachedInventory;

	

	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> TargetItemIcon;


	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> IngredItemIcon;


	FB2Item TargetItem;
	FB2Item SourceItem;
	FB2Item ResultItem;

	TArray<issue_ptr>	Issues;


	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeCost;
	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeTitle;
	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeBtn;

	TWeakObjectPtr<UB2Button> BTN_EnhanceChange;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_BackgroundBlock;
	TWeakObjectPtr<UB2Button> BTN_SkipResult;

	TWeakObjectPtr<UCanvasPanel> CP_TargetItemIconSet;
	TWeakObjectPtr<UCanvasPanel> CP_IngredItemIconSet;


	TArray<FHeroItemChangeCharacterSlot>		CharacterSlot;

	//static Text

	TWeakObjectPtr<UTextBlock> TB_HeroChangeTitle;
	TWeakObjectPtr<UTextBlock> TB_HeroChangeBtn;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> TB_HeroChageGuide;

	TWeakObjectPtr<UWidgetAnimation> ChangItemResultAni;

private:
	FTimerHandle TimeHandler;

};
