#pragma once

#include "B2NetworkPlugin.h"

#include "Config.h"

namespace b2network {

	class CredentialInfo
	{
	public:
		std::string m_platformUserId;
		int32 m_platformType;			// @see b2network::B2PlatformCode
		int32 m_osType;					// @see b2network::B2OSType
		int32 m_marketType;				// @see b2network::B2MarketType
		std::string m_countryCode;
		std::string m_langCode;
		std::string m_pushToken;
		std::string m_advertizeId;
		std::string m_accessToken;
		std::string m_client_cl;

	public:
		CredentialInfo()
		{
			Init();
		};

		//CredentialInfo(const CredentialInfo& rhs)
		//{
		//	*this = rhs;
		//}

		~CredentialInfo() {};

		void Init()
		{
			m_platformType = 0; //  b2network::B2PlatformCode::NONE;
			m_osType = 0;		//  b2network::B2OSType::PC;
			m_marketType = 0;	//  b2network::B2MarketType::NONE;
			m_platformUserId.clear();
			m_countryCode.clear();
			m_langCode.clear();
			m_pushToken.clear();
			m_advertizeId.clear();
			m_accessToken.clear();
			m_client_cl.clear();
		}

		CredentialInfo& operator=(const CredentialInfo& rhs)
		{
			m_platformType = rhs.m_platformType;
			m_osType = rhs.m_osType;
			m_marketType = rhs.m_marketType;
			m_platformUserId = rhs.m_platformUserId;
			m_countryCode = rhs.m_countryCode;
			m_langCode = rhs.m_langCode;
			m_pushToken = rhs.m_pushToken;
			m_advertizeId = rhs.m_advertizeId;
			m_accessToken = rhs.m_accessToken;
			m_client_cl = rhs.m_client_cl;
			return *this;
		}

		std::string ToString() const
		{
			std::ostringstream oss;
			oss << "PId=" << "[" << m_platformType << "]" << m_platformUserId << ","
				<< "osType=" << m_osType << ","
				<< "marketType=" << m_marketType << ","
				<< "countryCode=" << m_countryCode << ","
				<< "langCode=" << m_langCode << ","
				<< "pushToken=" << m_pushToken << ","
				<< "advertizeId=" << m_advertizeId << ","
				<< "accessToken=" << m_accessToken << ","
			    << "clientCL=" << m_client_cl;
			return oss.str();
		}

		std::string ToStringPUID() const
		{
			std::ostringstream oss;
			oss << "PUID=" << "[" << m_platformType << "]" << m_platformUserId;
			return oss.str();
		}
	};

}


