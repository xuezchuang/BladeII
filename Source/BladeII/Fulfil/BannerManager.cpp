
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
	{ // 捞 UBannerManager 啊 RootSet 牢 捞惑 咯扁俊 UPROPERTY 肺 楷搬登绊 TransientPackage 甫 Outer 肺 茄 tex 坷宏璃飘绰 GC 登瘤 臼绰促.
		// 弊矾唱 狼固惑 疙犬洒 窍扁 困秦 AddToRoot 甫 妮窃.
		// 荤捞令啊 农瘤 臼阑 巴捞绊, 皋葛府俊 焊粮秦 滴瘤 臼栏搁 概锅 肺厚 皋牢栏肺 哎 锭付促 促矫 促款肺靛甫 秦具 秦辑 IOS 俊辑档 RootSet 俊 持阑 鞘夸啊 乐阑 巴捞促.
		// 父老 酒囱 版快啊 惯积窍搁 咯扁 咆胶媚 包府啊 粱 甸绢啊具.
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
