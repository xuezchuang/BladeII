#pragma once

#include "B2UIWidget.h"
#include "B2LobbySceneManager.h"
#include "B2UISingleModSelect.h"
#include "B2UIModSelect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UB2UIModSelect : public UB2UIWidget
{
	GENERATED_BODY()

protected:

	/** If true, each mod selection part location will be dynamically updated by 3d world marker */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Chapter")
	uint32 bExternalDynamicSetupReady : 1;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Chapter")
	float TimeAutoScrolling;

public:

	UB2UIModSelect(const FObjectInitializer& ObjectInitializer);
	void OnClickPopUpProcess(ELobbyModSceneElem InModEnum);
private:
	void SelectModOpenPopUp(ELobbyModSceneElem InModEnum, bool bLock = false);

private:
	virtual void Init() override;
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
	void OnClickBtnTagMatch();

	UFUNCTION()
	void OnClickBtnDayDungeon();

	UFUNCTION()
	void OnClickBtnHeroTop();

	UFUNCTION()
	void OnClickBtnTeamMatch();

	UFUNCTION()
	void OnClickBtnRaid();

	UFUNCTION()
	void OnClickBtnGuildMatch();

	UFUNCTION()
	void OnClickBtnControl();

public:

	/** Update each mode button widget position by Position2D, the array element are supposed to have the same order to ELobbyModSceneElem. */
	void UpdateModUIPosition(const TArray<FVector2D>& Positions2D);


private:

	TWeakObjectPtr<UImage> IMG_BG;

	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_PVPP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_DayDungeonP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_HeroTopP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_TeamMatchP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_RaidP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_GuildMatchP;
	TWeakObjectPtr<UB2UISingleModSelect> UIP_ModSelect_ControlP;

	UB2UISingleModSelect* GetModSelectWidget(ELobbyModSceneElem InModeScene); // ELobbyModSceneElem to selection widget mapping.
};