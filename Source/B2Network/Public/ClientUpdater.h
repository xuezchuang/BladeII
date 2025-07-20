#pragma once

#include "Config.h"
#include "HAL/Runnable.h"          // ���� FRunnable
#include "HAL/RunnableThread.h"    // �����Ҫ�����߳� (FRunnableThread)
#include "HAL/PlatformProcess.h"   // ����� FPlatformProcess::Sleep

namespace b2network {

	class Client;

	class ClientUpdater : public FRunnable
	{
	private:
		Client* m_client;
		FRunnableThread* m_thread;
		bool m_stopped;

	public:
		ClientUpdater(Client* client);
		virtual ~ClientUpdater();

		virtual uint32 Run() final;

		bool Start();
		uint32 GetThreadId();
		void Wait();
	};

} // namespace b2network
