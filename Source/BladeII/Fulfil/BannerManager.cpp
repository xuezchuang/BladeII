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
	{ // �� UBannerManager �� RootSet �� �̻� ���⿡ UPROPERTY �� ����ǰ� TransientPackage �� Outer �� �� tex ������Ʈ�� GC ���� �ʴ´�.
		// �׷��� �ǹ̻� ��Ȯ�� �ϱ� ���� AddToRoot �� ����.
		// ����� ũ�� ���� ���̰�, �޸𸮿� ������ ���� ������ �Ź� �κ� �������� �� ������ �ٽ� �ٿ�ε带 �ؾ� �ؼ� IOS ������ RootSet �� ���� �ʿ䰡 ���� ���̴�.
		// ���� �ƴ� ��찡 �߻��ϸ� ���� �ؽ��� ������ �� ����.
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
