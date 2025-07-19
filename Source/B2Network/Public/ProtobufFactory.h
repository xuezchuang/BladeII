#pragma once

#include "Config.h"

namespace b2network {

class B2NETWORK_API ProtobufFactory
{
public:
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef google::protobuf::Descriptor Descriptor;

    enum ServerType { Session, Channel };
    enum MessageType { Request, Response, Command, Notify };

    struct Metadata
    {
        Metadata(const Message* message,
                 const ProtobufFactory::ServerType serverType_,
                 const ProtobufFactory::MessageType messageType_,
                 const Descriptor* descriptor_)
            :instance(message),
             serverType(serverType_),
             messageType(messageType_),
             descriptor(descriptor_)
        {
        }

        const Message* instance;
        const ProtobufFactory::ServerType serverType;
        const ProtobufFactory::MessageType messageType;
        const Descriptor* descriptor;
    };

private:
    //static google::protobuf::DynamicMessageFactory s_factory;
    static std::unordered_map<uint32_t, Metadata> s_metadataMap;

    //static std::unordered_map<uint32_t, uint32_t> s_relationMap;
    static std::unordered_map<uint32_t, uint32_t> s_requestToResponseMap;
    static std::unordered_map<uint32_t, uint32_t> s_responseToRequestMap;

    //static std::unordered_map<uint32_t, const Message*> s_prototypes;
    //static std::unordered_map<uint32_t, const Descriptor*> s_descriptors;

	static std::string s_masterDataHash;

public:
    static void Initialize();
    static void Finalize();

    static MessagePtr CreateMessage(uint32_t type);

    static bool IsRequest(const MessagePtr message);
    static bool IsResponse(const MessagePtr message);
    static bool IsNotify(const MessagePtr message);

    static bool IsTypeOf(const MessagePtr message, const Message& type);
    static bool IsResponseOf(const MessagePtr request, const MessagePtr response);

    static uint32_t GetResponseType(const MessagePtr request);

    static Metadata GetMetadata(const uint32_t type);
    static Metadata GetMetadata(const Message& message);
    static Metadata GetMetadata(const MessagePtr message);

	static void SetMasterDataHash(const std::string& hashStr);
	static std::string GetMasterDataHash();

private:
    static void Register(const ServerType serverType, const Message* request, const Message* response);
    static uint32_t Register(const ServerType serverType, const MessageType messageType, const Message* prototype);
};

}

namespace std {

template <> struct hash<b2network::ProtobufFactory::ServerType>
{
    size_t operator()(const b2network::ProtobufFactory::ServerType& serverType) const
    {
        return std::hash<int>()(serverType);
    }
};

}
