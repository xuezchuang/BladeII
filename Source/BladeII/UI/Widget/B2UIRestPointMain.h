#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIRestPointMain.generated.h"

struct FRestRewardInfo
{
	FRestRewardInfo(int32 id, int32 itemid, float countper, int32 count)
		: Index(id)
		, RewardItem_Id(itemid)
		, Count_Per_Point(countper)
		, Count(count)
	{
	}

	explicit FRestRewardInfo()
		: Index(0)
		, RewardItem_Id(0)
		, Count_Per_Point(0)
		, Count(0)
	{
	}

	int32 Index = 0;
	int32 RewardItem_Id = 0;
	float Count_Per_Point = 0;
	int32 Count = 0;
};

/** Initially introduced to send request information to lobby scene to make 
 * our great elegant highness Remiel do some crazy sexy dance. */
enum class ERestPointRewardRequestState : uint8
{
	ERPRR_None,
	ERPRR_Normal,
	ERPRR_Double,
	ERPRR_End
};

UCLASS() 
class UB2UIRestPointMain : public UB2UIWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RestMain")
	TSubclassOf<class UB2UIRestPointReward> RestRewardClass;

protected:
	TWeakObjectPtr<UProgressBar> PB_RestPoint;

	TWeakObjectPtr<UB2Button> BTN_NormalReward;
	TWeakObjectPtr<UB2Button> BTN_DubbleReward;

	TWeakObjectPtr<UTextBlock> TB_RestTitle;
	TWeakObjectPtr<UTextBlock> TB_RewardPoint;
	TWeakObjectPtr<UTextBlock> TB_NeedRestPoint;
	TWeakObjectPtr<UTextBlock> TB_NeedRestPoint2;
	TWeakObjectPtr<UTextBlock> TB_NeedGem;

	TWeakObjectPtr<UTextBlock> STB_RestTitle;
	TWeakObjectPtr<UTextBlock> STB_DubbleReward;
	TWeakObjectPtr<UTextBlock> STB_NormalReward;

	TWeakObjectPtr<UHorizontalBox> List_Rest;
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
	
	FORCEINLINE ERestPointRewardRequestState GetLastRewardRequestState() const { return bCachedRewardRequestState; }

	UFUNCTION()
	void OnClicked_NormalReward();
	UFUNCTION()
	void OnClicked_DubbleReward();
private:
	void SetRewardList();
	void UpdateRewardList();
	void SetMyRestInfo();
	void OnSelectedReward(FRestRewardInfo RewardInfo);
	void OnResponseReward(const FB2ResponseRestRewardPtr& msgPtr);

	void ShowEnoughPointPopup();

	void SubscribeEvents();
	void UnsubscribeEvents();

	TArray<class UB2UIRestPointReward*> RewardItemArray;

	ERestPointRewardRequestState bCachedRewardRequestState;

	int32 DeliveryRestRewardTicket;
};

DECLARE_DELEGATE_OneParam(FButtonDelegate, FRestRewardInfo)
UCLASS()
class UB2UIRestPointReward : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetRewardItemInfo(FRestRewardInfo RewardInfo);
	FORCEINLINE FRestRewardInfo GetRewardInfo() { return CurrentRewardInfo; }

	void SetSelected(bool bSelected);
	bool GetSelected();

	UFUNCTION()
	void OnClicked_RewardItem();

	TWeakObjectPtr<UB2Button> BTN_Area;
	TWeakObjectPtr<UTextBlock> TB_RewardName;
	TWeakObjectPtr<UTextBlock> TB_RewardCount;
	TWeakObjectPtr<UPanelWidget> P_Check;
	TWeakObjectPtr<UImage> IMG_NormalBG;
	TWeakObjectPtr<UImage> IMG_CheckBG;
	TWeakObjectPtr<UBorder> P_Border;
	
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon;

	FButtonDelegate	ButtonDelegate;
private:
	FRestRewardInfo CurrentRewardInfo;
	bool bIsSelected = false;
};