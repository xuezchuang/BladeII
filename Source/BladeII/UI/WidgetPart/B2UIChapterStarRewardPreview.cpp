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
	// [fixme][@AKI, 170430] 老窜 IsOnStar(false)牢吧肺 焊酒 咯扁绰 Gold父 甸绢咳栏肺 捞犯霸 窃.
	// 眠饶俊 RewardID甫 罐酒坷搁 嘎霸 函版秦具 窃.
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
	// [fixme][@AKI, 170430] 老窜 IsOnStar(false)牢吧肺 焊酒 咯扁绰 Gold父 甸绢咳栏肺 捞犯霸 窃.
	// 眠饶俊 RewardID甫 罐酒坷搁 嘎霸 函版秦具 窃.
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
	// Reward 喊肺 窍唱究 积己茄促.
	if (InGoldAmount > 0)
	{
		//[@AKI, 170605] Goods甫 汲沥窍扁 困秦 Icon阑 持绰 规侥俊辑 Index甫 持绰 规侥栏肺 函版
		//CreateRewardIconCommon(RewardIcon_Gold, FText::AsNumber(InGoldAmount));
		CreateRewardIconCommon(Reward_GoldIdx, FText::AsNumber(InGoldAmount));
	}
	if (InGemAmount > 0)
	{
		//[@AKI, 170605] Goods甫 汲沥窍扁 困秦 Icon阑 持绰 规侥俊辑 Index甫 持绰 规侥栏肺 函版
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
		{ // 酒 老窜 鞘夸绝澜. 趣矫扼档 鞘夸秦瘤搁.. 老窜篮 DynCreateInHorizontalBox 率俊 眠啊 概俺函荐 逞败林绰 吧 刚历 秦 焊绊..
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
		{ // 酒 老窜 鞘夸绝澜. 趣矫扼档 鞘夸秦瘤搁.. 老窜篮 DynCreateInHorizontalBox 率俊 眠啊 概俺函荐 逞败林绰 吧 刚历 秦 焊绊..
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
	// 悼利 积己窍绰 纳捞胶俊辑 RewardTemplate 鳖瘤 蝶肺 持绢林绢具 秦辑 捞繁 窿阑 窃. 
	// RewardTemplate 捞 鞘夸秦瘤扁 傈俊 技泼秦具
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
	if (bCanGetReward) // False 搁 局寸檬 滚瓢 努腐捞 阂啊瓷秦具 窃.
	{ // 焊惑 夸没俊绰 牢郸胶啊 绝栏骨肺 咯扁辑 捞傈 焊惑阑 罐疽绰瘤 咯何俊 蝶扼 夸没阑 焊尘瘤 绢冻瘤 眉农秦具 且 荐档 乐澜.
		// 捞傈俊 罐瘤 臼篮 霸 乐促搁 GetReward 滚瓢阑 厚劝己拳 窍绰 霸 唱阑 淀.
		BladeIIGameImpl::GetStageDataStore().RequestGetChapterClearReward(ChapterNum, Difficulty, MyRewardIndex);
		CloseMe(); // 罐磊付磊 摧酒具 楷免档 焊绊 肚 顷等 览翠档 救窍绊.
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
		this->SetVisibility(ESlateVisibility::Collapsed); // 郴何 酒捞能甸篮 destroy 窍瘤父 磊脚篮 见扁扁父 窃.
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}