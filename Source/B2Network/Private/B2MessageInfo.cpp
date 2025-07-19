#include "B2MessageInfo.h"
#include "B2MessageInfoConverter.h"
#include "B2NetworkPlugin.h"


#include "Client.h"

#include <unordered_map>

namespace b2network 
{
	typedef std::shared_ptr<Client> ClientPtr;

	//////////////////////////////////////////////////////////////////////////
	FB2MessageHandlerManagerPtr FB2MessageHandlerManager::m_pInstance = nullptr;

	FB2MessageHandlerManagerPtr FB2MessageHandlerManager::GetInstance()
	{
		if (!m_pInstance)
		{
//			m_pInstance = std::make_unique<FB2MessageHandlerManager>();
			m_pInstance = std::unique_ptr<FB2MessageHandlerManager>(new FB2MessageHandlerManager);
		}
		return m_pInstance;
	}

	B2MessageInfoHandler FB2MessageHandlerManager::GetHandler(const std::string& MessageType)
	{
		const auto it = handlerMap.find(MessageType);
		if (it != handlerMap.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void FB2MessageHandlerManager::RegistHandler(const std::string& MessageType, B2MessageInfoHandler Handler)
	{
		const auto it = handlerMap.find(MessageType);
		if (it == handlerMap.end())
		{
			handlerMap.insert(make_pair(MessageType, Handler));
		}
	}

    B2MessageInfoListener FB2MessageHandlerManager::GetListener(const std::string& messageType)
    {
        const auto it = listenerMap.find(messageType);
        if (it != listenerMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void FB2MessageHandlerManager::RegisterListener(const std::string& messageType, B2MessageInfoListener listener)
    {
        const auto it = listenerMap.find(messageType);
        if (it == listenerMap.end())
        {
            listenerMap.emplace(messageType, listener);
        }
    }

	//////////////////////////////////////////////////////////////////////////
	void FB2MessageInfoRequestSignUpHandler(ClientPtr client, FB2MessageInfoPtr messageInfo)
	{
		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSignUp>(messageInfo);
		std::string name = TCHAR_TO_ANSI(*(B2MessageInfo->name));
        /*
		client->SignUp(name,
			[client, messageInfo](int resultCode)
			{
				auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSignUp>();
				B2MessageInfoResponse->SetErrorCode(resultCode);
				messageInfo->DoCallback(B2MessageInfoResponse);
			});
        */
    }

    //////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN_WITH_HANDLER(class_name, handler_name)						\
	class_name::class_name(																					\
		FB2MessageCallback InSuccessCallback,																\
		FB2MessageCallback InFailCallback)																	\
		: FB2MessageInfoRequest(#class_name, InSuccessCallback, InFailCallback)								\
	{																										\
	};																										\
	static struct _register_##class_name : public reflection::ReflectionRegisterHelper<class_name>			\
	{																										\
		_register_##class_name()																			\
		{																									\
			std::string strClassName = #class_name;															\
			FB2MessageHandlerManager::GetInstance()->RegistHandler(strClassName, &handler_name);			\
			register_class(strClassName);																	\
			const uint32 field_num = class_name::field_num;


#define IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(class_name, sub_name)											\
	class_name::class_name(																					\
		FB2MessageCallback InSuccessCallback,																\
		FB2MessageCallback InFailCallback)																	\
		: FB2MessageInfoRequest(#class_name, InSuccessCallback, InFailCallback)								\
	{																										\
	};																										\
	void Handler_##sub_name(ClientPtr client, FB2MessageInfoPtr messageInfo)								\
	{																										\
		auto request = FB2MessageInfoConverter::To##sub_name##Request(messageInfo);							\
		client->SendRequest(request,																		\
			[client, messageInfo](MessagePtr message)														\
			{																								\
				auto response = FB2MessageInfoConverter::From##sub_name##Response(message);					\
				messageInfo->DoCallback(response);															\
			});																								\
	};																										\
	static struct _register_##class_name : public reflection::ReflectionRegisterHelper<class_name>			\
	{																										\
		_register_##class_name()																			\
		{																									\
			std::string strClassName = #class_name;															\
			FB2MessageHandlerManager::GetInstance()->RegistHandler(strClassName, &Handler_##sub_name);		\
			register_class(strClassName);																	\
			const uint32 field_num = class_name::field_num;

#define IMPLEMENT_B2MESSAGEINFO_REQUEST_END(count)															\
			static_assert(count == field_num, "different number of field from declartion");					\
		};																									\
	} __AUTO_NAME;

    //////////////////////////////////////////////////////////////////////////
    // command

#define IMPLEMENT_B2MESSAGEINFO_COMMAND_BEGIN(class_name, sub_name)											\
	void Handler_Command##sub_name(ClientPtr client, FB2MessageInfoPtr messageInfo)							\
    {																										\
		MessagePtr command = FB2MessageInfoConverter::To##sub_name##Command(messageInfo);					\
		client->SendCommand(command);																		\
    };																										\
	static struct _register_##class_name : public reflection::ReflectionRegisterHelper<class_name>			\
    {																										\
        _register_##class_name()																			\
    	{																									\
			std::string strClassName = #class_name;															\
			FB2MessageHandlerManager::GetInstance()->RegistHandler(strClassName, &Handler_Command##sub_name);\
			register_class(strClassName);																	\
			const uint32 field_num = class_name::field_num;

#define IMPLEMENT_B2MESSAGEINFO_COMMAND_END(count)															\
			static_assert(count == field_num, "different number of field from declartion");					\
    	};																									\
   	} __AUTO_NAME;

	//////////////////////////////////////////////////////////////////////////
	// notify

#define IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(class_name, sub_name)												\
	FB2MessageInfoPtr Handler_Notify##sub_name(ClientPtr client, MessagePtr message)							\
    {																											\
		return FB2MessageInfoConverter::From##sub_name##Notify(message);										\
    };																											\
	static struct _register_##class_name : public reflection::ReflectionRegisterHelper<class_name>				\
    {																											\
		_register_##class_name()																				\
        {																										\
			std::string strClassName = #class_name;																\
			FB2MessageHandlerManager::GetInstance()->RegisterListener(strClassName, &Handler_Notify##sub_name);	\
			register_class(strClassName);																		\
			const uint32 field_num = class_name::field_num;

#define IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(count)																\
			static_assert(count == field_num, "different number of field from declartion");						\
        };																										\
    } __AUTO_NAME;

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccount, GetAccount)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFindAccount, FindAccount)
		REFLECTION_REGISTER_FIELD(TArray<B2FindAccountQueryPtr>, query_parameters)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccountItem, GetAccountItem)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccountCostume, GetAccountCostume)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMasterData, GetMasterData)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetLatestEventData, GetLatestEventData)
		REFLECTION_REGISTER_FIELD(int64, last_update_time)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN_WITH_HANDLER(FB2MessageInfoRequestSignUp, FB2MessageInfoRequestSignUpHandler)
		REFLECTION_REGISTER_FIELD(FString, name)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSellItem, SellItem)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSellConsumableItem, SellConsumableItem)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, sell_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestConvertData, ConvertData)
		REFLECTION_REGISTER_FIELD(int32, item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestConvertItem, ConvertItem)
		REFLECTION_REGISTER_FIELD(int32, item_id)
		REFLECTION_REGISTER_FIELD(int32, convert_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEquipItem, EquipItem)
		REFLECTION_REGISTER_FIELD(int32, preset_id)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnequipItem, UnequipItem)
		REFLECTION_REGISTER_FIELD(int32, preset_id)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEquipCostume, EquipCostume)
		REFLECTION_REGISTER_FIELD(TArray<int64>, costume_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnequipCostume, UnequipCostume)
		REFLECTION_REGISTER_FIELD(TArray<int64>, costume_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceCostume, EnhanceCostume)
		REFLECTION_REGISTER_FIELD(int64, costume_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDismantleCostumes, DismantleCostumes)
		REFLECTION_REGISTER_FIELD(TArray<int64>, costume_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLockCostumes, LockCostumes)
		REFLECTION_REGISTER_FIELD(TArray<int64>, costume_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnlockCostumes, UnlockCostumes)
		REFLECTION_REGISTER_FIELD(TArray<int64>, costume_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLockItem, LockItem)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnlockItem, UnlockItem)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestExtendInventory, ExtendInventory)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, extend_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLevelupItem, LevelupItem)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(TArray<int64>, material_item_ids)
		REFLECTION_REGISTER_FIELD(bool, auto_levelup)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceItem, EnhanceItem)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, scroll_item_id)
		REFLECTION_REGISTER_FIELD(bool, use_protection)
		REFLECTION_REGISTER_FIELD(int32, protection_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDismantleItems, DismantleItems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestExchangeItemEnhanceLevel, ExchangeItemEnhanceLevel)
		REFLECTION_REGISTER_FIELD(int64, item_id_1)
		REFLECTION_REGISTER_FIELD(int64, item_id_2)
		REFLECTION_REGISTER_FIELD(int32, price_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestOpenSealSlotItem, OpenSealSlotItem)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, seal_slot)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCarveSealItem, CarveSealItem)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, seal_slot)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSurpassItem, SurpassItem)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpgradeItem, UpgradeItem)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestComposeItem, ComposeItem)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, sub_item_id)
		REFLECTION_REGISTER_FIELD(int32, current_preset_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestChangeItemCharacterType, ChangeItemCharacterType)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetChangeItemCharacterTypeCost, GetChangeItemCharacterTypeCost)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartStage, StartStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, selected_option_flags)
		REFLECTION_REGISTER_FIELD(int32, stage_repeat_mode)
		REFLECTION_REGISTER_FIELD(bool, boost_mode)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_preset_id)
		REFLECTION_REGISTER_FIELD(int32, tag_character_preset_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(9)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestClearStage, ClearStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, clear_time_second)
		REFLECTION_REGISTER_FIELD(TArray<B2KillMonsterInfoPtr>, kill_monster_infos)
		REFLECTION_REGISTER_FIELD(int32, total_monster_kill_count)
		REFLECTION_REGISTER_FIELD(bool, is_all_survival)
		REFLECTION_REGISTER_FIELD(int32, qte_start_count)
		REFLECTION_REGISTER_FIELD(int32, qte_finish_count)
		REFLECTION_REGISTER_FIELD(B2ItemSpotSaleOptionPtr, item_sale_option)
		REFLECTION_REGISTER_FIELD(B2AetherSpotSaleOptionPtr, aether_sale_option)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSweepStage, SweepStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, main_character_preset_id)
		REFLECTION_REGISTER_FIELD(int32, tag_character_preset_id)
		REFLECTION_REGISTER_FIELD(int32, repeat_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFailStage, FailStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, reason)
		REFLECTION_REGISTER_FIELD(int32, play_time_second)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResurrection, Resurrection)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, selected_buff_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckBladePointCharge, CheckBladePointCharge)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetItemSurpassCost, GetItemSurpassCost)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetItemUpgradeCost, GetItemUpgradeCost)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetItemComposeCost, GetItemComposeCost)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int32, current_preset_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetActInfo, GetActInfo)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, act_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetStageInfo, GetStageInfo)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoContentsModeStateNotify, ContentsModeState)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int32, prev_state)
		REFLECTION_REGISTER_FIELD(int32, current_state)
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMatchStatus, GetMatchStatus)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoMatchmakingRequest, Matchmaking)
        REFLECTION_REGISTER_FIELD(int32, match_type)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(bool, use_additional_count)
        REFLECTION_REGISTER_FIELD(FString, room_seed)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRaidMatchmakingRequest, RaidMatchmaking)
        REFLECTION_REGISTER_FIELD(int32, raid_req_type)
        REFLECTION_REGISTER_FIELD(int32, character_type)
        REFLECTION_REGISTER_FIELD(int32, raid_type)
        REFLECTION_REGISTER_FIELD(int32, raid_step)
		REFLECTION_REGISTER_FIELD(B2EndpointPtr, channel_endpoint)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoCancelMatchmakingRequest, CancelMatchmaking)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

    //////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoMatchResultRequest, MatchResult)
		REFLECTION_REGISTER_FIELD(B2MatchResultPtr, result)
		REFLECTION_REGISTER_FIELD(int32, match_type)
		REFLECTION_REGISTER_FIELD(B2RoomCharacterPtr, my_main_character)
		REFLECTION_REGISTER_FIELD(B2RoomCharacterPtr, my_tag_character)
		REFLECTION_REGISTER_FIELD(B2RoomCharacterPtr, opponent_main_character)
		REFLECTION_REGISTER_FIELD(B2RoomCharacterPtr, opponent_tag_character)
		REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(bool, is_give_up)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(8)

	//////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoInvitationRequest, Invitation)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(FString, room_id)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoAcceptInvitationRequest, AcceptInvitation)
        REFLECTION_REGISTER_FIELD(int32, invitation_type)
        REFLECTION_REGISTER_FIELD(FString, inviter)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFindGuild, FindGuild)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestObtainGuildMaster, ObtainGuildMaster)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestHandOverGuildMaster, HandOverGuildMaster)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildRankingList, GetGuildRankingList)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoInvitationNotify, Invitation)
        REFLECTION_REGISTER_FIELD(int32, invitation_type)
        REFLECTION_REGISTER_FIELD(FString, inviter)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
        REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRaidInvitationNotify, RaidInvitation)
		REFLECTION_REGISTER_FIELD(FString, inviter)
		REFLECTION_REGISTER_FIELD(FString, invitation_key)
		REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
		REFLECTION_REGISTER_FIELD(int32, map_id)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(5)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoAcceptInvitationNotify, AcceptInvitation)
        REFLECTION_REGISTER_FIELD(int32, invitation_type)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
        REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(4)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoDeclineInvitationRequest, DeclineInvitation)
        REFLECTION_REGISTER_FIELD(int32, invitation_type)
        REFLECTION_REGISTER_FIELD(FString, inviter)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
        REFLECTION_REGISTER_FIELD(FString, invitee)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoDeclineInvitationNotify, DeclineInvitation)
        REFLECTION_REGISTER_FIELD(int32, invitation_type)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomJoinRequest, RoomJoin)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(int32, room_access)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomPropertyPtr>, properties)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomCharacterPtr>, room_characters)
		REFLECTION_REGISTER_FIELD(TArray<B2PresetNumPtr>, character_presets)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(6)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomLeaveRequest, RoomLeave)
        REFLECTION_REGISTER_FIELD(FString, room_id)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_COMMAND_BEGIN(FB2MessageInfoRoomMessageCommand, RoomMessage)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int32, receiver_id)
		REFLECTION_REGISTER_FIELD(FString, message)
    IMPLEMENT_B2MESSAGEINFO_COMMAND_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomKickRequest, RoomKick)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int64, kick_account_id)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomParticipantJoinNotify, RoomParticipantJoin)
		REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(B2RoomParticipantPtr, joiner)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomParticipantLeaveNotify, RoomParticipantLeave)
		REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(B2RoomParticipantPtr, leaver)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomHostChangedNotify, RoomHostChanged)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(int32, host_id)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomMessageNotify, RoomMessage)
		REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(B2RoomParticipantPtr, sender)
		REFLECTION_REGISTER_FIELD(FString, message)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(4)

    //////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomHeartbeatNotify, RoomHeartbeat)
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(0)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomSetPropertyRequest, RoomSetProperty)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomPropertyPtr>, properties)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoRoomPropertyUpdatedNotify, RoomPropertyUpdated)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomPropertyPtr>, properties)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(2)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomInviteRequest, RoomInvite)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(int32, invitation_number)
		REFLECTION_REGISTER_FIELD(int32, map_id)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomCloseRequest, RoomClose)
        REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int32, map_id)
		REFLECTION_REGISTER_FIELD(int32, reason)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomLeaveResultRequest, RoomLeaveResult)
        REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int64, leaved_account_id)
		REFLECTION_REGISTER_FIELD(FString, match_token)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRoomAccessControlRequest, RoomAccessControl)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int32, access)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetRoomDebugMode, SetRoomDebugMode)
		REFLECTION_REGISTER_FIELD(bool, skip_room_heart_beat_check)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoReportUserViaChannelRequest, ReportUserViaChannel)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
		REFLECTION_REGISTER_FIELD(int32, mode_type)
		REFLECTION_REGISTER_FIELD(int32, reason_type)
		REFLECTION_REGISTER_FIELD(FString, explain)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoWorldJoinRequest, WorldJoin)
		REFLECTION_REGISTER_FIELD(FString, world_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoWorldLeaveRequest, WorldLeave)
		REFLECTION_REGISTER_FIELD(FString, world_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLevelUpSkill, LevelUpSkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResetSkill, ResetSkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetUsingSkill, SetUsingSkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_1)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_2)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_3)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuySkillPoint, BuySkillPoint)
		REFLECTION_REGISTER_FIELD(TArray<B2BuySkillPointRequestInfoPtr>, buy_infos)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_COMMAND_BEGIN(FB2MessageInfoChatCommand, Chat)
        REFLECTION_REGISTER_FIELD(int32, type)
        REFLECTION_REGISTER_FIELD(FString, to)
        REFLECTION_REGISTER_FIELD(int32, format)
        REFLECTION_REGISTER_FIELD(FString, body)
		REFLECTION_REGISTER_FIELD(int32, character_level_max)
    IMPLEMENT_B2MESSAGEINFO_COMMAND_END(5)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoChatNotify, Chat)
        REFLECTION_REGISTER_FIELD(int32, type)
        REFLECTION_REGISTER_FIELD(FString, from)
        REFLECTION_REGISTER_FIELD(int32, format)
        REFLECTION_REGISTER_FIELD(FString, body)
        REFLECTION_REGISTER_FIELD(B2GuildInfoPtr, guild)
        REFLECTION_REGISTER_FIELD(int32, notice_type)
        REFLECTION_REGISTER_FIELD(TArray<FString>, notice_args)
        REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, character_level_max)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(9)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoAdminMessageNotify, AdminMessage)
        REFLECTION_REGISTER_FIELD(TArray<B2AdminMessagePtr>, messages)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildChat, GuildChat)
		REFLECTION_REGISTER_FIELD(int32, format)
		REFLECTION_REGISTER_FIELD(FString, body)
		REFLECTION_REGISTER_FIELD(int32, character_level_max)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildChat)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildChat, GetGuildChat)
		REFLECTION_REGISTER_FIELD(int32, chat_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildChat)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildChatDataPtr>, chat_list)
		REFLECTION_REGISTER_FIELD(int64, next_ask_support_time)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAskGuildSupport, AskGuildSupport)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAskGuildSupport)
		REFLECTION_REGISTER_FIELD(int64, next_ask_support_time)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildSupport, GuildSupport)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
		REFLECTION_REGISTER_FIELD(int64, chat_id)
		REFLECTION_REGISTER_FIELD(int32, count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildSupport)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildDonation, GuildDonation)
		REFLECTION_REGISTER_FIELD(int32, mercenary_id)
		REFLECTION_REGISTER_FIELD(int32, donation_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildDonation)
		REFLECTION_REGISTER_FIELD(B2LightAccountPtr, light_account)
		REFLECTION_REGISTER_FIELD(B2GuildMemberPtr, guild_member)
		REFLECTION_REGISTER_FIELD(B2GuildMercenaryPtr, mercenary)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoGuildChatNotify, GuildChat)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildChatDataPtr>, chat_list)
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(1)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGiveNickname, GiveNickname)
        REFLECTION_REGISTER_FIELD(FString, nickname)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateNickname, UpdateNickname)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetServerVersion, GetServerVersion)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0);

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGiveCharacterName, GiveCharacterName)
        REFLECTION_REGISTER_FIELD(int32, character_type)
        REFLECTION_REGISTER_FIELD(FString, name)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveActClearReward, ReceiveActClearReward)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, act_id)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetTeamBattleStatus, GetTeamBattleStatus)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAssaultBattleStatus, GetAssaultBattleStatus)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateTeamBattleEntry, UpdateTeamBattleEntry)
		REFLECTION_REGISTER_FIELD(B2TeamBattleEntryPtr, entry)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceTeamBattleFormation, EnhanceTeamBattleFormation)
		REFLECTION_REGISTER_FIELD(int32, formation_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestTeamMatchmaking, TeamMatchmaking)
		REFLECTION_REGISTER_FIELD(bool, use_additional_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestTeamMatchResult, TeamMatchResult)
		REFLECTION_REGISTER_FIELD(FString, match_token)
		REFLECTION_REGISTER_FIELD(int32, match_result)
		REFLECTION_REGISTER_FIELD(bool, is_give_up)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetRank, GetRank)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceRankNode, EnhanceRankNode)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoGetChatChannelRequest, GetChatChannel)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

    IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoChangeChatChannelRequest, ChangeChatChannel)
        REFLECTION_REGISTER_FIELD(int32, channel_number)
    IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMailList, GetMailList)
		REFLECTION_REGISTER_FIELD(int32, mail_category)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestOpenMail, OpenMail)
		REFLECTION_REGISTER_FIELD(int64, mail_id)
		REFLECTION_REGISTER_FIELD(int32, lottery_character_type)
		REFLECTION_REGISTER_FIELD(int32, lottery_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, lottery_aether_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestOpenAllMail, OpenAllMail)
		REFLECTION_REGISTER_FIELD(int32, mail_category)
		REFLECTION_REGISTER_FIELD(int64, start_id)
		REFLECTION_REGISTER_FIELD(int64, end_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetNewMailCount, GetNewMailCount)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAskFriend, AskFriend)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAcceptAskFriend, AcceptAskFriend)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRejectAskFriend, RejectAskFriend)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetFriendList, GetFriendList)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCancelAskFriend, CancelAskFriend)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDeleteFriend, DeleteFriend)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAllSendAskFriend, GetAllSendAskFriend)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAllReceiveAskFriend, GetAllReceiveAskFriend)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSendSocialPoint, SendSocialPoint)
		REFLECTION_REGISTER_FIELD(TArray<int64>, account_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetRecommendFriend, GetRecommendFriend)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFindUser, FindUser)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(bool, guild_invite_state)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestConfirmNewFriend, ConfirmNewFriend)
		REFLECTION_REGISTER_FIELD(bool, is_all_confirm)
		REFLECTION_REGISTER_FIELD(TArray<int64>, friend_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPlatformFriendInviteInfo, PlatformFriendInviteInfo)
		REFLECTION_REGISTER_FIELD(int32, platform_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPlatformFriendInviteCountUp, PlatformFriendInviteCountUp)
		REFLECTION_REGISTER_FIELD(int32, platform_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPlatformFriendJoinerCountUp, PlatformFriendJoinerCountUp)
		REFLECTION_REGISTER_FIELD(int32, platform_type)
		REFLECTION_REGISTER_FIELD(int32, joiner_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPlatformFriendInviteReward, PlatformFriendInviteReward)
		REFLECTION_REGISTER_FIELD(int32, platform_type)
		REFLECTION_REGISTER_FIELD(int32, reward_type)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetQuest, GetQuest)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAcceptQuest, AcceptQuest)
		REFLECTION_REGISTER_FIELD(int32, slot)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveQuestReward, ReceiveQuestReward)
		REFLECTION_REGISTER_FIELD(int32, slot)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetCounterDungeon, GetCounterDungeon)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartCounterDungeon, StartCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFinishCounterDungeon, FinishCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
		REFLECTION_REGISTER_FIELD(FString, play_token)
		REFLECTION_REGISTER_FIELD(bool, is_success)
		REFLECTION_REGISTER_FIELD(bool, is_give_up)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSweepCounterDungeon, SweepCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyCounterDungeonTicket, BuyCounterDungeonTicket)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCounterDungeonWeeklyReward, CounterDungeonWeeklyReward)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetDimension, GetDimension)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartDimension, StartDimension)
		REFLECTION_REGISTER_FIELD(int32, difficulty_level)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, total_power)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFinishDimension, FinishDimension)
		REFLECTION_REGISTER_FIELD(int32, difficulty_level)
		REFLECTION_REGISTER_FIELD(FString, play_token)
		REFLECTION_REGISTER_FIELD(int32, play_result)
		REFLECTION_REGISTER_FIELD(int32, play_time_millis)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCreateGuild, CreateGuild)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, join_option)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAskJoinGuild, AskJoinGuild)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLeaveGuild, LeaveGuild)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildDetailInfo, GetGuildDetailInfo)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildMember, GetGuildMember)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDeleteGuild, DeleteGuild)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetRecommendGuild, GetRecommendGuild)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildMemberCandidates, GetGuildMemberCandidates)
		REFLECTION_REGISTER_FIELD(bool, need_friend_list)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestInviteGuild, InviteGuild)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildInviteList, GetGuildInviteList)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRejectInviteGuild, RejectInviteGuild)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAcceptInviteGuild, AcceptInviteGuild)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAppointSubGuildMaster, AppointSubGuildMaster)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDismissSubGuildMaster, DismissSubGuildMaster)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestKickGuildMember, KickGuildMember)
		REFLECTION_REGISTER_FIELD(int64, account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateGuild, UpdateGuild)
		REFLECTION_REGISTER_FIELD(int32, join_option)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateGuildNotice, UpdateGuildNotice)
		REFLECTION_REGISTER_FIELD(FString, notice)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)
	
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLevelUpGuildSkill, LevelUpGuildSkill)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyGuildSkill, BuyGuildSkill)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMission, GetMission)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveMissionReward, ReceiveMissionReward)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveCompleteMissionReward, ReceiveCompleteMissionReward)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetWing, GetWing)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceWing, EnhanceWing)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpgradeWing, UpgradeWing)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceWingOption, EnhanceWingOption)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, option_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetHeroTower, GetHeroTower)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartHeroTower, StartHeroTower)
		REFLECTION_REGISTER_FIELD(int32, floor)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(FString, play_token)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFinishHeroTower, FinishHeroTower)
		REFLECTION_REGISTER_FIELD(int32, floor)
		REFLECTION_REGISTER_FIELD(FString, play_token)
		REFLECTION_REGISTER_FIELD(int32, clear_time_sec)
		REFLECTION_REGISTER_FIELD(bool, is_success)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAbandonHeroTower, AbandonHeroTower)
		REFLECTION_REGISTER_FIELD(int32, floor)
		REFLECTION_REGISTER_FIELD(FString, play_token)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceRelic, EnhanceRelic)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPromotionRelic, PromotionRelic)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccountRelic, GetAccountRelic)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSweepHeroTower, SweepHeroTower)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyHeroTowerTicket, BuyHeroTowerTicket)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDrawShopLottery, DrawShopLottery)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, draw_price_type)
		REFLECTION_REGISTER_FIELD(bool, continuous_draw) 
		REFLECTION_REGISTER_FIELD(bool, is_draw_again)
		REFLECTION_REGISTER_FIELD(FString, last_lottery_key)
		REFLECTION_REGISTER_FIELD(bool, is_free_lottery)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardShopMileage, RewardShopMileage)
		REFLECTION_REGISTER_FIELD(int32, shop_mileage_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetLotteryShop, GetLotteryShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyShopProduct, BuyShopProduct)
		REFLECTION_REGISTER_FIELD(int32, product_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPurchaseCostume, PurchaseCostume)
		REFLECTION_REGISTER_FIELD(int32, product_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGeneralShop, GetGeneralShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetCostumeShop, GetCostumeShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMagicShop, GetMagicShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRenewalMagicShop, RenewalMagicShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPurchaseMagicShop, PurchaseMagicShop)
		REFLECTION_REGISTER_FIELD(int32, id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetCollectionItem, GetCollectionItem)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardCollectionItem, RewardCollectionItem)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardCollectionSetItem, RewardCollectionSetItem)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, group_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetMarketProduct, GetMarketProduct)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPreparePurchase, PreparePurchase)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestVerifyPurchase, VerifyPurchase)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
		REFLECTION_REGISTER_FIELD(FString, purchase_data)
		REFLECTION_REGISTER_FIELD(FString, purchase_signature)
		REFLECTION_REGISTER_FIELD(bool, is_restore)
		REFLECTION_REGISTER_FIELD(FString, currency_code)
		REFLECTION_REGISTER_FIELD(float, price)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCancelPurchase, CancelPurchase)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
		REFLECTION_REGISTER_FIELD(int32, cancel_reason)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFinishPurchase, FinishPurchase)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRaidResult, RaidResult)
		REFLECTION_REGISTER_FIELD(FString, raid_token)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(bool, is_bonus_player)
		REFLECTION_REGISTER_FIELD(bool, is_reward)
		REFLECTION_REGISTER_FIELD(bool, is_give_up)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDailyAttendance, DailyAttendance)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEventAttendance, EventAttendance)
		REFLECTION_REGISTER_FIELD(int32, eventId)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBeginnerAttendance, BeginnerAttendance)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateDailyPlayTime, UpdateDailyPlayTime)
		REFLECTION_REGISTER_FIELD(int32, play_time_in_sec)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveDailyPlayTimeReward, ReceiveDailyPlayTimeReward)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestJoinGuildBattle, JoinGuildBattle)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetGuildBattle, GetGuildBattle)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartGuildBattle, StartGuildBattle)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestFinishGuildBattle, FinishGuildBattle)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
		REFLECTION_REGISTER_FIELD(FString, battle_token)
		REFLECTION_REGISTER_FIELD(int32, result)
		REFLECTION_REGISTER_FIELD(int32, round_win_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetGuildBattleEntry, SetGuildBattleEntry)
		REFLECTION_REGISTER_FIELD(int32, slot_1)
		REFLECTION_REGISTER_FIELD(int32, slot_2)
		REFLECTION_REGISTER_FIELD(int32, slot_3)
		REFLECTION_REGISTER_FIELD(int32, slot_4)
		REFLECTION_REGISTER_FIELD(int32, slot_5)
		REFLECTION_REGISTER_FIELD(int32, slot_6)
		REFLECTION_REGISTER_FIELD(int32, slot_7)
		REFLECTION_REGISTER_FIELD(int32, slot_8)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(8)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildBattleTurnResult, GuildBattleTurnResult)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildBattleTurnReward, GuildBattleTurnReward)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildBattleSeasonReward, GuildBattleSeasonReward)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGuildBattleHistory, GuildBattleHistory)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetWingVisible, SetWingVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetHelmetVisible, SetHelmetVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetCostumeVisible, SetCostumeVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, costume_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetRaid, GetRaid)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRaidResurrection, RaidResurrection)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int, selected_buff_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)
	
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestChangeQuestId, ChangeQuestId)
		REFLECTION_REGISTER_FIELD(int32, slot_type)
		REFLECTION_REGISTER_FIELD(int32, quest_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestChangeQuestCount, ChangeQuestCount)
		REFLECTION_REGISTER_FIELD(int32, slot_type)
		REFLECTION_REGISTER_FIELD(int32, count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAssaultMatchmaking, AssaultMatchmaking)
		REFLECTION_REGISTER_FIELD(int32, battle_character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCancelAssaultMatchmaking, CancelAssaultMatchmaking)
		REFLECTION_REGISTER_FIELD(FString, token)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAssaultResult, AssaultResult)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, match_result)
		REFLECTION_REGISTER_FIELD(bool, is_mvp_player)
		REFLECTION_REGISTER_FIELD(bool, is_ghost_user)
		REFLECTION_REGISTER_FIELD(B2AssaultBattleInfoPtr, battle_info)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAssaultResurrection, AssaultResurrection)
		REFLECTION_REGISTER_FIELD(FString, token)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckPvPMatchPointCharge, CheckPvPMatchPointCharge)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckTeamMatchPointCharge, CheckTeamMatchPointCharge)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetFriendRankings, GetFriendRankings)
		REFLECTION_REGISTER_FIELD(int32, opponent_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetItemRandomOption, GetItemRandomOption)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGenerateItemRandomOption, GenerateItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, price_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestApplyItemRandomOption, ApplyItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDiscardItemRandomOption, DiscardItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetItemRandomQuality, GetItemRandomQuality)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGenerateItemRandomQuality, GenerateItemRandomQuality)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestApplyItemRandomQuality, ApplyItemRandomQuality)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDiscardItemRandomQuality, DiscardItemRandomQuality)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetFactory, GetFactory)		
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestMakeFactoryItem, MakeFactoryItem)
		REFLECTION_REGISTER_FIELD(int64, factory_id)
		REFLECTION_REGISTER_FIELD(int64, make_count)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_id1)
		REFLECTION_REGISTER_FIELD(int64, material_item_id2)
		REFLECTION_REGISTER_FIELD(int64, material_item_id3)
		REFLECTION_REGISTER_FIELD(int64, material_item_id4)
		REFLECTION_REGISTER_FIELD(int64, main_item_template_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_template_id1)
		REFLECTION_REGISTER_FIELD(int64, material_item_template_id2)
		REFLECTION_REGISTER_FIELD(int64, material_item_template_id3)
		REFLECTION_REGISTER_FIELD(int64, material_item_template_id4)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnsealBox, UnsealBox)
		REFLECTION_REGISTER_FIELD(int64, seal_box_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int64, unseal_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckRedDot, CheckRedDot)
		REFLECTION_REGISTER_FIELD(TArray<int32>, red_dot_hint)
		IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseDarkCrystalPaid, IncreaseDarkCrystalPaid)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal_paid)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestModifyItemRandomOption, ModifyItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, slot_num)
		REFLECTION_REGISTER_FIELD(int32, random_option_id)
		REFLECTION_REGISTER_FIELD(float, random_option_value)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestModifyItemNewFlag, ModifyItemNewFlag)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestModifyCostumeNewFlag, ModifyCostumeNewFlag)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestStartRaid, StartRaid)
		REFLECTION_REGISTER_FIELD(int32, battle_character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

    //////////////////////////////////////////////////////////////////////////
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoSessionClosedNotify, SessionClosed)
        REFLECTION_REGISTER_FIELD(int32, reason)
    IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetContentsOpenStatus, GetContentsOpenStatus)
		REFLECTION_REGISTER_FIELD(int32, link_key)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetDuelRewardBoxInfo, GetDuelRewardBoxInfo)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetDuelRewardBoxGainableList, GetDuelRewardBoxGainableList)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
		REFLECTION_REGISTER_FIELD(int32, grade)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestOpenDuelRewardBox, OpenDuelRewardBox)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveAssaultMvpReward, ReceiveAssaultMvpReward)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccountAether, GetAccountAether)
		REFLECTION_REGISTER_FIELD(int32, aether_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDismantleAethers, DismantleAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestMountAethers, MountAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnmountAethers, UnmountAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSelectAetherDefenseOption, SelectAetherDefenseOption)
		REFLECTION_REGISTER_FIELD(int32, set_option_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSelectAetherUnitySkillOption, SelectAetherUnitySkillOption)
		REFLECTION_REGISTER_FIELD(int32, set_option_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLockAethers, LockAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnlockAethers, UnlockAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestExtendAetherInventory, ExtendAetherInventory)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEnhanceAether, EnhanceAether)
		REFLECTION_REGISTER_FIELD(int64, aether_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSellAethers, SellAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, aether_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestModifyAetherNewFlag, ModifyAetherNewFlag)
		REFLECTION_REGISTER_FIELD(int32, aether_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetTutorialProgress, GetTutorialProgress)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetTutorialProgress, SetTutorialProgress)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(bool, is_item_prepared)
		REFLECTION_REGISTER_FIELD(bool, is_lottery_drawn)
		REFLECTION_REGISTER_FIELD(bool, is_finished)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUpdateTutorialProgress, UpdateTutorialProgress)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSkipTutorialProgress, SkipTutorialProgress)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDrawTutorialItem, DrawTutorialItem)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPrepareTutorial, PrepareTutorial)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetUnitySkillMission, GetUnitySkillMission)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveUnitySkillReward, ReceiveUnitySkillReward)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, unity_character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAwakenUnitySkillMission, AwakenUnitySkillMission)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAwakenUnitySkill, AwakenUnitySkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetDuelModeInfo, GetDuelModeInfo)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetChallengeModeInfo, GetChallengeModeInfo)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetHallOfFame, GetHallOfFame)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(bool, need_user_info)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPraiseTargetRanker, PraiseTargetRanker)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, ranking)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveRestReward, ReceiveRestReward)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(bool, isDouble)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckModeOpen, CheckModeOpen)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGiveTutorialReward, GiveTutorialReward)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckReachPower, CheckReachPower)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerPtr>, character_power)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestTrackingPowerDetail, TrackingPowerDetail)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, item_preset_id)
		REFLECTION_REGISTER_FIELD(int32, skill_slot)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestKakaoMessageProcessing, KakaoMessageProcessing)
		REFLECTION_REGISTER_FIELD(FString, message_box_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestKakaoCouponUse, KakaoCouponUse)
		REFLECTION_REGISTER_FIELD(FString, coupon_code)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestKakaoAccountConversionPrepare, KakaoAccountConversionPrepare)
		REFLECTION_REGISTER_FIELD(FString, idp_code)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestKakaoAccountConversionComplete, KakaoAccountConversionComplete)
		REFLECTION_REGISTER_FIELD(FString, idp_code)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetDonationEvent, GetDonationEvent)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestMakeDonation, MakeDonation)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(bool, is_bundle)
		REFLECTION_REGISTER_FIELD(bool, is_again)
		REFLECTION_REGISTER_FIELD(bool, is_free)
		REFLECTION_REGISTER_FIELD(FString, last_donation_key)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDonationPointReward, DonationPointReward)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, step)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetFairy, GetFairy)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLevelupFairy, LevelupFairy)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBlessFairy, BlessFairy)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
		REFLECTION_REGISTER_FIELD(int32, bless_option_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveFairyGift, ReceiveFairyGift)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestCheckChargePoints, CheckChargePoints)
		REFLECTION_REGISTER_FIELD(bool, check_blade_point)
		REFLECTION_REGISTER_FIELD(bool, check_pvp_match_point)
		REFLECTION_REGISTER_FIELD(bool, check_team_match_point)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetFeaturedMission, GetFeaturedMission)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetEvent, GetEvent)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceivePointEventReward, ReceivePointEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveLevelUpEventReward, ReceiveLevelUpEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetSeasonMission, GetSeasonMission)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardSeasonMission, RewardSeasonMission)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAttendSeasonMission, AttendSeasonMission)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveStageClearEventReward, ReceiveStageClearEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyHotTimeBuff, BuyHotTimeBuff)
		REFLECTION_REGISTER_FIELD(int32, buff_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardFlatRatePackageBonus, RewardFlatRatePackageBonus)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetPackageShop, GetPackageShop)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetPeriodPackageState, GetPeriodPackageState)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardLevelUpPackage, RewardLevelUpPackage)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardReachLevelPackage, RewardReachLevelPackage)
		REFLECTION_REGISTER_FIELD(int32, package_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGiveUpQuest, GiveUpQuest)
		REFLECTION_REGISTER_FIELD(int32, slot)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetContentsModeState, GetContentsModeState)
		REFLECTION_REGISTER_FIELD(int32, mode)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardPointShopEvent, RewardPointShopEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReportUser, ReportUser)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
		REFLECTION_REGISTER_FIELD(int32, mode_type)
		REFLECTION_REGISTER_FIELD(int32, reason_type)
		REFLECTION_REGISTER_FIELD(FString, explain)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBlockChat, BlockChat)
		REFLECTION_REGISTER_FIELD(int64, block_account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnblockChat, UnblockChat)
		REFLECTION_REGISTER_FIELD(int64, unblock_account_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetBlockChatList, GetBlockChatList)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestClientStateReport, ClientStateReport)
		REFLECTION_REGISTER_FIELD(FString, category)
		REFLECTION_REGISTER_FIELD(FString, event)
		REFLECTION_REGISTER_FIELD(FString, label)
		REFLECTION_REGISTER_FIELD(FString, detail_info)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardTenLotteryEvent, RewardTenLotteryEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestReceiveGemPurchaseEventReward, ReceiveGemPurchaseEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardSpendGemEvent, RewardSpendGemEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardTenLotterySavingsEvent, RewardTenLotterySavingsEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRollDiceEvent, RollDiceEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(bool, is_free_dice)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestBuyDiceEventPoint, BuyDiceEventPoint)
		REFLECTION_REGISTER_FIELD(int32, event_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRewardDiceEventMileage, RewardDiceEventMileage)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	// REPLACEMENT POINT: MESSAGE_INFO CPP REQUEST
	// NEVER DELETE LINE ABOVE
	
	// for console command

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDeleteAccount, DeleteAccount)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAddItemToInventory, AddItemToInventory)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, item_quality)
		REFLECTION_REGISTER_FIELD(int32, item_amount)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAddCostumeToInventory, AddCostumeToInventory)
		REFLECTION_REGISTER_FIELD(int32, template_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAddTotemToInventory, AddTotemToInventory)
		REFLECTION_REGISTER_FIELD(int32, template_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseCashPaid, IncreaseCashPaid)
		REFLECTION_REGISTER_FIELD(int32, cash_paid)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseMoney, IncreaseMoney)
		REFLECTION_REGISTER_FIELD(int32, money)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseSocialPoint, IncreaseSocialPoint)
		REFLECTION_REGISTER_FIELD(int32, social_point)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetItemLevelup, SetItemLevelup)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(float, exp)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetCharacterLevelExp, SetCharacterLevelExp)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(float, exp)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetCharacterRankInfo, SetCharacterRankInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(int32, nodeIndex)
		REFLECTION_REGISTER_FIELD(bool, nodeState)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetCharacterRelic, SetCharacterRelic)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetBladePoint, SetBladePoint)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseRankPromotionPoint, IncreaseRankPromotionPoint)
		REFLECTION_REGISTER_FIELD(int32, rank_promotion_point)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAddMail, AddMail)
		REFLECTION_REGISTER_FIELD(int32, mail_category)
		REFLECTION_REGISTER_FIELD(int32, attach_type)
		REFLECTION_REGISTER_FIELD(int32, source_type)
		REFLECTION_REGISTER_FIELD(int32, sender_type)
		REFLECTION_REGISTER_FIELD(int32, amount)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, general_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, fixed_grade_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, selective_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, selective_item_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, prefix_selective_lottery_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(11)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestMakeRanking, MakeRanking)
		REFLECTION_REGISTER_FIELD(int32, match_type)
		REFLECTION_REGISTER_FIELD(int32, ranking)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestForceClearStage, ForceClearStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetQuestState, SetQuestState)
		REFLECTION_REGISTER_FIELD(int32, quest_id)
		REFLECTION_REGISTER_FIELD(int32, quest_state)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestIncreaseHeroPiece, IncreaseHeroPiece)
		REFLECTION_REGISTER_FIELD(int32, hero_piece)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestForceClearHeroTower, ForceClearHeroTower)
		REFLECTION_REGISTER_FIELD(int32, floor)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestForceStartCounterDungeon, ForceStartCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPrepareSweepHeroTower, PrepareSweepHeroTower)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPrepareGuild, PrepareGuild)
		REFLECTION_REGISTER_FIELD(int32, member_count)
		REFLECTION_REGISTER_FIELD(int32, add_guild_count)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetGuildBattleState, SetGuildBattleState)
		REFLECTION_REGISTER_FIELD(bool, is_new_season)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResetAttendanceTime, ResetAttendanceTime)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResetEventAttendance, ResetEventAttendance)
		REFLECTION_REGISTER_FIELD(bool, is_only_attendance_time)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResetDailyPlayTimeRecord, ResetDailyPlayTimeRecord)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestResetTeamMatchPoint, ResetTeamMatchPoint)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestPrepareFriend, PrepareFriend)
		REFLECTION_REGISTER_FIELD(int32, friend_count)
		REFLECTION_REGISTER_FIELD(int32, job_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetGuildLevelExp, SetGuildLevelExp)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestAddAetherToInventory, AddAetherToInventory)
		REFLECTION_REGISTER_FIELD(int32, aether_type)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, tier_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestForceUpdateAether, ForceUpdateAether)
		REFLECTION_REGISTER_FIELD(int64, aether_id)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, grade)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetMatchSeasonReward, SetMatchSeasonReward)
		REFLECTION_REGISTER_FIELD(int32, match_type)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int32, point)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetStageBoostCount, SetStageBoostCount)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSkipMatchTutorial, SkipMatchTutorial)
		REFLECTION_REGISTER_FIELD(int32, match_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSetDebugMode, SetDebugMode)
		REFLECTION_REGISTER_FIELD(bool, skip_keep_alive_check)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestServerCmd, ServerCmd)
		REFLECTION_REGISTER_FIELD(FString, command)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestTrace, Trace)
		REFLECTION_REGISTER_FIELD(FString, tag)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccount)
		REFLECTION_REGISTER_FIELD(B2AccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterServerInfoPtr>, characters)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, equips)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, equip_costumes)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, equip_totems)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemPresetPtr>, item_presets)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(int32, my_rank)
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, relics)
		REFLECTION_REGISTER_FIELD(TArray<B2WingOptionInfoPtr>, wing_options)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2EventAttendanceStatusPtr>, event_attendance_statuses)
		REFLECTION_REGISTER_FIELD(int32, aether_inventory_slot_count)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, mount_aethers)
		REFLECTION_REGISTER_FIELD(TArray<B2FairyStatusPtr>, fairies)
		REFLECTION_REGISTER_FIELD(bool, is_all_tutorial_finished)
		REFLECTION_REGISTER_FIELD(TArray<B2UnitySkillMissionPtr>, unitySkills)
		REFLECTION_REGISTER_FIELD(TArray<B2RollingBannerPtr>, rolling_banners)
		REFLECTION_REGISTER_FIELD(B2BeginnerAttendanceStatusPtr, beginner_attendance_status)
		REFLECTION_REGISTER_FIELD(TArray<B2UnitySkillAwakenMissionPtr>, unity_awaken_missions)
	REFLECTION_REGISTER_END(24)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFindAccount)
		REFLECTION_REGISTER_FIELD(TArray<B2FindAccountInfoPtr>, account_infos)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccountItem)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccountCostume)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, costumes)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMasterData)
		REFLECTION_REGISTER_FIELD(B2ConfigDataPtr, config_data)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSkillInfoPtr>, md_skill_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSkillOptionPtr>, md_skill_options)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemLevelupExpFactorPtr>, md_item_levelup_exp_factors)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemOptionAddValuePtr>, md_item_option_add_values)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemLevelupCostPtr>, md_item_levelup_costs)
		REFLECTION_REGISTER_FIELD(TArray<B2mdStageInfoPtr>, md_stage_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdResurrectionInfoPtr>, md_resurrection_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTeamBattleFormationInfoPtr>, md_formation_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRankInfoPtr>, md_rank_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDailyMatchRewardPtr>, md_daily_match_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMatchRankingRewardPtr>, md_pvp_ranking_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMatchRankingRewardPtr>, md_team_ranking_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCounterDungeonPtr>, md_counter_dungeons)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCounterDungeonPeriodPtr>, md_counter_dungeon_periods)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCounterDungeonWeekRewardPtr>, md_counter_dungeon_week_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMainQuestInfoPtr>, md_main_quest_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDailyQuestInfoPtr>, md_daily_quest_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRewardInfoPtr>, md_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMissionInfoPtr>, md_mission_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSerialMissionInfoPtr>, md_serial_mission_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMissionCompletePointInfoPtr>, md_mission_complete_point_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWingEnhancePtr>, md_wing_enhance_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWingUpgradePtr>, md_wing_upgrade_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWingOptionValuePtr>, md_wing_option_value_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWingOptionOpenPtr>, md_wing_option_open_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWingOptionMaxLevelPtr>, md_wing_option_max_level_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdHeroTowerFloorPtr>, md_hero_tower_floors)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRelicInfoPtr>, md_relic_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRelicGradeInfoPtr>, md_relic_grade_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRelicLevelInfoPtr>, md_relic_level_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLotteryPricePtr>, md_shop_lottery_prices)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLotteryPossibleGradePtr>, md_shop_lottery_possible_grades)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLotterySpecialGradePtr>, md_shop_lottery_special_grades)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopProductPtr>, md_shop_products)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopPackagePtr>, md_shop_packages)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopMileageRewardPtr>, md_shop_mileage_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLotteryDailyLimitPtr>, md_shop_lottery_daily_limits)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCollectionRewardPtr>, md_collection_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDailyAttendanceInfoPtr>, md_daily_atttendances)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildBattleResultRewardPtr>, md_guild_battle_result_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildBattleSeasonRewardPtr>, md_guild_battle_season_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRaidInfoPtr>, md_raid_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMonsterInfoPtr>, md_monster_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGeneralLotteryPtr>, md_general_lotteries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSelectiveLotteryPtr>, md_selective_lotteries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFixedGradeLotteryPtr>, md_fixed_grade_lotteries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemEnhanceCostPtr>, md_item_enhance_costs)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemEnhanceEffectInfoPtr>, md_item_enhance_effect_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAdditionalMatchCostPtr>, md_additional_match_cost_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildSkillPtr>, md_guild_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildExpPtr>, md_guild_exps)
		REFLECTION_REGISTER_FIELD(TArray<B2mdStageFixedDropPtr>, md_stage_fixed_drops)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAssaultVictoryRewardInfoPtr>, md_assault_victory_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAssaultMvpRewardInfoPtr>, md_assault_mvp_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdEventAttendanceRewardPtr>, md_event_attendance_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDailyPlayTimeRewardPtr>, md_daily_play_time_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMatchRankingRewardPtr>, md_assault_ranking_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTutorialInfoPtr>, md_tutorial_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDuelModeRewardPtr>, md_duel_mode_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRaidExpPtr>, md_raid_exp)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAetherOptionInfoPtr>, md_aether_option_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAetherSlotInfoPtr>, md_aether_slot_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAetherEnhanceInfoPtr>, md_aether_enhance_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAetherEnhanceOptionInfoPtr>, md_aether_enhance_option_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdAetherSetOptionInfoPtr>, md_aether_set_option_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildSupportItemPtr>, md_guild_support_items)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildBattleScheduleInfoPtr>, md_guild_battle_schedule_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdUnitySkillMissionPtr>, md_unity_skill_missions)
		REFLECTION_REGISTER_FIELD(TArray<B2mdRestRewardPtr>, md_rest_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildMercenaryPtr>, md_guild_mercenaries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildMercenarySkillPtr>, md_guild_mercenary_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildMercenaryMasteryPtr>, md_guild_mercenary_masteries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGuildMercenaryDonationPtr>, md_guild_mercenary_donations)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemInfoPtr>, md_item_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemSetPtr>, md_item_set_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdPointEventPtr>, md_point_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdPointEventRewardPtr>, md_point_event_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdLevelUpEventPtr>, md_level_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSeasonMissionEventPtr>, md_season_mission_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdKakaoFriendInviteRewardPtr>, md_kakao_friend_invite_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdHotTimeProductPtr>, md_hot_time_products)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopCashBonusPtr>, md_shop_cash_bonus_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopFundsSalePtr>, md_shop_funds_sale_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLotterySalePtr>, md_shop_lottery_sale_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopPackageProductPtr>, md_shop_package_products)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSkillLevelUpPtr>, md_skill_levelups)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDuelModeSettleSchedulePtr>, md_duel_mode_settle_schedules)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopLevelUpPackagePtr>, md_shop_level_up_packages)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopReachLevelPackagePtr>, md_shop_reach_level_packages)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopFlatRatePackageBonusPtr>, md_shop_flat_rate_package_bonus)
		REFLECTION_REGISTER_FIELD(TArray<B2mdShopPackageTabPtr>, md_shop_package_tab_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdUnitySkillOptionPtr>, md_unity_skill_options)
		REFLECTION_REGISTER_FIELD(TArray<B2mdStageClearEventPtr>, md_stage_clear_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdWebUrlPtr>, md_web_urls)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTenLotteryEventPtr>, md_ten_lottery_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMagicShopProductPtr>, md_magic_shop_product)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMagicShopRenewalCostPtr>, md_magic_shop_renewal_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2mdGemPurchaseEventRewardPtr>, md_gem_purchase_event_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSpendGemEventPtr>, md_spend_gem_event_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdEventAttendancePtr>, md_event_attendances)
		REFLECTION_REGISTER_FIELD(TArray<B2mdEventInfoPtr>, md_event_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTenLotterySavingsEventPtr>, md_ten_lottery_savings_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemLevelupFactorPtr>, md_item_levelup_factors)
		REFLECTION_REGISTER_FIELD(TArray<B2mdPrefixSelectiveLotteryPtr>, md_prefix_selective_lotteries)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSelectItemOptionPtr>, md_select_item_option)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDiceEventMissionPtr>, md_dice_event_missions)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDiceEventCellPtr>, md_dice_event_cells)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDiceEventFinishRewardPtr>, md_dice_event_finish_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemSurpassLevelPtr>, md_item_surpass_levels)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDiceEventBuyPointPtr>, md_dice_event_buy_points)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDiceEventMileagePtr>, md_dice_event_mileages)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemDismantleInfoPtr>, md_item_dismantle_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFairyInfoPtr>, md_fairy_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFairyLevelupCostPtr>, md_fairy_levelup_costs)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFairyOpenInfoPtr>, md_fairy_open_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCostumeInfoPtr>, md_costume_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDonationPointRankingRewardInfoPtr>, md_donation_point_ranking_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDonationBundleFixedRewardInfoPtr>, md_donation_bundle_fixed_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDonationPointStepRewardInfoPtr>, md_donation_point_step_reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdUnitySkillAwakenMissionPtr>, md_unity_skill_awaken_missions)
		REFLECTION_REGISTER_FIELD(B2mdExtendMessagePtr, md_extend_massages)
		//REFLECTION_REGISTER_FIELD(TArray<B2mdExchangeSystemInfoPtr>, md_exchange_system_info)
	REFLECTION_REGISTER_END(123)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSignUp)

	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetLatestEventData)
		REFLECTION_REGISTER_FIELD(int64, last_update_time)
		REFLECTION_REGISTER_FIELD(TArray<B2mdEventInfoPtr>, md_event_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdEventAttendancePtr>, md_event_attendances)
		REFLECTION_REGISTER_FIELD(TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDonationEventInfoPtr>, md_donation_event_infos)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSellItem)
		REFLECTION_REGISTER_FIELD(int32, money_total)
		REFLECTION_REGISTER_FIELD(TArray<int64>, sell_item_ids)
		REFLECTION_REGISTER_FIELD(int32, sell_price)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSellConsumableItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, updated_item)
		REFLECTION_REGISTER_FIELD(int64, deleted_item_id)
		REFLECTION_REGISTER_FIELD(int32, sell_price)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseConvertData)
		REFLECTION_REGISTER_FIELD(int32, templateId)
		REFLECTION_REGISTER_FIELD(int32, exchange_remain_count_gold)
		REFLECTION_REGISTER_FIELD(int32, exchange_remain_count_gem)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseConvertItem)
		REFLECTION_REGISTER_FIELD(b2::protocol::commondata::ExchangePriceType, exchangetype)
		REFLECTION_REGISTER_FIELD(int32, currentGold)
		REFLECTION_REGISTER_FIELD(int32, currentCash)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, exchangeItem)
		REFLECTION_REGISTER_FIELD(int32, exchangeItemAmount)
		REFLECTION_REGISTER_FIELD(int32, costCash)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEquipItem)
		REFLECTION_REGISTER_FIELD(int32, preset_id)
		REFLECTION_REGISTER_FIELD(TArray<int64>, equiped_item_ids)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_item_ids)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnequipItem)
			REFLECTION_REGISTER_FIELD(int32, preset_id)
			REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_item_ids)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEquipCostume)
		REFLECTION_REGISTER_FIELD(TArray<int64>, equiped_costume_ids)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_costume_ids)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnequipCostume)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_costume_ids)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceCostume)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(B2CostumeServerInfoPtr, enhanced_costume)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_material_item_ids)
		REFLECTION_REGISTER_FIELD(int32, current_enhance_level)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDismantleCostumes)
		REFLECTION_REGISTER_FIELD(TArray<int64>, dismantled_costume_ids)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, extracted_items)
		REFLECTION_REGISTER_FIELD(TArray<int32>, extracted_item_change_amount)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, restore_item)
		REFLECTION_REGISTER_FIELD(int32, restore_items_change_amount)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLockCostumes)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, locked_costumes)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnlockCostumes)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, unlocked_costumes)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLockItem)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, locked_items)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnlockItem)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, unlocked_items)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseExtendInventory)
		REFLECTION_REGISTER_FIELD(B2CharacterServerInfoPtr, character)
		REFLECTION_REGISTER_FIELD(int32, cash)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLevelupItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, main_item)
		REFLECTION_REGISTER_FIELD(TArray<int64>, item_ids)
		REFLECTION_REGISTER_FIELD(bool, jackpoted)
		REFLECTION_REGISTER_FIELD(bool, great_jackpoted)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, enhanced_item)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_enhance_scroll_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_enhance_scroll_item_ids)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_protection_scroll_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_protection_scroll_item_ids)
		REFLECTION_REGISTER_FIELD(int32, changed_enhance_level)
		REFLECTION_REGISTER_FIELD(bool, is_success)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDismantleItems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, dismantled_item_ids)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, extracted_items)
		REFLECTION_REGISTER_FIELD(TArray<int32>, extracted_item_change_amount)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseExchangeItemEnhanceLevel)
		REFLECTION_REGISTER_FIELD(int32, current_dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item_data_1)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item_data_2)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseOpenSealSlotItem)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, slot_opened_item)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCarveSealItem)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, sealed_item)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, updated_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_material_item_ids)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSurpassItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, main_item)
		REFLECTION_REGISTER_FIELD(int64, material_item_id)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpgradeItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, main_item)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, upgrade_stone_items)
		REFLECTION_REGISTER_FIELD(int32, use_upgrade_stone_count)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseComposeItem)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, new_composed_item)
		REFLECTION_REGISTER_FIELD(int64, deleted_main_item_id)
		REFLECTION_REGISTER_FIELD(int64, deleted_sub_item_id)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseChangeItemCharacterType)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, updated_item)
		REFLECTION_REGISTER_FIELD(int32, current_dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal_cost)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetChangeItemCharacterTypeCost)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, change_items)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartStage)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, drop_gold)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, drop_items)
		REFLECTION_REGISTER_FIELD(int32, drop_gold_by_gold_monster)
		REFLECTION_REGISTER_FIELD(int32, max_resurrection_count)
		REFLECTION_REGISTER_FIELD(int32, need_cash_for_resurrection)
		REFLECTION_REGISTER_FIELD(B2AccountInfoPtr, account);
		REFLECTION_REGISTER_FIELD(int32, selected_option_flags);
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, drop_datas);
		REFLECTION_REGISTER_FIELD(TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
		REFLECTION_REGISTER_FIELD(int32, quest_item_id);
		REFLECTION_REGISTER_FIELD(int32, quest_item_count);
		REFLECTION_REGISTER_FIELD(int32, current_aether_count);
		REFLECTION_REGISTER_FIELD(int32, current_aether_max_count);
		REFLECTION_REGISTER_FIELD(int32, qte_bonus_attack);
		REFLECTION_REGISTER_FIELD(int32, qte_bonus_finish);
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket);
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills);
		REFLECTION_REGISTER_FIELD(int32, gold_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, exp_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_exp_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_exp_rate)
	REFLECTION_REGISTER_END(24)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseClearStage)
		REFLECTION_REGISTER_FIELD(B2AccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterServerInfoPtr>, characters)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(int32, clear_grade)
		REFLECTION_REGISTER_FIELD(int32, total_added_drop_gold)
		REFLECTION_REGISTER_FIELD(int32, total_added_reward_gold)
		REFLECTION_REGISTER_FIELD(int32, total_added_qte_gold)
		REFLECTION_REGISTER_FIELD(int32, reward_exp)
		REFLECTION_REGISTER_FIELD(int32, quest_item_id);
		REFLECTION_REGISTER_FIELD(int32, quest_item_count);
		REFLECTION_REGISTER_FIELD(bool, is_level_up_main_character);
		REFLECTION_REGISTER_FIELD(bool, is_level_up_tag_character);
		REFLECTION_REGISTER_FIELD(int32, clear_mask);
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, aethers)
		REFLECTION_REGISTER_FIELD(TArray<int32>, reward_golds);
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, fixed_items);
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, update_items)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, update_aethers)
		REFLECTION_REGISTER_FIELD(int32, multiple_value);
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, created_relics)
		REFLECTION_REGISTER_FIELD(int32, total_added_gold)
		REFLECTION_REGISTER_FIELD(int32, gold_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, exp_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_exp_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_exp_rate)
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, material_items);
		REFLECTION_REGISTER_FIELD(TArray<B2SoldItemInfoPtr>, sold_items);
		REFLECTION_REGISTER_FIELD(TArray<B2SoldAetherInfoPtr>, sold_aethers);
		REFLECTION_REGISTER_FIELD(int32, total_added_sales_gold)
	REFLECTION_REGISTER_END(31)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSweepStage)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, repeat_count)
		REFLECTION_REGISTER_FIELD(int32, stage_sweep_ticket)
		REFLECTION_REGISTER_FIELD(B2AccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterServerInfoPtr>, characters)
		REFLECTION_REGISTER_FIELD(bool, is_level_up_main_character)
		REFLECTION_REGISTER_FIELD(bool, is_level_up_tag_character)
		REFLECTION_REGISTER_FIELD(int32, reward_exp)
		REFLECTION_REGISTER_FIELD(int32, total_added_drop_gold)
		REFLECTION_REGISTER_FIELD(int32, total_added_reward_gold)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, aethers)
		REFLECTION_REGISTER_FIELD(TArray<int32>, reward_golds)
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, material_items)
		REFLECTION_REGISTER_FIELD(int32, quest_item_id)
		REFLECTION_REGISTER_FIELD(int32, quest_item_count)
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, fixed_items)
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, created_relics)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, update_items)
		REFLECTION_REGISTER_FIELD(int32, total_added_gold)
		REFLECTION_REGISTER_FIELD(int32, gold_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, exp_buff_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, equip_item_exp_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_gold_rate)
		REFLECTION_REGISTER_FIELD(int32, item_set_exp_rate)
		REFLECTION_REGISTER_FIELD(TArray<B2InventoryCountServerInfoPtr>, not_enough_inventories)
		REFLECTION_REGISTER_FIELD(int32, current_aether_count)
		REFLECTION_REGISTER_FIELD(int32, current_aether_max_count)
	REFLECTION_REGISTER_END(29)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFailStage)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResurrection)
		REFLECTION_REGISTER_FIELD(int32, max_resurrection_count)
		REFLECTION_REGISTER_FIELD(int32, resurrection_count)
		REFLECTION_REGISTER_FIELD(int32, used_cash)
		REFLECTION_REGISTER_FIELD(int32, total_cash)
		REFLECTION_REGISTER_FIELD(int32, next_resurrection_gem_cost)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff1)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff2)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff3)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckBladePointCharge)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
		REFLECTION_REGISTER_FIELD(int32, blade_point_max)
		REFLECTION_REGISTER_FIELD(int32, blade_point_charge_lefttime_second)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetItemSurpassCost)
		REFLECTION_REGISTER_FIELD(int32, need_gold)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetItemUpgradeCost)
		REFLECTION_REGISTER_FIELD(int32, need_gold)
		REFLECTION_REGISTER_FIELD(int32, need_upgrade_stone_template_id)
		REFLECTION_REGISTER_FIELD(int32, need_upgrade_stone_count)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, next_item)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetItemComposeCost)
		REFLECTION_REGISTER_FIELD(int32, need_gold)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetActInfo)
		REFLECTION_REGISTER_FIELD(TArray<B2StageClearInfoPtr>, stage_clear_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2ActRewardInfoPtr>, reward_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2ActIdentifierPtr>, has_reward_acts)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetStageInfo)
		REFLECTION_REGISTER_FIELD(TArray<B2StageClearRewardItemInfoPtr>, reward_item_infos)
		REFLECTION_REGISTER_FIELD(int32, auto_repeat_battle_cost)
		REFLECTION_REGISTER_FIELD(int32, need_blade_point)
		REFLECTION_REGISTER_FIELD(bool, enable_auto_repeat_battle)
		REFLECTION_REGISTER_FIELD(int32, left_buff_ticket_exp)
		REFLECTION_REGISTER_FIELD(int32, left_buff_ticket_gold)
		REFLECTION_REGISTER_FIELD(int32, left_buff_ticket_autoskill)
		REFLECTION_REGISTER_FIELD(int32, left_buff_ticket_boost)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)
		REFLECTION_REGISTER_FIELD(int32, best_clear_mask)
		REFLECTION_REGISTER_FIELD(TArray<B2StageClearRewardAetherInfoPtr>, reward_aether_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, material_items);
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMatchStatus)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
		REFLECTION_REGISTER_FIELD(int32, from_ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, total_power)
		REFLECTION_REGISTER_FIELD(B2RankingRewardPtr, ranking_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePtr>, hall_of_fames)
		REFLECTION_REGISTER_FIELD(int64, settlement_time)
		REFLECTION_REGISTER_FIELD(int32, settlement_period_hour)
		REFLECTION_REGISTER_FIELD(bool, today_reward)
		REFLECTION_REGISTER_FIELD(bool, last_reward)
		REFLECTION_REGISTER_FIELD(int32, today_participation_count)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, current_bladepoint)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(int32, additional_match_count)
		REFLECTION_REGISTER_FIELD(int32, match_point_next_charge_second)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
	REFLECTION_REGISTER_END(22)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoMatchmakingResponse)
        REFLECTION_REGISTER_FIELD(B2MatchInfoPtr, match_info)
        REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, addadditional_match_count)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(B2MatcherPtr, ai_opponent)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
	REFLECTION_REGISTER_END(8)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoRaidMatchmakingResponse)
		REFLECTION_REGISTER_FIELD(int32, req_type)
		REFLECTION_REGISTER_FIELD(FString, raid_token)
		REFLECTION_REGISTER_FIELD(int32, room_type)
		REFLECTION_REGISTER_FIELD(int32, room_access)
		REFLECTION_REGISTER_FIELD(B2EndpointPtr, channel_endpoint)
		REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
    REFLECTION_REGISTER_END(8)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoCancelMatchmakingResponse)
    REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoMatchResultResponse)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(B2MatchRankerPtr, prev_ranker)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
	REFLECTION_REGISTER_END(5)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoInvitationResponse)
        REFLECTION_REGISTER_FIELD(FString, invitee)
        REFLECTION_REGISTER_FIELD(FString, invitation_ref)
        REFLECTION_REGISTER_FIELD(B2RoomLocationPtr, room_location)
    REFLECTION_REGISTER_END(3)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoAcceptInvitationResponse)
    REFLECTION_REGISTER_END(0)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoDeclineInvitationResponse)
    REFLECTION_REGISTER_END(0)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoRoomJoinResponse)
        REFLECTION_REGISTER_FIELD(FString, room_id)
        REFLECTION_REGISTER_FIELD(int32, room_type)
        REFLECTION_REGISTER_FIELD(B2RoomParticipantPtr, host)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomParticipantPtr>, participants)
        REFLECTION_REGISTER_FIELD(TArray<B2RoomPropertyPtr>, properties)
    REFLECTION_REGISTER_END(5)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoRoomLeaveResponse)
        REFLECTION_REGISTER_FIELD(TArray<FString>, room_id_list)
    REFLECTION_REGISTER_END(1)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(FB2MessageInfoRoomKickResponse)
        REFLECTION_REGISTER_FIELD(int64, kicked_account_id)
    REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLevelUpSkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, left_skill_point)
		REFLECTION_REGISTER_FIELD(B2SkillServerInfoPtr, skill)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResetSkill)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, left_skill_point)
		REFLECTION_REGISTER_FIELD(TArray<B2SkillServerInfoPtr>, skills)
		REFLECTION_REGISTER_FIELD(TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetUsingSkill)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(B2SkillQuickSlotPtr, skill_quick_slot)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuySkillPoint)
		REFLECTION_REGISTER_FIELD(int32, left_gem)
		REFLECTION_REGISTER_FIELD(TArray<B2BuySkillPointResponseInfoPtr>, buy_results)
	REFLECTION_REGISTER_END(2)

    //////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGiveNickname)
    REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateNickname)
		REFLECTION_REGISTER_FIELD(FString, old_nickname)
		REFLECTION_REGISTER_FIELD(FString, new_nickname)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, updated_item)
		REFLECTION_REGISTER_FIELD(int64, deleted_item_id)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetServerVersion)
		REFLECTION_REGISTER_FIELD(FString, version)
		REFLECTION_REGISTER_FIELD(FString, stage)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveActRepeatClearReward)
		REFLECTION_REGISTER_FIELD(int32, clear_count)
		REFLECTION_REGISTER_FIELD(int32, next_require_clear_count)
		REFLECTION_REGISTER_FIELD(int32, next_reward_gold)
		REFLECTION_REGISTER_FIELD(int32, rewarded_gold)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveActClearReward)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
		REFLECTION_REGISTER_FIELD(int32, reward_gold)
		REFLECTION_REGISTER_FIELD(int32, reward_gem)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetTeamBattleStatus)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
		REFLECTION_REGISTER_FIELD(int32, from_ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(TArray<B2BattleFormationPtr>, formations)
		REFLECTION_REGISTER_FIELD(B2TeamBattleEntryPtr, entry)
		REFLECTION_REGISTER_FIELD(B2RankingRewardPtr, ranking_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePtr>, hall_of_fames)
		REFLECTION_REGISTER_FIELD(int64, settlement_time)
		REFLECTION_REGISTER_FIELD(int32, settlement_period_hour)
		REFLECTION_REGISTER_FIELD(bool, today_reward)
		REFLECTION_REGISTER_FIELD(bool, last_reward)
		REFLECTION_REGISTER_FIELD(int32, today_participation_count)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, current_bladepoint)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(int32, additional_match_count)
		REFLECTION_REGISTER_FIELD(int32, match_point_next_charge_second)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
	REFLECTION_REGISTER_END(21)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAssaultBattleStatus)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
		REFLECTION_REGISTER_FIELD(int32, from_ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(int32, battle_character_type)
		REFLECTION_REGISTER_FIELD(int32, total_power)
		REFLECTION_REGISTER_FIELD(B2RankingRewardPtr, ranking_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePtr>, hall_of_fames)
		REFLECTION_REGISTER_FIELD(int64, settlement_time)
		REFLECTION_REGISTER_FIELD(int32, settlement_period_hour)
		REFLECTION_REGISTER_FIELD(bool, today_reward)
		REFLECTION_REGISTER_FIELD(bool, last_reward)
		REFLECTION_REGISTER_FIELD(int32, today_participation_count)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, current_bladepoint)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
		REFLECTION_REGISTER_FIELD(int32, mvp_point)
	REFLECTION_REGISTER_END(19)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateTeamBattleEntry)
		REFLECTION_REGISTER_FIELD(B2TeamBattleEntryPtr, entry)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceTeamBattleFormation)
		REFLECTION_REGISTER_FIELD(B2BattleFormationPtr, formation)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseTeamMatchmaking)
		REFLECTION_REGISTER_FIELD(FString, match_token)
		REFLECTION_REGISTER_FIELD(B2TeamPtr, opponent_team)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, addadditional_match_count)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseTeamMatchResult)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(B2MatchRankerPtr, prev_ranker)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetRank)
		REFLECTION_REGISTER_FIELD(TArray<B2RankInfoPtr>, rankInfos)	
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceRankNode)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(int32, rank_node_index)
		REFLECTION_REGISTER_FIELD(bool, rank_node_state)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_rank_promotion_points)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_rank_promotion_point_ids)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(bool, is_success)
	REFLECTION_REGISTER_END(8)

    REFLECTION_REGISTER_BEGIN(FB2MessageInfoGetChatChannelResponse)
        REFLECTION_REGISTER_FIELD(int32, channel_number)
    REFLECTION_REGISTER_END(1)

    REFLECTION_REGISTER_BEGIN(FB2MessageInfoChangeChatChannelResponse)
    REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMailList)
		REFLECTION_REGISTER_FIELD(TArray<B2MailInfoPtr>, mail_infos)
		REFLECTION_REGISTER_FIELD(int32, new_present_mail_count)
		REFLECTION_REGISTER_FIELD(int32, new_bladepoint_mail_count)
		REFLECTION_REGISTER_FIELD(int32, new_socialpoint_mail_count)
		REFLECTION_REGISTER_FIELD(int32, new_lottery_mail_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseOpenMail)		
		REFLECTION_REGISTER_FIELD(int64, mail_id);
		REFLECTION_REGISTER_FIELD(B2MailItemInfoPtr, mail_item_info);
		REFLECTION_REGISTER_FIELD(int32, gold);
		REFLECTION_REGISTER_FIELD(int32, gem);
		REFLECTION_REGISTER_FIELD(int32, blade_point);
		REFLECTION_REGISTER_FIELD(int32, social_point);
		REFLECTION_REGISTER_FIELD(int32, dark_crystal);
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket);
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket);
		REFLECTION_REGISTER_FIELD(int32, hot_time_gold_buff_ticket);
		REFLECTION_REGISTER_FIELD(int32, hot_time_exp_buff_ticket);
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket);
		REFLECTION_REGISTER_FIELD(int32, fame_point);
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, aether);
		REFLECTION_REGISTER_FIELD(B2TotemServerInfoPtr, totem);
		REFLECTION_REGISTER_FIELD(int32, exceed_inven_character_type);
		REFLECTION_REGISTER_FIELD(int32, current_aether_count);
		REFLECTION_REGISTER_FIELD(int32, aether_inventory_max_count);
	REFLECTION_REGISTER_END(18)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseOpenAllMail)
		REFLECTION_REGISTER_FIELD(TArray<int64>, mail_ids);
		REFLECTION_REGISTER_FIELD(int32, gold);
		REFLECTION_REGISTER_FIELD(int32, gem);
		REFLECTION_REGISTER_FIELD(int32, blade_point);
		REFLECTION_REGISTER_FIELD(int32, social_point);
		REFLECTION_REGISTER_FIELD(TArray<B2MailItemInfoPtr>, item_infos);
		REFLECTION_REGISTER_FIELD(int32, dark_crystal);
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket);
		REFLECTION_REGISTER_FIELD(int32, hot_time_gold_buff_ticket);
		REFLECTION_REGISTER_FIELD(int32, hot_time_exp_buff_ticket);
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket);
		REFLECTION_REGISTER_FIELD(int32, fame_point);
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket);
	REFLECTION_REGISTER_END(13)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetNewMailCount)
		REFLECTION_REGISTER_FIELD(int32, new_mail_all);
		REFLECTION_REGISTER_FIELD(int32, new_present_mail_count);
		REFLECTION_REGISTER_FIELD(int32, new_bladepoint_mail_count);
		REFLECTION_REGISTER_FIELD(int32, new_socialpoint_mail_count);
		REFLECTION_REGISTER_FIELD(int32, new_lottery_mail_count);
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAskFriend)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAcceptAskFriend)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRejectAskFriend)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetFriendList)
		REFLECTION_REGISTER_FIELD(TArray<B2FriendPtr>, friends);
		REFLECTION_REGISTER_FIELD(int32, left_delete_friend_count);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCancelAskFriend)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDeleteFriend)
		REFLECTION_REGISTER_FIELD(int32, left_delete_friend_count)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAllSendAskFriend)
		REFLECTION_REGISTER_FIELD(TArray<B2UserPtr>, acceptors);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAllReceiveAskFriend)
		REFLECTION_REGISTER_FIELD(TArray<B2UserPtr>, requestors);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSendSocialPoint)
		REFLECTION_REGISTER_FIELD(int32, current_social_point);
		REFLECTION_REGISTER_FIELD(TArray<int64>, success_account_ids);
		REFLECTION_REGISTER_FIELD(int64, fail_account_id);
		REFLECTION_REGISTER_FIELD(int32, fail_error_code);
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetRecommendFriend)
		REFLECTION_REGISTER_FIELD(TArray<B2UserPtr>, recommended_users);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFindUser)
		REFLECTION_REGISTER_FIELD(B2UserPtr, user);
		REFLECTION_REGISTER_FIELD(int32, guild_invite_state);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseConfirmNewFriend)
		REFLECTION_REGISTER_FIELD(TArray<long>, applied_friend_ids);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePlatformFriendInviteInfo)
		REFLECTION_REGISTER_FIELD(B2FriendInviteRewardInfoPtr, invite_reward_info)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePlatformFriendInviteCountUp)
		REFLECTION_REGISTER_FIELD(B2FriendInviteRewardInfoPtr, invite_reward_info)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePlatformFriendJoinerCountUp)
		REFLECTION_REGISTER_FIELD(B2FriendInviteRewardInfoPtr, invite_reward_info)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePlatformFriendInviteReward)
		REFLECTION_REGISTER_FIELD(B2FriendInviteRewardInfoPtr, invite_reward_info)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetQuest)
		REFLECTION_REGISTER_FIELD(TArray<B2QuestInfoPtr>, quest_infos); 
		REFLECTION_REGISTER_FIELD(bool, daily_quest_initialized);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAcceptQuest)
		REFLECTION_REGISTER_FIELD(B2QuestInfoPtr, quest_info);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveQuestReward)
		REFLECTION_REGISTER_FIELD(int32, received_reward_slot);
		REFLECTION_REGISTER_FIELD(B2QuestInfoPtr, next_quest_info);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
	REFLECTION_REGISTER_END(3)		
		
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoQuestAlarmNotify, QuestAlarm)
		REFLECTION_REGISTER_FIELD(int32, completed_slot);
		REFLECTION_REGISTER_FIELD(B2QuestInfoPtr, next_quest_info);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetCounterDungeon)
		REFLECTION_REGISTER_FIELD(TArray<int32>, opened_dungeon_types);
		REFLECTION_REGISTER_FIELD(int32, left_ticket);
		REFLECTION_REGISTER_FIELD(int32, weekly_clear_count);
		REFLECTION_REGISTER_FIELD(int32, received_weekly_reward);
		REFLECTION_REGISTER_FIELD(int32, next_buy_ticket_gem_cost);
		REFLECTION_REGISTER_FIELD(int32, weekly_reset_day_of_week);
		REFLECTION_REGISTER_FIELD(int32, main_character_type);
		REFLECTION_REGISTER_FIELD(int32, buy_ticket_count);
		REFLECTION_REGISTER_FIELD(int32, buy_ticket_count_max);
		REFLECTION_REGISTER_FIELD(int32, highest_difficulty_level);
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartCounterDungeon)
		REFLECTION_REGISTER_FIELD(FString, play_token);
		REFLECTION_REGISTER_FIELD(int32, drop_gold);
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills);
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFinishCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, current_gold);
		REFLECTION_REGISTER_FIELD(int32, current_gem);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
		REFLECTION_REGISTER_FIELD(int32, left_ticket);
		REFLECTION_REGISTER_FIELD(int32, weekly_clear_count);
		REFLECTION_REGISTER_FIELD(int32, next_buy_ticket_gem_cost);
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSweepCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id);
		REFLECTION_REGISTER_FIELD(int32, current_gold);
		REFLECTION_REGISTER_FIELD(int32, current_gem);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
		REFLECTION_REGISTER_FIELD(int32, left_ticket);
		REFLECTION_REGISTER_FIELD(int32, weekly_clear_count);
		REFLECTION_REGISTER_FIELD(int32, next_buy_ticket_gem_cost);
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyCounterDungeonTicket)
		REFLECTION_REGISTER_FIELD(int32, current_gem);
		REFLECTION_REGISTER_FIELD(int32, left_ticket);
		REFLECTION_REGISTER_FIELD(int32, buy_ticket_count);
		REFLECTION_REGISTER_FIELD(int32, buy_ticket_count_max);
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCounterDungeonWeeklyReward)
		REFLECTION_REGISTER_FIELD(int32, current_gold);
		REFLECTION_REGISTER_FIELD(int32, current_gem);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
		REFLECTION_REGISTER_FIELD(int32, received_weekly_reward);
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetDimension)
		REFLECTION_REGISTER_FIELD(B2DimensionStatusPtr, dimension_status);
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers);
		REFLECTION_REGISTER_FIELD(int32, from_ranking);
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count);
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(B2DimensionRankingRewardPtr, ranking_reward);
		REFLECTION_REGISTER_FIELD(int64, settlement_time);
		REFLECTION_REGISTER_FIELD(int32, settlement_period_hour);
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartDimension)
		REFLECTION_REGISTER_FIELD(FString, play_token);
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFinishDimension)
		REFLECTION_REGISTER_FIELD(int32, play_result);
		REFLECTION_REGISTER_FIELD(B2DimensionStatusPtr, dimension_status);
		REFLECTION_REGISTER_FIELD(bool, is_new_record);
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, totems);
		REFLECTION_REGISTER_FIELD(TArray<B2DropDataPtr>, fixed_items);
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, update_items);
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCreateGuild)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild_info)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAskJoinGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLeaveGuild)
		REFLECTION_REGISTER_FIELD(int64, leave_guild_time);
		REFLECTION_REGISTER_FIELD(int64, guild_id);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildDetailInfo)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMemberPtr>, guild_members)
		REFLECTION_REGISTER_FIELD(B2GuildBattleStatePtr, guild_battle_state)
		REFLECTION_REGISTER_FIELD(B2GuildBattleEntryPtr, entry)
		REFLECTION_REGISTER_FIELD(bool, has_turn_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMercenaryPtr>, mercenaries)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
		REFLECTION_REGISTER_FIELD(bool, joined_guild_battle_member)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerPtr>, character_powers)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildMember)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMemberPtr>, guild_members)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDeleteGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetRecommendGuild)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildPtr>, guilds)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildMemberCandidates)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMemberCandidatePtr>, candidates)
		REFLECTION_REGISTER_FIELD(bool, need_friend_list)
		REFLECTION_REGISTER_FIELD(TArray<B2FriendPtr>, friends);
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseInviteGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildInviteList)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildInvitePtr>, invites)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRejectInviteGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAcceptInviteGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFindGuild)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseObtainGuildMaster)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMemberPtr>, changed_members)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseHandOverGuildMaster)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildMemberPtr>, changed_members)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildRankingList)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildRankingPtr>, guild_rankings)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAppointSubGuildMaster)
		REFLECTION_REGISTER_FIELD(B2GuildMemberPtr, changed_member)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDismissSubGuildMaster)
		REFLECTION_REGISTER_FIELD(B2GuildMemberPtr, changed_member)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseKickGuildMember)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateGuild)
		REFLECTION_REGISTER_FIELD(int32, join_option)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateGuildNotice)
		REFLECTION_REGISTER_FIELD(FString, notice)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLevelUpGuildSkill)
		REFLECTION_REGISTER_FIELD(B2GuildSkillPtr, skill)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyGuildSkill)
		REFLECTION_REGISTER_FIELD(B2GuildSkillPtr, skill)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMission)
		REFLECTION_REGISTER_FIELD(TArray<B2MissionInfoPtr>, mission_infos);
		REFLECTION_REGISTER_FIELD(TArray<B2MissionCompletePointInfoPtr>, mission_complete_point_infos);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveMissionReward)
		REFLECTION_REGISTER_FIELD(B2MissionInfoPtr, received_mission_info);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveCompleteMissionReward)
		REFLECTION_REGISTER_FIELD(int32, received_mission_type);
		REFLECTION_REGISTER_FIELD(int32, received_reward_index);
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetWing)
		REFLECTION_REGISTER_FIELD(int32, character_type);
		REFLECTION_REGISTER_FIELD(int32, grade);
		REFLECTION_REGISTER_FIELD(int32, level);
		REFLECTION_REGISTER_FIELD(int32, enhance_point);
		REFLECTION_REGISTER_FIELD(int32, upgrade_fail_point);
		REFLECTION_REGISTER_FIELD(TArray<B2WingOptionInfoPtr>, wing_option_infos);
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceWing)
		REFLECTION_REGISTER_FIELD(int32, character_type);
		REFLECTION_REGISTER_FIELD(int32, level);
		REFLECTION_REGISTER_FIELD(int32, enhance_point);
		REFLECTION_REGISTER_FIELD(int32, current_money);
		REFLECTION_REGISTER_FIELD(int32, material_item_count);
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpgradeWing)
		REFLECTION_REGISTER_FIELD(int32, character_type);
		REFLECTION_REGISTER_FIELD(bool, isUpgradeSuccess);
		REFLECTION_REGISTER_FIELD(int32, level);
		REFLECTION_REGISTER_FIELD(int32, grade);
		REFLECTION_REGISTER_FIELD(int32, current_money);
		REFLECTION_REGISTER_FIELD(int32, fail_point);
		REFLECTION_REGISTER_FIELD(int32, material_item_count);
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceWingOption)
		REFLECTION_REGISTER_FIELD(int32, character_type);
		REFLECTION_REGISTER_FIELD(B2WingOptionInfoPtr, enhanced_wing_option);
		REFLECTION_REGISTER_FIELD(int32, material_item_count);
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_BEGIN(FB2MessageInfoMissionAlarmNotify, MissionAlarm)
		REFLECTION_REGISTER_FIELD(int32, completed_quest_id);
		REFLECTION_REGISTER_FIELD(int32, completed_quest_complete_step);
	IMPLEMENT_B2MESSAGEINFO_NOTIFY_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetHeroTower)
		REFLECTION_REGISTER_FIELD(int32, start_floor)
		REFLECTION_REGISTER_FIELD(int32, max_clear_floor)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, daily_try_count)
		REFLECTION_REGISTER_FIELD(int32, current_free_ticket)
		REFLECTION_REGISTER_FIELD(int32, current_buy_ticket)
		REFLECTION_REGISTER_FIELD(int32, possible_try_count)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, current_hero_piece)
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, relics)
		REFLECTION_REGISTER_FIELD(bool, is_sweep)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
		REFLECTION_REGISTER_FIELD(int32, from_ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
	REFLECTION_REGISTER_END(15)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartHeroTower)
		REFLECTION_REGISTER_FIELD(FString, play_token)
		REFLECTION_REGISTER_FIELD(int32, left_free_ticket)
		REFLECTION_REGISTER_FIELD(int32, daily_enter_count)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFinishHeroTower)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(int32, next_floor)
		REFLECTION_REGISTER_FIELD(FString, play_token)
		REFLECTION_REGISTER_FIELD(bool, is_continue)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAbandonHeroTower)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceRelic)
		REFLECTION_REGISTER_FIELD(B2RelicPtr, relic)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_hero_piece_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_hero_piece_item_ids)
		REFLECTION_REGISTER_FIELD(int32, current_money)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePromotionRelic)
		REFLECTION_REGISTER_FIELD(B2RelicPtr, relic)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_boss_piece_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_boss_piece_item_ids)
		REFLECTION_REGISTER_FIELD(int32, current_money)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccountRelic)
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, relics)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSweepHeroTower)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyHeroTowerTicket)
		REFLECTION_REGISTER_FIELD(int32, current_cash)
		REFLECTION_REGISTER_FIELD(int32, current_free_ticket)
		REFLECTION_REGISTER_FIELD(int32, current_buy_ticket)
		REFLECTION_REGISTER_FIELD(int32, possible_try_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDrawShopLottery)
		REFLECTION_REGISTER_FIELD(bool, free_lottery)	
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)
		REFLECTION_REGISTER_FIELD(int32, gem)
		REFLECTION_REGISTER_FIELD(int32, social_point)
		REFLECTION_REGISTER_FIELD(int32, mileage)
		REFLECTION_REGISTER_FIELD(int64, next_free_time)
		REFLECTION_REGISTER_FIELD(int32, daily_lottery_count)
		REFLECTION_REGISTER_FIELD(FString, last_lottery_key)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardShopMileage)
		REFLECTION_REGISTER_FIELD(int32, shop_mileage_type)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards);
		REFLECTION_REGISTER_FIELD(int32, received_reward_index)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetLotteryShop)
		REFLECTION_REGISTER_FIELD(int32, mileage)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopLotteryPtr>, shop_lotteries)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyShopProduct)
		REFLECTION_REGISTER_FIELD(int32, buy_price_type)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(B2BuyShopResultInfoPtr, buy_result_info)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePurchaseCostume)
		REFLECTION_REGISTER_FIELD(int32, buy_price_type)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, costumes)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGeneralShop)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopMileagePtr>, shop_mileages)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopProductMarkPtr>, shop_product_marks)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetCostumeShop)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopProductMarkPtr>, shop_product_marks)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMagicShop)
		REFLECTION_REGISTER_FIELD(int32, renewal_count)
		REFLECTION_REGISTER_FIELD(int64, next_free_renewal_time)
		REFLECTION_REGISTER_FIELD(bool, is_free_renewal)
		REFLECTION_REGISTER_FIELD(TArray<B2MagicShopPtr>, magic_shops)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRenewalMagicShop)
		REFLECTION_REGISTER_FIELD(int32, renewal_count)
		REFLECTION_REGISTER_FIELD(int64, next_free_renewal_time)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(bool, is_free_renewal)
		REFLECTION_REGISTER_FIELD(TArray<B2MagicShopPtr>, magic_shops)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePurchaseMagicShop)
		REFLECTION_REGISTER_FIELD(int32, renewal_count)
		REFLECTION_REGISTER_FIELD(int64, next_free_renewal_time)
		REFLECTION_REGISTER_FIELD(int32, buy_price_type)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(bool, is_free_renewal)
		REFLECTION_REGISTER_FIELD(TArray<B2MagicShopPtr>, magic_shops)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetCollectionItem)
		REFLECTION_REGISTER_FIELD(TArray<B2CollectionItemPtr>, collection_items)
		REFLECTION_REGISTER_FIELD(TArray<B2CollectionSetItemPtr>, reward_collection_items)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardCollectionItem)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, received_reward)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardCollectionSetItem)
		REFLECTION_REGISTER_FIELD(int32, gropup_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, received_reward)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetMarketProduct)
		REFLECTION_REGISTER_FIELD(TArray<B2mdMarketProductPtr>, md_market_products)
		REFLECTION_REGISTER_FIELD(TArray<B2MarketPurchaseInfoPtr>, purchase_infos)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePreparePurchase)
		REFLECTION_REGISTER_FIELD(B2MarketPurchaseInfoPtr, purchase_info)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseVerifyPurchase)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
		REFLECTION_REGISTER_FIELD(B2BuyShopResultInfoPtr, buy_result_info)
		REFLECTION_REGISTER_FIELD(int32, receive_error_code)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCancelPurchase)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFinishPurchase)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoRoomCloseResponse)
		REFLECTION_REGISTER_FIELD(FString, room_id)
		REFLECTION_REGISTER_FIELD(int32, room_type)
		REFLECTION_REGISTER_FIELD(FString, token)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoRoomLeaveResultResponse)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetRoomDebugMode)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoReportUserViaChannelResponse)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoWorldJoinResponse)
		REFLECTION_REGISTER_FIELD(FString, world_id)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoWorldLeaveResponse)
		REFLECTION_REGISTER_FIELD(TArray<FString>, world_id_list)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRaidResult)
		REFLECTION_REGISTER_FIELD(TArray<B2RaidRankingRewardPtr>, raid_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(TArray<B2LightCharacterPtr>, light_characters)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, step_clear_reward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, main_reward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, main_bonus_reward)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDailyAttendance)
		REFLECTION_REGISTER_FIELD(bool, is_attendance)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(int32, attendance_type)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEventAttendance)
		REFLECTION_REGISTER_FIELD(int32, eventId)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
		REFLECTION_REGISTER_FIELD(int32, bonus_type)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBeginnerAttendance)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateDailyPlayTime)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_in_sec)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_reward_index)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveDailyPlayTimeReward)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_in_sec)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_reward_index)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseJoinGuildBattle)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetGuildBattle)
		REFLECTION_REGISTER_FIELD(int64, battle_end_time)
		REFLECTION_REGISTER_FIELD(B2GuildBattleGuildPtr, my_guild)
		REFLECTION_REGISTER_FIELD(B2GuildBattleGuildPtr, target_guild)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartGuildBattle)
		REFLECTION_REGISTER_FIELD(FString, battle_token)
		REFLECTION_REGISTER_FIELD(B2FindAccountInfoPtr, target)
		REFLECTION_REGISTER_FIELD(int32, target_left_medal)
		REFLECTION_REGISTER_FIELD(B2FindAccountInfoPtr, my_info)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseFinishGuildBattle)
		REFLECTION_REGISTER_FIELD(int32, battle_result)
		REFLECTION_REGISTER_FIELD(int32, obtain_medal)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetGuildBattleEntry)
		REFLECTION_REGISTER_FIELD(B2GuildBattleEntryPtr, entry)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildBattleTurnResult)
		REFLECTION_REGISTER_FIELD(int64, mvp_account_id)
		REFLECTION_REGISTER_FIELD(int32, battle_result)
		REFLECTION_REGISTER_FIELD(int32, increased_exp)
		REFLECTION_REGISTER_FIELD(B2GuildBattleGuildPtr, my_guild)
		REFLECTION_REGISTER_FIELD(B2GuildBattleGuildPtr, target_guild)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildBattleTurnReward)
		REFLECTION_REGISTER_FIELD(int32, battle_result)
		REFLECTION_REGISTER_FIELD(int32, guild_medal)
		REFLECTION_REGISTER_FIELD(int32, reward_grade)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildBattleSeasonReward)
		REFLECTION_REGISTER_FIELD(int32, season_ranking)
		REFLECTION_REGISTER_FIELD(int32, season_ranking_total_count)
		REFLECTION_REGISTER_FIELD(int32, gem)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGuildBattleHistory)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildBattleHistoryPtr>, battle_histories)
		REFLECTION_REGISTER_FIELD(B2GuildBattleRecordPtr, season_record)
		REFLECTION_REGISTER_FIELD(B2GuildBattleRecordPtr, best_season_record)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetWingVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetHelmetVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetCostumeVisible)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, costume_type)
		REFLECTION_REGISTER_FIELD(bool, is_visible)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetRaid)
		REFLECTION_REGISTER_FIELD(int64, penalty_remove_time)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRaidResurrection)
		REFLECTION_REGISTER_FIELD(int32, resurrection_count)
		REFLECTION_REGISTER_FIELD(int32, used_gem)
		REFLECTION_REGISTER_FIELD(int32, total_gem)
		REFLECTION_REGISTER_FIELD(int32, next_resurrection_gem_cost)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff1)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff2)
		REFLECTION_REGISTER_FIELD(int32, selected_count_resurrection_buff3)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAssaultMatchmaking)
		REFLECTION_REGISTER_FIELD(FString, token)
		REFLECTION_REGISTER_FIELD(int32, room_type)
		REFLECTION_REGISTER_FIELD(B2EndpointPtr, channel_endpoint)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCancelAssaultMatchmaking)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAssaultResult)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
		REFLECTION_REGISTER_FIELD(B2MatchRankerPtr, prev_ranker)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, victory_rewards)
		REFLECTION_REGISTER_FIELD(int32, increased_mvp_point)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAssaultResurrection)
		REFLECTION_REGISTER_FIELD(int32, resurrection_count)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckPvPMatchPointCharge)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(int32, match_point_charge_lefttime_second)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckTeamMatchPointCharge)
		REFLECTION_REGISTER_FIELD(int32, match_point)
		REFLECTION_REGISTER_FIELD(int32, match_point_charge_lefttime_second)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetFriendRankings)
		REFLECTION_REGISTER_FIELD(int32, opponent_type)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
	REFLECTION_REGISTER_END(2)
	
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, random_option_id1)
		REFLECTION_REGISTER_FIELD(int32, random_option_id2)
		REFLECTION_REGISTER_FIELD(int32, random_option_id3)
		REFLECTION_REGISTER_FIELD(float, random_option_value1)
		REFLECTION_REGISTER_FIELD(float, random_option_value2)
		REFLECTION_REGISTER_FIELD(float, random_option_value3)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGenerateItemRandomOption)
		REFLECTION_REGISTER_FIELD(int32, current_dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, current_cash)
		REFLECTION_REGISTER_FIELD(int32, random_option_id1)
		REFLECTION_REGISTER_FIELD(int32, random_option_id2)
		REFLECTION_REGISTER_FIELD(int32, random_option_id3)
		REFLECTION_REGISTER_FIELD(float, random_option_value1)
		REFLECTION_REGISTER_FIELD(float, random_option_value2)
		REFLECTION_REGISTER_FIELD(float, random_option_value3)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseApplyItemRandomOption)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item_data)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDiscardItemRandomOption)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetItemRandomQuality)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, main_item_infos)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, quality)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGenerateItemRandomQuality)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_id)
		REFLECTION_REGISTER_FIELD(int32, quality)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseApplyItemRandomQuality)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, main_item)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetFactory)
		REFLECTION_REGISTER_FIELD(TArray<B2FactoryPeriodicStatusPtr>, factory_periodic_status)
		REFLECTION_REGISTER_FIELD(TArray<B2FactoryLimitStatusPtr>, limit_status)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseMakeFactoryItem)
		REFLECTION_REGISTER_FIELD(int64, factory_id)
		REFLECTION_REGISTER_FIELD(int64, main_item_id)
		REFLECTION_REGISTER_FIELD(int64, material_item_id1)
		REFLECTION_REGISTER_FIELD(int64, material_item_id2)
		REFLECTION_REGISTER_FIELD(int64, material_item_id3)
		REFLECTION_REGISTER_FIELD(int64, material_item_id4)
		REFLECTION_REGISTER_FIELD(TArray<B2FactoryResultPtr>, make_result)
		REFLECTION_REGISTER_FIELD(int64, left_main_item_count)
		REFLECTION_REGISTER_FIELD(int64, left_material_item_count1)
		REFLECTION_REGISTER_FIELD(int64, left_material_item_count2)
		REFLECTION_REGISTER_FIELD(int64, left_material_item_count3)
		REFLECTION_REGISTER_FIELD(int64, left_material_item_count4)
		REFLECTION_REGISTER_FIELD(int64, current_gold)
		REFLECTION_REGISTER_FIELD(int64, current_gem)
		REFLECTION_REGISTER_FIELD(int64, current_dark_crystal)
		REFLECTION_REGISTER_FIELD(int64, cost_gold)
		REFLECTION_REGISTER_FIELD(int64, cost_gem)
		REFLECTION_REGISTER_FIELD(int64, cost_dark_crystal)
		REFLECTION_REGISTER_FIELD(int64, left_count)
	REFLECTION_REGISTER_END(19)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnsealBox)
		REFLECTION_REGISTER_FIELD(TArray<B2UnsealBoxRewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(int64, seal_box_id)
		REFLECTION_REGISTER_FIELD(int64, left_amount)
		REFLECTION_REGISTER_FIELD(int32, gold);
		REFLECTION_REGISTER_FIELD(int32, gem);
		REFLECTION_REGISTER_FIELD(int32, blade_point);
		REFLECTION_REGISTER_FIELD(int32, social_point);
		REFLECTION_REGISTER_FIELD(int32, dark_crystal);
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket);
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket);
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket);
		REFLECTION_REGISTER_FIELD(int32, fame_point);
	REFLECTION_REGISTER_END(12)


	

	//////////////////////////////////////////////////////////////////////////
/*
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckRedDot)
		REFLECTION_REGISTER_FIELD(bool, has_new_equipment)
		REFLECTION_REGISTER_FIELD(bool, has_new_collection)
		REFLECTION_REGISTER_FIELD(bool, has_mission_to_reward)
		REFLECTION_REGISTER_FIELD(bool, has_mission_mileage_reward)
		REFLECTION_REGISTER_FIELD(bool, has_serial_mission_to_reward)
		REFLECTION_REGISTER_FIELD(bool, has_friend_to_send_point)
		REFLECTION_REGISTER_FIELD(bool, has_friend_request)
		REFLECTION_REGISTER_FIELD(bool, has_adventure_mileage_reward)
		REFLECTION_REGISTER_FIELD(int32, counter_dungeon_weekly_clear_count)
		REFLECTION_REGISTER_FIELD(int32, counter_dungeon_weekly_reward_received)
		REFLECTION_REGISTER_FIELD(int32, tickets_for_counter_dungeon)
		REFLECTION_REGISTER_FIELD(int32, used_tickets_for_hero_tower)
		REFLECTION_REGISTER_FIELD(int32, tickets_for_raid)
		REFLECTION_REGISTER_FIELD(bool, has_one_vs_one_reward)
		REFLECTION_REGISTER_FIELD(bool, has_team_battle_reward)
		REFLECTION_REGISTER_FIELD(bool, has_team_assault_reward)
		REFLECTION_REGISTER_FIELD(int32, team_assault_mvp_point)
		REFLECTION_REGISTER_FIELD(int64, remaining_free_item_time)
		REFLECTION_REGISTER_FIELD(bool, has_shop_item_mileage_reward)
		REFLECTION_REGISTER_FIELD(bool, has_shop_cash_mileage_reward)
		REFLECTION_REGISTER_FIELD(int32, unread_mail_count)
		REFLECTION_REGISTER_FIELD(bool, has_present_mail)
		REFLECTION_REGISTER_FIELD(bool, has_blade_point_mail)
		REFLECTION_REGISTER_FIELD(bool, has_social_point_mail)
		REFLECTION_REGISTER_FIELD(bool, has_lottery_mail)
		REFLECTION_REGISTER_FIELD(bool, has_item_random_option_change)
	REFLECTION_REGISTER_END(26)
*/
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseDarkCrystalPaid)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal_paid)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseModifyItemRandomOption)
		REFLECTION_REGISTER_FIELD(int64, item_id)
		REFLECTION_REGISTER_FIELD(int32, random_option_id1)
		REFLECTION_REGISTER_FIELD(int32, random_option_id2)
		REFLECTION_REGISTER_FIELD(int32, random_option_id3)
		REFLECTION_REGISTER_FIELD(float, random_option_value1)
		REFLECTION_REGISTER_FIELD(float, random_option_value2)
		REFLECTION_REGISTER_FIELD(float, random_option_value3)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseModifyItemNewFlag)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseModifyCostumeNewFlag)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseStartRaid)
		REFLECTION_REGISTER_FIELD(TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetContentsOpenStatus)
		REFLECTION_REGISTER_FIELD(TArray<B2ContentsOpenStatusPtr>, contents)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetDuelRewardBoxInfo)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
		REFLECTION_REGISTER_FIELD(TArray<B2DuelRewardBoxInfoResponseRewardBoxPtr>, reward_boxes)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetDuelRewardBoxGainableList)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(TArray<B2DuelRewardBoxGainableListResponseRewardItemPtr>, reward_items)
		REFLECTION_REGISTER_FIELD(int32, reward_item_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseOpenDuelRewardBox)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards)
		REFLECTION_REGISTER_FIELD(B2DuelRewardBoxStatusPtr, reward_box_status)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveAssaultMvpReward)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, received_rewards)
	REFLECTION_REGISTER_END(1)
	
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccountAether)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, aethers)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterSetOptionInfoPtr>, current_character_set_option_Infos)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDismantleAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, dismantle_aether_Ids)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, current_aether_piece)
		REFLECTION_REGISTER_FIELD(int32, obtain_aether_piece_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseMountAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, mounted_aether_ids)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unmounted_aether_ids)
		REFLECTION_REGISTER_FIELD(int32, selected_offense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, selected_defense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, selected_unity_skill_set_option_id)
		REFLECTION_REGISTER_FIELD(TArray<int32>, defense_set_option_ids)
		REFLECTION_REGISTER_FIELD(TArray<int32>, unity_skill_set_option_ids)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnmountAethers)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unmounted_aether_ids)
		REFLECTION_REGISTER_FIELD(int32, selected_offense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, selected_defense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, selected_unity_skill_set_option_id)
		REFLECTION_REGISTER_FIELD(TArray<int32>, defense_set_option_ids)
		REFLECTION_REGISTER_FIELD(TArray<int32>, unity_skill_set_option_ids)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSelectAetherDefenseOption)
		REFLECTION_REGISTER_FIELD(int32, selected_defense_set_option_id)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSelectAetherUnitySkillOption)
		REFLECTION_REGISTER_FIELD(int32, selected_unity_skill_set_option_id)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLockAethers)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, locked_aethers)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnlockAethers)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, unlocked_aethers)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseExtendAetherInventory)
		REFLECTION_REGISTER_FIELD(B2AccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(int32, current_slot_count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEnhanceAether)
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, aether)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_aether_piece_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_aether_piece_ids)
		REFLECTION_REGISTER_FIELD(bool, is_success)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSellAethers)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(TArray<int64>, sell_aether_ids)
		REFLECTION_REGISTER_FIELD(int32, obtain_gold)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseModifyAetherNewFlag)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetTutorialProgress)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(bool, is_item_prepared)
		REFLECTION_REGISTER_FIELD(bool, is_lottery_drawn)
		REFLECTION_REGISTER_FIELD(bool, is_finished)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetTutorialProgress)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(bool, is_item_prepared)
		REFLECTION_REGISTER_FIELD(bool, is_lottery_drawn)
		REFLECTION_REGISTER_FIELD(bool, is_finished)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUpdateTutorialProgress)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(bool, is_item_prepared)
		REFLECTION_REGISTER_FIELD(bool, is_lottery_drawn)
		REFLECTION_REGISTER_FIELD(bool, is_finished)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSkipTutorialProgress)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(bool, is_item_prepared)
		REFLECTION_REGISTER_FIELD(bool, is_lottery_drawn)
		REFLECTION_REGISTER_FIELD(bool, is_finished)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDrawTutorialItem)
		REFLECTION_REGISTER_FIELD(bool, free_lottery)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(int32, gem)
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)
		REFLECTION_REGISTER_FIELD(int32, social_point)
		REFLECTION_REGISTER_FIELD(int32, mileage)
		REFLECTION_REGISTER_FIELD(int64, next_free_time)
		REFLECTION_REGISTER_FIELD(int32, daily_lottery_count)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePrepareTutorial)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(int32, current_cash)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, added_item_list)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, added_aether_list)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetUnitySkillMission)
		REFLECTION_REGISTER_FIELD(TArray<B2UnitySkillMissionPtr>, missions)
		REFLECTION_REGISTER_FIELD(TArray<B2UnitySkillAwakenMissionPtr>, awaken_missions)
		REFLECTION_REGISTER_FIELD(bool, all_mission_completed)
		REFLECTION_REGISTER_FIELD(bool, all_awaken_mission_completed)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveUnitySkillReward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward)
		REFLECTION_REGISTER_FIELD(B2UnitySkillMissionPtr, next_mission)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAwakenUnitySkillMission)
		REFLECTION_REGISTER_FIELD(B2UnitySkillAwakenMissionPtr, awaken_mission)
		REFLECTION_REGISTER_FIELD(int32, current_money)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_material_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_item_ids)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAwakenUnitySkill)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetDuelModeInfo)
		REFLECTION_REGISTER_FIELD(TArray<B2ContentsModeInfoPtr>, mode_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetChallengeModeInfo)
		REFLECTION_REGISTER_FIELD(TArray<B2ContentsModeInfoPtr>, mode_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
		REFLECTION_REGISTER_FIELD(int32, hero_tower_ticket_count)
		REFLECTION_REGISTER_FIELD(int32, counter_dungeon_left_ticket_count)
		REFLECTION_REGISTER_FIELD(int32, dimension_daily_clear_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetHallOfFame)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePtr>, basics)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFameUserPtr>, rankers)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFameRewardPtr>, ranker_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePraisePtr>, left_praise_counts)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePraiseTargetRanker)
		REFLECTION_REGISTER_FIELD(TArray<B2HallOfFamePtr>, basics)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(B2HallOfFamePraisePtr, left_praise_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveRestReward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward)
		REFLECTION_REGISTER_FIELD(int32, used_cash)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckModeOpen)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
		REFLECTION_REGISTER_FIELD(int32, guild_battle_state)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGiveTutorialReward)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, tutorial_rewards)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckReachPower)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerPtr>, character_power)
		REFLECTION_REGISTER_FIELD(TArray<int32>, required_tracking_targets)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseTrackingPowerDetail)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, item_preset_id)
		REFLECTION_REGISTER_FIELD(int32, skill_slot)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
		REFLECTION_REGISTER_FIELD(int32, different_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseKakaoMessageProcessing)
		REFLECTION_REGISTER_FIELD(FString, message_box_id)
		REFLECTION_REGISTER_FIELD(int32, processing_count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseKakaoCouponUse)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseKakaoAccountConversionPrepare)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseKakaoAccountConversionComplete)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetDonationEvent)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, free_donation_count)
		REFLECTION_REGISTER_FIELD(int32, point)
		REFLECTION_REGISTER_FIELD(int32, whole_step)
		REFLECTION_REGISTER_FIELD(TArray<B2DonationPointRewardStatePtr>, point_reward_state)
		REFLECTION_REGISTER_FIELD(bool, settled)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerPtr>, rankers)
		REFLECTION_REGISTER_FIELD(int32, from_ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
		REFLECTION_REGISTER_FIELD(B2RankingPtr, ranking)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseMakeDonation)
		REFLECTION_REGISTER_FIELD(bool, is_free_donation)
		REFLECTION_REGISTER_FIELD(int32, free_donation_count)
		REFLECTION_REGISTER_FIELD(TArray<B2DonationPresentPtr>, donation_presents)
		REFLECTION_REGISTER_FIELD(int32, decrease_gem)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, decrease_gold)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(int32, obtain_point)
		REFLECTION_REGISTER_FIELD(int32, current_point)
		REFLECTION_REGISTER_FIELD(FString, last_donation_key)
		REFLECTION_REGISTER_FIELD(B2RawRewardPtr, bundled_fixed_reward)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
		REFLECTION_REGISTER_FIELD(int32, present_gem)
		REFLECTION_REGISTER_FIELD(int32, present_blade_point)
	REFLECTION_REGISTER_END(14)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDonationPointReward)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2DonationPointRewardStatePtr>, point_reward_state)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetFairy)
		REFLECTION_REGISTER_FIELD(TArray<B2FairyStatusPtr>, fairies)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLevelupFairy)
		REFLECTION_REGISTER_FIELD(B2FairyStatusPtr, updated_fairy)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, changed_material_item)
		REFLECTION_REGISTER_FIELD(int64, deleted_item_id)
		REFLECTION_REGISTER_FIELD(bool, is_levelup)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBlessFairy)
		REFLECTION_REGISTER_FIELD(B2FairyStatusPtr, updated_fairy)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveFairyGift)
		REFLECTION_REGISTER_FIELD(bool, jackpoted)
		REFLECTION_REGISTER_FIELD(int32, multiple_value)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(int64, next_fairy_gift_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseCheckChargePoints)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
		REFLECTION_REGISTER_FIELD(int32, blade_point_max)
		REFLECTION_REGISTER_FIELD(int32, blade_point_charge_lefttime_second)
		REFLECTION_REGISTER_FIELD(int32, pvp_match_point)
		REFLECTION_REGISTER_FIELD(int32, pvp_match_point_charge_lefttime_second)
		REFLECTION_REGISTER_FIELD(int32, team_match_point)
		REFLECTION_REGISTER_FIELD(int32, team_match_point_charge_lefttime_second)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetFeaturedMission)
		REFLECTION_REGISTER_FIELD(TArray<B2MissionInfoPtr>, mission_infos);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetEvent)
		REFLECTION_REGISTER_FIELD(TArray<B2PointEventStatusPtr>, point_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2LevelUpEventInfoPtr>, level_up_event_infos);
		REFLECTION_REGISTER_FIELD(TArray<B2StageClearEventStatusPtr>, stage_clear_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2PointShopEventStatusPtr>, point_shop_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2TenLotteryEventStatusPtr>, ten_lottery_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2GemPurchaseEventStatusPtr>, gem_purchase_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2SpendGemEventStatusPtr>, spend_gem_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2TenLotterySavingsStatusPtr>, ten_lottery_savings_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2DiceEventStatusPtr>, dice_event_statuses);
		REFLECTION_REGISTER_FIELD(TArray<B2PointShopEventRewardHistoryPtr>, point_shop_event_reward_histories);
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceivePointEventReward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveLevelUpEventReward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetSeasonMission)
		REFLECTION_REGISTER_FIELD(TArray<B2SeasonMissionInfoPtr>, missions);
		REFLECTION_REGISTER_FIELD(TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardSeasonMission)
		REFLECTION_REGISTER_FIELD(B2SeasonMissionInfoPtr, reward_mission);
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAttendSeasonMission)
		REFLECTION_REGISTER_FIELD(TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveStageClearEventReward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyHotTimeBuff)
		REFLECTION_REGISTER_FIELD(int32, buff_type);
		REFLECTION_REGISTER_FIELD(int64, buff_end_time);
		REFLECTION_REGISTER_FIELD(int32, buff_ticket); 
		REFLECTION_REGISTER_FIELD(int32, current_gem);
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardFlatRatePackageBonus)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, received_reward);
		REFLECTION_REGISTER_FIELD(B2FlatRatePackageBonusStatePtr, next_bonus_state);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetPackageShop)
		REFLECTION_REGISTER_FIELD(TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
		REFLECTION_REGISTER_FIELD(B2FlatRatePackageBonusStatePtr, flat_rate_package_bonus_state);
		REFLECTION_REGISTER_FIELD(TArray<B2LevelUpPackageMissionPtr>, level_up_package_missions);
		REFLECTION_REGISTER_FIELD(TArray<B2ReachLevelPackageMissionPtr>, reach_level_package_missions);
		REFLECTION_REGISTER_FIELD(TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopProductMarkPtr>, shop_product_marks)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetPeriodPackageState)
		REFLECTION_REGISTER_FIELD(TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardLevelUpPackage)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, received_reward);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardReachLevelPackage)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, received_reward);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGiveUpQuest)
		REFLECTION_REGISTER_FIELD(B2QuestInfoPtr, next_quest_info);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetContentsModeState)
		REFLECTION_REGISTER_FIELD(TArray<B2ContentsModeInfoPtr>, mode_infos);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardPointShopEvent)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, reward);
		REFLECTION_REGISTER_FIELD(B2PointShopEventStatusPtr, point_shop_event_status);
		REFLECTION_REGISTER_FIELD(int32, index);
		REFLECTION_REGISTER_FIELD(int32, total_reward_count);
		REFLECTION_REGISTER_FIELD(int32, buy_count);
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReportUser)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBlockChat)
		REFLECTION_REGISTER_FIELD(B2BlockChatUserPtr, blocked_user);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnblockChat)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetBlockChatList)
		REFLECTION_REGISTER_FIELD(TArray<B2BlockChatUserPtr>, blocked_users);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseClientStateReport)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardTenLotteryEvent)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards);
		REFLECTION_REGISTER_FIELD(int32, reward_index);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseReceiveGemPurchaseEventReward)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardSpendGemEvent)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardTenLotterySavingsEvent)
		REFLECTION_REGISTER_FIELD(int32, current_point)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRollDiceEvent)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, cell_reward)
		REFLECTION_REGISTER_FIELD(B2RewardPtr, finish_reward)
		REFLECTION_REGISTER_FIELD(int32, current_position)
		REFLECTION_REGISTER_FIELD(int32, dice_number)
		REFLECTION_REGISTER_FIELD(int32, dice_cell_type)
		REFLECTION_REGISTER_FIELD(bool, is_finish)
		REFLECTION_REGISTER_FIELD(int32, reward_count_times)
		REFLECTION_REGISTER_FIELD(int32, free_dice_count)
		REFLECTION_REGISTER_FIELD(int32, finish_count)
		REFLECTION_REGISTER_FIELD(int32, roll_dice_count)
		REFLECTION_REGISTER_FIELD(int32, current_point)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyDiceEventPoint)
		REFLECTION_REGISTER_FIELD(int32, current_gem)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
		REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRewardDiceEventMileage)
		REFLECTION_REGISTER_FIELD(int32, reward_value)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(2)

	// totem
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestGetAccountTotem, GetAccountTotem)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseGetAccountTotem)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, totems)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestEquipTotems, EquipTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseEquipTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, equiped_totem_ids)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_totem_ids)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnequipTotems, UnequipTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnequipTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, unequiped_totem_ids)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestLockTotems, LockTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseLockTotems)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, locked_totems)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestUnlockTotems, UnlockTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseUnlockTotems)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, unlocked_totems)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestSellTotems, SellTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSellTotems)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(TArray<int64>, sell_totem_ids)
		REFLECTION_REGISTER_FIELD(int32, obtain_gold)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestDismantleTotems, DismantleTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, totem_ids)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDismantleTotems)
		REFLECTION_REGISTER_FIELD(TArray<int64>, dismantle_totem_ids)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, current_dimensional_piece)
		REFLECTION_REGISTER_FIELD(int32, obtain_dimensional_piece_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestRefineTotem, RefineTotem)
		REFLECTION_REGISTER_FIELD(int64, totem_id)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, cost_type)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRefineTotem)
		REFLECTION_REGISTER_FIELD(int32, current_gold)
		REFLECTION_REGISTER_FIELD(B2TotemServerInfoPtr, refined_totem)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, changed_dimensional_piece_items)
		REFLECTION_REGISTER_FIELD(TArray<int64>, deleted_dimensional_piece_ids)
		REFLECTION_REGISTER_FIELD(bool, is_success)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_B2MESSAGEINFO_REQUEST_BEGIN(FB2MessageInfoRequestModifyTotemNewFlag, ModifyTotemNewFlag)
	IMPLEMENT_B2MESSAGEINFO_REQUEST_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseModifyTotemNewFlag)
	REFLECTION_REGISTER_END(0)


	// REPLACEMENT POINT: MESSAGE_INFO CPP RESPONSE
	// NEVER DELETE LINE ABOVE
			
	// for console command

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseDeleteAccount)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAddItemToInventory)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, items)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAddCostumeToInventory)
		REFLECTION_REGISTER_FIELD(B2CostumeServerInfoPtr, costume)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAddTotemToInventory)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, totems)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseCashPaid)
		REFLECTION_REGISTER_FIELD(int32, cash_paid)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseMoney)
		REFLECTION_REGISTER_FIELD(int32, money)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetItemLevelup)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetCharacterLevelExp)
		REFLECTION_REGISTER_FIELD(B2CharacterServerInfoPtr, character)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetCharacterRankInfo)
		REFLECTION_REGISTER_FIELD(B2CharacterServerInfoPtr, character)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetCharacterRelic)
		REFLECTION_REGISTER_FIELD(B2RelicPtr, relic)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetBladePoint)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseRankPromotionPoint)
		REFLECTION_REGISTER_FIELD(int32, rank_promotion_point)
	REFLECTION_REGISTER_END(1)
	
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseSocialPoint)
		REFLECTION_REGISTER_FIELD(int32, current_social_point)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAddMail)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseMakeRanking)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseForceClearStage)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetQuestState)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseIncreaseHeroPiece)
		REFLECTION_REGISTER_FIELD(int32, current_hero_piece)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseForceClearHeroTower)
		REFLECTION_REGISTER_FIELD(int32, start_floor)
		REFLECTION_REGISTER_FIELD(int32, max_clear_floor)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, tag_character_type)
		REFLECTION_REGISTER_FIELD(int32, daily_try_count)
		REFLECTION_REGISTER_FIELD(int32, daily_max_free_try_count)
		REFLECTION_REGISTER_FIELD(int32, daily_left_free_try_count)
		REFLECTION_REGISTER_FIELD(int32, current_hero_piece)
		REFLECTION_REGISTER_FIELD(bool, is_sweep)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseForceStartCounterDungeon)
		REFLECTION_REGISTER_FIELD(FString, play_token);
		REFLECTION_REGISTER_FIELD(int32, drop_gold);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePrepareSweepHeroTower)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePrepareGuild)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetGuildBattleState)
		REFLECTION_REGISTER_FIELD(B2GuildBattleStatePtr, state);
		REFLECTION_REGISTER_FIELD(FString, state_message);
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResetAttendanceTime)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResetEventAttendance)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResetDailyPlayTimeRecord)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_in_sec)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_reward_index)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseResetTeamMatchPoint)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseChangeQuestId)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseChangeQuestCount)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponsePrepareFriend)
		REFLECTION_REGISTER_FIELD(int32, new_friend_count)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetGuildLevelExp)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild);
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseAddAetherToInventory)
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, aether)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseForceUpdateAether)
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, updated_aether)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetMatchSeasonReward)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetStageBoostCount)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSkipMatchTutorial)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseSetDebugMode)
	REFLECTION_REGISTER_END(0)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseServerCmd)
		REFLECTION_REGISTER_FIELD(FString, result_message)
		REFLECTION_REGISTER_FIELD(bool, need_restart)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseTrace)
		REFLECTION_REGISTER_FIELD(FString, tag)
		REFLECTION_REGISTER_FIELD(int64, client_send_time)
		REFLECTION_REGISTER_FIELD(int64, session_recv_time)
		REFLECTION_REGISTER_FIELD(int64, app_recv_time)
		REFLECTION_REGISTER_FIELD(int64, db_request_time)
		REFLECTION_REGISTER_FIELD(int64, db_response_time)
		REFLECTION_REGISTER_FIELD(int64, app_send_time)
		REFLECTION_REGISTER_FIELD(int64, session_send_time)
	REFLECTION_REGISTER_END(8)


}