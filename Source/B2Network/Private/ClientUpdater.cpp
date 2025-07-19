#include "ClientUpdater.h"
#include "B2NetworkPlugin.h"

#include "Exception.h"
#include "Client.h"


using namespace b2network;

ClientUpdater::ClientUpdater(Client* client)
    :m_client(client),
    m_thread(nullptr),
    m_stopped(false)
{
}

ClientUpdater::~ClientUpdater()
{
    // TODO need to delete thread instance?
}

uint32 ClientUpdater::Run()
{
    m_client->WorkThreadUpdate();

    m_stopped = true;

    return 0;
}

bool ClientUpdater::Start()
{
    if (m_thread)
    {
        return false;
    }

    m_thread = FRunnableThread::Create(this, TEXT("B2NetworkUpdater"));
    if (m_thread)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint32 ClientUpdater::GetThreadId()
{
    if (m_thread)
    {
        return m_thread->GetThreadID();
    }
    else
    {
        return 0;
    }
}

void ClientUpdater::Wait()
{
    if (m_thread)
    {
        m_thread->WaitForCompletion();
    }
}
