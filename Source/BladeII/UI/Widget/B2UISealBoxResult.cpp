// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISealBoxResult.h"
#include "B2UIManager.h"

#include "BladeIIGameImpl.h"
#include "CommonStruct.h"
#include "B2UIItemForgeIngredientList.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UISlotItem.h"
#include "B2UIDocHelper.h"
#include "Engine/Engine.h"
#include "B2UIUtil.h"
#include "B2Inventory.h"
#include "B2SomeInfo.h"
#include "B2ItemInfo.h"
#include "Retailer.h"
#include "B2AssetLoader.h"


UB2UISealBoxResult::UB2UISealBoxResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CreatedRewardFxPart = NULL;
}


void UB2UISealBoxResult::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_OpenAgain)
	GET_SLOT(UButton, BTN_OpenAgain)

	GET_SLOT(UCanvasPanel, CP_CloseThis)
	GET_SLOT(UButton, BTN_CloseThis)
}

void UB2UISealBoxResult::UpdateStaticText()
{
}

void UB2UISealBoxResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OpenAgain, &UB2UISealBoxResult::OnClickBtnOpenAgain)
	BIND_CLICK_FUNC_TO_BTN(BTN_CloseThis, &UB2UISealBoxResult::OnClickBtnCloseThis)
}

void UB2UISealBoxResult::GatherRewardItemData()
{
	if (UnsealData)
	{
		RewardItemData.Empty();

		/*
		TArray<FB2Item> RewardItems;
		TArray<FB2Item> EtherItems;
//		TArray<FB2Totem> TotemItems;

		for (int32 i = 0; i < UnsealData->rewards.Num(); i++)
		{
			if (UnsealData->rewards[i]->item->id>0)
			{
				FB2Item tempitem;
				tempitem=UnsealData->rewards[i]->item;
				RewardItems.Add(tempitem);
			}
			if (UnsealData->rewards[i]->aether->level>0)
			{
				FB2Item tempitem;
				tempitem = UnsealData->rewards[i]->aether;
				EtherItems.Add(tempitem);
			}
// 			if (UnsealData->rewards[i]->aether != nullptr)
// 			{
// 				FB2Totem tempitem;
// 				tempitem = UnsealData->rewards[i]->totem;
// 				TotemItems.Add(tempitem);
// 			}
		}

		for (const FB2Item& Elem : RewardItems)
		{
			FB2Item RewardItem = Elem;
			// 抗傈浚 鞍篮 厘厚绰 般媚辑 焊咯玲瘤父 捞力 盒府秦辑 焊咯林扁锭巩俊
			// 酒捞袍(厘厚)绰 Multiple Value(= 1 or 3硅 葛靛)肺 榜靛绰 ConsumingAmount肺 焊咯霖促
			RewardItem.ConsumingAmount = (RewardItem.ItemClass == EItemClass::EIC_Gold) ? FMath::Max(1, RewardItem.ConsumingAmount) : FMath::Max(1, RewardItem.ConsumingAmount);
			RewardItemData.Add(RewardItem);
		}

		for (const FB2Item& Elem : EtherItems)
		{
			FB2Item EtherItem = Elem;
			EtherItem.ConsumingAmount = FMath::Max(1, EtherItem.ConsumingAmount);
			RewardItemData.Add(EtherItem);
		}
// 		for (const FB2Totem& Elem : TotemItems)
// 		{
// 			FB2Totem MaterialItem = Elem;
// 			MaterialItem.ConsumingAmount = FMath::Max(1, MaterialItem.ConsumingAmount);
// 			MaterialItem.bMaterialFromAtlas = IsMaterialFromAtlas(MaterialItem.ItemRefID);
// 			RewardItemData.Add(MaterialItem);
// 		}
*/

		for (int32 i = 0; i < UnsealData->rewards.Num(); i++)
		{
			if (UnsealData->rewards[i]->reward_type == b2network::B2RewardType::ITEM)
			{
				FB2Item tempItem;
				tempItem = UnsealData->rewards[i]->item;
				RewardItemData.Add(tempItem);
			}
			else
			{
				FB2Item RewardItem;
				RewardItem = UnsealData->rewards[i]->item;
				// 抗傈浚 鞍篮 厘厚绰 般媚辑 焊咯玲瘤父 捞力 盒府秦辑 焊咯林扁锭巩俊
				// 酒捞袍(厘厚)绰 Multiple Value(= 1 or 3硅 葛靛)肺 榜靛绰 ConsumingAmount肺 焊咯霖促
				RewardItem.ItemRefID = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(UnsealData->rewards[i]->reward_type);
				RewardItem.ConsumingAmount = UnsealData->rewards[i]->amount;
				RewardItemData.Add(RewardItem);
			}
		}
	}
}

bool UB2UISealBoxResult::IsMaterialFromAtlas(int32 ItemRefID)
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

void UB2UISealBoxResult::SetRewardDataManually(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr, int32 pcClass)
{
	UnsealData = UnsealBoxPtr;
	savedClass = pcClass;

	if (UnsealData->left_amount > 0)
	{
		if (CP_OpenAgain.IsValid())
			CP_OpenAgain->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		if (CP_OpenAgain.IsValid())
			CP_OpenAgain->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UISealBoxResult::Init()
{
	Super::Init();

#if WITH_EDITOR // 酒抗 葛碘 肺爹矫俊 秦 林绰 霸 歹 亮变 窃. 累篮 芭聪 傲 捞吧肺 父练窍磊.
	if (GIsEditor)
	{
		RewardPSSetup.EditorLoadAll();

		auto* AssetLoader = UB2AssetLoader::Get();
		BII_CHECK(AssetLoader);

		for (TSoftClassPtr<UB2SealCardFx>& ThisToLoad : RewardFxPartWidgetClasses)
			AssetLoader->LoadSynchronous(ThisToLoad);
	}
#endif

	if (GetWorld())
	{
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}
}

void UB2UISealBoxResult::BindDocAuto()
 {
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UISealBoxResult::OnChangedRepeatBattleCurrentOn);
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

void UB2UISealBoxResult::BeginRewardItemFx()
{
	// RewardItemData 绰 荤傈俊 霖厚登绢 乐绢具 窃.
	int32 RewardItemNum = RewardItemData.Num();
	UB2SealCardFx* DesiredFxPart = (RewardItemNum > 0) ? GetRewardPartForRewardNum(RewardItemNum) : NULL;
	if (DesiredFxPart)
	{
		DesiredFxPart->SetVisibility(ESlateVisibility::HitTestInvisible);
		DesiredFxPart->BeginScene(RewardItemData, RewardPSSetup, UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);
	}
}

void UB2UISealBoxResult::Show3DWingEffect()
{
	//ShowStageClearWingEffectClass<>::GetInstance().Signal();
}

void UB2UISealBoxResult::Show3DGemEffect()
{
	//ShowStageClearGemEffectClass<>::GetInstance().Signal();
}

void UB2UISealBoxResult::UnbindDoc()
{
	SetDoc(nullptr);

	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
	}
}

void UB2UISealBoxResult::DestroySelf(class UB2UIManager* InUIManager)
{
#if WITH_EDITOR
	if (!GIsEditor)
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

	Super::DestroySelf(InUIManager);
}

void UB2UISealBoxResult::CloseWidgetDelegate()
{
	OnClickBtnCloseThis();
}

void UB2UISealBoxResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWidgetCustomTimer();
}

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2UISealBoxResult::CleanupOnPreSave()
{
	LoadedRewardPSHolder.LoadedPSPtr.Empty();
}
void UB2UISealBoxResult::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave(); // 狼档摹 臼霸 历厘瞪 荐 乐绰 饭欺繁胶甫 朝妨霖促.
}
#endif

void UB2UISealBoxResult::SetupWidgetCustomTimer()
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

void UB2UISealBoxResult::SetupWidgetCustomHideTimer()
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

void UB2UISealBoxResult::UpdateWidgetCustomTimer()
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

UB2SealCardFx* UB2UISealBoxResult::GetRewardPartForRewardNum(int32 InRewardNum)
{
	if (!CreatedRewardFxPart && RewardFxPartWidgetClasses.Num() > 0)
	{
		// 捞亥飘肺 俺荐啊 歹喉捞 登绰 扒 咯扁 RewardNum 栏肺 逞绢坷瘤 臼绰 吧肺.. x2 钎矫
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// 何加 颇飘甫 UMG 硅摹啊 酒囱 TAsset 栏肺 贸府茄 漂荐 纳捞胶.
			TSoftClassPtr<UB2SealCardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];

			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2SealCardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager 俊辑 AddChild 窍绰 规侥措肺 给窍摆绢辑.. 咯扁急 捞 困俊 唱客具 窍绰 霸 乐绢辑 Z-order 甫 窍唱 撤冕促.
					CreatedRewardFxPart->AddToViewport(BII_WIDGET_ZORDER_SEPARATE_BELOW_UIMAIN);
					CreatedRewardFxPart->SetVisibility(ESlateVisibility::HitTestInvisible);
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
void UB2UISealBoxResult::OnClickBtnOpenAgain()
{
	//if (UnsealData->left_amount > 0)
	//{
	//	data_trader::Retailer::GetInstance().RequestUnsealBox(UnsealData->seal_box_id, savedClass + 1, 1);	//茄锅俊 割俺 凯扼绰 郴侩篮 扁裙俊 绝绢辑 茄锅俊 1俺究 凯档废 窍靛内爹 凳
	//	SealBoxSelectCharClass<int32>::GetInstance().Signal(savedClass);
	//}

	//OnClickBtnCloseThis();
}

void UB2UISealBoxResult::OnClickBtnCloseThis()
{
	//UnbindDoc();
	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UISealBoxResult::OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn)
{
	if (bCurrentOn != bPrevOn)
	{
		
	}
}