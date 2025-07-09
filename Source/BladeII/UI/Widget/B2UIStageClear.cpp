#include "BladeII.h"
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
		UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Hidden); // ���⼱ ���� �������� ī��Ʈ�ٿ� ����� ���� ����. �ϴ� ���ܳ���
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
		//�������� Ŭ���� ��������� ó��. �ٲ��� �����Ƿ� Delegateó�� �ʿ����
		const TArray<FB2Item>& RewardItems = DocBS->GetStageClearRewardItemList();
		const TArray<FB2Item>& EtherItems = DocBS->GetStageClearEtherItemsList();
		const TArray<FB2Item>& MaterialItems = DocBS->GetStageClearMaterialItemsList();
		const TArray<FB2Item>& SoldItems = DocBS->GetStageClearSoldItemsList();

		const int32 MultipleValue = DocBS->GetStageClearMultipleValue();

		RewardItemData.Empty();

		for (const FB2Item& Elem : RewardItems)
		{
			FB2Item RewardItem = Elem;
			// ������ ���� ���� ���ļ� ���������� ���� �и��ؼ� �����ֱ⶧����
			// ������(���)�� Multiple Value(= 1 or 3�� ���)�� ���� ConsumingAmount�� �����ش�
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
	
#if WITH_EDITOR // �ƿ� ��� �ε��ÿ� �� �ִ� �� �� ���� ��. ���� �Ŵ� �� �̰ɷ� ��������.
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

void UB2UIStageClear::BeginRewardItemFx()
{
	// RewardItemData �� ������ �غ�Ǿ� �־�� ��.
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
	ShowStageClearWingEffectClass<>::GetInstance().Signal();
}

void UB2UIStageClear::Show3DGemEffect()
{
	ShowStageClearGemEffectClass<>::GetInstance().Signal();
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
		// StageClear / Result �� �ΰ��� �޸� ��ũ�� �ǰ� �ֱ� ������ Clear -> Result �� �Ѿ�鼭 ���� ���� ������ ����Ǵ� �����͸� �� ������. 
		
		// ������ ������ ��ƼŬ ����Ʈ ��ε�
		RewardPSSetup.UnloadTAssets(UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);

		// ������ ������ �� Ŭ���� ���⼭ ����� �ֿ� ��ƼŬ ����Ʈ�� ��ε�.
		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
		if (SomeInfo)
		{
			SomeInfo->UnloadStageClearBGFxTemplate();
			SomeInfo->UnloadStageClearGemFxTemplate();
			SomeInfo->UnloadStageClearWingFxTemplate();
		}
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable)
		{ // Ŭ���� ���⿡�� �����ϴ� ���� ������ �ؽ��İ� �ſ� Ŭ ���̹Ƿ� ������ �� ���ڴ�.
			ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
		}

		// UI Scene ��ȯ�ϸ鼭 GC ������ �� ���� ������ ���� �޶��� ���� �ְ� �ϴ� ���� Ȯ���� �����ش�.
		//UWorld* TheWorld = GetWorld();
		//if (TheWorld)
		//{
		//	TheWorld->ForceGarbageCollection();
		//}
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
	CleanupOnPreSave(); // �ǵ�ġ �ʰ� ����� �� �ִ� ���۷����� �����ش�.
}
#endif

void UB2UIStageClear::SetupWidgetCustomTimer()
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

void UB2UIStageClear::UpdateWidgetCustomTimer()
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

void UB2UIStageClear::StartRepeatBattleCountdown()
{
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->StartCountdown(FRepeatBattleCountDownCB::CreateLambda([this](){
			BeginStageClearScene_Phase2Class<>::GetInstance().Signal(); // ī��Ʈ�ٿ� �� �Ǹ� ��� ȭ������ �Ѿ��. (StageClear Phase 2)
		}));
	}
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
	{// �䱸���׿� ���� ��� UI ������ �̰� �����. ���������� ī��Ʈ�ϴ� �뵵�θ�.
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
		// �̺�Ʈ�� ������ ������ �Ǵ� �� ���� RewardNum ���� �Ѿ���� �ʴ� �ɷ�.. x2 ǥ��
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// �μ� ��Ʈ�� UMG ��ġ�� �ƴ� TAsset ���� ó���� Ư�� ���̽�.
			TSoftClassPtr<UB2UIStageClearRewardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];
			
			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2UIStageClearRewardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager ���� AddChild �ϴ� ��Ĵ�� ���ϰھ.. ���⼱ �� ���� ���;� �ϴ� �� �־ Z-order �� �ϳ� �����.
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

	if (IsRepeatBattleOn())
	{
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // �ݺ� ���� ���¿����� �ߴ� ��ư�� ������ ��.
	}
	else
	{	// StageEventDirector �� �¸����� ����� �Բ� StageResult UIScene �� ���۵� ���̴�. (StageClear Phase 2)
		BeginStageClearScene_Phase2Class<>::GetInstance().Signal();
	}
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