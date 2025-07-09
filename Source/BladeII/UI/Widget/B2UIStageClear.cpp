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
		UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Hidden); // 여기선 단지 내부적인 카운트다운 기능을 위해 쓴다. 일단 숨겨놓음
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
		//스테이지 클리어 보상아이템 처리. 바뀔일 없으므로 Delegate처리 필요없음
		const TArray<FB2Item>& RewardItems = DocBS->GetStageClearRewardItemList();
		const TArray<FB2Item>& EtherItems = DocBS->GetStageClearEtherItemsList();
		const TArray<FB2Item>& MaterialItems = DocBS->GetStageClearMaterialItemsList();
		const TArray<FB2Item>& SoldItems = DocBS->GetStageClearSoldItemsList();

		const int32 MultipleValue = DocBS->GetStageClearMultipleValue();

		RewardItemData.Empty();

		for (const FB2Item& Elem : RewardItems)
		{
			FB2Item RewardItem = Elem;
			// 예전엔 같은 장비는 겹쳐서 보여줬지만 이제 분리해서 보여주기때문에
			// 아이템(장비)는 Multiple Value(= 1 or 3배 모드)로 골드는 ConsumingAmount로 보여준다
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
	
#if WITH_EDITOR // 아예 모듈 로딩시에 해 주는 게 더 좋긴 함. 작은 거니 걍 이걸로 만족하자.
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

void UB2UIStageClear::BeginRewardItemFx()
{
	// RewardItemData 는 사전에 준비되어 있어야 함.
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
		// StageClear / Result 가 인게임 메모리 피크가 되고 있기 때문에 Clear -> Result 로 넘어가면서 쓰지 않을 것으로 예상되는 데이터를 좀 날린다. 
		
		// 리워드 아이템 파티클 이펙트 언로딩
		RewardPSSetup.UnloadTAssets(UB2AssetLoader::GetStreamableManager(), LoadedRewardPSHolder);

		// 리워드 아이템 전 클리어 연출서 사용한 주요 파티클 이펙트들 언로딩.
		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
		if (SomeInfo)
		{
			SomeInfo->UnloadStageClearBGFxTemplate();
			SomeInfo->UnloadStageClearGemFxTemplate();
			SomeInfo->UnloadStageClearWingFxTemplate();
		}
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable)
		{ // 클리어 연출에서 등장하는 보상 아이콘 텍스쳐가 매우 클 것이므로 내리는 게 좋겠다.
			ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
		}

		// UI Scene 전환하면서 GC 돌리긴 할 껀데 설정에 따라 달라질 수도 있고 하니 여긴 확실히 돌려준다.
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
	CleanupOnPreSave(); // 의도치 않게 저장될 수 있는 레퍼런스를 날려준다.
}
#endif

void UB2UIStageClear::SetupWidgetCustomTimer()
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

void UB2UIStageClear::UpdateWidgetCustomTimer()
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

void UB2UIStageClear::StartRepeatBattleCountdown()
{
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->StartCountdown(FRepeatBattleCountDownCB::CreateLambda([this](){
			BeginStageClearScene_Phase2Class<>::GetInstance().Signal(); // 카운트다운 다 되면 결과 화면으로 넘어간다. (StageClear Phase 2)
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
	{// 요구사항에 의해 결과 UI 에서는 이건 숨긴다. 내부적으로 카운트하는 용도로만.
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
		// 이벤트로 개수가 더블이 되는 건 여기 RewardNum 으로 넘어오지 않는 걸로.. x2 표시
		int32 SupposedIndex = InRewardNum - 1;
		if (RewardFxPartWidgetClasses.IsValidIndex(SupposedIndex))
		{
			// 부속 파트를 UMG 배치가 아닌 TAsset 으로 처리한 특수 케이스.
			TSoftClassPtr<UB2UIStageClearRewardFx> ClassToLoad = RewardFxPartWidgetClasses[SupposedIndex];
			
			if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get())
			{
				UClass* LoadedClass = AssetLoader->LoadSynchronous(ClassToLoad);

				CreatedRewardFxPart = CreateWidget<UB2UIStageClearRewardFx>(GetOwningPlayer(), LoadedClass);
				if (CreatedRewardFxPart)
				{// UIManager 에서 AddChild 하는 방식대로 못하겠어서.. 여기선 이 위에 나와야 하는 게 있어서 Z-order 를 하나 낮춘다.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 반복 전투 상태에서는 중단 버튼의 역할이 됨.
	}
	else
	{	// StageEventDirector 의 승리포즈 연출과 함께 StageResult UIScene 이 시작될 것이다. (StageClear Phase 2)
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