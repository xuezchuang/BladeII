#include "B2NetworkPlugin.h"

//#include <sodium.h>
#if 1 //!UE_BUILD_SHIPPING ��ȣȭ ��� ���� �ذ�� ����..
#include "NetConfigure.h"
#endif

class FB2Network : public FDefaultModuleImpl
{
public:
	FB2Network() {}
	~FB2Network() {}

	virtual void StartupModule() override;
};

void FB2Network::StartupModule()
{
//#if !PLATFORM_MAC
//	if (sodium_init() == -1)
//	{
//		FPlatformMisc::LowLevelOutputDebugString(TEXT("failed to intialize sodium library..."));
//		checkf(false, TEXT("failed to intialize sodium library..."));
//	}
//#endif
//#if 1 //!UE_BUILD_SHIPPING ��ȣȭ ��� ���� �ذ�� ����..
//	NetConfigure::GetInstance().ParseCommandLine();
//#endif
}

IMPLEMENT_MODULE(FB2Network, B2Network);
