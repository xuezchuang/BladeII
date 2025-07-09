// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, ACTION SQUARE

#pragma once

#include "B2MessageInfo.h"
#include "B2AuthInfo.h"

class FB2NetworkDriver;

class FB2NetworkBridge
{
private:
	static std::shared_ptr<FB2NetworkBridge> m_pInstance;
	std::shared_ptr<FB2NetworkDriver> m_pNetworkDriver = nullptr;

public:
	FB2NetworkBridge();
	~FB2NetworkBridge();

	static std::shared_ptr<FB2NetworkBridge> GetInstance();

	void Init();
	void Shutdown();

    void CloseChannel(const std::string& reason);

    bool Configure(const std::string& optionName, bool value);

    void ResetNetwork();

	bool SendB2Message(FB2MessageInfoPtr messageInfo);

	void ChannelHeartbeat(const FString&);

	// debug
    bool IsServiceAvailable() const;
	bool IsChannelDisconnected() const;
	void SetHeartbeatPeriod(int sessionPeriod, int channelPeriod);

	int64 GetAccountID();

    void Dump();
	void TestSodium();
};

typedef std::shared_ptr<FB2NetworkBridge> FB2NetworkBridgePtr;
