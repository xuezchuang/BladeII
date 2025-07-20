#include "B2UIBattleStageInfo.h"
#include "B2UISlotItem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "B2UIDocBattle.h"
#include "B2UIDocUICondition.h"
#include "B2StageInfo.h"
#include "B2ItemInfo.h"
#include "Event.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"
#include "B2UISlotStageBuffItem.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIHeader.h"
#include "B2UIStartButton.h"
#include "B2StageManager.h"
#include "B2ChapterInfo.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIBattleStageInfoToolTip.h"
#include "Global.h"
#include "BladeIIGameImpl.h"

UB2UIBattleStageInfo::UB2UIBattleStageInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BattleRewardItems.Empty();
	CachedCurrentStageInfoData = nullptr;
	ClientStageId = INDEX_NONE;
	ServerStageId = FServerStageID();
	StageDifficulty = EStageDifficulty::ESD_Normal;

	StageGameStarted = false;
}

void UB2UIBattleStageInfo::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_BattleReady);
	GET_SLOT(UB2ScrollBox, SB_RewardItems);
	
	GET_SLOT(UB2RichTextBlock, TB_StageStory);
	GET_SLOT(UB2Image, IMG_MainTexture);
	GET_SLOT(UB2Image, IMG_NPC);
	GET_SLOT(UB2RichTextBlock, TB_TitleName);
	GET_SLOT(UWidgetSwitcher, WS_ChangeDiffImage);
	GET_SLOT(UB2UIBattleStageInfoToolTip, UIP_BattleStageInfoToolTip);
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->Init();
	}

	WS_ClearResultStar.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[0], WS_ClearResultStar1);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[1], WS_ClearResultStar2);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[2], WS_ClearResultStar3);

	TB_ClearResult.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[0], TB_ClearResult1);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[1], TB_ClearResult2);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[2], TB_ClearResult3);
}

void UB2UIBattleStageInfo::UpdateStaticText()
{
	// 捞芭 StaticText 技泼 喉风橇赴飘俊 腹捞 乐澜.

}

void UB2UIBattleStageInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_BattleReady, &UB2UIBattleStageInfo::OnClickBattleReady)
}

void UB2UIBattleStageInfo::Init()
{
	Super::Init();
	SubscribeEvent();
}

void UB2UIBattleStageInfo::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	StageGameStartedResetCB();

	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIBattleStageInfo::BeginDestroy()
{
	UnsubscribeEvent();
	Super::BeginDestroy();
}

void UB2UIBattleStageInfo::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIBattleStageInfo);

	CancelStageGameStartTicket = CancelStartGameStageClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT()
			Capture->CancelStageGameStart();
		END_CAPTURE_OBJECT()
		);
}

void UB2UIBattleStageInfo::UnsubscribeEvent()
{
	CancelStartGameStageClass<>::GetInstance().Unsubscribe(CancelStageGameStartTicket);
}

void UB2UIBattleStageInfo::SetHeaderTitleAsChapterNum()
{
	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (CachedCurrentStageInfoData && DocBattleStage) // CachedCurrentStageInfoData 掘绢柯 矫痢 捞饶咯具
	{
		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)StageDifficulty));
		FText StageText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()), CachedCurrentStageInfoData->GetLocalizedStageName());
		
		SetLobbyUIHeaderTitle(StageText);
	}
}

void UB2UIBattleStageInfo::SetStageDiffulty()
{
	if (TB_TitleName.IsValid())
	{
		UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();

		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)StageDifficulty));
		FText StageText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()), ChapterInfoTable->GetLocalizedDifficultyName(StageDifficulty));
		TB_TitleName->SetText(StageText);
	}

	WS_ChangeDiffImage->SetActiveWidgetIndex((int32)StageDifficulty - 1);
}

void UB2UIBattleStageInfo::BindDocAuto()
{
	UnbindDoc();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBattleStage);
	if (DocBattleStage)
	{
		ClientStageId = DocBattleStage->GetClientStageId();
		StageDifficulty = DocBattleStage->GetStageDifficulty();
		ServerStageId = DocBattleStage->GetServerStageId();

		//Cache StageInfoData

		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		int32 ChapterNum = StageInfoTable ? StageInfoTable->GetChapterNumByClientStageId(ClientStageId) : 0;
		int32 StageNum = StageInfoTable ? StageInfoTable->GetSubChapterNumByClientStageId(ClientStageId) : 0;
		
		CachedCurrentStageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(ClientStageId, StageDifficulty) : NULL;

		SetHeaderTitleAsChapterNum(); // CachedCurrentStageInfoData 掘绢柯 捞饶..
		SetStageDiffulty();

		if (CachedCurrentStageInfoData)
		{
			SetStageMainTexture(CachedCurrentStageInfoData->GetStageTexture());
			SetStageStory(CachedCurrentStageInfoData->GetLocalizedStageStory());
			SetNPCTexture(CachedCurrentStageInfoData->GetNPCTexture());
		}

		// 焊胶 炼阿捞 焊惑 酒捞袍 焊促 刚历 唱客具 登辑 菊俊 眠啊 窃
		FStageFixedDropsInfo* const BossPiece = DocBattleStage->GetBossDropItemsList();
		AddBossPieceRewardItem(BossPiece);
		// 厘厚焊促 俊抛福 炼阿捞 刚历
		const TArray<FB2AetherItemPreviewInfo>& AetherItems = DocBattleStage->GetAetherItemList();
		for (int AetherCnt = 0; AetherCnt < AetherItems.Num(); ++AetherCnt)
		{
			AddAetherItem(AetherItems[AetherCnt]);
		}
		// 犁丰 酒捞袍 Preview
		const TArray<FB2MaterialItemPreviewInfo>& MaterialItems = DocBattleStage->GetMaterialItemList();
		for (int MaterialCnt = 0; MaterialCnt < MaterialItems.Num(); ++MaterialCnt)
		{
			AddMaterialItem(MaterialItems[MaterialCnt]);
		}
		// 焊惑酒捞袍 贸府. 官拆老 绝栏骨肺 Delegate贸府 鞘夸绝澜
		const TArray<FB2RewardItemPreviewInfo>& RewardItems = DocBattleStage->GetRewardItemList();
		for (int RewardCnt = 0; RewardCnt < RewardItems.Num(); ++RewardCnt)
		{
			AddRewardItem(RewardItems[RewardCnt]);
		}

		//SetRepeatMissionReward(DocBattleStage->GetRepeatMissionReward());

		SetStageGradeInfo(DocBattleStage->GetStageClearConditions());

		// 檬扁柳涝篮 公炼扒 檬扁拳
		TArray<EStageBuffType> StageBuffs;
		DocBattleStage->SetSelectedStageBuffs(StageBuffs);
	}
	
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		//皋牢康旷苞 挤康旷狼 单捞磐甫 啊廉咳
		MainHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
		SubHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());
		
		//皋牢康旷苞 挤康快捞 官差菌阑锭 贸府
		DocBattle->OnCurPCClassChanged.AddUObject(this, &UB2UIBattleStageInfo::OnChangedCurPCClass);
		DocBattle->OnTagPCClassChanged.AddUObject(this, &UB2UIBattleStageInfo::OnChangedTagPCClass);

		UpdateHerosPower();
	}
}

void UB2UIBattleStageInfo::SetStageGradeInfo(const TArray<bool>& Grade)
{
	for (int32 Index = 0; Index < Grade.Num(); ++Index)
	{
		if (false == WS_ClearResultStar.IsValidIndex(Index) || nullptr == WS_ClearResultStar[Index])
		{
			BII_CHECK(false);
			continue;
		}

		WS_ClearResultStar[Index]->SetActiveWidgetIndex(Grade[Index] ? 0 : 1);
	}

	if (TB_ClearResult.IsValidIndex(0) && TB_ClearResult[0].IsValid())
	{
		TB_ClearResult[0]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[0] ? TEXT("Stage_Grade_Clear_1") : TEXT("Stage_Grade_1")));
	}
	if (const auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		if (TB_ClearResult.IsValidIndex(1) && TB_ClearResult[1].IsValid())
		{
			TB_ClearResult[1]->SetText(GetStageClearGradeInfoText(DocBattleStage->GetServerStageId(), 3, Grade[1]));
		}
	}
	if (TB_ClearResult.IsValidIndex(2) && TB_ClearResult[2].IsValid())
	{
		TB_ClearResult[2]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[2] ? TEXT("Stage_Grade_Clear_3") : TEXT("Stage_Grade_3")));
	}
}

void UB2UIBattleStageInfo::UnbindDoc()
{
	ClearRewardItems();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnBattleDifficultyChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionCurNumChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionMaxNumChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionRewardChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleBoostOnChanged.RemoveAll(this);
	}

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnCurPCClassChanged.RemoveAll(this);
		DocBattle->OnTagPCClassChanged.RemoveAll(this);
	}

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{ 
		DocUser->OnGoldChanged.RemoveAll(this);
	}

	for (UB2UISlotItem* RewardSlotItem : BattleRewardItems)
	{
		if (RewardSlotItem)
		{
			RewardSlotItem->UnbindDoc();
		}
	}

	ClientStageId = INDEX_NONE;
	ServerStageId = FServerStageID();

	MainHeroDoc = nullptr;
	SubHeroDoc = nullptr;
	SetDoc(nullptr);	
}

void UB2UIBattleStageInfo::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetHeaderTitleAsChapterNum();
}

void UB2UIBattleStageInfo::AddBossPieceRewardItem(const FStageFixedDropsInfo* _BossPieceData)
{
	//焊胶 炼阿捞 唱坷瘤 臼绰 Stage俊辑绰 nullptr肺 秦林绊 乐绢辑 return窃
	if (!_BossPieceData) return;

	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		FB2Item b2item;
		b2item.ItemRefID = _BossPieceData->Iteminfo_idx;
		b2item.ItemClass = EItemClass::EIC_BossPiece;
		NewRewardItem->BindDoc(b2item);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, true);
		NewRewardItem->SetStageInfoStarGrade(_BossPieceData->Min_Gain, _BossPieceData->Max_Gain);

		// 胶抛捞瘤 焊惑 酒捞袍 眠啊 沥焊甫 持绢淋
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = _BossPieceData->Max_Gain;
		ExtraStageRewardInfo.PrimPointMinValue = _BossPieceData->Min_Gain;
		int convert_itemidx = FItemRefIDHelper::GetRelicIDFromBossPieceRefID(_BossPieceData->Iteminfo_idx);
		ExtraStageRewardInfo.RandomOptionCount = convert_itemidx;//咯扁绰 胶飘傅 甸绢哎 磊府. 磊府甫 撅瘤肺 父电 蠢肠捞 乐变 窍促...
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnBossPieceItem);

		// 捞 SlotItem 甸阑 甸绊 乐绰 ScrollBox 俊 涝仿阑 焊郴扁 困茄 悸诀
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddRewardItem(const FB2RewardItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		NewRewardItem->BindDoc(InItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, false);

		// 胶抛捞瘤 焊惑 酒捞袍 眠啊 沥焊甫 持绢淋
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = InItemData.PrimPointMaxValue;
		ExtraStageRewardInfo.PrimPointMinValue = InItemData.PrimPointMinValue;
		ExtraStageRewardInfo.RandomOptionCount = InItemData.RandomOptionCount;
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnRewardItem);

		// 捞 SlotItem 甸阑 甸绊 乐绰 ScrollBox 俊 涝仿阑 焊郴扁 困茄 悸诀
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddAetherItem(const FB2AetherItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		NewRewardItem->BindEther(InItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, false);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnEtherItem);

		// 捞 SlotItem 甸阑 甸绊 乐绰 ScrollBox 俊 涝仿阑 焊郴扁 困茄 悸诀
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddMaterialItem(const FB2MaterialItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		
		FB2Item ItemData;
		ItemData.ItemRefID = InItemData.ItemRefID;
		ItemData.ConsumingAmount = InItemData.ItemDropCount;
		ItemData.InventoryType = EItemInvenType::EIIVT_Consumables;
		ItemData.ItemClass = EItemClass::EIC_End;

		NewRewardItem->BindDoc(ItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::Collapsed, false);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnMaterialItem);

		// 捞 SlotItem 甸阑 甸绊 乐绰 ScrollBox 俊 涝仿阑 焊郴扁 困茄 悸诀
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::ClearRewardItems()
{
	for (auto RewarItem : BattleRewardItems)
	{
		RewarItem->UnbindDoc();
		RewarItem->OnSlotItemClickDelgate.Unbind();
	}

	BattleRewardItems.Empty();

	RemoveAllRewardItems();
}

void UB2UIBattleStageInfo::ArinCheck()
{
	if (StageGameStarted)
		return;

	//if (!CheckAndInvokeArinConsulting()) // 付瘤阜栏肺 Arin 狼 炼攫 眉农
	//{
	//	UIP_StartBT->StartCloseAni();
	//}
}

void UB2UIBattleStageInfo::UpdateHerosPower()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		MainHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
		SubHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());
	}
}

void UB2UIBattleStageInfo::SetStageMainTexture(UTexture2D* MainTexture)
{
	if (IMG_MainTexture.IsValid())
		IMG_MainTexture->SetBrushFromTexture(MainTexture);
}

void UB2UIBattleStageInfo::SetNPCTexture(UTexture2D* NPCTexture)
{
	if (IMG_NPC.IsValid())
		IMG_NPC->SetBrushFromTexture(NPCTexture);
}

void UB2UIBattleStageInfo::SetStageStory(FText StageStory)
{
	if (TB_StageStory.IsValid())
		TB_StageStory->SetText(StageStory);
}

bool UB2UIBattleStageInfo::CheckAndInvokeArinConsulting()
{
	// 胶抛捞瘤 矫累 滚瓢 喘范阑 锭 阂府绰 芭 啊沥.
	
	// 老沥 冉荐 捞傈俊 公矫沁带 版快. 捞吧 扁夯利牢 Arin 狼 炼攫 惑怕 眉农 捞傈俊 角青秦辑 胶诺 冉荐 瞒皑捞 概锅 角青登档废 窃.
	// Arin 狼 炼攫捞 惯悼窍绰 惑炔俊辑父 捞吧 八荤且 版快 磊末 呈公 坷贰 Arin 狼 炼攫阑 给 焊霸 瞪 荐档 乐促.
	UB2UIDocUICondition* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (DocCondition && DocCondition->ShouldIgnorePreCombatArinConsulting())
	{ // 窍唱 瞒皑窍绊 捞锅浚 胶诺
		DocCondition->SetPreCombatArinConsultIgnoreCount(DocCondition->GetPreCombatArinConsultIgnoreCount() - 1);
		return false;
	}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// ArinConsultingSubject 绰 捞 其捞瘤 甸绢棵锭 郴瘤绰 康旷包府唱 某腐磐 急琶窍搁辑 技泼登绢具 窃.
	EArinConsultPreCombat_MainSubject ArinConsultSubject = DocBS ? DocBS->GetArinConsultingSubject() : EArinConsultPreCombat_MainSubject::ARCPCMS_None;

	if (ArinConsultSubject != EArinConsultPreCombat_MainSubject::ARCPCMS_None)
	{
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenUI(UIFName::ArinConsult_PreCombat);
			return true;
		}
	}
	return false;
}

void UB2UIBattleStageInfo::ShowRewardItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowRewardItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowBossPieceInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowBossPieceInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowEtherItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowEtherItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowMaterialItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowMaterialItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::HideRewardItemInfo()
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->HideToolTip();
	}
}

void UB2UIBattleStageInfo::OnOpenComplete()
{
	Super::OnOpenComplete();
}

void UB2UIBattleStageInfo::OnCloseComplete()
{
	Super::OnCloseComplete();
	StageGameStarted = false;
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIBattleStageInfo::OnClickBtnChangeMainSub()							//Main, Sub 背眉 混妨滴摆嚼聪促...
{
	// UB2UICharacterSelect 俊辑 窍绰 巴苞 吝汗登变 窍绰单 蝶肺 哗扁档 构窍焙..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{// Main/Sub 官层摹扁
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetTagPCClass()), IntToPCClass(DocBattle->GetCurPCClass()));

		// 急琶 努贰胶啊 函版凳俊 蝶扼 酒赴狼 炼攫档 眉农. 傈捧仿篮 笛捞 钦摹聪 崔扼龙 霸 绝摆瘤父 胶懦器牢飘 鞍篮 芭俊 蝶扼 捞悼 矫 穿备 拳搁栏肺 啊绰瘤 沥档 崔扼瘤摆焙 せ
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		PlayChangeChacterAnimBP();
	}
}

void UB2UIBattleStageInfo::OnClickBtnBossPieceItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
	return;

	if (ClickedItem)
	{
		ShowBossPieceInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnRewardItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowRewardItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnEtherItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowEtherItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnMaterialItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowMaterialItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnHideRewardItemInfoSet()
{
	HideRewardItemInfo();
}

void UB2UIBattleStageInfo::OnClickBattleReady()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterSelectPage);
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UIBattleStageInfo::OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	UpdateHerosPower();
}

void UB2UIBattleStageInfo::OnChangedTagPCClass(class UB2UIDocBase* Sender, int32 TagPCClass, int32 PrevTagPCClass)
{
	UpdateHerosPower();
}

void UB2UIBattleStageInfo::MarkStageGameStarted()
{
	StageGameStarted = true; // 胶抛捞瘤 矫累 滚瓢 鞍篮 吧 努腐 矫 咯辨 烹苞窍霸 秦辑 割割 吝夸茄 滚瓢甸捞 馆汗 努腐登瘤 臼档废 茄促.

	// 窜, 埃趣 漂荐茄 荤沥栏肺 (辑滚螟 荤沥) 角力肺 胶抛捞瘤 繁莫捞 救瞪 荐 乐栏聪 绢蠢 沥档 矫埃捞 儒弗 饶俊绰 府悸矫虐档废.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(StageGameStartedResetTH, this, &UB2UIBattleStageInfo::StageGameStartedResetCB, 5.0f, false);
	}
}

void UB2UIBattleStageInfo::StageGameStartedResetCB()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(StageGameStartedResetTH);
	}
	StageGameStarted = false;
}