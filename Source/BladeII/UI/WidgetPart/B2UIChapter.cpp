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
	bExternalDynamicSetupReady = true; // �¾��� �� �Ǿ� �־ UMG �� ��Ƴ��� �¾��� ���ٸ� false ��..
}

void UB2UIChapter::CacheAssets()
{
	//GET_SLOT(UImage,	IMG_Sample)

	for (int Cnt = 1; Cnt <= StageNum; ++Cnt )
	{
		FString StageAssetStr = PrefixStageName + FString::FormatAsNumber(Cnt);
		UB2UIChapterStage* StageItem = GetSlot<UB2UIChapterStage>(FName(*StageAssetStr));
		if (StageItem) 
		{ // UniquelyDefinedStageParts �� ���� �����ϵ��� ������ ���� �� �ִ�.
			ChapterStages.Add(StageItem);
		}
		else 
		{ // UniquelyDefinedStageParts �� ���� �����ϴ� ���� ���� ���̹��� PanelWidget �� �־�� ��.
			UCanvasPanel* PanelWidgetItem = GetSlot<UCanvasPanel>(FName(*StageAssetStr));
			UniquelyDefinedPartsCreatePanels.Add(Cnt, PanelWidgetItem);
		}
	}
	check(ChapterStages.Num() + UniquelyDefinedPartsCreatePanels.Num() == StageNum);

	// GetSlot ���� ���� �� �ִ� ��ġ�� �� ���� ���� ���� �����ϴ� ��.. 
	// �̷��� �ϸ� �Ϻ� �������� ��Ʈ�� (��ü�� ���� ��������) �ٸ��� �������� �� �Ȱ��� ���� é�͸� �������� 
	// �ϳ��� UIChapter WidgetBP ������ ������ ��Ȱ���� �����ؼ� é�Ͱ� �ٷ� �̵� �� �� ���ҿ� ����
	GenerateUniquelyDefinedPartsOfCurrentChapter(DynCreatedStages);
	for (UB2UIChapterStage* ThisCreatedParts : DynCreatedStages)
	{
		if (ThisCreatedParts)
		{
			// �ش� �������� �ѹ��� UniquelyDefinedPartsCreatePanels �� ã�� �������� ���ۿ� �������� �ȵǾ�����..
			check(UniquelyDefinedPartsCreatePanels.Find(ThisCreatedParts->GetSubChapterNum()));
			// ������ ���� ChapterStages �� �����ֱ� �Ѵ�. ������ ChapterStages �� SubChapterNum �� �迭 �ε��� ������� �������� ��..
			ChapterStages.Insert(ThisCreatedParts, ThisCreatedParts->GetSubChapterNum() - 1);
		}
	}
	// ��ġ�� �� + �������� ���� �⺻ �����Ǵ� ��ü StageNum �� ���� ������ ���� ���߷ȴ���.. �ƴϸ� ������ �Ʒ� ������ ���� �� ���� ��Ȳ�̶�� UIChapter WidgetBP �� ������ �ٹ̵���.
	check(ChapterStages.Num() == StageNum);

	GET_SLOT(UPanelWidget, CP_TempNonMatineeBG)
	if (CP_TempNonMatineeBG.IsValid())
	{
		CP_TempNonMatineeBG->SetVisibility(bExternalDynamicSetupReady ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); // External dynamic (matinee) �¾��� �ȵǾ� ���� �� ���̱� ���� ��.
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
			// UniquelyDefinedStageParts �� ������ ���� �����ϴ� ��� �Ѵܰ� ���� PanelSlot �� ã�ƾ� ��. 
			UCanvasPanel** DynamicCreateBasePanelPtr = UniquelyDefinedPartsCreatePanels.Find(Cnt + 1);
			UCanvasPanel* DynamicCreateBasePanel = DynamicCreateBasePanelPtr ? *DynamicCreateBasePanelPtr : nullptr;
			bool bIsDynamicallyCreated = (DynamicCreateBasePanel != nullptr);
			// CanvasSlotToAdjust �� ��ũ�� �������� �ֻ��� �г��� �Ǿ�� �Ѵ�.
			UCanvasPanelSlot* CanvasSlotToAdjust = bIsDynamicallyCreated ?
				Cast<UCanvasPanelSlot>(DynamicCreateBasePanel->Slot) : Cast<UCanvasPanelSlot>(StageItem->Slot);
			if (CanvasSlotToAdjust)
			{
				//��ġ����.
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
	// UniquelyDefinedPartsCreatePanels �� ������ ��Ȳ���� ����� ����
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
						// ������ UIP �� ��ġ�ϴ� Slot �� �Ѵܰ� �� �ö� Slot �� �� �Ȱ��� ������.
						if (AddedCPSlot) {
							AddedCPSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
					}
					else { // ������ ��ð� �� �Ǿ� ������ �ڵ� ����
						if (AddedCPSlot) {
							AddedCPSlot->SetAutoSize(true);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetAutoSize(true);
						}
					}
					NewCreated->Init();
					NewCreated->SetSubChapterNum(ThisSettingKey.SubStageNum); // Init ���� �ᱹ SubChapterNum �� �ٽ� ä������ ���� �� �� ��Ʈ������ �������� ���⼭ ������ �͵��� ChapterStages �迭�� �����ϰ� �־���ƾ� ��.
					// �̰͵� �� ChapterStages �� �����־��..
					OutCreatedParts.Add(NewCreated);
				}
			}
		}
	}
}

void UB2UIChapter::ResetStateForReuse()
{
	// ChapterFrame ���� ��Ȱ���� �ؼ� ���� ��Ȳ�� �־ ������� ��..

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
	ResetStateForReuse(); // ���� �ϴ� ��츦 ����.

	Super::Init();
	
	// ���⼭ ChapterStages �������� SubChapterNum �� ������� �ְ� �ǹǷ� ���� �����ϴ� UniquelyDefinedStageParts �� �� ���� ������ ���ƾ� �Ѵ�.
	for (int Cnt = 0; Cnt < ChapterStages.Num(); ++Cnt )
	{
		auto StageItem = ChapterStages[Cnt];
		StageItem->Init();
		StageItem->SetSubChapterNum(Cnt + 1);
		//const FStage��¼��& StageData = ;
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

void UB2UIChapter::SetStageDatas(/*TArray<FStage��¼��> StageDatas*/)
{
	for (auto StageItem : ChapterStages)
		StageItem->BindDoc(/*FStage��¼��*/);
}

//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
