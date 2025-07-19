#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include "Config.h"
#include "B2MessageInfoCommonStruct.h"

//////////////////////////////////////////////////////////////////////////
// forward declare
namespace b2network {
    class Client;
    class FB2MessageInfo;
    class FB2MessageInfoNotify;
    class FB2MessageInfoCommand;

    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
}

typedef std::shared_ptr<b2network::FB2MessageInfo> FB2MessageInfoPtr;
typedef std::function<void(FB2MessageInfoPtr)> FB2MessageCallback;

typedef std::shared_ptr<b2network::FB2MessageInfoNotify> FB2MessageInfoNotifyPtr;
typedef std::shared_ptr<b2network::FB2MessageInfoCommand> FB2MessageInfoCommandPtr;

namespace b2network {
	//////////////////////////////////////////////////////////////////////////
	// define network result code
	enum EB2ResultCode : int
	{
		B2RESULT_SUCCESS = 0,
		B2RESULT_UNKNOWN,

		B2RESULT_DISCONNECT_AUTH_SERVER,
		B2RESULT_DISCONNECT_SESSION_SERVER,

		B2RESULT_DUPLICATED_USER = 100,
		B2RESULT_DUPLICATED_CHARACTER_NAME = 101,
		B2RESULT_ACCOUNT_NOT_FOUND = 102,

		B2RESULT_UNAUTHENTICATED = 401,
		B2RESULT_UNAUTHORIZED = 402,

		B2RESULT_NOT_ENOUGH_INVENTORY_COUNT = 4015,
		B2RESULT_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT = 4030,
	};


	//////////////////////////////////////////////////////////////////////////
	// b2message handler manager

    typedef std::function<void(std::shared_ptr<b2network::Client>, FB2MessageInfoPtr)> B2MessageInfoHandler;
    typedef std::function<FB2MessageInfoPtr(std::shared_ptr<b2network::Client>, MessagePtr)> B2MessageInfoListener;

    class FB2MessageHandlerManager
	{
		static std::shared_ptr<FB2MessageHandlerManager> m_pInstance;

		typedef std::unordered_map<std::string, B2MessageInfoHandler> B2MessageInfoHandlerMap;
		B2MessageInfoHandlerMap handlerMap;

        typedef std::unordered_map<std::string, B2MessageInfoListener> B2MessageInfoListenerMap;
        B2MessageInfoListenerMap listenerMap;

	public:
		static std::shared_ptr<FB2MessageHandlerManager> GetInstance();

        B2MessageInfoHandler GetHandler(const std::string& MessageType);
		void RegistHandler(const std::string& MessageType, B2MessageInfoHandler Handler);

        B2MessageInfoListener GetListener(const std::string& messageType);
        void RegisterListener(const std::string& messageType, B2MessageInfoListener listener);
	};

    typedef std::shared_ptr<FB2MessageHandlerManager> FB2MessageHandlerManagerPtr;

	//////////////////////////////////////////////////////////////////////////
	// b2messageinfo base class
	class B2NETWORK_API FB2MessageInfo : public Reflection
	{
	protected:
		std::string MessageInfoType;
		EB2ResultCode ErrorCode;

	public:
		FB2MessageInfo(std::string className) : Reflection(className)
		{
			MessageInfoType = className;
			ErrorCode = B2RESULT_SUCCESS;
		}
        virtual ~FB2MessageInfo(){};

		std::string& GetType() { return MessageInfoType; }

        bool IsSuccess() { return (ErrorCode == B2RESULT_SUCCESS); }
		EB2ResultCode GetErrorCode() { return ErrorCode; }
		void SetErrorCode(int InErrorCode) { ErrorCode = static_cast<EB2ResultCode>(InErrorCode); };

		virtual void DoCallback(FB2MessageInfoPtr InParam) = 0;
		virtual FB2MessageCallback GetSuccessCallback() = 0;
		virtual FB2MessageCallback GetFailCallback() = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class B2NETWORK_API FB2MessageInfoRequest : public FB2MessageInfo
	{
	protected:
		FB2MessageCallback SuccessCallback;
		FB2MessageCallback FailCallback;

	public:
		FB2MessageInfoRequest(std::string className, FB2MessageCallback InSuccessCallback = nullptr,
			FB2MessageCallback InFailCallback = nullptr) : FB2MessageInfo(className)
		{
			SuccessCallback = InSuccessCallback;
			FailCallback = InFailCallback;
		}
        virtual ~FB2MessageInfoRequest(){};

		virtual FB2MessageCallback GetSuccessCallback() override 
		{
			return SuccessCallback;
		}

		virtual FB2MessageCallback GetFailCallback() override
		{
			return FailCallback;
		}

		virtual void DoCallback(FB2MessageInfoPtr InParam) override
		{
			if (InParam)
			{
				if (InParam->IsSuccess())
				{
					if (SuccessCallback)
					{
						SuccessCallback(InParam);
					}
				}
				else
				{
					if (FailCallback)
					{
						FailCallback(InParam);
					}
				}
			}
		}
	};

    //////////////////////////////////////////////////////////////////////////
    class B2NETWORK_API FB2MessageInfoCommand : public FB2MessageInfo
    {
    public:
        FB2MessageInfoCommand(std::string className) : FB2MessageInfo(className)
        {
        };
        virtual ~FB2MessageInfoCommand(){};

		virtual void DoCallback(FB2MessageInfoPtr InParam) override {};
		virtual FB2MessageCallback GetSuccessCallback() override { return nullptr; }
		virtual FB2MessageCallback GetFailCallback() override { return nullptr; }
    };

    //////////////////////////////////////////////////////////////////////////
    class B2NETWORK_API FB2MessageInfoNotify : public FB2MessageInfo
    {
    public:
        FB2MessageInfoNotify(std::string className) : FB2MessageInfo(className)
        {
        };
        virtual ~FB2MessageInfoNotify(){};

		virtual void DoCallback(FB2MessageInfoPtr InParam) override {};
		virtual FB2MessageCallback GetSuccessCallback() override { return nullptr; }
		virtual FB2MessageCallback GetFailCallback() override { return nullptr; }
    };

    //////////////////////////////////////////////////////////////////////////
#define DECLARE_B2MESSAGEINFO_REQUEST(ClassName)										\
	class ClassName;																	\
	using ClassName##Ptr = std::shared_ptr<ClassName>;									\
	class B2NETWORK_API ClassName : public FB2MessageInfoRequest						\
	{																					\
	public:																				\
		ClassName() : FB2MessageInfoRequest(#ClassName) {};								\
        virtual ~ClassName(){};                                                         \
		ClassName(FB2MessageCallback InSuccessCallback,									\
			FB2MessageCallback InFailCallback);											\
		using type_list0 = TU::TypeList<>;

#define DECLARE_B2MESSAGEINFO_REQUEST_END(count)										\
		FINALIZE_TYPELIST(count)														\
		static const uint32 field_num = count;											\
   	};

#define DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()									\
		static const uint32 field_num = 0;												\
    };

    //////////////////////////////////////////////////////////////////////////
#define DECLARE_B2MESSAGEINFO_COMMAND(ClassName)										\
	class ClassName;																	\
	using ClassName##Ptr = std::shared_ptr<ClassName>;									\
    class B2NETWORK_API ClassName : public FB2MessageInfoCommand                        \
	{																			        \
	public:																				\
		ClassName() : FB2MessageInfoCommand(#ClassName) {};                             \
        virtual ~ClassName(){};                                                         \
		ClassName(FB2MessageCallback InSuccessCallback,									\
			FB2MessageCallback InFailCallback) : FB2MessageInfoCommand(#ClassName) {};	\
		using type_list0 = TU::TypeList<>;

#define DECLARE_B2MESSAGEINFO_COMMAND_END(count)										\
		FINALIZE_TYPELIST(count)														\
		static const uint32 field_num = count;											\
    };

#define DECLARE_B2MESSAGEINFO_COMMAND_END_NO_FIELD()									\
		static const uint32 field_num = 0;												\
    };

    //////////////////////////////////////////////////////////////////////////
#define DECLARE_B2MESSAGEINFO_NOTIFY(ClassName)										    \
	class ClassName;																	\
	using ClassName##Ptr = std::shared_ptr<ClassName>;									\
    class B2NETWORK_API ClassName : public FB2MessageInfoNotify                         \
    {																		            \
	public:																				\
        ClassName() : FB2MessageInfoNotify(#ClassName) {};                              \
        virtual ~ClassName(){};                                                         \
		using type_list0 = TU::TypeList<>;

#define DECLARE_B2MESSAGEINFO_NOTIFY_END(count)										    \
		FINALIZE_TYPELIST(count)														\
		static const uint32 field_num = count;											\
    };

#define DECLARE_B2MESSAGEINFO_NOTIFY_END_NO_FIELD()									    \
		static const uint32 field_num = 0;												\
    };

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccount)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFindAccount)
		B2_DECLARE_FIELD(0, TArray<B2FindAccountQueryPtr>, query_parameters)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountItem)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountCostume)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMasterData)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetLatestEventData)
		B2_DECLARE_FIELD(0, int64, last_update_time)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSignUp)
		B2_DECLARE_FIELD(0, FString, name)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellItem)
		B2_DECLARE_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConvertData)
		B2_DECLARE_FIELD(0, int32, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConvertItem)
		B2_DECLARE_FIELD(0, int32, item_id)
		B2_DECLARE_FIELD(1, int32, convert_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellConsumableItem)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, sell_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipItem)
		B2_DECLARE_FIELD(0, int32, preset_id)
		B2_DECLARE_FIELD(1, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipItem)
		B2_DECLARE_FIELD(0, int32, preset_id)
		B2_DECLARE_FIELD(1, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipCostume)
		B2_DECLARE_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipCostume)
		B2_DECLARE_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceCostume)
		B2_DECLARE_FIELD(0, int64, costume_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleCostumes)
		B2_DECLARE_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockCostumes)
		B2_DECLARE_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockCostumes)
		B2_DECLARE_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockItem)
		B2_DECLARE_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockItem)
		B2_DECLARE_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExtendInventory)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, extend_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelupItem)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, TArray<int64>, material_item_ids)
		B2_DECLARE_FIELD(2, bool, auto_levelup)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceItem)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, scroll_item_id)
		B2_DECLARE_FIELD(2, bool, use_protection)
		B2_DECLARE_FIELD(3, int32, protection_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleItems)
		B2_DECLARE_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExchangeItemEnhanceLevel)
		B2_DECLARE_FIELD(0, int64, item_id_1)
		B2_DECLARE_FIELD(1, int64, item_id_2)
		B2_DECLARE_FIELD(2, int32, price_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenSealSlotItem)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, seal_slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCarveSealItem)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, seal_slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSurpassItem)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpgradeItem)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestComposeItem)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int64, sub_item_id)
		B2_DECLARE_FIELD(2, int32, current_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeItemCharacterType)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetChangeItemCharacterTypeCost)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, main_character_type)
		B2_DECLARE_FIELD(2, int32, tag_character_type)
		B2_DECLARE_FIELD(3, int32, selected_option_flags)
		B2_DECLARE_FIELD(4, int32, stage_repeat_mode)
		B2_DECLARE_FIELD(5, bool, boost_mode)
		B2_DECLARE_FIELD(6, int32, tutorial_id)
		B2_DECLARE_FIELD(7, int32, main_character_preset_id)
		B2_DECLARE_FIELD(8, int32, tag_character_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestClearStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, FString, token)
		B2_DECLARE_FIELD(2, int32, main_character_type)
		B2_DECLARE_FIELD(3, int32, tag_character_type)
		B2_DECLARE_FIELD(4, int32, clear_time_second)
		B2_DECLARE_FIELD(5, TArray<B2KillMonsterInfoPtr>, kill_monster_infos)
		B2_DECLARE_FIELD(6, int32, total_monster_kill_count)
		B2_DECLARE_FIELD(7, bool, is_all_survival)
		B2_DECLARE_FIELD(8, int32, qte_start_count)
		B2_DECLARE_FIELD(9, int32, qte_finish_count)
		B2_DECLARE_FIELD(10, B2ItemSpotSaleOptionPtr, item_sale_option)
		B2_DECLARE_FIELD(11, B2AetherSpotSaleOptionPtr, aether_sale_option)
	DECLARE_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, main_character_type)
		B2_DECLARE_FIELD(2, int32, tag_character_type)
		B2_DECLARE_FIELD(3, int32, main_character_preset_id)
		B2_DECLARE_FIELD(4, int32, tag_character_preset_id)
		B2_DECLARE_FIELD(5, int32, repeat_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFailStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, FString, token)
		B2_DECLARE_FIELD(2, int32, reason)
		B2_DECLARE_FIELD(3, int32, play_time_second)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResurrection)
		B2_DECLARE_FIELD(0, FString, token)
		B2_DECLARE_FIELD(1, int32, selected_buff_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckBladePointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemSurpassCost)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemUpgradeCost)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemComposeCost)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int32, current_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetActInfo)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetStageInfo)
		B2_DECLARE_FIELD(0, int32, stage_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelUpSkill)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, skill_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetSkill)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetUsingSkill)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, slot)
		B2_DECLARE_FIELD(2, int32, using_skill_id_1)
		B2_DECLARE_FIELD(3, int32, using_skill_id_2)
		B2_DECLARE_FIELD(4, int32, using_skill_id_3)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuySkillPoint)
		B2_DECLARE_FIELD(0, TArray<B2BuySkillPointRequestInfoPtr>, buy_infos)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveNickname)
        B2_DECLARE_FIELD(0, FString, nickname)
    DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateNickname)
		B2_DECLARE_FIELD(0, FString, nickname)
		B2_DECLARE_FIELD(1, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetServerVersion)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD();

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveCharacterName)
        B2_DECLARE_FIELD(0, int32, character_type);
        B2_DECLARE_FIELD(1, FString, name);
    DECLARE_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveActRepeatClearReward)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveActClearReward)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
		B2_DECLARE_FIELD(2, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetTeamBattleStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAssaultBattleStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateTeamBattleEntry)
		B2_DECLARE_FIELD(0, B2TeamBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceTeamBattleFormation)
		B2_DECLARE_FIELD(0, int32, formation_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTeamMatchmaking)
		B2_DECLARE_FIELD(0, bool, use_additional_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTeamMatchResult)
		B2_DECLARE_FIELD(0, FString, match_token)
		B2_DECLARE_FIELD(1, int32, match_result)
		B2_DECLARE_FIELD(2, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRank)		
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceRankNode)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMailList)
		B2_DECLARE_FIELD(0, int32, mail_category)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenMail)
		B2_DECLARE_FIELD(0, int64, mail_id)
		B2_DECLARE_FIELD(1, int32, lottery_character_type)
		B2_DECLARE_FIELD(2, int32, lottery_item_template_id)
		B2_DECLARE_FIELD(3, int32, lottery_aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenAllMail)
		B2_DECLARE_FIELD(0, int32, mail_category)
		B2_DECLARE_FIELD(1, int64, start_id)
		B2_DECLARE_FIELD(2, int64, end_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetNewMailCount)		
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskFriend)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptAskFriend)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRejectAskFriend)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFriendList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelAskFriend)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDeleteFriend)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAllSendAskFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAllReceiveAskFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSendSocialPoint)
		B2_DECLARE_FIELD(0, TArray<int64>, account_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRecommendFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFindUser)
		B2_DECLARE_FIELD(0, FString, nickname)
		B2_DECLARE_FIELD(1, bool, guild_invite_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConfirmNewFriend)
		B2_DECLARE_FIELD(0, bool, is_all_confirm)				// true 면 모든 미확인 대상, false : 지정된 대상.	
		B2_DECLARE_FIELD(1, TArray<int64>, friend_ids)			// 친구 확인 목록 ( case is_all_confirm = false ) 
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteInfo)
		B2_DECLARE_FIELD(0, int32, platform_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteCountUp)
		B2_DECLARE_FIELD(0, int32, platform_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendJoinerCountUp)
		B2_DECLARE_FIELD(0, int32, platform_type)
		B2_DECLARE_FIELD(1, int32, joiner_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteReward)
		B2_DECLARE_FIELD(0, int32, platform_type)
		B2_DECLARE_FIELD(1, int32, reward_type)
		B2_DECLARE_FIELD(2, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetQuest)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptQuest)
		B2_DECLARE_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveQuestReward)
		B2_DECLARE_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetCounterDungeon)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id)
		B2_DECLARE_FIELD(1, int32, main_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id)
		B2_DECLARE_FIELD(1, FString, play_token)
		B2_DECLARE_FIELD(2, bool, is_success)
		B2_DECLARE_FIELD(3, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id)
		B2_DECLARE_FIELD(1, int32, main_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyCounterDungeonTicket)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCounterDungeonWeeklyReward)
		B2_DECLARE_FIELD(0, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDimension)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartDimension)
		B2_DECLARE_FIELD(0, int32, difficulty_level)
		B2_DECLARE_FIELD(1, int32, dungeon_id)
		B2_DECLARE_FIELD(2, int32, main_character_type)
		B2_DECLARE_FIELD(3, int32, total_power)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishDimension)
		B2_DECLARE_FIELD(0, int32, difficulty_level)
		B2_DECLARE_FIELD(1, FString, play_token)
		B2_DECLARE_FIELD(2, int32, play_result)
		B2_DECLARE_FIELD(3, int32, play_time_millis)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCreateGuild)
		B2_DECLARE_FIELD(0, FString, guild_name)
		B2_DECLARE_FIELD(1, int32, join_option)
		B2_DECLARE_FIELD(2, int32, mark_index)
		B2_DECLARE_FIELD(3, int32, mark_color)
		B2_DECLARE_FIELD(4, int32, mark_bg_index)
		B2_DECLARE_FIELD(5, int32, mark_bg_color)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskJoinGuild)
		B2_DECLARE_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLeaveGuild)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildDetailInfo)
		B2_DECLARE_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildMember)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDeleteGuild)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRecommendGuild)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildMemberCandidates)
		B2_DECLARE_FIELD(0, bool, need_friend_list)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestInviteGuild)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildInviteList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRejectInviteGuild)
		B2_DECLARE_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptInviteGuild)
		B2_DECLARE_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFindGuild)
		B2_DECLARE_FIELD(0, FString, guild_name)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestObtainGuildMaster)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestHandOverGuildMaster)
		B2_DECLARE_FIELD(0, int64, account_id);
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildRankingList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAppointSubGuildMaster)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismissSubGuildMaster)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKickGuildMember)
		B2_DECLARE_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateGuild)
		B2_DECLARE_FIELD(0, int32, join_option)
		B2_DECLARE_FIELD(1, int32, mark_index)
		B2_DECLARE_FIELD(2, int32, mark_color)
		B2_DECLARE_FIELD(3, int32, mark_bg_index)
		B2_DECLARE_FIELD(4, int32, mark_bg_color)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateGuildNotice)
		B2_DECLARE_FIELD(0, FString, notice)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelUpGuildSkill)
		B2_DECLARE_FIELD(0, int32, skill_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyGuildSkill)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMission)
		B2_DECLARE_FIELD(0, int32, mission_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveMissionReward)
		B2_DECLARE_FIELD(0, int32, mission_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveCompleteMissionReward)
		B2_DECLARE_FIELD(0, int32, mission_type)
		B2_DECLARE_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetWing)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceWing)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpgradeWing)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceWingOption)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, option_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMissionAlarmNotify)
		B2_DECLARE_FIELD(0, int32, completed_quest_id)
		B2_DECLARE_FIELD(1, int32, completed_quest_complete_step)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartHeroTower)
		B2_DECLARE_FIELD(0, int32, floor)
		B2_DECLARE_FIELD(1, int32, main_character_type)
		B2_DECLARE_FIELD(2, int32, tag_character_type)
		B2_DECLARE_FIELD(3, FString, play_token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishHeroTower)
		B2_DECLARE_FIELD(0, int32, floor)
		B2_DECLARE_FIELD(1, FString, play_token)
		B2_DECLARE_FIELD(2, int32, clear_time_sec)
		B2_DECLARE_FIELD(3, bool, is_success)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAbandonHeroTower)
		B2_DECLARE_FIELD(0, int32, floor)
		B2_DECLARE_FIELD(1, FString, play_token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceRelic)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, relic_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPromotionRelic)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, relic_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountRelic)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyHeroTowerTicket)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDrawShopLottery)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, draw_price_type)
		B2_DECLARE_FIELD(2, bool, continuous_draw)
		B2_DECLARE_FIELD(3, bool, is_draw_again)
		B2_DECLARE_FIELD(4, FString, last_lottery_key)
		B2_DECLARE_FIELD(5, bool, is_free_lottery)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardShopMileage)
		B2_DECLARE_FIELD(0, int32, shop_mileage_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetLotteryShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyShopProduct)
		B2_DECLARE_FIELD(0, int32, product_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPurchaseCostume)
		B2_DECLARE_FIELD(0, int32, product_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGeneralShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetCostumeShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMagicShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRenewalMagicShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPurchaseMagicShop)
		B2_DECLARE_FIELD(0, int32, id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetCollectionItem)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, equip_category)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardCollectionItem)
		B2_DECLARE_FIELD(0, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardCollectionSetItem)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, group_id)		
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMarketProduct)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPreparePurchase)
		B2_DECLARE_FIELD(0, int32, product_id)
		B2_DECLARE_FIELD(1, FString, market_product_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestVerifyPurchase)
		B2_DECLARE_FIELD(0, FString, market_product_id)
		B2_DECLARE_FIELD(1, FString, developer_payload)
		B2_DECLARE_FIELD(2, FString, purchase_data)
		B2_DECLARE_FIELD(3, FString, purchase_signature)
		B2_DECLARE_FIELD(4, bool, is_restore)
		B2_DECLARE_FIELD(5, FString, currency_code)
		B2_DECLARE_FIELD(6, float, price)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelPurchase)
		B2_DECLARE_FIELD(0, FString, market_product_id)
		B2_DECLARE_FIELD(1, FString, developer_payload)
		B2_DECLARE_FIELD(2, int32, cancel_reason)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishPurchase)
		B2_DECLARE_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRaidResult)
		B2_DECLARE_FIELD(0, FString, raid_token)
		B2_DECLARE_FIELD(1, int32, ranking)
		B2_DECLARE_FIELD(2, bool, is_bonus_player)
		B2_DECLARE_FIELD(3, bool, is_reward)
		B2_DECLARE_FIELD(4, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDailyAttendance)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEventAttendance)
		B2_DECLARE_FIELD(0, int32, eventId)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBeginnerAttendance)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateDailyPlayTime)
		B2_DECLARE_FIELD(0, int32, play_time_in_sec)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveDailyPlayTimeReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestJoinGuildBattle)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildBattle)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartGuildBattle)
		B2_DECLARE_FIELD(0, int64, target_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishGuildBattle)
		B2_DECLARE_FIELD(0, int64, target_account_id)
		B2_DECLARE_FIELD(1, FString, battle_token)
		B2_DECLARE_FIELD(2, int32, result)
		B2_DECLARE_FIELD(3, int32, round_win_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildBattleEntry)
		B2_DECLARE_FIELD(0, int32, slot_1)
		B2_DECLARE_FIELD(1, int32, slot_2)
		B2_DECLARE_FIELD(2, int32, slot_3)
		B2_DECLARE_FIELD(3, int32, slot_4)
		B2_DECLARE_FIELD(4, int32, slot_5)
		B2_DECLARE_FIELD(5, int32, slot_6)
		B2_DECLARE_FIELD(6, int32, slot_7)
		B2_DECLARE_FIELD(7, int32, slot_8)
	DECLARE_B2MESSAGEINFO_REQUEST_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildBattleTurnResult)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildBattleTurnReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildBattleSeasonReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildBattleHistory)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetWingVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetHelmetVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCostumeVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, costume_type)
		B2_DECLARE_FIELD(2, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRaid)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRaidResurrection)		
		B2_DECLARE_FIELD(0, FString, token)
		B2_DECLARE_FIELD(1, int, selected_buff_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeQuestId)
		B2_DECLARE_FIELD(0, int32, slot_type)
		B2_DECLARE_FIELD(1, int32, quest_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeQuestCount)
		B2_DECLARE_FIELD(0, int32, slot_type)
		B2_DECLARE_FIELD(1, int32, count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultMatchmaking)
		B2_DECLARE_FIELD(0, int32, battle_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelAssaultMatchmaking)
	B2_DECLARE_FIELD(0, FString, token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultResult)
		B2_DECLARE_FIELD(0, FString, token)
		B2_DECLARE_FIELD(1, int32, match_result)
		B2_DECLARE_FIELD(2, bool, is_mvp_player)
		B2_DECLARE_FIELD(3, bool, is_ghost_user)
		B2_DECLARE_FIELD(4, B2AssaultBattleInfoPtr, battle_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultResurrection)
	B2_DECLARE_FIELD(0, FString, token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckPvPMatchPointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckTeamMatchPointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFriendRankings)
	B2_DECLARE_FIELD(0, int32, opponent_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemRandomOption)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGenerateItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, price_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestApplyItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDiscardItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckRedDot)
		B2_DECLARE_FIELD(0, TArray<int32>, red_dot_hint)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseDarkCrystalPaid)
		B2_DECLARE_FIELD(0, int32, dark_crystal_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, slot_num)
		B2_DECLARE_FIELD(2, int32, random_option_id)
		B2_DECLARE_FIELD(3, float, random_option_value)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyItemNewFlag)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyCostumeNewFlag)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartRaid)
		B2_DECLARE_FIELD(0, int32, battle_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetContentsOpenStatus)
		B2_DECLARE_FIELD(0, int32, link_key)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelRewardBoxInfo)
		B2_DECLARE_FIELD(0, int32, duel_mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelRewardBoxGainableList)
		B2_DECLARE_FIELD(0, int32, duel_mode)
		B2_DECLARE_FIELD(1, int32, grade)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenDuelRewardBox)
		B2_DECLARE_FIELD(0, int32, duel_mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveAssaultMvpReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountAether)
		B2_DECLARE_FIELD(0, int32, aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMountAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnmountAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSelectAetherDefenseOption)
		B2_DECLARE_FIELD(0, int32, set_option_id)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSelectAetherUnitySkillOption)
		B2_DECLARE_FIELD(0, int32, set_option_id)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExtendAetherInventory)
		B2_DECLARE_FIELD(0, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceAether)
		B2_DECLARE_FIELD(0, int64, aether_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyAetherNewFlag)
		B2_DECLARE_FIELD(0, int32, aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetTutorialProgress)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, bool, is_item_prepared)
		B2_DECLARE_FIELD(2, bool, is_lottery_drawn)
		B2_DECLARE_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSkipTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDrawTutorialItem)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareTutorial)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveUnitySkillReward)
		B2_DECLARE_FIELD(0, int32, main_character_type)
		B2_DECLARE_FIELD(1, int32, unity_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetUnitySkillMission)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAwakenUnitySkillMission)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, mission_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAwakenUnitySkill)
		B2_DECLARE_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelModeInfo)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetChallengeModeInfo)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetHallOfFame)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, bool, need_user_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPraiseTargetRanker)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, int64, account_id)
		B2_DECLARE_FIELD(2, int32, ranking)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveRestReward)
		B2_DECLARE_FIELD(0, int32, reward_id)
		B2_DECLARE_FIELD(1, bool, isDouble)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckModeOpen)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveTutorialReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckReachPower)
		B2_DECLARE_FIELD(0, TArray<B2CharacterPowerPtr>, character_power)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTrackingPowerDetail)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, item_preset_id)
		B2_DECLARE_FIELD(2, int32, skill_slot)
		B2_DECLARE_FIELD(3, TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoMessageProcessing)
		B2_DECLARE_FIELD(0, FString, message_box_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoCouponUse)
		B2_DECLARE_FIELD(0, FString, coupon_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoAccountConversionPrepare)
		B2_DECLARE_FIELD(0, FString, idp_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoAccountConversionComplete)
		B2_DECLARE_FIELD(0, FString, idp_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDonationEvent)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeDonation)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, bool, is_bundle)
		B2_DECLARE_FIELD(2, bool, is_again)
		B2_DECLARE_FIELD(3, bool, is_free)
		B2_DECLARE_FIELD(4, FString, last_donation_key)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDonationPointReward)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, step)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFairy)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelupFairy)
		B2_DECLARE_FIELD(0, int32, fairy_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBlessFairy)
		B2_DECLARE_FIELD(0, int32, fairy_type)
		B2_DECLARE_FIELD(1, int32, bless_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveFairyGift)
		B2_DECLARE_FIELD(0, int32, fairy_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckChargePoints)
		B2_DECLARE_FIELD(0, bool, check_blade_point)
		B2_DECLARE_FIELD(1, bool, check_pvp_match_point)
		B2_DECLARE_FIELD(2, bool, check_team_match_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFeaturedMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetEvent)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceivePointEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveLevelUpEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetSeasonMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardSeasonMission)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, day)
		B2_DECLARE_FIELD(2, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAttendSeasonMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveStageClearEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyHotTimeBuff)
		B2_DECLARE_FIELD(0, int32, buff_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardFlatRatePackageBonus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetPackageShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetPeriodPackageState)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardLevelUpPackage)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardReachLevelPackage)
		B2_DECLARE_FIELD(0, int32, package_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveUpQuest)
		B2_DECLARE_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetContentsModeState)
		B2_DECLARE_FIELD(0, int32, mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardPointShopEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReportUser)
		B2_DECLARE_FIELD(0, int64, target_account_id)
		B2_DECLARE_FIELD(1, int32, mode_type)
		B2_DECLARE_FIELD(2, int32, reason_type)
		B2_DECLARE_FIELD(3, FString, explain)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBlockChat)
		B2_DECLARE_FIELD(0, int64, block_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnblockChat)
		B2_DECLARE_FIELD(0, int64, unblock_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetBlockChatList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestClientStateReport)
		B2_DECLARE_FIELD(0, FString, category)
		B2_DECLARE_FIELD(1, FString, event)
		B2_DECLARE_FIELD(2, FString, label)
		B2_DECLARE_FIELD(3, FString, detail_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardTenLotteryEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveGemPurchaseEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardSpendGemEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardTenLotterySavingsEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRollDiceEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, bool, is_free_dice)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyDiceEventPoint)
		B2_DECLARE_FIELD(0, int32, event_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardDiceEventMileage)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	// REPLACEMENT POINT: MESSAGE_INFO HEADER REQUEST
	// NEVER DELETE LINE ABOVE

	// for console command

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDeleteAccount)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddItemToInventory)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, int32, item_quality)
		B2_DECLARE_FIELD(2, int32, item_amount)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddCostumeToInventory)
		B2_DECLARE_FIELD(0, int32, template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddTotemToInventory)
		B2_DECLARE_FIELD(0, int32, template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseCashPaid)
		B2_DECLARE_FIELD(0, int32, cash_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseMoney)
		B2_DECLARE_FIELD(0, int32, money)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseSocialPoint)
		B2_DECLARE_FIELD(0, int32, social_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetItemLevelup)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, float, exp)
		B2_DECLARE_FIELD(3, int32, enhance_level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterLevelExp)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, float, exp)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterRankInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, rank)
		B2_DECLARE_FIELD(2, int32, nodeIndex)
		B2_DECLARE_FIELD(3, bool, nodeState)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterRelic)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, relic_id)
		B2_DECLARE_FIELD(2, int32, grade)
		B2_DECLARE_FIELD(3, int32, enhance_level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetBladePoint)
		B2_DECLARE_FIELD(0, int32, blade_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseRankPromotionPoint)
		B2_DECLARE_FIELD(0, int32, rank_promotion_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddMail)
		B2_DECLARE_FIELD(0, int32, mail_category)
		B2_DECLARE_FIELD(1, int32, attach_type)
		B2_DECLARE_FIELD(2, int32, source_type)
		B2_DECLARE_FIELD(3, int32, sender_type)
		B2_DECLARE_FIELD(4, int32, amount)
		B2_DECLARE_FIELD(5, int32, item_template_id)
		B2_DECLARE_FIELD(6, int32, general_lottery_id)
		B2_DECLARE_FIELD(7, int32, fixed_grade_lottery_id)
		B2_DECLARE_FIELD(8, int32, selective_lottery_id)
		B2_DECLARE_FIELD(9, int32, selective_item_lottery_id)
		B2_DECLARE_FIELD(10, int32, prefix_selective_lottery_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeRanking)
		B2_DECLARE_FIELD(0, int32, match_type)
		B2_DECLARE_FIELD(1, int32, ranking)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceClearStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetQuestState)
		B2_DECLARE_FIELD(0, int32, quest_id)
		B2_DECLARE_FIELD(1, int32, quest_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseHeroPiece)
		B2_DECLARE_FIELD(0, int32, hero_piece)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceClearHeroTower)
		B2_DECLARE_FIELD(0, int32, floor)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceStartCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareGuild)
		B2_DECLARE_FIELD(0, int32, member_count)
		B2_DECLARE_FIELD(1, int32, add_guild_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareSweepHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildBattleState)
		B2_DECLARE_FIELD(0, bool, is_new_season)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetAttendanceTime)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetEventAttendance)
		B2_DECLARE_FIELD(0, bool, is_only_attendance_time)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetDailyPlayTimeRecord)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetTeamMatchPoint)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareFriend)
		B2_DECLARE_FIELD(0, int32, friend_count)
		B2_DECLARE_FIELD(1, int32, job_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildLevelExp)
		B2_DECLARE_FIELD(0, int32, level)
		B2_DECLARE_FIELD(1, int32, exp)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddAetherToInventory)
		B2_DECLARE_FIELD(0, int32, aether_type)
		B2_DECLARE_FIELD(1, int32, slot)
		B2_DECLARE_FIELD(2, int32, grade)
		B2_DECLARE_FIELD(3, int32, tier_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceUpdateAether)
		B2_DECLARE_FIELD(0, int64, aether_id)
		B2_DECLARE_FIELD(1, int32, type)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, grade)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetMatchSeasonReward)
		B2_DECLARE_FIELD(0, int32, match_type)
		B2_DECLARE_FIELD(1, int32, ranking)
		B2_DECLARE_FIELD(2, int32, point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetStageBoostCount)
		B2_DECLARE_FIELD(0, int32, stage_boost_ticket)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSkipMatchTutorial)
		B2_DECLARE_FIELD(0, int32, match_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetDebugMode)
		B2_DECLARE_FIELD(0, bool, skip_keep_alive_check)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestServerCmd)
		B2_DECLARE_FIELD(0, FString, command)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTrace)
		B2_DECLARE_FIELD(0, FString, tag)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class B2NETWORK_API FB2MessageInfoResponse : public FB2MessageInfo
	{
	public :
		FB2MessageInfoResponse(std::string className) : FB2MessageInfo(className) {}

		virtual void DoCallback(FB2MessageInfoPtr InParam) override {};
		virtual FB2MessageCallback GetSuccessCallback() override { return nullptr; }
		virtual FB2MessageCallback GetFailCallback() override { return nullptr; }
	};

	//////////////////////////////////////////////////////////////////////////
#define DECLARE_B2MESSAGEINFO_RESPONSE(ClassName)										\
	class ClassName;																	\
	using ClassName##Ptr = std::shared_ptr<ClassName>;									\
	class B2NETWORK_API ClassName : public FB2MessageInfoResponse						\
	{																					\
	public:																				\
		ClassName() : FB2MessageInfoResponse(#ClassName) {};							\
		using type_list0 = TU::TypeList<>;

#define DECLARE_B2MESSAGEINFO_RESPONSE_END(count)										\
		FINALIZE_TYPELIST(count)														\
		static const uint32 field_num = count;											\
	};

#define DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()									\
		static const uint32 field_num = 0;												\
	};

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccount)
		B2_DECLARE_FIELD(0, B2AccountInfoPtr, account)
		B2_DECLARE_FIELD(1, TArray<B2CharacterServerInfoPtr>, characters)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, equips)
		B2_DECLARE_FIELD(3, TArray<B2CostumeServerInfoPtr>, equip_costumes)
		B2_DECLARE_FIELD(4, TArray<B2TotemServerInfoPtr>, equip_totems)
		B2_DECLARE_FIELD(5, TArray<B2ItemPresetPtr>, item_presets)
		B2_DECLARE_FIELD(6, FString, guild_name)
		B2_DECLARE_FIELD(7, int32, mark_index)
		B2_DECLARE_FIELD(8, int32, mark_color)
		B2_DECLARE_FIELD(9, int32, mark_bg_index)
		B2_DECLARE_FIELD(10, int32, mark_bg_color)
		B2_DECLARE_FIELD(11, int32, my_rank)
		B2_DECLARE_FIELD(12, TArray<B2RelicPtr>, relics)
		B2_DECLARE_FIELD(13, TArray<B2WingOptionInfoPtr>, wing_options)
		B2_DECLARE_FIELD(14, TArray<B2GuildSkillPtr>, guild_skills)
		B2_DECLARE_FIELD(15, TArray<B2EventAttendanceStatusPtr>, event_attendance_statuses)
		B2_DECLARE_FIELD(16, int32, aether_inventory_slot_count)
		B2_DECLARE_FIELD(17, TArray<B2AetherServerInfoPtr>, mount_aethers)
		B2_DECLARE_FIELD(18, TArray<B2FairyStatusPtr>, fairies)
		B2_DECLARE_FIELD(19, bool, is_all_tutorial_finished)
		B2_DECLARE_FIELD(20, TArray<B2UnitySkillMissionPtr>, unitySkills)
		B2_DECLARE_FIELD(21, TArray<B2RollingBannerPtr>, rolling_banners)
		B2_DECLARE_FIELD(22, B2BeginnerAttendanceStatusPtr, beginner_attendance_status)
		B2_DECLARE_FIELD(23, TArray<B2UnitySkillAwakenMissionPtr>, unity_awaken_missions)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(24)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindAccount)
		B2_DECLARE_FIELD(0, TArray<B2FindAccountInfoPtr>, account_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountItem)
		B2_DECLARE_FIELD(0, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountCostume)
		B2_DECLARE_FIELD(0, TArray<B2CostumeServerInfoPtr>, costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMasterData)
		B2_DECLARE_FIELD(0, B2ConfigDataPtr, config_data)
		B2_DECLARE_FIELD(1, TArray<B2mdSkillInfoPtr>, md_skill_infos)
		B2_DECLARE_FIELD(2, TArray<B2mdSkillOptionPtr>, md_skill_options)
		B2_DECLARE_FIELD(3, TArray<B2mdItemLevelupExpFactorPtr>, md_item_levelup_exp_factors)
		B2_DECLARE_FIELD(4, TArray<B2mdItemOptionAddValuePtr>, md_item_option_add_values)
		B2_DECLARE_FIELD(5, TArray<B2mdItemLevelupCostPtr>, md_item_levelup_costs)
		B2_DECLARE_FIELD(6, TArray<B2mdStageInfoPtr>, md_stage_infos)
        B2_DECLARE_FIELD(7, TArray<B2mdResurrectionInfoPtr>, md_resurrection_infos)
		B2_DECLARE_FIELD(8, TArray<B2mdTeamBattleFormationInfoPtr>, md_formation_infos)
		B2_DECLARE_FIELD(9, TArray<B2mdRankInfoPtr>, md_rank_infos)
		B2_DECLARE_FIELD(10, TArray<B2mdDailyMatchRewardPtr>, md_daily_match_rewards)
		B2_DECLARE_FIELD(11, TArray<B2mdMatchRankingRewardPtr>, md_pvp_ranking_rewards)
		B2_DECLARE_FIELD(12, TArray<B2mdMatchRankingRewardPtr>, md_team_ranking_rewards)
		B2_DECLARE_FIELD(13, TArray<B2mdCounterDungeonPtr>, md_counter_dungeons)
		B2_DECLARE_FIELD(14, TArray<B2mdCounterDungeonPeriodPtr>, md_counter_dungeon_periods)
		B2_DECLARE_FIELD(15, TArray<B2mdCounterDungeonWeekRewardPtr>, md_counter_dungeon_week_rewards)
		B2_DECLARE_FIELD(16, TArray<B2mdMainQuestInfoPtr>, md_main_quest_infos)
		B2_DECLARE_FIELD(17, TArray<B2mdDailyQuestInfoPtr>, md_daily_quest_infos)
		B2_DECLARE_FIELD(18, TArray<B2mdRewardInfoPtr>, md_reward_infos)
		B2_DECLARE_FIELD(19, TArray<B2mdMissionInfoPtr>, md_mission_infos)
		B2_DECLARE_FIELD(20, TArray<B2mdSerialMissionInfoPtr>, md_serial_mission_infos)
		B2_DECLARE_FIELD(21, TArray<B2mdMissionCompletePointInfoPtr>, md_mission_complete_point_infos)
		B2_DECLARE_FIELD(22, TArray<B2mdWingEnhancePtr>, md_wing_enhance_infos)
		B2_DECLARE_FIELD(23, TArray<B2mdWingUpgradePtr>, md_wing_upgrade_infos)
		B2_DECLARE_FIELD(24, TArray<B2mdWingOptionValuePtr>, md_wing_option_value_infos)
		B2_DECLARE_FIELD(25, TArray<B2mdWingOptionOpenPtr>, md_wing_option_open_infos)
		B2_DECLARE_FIELD(26, TArray<B2mdWingOptionMaxLevelPtr>, md_wing_option_max_level_infos)
		B2_DECLARE_FIELD(27, TArray<B2mdHeroTowerFloorPtr>, md_hero_tower_floors)
		B2_DECLARE_FIELD(28, TArray<B2mdRelicInfoPtr>, md_relic_infos)
		B2_DECLARE_FIELD(29, TArray<B2mdRelicGradeInfoPtr>, md_relic_grade_infos)
		B2_DECLARE_FIELD(30, TArray<B2mdRelicLevelInfoPtr>, md_relic_level_infos)
		B2_DECLARE_FIELD(31, TArray<B2mdShopLotteryPricePtr>, md_shop_lottery_prices)
		B2_DECLARE_FIELD(32, TArray<B2mdShopLotteryPossibleGradePtr>, md_shop_lottery_possible_grades)
		B2_DECLARE_FIELD(33, TArray<B2mdShopLotterySpecialGradePtr>, md_shop_lottery_special_grades)
		B2_DECLARE_FIELD(34, TArray<B2mdShopProductPtr>, md_shop_products)
		B2_DECLARE_FIELD(35, TArray<B2mdShopPackagePtr>, md_shop_packages)
		B2_DECLARE_FIELD(36, TArray<B2mdShopMileageRewardPtr>, md_shop_mileage_rewards)
		B2_DECLARE_FIELD(37, TArray<B2mdShopLotteryDailyLimitPtr>, md_shop_lottery_daily_limits)
		B2_DECLARE_FIELD(38, TArray<B2mdCollectionRewardPtr>, md_collection_rewards)
		B2_DECLARE_FIELD(39, TArray<B2mdDailyAttendanceInfoPtr>, md_daily_atttendances)
		B2_DECLARE_FIELD(40, TArray<B2mdGuildBattleResultRewardPtr>, md_guild_battle_result_rewards)
		B2_DECLARE_FIELD(41, TArray<B2mdGuildBattleSeasonRewardPtr>, md_guild_battle_season_rewards)
		B2_DECLARE_FIELD(42, TArray<B2mdRaidInfoPtr>, md_raid_infos)
		B2_DECLARE_FIELD(43, TArray<B2mdMonsterInfoPtr>, md_monster_infos)
		B2_DECLARE_FIELD(44, TArray<B2mdGeneralLotteryPtr>, md_general_lotteries)
		B2_DECLARE_FIELD(45, TArray<B2mdSelectiveLotteryPtr>, md_selective_lotteries)
		B2_DECLARE_FIELD(46, TArray<B2mdFixedGradeLotteryPtr>, md_fixed_grade_lotteries)
		B2_DECLARE_FIELD(47, TArray<B2mdItemEnhanceCostPtr>, md_item_enhance_costs)
		B2_DECLARE_FIELD(48, TArray<B2mdItemEnhanceEffectInfoPtr>, md_item_enhance_effect_infos)
		B2_DECLARE_FIELD(49, TArray<B2mdAdditionalMatchCostPtr>, md_additional_match_cost_infos)
		B2_DECLARE_FIELD(50, TArray<B2mdGuildSkillPtr>, md_guild_skills)
		B2_DECLARE_FIELD(51, TArray<B2mdGuildExpPtr>, md_guild_exps)
		B2_DECLARE_FIELD(52, TArray<B2mdStageFixedDropPtr>, md_stage_fixed_drops)
		B2_DECLARE_FIELD(53, TArray<B2mdAssaultVictoryRewardInfoPtr>, md_assault_victory_reward_infos)
		B2_DECLARE_FIELD(54, TArray<B2mdAssaultMvpRewardInfoPtr>, md_assault_mvp_reward_infos)
		B2_DECLARE_FIELD(55, TArray<B2mdEventAttendanceRewardPtr>, md_event_attendance_rewards)
		B2_DECLARE_FIELD(56, TArray<B2mdDailyPlayTimeRewardPtr>, md_daily_play_time_rewards)
		B2_DECLARE_FIELD(57, TArray<B2mdMatchRankingRewardPtr>, md_assault_ranking_rewards)
		B2_DECLARE_FIELD(58, TArray<B2mdTutorialInfoPtr>, md_tutorial_infos)
		B2_DECLARE_FIELD(59, TArray<B2mdDuelModeRewardPtr>, md_duel_mode_reward)
		B2_DECLARE_FIELD(60, TArray<B2mdRaidExpPtr>, md_raid_exp)
		B2_DECLARE_FIELD(61, TArray<B2mdAetherOptionInfoPtr>, md_aether_option_infos)
		B2_DECLARE_FIELD(62, TArray<B2mdAetherSlotInfoPtr>, md_aether_slot_infos)
		B2_DECLARE_FIELD(63, TArray<B2mdAetherEnhanceInfoPtr>, md_aether_enhance_infos)
		B2_DECLARE_FIELD(64, TArray<B2mdAetherEnhanceOptionInfoPtr>, md_aether_enhance_option_infos)
		B2_DECLARE_FIELD(65, TArray<B2mdAetherSetOptionInfoPtr>, md_aether_set_option_infos)
		B2_DECLARE_FIELD(66, TArray<B2mdGuildSupportItemPtr>, md_guild_support_items)
		B2_DECLARE_FIELD(67, TArray<B2mdGuildBattleScheduleInfoPtr>, md_guild_battle_schedule_infos)
		B2_DECLARE_FIELD(68, TArray<B2mdUnitySkillMissionPtr>, md_unity_skill_missions)
		B2_DECLARE_FIELD(69, TArray<B2mdRestRewardPtr>, md_rest_rewards)
		B2_DECLARE_FIELD(70, TArray<B2mdGuildMercenaryPtr>, md_guild_mercenaries)
		B2_DECLARE_FIELD(71, TArray<B2mdGuildMercenarySkillPtr>, md_guild_mercenary_skills)
		B2_DECLARE_FIELD(72, TArray<B2mdGuildMercenaryMasteryPtr>, md_guild_mercenary_masteries)
		B2_DECLARE_FIELD(73, TArray<B2mdGuildMercenaryDonationPtr>, md_guild_mercenary_donations)
		B2_DECLARE_FIELD(74, TArray<B2mdItemInfoPtr>, md_item_infos)
		B2_DECLARE_FIELD(75, TArray<B2mdItemSetPtr>, md_item_set_infos)
		B2_DECLARE_FIELD(76, TArray<B2mdPointEventPtr>, md_point_events)
		B2_DECLARE_FIELD(77, TArray<B2mdPointEventRewardPtr>, md_point_event_rewards)
		B2_DECLARE_FIELD(78, TArray<B2mdLevelUpEventPtr>, md_level_events)
		B2_DECLARE_FIELD(79, TArray<B2mdSeasonMissionEventPtr>, md_season_mission_events)
		B2_DECLARE_FIELD(80, TArray<B2mdKakaoFriendInviteRewardPtr>, md_kakao_friend_invite_rewards)
		B2_DECLARE_FIELD(81, TArray<B2mdHotTimeProductPtr>, md_hot_time_products)
		B2_DECLARE_FIELD(82, TArray<B2mdShopCashBonusPtr>, md_shop_cash_bonus_infos)
		B2_DECLARE_FIELD(83, TArray<B2mdShopFundsSalePtr>, md_shop_funds_sale_infos)
		B2_DECLARE_FIELD(84, TArray<B2mdShopLotterySalePtr>, md_shop_lottery_sale_infos)
		B2_DECLARE_FIELD(85, TArray<B2mdShopPackageProductPtr>, md_shop_package_products)
		B2_DECLARE_FIELD(86, TArray<B2mdSkillLevelUpPtr>, md_skill_levelups)
		B2_DECLARE_FIELD(87, TArray<B2mdDuelModeSettleSchedulePtr>, md_duel_mode_settle_schedules)
		B2_DECLARE_FIELD(88, TArray<B2mdShopLevelUpPackagePtr>, md_shop_level_up_packages)
		B2_DECLARE_FIELD(89, TArray<B2mdShopReachLevelPackagePtr>, md_shop_reach_level_packages)
		B2_DECLARE_FIELD(90, TArray<B2mdShopFlatRatePackageBonusPtr>, md_shop_flat_rate_package_bonus)
		B2_DECLARE_FIELD(91, TArray<B2mdShopPackageTabPtr>, md_shop_package_tab_infos)
		B2_DECLARE_FIELD(92, TArray<B2mdUnitySkillOptionPtr>, md_unity_skill_options)
		B2_DECLARE_FIELD(93, TArray<B2mdStageClearEventPtr>, md_stage_clear_events)
		B2_DECLARE_FIELD(94, TArray<B2mdWebUrlPtr>, md_web_urls)
		B2_DECLARE_FIELD(95, TArray<B2mdTenLotteryEventPtr>, md_ten_lottery_events)
		B2_DECLARE_FIELD(96, TArray<B2mdMagicShopProductPtr>, md_magic_shop_product)
		B2_DECLARE_FIELD(97, TArray<B2mdMagicShopRenewalCostPtr>, md_magic_shop_renewal_cost)
		B2_DECLARE_FIELD(98, TArray<B2mdGemPurchaseEventRewardPtr>, md_gem_purchase_event_rewards)
		B2_DECLARE_FIELD(99, TArray<B2mdSpendGemEventPtr>, md_spend_gem_event_rewards)
		B2_DECLARE_FIELD(100, TArray<B2mdEventAttendancePtr>, md_event_attendances)
		B2_DECLARE_FIELD(101, TArray<B2mdEventInfoPtr>, md_event_infos)
		B2_DECLARE_FIELD(102, TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
		B2_DECLARE_FIELD(103, TArray<B2mdTenLotterySavingsEventPtr>, md_ten_lottery_savings_events)
		B2_DECLARE_FIELD(104, TArray<B2mdItemLevelupFactorPtr>, md_item_levelup_factors)
		B2_DECLARE_FIELD(105, TArray<B2mdPrefixSelectiveLotteryPtr>, md_prefix_selective_lotteries)
		B2_DECLARE_FIELD(106, TArray<B2mdSelectItemOptionPtr>, md_select_item_option)
		B2_DECLARE_FIELD(107, TArray<B2mdDiceEventMissionPtr>, md_dice_event_missions)
		B2_DECLARE_FIELD(108, TArray<B2mdDiceEventCellPtr>, md_dice_event_cells)
		B2_DECLARE_FIELD(109, TArray<B2mdDiceEventFinishRewardPtr>, md_dice_event_finish_rewards)
		B2_DECLARE_FIELD(110, TArray<B2mdItemSurpassLevelPtr>, md_item_surpass_levels)
		B2_DECLARE_FIELD(111, TArray<B2mdDiceEventBuyPointPtr>, md_dice_event_buy_points)
		B2_DECLARE_FIELD(112, TArray<B2mdDiceEventMileagePtr>, md_dice_event_mileages)
		B2_DECLARE_FIELD(113, TArray<B2mdItemDismantleInfoPtr>, md_item_dismantle_infos)
		B2_DECLARE_FIELD(114, TArray<B2mdFairyInfoPtr>, md_fairy_infos)
		B2_DECLARE_FIELD(115, TArray<B2mdFairyLevelupCostPtr>, md_fairy_levelup_costs)
		B2_DECLARE_FIELD(116, TArray<B2mdFairyOpenInfoPtr>, md_fairy_open_infos)
		B2_DECLARE_FIELD(117, TArray<B2mdCostumeInfoPtr>, md_costume_infos)
		B2_DECLARE_FIELD(118, TArray<B2mdDonationPointRankingRewardInfoPtr>, md_donation_point_ranking_reward_infos)
		B2_DECLARE_FIELD(119, TArray<B2mdDonationBundleFixedRewardInfoPtr>, md_donation_bundle_fixed_reward_infos)
		B2_DECLARE_FIELD(120, TArray<B2mdDonationPointStepRewardInfoPtr>, md_donation_point_step_reward_infos)
		B2_DECLARE_FIELD(121, TArray<B2mdUnitySkillAwakenMissionPtr>, md_unity_skill_awaken_missions)
		B2_DECLARE_FIELD(122, B2mdExtendMessagePtr, md_extend_massages)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(123)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetLatestEventData)
		B2_DECLARE_FIELD(0, int64, last_update_time)
		B2_DECLARE_FIELD(1, TArray<B2mdEventInfoPtr>, md_event_infos)
		B2_DECLARE_FIELD(2, TArray<B2mdEventAttendancePtr>, md_event_attendances)
		B2_DECLARE_FIELD(3, TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
		B2_DECLARE_FIELD(4, TArray<B2mdDonationEventInfoPtr>, md_donation_event_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSignUp)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellItem)
		B2_DECLARE_FIELD(0, int32, money_total)
		B2_DECLARE_FIELD(1, TArray<int64>, sell_item_ids)
		B2_DECLARE_FIELD(2, int32, sell_price)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellConsumableItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, updated_item)
		B2_DECLARE_FIELD(2, int64, deleted_item_id)
		B2_DECLARE_FIELD(3, int32, sell_price)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConvertData)
		B2_DECLARE_FIELD(0, int32, templateId)
		B2_DECLARE_FIELD(1, int32, exchange_remain_count_gold)
		B2_DECLARE_FIELD(2, int32, exchange_remain_count_gem)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConvertItem)
		B2_DECLARE_FIELD(0, b2::protocol::commondata::ExchangePriceType, exchangetype)
		B2_DECLARE_FIELD(1, int32, currentGold)
		B2_DECLARE_FIELD(2, int32, currentCash)
		B2_DECLARE_FIELD(3, B2ItemServerInfoPtr, exchangeItem)
		B2_DECLARE_FIELD(4, int32, exchangeItemAmount)
		B2_DECLARE_FIELD(5, int32, costCash)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipItem)
		B2_DECLARE_FIELD(0, int32, preset_id)
		B2_DECLARE_FIELD(1, TArray<int64>, equiped_item_ids)
		B2_DECLARE_FIELD(2, TArray<int64>, unequiped_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipItem)
		B2_DECLARE_FIELD(0, int32, preset_id)
		B2_DECLARE_FIELD(1, TArray<int64>, unequiped_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipCostume)
		B2_DECLARE_FIELD(0, TArray<int64>, equiped_costume_ids)
		B2_DECLARE_FIELD(1, TArray<int64>, unequiped_costume_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipCostume)
		B2_DECLARE_FIELD(0, TArray<int64>, unequiped_costume_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceCostume)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, B2CostumeServerInfoPtr, enhanced_costume)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_material_items)
		B2_DECLARE_FIELD(3, TArray<int64>, deleted_material_item_ids)
		B2_DECLARE_FIELD(4, int32, current_enhance_level)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleCostumes)
		B2_DECLARE_FIELD(0, TArray<int64>, dismantled_costume_ids)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, extracted_items)
		B2_DECLARE_FIELD(2, TArray<int32>, extracted_item_change_amount)
		B2_DECLARE_FIELD(3, B2ItemServerInfoPtr, restore_item)
		B2_DECLARE_FIELD(4, int32, restore_items_change_amount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockCostumes)
		B2_DECLARE_FIELD(0, TArray<B2CostumeServerInfoPtr>, locked_costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockCostumes)
		B2_DECLARE_FIELD(0, TArray<B2CostumeServerInfoPtr>, unlocked_costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockItem)
		B2_DECLARE_FIELD(0, TArray<B2ItemServerInfoPtr>, locked_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockItem)
		B2_DECLARE_FIELD(0, TArray<B2ItemServerInfoPtr>, unlocked_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExtendInventory)
		B2_DECLARE_FIELD(0, B2CharacterServerInfoPtr, character)
		B2_DECLARE_FIELD(1, int32, cash)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelupItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, int32, money_cost)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, main_item)
		B2_DECLARE_FIELD(3, TArray<int64>, item_ids)
		B2_DECLARE_FIELD(4, bool, jackpoted)
		B2_DECLARE_FIELD(5, bool, great_jackpoted)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, enhanced_item)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_enhance_scroll_items)
		B2_DECLARE_FIELD(3, TArray<int64>, deleted_enhance_scroll_item_ids)
		B2_DECLARE_FIELD(4, TArray<B2ItemServerInfoPtr>, changed_protection_scroll_items)
		B2_DECLARE_FIELD(5, TArray<int64>, deleted_protection_scroll_item_ids)
		B2_DECLARE_FIELD(6, int32, changed_enhance_level)
		B2_DECLARE_FIELD(7, bool, is_success)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleItems)
		B2_DECLARE_FIELD(0, TArray<int64>, dismantled_item_ids)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, extracted_items)
		B2_DECLARE_FIELD(2, TArray<int32>, extracted_item_change_amount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExchangeItemEnhanceLevel)
		B2_DECLARE_FIELD(0, int32, current_dark_crystal)
		B2_DECLARE_FIELD(1, int32, current_gem)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, item_data_1)
		B2_DECLARE_FIELD(3, B2ItemServerInfoPtr, item_data_2)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenSealSlotItem)
		B2_DECLARE_FIELD(0, int32, current_gold)
		B2_DECLARE_FIELD(1, int32, current_gem)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, slot_opened_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCarveSealItem)
		B2_DECLARE_FIELD(0, int32, current_gold)
		B2_DECLARE_FIELD(1, int32, current_gem)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, sealed_item)
		B2_DECLARE_FIELD(3, TArray<B2ItemServerInfoPtr>, updated_material_items)
		B2_DECLARE_FIELD(4, TArray<int64>, deleted_material_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSurpassItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, int32, money_cost)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, main_item)
		B2_DECLARE_FIELD(3, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpgradeItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, int32, money_cost)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, main_item)
		B2_DECLARE_FIELD(3, TArray<B2ItemServerInfoPtr>, upgrade_stone_items)
		B2_DECLARE_FIELD(4, int32, use_upgrade_stone_count)
		B2_DECLARE_FIELD(5, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseComposeItem)
		B2_DECLARE_FIELD(0, int32, current_money)
		B2_DECLARE_FIELD(1, int32, money_cost)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, new_composed_item)
		B2_DECLARE_FIELD(3, int64, deleted_main_item_id)
		B2_DECLARE_FIELD(4, int64, deleted_sub_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseChangeItemCharacterType)
		B2_DECLARE_FIELD(0, B2ItemServerInfoPtr, updated_item)
		B2_DECLARE_FIELD(1, int32, current_dark_crystal)
		B2_DECLARE_FIELD(2, int32, dark_crystal_cost)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetChangeItemCharacterTypeCost)
		B2_DECLARE_FIELD(0, int32, dark_crystal_cost)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, change_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartStage)
		B2_DECLARE_FIELD(0, FString, token)
		B2_DECLARE_FIELD(1, int32, drop_gold)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, drop_items)
		B2_DECLARE_FIELD(3, int32, drop_gold_by_gold_monster)
		B2_DECLARE_FIELD(4, int32, max_resurrection_count)
		B2_DECLARE_FIELD(5, int32, need_cash_for_resurrection)
		B2_DECLARE_FIELD(6, B2AccountInfoPtr, account);
		B2_DECLARE_FIELD(7, int32, selected_option_flags);
		B2_DECLARE_FIELD(8, TArray<B2DropDataPtr>, drop_datas);
		B2_DECLARE_FIELD(9, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
		B2_DECLARE_FIELD(10, int32, quest_item_id);
		B2_DECLARE_FIELD(11, int32, quest_item_count);
		B2_DECLARE_FIELD(12, int32, current_aether_count);
		B2_DECLARE_FIELD(13, int32, current_aether_max_count);
		B2_DECLARE_FIELD(14, int32, qte_bonus_attack);
		B2_DECLARE_FIELD(15, int32, qte_bonus_finish);
		B2_DECLARE_FIELD(16, int32, stage_boost_ticket);
		B2_DECLARE_FIELD(17, TArray<B2GuildSkillPtr>, guild_skills);
		B2_DECLARE_FIELD(18, int32, gold_buff_rate)
		B2_DECLARE_FIELD(19, int32, exp_buff_rate)
		B2_DECLARE_FIELD(20, int32, equip_item_gold_rate)
		B2_DECLARE_FIELD(21, int32, equip_item_exp_rate)
		B2_DECLARE_FIELD(22, int32, item_set_gold_rate)
		B2_DECLARE_FIELD(23, int32, item_set_exp_rate)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(24)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseClearStage)
		B2_DECLARE_FIELD(0, B2AccountInfoPtr, account)
		B2_DECLARE_FIELD(1, TArray<B2CharacterServerInfoPtr>, characters)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(3, int32, clear_grade)
		B2_DECLARE_FIELD(4, int32, total_added_drop_gold)
		B2_DECLARE_FIELD(5, int32, total_added_reward_gold)
		B2_DECLARE_FIELD(6, int32, total_added_qte_gold)
		B2_DECLARE_FIELD(7, int32, reward_exp)
		B2_DECLARE_FIELD(8, int32, quest_item_id);
		B2_DECLARE_FIELD(9, int32, quest_item_count);
		B2_DECLARE_FIELD(10, bool, is_level_up_main_character);
		B2_DECLARE_FIELD(11, bool, is_level_up_tag_character);
		B2_DECLARE_FIELD(12, int32, clear_mask);
		B2_DECLARE_FIELD(13, TArray<B2AetherServerInfoPtr>, aethers)
		B2_DECLARE_FIELD(14, TArray<int32>, reward_golds);
		B2_DECLARE_FIELD(15, TArray<B2DropDataPtr>, fixed_items);
		B2_DECLARE_FIELD(16, TArray<B2ItemServerInfoPtr>, update_items)
		B2_DECLARE_FIELD(17, TArray<B2AetherServerInfoPtr>, update_aethers)
		B2_DECLARE_FIELD(18, int32, multiple_value);
		B2_DECLARE_FIELD(19, TArray<B2RelicPtr>, created_relics)
		B2_DECLARE_FIELD(20, int32, total_added_gold)
		B2_DECLARE_FIELD(21, int32, gold_buff_rate)
		B2_DECLARE_FIELD(22, int32, exp_buff_rate)
		B2_DECLARE_FIELD(23, int32, equip_item_gold_rate)
		B2_DECLARE_FIELD(24, int32, equip_item_exp_rate)
		B2_DECLARE_FIELD(25, int32, item_set_gold_rate)
		B2_DECLARE_FIELD(26, int32, item_set_exp_rate)
		B2_DECLARE_FIELD(27, TArray<B2DropDataPtr>, material_items);
		B2_DECLARE_FIELD(28, TArray<B2SoldItemInfoPtr>, sold_items);
		B2_DECLARE_FIELD(29, TArray<B2SoldAetherInfoPtr>, sold_aethers);
		B2_DECLARE_FIELD(30, int32, total_added_sales_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(31)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepStage)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, repeat_count)
		B2_DECLARE_FIELD(2, int32, stage_sweep_ticket)
		B2_DECLARE_FIELD(3, B2AccountInfoPtr, account)
		B2_DECLARE_FIELD(4, TArray<B2CharacterServerInfoPtr>, characters)
		B2_DECLARE_FIELD(5, bool, is_level_up_main_character)
		B2_DECLARE_FIELD(6, bool, is_level_up_tag_character)
		B2_DECLARE_FIELD(7, int32, reward_exp)
		B2_DECLARE_FIELD(8, int32, total_added_drop_gold)
		B2_DECLARE_FIELD(9, int32, total_added_reward_gold)
		B2_DECLARE_FIELD(10, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(11, TArray<B2AetherServerInfoPtr>, aethers)
		B2_DECLARE_FIELD(12, TArray<int32>, reward_golds)
		B2_DECLARE_FIELD(13, TArray<B2DropDataPtr>, material_items)
		B2_DECLARE_FIELD(14, int32, quest_item_id)
		B2_DECLARE_FIELD(15, int32, quest_item_count)
		B2_DECLARE_FIELD(16, TArray<B2DropDataPtr>, fixed_items)
		B2_DECLARE_FIELD(17, TArray<B2RelicPtr>, created_relics)
		B2_DECLARE_FIELD(18, TArray<B2ItemServerInfoPtr>, update_items)
		B2_DECLARE_FIELD(19, int32, total_added_gold)
		B2_DECLARE_FIELD(20, int32, gold_buff_rate)
		B2_DECLARE_FIELD(21, int32, exp_buff_rate)
		B2_DECLARE_FIELD(22, int32, equip_item_gold_rate)
		B2_DECLARE_FIELD(23, int32, equip_item_exp_rate)
		B2_DECLARE_FIELD(24, int32, item_set_gold_rate)
		B2_DECLARE_FIELD(25, int32, item_set_exp_rate)
		B2_DECLARE_FIELD(26, TArray<B2InventoryCountServerInfoPtr>, not_enough_inventories)
		B2_DECLARE_FIELD(27, int32, current_aether_count)
		B2_DECLARE_FIELD(28, int32, current_aether_max_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(29)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFailStage)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResurrection)
		B2_DECLARE_FIELD(0, int32, max_resurrection_count)
		B2_DECLARE_FIELD(1, int32, resurrection_count)
		B2_DECLARE_FIELD(2, int32, used_cash)
		B2_DECLARE_FIELD(3, int32, total_cash)
		B2_DECLARE_FIELD(4, int32, next_resurrection_gem_cost)
		B2_DECLARE_FIELD(5, int32, selected_count_resurrection_buff1)
		B2_DECLARE_FIELD(6, int32, selected_count_resurrection_buff2)
		B2_DECLARE_FIELD(7, int32, selected_count_resurrection_buff3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckBladePointCharge)
		B2_DECLARE_FIELD(0, int32, blade_point)
		B2_DECLARE_FIELD(1, int32, blade_point_max)
		B2_DECLARE_FIELD(2, int32, blade_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemSurpassCost)
		B2_DECLARE_FIELD(0, int32, need_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemUpgradeCost)
		B2_DECLARE_FIELD(0, int32, need_gold)
		B2_DECLARE_FIELD(1, int32, need_upgrade_stone_template_id)
		B2_DECLARE_FIELD(2, int32, need_upgrade_stone_count)
		B2_DECLARE_FIELD(3, B2ItemServerInfoPtr, next_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemComposeCost)
		B2_DECLARE_FIELD(0, int32, need_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetActInfo)
		B2_DECLARE_FIELD(0, TArray<B2StageClearInfoPtr>, stage_clear_infos)
		B2_DECLARE_FIELD(1, TArray<B2ActRewardInfoPtr>, reward_infos)
		B2_DECLARE_FIELD(2, TArray<B2ActIdentifierPtr>, has_reward_acts)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetStageInfo)
		B2_DECLARE_FIELD(0, TArray<B2StageClearRewardItemInfoPtr>, reward_item_infos)
		B2_DECLARE_FIELD(1, int32, auto_repeat_battle_cost)
		B2_DECLARE_FIELD(2, int32, need_blade_point)
		B2_DECLARE_FIELD(3, bool, enable_auto_repeat_battle)
		B2_DECLARE_FIELD(4, int32, left_buff_ticket_exp)
		B2_DECLARE_FIELD(5, int32, left_buff_ticket_gold)
		B2_DECLARE_FIELD(6, int32, left_buff_ticket_autoskill)
		B2_DECLARE_FIELD(7, int32, left_buff_ticket_boost)
		B2_DECLARE_FIELD(8, int32, stage_boost_ticket)
		B2_DECLARE_FIELD(9, int32, best_clear_mask)
		B2_DECLARE_FIELD(10, TArray<B2StageClearRewardAetherInfoPtr>, reward_aether_infos)
		B2_DECLARE_FIELD(11, TArray<B2DropDataPtr>, material_items);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoContentsModeStateNotify)
		B2_DECLARE_FIELD(0, int32, mode)				// B2ContentsMode
		B2_DECLARE_FIELD(1, int32, prev_state)			// B2ContentsModeState
		B2_DECLARE_FIELD(2, int32, current_state)		// B2ContentsModeState
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMatchStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoResponseGetMatchStatus)
		B2_DECLARE_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(1, int32, from_ranking)
		B2_DECLARE_FIELD(2, int32, total_ranker_count)
		B2_DECLARE_FIELD(3, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(4, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(5, int32, main_character_type)
		B2_DECLARE_FIELD(6, int32, tag_character_type)
		B2_DECLARE_FIELD(7, int32, total_power)
		B2_DECLARE_FIELD(8, B2RankingRewardPtr, ranking_reward)
		B2_DECLARE_FIELD(9, TArray<B2HallOfFamePtr>, hall_of_fames)
		B2_DECLARE_FIELD(10, int64, settlement_time)
		B2_DECLARE_FIELD(11, int32, settlement_period_hour)
		B2_DECLARE_FIELD(12, bool, today_reward)
		B2_DECLARE_FIELD(13, bool, last_reward)
		B2_DECLARE_FIELD(14, int32, today_participation_count)
		B2_DECLARE_FIELD(15, int32, current_gold)
		B2_DECLARE_FIELD(16, int32, current_bladepoint)
		B2_DECLARE_FIELD(17, int32, current_gem)
		B2_DECLARE_FIELD(18, int32, match_point)
		B2_DECLARE_FIELD(19, int32, additional_match_count)
		B2_DECLARE_FIELD(20, int32, match_point_next_charge_second)
		B2_DECLARE_FIELD(21, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(22)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMatchmakingRequest)
        B2_DECLARE_FIELD(0, int32, match_type)
        B2_DECLARE_FIELD(1, int32, main_character_type)
        B2_DECLARE_FIELD(2, int32, tag_character_type)
		B2_DECLARE_FIELD(3, bool, use_additional_count)
        B2_DECLARE_FIELD(4, FString, room_seed)
    DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoMatchmakingResponse)
		B2_DECLARE_FIELD(0, B2MatchInfoPtr, match_info)
		B2_DECLARE_FIELD(1, int32, room_type)
		B2_DECLARE_FIELD(2, B2RoomLocationPtr, room_location)
		B2_DECLARE_FIELD(3, int32, current_gem)
		B2_DECLARE_FIELD(4, int32, addadditional_match_count)
		B2_DECLARE_FIELD(5, int32, match_point)
		B2_DECLARE_FIELD(6, B2MatcherPtr, ai_opponent)
		B2_DECLARE_FIELD(7, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRaidMatchmakingRequest)
        B2_DECLARE_FIELD(0, int32, raid_req_type)
        B2_DECLARE_FIELD(1, int32, character_type)
        B2_DECLARE_FIELD(2, int32, raid_type)
        B2_DECLARE_FIELD(3, int32, raid_step)
		B2_DECLARE_FIELD(4, B2EndpointPtr, channel_endpoint)
    DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRaidMatchmakingResponse)
        B2_DECLARE_FIELD(0, int32, req_type)
        B2_DECLARE_FIELD(1, FString, raid_token)
        B2_DECLARE_FIELD(2, int32, room_type)
        B2_DECLARE_FIELD(3, int32, room_access)
        B2_DECLARE_FIELD(4, B2EndpointPtr, channel_endpoint)
        B2_DECLARE_FIELD(5, B2RoomLocationPtr, room_location)
		B2_DECLARE_FIELD(6, TArray<B2GuildSkillPtr>, guild_skills)
		B2_DECLARE_FIELD(7, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoCancelMatchmakingRequest)
    DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoCancelMatchmakingResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRaidInvitationResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRaidInvitationNotify)
        B2_DECLARE_FIELD(0, FString, inviter)
        B2_DECLARE_FIELD(1, FString, invitation_key)
        B2_DECLARE_FIELD(2, B2RoomLocationPtr, room_location)
		B2_DECLARE_FIELD(3, int32, map_id)
		B2_DECLARE_FIELD(4, int32, difficulty)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(5)

    //////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMatchResultRequest)
		B2_DECLARE_FIELD(0, B2MatchResultPtr, result)
		B2_DECLARE_FIELD(1, int32, match_type)
		B2_DECLARE_FIELD(2, B2RoomCharacterPtr, my_main_character)
		B2_DECLARE_FIELD(3, B2RoomCharacterPtr, my_tag_character)
		B2_DECLARE_FIELD(4, B2RoomCharacterPtr, opponent_main_character)
		B2_DECLARE_FIELD(5, B2RoomCharacterPtr, opponent_tag_character)
		B2_DECLARE_FIELD(6, FString, room_id)
		B2_DECLARE_FIELD(7, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(8)


	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoMatchResultResponse)
		B2_DECLARE_FIELD(0, B2MatchStatusPtr, status)
		B2_DECLARE_FIELD(1, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(2, B2MatchRankerPtr, prev_ranker)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
		B2_DECLARE_FIELD(4, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoInvitationRequest)
        B2_DECLARE_FIELD(0, FString, invitee)
        B2_DECLARE_FIELD(1, FString, room_id)
    DECLARE_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoInvitationResponse)
        B2_DECLARE_FIELD(0, FString, invitee)
        B2_DECLARE_FIELD(1, FString, invitation_ref)
        B2_DECLARE_FIELD(2, B2RoomLocationPtr, room_location)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoInvitationNotify)
        B2_DECLARE_FIELD(0, int32, invitation_type)
        B2_DECLARE_FIELD(1, FString, inviter)
        B2_DECLARE_FIELD(2, FString, invitation_ref)
        B2_DECLARE_FIELD(3, B2RoomLocationPtr, room_location)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoAcceptInvitationRequest)
        B2_DECLARE_FIELD(0, int32, invitation_type)
        B2_DECLARE_FIELD(1, FString, inviter)
        B2_DECLARE_FIELD(2, FString, invitation_ref)
        B2_DECLARE_FIELD(3, FString, invitee)
        B2_DECLARE_FIELD(4, B2RoomLocationPtr, room_location)
    DECLARE_B2MESSAGEINFO_REQUEST_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoAcceptInvitationResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoAcceptInvitationNotify)
        B2_DECLARE_FIELD(0, int32, invitation_type)
        B2_DECLARE_FIELD(1, FString, invitee)
        B2_DECLARE_FIELD(2, FString, invitation_ref)
        B2_DECLARE_FIELD(3, B2RoomLocationPtr, room_location)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoDeclineInvitationRequest)
        B2_DECLARE_FIELD(0, int32, invitation_type)
        B2_DECLARE_FIELD(1, FString, inviter)
        B2_DECLARE_FIELD(2, FString, invitation_ref)
        B2_DECLARE_FIELD(3, FString, invitee)
    DECLARE_B2MESSAGEINFO_REQUEST_END(4)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoDeclineInvitationResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoDeclineInvitationNotify)
        B2_DECLARE_FIELD(0, int32, invitation_type)
        B2_DECLARE_FIELD(1, FString, invitee)
        B2_DECLARE_FIELD(2, FString, invitation_ref)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomJoinRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, int32, room_access)
        B2_DECLARE_FIELD(3, TArray<B2RoomPropertyPtr>, properties)
        B2_DECLARE_FIELD(4, TArray<B2RoomCharacterPtr>, room_characters)
		B2_DECLARE_FIELD(5, TArray<B2PresetNumPtr>, character_presets)
    DECLARE_B2MESSAGEINFO_REQUEST_END(6)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomJoinResponse)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, B2RoomParticipantPtr, host)
        B2_DECLARE_FIELD(3, TArray<B2RoomParticipantPtr>, participants)
        B2_DECLARE_FIELD(4, TArray<B2RoomPropertyPtr>, properties)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomLeaveRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
    DECLARE_B2MESSAGEINFO_REQUEST_END(1)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomLeaveResponse)
        B2_DECLARE_FIELD(0, TArray<FString>, room_id_list)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomKickRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int64, kick_account_id)
    DECLARE_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomKickResponse)
        B2_DECLARE_FIELD(0, int64, kicked_account_id)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_COMMAND(FB2MessageInfoRoomMessageCommand)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, receiver_id)
        B2_DECLARE_FIELD(2, FString, message)
    DECLARE_B2MESSAGEINFO_COMMAND_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomParticipantJoinNotify)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, B2RoomParticipantPtr, joiner)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomParticipantLeaveNotify)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, B2RoomParticipantPtr, leaver)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomHostChangedNotify)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, int32, host_id)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomMessageNotify)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, room_type)
        B2_DECLARE_FIELD(2, B2RoomParticipantPtr, sender)
        B2_DECLARE_FIELD(3, FString, message)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

    //////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomHeartbeatNotify)
	DECLARE_B2MESSAGEINFO_NOTIFY_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomSetPropertyRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, TArray<B2RoomPropertyPtr>, properties)
    DECLARE_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomSetPropertyResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomPropertyUpdatedNotify)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, TArray<B2RoomPropertyPtr>, properties)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomInviteRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, FString, invitee)
        B2_DECLARE_FIELD(2, int32, invitation_number)
		B2_DECLARE_FIELD(3, int32, map_id)
		B2_DECLARE_FIELD(4, int32, difficulty)
    DECLARE_B2MESSAGEINFO_REQUEST_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomInviteResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomCloseRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
		B2_DECLARE_FIELD(1, int32, map_id)
		B2_DECLARE_FIELD(2, int32, reason)
    DECLARE_B2MESSAGEINFO_REQUEST_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomLeaveResultRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
		B2_DECLARE_FIELD(1, int64, leaved_account_id)
		B2_DECLARE_FIELD(2, FString, match_token)
    DECLARE_B2MESSAGEINFO_REQUEST_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomCloseResponse)
		B2_DECLARE_FIELD(0, FString, room_id)
		B2_DECLARE_FIELD(1, int32, room_type)
		B2_DECLARE_FIELD(2, FString, token)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomLeaveResultResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomAccessControlRequest)
        B2_DECLARE_FIELD(0, FString, room_id)
        B2_DECLARE_FIELD(1, int32, access)
    DECLARE_B2MESSAGEINFO_REQUEST_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomAccessControlResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetRoomDebugMode)
		B2_DECLARE_FIELD(0, bool, skip_room_heart_beat_check)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetRoomDebugMode)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoReportUserViaChannelRequest)
		B2_DECLARE_FIELD(0, int64, target_account_id)
		B2_DECLARE_FIELD(1, int32, mode_type)
		B2_DECLARE_FIELD(2, int32, reason_type)
		B2_DECLARE_FIELD(3, FString, explain)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoReportUserViaChannelResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoWorldJoinRequest)
		B2_DECLARE_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoWorldJoinResponse)
		B2_DECLARE_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoWorldLeaveRequest)
		B2_DECLARE_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoWorldLeaveResponse)
		B2_DECLARE_FIELD(0, TArray<FString>, world_id_list)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelUpSkill)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, left_skill_point)
		B2_DECLARE_FIELD(2, B2SkillServerInfoPtr, skill)
		B2_DECLARE_FIELD(3, int32, current_gold)
		B2_DECLARE_FIELD(4, TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetSkill)
		B2_DECLARE_FIELD(0, int32, current_gem)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, left_skill_point)
		B2_DECLARE_FIELD(3, TArray<B2SkillServerInfoPtr>, skills)
		B2_DECLARE_FIELD(4, TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetUsingSkill)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, B2SkillQuickSlotPtr, skill_quick_slot)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuySkillPoint)
		B2_DECLARE_FIELD(0, int32, left_gem)
		B2_DECLARE_FIELD(1, TArray<B2BuySkillPointResponseInfoPtr>, buy_results)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveNickname)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateNickname)
		B2_DECLARE_FIELD(0, FString, old_nickname)
		B2_DECLARE_FIELD(1, FString, new_nickname)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, updated_item)
		B2_DECLARE_FIELD(3, int64, deleted_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetServerVersion)
		B2_DECLARE_FIELD(0, FString, version)
		B2_DECLARE_FIELD(1, FString, stage)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveCharacterName)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveActRepeatClearReward)
		B2_DECLARE_FIELD(0, int32, clear_count)
		B2_DECLARE_FIELD(1, int32, next_require_clear_count)
		B2_DECLARE_FIELD(2, int32, next_reward_gold)
		B2_DECLARE_FIELD(3, int32, rewarded_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveActClearReward)
		B2_DECLARE_FIELD(0, int32, reward_index)
		B2_DECLARE_FIELD(1, int32, reward_gold)		// @NOTE 삭제 예정
		B2_DECLARE_FIELD(2, int32, reward_gem)			// @NOTE 삭제 예정
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetTeamBattleStatus)
		B2_DECLARE_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(1, int32, from_ranking)
		B2_DECLARE_FIELD(2, int32, total_ranker_count)
		B2_DECLARE_FIELD(3, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(4, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(5, TArray<B2BattleFormationPtr>, formations)
		B2_DECLARE_FIELD(6, B2TeamBattleEntryPtr, entry)
		B2_DECLARE_FIELD(7, B2RankingRewardPtr, ranking_reward)
		B2_DECLARE_FIELD(8, TArray<B2HallOfFamePtr>, hall_of_fames)
		B2_DECLARE_FIELD(9, int64, settlement_time)
		B2_DECLARE_FIELD(10, int32, settlement_period_hour)
		B2_DECLARE_FIELD(11, bool, today_reward)
		B2_DECLARE_FIELD(12, bool, last_reward)
		B2_DECLARE_FIELD(13, int32, today_participation_count)
		B2_DECLARE_FIELD(14, int32, current_gold)
		B2_DECLARE_FIELD(15, int32, current_bladepoint)
		B2_DECLARE_FIELD(16, int32, current_gem)
		B2_DECLARE_FIELD(17, int32, match_point)
		B2_DECLARE_FIELD(18, int32, additional_match_count)
		B2_DECLARE_FIELD(19, int32, match_point_next_charge_second)
		B2_DECLARE_FIELD(20, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(21)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAssaultBattleStatus)
		B2_DECLARE_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(1, int32, from_ranking)
		B2_DECLARE_FIELD(2, int32, total_ranker_count)
		B2_DECLARE_FIELD(3, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(4, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(5, int32, battle_character_type)
		B2_DECLARE_FIELD(6, int32, total_power)
		B2_DECLARE_FIELD(7, B2RankingRewardPtr, ranking_reward)
		B2_DECLARE_FIELD(8, TArray<B2HallOfFamePtr>, hall_of_fames)
		B2_DECLARE_FIELD(9, int64, settlement_time)
		B2_DECLARE_FIELD(10, int32, settlement_period_hour)
		B2_DECLARE_FIELD(11, bool, today_reward)
		B2_DECLARE_FIELD(12, bool, last_reward)
		B2_DECLARE_FIELD(13, int32, today_participation_count)
		B2_DECLARE_FIELD(14, int32, current_gold)
		B2_DECLARE_FIELD(15, int32, current_bladepoint)
		B2_DECLARE_FIELD(16, int32, current_gem)
		B2_DECLARE_FIELD(17, B2DuelRewardBoxStatusPtr, reward_box_status)
		B2_DECLARE_FIELD(18, int32, mvp_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(19)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateTeamBattleEntry)
		B2_DECLARE_FIELD(0, B2TeamBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceTeamBattleFormation)
		B2_DECLARE_FIELD(0, B2BattleFormationPtr, formation)
		B2_DECLARE_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTeamMatchmaking)
		B2_DECLARE_FIELD(0, FString, match_token)
		B2_DECLARE_FIELD(1, B2TeamPtr, opponent_team)
		B2_DECLARE_FIELD(2, int32, current_gem)
		B2_DECLARE_FIELD(3, int32, addadditional_match_count)
		B2_DECLARE_FIELD(4, int32, match_point)
		B2_DECLARE_FIELD(5, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTeamMatchResult)
		B2_DECLARE_FIELD(0, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(1, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(2, B2MatchRankerPtr, prev_ranker)
		B2_DECLARE_FIELD(3, B2DuelRewardBoxStatusPtr, reward_box_status)
		B2_DECLARE_FIELD(4, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_COMMAND(FB2MessageInfoChatCommand)
        B2_DECLARE_FIELD(0, int32, type)
        B2_DECLARE_FIELD(1, FString, to)
        B2_DECLARE_FIELD(2, int32, format)
        B2_DECLARE_FIELD(3, FString, body)
		B2_DECLARE_FIELD(4, int32, character_level_max)
    DECLARE_B2MESSAGEINFO_COMMAND_END(5)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoChatNotify)
        B2_DECLARE_FIELD(0, int32, type)
        B2_DECLARE_FIELD(1, FString, from)
        B2_DECLARE_FIELD(2, int32, format)
        B2_DECLARE_FIELD(3, FString, body)
        B2_DECLARE_FIELD(4, B2GuildInfoPtr, guild)
        B2_DECLARE_FIELD(5, int32, notice_type)
        B2_DECLARE_FIELD(6, TArray<FString>, notice_args)
        B2_DECLARE_FIELD(7, int64, account_id)
		B2_DECLARE_FIELD(8, int32, character_level_max)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(9)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoAdminMessageNotify)
        B2_DECLARE_FIELD(0, TArray<B2AdminMessagePtr>, messages)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildChat)
		B2_DECLARE_FIELD(0, int32, format)
		B2_DECLARE_FIELD(1, FString, body)
		B2_DECLARE_FIELD(2, int32, character_level_max)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildChat)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildChat)
		B2_DECLARE_FIELD(0, int32, chat_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildChat)
		B2_DECLARE_FIELD(0, TArray<B2GuildChatDataPtr>, chat_list)
		B2_DECLARE_FIELD(1, int64, next_ask_support_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskGuildSupport)
		B2_DECLARE_FIELD(0, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAskGuildSupport)
		B2_DECLARE_FIELD(0, int64, next_ask_support_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildSupport)
		B2_DECLARE_FIELD(0, int64, target_account_id)
		B2_DECLARE_FIELD(1, int64, chat_id)
		B2_DECLARE_FIELD(2, int32, count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildSupport)
		B2_DECLARE_FIELD(0, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildDonation)
		B2_DECLARE_FIELD(0, int32, mercenary_id)
		B2_DECLARE_FIELD(1, int32, donation_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildDonation)
		B2_DECLARE_FIELD(0, B2LightAccountPtr, light_account)
		B2_DECLARE_FIELD(1, B2GuildMemberPtr, guild_member)
		B2_DECLARE_FIELD(2, B2GuildMercenaryPtr, mercenary)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoGuildChatNotify)
		B2_DECLARE_FIELD(0, TArray<B2GuildChatDataPtr>, chat_list)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRank)
		B2_DECLARE_FIELD(0, TArray<B2RankInfoPtr>, rankInfos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceRankNode)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, rank)
		B2_DECLARE_FIELD(2, int32, rank_node_index)
		B2_DECLARE_FIELD(3, bool, rank_node_state)
		B2_DECLARE_FIELD(4, TArray<B2ItemServerInfoPtr>, changed_rank_promotion_points)
		B2_DECLARE_FIELD(5, TArray<int64>, deleted_rank_promotion_point_ids)
		B2_DECLARE_FIELD(6, int32, current_gold)
		B2_DECLARE_FIELD(7, bool, is_success)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMailList)
		B2_DECLARE_FIELD(0, TArray<B2MailInfoPtr>, mail_infos);
		B2_DECLARE_FIELD(1, int32, new_present_mail_count);
		B2_DECLARE_FIELD(2, int32, new_bladepoint_mail_count);
		B2_DECLARE_FIELD(3, int32, new_socialpoint_mail_count);
		B2_DECLARE_FIELD(4, int32, new_lottery_mail_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenMail)
		B2_DECLARE_FIELD(0, int64, mail_id);
		B2_DECLARE_FIELD(1, B2MailItemInfoPtr, mail_item_info);
		B2_DECLARE_FIELD(2, int32, gold);
		B2_DECLARE_FIELD(3, int32, gem);
		B2_DECLARE_FIELD(4, int32, blade_point);
		B2_DECLARE_FIELD(5, int32, social_point);
		B2_DECLARE_FIELD(6, int32, dark_crystal);
		B2_DECLARE_FIELD(7, int32, stage_boost_ticket);
		B2_DECLARE_FIELD(8, int32, stage_auto_clear_ticket);
		B2_DECLARE_FIELD(9, int32, hot_time_gold_buff_ticket);
		B2_DECLARE_FIELD(10, int32, hot_time_exp_buff_ticket);
		B2_DECLARE_FIELD(11, int32, shop_ten_lottery_ticket);
		B2_DECLARE_FIELD(12, int32, fame_point);
		B2_DECLARE_FIELD(13, B2AetherServerInfoPtr, aether);
		B2_DECLARE_FIELD(14, B2TotemServerInfoPtr, totem);
		B2_DECLARE_FIELD(15, int32, exceed_inven_character_type);
		B2_DECLARE_FIELD(16, int32, current_aether_count);
		B2_DECLARE_FIELD(17, int32, aether_inventory_max_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(18)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenAllMail)
		B2_DECLARE_FIELD(0, TArray<int64>, mail_ids);		
		B2_DECLARE_FIELD(1, int32, gold);
		B2_DECLARE_FIELD(2, int32, gem);
		B2_DECLARE_FIELD(3, int32, blade_point);
		B2_DECLARE_FIELD(4, int32, social_point);
		B2_DECLARE_FIELD(5, TArray<B2MailItemInfoPtr>, item_infos);
		B2_DECLARE_FIELD(6, int32, dark_crystal);
		B2_DECLARE_FIELD(7, int32, stage_boost_ticket);
		B2_DECLARE_FIELD(8, int32, hot_time_gold_buff_ticket);
		B2_DECLARE_FIELD(9, int32, hot_time_exp_buff_ticket);
		B2_DECLARE_FIELD(10, int32, shop_ten_lottery_ticket);
		B2_DECLARE_FIELD(11, int32, fame_point);
		B2_DECLARE_FIELD(12, int32, stage_auto_clear_ticket);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(13)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetNewMailCount)
		B2_DECLARE_FIELD(0, int32, new_mail_all);
		B2_DECLARE_FIELD(1, int32, new_present_mail_count);
		B2_DECLARE_FIELD(2, int32, new_bladepoint_mail_count);
		B2_DECLARE_FIELD(3, int32, new_socialpoint_mail_count);
		B2_DECLARE_FIELD(4, int32, new_lottery_mail_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAskFriend)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAcceptAskFriend)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRejectAskFriend)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFriendList)
		B2_DECLARE_FIELD(0, TArray<B2FriendPtr>, friends);
		B2_DECLARE_FIELD(1, int32, left_delete_friend_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelAskFriend)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteFriend)
		B2_DECLARE_FIELD(0, int32, left_delete_friend_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAllSendAskFriend)
		B2_DECLARE_FIELD(0, TArray<B2UserPtr>, acceptors);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAllReceiveAskFriend)
		B2_DECLARE_FIELD(0, TArray<B2UserPtr>, requestors);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSendSocialPoint)
		B2_DECLARE_FIELD(0, int32, current_social_point);
		B2_DECLARE_FIELD(1, TArray<int64>, success_account_ids);
		B2_DECLARE_FIELD(2, int64, fail_account_id);
		B2_DECLARE_FIELD(3, int32, fail_error_code);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRecommendFriend)
		B2_DECLARE_FIELD(0, TArray<B2UserPtr>, recommended_users);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindUser)
		B2_DECLARE_FIELD(0, B2UserPtr, user);
		B2_DECLARE_FIELD(1, int32, guild_invite_state);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConfirmNewFriend)
		B2_DECLARE_FIELD(0, TArray<long>, applied_friend_ids);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteInfo)
		B2_DECLARE_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteCountUp)
		B2_DECLARE_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendJoinerCountUp)
		B2_DECLARE_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteReward)
		B2_DECLARE_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetQuest)
		B2_DECLARE_FIELD(0, TArray<B2QuestInfoPtr>, quest_infos);
		B2_DECLARE_FIELD(1, bool, daily_quest_initialized);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAcceptQuest)
		B2_DECLARE_FIELD(0, B2QuestInfoPtr, quest_info);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveQuestReward)
		B2_DECLARE_FIELD(0, int32, received_reward_slot);
		B2_DECLARE_FIELD(1, B2QuestInfoPtr, next_quest_info);		
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoQuestAlarmNotify)
		B2_DECLARE_FIELD(0, int32, completed_slot);
		B2_DECLARE_FIELD(1, B2QuestInfoPtr, next_quest_info);
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCounterDungeon)
		B2_DECLARE_FIELD(0, TArray<int32>, opened_dungeon_types);
		B2_DECLARE_FIELD(1, int32, left_ticket);
		B2_DECLARE_FIELD(2, int32, weekly_clear_count);
		B2_DECLARE_FIELD(3, int32, received_weekly_reward);
		B2_DECLARE_FIELD(4, int32, next_buy_ticket_gem_cost);
		B2_DECLARE_FIELD(5, int32, weekly_reset_day_of_week);
		B2_DECLARE_FIELD(6, int32, main_character_type);
		B2_DECLARE_FIELD(7, int32, buy_ticket_count);
		B2_DECLARE_FIELD(8, int32, buy_ticket_count_max);
		B2_DECLARE_FIELD(9, int32, highest_difficulty_level);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartCounterDungeon)
		B2_DECLARE_FIELD(0, FString, play_token);
		B2_DECLARE_FIELD(1, int32, drop_gold);
		B2_DECLARE_FIELD(2, TArray<B2GuildSkillPtr>, guild_skills);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishCounterDungeon)
		B2_DECLARE_FIELD(0, int32, current_gold);
		B2_DECLARE_FIELD(1, int32, current_gem);
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, received_rewards);
		B2_DECLARE_FIELD(3, int32, left_ticket);
		B2_DECLARE_FIELD(4, int32, weekly_clear_count);
		B2_DECLARE_FIELD(5, int32, next_buy_ticket_gem_cost);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id);
		B2_DECLARE_FIELD(1, int32, current_gold);
		B2_DECLARE_FIELD(2, int32, current_gem);
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, received_rewards);
		B2_DECLARE_FIELD(4, int32, left_ticket);
		B2_DECLARE_FIELD(5, int32, weekly_clear_count);
		B2_DECLARE_FIELD(6, int32, next_buy_ticket_gem_cost);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyCounterDungeonTicket)
		B2_DECLARE_FIELD(0, int32, current_gem);
		B2_DECLARE_FIELD(1, int32, left_ticket);
		B2_DECLARE_FIELD(2, int32, buy_ticket_count);
		B2_DECLARE_FIELD(3, int32, buy_ticket_count_max);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCounterDungeonWeeklyReward)
		B2_DECLARE_FIELD(0, int32, current_gold);
		B2_DECLARE_FIELD(1, int32, current_gem);
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, received_rewards);
		B2_DECLARE_FIELD(3, int32, received_weekly_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDimension)
		B2_DECLARE_FIELD(0, B2DimensionStatusPtr, dimension_status);
		B2_DECLARE_FIELD(1, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(2, int32, from_ranking)
		B2_DECLARE_FIELD(3, int32, total_ranker_count)
		B2_DECLARE_FIELD(4, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(5, B2DimensionRankingRewardPtr, ranking_reward)
		B2_DECLARE_FIELD(6, int64, settlement_time)
		B2_DECLARE_FIELD(7, int32, settlement_period_hour)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartDimension)
		B2_DECLARE_FIELD(0, FString, play_token);
		B2_DECLARE_FIELD(1, TArray<B2GuildSkillPtr>, guild_skills);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishDimension)
		B2_DECLARE_FIELD(0, int32, play_result);
		B2_DECLARE_FIELD(1, B2DimensionStatusPtr, dimension_status);
		B2_DECLARE_FIELD(2, bool, is_new_record);
		B2_DECLARE_FIELD(3, TArray<B2TotemServerInfoPtr>, totems)
		B2_DECLARE_FIELD(4, TArray<B2DropDataPtr>, fixed_items);
		B2_DECLARE_FIELD(5, TArray<B2ItemServerInfoPtr>, update_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCreateGuild)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAskJoinGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLeaveGuild)
		B2_DECLARE_FIELD(0, int64, leave_guild_time);
		B2_DECLARE_FIELD(1, int64, guild_id);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildDetailInfo)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild)
		B2_DECLARE_FIELD(1, TArray<B2GuildMemberPtr>, guild_members)
		B2_DECLARE_FIELD(2, B2GuildBattleStatePtr, guild_battle_state)
		B2_DECLARE_FIELD(3, B2GuildBattleEntryPtr, entry)
		B2_DECLARE_FIELD(4, bool, has_turn_reward)
		B2_DECLARE_FIELD(5, TArray<B2GuildMercenaryPtr>, mercenaries)
		B2_DECLARE_FIELD(6, TArray<B2GuildSkillPtr>, guild_skills)
		B2_DECLARE_FIELD(7, bool, joined_guild_battle_member)
		B2_DECLARE_FIELD(8, TArray<B2CharacterPowerPtr>, character_powers)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildMember)
		B2_DECLARE_FIELD(0, TArray<B2GuildMemberPtr>, guild_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRecommendGuild)
		B2_DECLARE_FIELD(0, TArray<B2GuildPtr>, guilds)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildMemberCandidates)
		B2_DECLARE_FIELD(0, TArray<B2GuildMemberCandidatePtr>, candidates)
		B2_DECLARE_FIELD(1, bool, need_friend_list)
		B2_DECLARE_FIELD(2, TArray<B2FriendPtr>, friends)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildInviteList)
		B2_DECLARE_FIELD(0, TArray<B2GuildInvitePtr>, invites)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRejectInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAcceptInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindGuild)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseObtainGuildMaster)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild)
		B2_DECLARE_FIELD(1, TArray<B2GuildMemberPtr>, changed_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseHandOverGuildMaster)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild)
		B2_DECLARE_FIELD(1, TArray<B2GuildMemberPtr>, changed_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildRankingList)
		B2_DECLARE_FIELD(0, TArray<B2GuildRankingPtr>, guild_rankings)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAppointSubGuildMaster)
		B2_DECLARE_FIELD(0, B2GuildMemberPtr, changed_member)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismissSubGuildMaster)
		B2_DECLARE_FIELD(0, B2GuildMemberPtr, changed_member)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKickGuildMember)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateGuild)
		B2_DECLARE_FIELD(0, int32, join_option)
		B2_DECLARE_FIELD(1, int32, mark_index)
		B2_DECLARE_FIELD(2, int32, mark_color)
		B2_DECLARE_FIELD(3, int32, mark_bg_index)
		B2_DECLARE_FIELD(4, int32, mark_bg_color)
		B2_DECLARE_FIELD(5, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateGuildNotice)
		B2_DECLARE_FIELD(0, FString, notice)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelUpGuildSkill)
		B2_DECLARE_FIELD(0, B2GuildSkillPtr, skill)
		B2_DECLARE_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyGuildSkill)
		B2_DECLARE_FIELD(0, B2GuildSkillPtr, skill)
		B2_DECLARE_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMission)
		B2_DECLARE_FIELD(0, TArray<B2MissionInfoPtr>, mission_infos);
		B2_DECLARE_FIELD(1, TArray<B2MissionCompletePointInfoPtr>, mission_complete_point_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveMissionReward)
		B2_DECLARE_FIELD(0, B2MissionInfoPtr, received_mission_info);
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveCompleteMissionReward)
		B2_DECLARE_FIELD(0, int32, received_mission_type);
		B2_DECLARE_FIELD(1, int32, received_reward_index);
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetWing)
		B2_DECLARE_FIELD(0, int32, character_type);
		B2_DECLARE_FIELD(1, int32, grade);
		B2_DECLARE_FIELD(2, int32, level);
		B2_DECLARE_FIELD(3, int32, enhance_point);
		B2_DECLARE_FIELD(4, int32, upgrade_fail_point);
		B2_DECLARE_FIELD(5, TArray<B2WingOptionInfoPtr>, wing_option_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceWing)
		B2_DECLARE_FIELD(0, int32, character_type);
		B2_DECLARE_FIELD(1, int32, level);
		B2_DECLARE_FIELD(2, int32, enhance_point);
		B2_DECLARE_FIELD(3, int32, current_money);
		B2_DECLARE_FIELD(4, int32, material_item_count);
		B2_DECLARE_FIELD(5, TArray<B2ItemServerInfoPtr>, changed_material_items)
		B2_DECLARE_FIELD(6, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpgradeWing)
		B2_DECLARE_FIELD(0, int32, character_type);
		B2_DECLARE_FIELD(1, bool, isUpgradeSuccess);
		B2_DECLARE_FIELD(2, int32, level);
		B2_DECLARE_FIELD(3, int32, grade);
		B2_DECLARE_FIELD(4, int32, current_money);
		B2_DECLARE_FIELD(5, int32, fail_point);
		B2_DECLARE_FIELD(6, int32, material_item_count);
		B2_DECLARE_FIELD(7, TArray<B2ItemServerInfoPtr>, changed_material_items)
		B2_DECLARE_FIELD(8, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceWingOption)
		B2_DECLARE_FIELD(0, int32, character_type);
		B2_DECLARE_FIELD(1, B2WingOptionInfoPtr, enhanced_wing_option);
		B2_DECLARE_FIELD(2, int32, material_item_count);
		B2_DECLARE_FIELD(3, TArray<B2ItemServerInfoPtr>, changed_material_items)
		B2_DECLARE_FIELD(4, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetHeroTower)
		B2_DECLARE_FIELD(0, int32, start_floor)
		B2_DECLARE_FIELD(1, int32, max_clear_floor)
		B2_DECLARE_FIELD(2, int32, main_character_type)
		B2_DECLARE_FIELD(3, int32, tag_character_type)
		B2_DECLARE_FIELD(4, int32, daily_try_count)
		B2_DECLARE_FIELD(5, int32, current_free_ticket)				// 현재 무료 입장 가능 횟수
		B2_DECLARE_FIELD(6, int32, current_buy_ticket)					// 현재 구매된 유료 입장 횟수
		B2_DECLARE_FIELD(7, int32, possible_try_count)					// 입장 가능한 횟수
		B2_DECLARE_FIELD(8, B2ItemServerInfoPtr, current_hero_piece)
		B2_DECLARE_FIELD(9, TArray<B2RelicPtr>, relics)
		B2_DECLARE_FIELD(10, bool, is_sweep)
		B2_DECLARE_FIELD(11, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(12, int32, from_ranking)
		B2_DECLARE_FIELD(13, int32, total_ranker_count)
		B2_DECLARE_FIELD(14, B2RankingPtr, ranking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(15)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartHeroTower)
		B2_DECLARE_FIELD(0, FString, play_token)
		B2_DECLARE_FIELD(1, int32, left_free_ticket)
		B2_DECLARE_FIELD(2, int32, daily_enter_count)
		B2_DECLARE_FIELD(3, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishHeroTower)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, rewards)
		B2_DECLARE_FIELD(1, int32, next_floor)
		B2_DECLARE_FIELD(2, FString, play_token)
		B2_DECLARE_FIELD(3, bool, is_continue)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAbandonHeroTower)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceRelic)
		B2_DECLARE_FIELD(0, B2RelicPtr, relic)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_hero_piece_items)
		B2_DECLARE_FIELD(2, TArray<int64>, deleted_hero_piece_item_ids)
		B2_DECLARE_FIELD(3, int32, current_money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePromotionRelic)
		B2_DECLARE_FIELD(0, B2RelicPtr, relic)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_boss_piece_items)
		B2_DECLARE_FIELD(2, TArray<int64>, deleted_boss_piece_item_ids)
		B2_DECLARE_FIELD(3, int32, current_money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountRelic)
		B2_DECLARE_FIELD(0, TArray<B2RelicPtr>, relics)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepHeroTower)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyHeroTowerTicket)
		B2_DECLARE_FIELD(0, int32, current_cash)
		B2_DECLARE_FIELD(1, int32, current_free_ticket)
		B2_DECLARE_FIELD(2, int32, current_buy_ticket)
		B2_DECLARE_FIELD(3, int32, possible_try_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDrawShopLottery)
		B2_DECLARE_FIELD(0, bool, free_lottery)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(2, int32, shop_ten_lottery_ticket)
		B2_DECLARE_FIELD(3, int32, gem)
		B2_DECLARE_FIELD(4, int32, social_point)
		B2_DECLARE_FIELD(5, int32, mileage)
		B2_DECLARE_FIELD(6, int64, next_free_time)
		B2_DECLARE_FIELD(7, int32, daily_lottery_count)
		B2_DECLARE_FIELD(8, FString, last_lottery_key)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardShopMileage)
		B2_DECLARE_FIELD(0, int32, shop_mileage_type)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, received_rewards);
		B2_DECLARE_FIELD(2, int32, received_reward_index)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetLotteryShop)
		B2_DECLARE_FIELD(0, int32, mileage)
		B2_DECLARE_FIELD(1, int32, reward_index);
		B2_DECLARE_FIELD(2, int32, shop_ten_lottery_ticket);
		B2_DECLARE_FIELD(3, TArray<B2ShopLotteryPtr>, shop_lotteries)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyShopProduct)
		B2_DECLARE_FIELD(0, int32, buy_price_type)
		B2_DECLARE_FIELD(1, int32, price)
		B2_DECLARE_FIELD(2, B2BuyShopResultInfoPtr, buy_result_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePurchaseCostume)
		B2_DECLARE_FIELD(0, int32, buy_price_type)
		B2_DECLARE_FIELD(1, int32, price)
		B2_DECLARE_FIELD(2, TArray<B2CostumeServerInfoPtr>, costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGeneralShop)
		B2_DECLARE_FIELD(0, TArray<B2ShopMileagePtr>, shop_mileages)
		B2_DECLARE_FIELD(1, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		B2_DECLARE_FIELD(2, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCostumeShop)
		B2_DECLARE_FIELD(0, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		B2_DECLARE_FIELD(1, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMagicShop)
		B2_DECLARE_FIELD(0, int32, renewal_count)
		B2_DECLARE_FIELD(1, int64, next_free_renewal_time)
		B2_DECLARE_FIELD(2, bool, is_free_renewal)
		B2_DECLARE_FIELD(3, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRenewalMagicShop)
		B2_DECLARE_FIELD(0, int32, renewal_count)
		B2_DECLARE_FIELD(1, int64, next_free_renewal_time)
		B2_DECLARE_FIELD(2, int32, current_gem)
		B2_DECLARE_FIELD(3, bool, is_free_renewal)
		B2_DECLARE_FIELD(4, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePurchaseMagicShop)
		B2_DECLARE_FIELD(0, int32, renewal_count)
		B2_DECLARE_FIELD(1, int64, next_free_renewal_time)
		B2_DECLARE_FIELD(2, int32, buy_price_type)
		B2_DECLARE_FIELD(3, int32, price)
		B2_DECLARE_FIELD(4, bool, is_free_renewal)
		B2_DECLARE_FIELD(5, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCollectionItem)
		B2_DECLARE_FIELD(0, TArray<B2CollectionItemPtr>, collection_items)
		B2_DECLARE_FIELD(1, TArray<B2CollectionSetItemPtr>, reward_collection_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardCollectionItem)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, B2RewardPtr, received_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardCollectionSetItem)
		B2_DECLARE_FIELD(0, int32, gropup_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, B2RewardPtr, received_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMarketProduct)
		B2_DECLARE_FIELD(0, TArray<B2mdMarketProductPtr>, md_market_products)
		B2_DECLARE_FIELD(1, TArray<B2MarketPurchaseInfoPtr>, purchase_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePreparePurchase)
		B2_DECLARE_FIELD(0, B2MarketPurchaseInfoPtr, purchase_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseVerifyPurchase)
		B2_DECLARE_FIELD(0, int32, product_id)
		B2_DECLARE_FIELD(1, FString, market_product_id)
		B2_DECLARE_FIELD(2, FString, developer_payload)
		B2_DECLARE_FIELD(3, B2BuyShopResultInfoPtr, buy_result_info)
		B2_DECLARE_FIELD(4, int32, receive_error_code)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelPurchase)
		B2_DECLARE_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishPurchase)
		B2_DECLARE_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRaidResult)
		B2_DECLARE_FIELD(0, TArray<B2RaidRankingRewardPtr>, raid_rewards)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(2, TArray<B2LightCharacterPtr>, light_characters)
		B2_DECLARE_FIELD(3, B2RewardPtr, step_clear_reward)
		B2_DECLARE_FIELD(4, B2RewardPtr, main_reward)
		B2_DECLARE_FIELD(5, B2RewardPtr, main_bonus_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDailyAttendance)
		B2_DECLARE_FIELD(0, bool, is_attendance)
		B2_DECLARE_FIELD(1, int32, attendance_day)
		B2_DECLARE_FIELD(2, int32, attendance_type)
		B2_DECLARE_FIELD(3, B2RewardPtr, reward)
		B2_DECLARE_FIELD(4, int64, next_attendance_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEventAttendance)
		B2_DECLARE_FIELD(0, int32, eventId)
		B2_DECLARE_FIELD(1, int32, attendance_day)
		B2_DECLARE_FIELD(2, B2RewardPtr, reward)
		B2_DECLARE_FIELD(3, int64, next_attendance_time)
		B2_DECLARE_FIELD(4, int32, bonus_type)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBeginnerAttendance)
		B2_DECLARE_FIELD(0, int32, attendance_day)
		B2_DECLARE_FIELD(1, B2RewardPtr, reward)
		B2_DECLARE_FIELD(2, int64, next_attendance_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateDailyPlayTime)
		B2_DECLARE_FIELD(0, int32, daily_play_time_in_sec)
		B2_DECLARE_FIELD(1, int32, daily_play_time_reward_index)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveDailyPlayTimeReward)
		B2_DECLARE_FIELD(0, int32, daily_play_time_in_sec)
		B2_DECLARE_FIELD(1, int32, daily_play_time_reward_index)
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseJoinGuildBattle)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildBattle)
		B2_DECLARE_FIELD(0, int64, battle_end_time)
		B2_DECLARE_FIELD(1, B2GuildBattleGuildPtr, my_guild)
		B2_DECLARE_FIELD(2, B2GuildBattleGuildPtr, target_guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartGuildBattle)
		B2_DECLARE_FIELD(0, FString, battle_token)
		B2_DECLARE_FIELD(1, B2FindAccountInfoPtr, target)
		B2_DECLARE_FIELD(2, int32, target_left_medal)
		B2_DECLARE_FIELD(3, B2FindAccountInfoPtr, my_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishGuildBattle)
		B2_DECLARE_FIELD(0, int32, battle_result)
		B2_DECLARE_FIELD(1, int32, obtain_medal)
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildBattleEntry)
		B2_DECLARE_FIELD(0, B2GuildBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleTurnResult)
		B2_DECLARE_FIELD(0, int64, mvp_account_id)
		B2_DECLARE_FIELD(1, int32, battle_result)
		B2_DECLARE_FIELD(2, int32, increased_exp)
		B2_DECLARE_FIELD(3, B2GuildBattleGuildPtr, my_guild)
		B2_DECLARE_FIELD(4, B2GuildBattleGuildPtr, target_guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleTurnReward)
		B2_DECLARE_FIELD(0, int32, battle_result)
		B2_DECLARE_FIELD(1, int32, guild_medal)
		B2_DECLARE_FIELD(2, int32, reward_grade)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleSeasonReward)
		B2_DECLARE_FIELD(0, int32, season_ranking)
		B2_DECLARE_FIELD(1, int32, season_ranking_total_count)
		B2_DECLARE_FIELD(2, int32, gem)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleHistory)
		B2_DECLARE_FIELD(0, TArray<B2GuildBattleHistoryPtr>, battle_histories)
		B2_DECLARE_FIELD(1, B2GuildBattleRecordPtr, season_record)
		B2_DECLARE_FIELD(2, B2GuildBattleRecordPtr, best_season_record)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetWingVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetHelmetVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCostumeVisible)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, costume_type)
		B2_DECLARE_FIELD(2, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRaid)
		B2_DECLARE_FIELD(0, int64, penalty_remove_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRaidResurrection)
		B2_DECLARE_FIELD(0, int32, resurrection_count)
		B2_DECLARE_FIELD(1, int32, used_gem)
		B2_DECLARE_FIELD(2, int32, total_gem)
		B2_DECLARE_FIELD(3, int32, next_resurrection_gem_cost)
		B2_DECLARE_FIELD(4, int32, selected_count_resurrection_buff1)
		B2_DECLARE_FIELD(5, int32, selected_count_resurrection_buff2)
		B2_DECLARE_FIELD(6, int32, selected_count_resurrection_buff3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)


	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultMatchmaking)
		B2_DECLARE_FIELD(0, FString, token)
		B2_DECLARE_FIELD(1, int32, room_type)
		B2_DECLARE_FIELD(2, B2EndpointPtr, channel_endpoint)
		B2_DECLARE_FIELD(3, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelAssaultMatchmaking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultResult)
		B2_DECLARE_FIELD(0,B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(1, B2RankingPtr, ranking)
		B2_DECLARE_FIELD(2, B2MatchRankerPtr, prev_ranker)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, victory_rewards)
		B2_DECLARE_FIELD(4, int32, increased_mvp_point)
		B2_DECLARE_FIELD(5, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultResurrection)
		B2_DECLARE_FIELD(0, int32, resurrection_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckPvPMatchPointCharge)
		B2_DECLARE_FIELD(0, int32, match_point)
		B2_DECLARE_FIELD(1, int32, match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckTeamMatchPointCharge)
		B2_DECLARE_FIELD(0, int32, match_point)
		B2_DECLARE_FIELD(1, int32, match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFriendRankings)
		B2_DECLARE_FIELD(0, int32, opponent_type)
		B2_DECLARE_FIELD(1, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, random_option_id1)
		B2_DECLARE_FIELD(2, int32, random_option_id2)
		B2_DECLARE_FIELD(3, int32, random_option_id3)
		B2_DECLARE_FIELD(4, float, random_option_value1)
		B2_DECLARE_FIELD(5, float, random_option_value2)
		B2_DECLARE_FIELD(6, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGenerateItemRandomOption)
		B2_DECLARE_FIELD(0, int32, current_dark_crystal)
		B2_DECLARE_FIELD(1, int32, current_cash)
		B2_DECLARE_FIELD(2, int32, random_option_id1)
		B2_DECLARE_FIELD(3, int32, random_option_id2)
		B2_DECLARE_FIELD(4, int32, random_option_id3)
		B2_DECLARE_FIELD(5, float, random_option_value1)
		B2_DECLARE_FIELD(6, float, random_option_value2)
		B2_DECLARE_FIELD(7, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseApplyItemRandomOption)
		B2_DECLARE_FIELD(0, B2ItemServerInfoPtr, item_data)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDiscardItemRandomOption)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()



	//////////////////////////////////////////////////////////////////////////

	class FB2MessageInfoResponseCheckRedDot;
	class B2NETWORK_API FB2MessageInfoResponseCheckRedDot : public FB2MessageInfoResponse
	{
	public:
		FB2MessageInfoResponseCheckRedDot() : FB2MessageInfoResponse("FB2MessageInfoResponseCheckRedDot")
		{
		}
		// result of hint RED_DOT_EQUIP
		bool is_valid_equip;
		bool has_new_equipment;
		bool has_new_aether;
		bool has_new_costume;
		bool has_new_totem;

		// result of hint RED_DOT_COLLECTION
		bool is_valid_collection;
		bool has_new_collection;

		// result of hint RED_DOT_MISSION
		bool is_valid_mission;
		bool has_mission_to_reward;
		bool has_mission_mileage_reward;
		bool has_serial_mission_to_reward;

		// result of hint RED_DOT_FRIEND
		bool is_valid_friend;
		bool has_new_friend;
		bool has_friend_request;

		// result of hint RED_DOT_ADVENTURE
		bool is_valid_adventure;
		bool has_adventure_mileage_reward;

		// result of hint RED_DOT_DUNGEON
		bool is_valid_dungeon;
		int32 counter_dungeon_weekly_clear_count;
		int32 counter_dungeon_weekly_reward_received;
		int32 tickets_for_counter_dungeon;
		int32 tickets_for_hero_tower;
		int32 used_tickets_for_hero_tower;
		int32 dimension_daily_clear_count;

		// result of hint RED_DOT_PVP
		bool is_valid_pvp;
		bool has_one_vs_one_reward;
		bool has_team_battle_reward;
		bool has_team_assault_reward;
		int32 team_assault_mvp_point;

		// result of hint RED_DOT_GUILD
		bool is_valid_guild;
		bool has_guild_battle_reward_box;

		// result of hint RED_DOT_SHOP
		bool is_valid_shop;
		int64 remaining_free_item_time;
		bool has_shop_item_mileage_reward;
		bool has_shop_cash_mileage_reward;

		// result of hint RED_DOT_MAIL
		bool is_valid_mail;
		int32 unread_mail_count;
		bool has_present_mail;
		bool has_blade_point_mail;
		bool has_social_point_mail;
		bool has_lottery_mail;

		// result of hint RED_DOT_EVENT
		bool is_valid_event;
		bool has_event_level_up_reward;
		bool has_event_point_reward;
		bool has_event_season_mission_reward;
		bool has_event_stage_clear_reward;				 // 스테이지 클리어 이벤트 보상 유무 체크
		bool has_event_point_shop_reward;				 

		// result of hint RED_DOT_SHOP_PACKAGE
		bool is_valid_shop_package;
		bool has_shop_package_flat_rate_reward;          // 정액제 구매 보너스 받을게 있는지 체크
		bool has_shop_package_level_up_reward;           // 레벨업 패키지 보상 받을게 있는지 체크
		bool has_shop_package_reach_level_reward;        // 레벨 달성 패키지 보상 받을게 있는지 체크
		bool has_event_gem_purchase_reward;
		bool has_event_ten_lottery_reward;
		bool has_event_spend_gem_reward;
		bool has_event_ten_lottery_savings_reward;
		bool has_event_dice_reward;

		// result of hint RED_DOT_MAGIC_SHOP
		bool is_valid_magic_shop;
		bool has_magic_shop_free_renewal;				// 마법상점 무료갱신 여부

		// result of hint RED_DOT_HALL_OF_FAME
		bool is_valid_hall_of_fame;
		bool has_hall_of_fame_praise_count;				// 명예의 전당 경배 횟수 남았는지 여부

		// result of hint RED_DOT_FAIRY
		bool is_valid_not_apply_bless_fairy;
		bool has_not_apply_bless_fairy;					// 페어리 축복이 적용되지 않은 요정이 존재하는지 여부
														// result of hint RED_DOT_FAIRY
		bool is_valid_donation;
		bool has_donation_point_reward;					// 획득 가능한 포인트 보상이 존재하는지 체크
		bool has_usable_free_donation_count;			// 사용 가능한 무료 기부 횟수가 존재하는지 체크

		// result of hint RED_DOT_ETC
		bool is_valid_etc;
		bool has_item_random_option_change;
	};
/*
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckRedDot)
		B2_DECLARE_FIELD(0, bool, has_new_equipment)
		B2_DECLARE_FIELD(1, bool, has_new_collection)
		B2_DECLARE_FIELD(2, bool, has_mission_to_reward)
		B2_DECLARE_FIELD(3, bool, has_mission_mileage_reward)
		B2_DECLARE_FIELD(4, bool, has_serial_mission_to_reward)
		B2_DECLARE_FIELD(5, bool, has_friend_to_send_point)
		B2_DECLARE_FIELD(6, bool, has_friend_request)
		B2_DECLARE_FIELD(7, bool, has_adventure_mileage_reward)
		B2_DECLARE_FIELD(8, int32, counter_dungeon_weekly_clear_count)
		B2_DECLARE_FIELD(9, int32, counter_dungeon_weekly_reward_received)
		B2_DECLARE_FIELD(10, int32, tickets_for_counter_dungeon)
		B2_DECLARE_FIELD(11, int32, used_tickets_for_hero_tower)
		B2_DECLARE_FIELD(12, int32, tickets_for_raid)
		B2_DECLARE_FIELD(13, bool, has_one_vs_one_reward)
		B2_DECLARE_FIELD(14, bool, has_team_battle_reward)
		B2_DECLARE_FIELD(15, bool, has_team_assault_reward)
		B2_DECLARE_FIELD(16, int32, team_assault_mvp_point)
		B2_DECLARE_FIELD(17, int64, remaining_free_item_time)
		B2_DECLARE_FIELD(18, bool, has_shop_item_mileage_reward)
		B2_DECLARE_FIELD(19, bool, has_shop_cash_mileage_reward)
		B2_DECLARE_FIELD(20, int32, unread_mail_count)
		B2_DECLARE_FIELD(21, bool, has_present_mail)
		B2_DECLARE_FIELD(22, bool, has_blade_point_mail)
		B2_DECLARE_FIELD(23, bool, has_social_point_mail)
		B2_DECLARE_FIELD(24, bool, has_lottery_mail)
		B2_DECLARE_FIELD(25, bool, has_item_random_option_change)
	DECLARE_B2MESSAGEINFO_REQUEST_END(26)
*/

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseDarkCrystalPaid)
		B2_DECLARE_FIELD(0, int32, dark_crystal_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyItemRandomOption)
		B2_DECLARE_FIELD(0, int64, item_id)
		B2_DECLARE_FIELD(1, int32, random_option_id1)
		B2_DECLARE_FIELD(2, int32, random_option_id2)
		B2_DECLARE_FIELD(3, int32, random_option_id3)
		B2_DECLARE_FIELD(4, float, random_option_value1)
		B2_DECLARE_FIELD(5, float, random_option_value2)
		B2_DECLARE_FIELD(6, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyItemNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyCostumeNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartRaid)
		B2_DECLARE_FIELD(0, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoSessionClosedNotify)
        B2_DECLARE_FIELD(0, int32, reason)
    DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetContentsOpenStatus)
		B2_DECLARE_FIELD(0, TArray<B2ContentsOpenStatusPtr>, contents)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelRewardBoxInfo)
		B2_DECLARE_FIELD(0, int32, duel_mode)
		B2_DECLARE_FIELD(1, TArray<B2DuelRewardBoxInfoResponseRewardBoxPtr>, reward_boxes)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelRewardBoxGainableList)
		B2_DECLARE_FIELD(0, int32, duel_mode)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, TArray<B2DuelRewardBoxGainableListResponseRewardItemPtr>, reward_items)
		B2_DECLARE_FIELD(3, int32, reward_item_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenDuelRewardBox)
		B2_DECLARE_FIELD(0, int32, duel_mode)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, received_rewards)
		B2_DECLARE_FIELD(2, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveAssaultMvpReward)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, received_rewards)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountAether)
		B2_DECLARE_FIELD(0, TArray<B2AetherServerInfoPtr>, aethers)
		B2_DECLARE_FIELD(1, TArray<B2CharacterSetOptionInfoPtr>, current_character_set_option_Infos)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, dismantle_aether_Ids)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, current_aether_piece)
		B2_DECLARE_FIELD(2, int32, obtain_aether_piece_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMountAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, mounted_aether_ids)
		B2_DECLARE_FIELD(1, TArray<int64>, unmounted_aether_ids)
		B2_DECLARE_FIELD(2, int32, selected_offense_set_option_id)
		B2_DECLARE_FIELD(3, int32, selected_defense_set_option_id)
		B2_DECLARE_FIELD(4, int32, selected_unity_skill_set_option_id)
		B2_DECLARE_FIELD(5, TArray<int32>, defense_set_option_ids)
		B2_DECLARE_FIELD(6, TArray<int32>, unity_skill_set_option_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnmountAethers)
		B2_DECLARE_FIELD(0, TArray<int64>, unmounted_aether_ids)
		B2_DECLARE_FIELD(1, int32, selected_offense_set_option_id)
		B2_DECLARE_FIELD(2, int32, selected_defense_set_option_id)
		B2_DECLARE_FIELD(3, int32, selected_unity_skill_set_option_id)
		B2_DECLARE_FIELD(4, TArray<int32>, defense_set_option_ids)
		B2_DECLARE_FIELD(5, TArray<int32>, unity_skill_set_option_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSelectAetherDefenseOption)
		B2_DECLARE_FIELD(0, int32, selected_defense_set_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSelectAetherUnitySkillOption)
		B2_DECLARE_FIELD(0, int32, selected_unity_skill_set_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockAethers)
		B2_DECLARE_FIELD(0, TArray<B2AetherServerInfoPtr>, locked_aethers)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockAethers)
		B2_DECLARE_FIELD(0, TArray<B2AetherServerInfoPtr>, unlocked_aethers)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExtendAetherInventory)
		B2_DECLARE_FIELD(0, B2AccountInfoPtr, account)
		B2_DECLARE_FIELD(1, int32, current_slot_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceAether)
		B2_DECLARE_FIELD(0, B2AetherServerInfoPtr, aether)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_aether_piece_items)
		B2_DECLARE_FIELD(2, TArray<int64>, deleted_aether_piece_ids)
		B2_DECLARE_FIELD(3, bool, is_success)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellAethers)
		B2_DECLARE_FIELD(0, int32, current_gold)
		B2_DECLARE_FIELD(1, TArray<int64>, sell_aether_ids)
		B2_DECLARE_FIELD(2, int32, obtain_gold)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyAetherNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetTutorialProgress)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, bool, is_item_prepared)
		B2_DECLARE_FIELD(2, bool, is_lottery_drawn)
		B2_DECLARE_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateTutorialProgress)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, bool, is_item_prepared)
		B2_DECLARE_FIELD(2, bool, is_lottery_drawn)
		B2_DECLARE_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSkipTutorialProgress)
	B2_DECLARE_FIELD(0, int32, tutorial_id)
	B2_DECLARE_FIELD(1, bool, is_item_prepared)
	B2_DECLARE_FIELD(2, bool, is_lottery_drawn)
	B2_DECLARE_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDrawTutorialItem)
		B2_DECLARE_FIELD(0, bool, free_lottery)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(2, int32, shop_ten_lottery_ticket)
		B2_DECLARE_FIELD(3, int32, gem)
		B2_DECLARE_FIELD(4, int32, social_point)
		B2_DECLARE_FIELD(5, int32, mileage)
		B2_DECLARE_FIELD(6, int64, next_free_time)
		B2_DECLARE_FIELD(7, int32, daily_lottery_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareTutorial)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, int32, current_cash)
		B2_DECLARE_FIELD(2, int32, current_gold)
		B2_DECLARE_FIELD(3, TArray<B2ItemServerInfoPtr>, added_item_list)
		B2_DECLARE_FIELD(4, TArray<B2AetherServerInfoPtr>, added_aether_list)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetUnitySkillMission)
		B2_DECLARE_FIELD(0, TArray<B2UnitySkillMissionPtr>, missions)
		B2_DECLARE_FIELD(1, TArray<B2UnitySkillAwakenMissionPtr>, awaken_missions)
		B2_DECLARE_FIELD(2, bool, all_mission_completed)
		B2_DECLARE_FIELD(3, bool, all_awaken_mission_completed)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveUnitySkillReward)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward)
		B2_DECLARE_FIELD(1, B2UnitySkillMissionPtr, next_mission)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAwakenUnitySkillMission)
		B2_DECLARE_FIELD(0, B2UnitySkillAwakenMissionPtr, awaken_mission)
		B2_DECLARE_FIELD(1, int32, current_money)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_material_items)
		B2_DECLARE_FIELD(3, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAwakenUnitySkill)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelModeInfo)
		B2_DECLARE_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos)
		B2_DECLARE_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetChallengeModeInfo)
		B2_DECLARE_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos)
		B2_DECLARE_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
		B2_DECLARE_FIELD(2, int32, hero_tower_ticket_count)
		B2_DECLARE_FIELD(3, int32, counter_dungeon_left_ticket_count)
		B2_DECLARE_FIELD(4, int32, dimension_daily_clear_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetHallOfFame)
		B2_DECLARE_FIELD(0, TArray<B2HallOfFamePtr>, basics)
		B2_DECLARE_FIELD(1, TArray<B2HallOfFameUserPtr>, rankers)
		B2_DECLARE_FIELD(2, TArray<B2HallOfFameRewardPtr>, ranker_rewards)
		B2_DECLARE_FIELD(3, TArray<B2HallOfFamePraisePtr>, left_praise_counts)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePraiseTargetRanker)
		B2_DECLARE_FIELD(0, TArray<B2HallOfFamePtr>, basics)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, rewards)
		B2_DECLARE_FIELD(2, B2HallOfFamePraisePtr, left_praise_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveRestReward)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward)
		B2_DECLARE_FIELD(1, int32, used_cash)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckModeOpen)
		B2_DECLARE_FIELD(0, TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
		B2_DECLARE_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
		B2_DECLARE_FIELD(2, int32, guild_battle_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveTutorialReward)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, tutorial_rewards)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckReachPower)
		B2_DECLARE_FIELD(0, TArray<B2CharacterPowerPtr>, character_power)
		B2_DECLARE_FIELD(1, TArray<int32>, required_tracking_targets)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTrackingPowerDetail)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, item_preset_id)
		B2_DECLARE_FIELD(2, int32, skill_slot)
		B2_DECLARE_FIELD(3, TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
		B2_DECLARE_FIELD(4, int32, different_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoMessageProcessing)
		B2_DECLARE_FIELD(0, FString, message_box_id)
		B2_DECLARE_FIELD(1, int32, processing_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoCouponUse)
		B2_DECLARE_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoAccountConversionPrepare)
		B2_DECLARE_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoAccountConversionComplete)
		B2_DECLARE_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDonationEvent)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, free_donation_count)
		B2_DECLARE_FIELD(2, int32, point)
		B2_DECLARE_FIELD(3, int32, whole_step)
		B2_DECLARE_FIELD(4, TArray<B2DonationPointRewardStatePtr>, point_reward_state)
		B2_DECLARE_FIELD(5, bool, settled)
		B2_DECLARE_FIELD(6, TArray<B2MatchRankerPtr>, rankers)
		B2_DECLARE_FIELD(7, int32, from_ranking)
		B2_DECLARE_FIELD(8, int32, total_ranker_count)
		B2_DECLARE_FIELD(9, B2RankingPtr, ranking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMakeDonation)
		B2_DECLARE_FIELD(0, bool, is_free_donation)
		B2_DECLARE_FIELD(1, int32, free_donation_count)
		B2_DECLARE_FIELD(2, TArray<B2DonationPresentPtr>, donation_presents)
		B2_DECLARE_FIELD(3, int32, decrease_gem)
		B2_DECLARE_FIELD(4, int32, current_gem)
		B2_DECLARE_FIELD(5, int32, decrease_gold)
		B2_DECLARE_FIELD(6, int32, current_gold)
		B2_DECLARE_FIELD(7, int32, obtain_point)
		B2_DECLARE_FIELD(8, int32, current_point)
		B2_DECLARE_FIELD(9, FString, last_donation_key)
		B2_DECLARE_FIELD(10, B2RawRewardPtr, bundled_fixed_reward)
		B2_DECLARE_FIELD(11, TArray<B2ItemServerInfoPtr>, items)
		B2_DECLARE_FIELD(12, int32, present_gem)
		B2_DECLARE_FIELD(13, int32, present_blade_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(14)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDonationPointReward)
		B2_DECLARE_FIELD(0, TArray<B2RawRewardPtr>, rewards)
		B2_DECLARE_FIELD(1, TArray<B2DonationPointRewardStatePtr>, point_reward_state)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFairy)
		B2_DECLARE_FIELD(0, TArray<B2FairyStatusPtr>, fairies)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelupFairy)
		B2_DECLARE_FIELD(0, B2FairyStatusPtr, updated_fairy)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, changed_material_item)
		B2_DECLARE_FIELD(2, int64, deleted_item_id)
		B2_DECLARE_FIELD(3, bool, is_levelup)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBlessFairy)
		B2_DECLARE_FIELD(0, B2FairyStatusPtr, updated_fairy)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveFairyGift)
		B2_DECLARE_FIELD(0, bool, jackpoted)
		B2_DECLARE_FIELD(1, int32, multiple_value)
		B2_DECLARE_FIELD(2, int32, fairy_type)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
		B2_DECLARE_FIELD(4, int64, next_fairy_gift_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckChargePoints)
		B2_DECLARE_FIELD(0, int32, blade_point)
		B2_DECLARE_FIELD(1, int32, blade_point_max)
		B2_DECLARE_FIELD(2, int32, blade_point_charge_lefttime_second)
		B2_DECLARE_FIELD(3, int32, pvp_match_point)
		B2_DECLARE_FIELD(4, int32, pvp_match_point_charge_lefttime_second)
		B2_DECLARE_FIELD(5, int32, team_match_point)
		B2_DECLARE_FIELD(6, int32, team_match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFeaturedMission)
		B2_DECLARE_FIELD(0, TArray<B2MissionInfoPtr>, mission_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetEvent)
		B2_DECLARE_FIELD(0, TArray<B2PointEventStatusPtr>, point_event_statuses);
		B2_DECLARE_FIELD(1, TArray<B2LevelUpEventInfoPtr>, level_up_event_infos);
		B2_DECLARE_FIELD(2, TArray<B2StageClearEventStatusPtr>, stage_clear_event_statuses);
		B2_DECLARE_FIELD(3, TArray<B2PointShopEventStatusPtr>, point_shop_event_statuses);
		B2_DECLARE_FIELD(4, TArray<B2TenLotteryEventStatusPtr>, ten_lottery_event_statuses);
		B2_DECLARE_FIELD(5, TArray<B2GemPurchaseEventStatusPtr>, gem_purchase_event_statuses);
		B2_DECLARE_FIELD(6, TArray<B2SpendGemEventStatusPtr>, spend_gem_event_statuses);
		B2_DECLARE_FIELD(7, TArray<B2TenLotterySavingsStatusPtr>, ten_lottery_savings_event_statuses);
		B2_DECLARE_FIELD(8, TArray<B2DiceEventStatusPtr>, dice_event_statuses);
		B2_DECLARE_FIELD(9, TArray<B2PointShopEventRewardHistoryPtr>, point_shop_event_reward_histories);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceivePointEventReward)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveLevelUpEventReward)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetSeasonMission)
		B2_DECLARE_FIELD(0, TArray<B2SeasonMissionInfoPtr>, missions);
		B2_DECLARE_FIELD(1, TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardSeasonMission)
		B2_DECLARE_FIELD(0, B2SeasonMissionInfoPtr, reward_mission);
		B2_DECLARE_FIELD(1, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAttendSeasonMission)
		B2_DECLARE_FIELD(0, TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveStageClearEventReward)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyHotTimeBuff)
		B2_DECLARE_FIELD(0, int32, buff_type);
		B2_DECLARE_FIELD(1, int64, buff_end_time);
		B2_DECLARE_FIELD(2, int32, buff_ticket); 
		B2_DECLARE_FIELD(3, int32, current_gem);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardFlatRatePackageBonus)
		B2_DECLARE_FIELD(0, B2RewardPtr, received_reward);
		B2_DECLARE_FIELD(1, B2FlatRatePackageBonusStatePtr, next_bonus_state);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetPackageShop)
		B2_DECLARE_FIELD(0, TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
		B2_DECLARE_FIELD(1, B2FlatRatePackageBonusStatePtr, flat_rate_package_bonus_state);
		B2_DECLARE_FIELD(2, TArray<B2LevelUpPackageMissionPtr>, level_up_package_missions);
		B2_DECLARE_FIELD(3, TArray<B2ReachLevelPackageMissionPtr>, reach_level_package_missions);
		B2_DECLARE_FIELD(4, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		B2_DECLARE_FIELD(5, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetPeriodPackageState)
		B2_DECLARE_FIELD(0, TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardLevelUpPackage)
		B2_DECLARE_FIELD(0, B2RewardPtr, received_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardReachLevelPackage)
		B2_DECLARE_FIELD(0, B2RewardPtr, received_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveUpQuest)
		B2_DECLARE_FIELD(0, B2QuestInfoPtr, next_quest_info);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetContentsModeState)
		B2_DECLARE_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardPointShopEvent)
		B2_DECLARE_FIELD(0, B2RewardPtr, reward);
		B2_DECLARE_FIELD(1, B2PointShopEventStatusPtr, point_shop_event_status);
		B2_DECLARE_FIELD(2, int32, index);
		B2_DECLARE_FIELD(3, int32, total_reward_count);	// 횟수 제한이 있는 경우만 누적카운트 유효
		B2_DECLARE_FIELD(4, int32, buy_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReportUser)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBlockChat)
		B2_DECLARE_FIELD(0, B2BlockChatUserPtr, blocked_user);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnblockChat)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetBlockChatList)
		B2_DECLARE_FIELD(0, TArray<B2BlockChatUserPtr>, blocked_users);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseClientStateReport)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardTenLotteryEvent)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, rewards);
		B2_DECLARE_FIELD(1, int32, reward_index);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveGemPurchaseEventReward)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardSpendGemEvent)
		B2_DECLARE_FIELD(0, TArray<B2RewardPtr>, rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardTenLotterySavingsEvent)
		B2_DECLARE_FIELD(0, int32, current_point)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRollDiceEvent)
		B2_DECLARE_FIELD(0, B2RewardPtr, cell_reward)
		B2_DECLARE_FIELD(1, B2RewardPtr, finish_reward)
		B2_DECLARE_FIELD(2, int32, current_position)
		B2_DECLARE_FIELD(3, int32, dice_number)
		B2_DECLARE_FIELD(4, int32, dice_cell_type)
		B2_DECLARE_FIELD(5, bool, is_finish)
		B2_DECLARE_FIELD(6, int32, reward_count_times)
		B2_DECLARE_FIELD(7, int32, free_dice_count)
		B2_DECLARE_FIELD(8, int32, finish_count)
		B2_DECLARE_FIELD(9, int32, roll_dice_count)
		B2_DECLARE_FIELD(10, int32, current_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyDiceEventPoint)
		B2_DECLARE_FIELD(0, int32, current_gem)
		B2_DECLARE_FIELD(1, int32, buy_count)
		B2_DECLARE_FIELD(2, int32, point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardDiceEventMileage)
		B2_DECLARE_FIELD(0, int32, reward_value)
		B2_DECLARE_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	// REPLACEMENT POINT: MESSAGE_INFO HEADER RESPONSE
	// NEVER DELETE LINE ABOVE

	// for console command

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteAccount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddItemToInventory)
		B2_DECLARE_FIELD(0, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddCostumeToInventory)
		B2_DECLARE_FIELD(0, B2CostumeServerInfoPtr, costume)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddTotemToInventory)
		B2_DECLARE_FIELD(0, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseCashPaid)
		B2_DECLARE_FIELD(0, int32, cash_paid)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseMoney)
		B2_DECLARE_FIELD(0, int32, money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseSocialPoint)
		B2_DECLARE_FIELD(0, int32, current_social_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetItemLevelup)
		B2_DECLARE_FIELD(0, B2ItemServerInfoPtr, item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterLevelExp)
		B2_DECLARE_FIELD(0, B2CharacterServerInfoPtr, character)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterRankInfo)
		B2_DECLARE_FIELD(0, B2CharacterServerInfoPtr, character)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterRelic)
		B2_DECLARE_FIELD(0, B2RelicPtr, relic)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetBladePoint)
		B2_DECLARE_FIELD(0, int32, blade_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseRankPromotionPoint)
		B2_DECLARE_FIELD(0, int32, rank_promotion_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddMail)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMakeRanking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceClearStage)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetQuestState)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseHeroPiece)
		B2_DECLARE_FIELD(0, int32, current_hero_piece)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceClearHeroTower)
		B2_DECLARE_FIELD(0, int32, start_floor)
		B2_DECLARE_FIELD(1, int32, max_clear_floor)
		B2_DECLARE_FIELD(2, int32, main_character_type)
		B2_DECLARE_FIELD(3, int32, tag_character_type)
		B2_DECLARE_FIELD(4, int32, daily_try_count)
		B2_DECLARE_FIELD(5, int32, daily_max_free_try_count)
		B2_DECLARE_FIELD(6, int32, daily_left_free_try_count)
		B2_DECLARE_FIELD(7, int32, current_hero_piece)
		B2_DECLARE_FIELD(8, bool, is_sweep)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceStartCounterDungeon)
		B2_DECLARE_FIELD(0, FString, play_token);
		B2_DECLARE_FIELD(1, int32, drop_gold);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareSweepHeroTower)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildBattleState)
		B2_DECLARE_FIELD(0, B2GuildBattleStatePtr, state);
		B2_DECLARE_FIELD(1, FString, state_message);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetAttendanceTime)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetEventAttendance)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()
		
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetDailyPlayTimeRecord)
		B2_DECLARE_FIELD(0, int32, daily_play_time_in_sec)
		B2_DECLARE_FIELD(1, int32, daily_play_time_reward_index)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetTeamMatchPoint)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseChangeQuestId)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseChangeQuestCount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareFriend)
		B2_DECLARE_FIELD(0, int32, new_friend_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildLevelExp)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddAetherToInventory)
		B2_DECLARE_FIELD(0, B2AetherServerInfoPtr, aether)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceUpdateAether)
		B2_DECLARE_FIELD(0, B2AetherServerInfoPtr, updated_aether)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetMatchSeasonReward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetTutorialProgress)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, bool, is_item_prepared)
		B2_DECLARE_FIELD(2, bool, is_lottery_drawn)
		B2_DECLARE_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetStageBoostCount)
		B2_DECLARE_FIELD(0, int32, stage_boost_ticket)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSkipMatchTutorial)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetDebugMode)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseServerCmd)
		B2_DECLARE_FIELD(0, FString, result_message)
		B2_DECLARE_FIELD(1, bool, need_restart)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTrace)
		B2_DECLARE_FIELD(0, FString, tag)
		B2_DECLARE_FIELD(1, int64, client_send_time)
		B2_DECLARE_FIELD(2, int64, session_recv_time)
		B2_DECLARE_FIELD(3, int64, app_recv_time)
		B2_DECLARE_FIELD(4, int64, db_request_time)
		B2_DECLARE_FIELD(5, int64, db_response_time)
		B2_DECLARE_FIELD(6, int64, app_send_time)
		B2_DECLARE_FIELD(7, int64, session_send_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	// totem
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountTotem)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountTotem)
		B2_DECLARE_FIELD(0, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
		B2_DECLARE_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, equiped_totem_ids)
		B2_DECLARE_FIELD(1, TArray<int64>, unequiped_totem_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, unequiped_totem_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)	

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockTotems)
		B2_DECLARE_FIELD(0, TArray<B2TotemServerInfoPtr>, locked_totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockTotems)
		B2_DECLARE_FIELD(0, TArray<B2TotemServerInfoPtr>, unlocked_totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellTotems)
		B2_DECLARE_FIELD(0, int32, current_gold)
		B2_DECLARE_FIELD(1, TArray<int64>, sell_totem_ids)
		B2_DECLARE_FIELD(2, int32, obtain_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleTotems)
		B2_DECLARE_FIELD(0, TArray<int64>, dismantle_totem_ids)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, current_dimensional_piece)
		B2_DECLARE_FIELD(2, int32, obtain_dimensional_piece_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRefineTotem)
		B2_DECLARE_FIELD(0, int64, totem_id)
		B2_DECLARE_FIELD(1, int32, option_id)
		B2_DECLARE_FIELD(2, int32, cost_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRefineTotem)
		B2_DECLARE_FIELD(0, int32, current_gold)
		B2_DECLARE_FIELD(1, B2TotemServerInfoPtr, refined_totem)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_dimensional_piece_items)
		B2_DECLARE_FIELD(3, TArray<int64>, deleted_dimensional_piece_ids)
		B2_DECLARE_FIELD(4, bool, is_success)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyTotemNewFlag)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyTotemNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()


    //////////////////////////////////////////////////////////////////////////////
    // chatting channel

    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoGetChatChannelRequest)
    DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoGetChatChannelResponse)
        B2_DECLARE_FIELD(0, int32, channel_number)
    DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

    DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoChangeChatChannelRequest)
        B2_DECLARE_FIELD(0, int32, channel_number)
    DECLARE_B2MESSAGEINFO_REQUEST_END(1)

    DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoChangeChatChannelResponse)
    DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	
	//////////////////////////////////////////////////////////////////////////
	// item random quality
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemRandomQuality)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemRandomQuality)
		B2_DECLARE_FIELD(0, TArray<B2ItemServerInfoPtr>, main_item_infos)
		B2_DECLARE_FIELD(1, int64, item_id)
		B2_DECLARE_FIELD(2, int32, quality)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGenerateItemRandomQuality)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGenerateItemRandomQuality)
		B2_DECLARE_FIELD(0, int64, main_item_id)
		B2_DECLARE_FIELD(1, int64, material_item_id)
		B2_DECLARE_FIELD(2, int32, quality)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestApplyItemRandomQuality)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseApplyItemRandomQuality)
		B2_DECLARE_FIELD(0, B2ItemServerInfoPtr, main_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDiscardItemRandomQuality)
		B2_DECLARE_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDiscardItemRandomQuality)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()


	//////////////////////////////////////////////////////////////////////////
	// factory
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFactory)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFactory)
		B2_DECLARE_FIELD(0, TArray<B2FactoryPeriodicStatusPtr>, factory_periodic_status)
		B2_DECLARE_FIELD(1, TArray<B2FactoryLimitStatusPtr>, limit_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeFactoryItem)
		B2_DECLARE_FIELD(0, int64, factory_id)
		B2_DECLARE_FIELD(1, int64, make_count)
		B2_DECLARE_FIELD(2, int64, main_item_id)
		B2_DECLARE_FIELD(3, int64, material_item_id1)
		B2_DECLARE_FIELD(4, int64, material_item_id2)
		B2_DECLARE_FIELD(5, int64, material_item_id3)
		B2_DECLARE_FIELD(6, int64, material_item_id4)
		B2_DECLARE_FIELD(7, int64, main_item_template_id)
		B2_DECLARE_FIELD(8, int64, material_item_template_id1)
		B2_DECLARE_FIELD(9, int64, material_item_template_id2)
		B2_DECLARE_FIELD(10, int64, material_item_template_id3)
		B2_DECLARE_FIELD(11, int64, material_item_template_id4)
	DECLARE_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMakeFactoryItem)
		B2_DECLARE_FIELD(0, int64, factory_id)
		B2_DECLARE_FIELD(1, int64, main_item_id)
		B2_DECLARE_FIELD(2, int64, material_item_id1)
		B2_DECLARE_FIELD(3, int64, material_item_id2)
		B2_DECLARE_FIELD(4, int64, material_item_id3)
		B2_DECLARE_FIELD(5, int64, material_item_id4)
		B2_DECLARE_FIELD(6, TArray<B2FactoryResultPtr>, make_result)
		B2_DECLARE_FIELD(7, int64, left_main_item_count)
		B2_DECLARE_FIELD(8, int64, left_material_item_count1)
		B2_DECLARE_FIELD(9, int64, left_material_item_count2)
		B2_DECLARE_FIELD(10, int64, left_material_item_count3)
		B2_DECLARE_FIELD(11, int64, left_material_item_count4)
		B2_DECLARE_FIELD(12, int64, current_gold)
		B2_DECLARE_FIELD(13, int64, current_gem)
		B2_DECLARE_FIELD(14, int64, current_dark_crystal)
		B2_DECLARE_FIELD(15, int64, cost_gold)
		B2_DECLARE_FIELD(16, int64, cost_gem)
		B2_DECLARE_FIELD(17, int64, cost_dark_crystal)
		B2_DECLARE_FIELD(18, int64, left_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(19)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnsealBox)
		B2_DECLARE_FIELD(0, int64, seal_box_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int64, unseal_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnsealBox)
		B2_DECLARE_FIELD(0, TArray<B2UnsealBoxRewardPtr>, rewards)
		B2_DECLARE_FIELD(1, int64, seal_box_id)
		B2_DECLARE_FIELD(2, int64, left_amount)		
		B2_DECLARE_FIELD(3, int32, gold);
		B2_DECLARE_FIELD(4, int32, gem);
		B2_DECLARE_FIELD(5, int32, blade_point);
		B2_DECLARE_FIELD(6, int32, social_point);
		B2_DECLARE_FIELD(7, int32, dark_crystal);
		B2_DECLARE_FIELD(8, int32, stage_boost_ticket);
		B2_DECLARE_FIELD(9, int32, stage_auto_clear_ticket);
		B2_DECLARE_FIELD(10, int32, shop_ten_lottery_ticket);
		B2_DECLARE_FIELD(11, int32, fame_point);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(12)
};
