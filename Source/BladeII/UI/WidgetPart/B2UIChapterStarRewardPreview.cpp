#include "BladeII.h"
#include "B2UIChapterStarRewardPreview.h"
#include "B2UIRewardEventSet.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2ItemInfo.h"
#include "B2UIHeader.h"

#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIChapterStarRewardPreview::UB2UIChapterStarRewardPreview(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCounterDungeonReward = false;
	bCanGetReward = false;
	ChapterNum = 1;
	Difficulty = EStageDifficulty::ESD_None;
	MyRewardIndex = INDEX_NONE;
	RewardTemplate = NULL;
	RewardIcon_Gold = NULL;
	RewardIcon_Gem = NULL;
}

void UB2UIChapterStarRewardPreview::Init()
{
	Super::Init();

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIChapterStarRewardPreview::CacheAssets()
{
	GET_SLOT(UHorizontalBox, X_Rewards)
	GET_SLOT(UTextBlock, TB_Title)
	GET_SLOT(UTextBlock, TB_RewardStarCount)
	GET_SLOT(UTextBlock, TB_GetReward)
	GET_SLOT(UB2Button, BTN_GetReward)
	GET_SLOT(UB2Button, BTN_Close)
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods)
}

void UB2UIChapterStarRewardPreview::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardPreviewTitle")));
	}
	if (TB_GetReward.IsValid())
	{
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardGet")));
	}
}

void UB2UIChapterStarRewardPreview::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIChapterStarRewardPreview::OnClickBtnGetReward)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIChapterStarRewardPreview::OnClickBtnClose)
}

void UB2UIChapterStarRewardPreview::ShowMe(bool bInCanGetReward, int32 InChapterNum, EStageDifficulty InDifficulty, int32 InRewardIndex, int32 InMyCurrStarCount, int32 InMyRequiredStarCount, TArray<b2network::B2RewardPtr> InRewards)
{
	bCanGetReward = bInCanGetReward;
	ChapterNum = InChapterNum;
	Difficulty = InDifficulty;
	MyRewardIndex = InRewardIndex;


	if (BTN_GetReward.IsValid())
	{
		BTN_GetReward->SetIsEnabled(bCanGetReward ? true : false);
	}

	DestroyRewardIcons();

	for (auto InRewardItem : InRewards)
	{
		CreateRewardIconCommon(InRewardItem,FText::AsNumber(InRewardItem->raw_reward->count));
	}

	SetPopupVisible(true);

	if (TB_RewardStarCount.IsValid())
	{
		TB_RewardStarCount->SetText(FText::AsNumber(InMyRequiredStarCount));
	}

	OnPopupBP();
	IsOnStar(true);
	// [fixme][@AKI, 170430] 일단 IsOnStar(false)인걸로 보아 여기는 Gold만 들어옴으로 이렇게 함.
	// 추후에 RewardID를 받아오면 맞게 변경해야 함.
	if (BTN_Goods.IsValid())
	{
		BTN_Goods->SetItemInfo(FItemRefIDHelper::GetGoodsID_Gem());
	}
}

void UB2UIChapterStarRewardPreview::JoinShowMe(bool bInCanGetReward, int32 InRewardIndex, int32 InMyJoinCount, int32 InMyRequiredJoinCount)
{
	bCounterDungeonReward = bInCanGetReward;
	MyRewardIndex = InRewardIndex;
	CounterDungeonClear = InMyRequiredJoinCount;

	if (BTN_GetReward.IsValid())
		BTN_GetReward->SetIsEnabled(bCounterDungeonReward ? true : false);

	SetPopupVisible(true);

	if (TB_RewardStarCount.IsValid())
	{
		TB_RewardStarCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDungeon_WeeklyReward"))
			, FText::AsNumber(InMyRequiredJoinCount)));

	}
	OnPopupBP();
	IsOnStar(false);
	// [fixme][@AKI, 170430] 일단 IsOnStar(false)인걸로 보아 여기는 Gold만 들어옴으로 이렇게 함.
	// 추후에 RewardID를 받아오면 맞게 변경해야 함.
	if (BTN_Goods.IsValid())
	{
		BTN_Goods->SetItemInfo(FItemRefIDHelper::GetGoodsID_Gold());
	}
}


void UB2UIChapterStarRewardPreview::CloseMe()
{
	DestroyRewardIcons();
	SetPopupVisible(false);

	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI != nullptr)
		HeaderUI->SetForcedBlockHeader(false);
}

bool UB2UIChapterStarRewardPreview::IsOpen()
{
	return (this->GetVisibility() != ESlateVisibility::Hidden && this->GetVisibility() != ESlateVisibility::Collapsed);
}

void UB2UIChapterStarRewardPreview::CreateRewardIcons(int32 InGoldAmount, int32 InGemAmount)
{
	DestroyRewardIcons();

	if (!RewardTemplate || !X_Rewards.IsValid()){
		return;
	}
	// Reward 별로 하나씩 생성한다.
	if (InGoldAmount > 0)
	{
		//[@AKI, 170605] Goods를 설정하기 위해 Icon을 넣는 방식에서 Index를 넣는 방식으로 변경
		//CreateRewardIconCommon(RewardIcon_Gold, FText::AsNumber(InGoldAmount));
		CreateRewardIconCommon(Reward_GoldIdx, FText::AsNumber(InGoldAmount));
	}
	if (InGemAmount > 0)
	{
		//[@AKI, 170605] Goods를 설정하기 위해 Icon을 넣는 방식에서 Index를 넣는 방식으로 변경
		//CreateRewardIconCommon(RewardIcon_Gem, FText::AsNumber(InGemAmount));
		CreateRewardIconCommon(Reward_GemIdx, FText::AsNumber(InGemAmount));
	}
}

void UB2UIChapterStarRewardPreview::CreateRewardIconCommon(class UMaterialInterface* InIconMat, const FText& InIconText)
{
	UB2UIRewardIcon* CreatedIcon = Cast<UB2UIRewardIcon>(DynCreateInHorizontalBox(RewardTemplate, this, X_Rewards.Get()));
	if (CreatedIcon){
		CreatedIcon->Init();
		UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(CreatedIcon->Slot);
		if (HBSlot)
		{ // 아 일단 필요없음. 혹시라도 필요해지면.. 일단은 DynCreateInHorizontalBox 쪽에 추가 매개변수 넘겨주는 걸 먼저 해 보고..
			//HBSlot->Size.SizeRule = ESlateSizeRule::Fill;
			//HBSlot->SetHorizontalAlignment(HAlign_Center);
		}

		CreatedIcon->SetIconAndText(InIconMat, InIconText);
		CreatedRewardIcons.Add(CreatedIcon);
	}
}

void UB2UIChapterStarRewardPreview::CreateRewardIconCommon(ERewardType _Type, const FText& _InIconText)
{
	CreateRewardIconCommon(static_cast<int32>(_Type), _InIconText);
}

void UB2UIChapterStarRewardPreview::CreateRewardIconCommon(int32 _rewardType, const FText& _InIconText)
{
	UB2UIRewardIcon* CreatedIcon = Cast<UB2UIRewardIcon>(DynCreateInHorizontalBox(RewardTemplate, this, X_Rewards.Get()));
	if (CreatedIcon) {
		CreatedIcon->Init();
		UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(CreatedIcon->Slot);
		if (HBSlot)
		{ // 아 일단 필요없음. 혹시라도 필요해지면.. 일단은 DynCreateInHorizontalBox 쪽에 추가 매개변수 넘겨주는 걸 먼저 해 보고..
		  //HBSlot->Size.SizeRule = ESlateSizeRule::Fill;
		  //HBSlot->SetHorizontalAlignment(HAlign_Center);
		}

		CreatedIcon->UpdateRewardIcon(_rewardType, _InIconText);
		CreatedRewardIcons.Add(CreatedIcon);
	}
}

void UB2UIChapterStarRewardPreview::CreateRewardIconCommon(b2network::B2RewardPtr InRewardItem, const FText& _InIconText)
{
	if (UB2UIRewardIcon* CreatedIcon = Cast<UB2UIRewardIcon>(DynCreateInHorizontalBox(RewardTemplate, this, X_Rewards.Get())))
	{
		CreatedIcon->Init();
		CreatedIcon->UpdateRewardIcon(InRewardItem);
		CreatedRewardIcons.Add(CreatedIcon);
	}
}

void UB2UIChapterStarRewardPreview::SetRewardTemplateClass(TSubclassOf<class UB2UIRewardIcon> InNewTemplate)
{ 
	// 동적 생성하는 케이스에서 RewardTemplate 까지 따로 넣어주어야 해서 이런 짓을 함. 
	// RewardTemplate 이 필요해지기 전에 세팅해야
	RewardTemplate = InNewTemplate;
}

void UB2UIChapterStarRewardPreview::DestroyRewardIcons()
{
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	for (UB2UIRewardIcon* ThisIcon : CreatedRewardIcons)
	{
		if (ThisIcon){
			ThisIcon->DestroySelf(UIMgr);
		}
	}
	CreatedRewardIcons.Empty();
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIChapterStarRewardPreview::OnClickBtnGetReward()
{
	if (bCanGetReward) // False 면 애당초 버튼 클릭이 불가능해야 함.
	{ // 보상 요청에는 인덱스가 없으므로 여기서 이전 보상을 받았는지 여부에 따라 요청을 보낼지 어떨지 체크해야 할 수도 있음.
		// 이전에 받지 않은 게 있다면 GetReward 버튼을 비활성화 하는 게 나을 듯.
		BladeIIGameImpl::GetStageDataStore().RequestGetChapterClearReward(ChapterNum, Difficulty, MyRewardIndex);
		CloseMe(); // 받자마자 닫아야 연출도 보고 또 헛된 응답도 안하고.
	}
	else if (bCounterDungeonReward)
	{
		data_trader::Retailer::GetInstance().RequestWeeklyRewardCounterDungeon(MyRewardIndex);
		CloseMe();
	}
}

void UB2UIChapterStarRewardPreview::OnClickBtnClose()
{
	CloseMe();
}

void UB2UIChapterStarRewardPreview::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIChapterStarRewardPreview::SetPopupVisible(bool IsVisible)
{
	if (IsVisible && this->GetVisibility() != ESlateVisibility::Visible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
	else if(IsVisible == false && this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		this->SetVisibility(ESlateVisibility::Collapsed); // 내부 아이콘들은 destroy 하지만 자신은 숨기기만 함.
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}