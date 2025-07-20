#pragma once

class FB2AuthInfo
{
private:
	// credential
	FString PlatformUserId;
	int32 PlatformType;
	int32 OsType;
	int32 MarketType;
	FString CountryCode;
	FString LangCode;
	FString PushToken;
	FString AdvertizeId;
	FString AccessToken;

	// session server info
	FString SesssionAddr;		// ip or FQDN
	int32 SessionPort;

public:
	static FB2AuthInfo& GetInstance() 
	{
		static FB2AuthInfo	inst;
		return inst;
	}

public:
	FB2AuthInfo(){ Init(); };
	~FB2AuthInfo() {};

	void Init()
	{
		PlatformType = 0;
		OsType = 0;
		MarketType = 0;
		PlatformUserId = TEXT("");
		CountryCode = TEXT("");
		LangCode= TEXT("");
		PushToken= TEXT("");
		AdvertizeId= TEXT("");
		AccessToken= TEXT("");
		SesssionAddr= TEXT("");
		SessionPort = 0;
	}

	const FString&	GetPlatformUserId() const
	{
		return PlatformUserId;
	}

	void SetPlatformUserId(const FString& val)
	{
		PlatformUserId = val;
	}


	int32 GetPlatformType() const
	{
		return PlatformType;
	}

	void SetPlatformType(int32 val)
	{
		PlatformType = val;
	}

	int32 GetOsType() const
	{
		return OsType;
	}

	void SetOsType(int32 val)
	{
		OsType = val;
	}

	int32 GetMarketType() const
	{
		return MarketType;
	}

	void SetMarketType(int32 val)
	{
		MarketType = val;
	}

	const FString&	GetCountryCode() const
	{
		return CountryCode;
	}

	void SetCountryCode(const FString& val)
	{
		CountryCode = val;
	}

	const FString&	GetLangCode() const
	{
		return LangCode;
	}

	void SetLangCode(const FString& val)
	{
		LangCode = val;
	}

	const FString&	GetPushToken() const
	{
		return PushToken;
	}

	void SetPushToken(const FString& val)
	{
		PushToken = val;
	}

	const FString&	GetAdvertizeId() const
	{
		return AdvertizeId;
	}

	void SetAdvertizeId(const FString& val)
	{
		AdvertizeId = val;
	}

	const FString&	GetAccessToken() const
	{
		return AccessToken;
	}

	void SetAccessToken(const FString& val)
	{
		AccessToken = val;
	}

	const FString&	GetSesssionAddr() const
	{
		return SesssionAddr;
	}

	void SetSesssionAddr(const FString& val)
	{
		SesssionAddr = val;
	}

	int32 GetSessionPort() const
	{
		return SessionPort;
	}

	void SetSessionPort(int32 val)
	{
		SessionPort = val;
	}
};