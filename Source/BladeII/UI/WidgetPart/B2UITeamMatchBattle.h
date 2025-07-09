#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "B2UITeamMatchBattle.generated.h"

using BattleSlotArray = TArray<TWeakObjectPtr<class UB2UITeamMatchBattleSlot>>;

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchBattle : public UB2UIWidget, public IB2UIBackWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UITeamMatchBattle(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetReadyLeftSlot(int32 SlotIdx, EPCClass CharacterClass, int32 Lv, int8 FormationNum = 0);
	void SetReadyRightSlot(int32 SlotIdx, EPCClass CharacterClass, int32 Lv);

	void SetLeftPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star);
	void SetRightPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star);

	void SetBattleTime(int32 BattleTiem);

	void SetSlotCamera(bool bSet, bool bLeft, int32 SlotIdx);
	void SetSlotDeath(bool bSet, bool bLeft, int32 SlotIdx);

	void SetDamagePercent(bool IsMyTeam, const TArray<float>& TotalDamages);
	
	void ShowResultImage(const ENetMatchResult& result);

	void OnSlotChangedHealthAndShieldCommon(bool bMyTeam, int32 SlotIdx);
	void OnSlotChangedOnClickCharacter(bool bMyTeam, int32 SlotIdx);

	/** To do some opening animation, which was invoked at OnOpen_BP, but was not visible at that time. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void OnPVPBeginEventSceneEnd();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayFocusTimer_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopFocusTimer_BP();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayResultImage_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopResultImage_BP();

	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayAnimationEvent_AnimResultAction(ENetMatchResult InMatchResult);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void BindDocChar(int32 slotIndex, class UB2UIDocHero* DocHero);
	void BindDocSkill(int32 SkillInputIndex, class UB2UIDocSkill* SkillDoc);

	BattleSlotArray& GetBattleSlot(bool IsMyTeam);

	UFUNCTION()
	void OnClickCamButton();

	UFUNCTION()
	void OnClickPauseButton();

	UFUNCTION()
	void OnPressedSkillBtn(class UB2UIBattleSkill* ClickedSkill);
protected:
	virtual void CloseWidgetDelegate() override;

	/* Name rules for obtaining BPSkillAssets in the code. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	FString PrefixSkillAsset = TEXT("");

	TWeakObjectPtr<class UB2UITeamMatchBattleNickName>	LeftNickName;
	BattleSlotArray LeftSlots;

	TWeakObjectPtr<class UB2UITeamMatchBattleNickName>	RightNickName;
	BattleSlotArray RightSlots;
	
	TArray<class UB2UIBattleSkill*>		SkillAssets;

	TWeakObjectPtr<UButton>				BTN_ChangeCam;
	TWeakObjectPtr<UTextBlock>			TB_BattleTime;
	TWeakObjectPtr<UImage>				IMG_FocusTimer;

	TWeakObjectPtr<UButton>				BTN_Pause;

	TWeakObjectPtr<UImage>				IMG_LongCamera;
	TWeakObjectPtr<UImage>				IMG_ShortCamera;

	TWeakObjectPtr<UImage>				IMG_Victory;
	TWeakObjectPtr<UImage>				IMG_Lose;
	TWeakObjectPtr<UImage>				IMG_Draw;
};