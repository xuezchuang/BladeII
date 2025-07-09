#include "BladeII.h"
#include "BannerManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "Engine/Texture2DDynamic.h"
UBannerManager::UBannerManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	LobbyBannerImages.Empty();
}

void UBannerManager::SetLobbyBannerImage(UTexture2DDynamic* tex, int32 idx)
{
	if (tex)
	{ // 이 UBannerManager 가 RootSet 인 이상 여기에 UPROPERTY 로 연결되고 TransientPackage 를 Outer 로 한 tex 오브젝트는 GC 되지 않는다.
		// 그러나 의미상 명확히 하기 위해 AddToRoot 를 콜함.
		// 사이즈가 크지 않을 것이고, 메모리에 보존해 두지 않으면 매번 로비 메인으로 갈 때마다 다시 다운로드를 해야 해서 IOS 에서도 RootSet 에 넣을 필요가 있을 것이다.
		// 만일 아닌 경우가 발생하면 여기 텍스쳐 관리가 좀 들어가야.
		tex->AddToRoot();
		LobbyBannerImages.Add(idx, tex);
	}
}

const TMap<int32, UTexture2DDynamic*>& UBannerManager::GetLobbyBannerImage() const
{
	return LobbyBannerImages;
}

UBannerManager* StaticFindLobbyBannerCacher()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(B2GameModule && B2GameModule->GetLobbyBannerCacher());
	return B2GameModule ? B2GameModule->GetLobbyBannerCacher() : NULL;
}
