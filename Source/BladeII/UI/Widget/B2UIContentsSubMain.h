#pragma once

#include "B2UIWidget.h"
#include "B2UIContentsSubMain.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIContensSubMain : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void RegisterUIMarkForRedDot() final;
	UFUNCTION(BlueprintImplementableEvent)
		void DoMarkRedDot_BP(int32 index, bool bMark);
	UFUNCTION(BlueprintImplementableEvent)
		void SetUseLock_BP(bool bLock);

	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoInvenScene();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoSkillScene();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoBrevetScene();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoWingScene();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoConstantheroScene();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoEtherSecne();
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoNotYetSecne();


	UFUNCTION()
		bool HasRedDot_Inventory();
	UFUNCTION()
		bool HasRedDot_Skill();
	UFUNCTION()
		bool HasRedDot_Brevet();
	UFUNCTION()
		bool HasRedDot_Wing();
	UFUNCTION()
		bool HasRedDot_Relic();
	UFUNCTION()
		bool HasRedDot_Ether();

private:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;	

protected:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	TWeakObjectPtr<UUserWidget>	UIP_ContentSubInventory;
	TWeakObjectPtr<UUserWidget>	UIP_ContentSubSkill;
	TWeakObjectPtr<UUserWidget>	UIP_ContentSubBrevet;
	TWeakObjectPtr<UUserWidget>	UIP_ContentSubWing;
	TWeakObjectPtr<UUserWidget>	UIP_ContentSubRelic;
	TWeakObjectPtr<UUserWidget>	UIP_ContentSubEther;

};
