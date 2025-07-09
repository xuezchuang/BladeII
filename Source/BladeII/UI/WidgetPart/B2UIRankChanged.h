#pragma once

#include "B2UIWidgetBase.h"
#include "B2RichTextBlock.h"
#include "B2UIRankerInfo.h"
#include "B2UITeamRankerInfo.h"
#include "B2UIRankChanged.generated.h"

DECLARE_DELEGATE(FCloseButtonDelegate)

UCLASS(BlueprintType, Blueprintable)
class UB2UIRankChanged : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIRankChanged(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnRankChange();

	void SetUsingGameMode(EB2GameMode ModeType);

	void SetMyRankingData(const FModRankerInfo& RankerInfo);
	void SetOtherRankingData(const FModRankerInfo& RankerInfo);

	void SetChangedRanking(int32 DiffRanking);

	template<typename FunctorType>
	inline void ButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
		void OnClickedConfirmBTN();

private:
	TWeakObjectPtr<UCanvasPanel> CP_MyRanking;
	TWeakObjectPtr<UCanvasPanel> CP_OtherRanking;

	TWeakObjectPtr<UB2UIRankerInfo> UIP_MyRankingInfo_TagMatch;
	TWeakObjectPtr<UB2UITeamRankerInfo> UIP_MyRankingInfo_TeamMatch;
	TWeakObjectPtr<UB2UIRankerInfo> UIP_MyRankingInfo_ControlMatch;
	TWeakObjectPtr<UB2UIRankerInfo> UIP_OtherRankingInfo_TagMatch;
	TWeakObjectPtr<UB2UITeamRankerInfo> UIP_OtherRankingInfo_TeamMatch;
	TWeakObjectPtr<UB2UIRankerInfo> UIP_OtherRankingInfo_ControlMatch;

	TWeakObjectPtr<UB2RichTextBlock> RTB_RankChanged;

	TWeakObjectPtr<UB2Button> BTN_Confirm;

	EB2GameMode UsingGameModeType;

	FCloseButtonDelegate	ButtonDelegate;
};