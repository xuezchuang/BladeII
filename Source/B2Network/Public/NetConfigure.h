#pragma once

#if 1// !UE_BUILD_SHIPPING
class NetConfigure
{
public:
	static NetConfigure& GetInstance(); 

	NetConfigure();

	NetConfigure(const NetConfigure&) = delete;
	NetConfigure& operator =(const NetConfigure&) = delete;

	void ParseCommandLine();

	bool UseCipher() const;

private:
	bool m_useCipher;
};
#endif