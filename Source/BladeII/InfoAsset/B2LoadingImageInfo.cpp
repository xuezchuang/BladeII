// Fill out your copyright notice in the Description page of Project Settings.


#include "B2LoadingImageInfo.h"
//#include "B2StageGameMode.h"
//#include "B2CounterAttackGameMode.h"
//#include "B2PVPGameMode.h"
//#include "B2TMGameMode.h"
//#include "B2RaidGameMode.h"
//#include "B2ControlGameMode.h"
//#include "B2HeroTowerGameMode.h"
//#include "B2GuildGameMode.h"
//#include "B2UIDocHelper.h"
//#include "B2DLCFrontGameMode.h"

UB2LoadingImageInfo::UB2LoadingImageInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TArray<TSoftObjectPtr<class UTexture2D>>& UB2LoadingImageInfo::GetTAssetArrayForGameMode(EB2GameMode InGM)
{

	if (InGM != EB2GameMode::Unknown && AllLoadingImages.Contains(InGM))
	{
		int ChapterIndex = GetChpaterIndex(InGM);

		if (AllLoadingImages[InGM].ChapterLoadingImages.Contains(ChapterIndex))
		{
			TArray<TSoftObjectPtr<class UTexture2D>> &Result = AllLoadingImages[InGM].ChapterLoadingImages[ChapterIndex].LoadingImages;

			if (Result.Num() > 0)
				return Result;
		}
	}
	return LoadingImages_Default;
}

UTexture2D* UB2LoadingImageInfo::GetLoadingImageCommon(TArray<TSoftObjectPtr<class UTexture2D>>& InTAssetArray, int32 InIndex)
{
	if (InTAssetArray.IsValidIndex(InIndex))
	{
		return LoadLoadingImage(InTAssetArray[InIndex]);
	}
	else if (InTAssetArray.Num() > 0)
	{
		// 예외로 없을 경우 첫번째 이미지를 노출시켜 준다.
		return LoadLoadingImage(InTAssetArray[0]);
	}

	return nullptr;
}

UTexture2D * UB2LoadingImageInfo::LoadLoadingImage(TSoftObjectPtr<class UTexture2D>& Texture)
{
	UTexture2D* LoadedTexture = nullptr;
	LoadedTexture = Texture.LoadSynchronous(); // 일반적인 로딩 이미지 로딩의 경우 async flush 를 걱정할 필요가 없다. 뭔가 막아놓고 로딩하려는 타이밍이므로.

	if (LoadedTexture != nullptr)
	{
		LoadedTexture->AddToRoot(); // 사용 특성상 이건 루트셋에 넣는다. 언로딩도 직접 해 줌.
		LoadedLoadingImages.AddUnique(LoadedTexture); // 여기에 레퍼런스 관리까지
		return LoadedTexture;
	}
	return nullptr;
}

int32 UB2LoadingImageInfo::GetChpaterIndex(EB2GameMode InGameMode) const
{
	/*switch (InGameMode)
	{
	case EB2GameMode::Unknown:
		break;
	case EB2GameMode::Lobby:
		break;
	case EB2GameMode::Scenario:
	{
		if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
		{
			return DocChapter->GetCurChapterNum();
		}
	}
		break;
	case EB2GameMode::CounterDungeon:
		break;
	case EB2GameMode::HeroTower:
		break;
	case EB2GameMode::PVP_Tag:
		break;
	case EB2GameMode::PVP_Team:
		break;
	case EB2GameMode::Raid:
		break;
	case EB2GameMode::Control:
		break;
	case EB2GameMode::Tutorial:
		break;
	case EB2GameMode::Guild:
		break;
	case EB2GameMode::Dimension:
		break;
	case EB2GameMode::PreRender:
		break;
	case EB2GameMode::End:
		break;
	default:
		break;
	}*/
	return 1;
}

UTexture2D* UB2LoadingImageInfo::RandGetLoadingImage(EB2GameMode InGameMode, int32& OutRandomlySetIndex)
{
	TArray<TSoftObjectPtr<class UTexture2D>>& RelevantAssetArray = GetTAssetArrayForGameMode(InGameMode);
	if (RelevantAssetArray.Num() == 0) {
		return NULL;
	}

	const int32 LocalRandomIndex = FMath::RandRange(0, RelevantAssetArray.Num() - 1);

	UTexture2D* LoadedImage = GetLoadingImageCommon(RelevantAssetArray, LocalRandomIndex);
	if (LoadedImage) 
	{
		OutRandomlySetIndex = LocalRandomIndex; // 리턴값이 있을 때에만 의미가 있다.
		return LoadedImage;
	}
	return NULL;
}

UTexture2D* UB2LoadingImageInfo::GetLoadingImageOfIndex(EB2GameMode InGameMode, int32 InImageIndex)
{
	TArray<TSoftObjectPtr<class UTexture2D>>& RelevantAssetArray = GetTAssetArrayForGameMode(InGameMode);
	
	return GetLoadingImageCommon(RelevantAssetArray, InImageIndex);
}

int32 UB2LoadingImageInfo::GetLoadingImageNum(EB2GameMode InGameMode)
{
	TArray<TSoftObjectPtr<UTexture2D>>& WantedArray = GetTAssetArrayForGameMode(InGameMode);
	return WantedArray.Num();
}

FString UB2LoadingImageInfo::GetLobbyLoginLoadingImageName() const
{
	//// 타이틀에서 랜덤으로 선택된 클래스 정보를 사용. 동영상에서부터 이어지도록.
	//EPCClass CachedTitleClass = AB2DLCFrontGameMode::GetSelectedFrontTitleMovieClass();
	//if (CachedTitleClass != EPCClass::EPC_End)
	//{
	//	const TSoftObjectPtr<UTexture2D>* FoundImagePtr = LobbyLoginLoadingImage_PerPCClass.Find(CachedTitleClass);
	//	if (FoundImagePtr && !FoundImagePtr->IsNull())
	//	{
	//		return FoundImagePtr->ToString();
	//	}
	//}
	return LobbyLoginLoadingImage_Fallback.ToString();
}

void UB2LoadingImageInfo::UnloadAll()
{
#if WITH_EDITOR
	if (GIsEditor) {
		return;
	}
#endif

	for (UTexture2D* ThisLoadedImage : LoadedLoadingImages)
	{
		if (ThisLoadedImage)
		{
			ThisLoadedImage->RemoveFromRoot();
			//ThisLoadedImage->MarkPendingKill(); // 음.. 글쎄?
		}
	}
	LoadedLoadingImages.Empty();
}

#if WITH_EDITOR
void UB2LoadingImageInfo::EditorLoadAll()
{
	if (GIsEditor == false){
		return;
	}

	// 한번 미리 불러주면 AssetPtr 이 valid 해 질 것.

	for (int32 II = 0; II < LoadingImages_Default.Num(); ++II)
	{
		GetLoadingImageCommon(LoadingImages_Default, II);
	}

	// 나머지도 추가되는 대로..




}
#endif

// TAsset lazy-loading 을 사용하면서 에디터에서 쌓여서 바로 로딩되어 버릴 수 있는 레퍼런스 제거
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2LoadingImageInfo::CleanupOnPreSave()
{
	LoadedLoadingImages.Empty();
}
void UB2LoadingImageInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif