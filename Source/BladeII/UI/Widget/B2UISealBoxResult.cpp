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
			// ������ ���� ���� ���ļ� ���������� ���� �и��ؼ� �����ֱ⶧����
			// ������(���)�� Multiple Value(= 1 or 3�� ���)�� ���� ConsumingAmount�� �����ش�
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
				// ������ ���� ���� ���ļ� ���������� ���� �и��ؼ� �����ֱ⶧����
				// ������(���)�� Multiple Value(= 1 or 3�� ���)�� ���� ConsumingAmount�� �����ش�
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

#if WITH_EDITOR // �ƿ� ��� �ε��ÿ� �� �ִ� �� �� ���� ��. ���� �Ŵ� �� �̰ɷ� ��������.
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

	RewardPSSetup.PreloadForItems(RewardItemData, UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder); // �̰� �� �̸� �ε��� �� �ʿ䰡 �ֱ�.. ���� ���� �� ������ �� ����.
	if (RewardItemData.Num() > 0)
	{
		GetRewardPartForRewardNum(RewardItemData.Num()); // �̰͵� �̸� �������� ���δ�.. ������ ������ ������ �����Ϸ��� ��ŸŸ���� ���� Ƣ�µ� ����Ʈ ��ü Ÿ�ְ̹� ����ġ�� ����� ��.
	}

	// Gold �ٽ� Effect �� �ְ� �Ǹ� ����. Ȥ ���⵵ 2D Overlay �ʿ����� �𸣴�. ������ PrimaryPoint ���� ConsumingAmount �� �����ϱ� ������.. �׷����� UISlotItem �ʿ��� ����ó�� �ؾ�.
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
	// RewardItemData �� ������ �غ�Ǿ� �־�� ��.
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
//		// StageClear / Result �� �ΰ��� �޸� ��ũ�� �ǰ� �ֱ� ������ Clear -> Result �� �Ѿ�鼭 ���� ���� ������ ����Ǵ� �����͸� �� ������. 
//
//		// ������ ������ ��ƼŬ ����Ʈ ��ε�
//		RewardPSSetup.UnloadTAssets(UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);
//
//		// ������ ������ �� Ŭ���� ���⼭ ����� �ֿ� ��ƼŬ ����Ʈ�� ��ε�.
//		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
//		if (SomeInfo)
//		{
//			SomeInfo->UnloadStageClearBGFxTemplate();
//			SomeInfo->UnloadStageClearGemFxTemplate();
//			SomeInfo->UnloadStageClearWingFxTemplate();
//		}
//		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//		if (ItemInfoTable)
//		{ // Ŭ���� ���⿡�� �����ϴ� ���� ������ �ؽ��İ� �ſ� Ŭ ���̹Ƿ� ������ �� ���ڴ�.
//			ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
//		}
//
//		// UI Scene ��ȯ�ϸ鼭 GC ������ �� ���� ������ ���� �޶��� ���� �ְ� �ϴ� ���� Ȯ���� �����ش�.
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
	CleanupOnPreSave(); // �ǵ�ġ �ʰ� ����� �� �ִ� ���۷����� �����ش�.
}
#endif

void UB2UISealBoxResult::SetupWidgetCustomTimer()
{
	// Pause ���¿��� �� ���̹Ƿ� �⺻ Ÿ�̸� �� ���� ���� ����

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // �̹� �ߵ��� �ɷ� ����.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide �� ������ ����� ������ �Ʒ� ������ ���� HideStop �� ���� �� ���� ������ ���⼭�� �ѹ�.

																		 // HideStopTime �� HideStartTime ���� ū���� ���� Ȯ����.. ���� ���� �ʴ´�.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // �ٷ� �ݹ� ����
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // �ٷ� �ݹ� ����. �ᱹ HideStopTime �� 0 ���ϸ� �� ������
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
	// Pause ���¿����� ���� �������� Ÿ�̸�
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
			// �ѹ��� HideStart �� �ǰ�. �� �׷��� ������ ��.
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
		// �̺�Ʈ�� ������ ������ �Ǵ� �� ���� RewardNum ���� �Ѿ���� �ʴ� �ɷ�.. x2 ǥ��
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// �μ� ��Ʈ�� UMG ��ġ�� �ƴ� TAsset ���� ó���� Ư�� ���̽�.
			TSoftClassPtr<UB2SealCardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];

			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2SealCardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager ���� AddChild �ϴ� ��Ĵ�� ���ϰھ.. ���⼱ �� ���� ���;� �ϴ� �� �־ Z-order �� �ϳ� �����.
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
		data_trader::Retailer::GetInstance().RequestUnsealBox(UnsealData->seal_box_id, savedClass + 1, 1);	//�ѹ��� � ����� ������ ��ȹ�� ��� �ѹ��� 1���� ������ �ϵ��ڵ� ��
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