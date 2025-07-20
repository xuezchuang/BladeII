#include "B2UIStageClear.h"
#include "B2UISlotItem.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattleStage.h"
#include "B2UIManager.h"
#include "B2Inventory.h"
#include "B2SomeInfo.h"
#include "B2ItemInfo.h"
#include "B2AssetLoader.h"

UB2UIStageClear::UB2UIStageClear(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CreatedRewardFxPart = NULL;
}


void UB2UIStageClear::CacheAssets()
{
	GET_SLOT(UButton, BTN_GoStageResult)
	GET_SLOT(UB2UIRepeatBattleCountDown, UIP_RepeatBattleCountDownP)
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->Init();
		UIP_RepeatBattleCountDownP->SetPauseAble(true);
		UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Hidden); // 咯扁急 窜瘤 郴何利牢 墨款飘促款 扁瓷阑 困秦 敬促. 老窜 见败初澜
	}
	GET_SLOT(UB2UIRepeatBattleInGameInfo, UIP_RepeatBattleInGameInfoP)
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->Init();
	}
}

void UB2UIStageClear::UpdateStaticText()
{
}

void UB2UIStageClear::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GoStageResult, &UB2UIStageClear::OnClickBtnGoStageResult)
}

void UB2UIStageClear::GatherRewardItemData()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		//胶抛捞瘤 努府绢 焊惑酒捞袍 贸府. 官拆老 绝栏骨肺 Delegate贸府 鞘夸绝澜
		const TArray<FB2Item>& RewardItems = DocBS->GetStageClearRewardItemList();
		const TArray<FB2Item>& EtherItems = DocBS->GetStageClearEtherItemsList();
		const TArray<FB2Item>& MaterialItems = DocBS->GetStageClearMaterialItemsList();
		const TArray<FB2Item>& SoldItems = DocBS->GetStageClearSoldItemsList();

		const int32 MultipleValue = DocBS->GetStageClearMultipleValue();

		RewardItemData.Empty();

		for (const FB2Item& Elem : RewardItems)
		{
			FB2Item RewardItem = Elem;
			// 抗傈浚 鞍篮 厘厚绰 般媚辑 焊咯玲瘤父 捞力 盒府秦辑 焊咯林扁锭巩俊
			// 酒捞袍(厘厚)绰 Multiple Value(= 1 or 3硅 葛靛)肺 榜靛绰 ConsumingAmount肺 焊咯霖促
			RewardItem.ConsumingAmount = (RewardItem.ItemClass == EItemClass::EIC_Gold) ? FMath::Max(1, RewardItem.ConsumingAmount) : FMath::Max(1, MultipleValue);
			RewardItemData.Add(RewardItem);
		}
		for (const FB2Item& Elem : SoldItems)
		{
			FB2Item SoldItem = Elem;
			SoldItem.ConsumingAmount = FMath::Max(1, MultipleValue);
			RewardItemData.Add(SoldItem);
		}

		for (const FB2Item& Elem : EtherItems)
		{
			FB2Item EtherItem = Elem;
			EtherItem.ConsumingAmount = FMath::Max(1, MultipleValue);
			RewardItemData.Add(EtherItem);
		}
		for (const FB2Item& Elem : MaterialItems)
		{
			FB2Item MaterialItem = Elem;
			MaterialItem.ConsumingAmount = FMath::Max(1, MaterialItem.ConsumingAmount);
			MaterialItem.bMaterialFromAtlas = IsMaterialFromAtlas(MaterialItem.ItemRefID);
			RewardItemData.Add(MaterialItem);
		}
	}
}

bool UB2UIStageClear::IsMaterialFromAtlas(int32 ItemRefID)
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable)
	{
		FSingleItemInfoData* InfoData = ItemInfoTable->GetInfoData(ItemRefID);
		if (InfoData)
		{
			return InfoData->bMIDAtlasParamFromUIMIC;
		}

		return false;
	}

	return false;
}

void UB2UIStageClear::Init()
{
	Super::Init();
	
#if WITH_EDITOR // 酒抗 葛碘 肺爹矫俊 秦 林绰 霸 歹 亮变 窃. 累篮 芭聪 傲 捞吧肺 父练窍磊.
	if (GIsEditor)
	{
		RewardPSSetup.EditorLoadAll();

		auto* AssetLoader = UB2AssetLoader::Get();
		BII_CHECK(AssetLoader);

		for (TSoftClassPtr<UB2UIStageClearRewardFx>& ThisToLoad : RewardFxPartWidgetClasses)
			AssetLoader->LoadSynchronous(ThisToLoad);
	}
#endif

	if (GetWorld())
	{
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}
}

void UB2UIStageClear::BindDocAuto()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UIStageClear::OnChangedRepeatBattleCurrentOn);
	}
	
	GatherRewardItemData();

	RewardPSSetup.PreloadForItems(RewardItemData, UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder); // 捞扒 粱 固府 肺爹秦 笛 鞘夸啊 乐焙.. 楷免 唱棵 锭 瘤厘捞 粱 乐澜.
	if (RewardItemData.Num() > 0)
	{
		GetRewardPartForRewardNum(RewardItemData.Num()); // 捞巴档 固府 父甸绢出具 静拔促.. 酒捞袍 唱坷绰 鉴埃俊 积己窍妨搁 胆鸥鸥烙捞 腹捞 脾绰单 捞棋飘 磊眉 鸥捞怪苞 阂老摹啊 积扁绰 淀.
	}

	// Gold 促矫 Effect 肺 持霸 登搁 曼绊. 趣 咯扁档 2D Overlay 鞘夸且瘤 葛福聪. 荐樊篮 PrimaryPoint 焊窜 ConsumingAmount 啊 利例窍变 窍瘤父.. 弊凡矾搁 UISlotItem 率俊档 抗寇贸府 秦具.
	/*if (UIP_StageClearGold.IsValid())
	{
		UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
		if (DocUser)
		{
			const int32 IncreasedGold = (DocUser->GetGold() - DocUser->GetCombatStartGold());

			FB2Item GoldItem;
			GenerateGoldItem(GoldItem, IncreasedGold);
			UIP_StageClearGold->BindDoc(GoldItem);
		}
	}*/
}

void UB2UIStageClear::BeginRewardItemFx()
{
	// RewardItemData 绰 荤傈俊 霖厚登绢 乐绢具 窃.
	int32 RewardItemNum = RewardItemData.Num();
	UB2UIStageClearRewardFx* DesiredFxPart = (RewardItemNum > 0) ? GetRewardPartForRewardNum(RewardItemNum) : NULL;
	if (DesiredFxPart)
	{
		DesiredFxPart->SetVisibility(ESlateVisibility::HitTestInvisible);
		DesiredFxPart->BeginScene(RewardItemData, RewardPSSetup, UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);
	}
}

void UB2UIStageClear::Show3DWingEffect()
{
	//ShowStageClearWingEffectClass<>::GetInstance().Signal();
}

void UB2UIStageClear::Show3DGemEffect()
{
	//ShowStageClearGemEffectClass<>::GetInstance().Signal();
}

void UB2UIStageClear::StartCountDownIfRepeatBattleIsOn()
{
	if (IsRepeatBattleOn())
	{
		SetupOnRepeatBattleStateChange(true);
	}
}

void UB2UIStageClear::UnbindDoc()
{
	SetDoc(nullptr);

	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
	}
}

void UB2UIStageClear::DestroySelf(class UB2UIManager* InUIManager)
{
#if WITH_EDITOR
	if(!GIsEditor)
#endif
	{
		// StageClear / Result 啊 牢霸烙 皋葛府 乔农啊 登绊 乐扁 锭巩俊 Clear -> Result 肺 逞绢啊搁辑 静瘤 臼阑 巴栏肺 抗惑登绰 单捞磐甫 粱 朝赴促. 
		
		// 府况靛 酒捞袍 颇萍努 捞棋飘 攫肺爹
		RewardPSSetup.UnloadTAssets(UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);

		// 府况靛 酒捞袍 傈 努府绢 楷免辑 荤侩茄 林夸 颇萍努 捞棋飘甸 攫肺爹.
		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
		if (SomeInfo)
		{
			SomeInfo->UnloadStageClearBGFxTemplate();
			SomeInfo->UnloadStageClearGemFxTemplate();
			SomeInfo->UnloadStageClearWingFxTemplate();
		}
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable)
		{ // 努府绢 楷免俊辑 殿厘窍绰 焊惑 酒捞能 咆胶媚啊 概快 努 巴捞骨肺 郴府绰 霸 亮摆促.
			ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
		}

		CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
	}

	if (CreatedRewardFxPart)
	{
		CreatedRewardFxPart->DestroySelf(InUIManager);
		CreatedRewardFxPart = NULL;
	}

	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->DestroySelf(InUIManager);
	}

	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIStageClear::CloseWidgetDelegate()
{
	OnClickBtnGoStageResult();
}

void UB2UIStageClear::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWidgetCustomTimer();
}

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2UIStageClear::CleanupOnPreSave()
{
	LoadedRewardPSHolder.LoadedPSPtr.Empty();
}
void UB2UIStageClear::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave(); // 狼档摹 臼霸 历厘瞪 荐 乐绰 饭欺繁胶甫 朝妨霖促.
}
#endif

void UB2UIStageClear::SetupWidgetCustomTimer()
{
	// Pause 惑怕俊辑 镜 巴捞骨肺 扁夯 鸥捞赣 给 静绊 蝶肺 技泼

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 捞固 惯悼茄 吧肺 埃林.
		}
	}
}

void UB2UIStageClear::SetupWidgetCustomHideTimer()
{
	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide == NULL)
		{
			continue;
		}

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 且 锭俊档 归诀秦 初瘤父 酒贰 肺流俊 蝶扼 HideStop 捞 刚历 瞪 荐档 乐栏聪 咯扁辑档 茄锅.

		// HideStopTime 捞 HideStartTime 焊促 奴瘤俊 措茄 犬牢篮.. 被捞 窍瘤 臼绰促.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 官肺 妮归 角青
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 官肺 妮归 角青. 搬惫 HideStopTime 捞 0 捞窍搁 救 见败咙
		}
	}
}

void UB2UIStageClear::UpdateWidgetCustomTimer()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == NULL)
	{
		return;
	}

	const float TimeSinceStart = TheWorld->GetRealTimeSeconds() - CachedWidgetStartTime;
	// Pause 惑怕俊辑档 流立 倒妨静绰 鸥捞赣
	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedCallbackInfo& ThisInfo = CustomTimedCallBackInfo[CTI];
		if (ThisInfo.bIsInvoked == true)
		{
			continue;
		}

		if (ThisInfo.InvokeTime <= TimeSinceStart)
		{
			OriginalCustomTimeCallback(CTI);
			ThisInfo.bIsInvoked = true;
		}
	}

	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

		if (ThisHideInfo.bIsHidden == false)
		{
			// 茄锅父 HideStart 啊 登霸. 救 弊矾搁 濒冠老 巴.
			if (ThisHideInfo.bHiddenDoneOnce == false && ThisHideInfo.HideStartTime <= TimeSinceStart)
			{
				CustomTimeHideStartCallback(CTI);
			}
		}
		else
		{
			if (ThisHideInfo.HideStopTime <= TimeSinceStart)
			{
				CustomTimeHideStopCallback(CTI);
			}
		}
	}
}

void UB2UIStageClear::StartRepeatBattleCountdown()
{
	//if (UIP_RepeatBattleCountDownP.IsValid())
	//{
	//	UIP_RepeatBattleCountDownP->StartCountdown(FRepeatBattleCountDownCB::CreateLambda([this](){
	//		BeginStageClearScene_Phase2Class<>::GetInstance().Signal(); // 墨款飘促款 促 登搁 搬苞 拳搁栏肺 逞绢埃促. (StageClear Phase 2)
	//	}));
	//}
}
void UB2UIStageClear::StopRepeatBattleCountdown()
{
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->StopCountdown();
		UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIStageClear::SetupOnRepeatBattleStateChange(bool bIsOn)
{

	if (UIP_RepeatBattleCountDownP.IsValid())
	{// 夸备荤亲俊 狼秦 搬苞 UI 俊辑绰 捞扒 见变促. 郴何利栏肺 墨款飘窍绰 侩档肺父.
		UIP_RepeatBattleCountDownP->BattleCountDownSetVisibility(/*bIsOn ? ESlateVisibility::HitTestInvisible :*/ ESlateVisibility::Collapsed);
	}

	if (bIsOn){
		StartRepeatBattleCountdown();
	}
	else{
		StopRepeatBattleCountdown();
	}
}

bool UB2UIStageClear::IsRepeatBattleOn()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	return DocBS ? DocBS->GetRepeatBattleCurrentOn() : false;
}

UB2UIStageClearRewardFx* UB2UIStageClear::GetRewardPartForRewardNum(int32 InRewardNum)
{
	if (!CreatedRewardFxPart && RewardFxPartWidgetClasses.Num() > 0)
	{
		// 捞亥飘肺 俺荐啊 歹喉捞 登绰 扒 咯扁 RewardNum 栏肺 逞绢坷瘤 臼绰 吧肺.. x2 钎矫
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// 何加 颇飘甫 UMG 硅摹啊 酒囱 TAsset 栏肺 贸府茄 漂荐 纳捞胶.
			TSoftClassPtr<UB2UIStageClearRewardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];
			
			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2UIStageClearRewardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager 俊辑 AddChild 窍绰 规侥措肺 给窍摆绢辑.. 咯扁急 捞 困俊 唱客具 窍绰 霸 乐绢辑 Z-order 甫 窍唱 撤冕促.
					CreatedRewardFxPart->AddToViewport(BII_WIDGET_ZORDER_SEPARATE_BELOW_UIMAIN);
					CreatedRewardFxPart->Init();
				}

				AssetLoader->UnloadAsset(ClassToLoad);
			}
		}
	}
	return CreatedRewardFxPart;
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIStageClear::OnClickBtnGoStageResult()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageResult);

	//if (IsRepeatBattleOn())
	//{
	//	PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
	//}
	//else
	//{	// StageEventDirector 狼 铰府器令 楷免苞 窃膊 StageResult UIScene 捞 矫累瞪 巴捞促. (StageClear Phase 2)
	//	BeginStageClearScene_Phase2Class<>::GetInstance().Signal();
	//}
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UIStageClear::OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn)
{
	if (bCurrentOn != bPrevOn)
	{
		SetupOnRepeatBattleStateChange(bCurrentOn);
	}
}