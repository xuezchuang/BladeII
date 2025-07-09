#include "BladeII.h"
#include "B2UIChapter.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIChapterStage.h"
#include "B2LobbyGameMode.h"


UB2UIChapter::UB2UIChapter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StageNum = 10;
	PrefixStageName = TEXT("UIP_ChapterStage");
	ChapterStages.Empty();
	ChapterNum = INDEX_NONE;
	StageDifficulty = EStageDifficulty::ESD_None;
	bExternalDynamicSetupReady = true; // 셋업이 안 되어 있어서 UMG 에 깔아놓은 셋업을 쓴다면 false 로..
}

void UB2UIChapter::CacheAssets()
{
	//GET_SLOT(UImage,	IMG_Sample)

	for (int Cnt = 1; Cnt <= StageNum; ++Cnt )
	{
		FString StageAssetStr = PrefixStageName + FString::FormatAsNumber(Cnt);
		UB2UIChapterStage* StageItem = GetSlot<UB2UIChapterStage>(FName(*StageAssetStr));
		if (StageItem) 
		{ // UniquelyDefinedStageParts 로 동적 생성하도록 했으면 빠질 수 있다.
			ChapterStages.Add(StageItem);
		}
		else 
		{ // UniquelyDefinedStageParts 로 동적 생성하는 경우는 같은 네이밍의 PanelWidget 이 있어야 함.
			UCanvasPanel* PanelWidgetItem = GetSlot<UCanvasPanel>(FName(*StageAssetStr));
			UniquelyDefinedPartsCreatePanels.Add(Cnt, PanelWidgetItem);
		}
	}
	check(ChapterStages.Num() + UniquelyDefinedPartsCreatePanels.Num() == StageNum);

	// GetSlot 으로 얻어올 수 있는 배치된 거 말고 따로 동적 생성하는 거.. 
	// 이렇게 하면 일부 스테이지 파트만 (대체로 보스 스테이지) 다르고 나머지는 다 똑같은 여러 챕터맵 구성들을 
	// 하나의 UIChapter WidgetBP 구성을 가지고 재활용이 가능해서 챕터간 바로 이동 시 랙 감소에 도움
	GenerateUniquelyDefinedPartsOfCurrentChapter(DynCreatedStages);
	for (UB2UIChapterStage* ThisCreatedParts : DynCreatedStages)
	{
		if (ThisCreatedParts)
		{
			// 해당 스테이지 넘버의 UniquelyDefinedPartsCreatePanels 을 찾지 못했으면 진작에 생성부터 안되었겠지..
			check(UniquelyDefinedPartsCreatePanels.Find(ThisCreatedParts->GetSubChapterNum()));
			// 생성한 것을 ChapterStages 에 끼워넣기 한다. 아직은 ChapterStages 의 SubChapterNum 이 배열 인덱스 순서대로 맞춰지기 전..
			ChapterStages.Insert(ThisCreatedParts, ThisCreatedParts->GetSubChapterNum() - 1);
		}
	}
	// 배치된 거 + 동적생성 수가 기본 가정되는 전체 StageNum 에 맞지 않으면 뭔가 빠뜨렸던지.. 아니면 구조상 아래 가정을 맞출 수 없는 상황이라면 UIChapter WidgetBP 를 별도로 꾸미든지.
	check(ChapterStages.Num() == StageNum);

	GET_SLOT(UPanelWidget, CP_TempNonMatineeBG)
	if (CP_TempNonMatineeBG.IsValid())
	{
		CP_TempNonMatineeBG->SetVisibility(bExternalDynamicSetupReady ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); // External dynamic (matinee) 셋업이 안되어 있을 때 보이기 위한 거.
	}
}

void UB2UIChapter::UpdateStaticText()
{
}

void UB2UIChapter::BindDelegates()
{
}

void UB2UIChapter::UpdateStageUIPosition(TArray<FVector2D>& Positions2D)
{	
	if (!bExternalDynamicSetupReady){
#if !UE_BUILD_SHIPPING
		UE_LOG(LogBladeII, Log, TEXT("Chapter #d is marked as external dynamic setup (possibly by matinee) not being ready. Stick to UMG placed location."), ChapterNum);
#endif
		return;
	}

	if (Positions2D.Num() > 0 && ChapterStages.Num() > 0 && Positions2D.Num() != ChapterStages.Num())
		UE_LOG(LogBladeII, Log, TEXT("Number of ChpaterStage has not matching matinee's"));

	for (int Cnt = 0; Cnt < ChapterStages.Num(); ++Cnt)
	{
		auto* StageItem = ChapterStages[Cnt];
		if (Positions2D.IsValidIndex(Cnt))
		{
			// UniquelyDefinedStageParts 를 가지고 동적 생성하는 경우 한단계 상위 PanelSlot 을 찾아야 함. 
			UCanvasPanel** DynamicCreateBasePanelPtr = UniquelyDefinedPartsCreatePanels.Find(Cnt + 1);
			UCanvasPanel* DynamicCreateBasePanel = DynamicCreateBasePanelPtr ? *DynamicCreateBasePanelPtr : nullptr;
			bool bIsDynamicallyCreated = (DynamicCreateBasePanel != nullptr);
			// CanvasSlotToAdjust 는 스크린 사이즈의 최상위 패널이 되어야 한다.
			UCanvasPanelSlot* CanvasSlotToAdjust = bIsDynamicallyCreated ?
				Cast<UCanvasPanelSlot>(DynamicCreateBasePanel->Slot) : Cast<UCanvasPanelSlot>(StageItem->Slot);
			if (CanvasSlotToAdjust)
			{
				//위치보정.
				FVector2D offset = -CanvasSlotToAdjust->GetSize();
				offset.X *= 0.5f;
				CanvasSlotToAdjust->SetPosition(Positions2D[Cnt] + offset);
			}
		}
	}
}

UB2UIChapterStage* UB2UIChapter::GetStageWidget(const int32 StageNumber)
{
	for (auto StageWidget : ChapterStages)
	{
		if (StageWidget && StageWidget->GetSubChapterNum() == StageNumber)
			return StageWidget;
	}

	return nullptr;
}

void UB2UIChapter::GenerateUniquelyDefinedPartsOfCurrentChapter(TArray<class UB2UIChapterStage*>& OutCreatedParts)
{
	// UniquelyDefinedPartsCreatePanels 를 가져온 상황에서 사용이 가능
	APlayerController* OwningPC = GetOwningPlayer();
	for (TMap<FChapterStageUniqueSettingKey, FChapterStageUniqueSettingValue>::TIterator UPIt(UniquelyDefinedStageParts); UPIt; ++UPIt)
	{
		FChapterStageUniqueSettingKey& ThisSettingKey = UPIt.Key();

		if (ThisSettingKey.ChapterNum == ChapterNum)
		{
			UCanvasPanel** ThisParentPanel = UniquelyDefinedPartsCreatePanels.Find(ThisSettingKey.SubStageNum);
			FChapterStageUniqueSettingValue& ThisSettingValue = UPIt.Value();
			TSoftClassPtr<class UB2UIChapterStage>& ToCreateClass = ThisSettingValue.WidgetClass;
			if (ThisParentPanel && *ThisParentPanel)
			{
				UB2UIChapterStage* NewCreated = CreateWidget<UB2UIChapterStage>(OwningPC, ToCreateClass.LoadSynchronous());
				if (NewCreated)
				{
					UCanvasPanelSlot* AddedCPSlot = Cast<UCanvasPanelSlot>((*ThisParentPanel)->AddChild(NewCreated));
					// More outer slot..
					UCanvasPanelSlot* ParentPanelSlot = Cast<UCanvasPanelSlot>((*ThisParentPanel)->Slot);
					if (ThisSettingValue.DisplayPanelSize.X > 0.0f && ThisSettingValue.DisplayPanelSize.Y > 0.0f) {
						// 생성된 UIP 가 위치하는 Slot 과 한단계 더 올라간 Slot 둘 다 똑같이 맞춰줌.
						if (AddedCPSlot) {
							AddedCPSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
					}
					else { // 사이즈 명시가 안 되어 있으면 자동 맞춤
						if (AddedCPSlot) {
							AddedCPSlot->SetAutoSize(true);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetAutoSize(true);
						}
					}
					NewCreated->Init();
					NewCreated->SetSubChapterNum(ThisSettingKey.SubStageNum); // Init 에서 결국 SubChapterNum 을 다시 채워넣을 껀데 그 때 흐트러지지 않으려면 여기서 생성된 것들을 ChapterStages 배열에 적절하게 넣어놓아야 함.
					// 이것도 곧 ChapterStages 에 끼워넣어야..
					OutCreatedParts.Add(NewCreated);
				}
			}
		}
	}
}

void UB2UIChapter::ResetStateForReuse()
{
	// ChapterFrame 에서 재활용을 해서 쓰는 상황이 있어서 비워놓는 거..

	ChapterStages.Empty();
	UniquelyDefinedPartsCreatePanels.Empty();

	for (UB2UIChapterStage* ThisDynCreated : DynCreatedStages)
	{
		if (ThisDynCreated) {
			ThisDynCreated->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	DynCreatedStages.Empty();
}

void UB2UIChapter::Init()
{
	ResetStateForReuse(); // 재사용 하는 경우를 위해.

	Super::Init();
	
	// 여기서 ChapterStages 파츠들의 SubChapterNum 을 순서대로 넣게 되므로 동적 생성하는 UniquelyDefinedStageParts 를 이 전에 생성해 놓아야 한다.
	for (int Cnt = 0; Cnt < ChapterStages.Num(); ++Cnt )
	{
		auto StageItem = ChapterStages[Cnt];
		StageItem->Init();
		StageItem->SetSubChapterNum(Cnt + 1);
		//const FStage어쩌고& StageData = ;
		//StageItem->BindDoc(StageData);						
	}

	PlayOpeningAnim();
}

void UB2UIChapter::SetChapterNum(int32 InValue, EStageDifficulty InStageDifficulty)
{
	ChapterNum = InValue;
	StageDifficulty = InStageDifficulty;

	for (auto StageItem : ChapterStages)
	{
		StageItem->SetChapterNum(InValue);
		StageItem->SetStageDifficulty(InStageDifficulty);
	}
}

void UB2UIChapter::SetStageDatas(/*TArray<FStage어쩌고> StageDatas*/)
{
	for (auto StageItem : ChapterStages)
		StageItem->BindDoc(/*FStage어쩌고*/);
}

//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
