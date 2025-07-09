// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UISealBoxResult.h"
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
			// 예전엔 같은 장비는 겹쳐서 보여줬지만 이제 분리해서 보여주기때문에
			// 아이템(장비)는 Multiple Value(= 1 or 3배 모드)로 골드는 ConsumingAmount로 보여준다
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
				// 예전엔 같은 장비는 겹쳐서 보여줬지만 이제 분리해서 보여주기때문에
				// 아이템(장비)는 Multiple Value(= 1 or 3배 모드)로 골드는 ConsumingAmount로 보여준다
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

#if WITH_EDITOR // 아예 모듈 로딩시에 해 주는 게 더 좋긴 함. 작은 거니 걍 이걸로 만족하자.
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

	RewardPSSetup.PreloadForItems(RewardItemData, UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder); // 이건 좀 미리 로딩해 둘 필요가 있군.. 연출 나올 때 지장이 좀 있음.
	if (RewardItemData.Num() > 0)
	{
		GetRewardPartForRewardNum(RewardItemData.Num()); // 이것도 미리 만들어놔야 쓰겄다.. 아이템 나오는 순간에 생성하려면 델타타임이 많이 튀는데 이펙트 자체 타이밍과 불일치가 생기는 듯.
	}

	// Gold 다시 Effect 로 넣게 되면 참고. 혹 여기도 2D Overlay 필요할지 모르니. 수량은 PrimaryPoint 보단 ConsumingAmount 가 적절하긴 하지만.. 그럴러면 UISlotItem 쪽에도 예외처리 해야.
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
	// RewardItemData 는 사전에 준비되어 있어야 함.
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
	ShowStageClearWingEffectClass<>::GetInstance().Signal();
}

void UB2UISealBoxResult::Show3DGemEffect()
{
	ShowStageClearGemEffectClass<>::GetInstance().Signal();
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
//#if WITH_EDITOR
//	if (!GIsEditor)
//#endif
//	{
//		// StageClear / Result 가 인게임 메모리 피크가 되고 있기 때문에 Clear -> Result 로 넘어가면서 쓰지 않을 것으로 예상되는 데이터를 좀 날린다. 
//
//		// 리워드 아이템 파티클 이펙트 언로딩
//		RewardPSSetup.UnloadTAssets(UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);
//
//		// 리워드 아이템 전 클리어 연출서 사용한 주요 파티클 이펙트들 언로딩.
//		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
//		if (SomeInfo)
//		{
//			SomeInfo->UnloadStageClearBGFxTemplate();
//			SomeInfo->UnloadStageClearGemFxTemplate();
//			SomeInfo->UnloadStageClearWingFxTemplate();
//		}
//		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//		if (ItemInfoTable)
//		{ // 클리어 연출에서 등장하는 보상 아이콘 텍스쳐가 매우 클 것이므로 내리는 게 좋겠다.
//			ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
//		}
//
//		// UI Scene 전환하면서 GC 돌리긴 할 껀데 설정에 따라 달라질 수도 있고 하니 여긴 확실히 돌려준다.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{
//			TheWorld->ForceGarbageCollection();
//		}
//	}
//
//	if (CreatedRewardFxPart)
//	{
//		CreatedRewardFxPart->DestroySelf(InUIManager);
//		CreatedRewardFxPart = NULL;
//	}

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
	CleanupOnPreSave(); // 의도치 않게 저장될 수 있는 레퍼런스를 날려준다.
}
#endif

void UB2UISealBoxResult::SetupWidgetCustomTimer()
{
	// Pause 상태에서 쓸 것이므로 기본 타이머 못 쓰고 따로 세팅

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 이미 발동한 걸로 간주.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 할 때에도 백업해 놓지만 아래 로직에 따라 HideStop 이 먼저 될 수도 있으니 여기서도 한번.

																		 // HideStopTime 이 HideStartTime 보다 큰지에 대한 확인은.. 굳이 하지 않는다.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 바로 콜백 실행
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 바로 콜백 실행. 결국 HideStopTime 이 0 이하면 안 숨겨짐
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
	// Pause 상태에서도 직접 돌려쓰는 타이머
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
			// 한번만 HideStart 가 되게. 안 그러면 깜박일 것.
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
		// 이벤트로 개수가 더블이 되는 건 여기 RewardNum 으로 넘어오지 않는 걸로.. x2 표시
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// 부속 파트를 UMG 배치가 아닌 TAsset 으로 처리한 특수 케이스.
			TSoftClassPtr<UB2SealCardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];

			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2SealCardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager 에서 AddChild 하는 방식대로 못하겠어서.. 여기선 이 위에 나와야 하는 게 있어서 Z-order 를 하나 낮춘다.
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
	if (UnsealData->left_amount > 0)
	{
		data_trader::Retailer::GetInstance().RequestUnsealBox(UnsealData->seal_box_id, savedClass + 1, 1);	//한번에 몇개 열라는 내용은 기획에 없어서 한번에 1개씩 열도록 하드코딩 됨
		SealBoxSelectCharClass<int32>::GetInstance().Signal(savedClass);
	}

	OnClickBtnCloseThis();
}

void UB2UISealBoxResult::OnClickBtnCloseThis()
{
	UnbindDoc();
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
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