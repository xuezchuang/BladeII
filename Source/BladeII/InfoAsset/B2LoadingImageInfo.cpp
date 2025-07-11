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
		// ���ܷ� ���� ��� ù��° �̹����� ������� �ش�.
		return LoadLoadingImage(InTAssetArray[0]);
	}

	return nullptr;
}

UTexture2D * UB2LoadingImageInfo::LoadLoadingImage(TSoftObjectPtr<class UTexture2D>& Texture)
{
	UTexture2D* LoadedTexture = nullptr;
	LoadedTexture = Texture.LoadSynchronous(); // �Ϲ����� �ε� �̹��� �ε��� ��� async flush �� ������ �ʿ䰡 ����. ���� ���Ƴ��� �ε��Ϸ��� Ÿ�̹��̹Ƿ�.

	if (LoadedTexture != nullptr)
	{
		LoadedTexture->AddToRoot(); // ��� Ư���� �̰� ��Ʈ�¿� �ִ´�. ��ε��� ���� �� ��.
		LoadedLoadingImages.AddUnique(LoadedTexture); // ���⿡ ���۷��� ��������
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
		OutRandomlySetIndex = LocalRandomIndex; // ���ϰ��� ���� ������ �ǹ̰� �ִ�.
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
	//// Ÿ��Ʋ���� �������� ���õ� Ŭ���� ������ ���. �����󿡼����� �̾�������.
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
			//ThisLoadedImage->MarkPendingKill(); // ��.. �۽�?
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

	// �ѹ� �̸� �ҷ��ָ� AssetPtr �� valid �� �� ��.

	for (int32 II = 0; II < LoadingImages_Default.Num(); ++II)
	{
		GetLoadingImageCommon(LoadingImages_Default, II);
	}

	// �������� �߰��Ǵ� ���..




}
#endif

// TAsset lazy-loading �� ����ϸ鼭 �����Ϳ��� �׿��� �ٷ� �ε��Ǿ� ���� �� �ִ� ���۷��� ����
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2LoadingImageInfo::CleanupOnPreSave()
{
	LoadedLoadingImages.Empty();
}
void UB2LoadingImageInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif