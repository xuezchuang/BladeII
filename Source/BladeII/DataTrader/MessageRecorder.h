#pragma once

#include "CommonStruct.h"
#include "B2MessageInfoConverter.h"

#include "Packet.h"
#include "PacketInterpreter.h"
#include "Serializer.h"

class FArchive;

class ReplayFormat
{
public:
    struct Header
    {
        uint32_t version;
        uint32_t roomType;
        uint32_t contentsSize;

        friend FArchive& operator<<(FArchive& Ar, Header& Ref)
        {
            Ar << Ref.version;
            Ar << Ref.roomType;
            Ar << Ref.contentsSize;
            return Ar;
        }
    };

    struct Sequence
    {
        uint16_t index;
        uint16_t options;
        uint32_t timestamp;
        FB2MessageInfoPtr messageInfo;
        //packet::Header roomMessageHeader;
        bool isValidRoomMessageHeader;
        FString logString;

        friend FArchive& operator<<(FArchive& Ar, Sequence& Ref)
        {
            //Ar << Ref.index;
            //Ar << Ref.options;
            //Ar << Ref.timestamp;

            //uint32_t payloadSize = 0;
            //Ar << payloadSize;

            //ANSICHAR* AnsiContents = (ANSICHAR*)FMemory::Malloc(payloadSize);
            //Ar.Serialize((void*)AnsiContents, payloadSize);

            //Ref.messageInfo = b2network::FB2MessageInfoConverter::FromRoomMessageNotifyBinary(AnsiContents, payloadSize);
            //if (Ref.messageInfo != nullptr)
            //{
            //    auto messageType = Ref.messageInfo->GetType();
            //    if (messageType == "FB2MessageInfoRoomMessageNotify")
            //    {
            //        const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomMessageNotify>(Ref.messageInfo);
            //        packet::ByteStream stream(b2MessageInfo->message);

            //        packet::Header	header;
            //        header.Version = packet::SliceStream<uint16>(stream);
            //        header.Type = packet::SliceStream<uint16>(stream);
            //        header.Sender = packet::SliceStream<uint32>(stream);
            //        header.Receiver = packet::SliceStream<uint32>(stream);
            //        header.Option = packet::SliceStream<uint8>(stream);

            //        Ref.roomMessageHeader = header;
            //        Ref.isValidRoomMessageHeader = true;

            //        FString headerTypeString = TEXT("");
            //        auto decoder = packet::PacketDecoderBank::GetInstance().GetDecoder(header.Type);
            //        if (decoder)
            //        {
            //            headerTypeString = decoder->GetName();
            //        }

            //        Ref.logString = FString::Printf(TEXT("%06.2f - [%05d] (%03d) => %03d <%04d> (%s)"),
            //            Ref.timestamp * 0.001f,
            //            Ref.index,
            //            header.Sender,
            //            header.Receiver, 
            //            header.Type,
            //            *headerTypeString);
            //    }
            //    else
            //    {
            //        Ref.isValidRoomMessageHeader = false;

            //        Ref.logString = FString::Printf(TEXT("%06.2f - [%05d] (   ) => (   ) <    > (%s)"),
            //            Ref.timestamp * 0.001f,
            //            Ref.index,
            //            ANSI_TO_TCHAR(messageType.c_str()));
            //    }
            //}

            //FMemory::Free(AnsiContents);

            return Ar;
        }
    };
};

class MessageRecorder
{
public:
	MessageRecorder();
	~MessageRecorder();

	void StartReplayByFile(const FString& ReplayFile);
    void StartReplayByData(TArray<uint8>& ReplayData);

    FORCEINLINE int32 GetCurrentSequenceIndex() const { return LastSeqIdx; }
    FORCEINLINE int32 GetNumSequences() const { return ReplaySequences.Num(); }
    FORCEINLINE int32 GetMarkForNextSequenceIndex() const {  return StopSeqIdx; }

    std::pair<int, FString> GetReplayLog(int32 index);
    FString GetReportExplainText();

    void SetSequenceIndex(int32 SequenceIdx);

    void ForwardToFirstMessage(uint16 MessageType);

	void ChangeMessageFilterType(uint16 MessgeType, bool IsAdd);
    int32 FindNextSequenceIndexForFilteredMessageType();

    void Rewind();
    void Play();
    void Pause();
    void Forward();

    int32 Tick(float DeltaSeconds);

private:
    ReplayFormat::Header ReplayHeader;
    TArray<ReplayFormat::Sequence> ReplaySequences;

	//std::set<uint16> MessageTypeFilterList;

    int32_t ReportReasonType;
    FString ReportExplain;

    bool HasValidPackets;

    // Sequences
    int32 LastSeqIdx; // last played sequence index
    int32 NextSeqIdx; // the sequence index played in the next tick
    int32 StopSeqIdx; //

    float ElapsedTime;

private:
	FString ReplayDirectory;
    FString Filename;

protected:
    void ResetSequences();
    void ProcessReplaySequence(const ReplayFormat::Sequence& Sequence);
};
