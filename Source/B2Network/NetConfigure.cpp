#include "NetConfigure.h"
#include "B2NetworkPlugin.h"


#if 1 //!UE_BUILD_SHIPPING ��ȣȭ ��� ���� �ذ�� ����..
NetConfigure& NetConfigure::GetInstance()
{
	static NetConfigure inst;
	return inst;
}

NetConfigure::NetConfigure() : m_useCipher(true)
{

}

void NetConfigure::ParseCommandLine()
{
	if (FParse::Param(FCommandLine::Get(), TEXT("NoCipher")))
	{
		m_useCipher = false;
	}
}

bool NetConfigure::UseCipher() const
{
	return m_useCipher;
}
#endif