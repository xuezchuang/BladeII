
#include "MessageRecorder.h"
#include "Event.h"
#include "ChannelManager.h"


//#include "HandlerBank.h"

MessageRecorder::MessageRecorder()
{
	//ReplayDirectory = FPaths::GameSavedDir() / FString(TEXT("Replay"));

	//IFileManager& FileManager = IFileManager::Get();
	//if (!FileManager.DirectoryExists(*ReplayDirectory))
	//	FileManager.MakeDirectory(*ReplayDirectory);

	////MessageTypeFilterList.empty();
}

MessageRecorder::~MessageRecorder()
{
}

void MessageRecorder::StartReplayByData(TArray<uint8>& ReplayData)
{
    // Init Replay data

    //ReplayHeader = {};
    //ReplaySequences.Empty();

    //MessageTypeFilterList = std::set<uint16>
    //{
    //    packet::STOPATTACK,
    //    //packet::COMBOEND,
    //    //packet::STOPGUARD
    //};

    // Read replay data

    //FMemoryReader Reader(ReplayData);

    //// Header

    //Reader << ReplayHeader;

    //// # of Sequences

    //uint32_t numSequences = 0;
    //Reader << numSequences;

    //// Sequences

    //for (uint32_t i = 0; i < numSequences; ++i)
    //{
    //    ReplayFormat::Sequence seq;
    //    Reader << seq;

    //    ReplaySequences.Add(seq);
    //}

    //// Report

    //Reader << ReportReasonType;

    //if (!Reader.ArIsError)
    //{
    //    uint32_t explainSize = 0;
    //    Reader << explainSize;
    //    if (!Reader.ArIsError && explainSize > 0)
    //    {
    //        ANSICHAR* explainString = (ANSICHAR*)FMemory::Malloc(explainSize + 1);
    //        FMemory::Memzero(explainString, explainSize + 1);
    //        Reader.Serialize((void*)explainString, explainSize);

    //        ReportExplain = UTF8_TO_TCHAR(explainString);
    //        FMemory::Free(explainString);
    //    }
    //    else
    //    {
    //        ReportExplain = TEXT("None");
    //    }
    //}
    //else
    //{
    //    ReportReasonType = 0;
    //    ReportExplain = TEXT("None");
    //}

    //Reader.FlushCache();

    //// TODO fix
    ////RecordedData.Empty();

    //Reader.Close();

    //// Initialize

    //HasValidPackets = ReplaySequences.Num() > 0 ? true : false;

    //// Make a channel for replay

    //if (HasValidPackets)
    //{
    //    for (const auto& seq : ReplaySequences)
    //    {
    //        if (seq.messageInfo == nullptr)
    //            continue;

    //        auto messageType = seq.messageInfo->GetType();
    //        if (messageType == "FB2MessageInfoRoomMessageNotify")
    //        {
    //            // NOTE Make an fake RoomJoinResponse and send to the handler

    //            const auto RoomMessage = std::static_pointer_cast<b2network::FB2MessageInfoRoomMessageNotify>(seq.messageInfo);

    //            auto RoomJoin = std::make_shared<b2network::FB2MessageInfoRoomJoinResponse>();
    //            RoomJoin->room_id = RoomMessage->room_id;
    //            RoomJoin->room_type = ReplayHeader.roomType;
    //            RoomJoin->host = nullptr;
    //            RoomJoin->participants.Empty();
    //            RoomJoin->properties.Empty();

    //            auto handler = data_trader::HandlerBank::GetInstance().GetHandler(RoomJoin->GetType().c_str());
    //            if (handler)
    //            {
    //                handler->Execute(RoomJoin);
    //            }

    //            break;
    //        }
    //    }
    //}

    //// Reset
    //ResetSequences();
}

void MessageRecorder::StartReplayByFile(const FString& _Filename)
{
	//if (_Filename.IsEmpty())
	//{
	//	FString clipboardString;
	//	FPlatformMisc::ClipboardPaste(clipboardString);
	//	Filename = ReplayDirectory / clipboardString;
	//}
	//else 
	//{
	//	Filename = ReplayDirectory / _Filename;
	//}

	//TArray<uint8> RecordedData;
	//FFileHelper::LoadFileToArray(RecordedData, *Filename);

 //   StartReplayByData(RecordedData);
}

void MessageRecorder::SetSequenceIndex(int32 SeqIdx)
{
    // NOTE not implemented yet
}

std::pair<int, FString> MessageRecorder::GetReplayLog(int32 SeqIdx)
{
	//if (SeqIdx > ReplaySequences.Num())
	//{
		return std::make_pair(0, TEXT(""));
	//}

	//const auto& Sequence = ReplaySequences[SeqIdx];

	//return std::make_pair(Sequence.roomMessageHeader.Sender, Sequence.logString);
}

FString MessageRecorder::GetReportExplainText()
{
    if (ReportExplain.IsEmpty())
    {
        return FString::Printf(TEXT("ReportReasonType : %d"), ReportReasonType);
    }
    else
    {
        return ReportExplain;
    }
}

void MessageRecorder::ForwardToFirstMessage(uint16 MessageType)
{
///*    for (int32 SeqIdx = NextSeqIdx; SeqIdx < ReplaySequences.Num(); ++SeqIdx)
//    {
//        const auto& Seq = ReplaySequences[SeqIdx];
//        if (Seq.isValidRoomMessageHeader)
//        {
//            if (Seq.roomMessageHeader.Type == MessageType)
//            {
//                StopSeqIdx = SeqIdx;
//                ElapsedTime = Seq.timestamp * 0.001f;
//
//                return;
//            }
//        */}
//    }
}

void MessageRecorder::ChangeMessageFilterType(uint16 MessageType, bool IsAdd)
{
    /*auto it = MessageTypeFilterList.find(MessageType);
    if (it != MessageTypeFilterList.end())
    {
        if (!IsAdd)
        {
            MessageTypeFilterList.erase(it);
        }
    }
    else
    {
        if (IsAdd)
        {
            MessageTypeFilterList.insert(MessageType);
        }
    }*/
}

int32 MessageRecorder::FindNextSequenceIndexForFilteredMessageType()
{
    ForwardToFirstMessage(packet::STOPATTACK);
    return StopSeqIdx;
}

void MessageRecorder::Rewind()
{
    ResetSequences();
    ForwardToFirstMessage(packet::SYNC_READY_TO_STARTGAME);
}

void MessageRecorder::Play()
{
    StopSeqIdx = ReplaySequences.Num() - 1;
}

void MessageRecorder::Pause()
{
    StopSeqIdx = LastSeqIdx;
}

void MessageRecorder::Forward()
{
    StopSeqIdx = FindNextSequenceIndexForFilteredMessageType();
}

int32 MessageRecorder::Tick(float DeltaSeconds)
{
    const auto ElapsedTimeInTick = ElapsedTime + DeltaSeconds;

    while (NextSeqIdx < ReplaySequences.Num())
    {
        if (LastSeqIdx >= StopSeqIdx)
        {
            // Stop
            break;
        }

        const auto& Seq = ReplaySequences[NextSeqIdx];
        if ((Seq.timestamp * 0.001f) <= ElapsedTimeInTick)
        {
            ProcessReplaySequence(Seq);

            // Advance
            LastSeqIdx = NextSeqIdx++;
        }
        else
        {
            // Not Yet
            break;
        }
    }

    ElapsedTime += DeltaSeconds;

    return LastSeqIdx;
}

void MessageRecorder::ResetSequences()
{
    LastSeqIdx = -1;
    NextSeqIdx = 0;
    StopSeqIdx = ReplaySequences.Num() - 1;

    ElapsedTime = 0.0f;
}

void MessageRecorder::ProcessReplaySequence(const ReplayFormat::Sequence& Seq)
{
	if (Seq.messageInfo != nullptr)
	{
        const auto messageType = Seq.messageInfo->GetType();

		if (messageType == "FB2MessageInfoRoomParticipantJoinNotify")
		{
			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomParticipantJoinNotify>(Seq.messageInfo);

			data_trader::RoomParticipantJoinNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance()
				.Signal(b2MessageInfo->room_id, b2MessageInfo->joiner);
		}
		else if (messageType == "FB2MessageInfoRoomParticipantLeaveNotify")
		{
			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomParticipantLeaveNotify>(Seq.messageInfo);

			data_trader::RoomParticipantLeaveNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance()
				.Signal(b2MessageInfo->room_id, b2MessageInfo->leaver);
		}
		else if (messageType == "FB2MessageInfoRoomMessageNotify")
		{
			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomMessageNotify>(Seq.messageInfo);

			data_trader::RoomMessageNotifyClass<const FString&, b2network::B2RoomParticipantPtr, const FString&>::GetInstance()
				.Signal(b2MessageInfo->room_id, b2MessageInfo->sender, b2MessageInfo->message);
		}
		else if (messageType == "FB2MessageInfoRoomHostChangedNotify")
		{
			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomHostChangedNotify>(Seq.messageInfo);

			data_trader::RoomHostChangedNotifyClass<const FString&, int32, int32>::GetInstance()
				.Signal(b2MessageInfo->room_id, b2MessageInfo->room_type, b2MessageInfo->host_id);
		}
		else if (messageType == "FB2MessageInfoRoomPropertyUpdatedNotify")
		{
			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomPropertyUpdatedNotify>(Seq.messageInfo);

			data_trader::RoomPropertyUpdatedNotifyClass<const FString&, const TArray<b2network::B2RoomPropertyPtr>&>::GetInstance()
				.Signal(b2MessageInfo->room_id, b2MessageInfo->properties);
		}
	}
}
