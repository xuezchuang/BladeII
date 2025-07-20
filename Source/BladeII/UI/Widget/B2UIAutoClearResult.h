// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2Image.h"
#include <memory>
#include "../../Common/EventSubsystem.h"
#include "B2UIAutoClearResult.generated.h"

/**
*
*/
UCLASS()
class BLADEII_API UB2UIAutoClearResult : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetInfo(const FB2SweepStageInfo& InSweepStageinfo);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickBTNOneClear();
	UFUNCTION()
		void OnClickBTNRepeatAutoClear();
	UFUNCTION()
		void OnClickBTNOK();

	void ResetInfo();
	void RequestSweepStagePacket();

	void SubscribeEvents();
	void UnsubscribeEvents();
	void SetRepeat(bool InValue);
	void OpenRepeatEndPopup();
	void SetCharacterInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InCharInfo, bool IsMainCharLevelUp, bool IsSubCharLevelUp);
	void GetResultItemInfo(OUT TArray<FB2Item>& ResultItems, const FB2SweepStageInfo& InSweepStageinfo);
	void AddSlot(TArray<FB2Item>& InItems, int32 InRepeatCount, int32 InTotalGold);
	void CheckRepeat(bool IsMainCharLevelUp, bool IsSubCharLevelUp);
	void OpenLevelUpPopup(float InDelayTime);
	void LevelupFinish();

public:
	UPROPERTY(EditAnywhere, Category = "Slot")
		TSubclassOf<class UB2UIPAutoClearResultRow> AutoClearResultRowClass;
	UPROPERTY(EditAnywhere, Category = "PopupOption")
		float DelayTime;
	UPROPERTY(EditAnywhere, Category = "PopupOption")
		int32 RepeatMaxCount;
	//馆汗 家帕 吝老锭 饭骇诀 其捞瘤 掉饭捞 矫埃
	UPROPERTY(EditAnywhere, Category = "PopupOption")
		float RepeatLevelUpDelayTime;
	//饭骇诀 其捞瘤 掉饭捞 矫埃
	UPROPERTY(EditAnywhere, Category = "PopupOption")
		float LevelUpDelayTime;

private:
	TWeakObjectPtr<UImage> IMG_On;
	TWeakObjectPtr<UImage> IMG_Off;
	TWeakObjectPtr<UB2Image> FX_On;
	TWeakObjectPtr<UThrobber> T_On;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_AutoClear;
	TWeakObjectPtr<UTextBlock> TB_AutoCount;
	TWeakObjectPtr<UTextBlock> TB_OneClear;
	TWeakObjectPtr<UTextBlock> TB_RepeatAutoClear;
	TWeakObjectPtr<UTextBlock> TB_OK;
	TWeakObjectPtr<UTextBlock> TB_Exp;
	TWeakObjectPtr<UTextBlock> TB_Default;
	TWeakObjectPtr<UTextBlock> TB_TripleInfo;
	TWeakObjectPtr<UB2ScrollBox> SB_ResultItem;
	TWeakObjectPtr<UB2Button> BTN_OneClear;
	TWeakObjectPtr<UB2Button> BTN_RepeatAutoClear;
	TWeakObjectPtr<UB2Button> BTN_OK;
	TWeakObjectPtr<class UB2UIPAutoClearResultCharacter> UIP_MainChar;
	TWeakObjectPtr<class UB2UIPAutoClearResultCharacter> UIP_SubChar;
	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;
	TArray<TWeakObjectPtr<class UB2UIPAutoClearResultRow>> RowSlots;
	int32 ServerTypeMainChar;
	int32 ServerTypeSubChar;
	FServerStageID ServerStageId;
	bool IsRepeat;
	int32 RepeatCount;
	TArray<std::shared_ptr<issue>> Issues;
	FTimerHandle SweepStagePacketTH;
};
