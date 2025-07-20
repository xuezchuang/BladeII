#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/Texture2DDynamic.h"
#include "BannerManager.generated.h"

UCLASS()
class UBannerManager : public UObject
{
	GENERATED_BODY()

public:
	UBannerManager(const FObjectInitializer& ObjectInitializer);

	void SetLobbyBannerImage(UTexture2DDynamic* tex, int32 idx);
	const TMap<int32, UTexture2DDynamic*>& GetLobbyBannerImage() const;
private:

	UPROPERTY(Transient)
	TMap<int32, UTexture2DDynamic*> LobbyBannerImages;
};

UBannerManager* StaticFindLobbyBannerCacher();