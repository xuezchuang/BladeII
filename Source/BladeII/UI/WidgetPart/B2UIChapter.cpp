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
	bExternalDynamicSetupReady = true; // 悸诀捞 救 登绢 乐绢辑 UMG 俊 彬酒初篮 悸诀阑 敬促搁 false 肺..
}

void UB2UIChapter::CacheAssets()
{
	//GET_SLOT(UImage,	IMG_Sample)

	for (int Cnt = 1; Cnt <= StageNum; ++Cnt )
	{
		FString StageAssetStr = PrefixStageName + FString::FormatAsNumber(Cnt);
		UB2UIChapterStage* StageItem = GetSlot<UB2UIChapterStage>(FName(*StageAssetStr));
		if (StageItem) 
		{ // UniquelyDefinedStageParts 肺 悼利 积己窍档废 沁栏搁 狐龙 荐 乐促.
			ChapterStages.Add(StageItem);
		}
		else 
		{ // UniquelyDefinedStageParts 肺 悼利 积己窍绰 版快绰 鞍篮 匙捞怪狼 PanelWidget 捞 乐绢具 窃.
			UCanvasPanel* PanelWidgetItem = GetSlot<UCanvasPanel>(FName(*StageAssetStr));
			UniquelyDefinedPartsCreatePanels.Add(Cnt, PanelWidgetItem);
		}
	}
	check(ChapterStages.Num() + UniquelyDefinedPartsCreatePanels.Num() == StageNum);

	// GetSlot 栏肺 掘绢棵 荐 乐绰 硅摹等 芭 富绊 蝶肺 悼利 积己窍绰 芭.. 
	// 捞犯霸 窍搁 老何 胶抛捞瘤 颇飘父 (措眉肺 焊胶 胶抛捞瘤) 促福绊 唱赣瘤绰 促 度鞍篮 咯矾 茅磐甘 备己甸阑 
	// 窍唱狼 UIChapter WidgetBP 备己阑 啊瘤绊 犁劝侩捞 啊瓷秦辑 茅磐埃 官肺 捞悼 矫 发 皑家俊 档框
	GenerateUniquelyDefinedPartsOfCurrentChapter(DynCreatedStages);
	for (UB2UIChapterStage* ThisCreatedParts : DynCreatedStages)
	{
		if (ThisCreatedParts)
		{
			// 秦寸 胶抛捞瘤 逞滚狼 UniquelyDefinedPartsCreatePanels 阑 茫瘤 给沁栏搁 柳累俊 积己何磐 救登菌摆瘤..
			check(UniquelyDefinedPartsCreatePanels.Find(ThisCreatedParts->GetSubChapterNum()));
			// 积己茄 巴阑 ChapterStages 俊 尝况持扁 茄促. 酒流篮 ChapterStages 狼 SubChapterNum 捞 硅凯 牢郸胶 鉴辑措肺 嘎苗瘤扁 傈..
			ChapterStages.Insert(ThisCreatedParts, ThisCreatedParts->GetSubChapterNum() - 1);
		}
	}
	// 硅摹等 芭 + 悼利积己 荐啊 扁夯 啊沥登绰 傈眉 StageNum 俊 嘎瘤 臼栏搁 购啊 狐哆啡带瘤.. 酒聪搁 备炼惑 酒贰 啊沥阑 嘎免 荐 绝绰 惑炔捞扼搁 UIChapter WidgetBP 甫 喊档肺 操固电瘤.
	check(ChapterStages.Num() == StageNum);

	GET_SLOT(UPanelWidget, CP_TempNonMatineeBG)
	if (CP_TempNonMatineeBG.IsValid())
	{
		CP_TempNonMatineeBG->SetVisibility(bExternalDynamicSetupReady ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); // External dynamic (matinee) 悸诀捞 救登绢 乐阑 锭 焊捞扁 困茄 芭.
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
			// UniquelyDefinedStageParts 甫 啊瘤绊 悼利 积己窍绰 版快 茄窜拌 惑困 PanelSlot 阑 茫酒具 窃. 
			UCanvasPanel** DynamicCreateBasePanelPtr = UniquelyDefinedPartsCreatePanels.Find(Cnt + 1);
			UCanvasPanel* DynamicCreateBasePanel = DynamicCreateBasePanelPtr ? *DynamicCreateBasePanelPtr : nullptr;
			bool bIsDynamicallyCreated = (DynamicCreateBasePanel != nullptr);
			// CanvasSlotToAdjust 绰 胶农赴 荤捞令狼 弥惑困 菩澄捞 登绢具 茄促.
			UCanvasPanelSlot* CanvasSlotToAdjust = bIsDynamicallyCreated ?
				Cast<UCanvasPanelSlot>(DynamicCreateBasePanel->Slot) : Cast<UCanvasPanelSlot>(StageItem->Slot);
			if (CanvasSlotToAdjust)
			{
				//困摹焊沥.
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
	// UniquelyDefinedPartsCreatePanels 甫 啊廉柯 惑炔俊辑 荤侩捞 啊瓷
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
						// 积己等 UIP 啊 困摹窍绰 Slot 苞 茄窜拌 歹 棵扼埃 Slot 笛 促 度鞍捞 嘎苗淋.
						if (AddedCPSlot) {
							AddedCPSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetSize(ThisSettingValue.DisplayPanelSize);
						}
					}
					else { // 荤捞令 疙矫啊 救 登绢 乐栏搁 磊悼 嘎勉
						if (AddedCPSlot) {
							AddedCPSlot->SetAutoSize(true);
						}
						if (ParentPanelSlot) {
							ParentPanelSlot->SetAutoSize(true);
						}
					}
					NewCreated->Init();
					NewCreated->SetSubChapterNum(ThisSettingKey.SubStageNum); // Init 俊辑 搬惫 SubChapterNum 阑 促矫 盲况持阑 搏单 弊 锭 儒飘矾瘤瘤 臼栏妨搁 咯扁辑 积己等 巴甸阑 ChapterStages 硅凯俊 利例窍霸 持绢初酒具 窃.
					// 捞巴档 梆 ChapterStages 俊 尝况持绢具..
					OutCreatedParts.Add(NewCreated);
				}
			}
		}
	}
}

void UB2UIChapter::ResetStateForReuse()
{
	// ChapterFrame 俊辑 犁劝侩阑 秦辑 静绰 惑炔捞 乐绢辑 厚况初绰 芭..

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
	ResetStateForReuse(); // 犁荤侩 窍绰 版快甫 困秦.

	Super::Init();
	
	// 咯扁辑 ChapterStages 颇明甸狼 SubChapterNum 阑 鉴辑措肺 持霸 登骨肺 悼利 积己窍绰 UniquelyDefinedStageParts 甫 捞 傈俊 积己秦 初酒具 茄促.
	for (int Cnt = 0; Cnt < ChapterStages.Num(); ++Cnt )
	{
		auto StageItem = ChapterStages[Cnt];
		StageItem->Init();
		StageItem->SetSubChapterNum(Cnt + 1);
		//const FStage绢录绊& StageData = ;
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

void UB2UIChapter::SetStageDatas(/*TArray<FStage绢录绊> StageDatas*/)
{
	for (auto StageItem : ChapterStages)
		StageItem->BindDoc(/*FStage绢录绊*/);
}

//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
