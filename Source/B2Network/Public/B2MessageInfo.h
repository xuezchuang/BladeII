#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
//#include "Config.h"
#include "B2MessageInfoCommonStruct.h"

//////////////////////////////////////////////////////////////////////////
// forward declare
namespace b2network
{
	class Client;
	class FB2MessageInfo;
	class FB2MessageInfoNotify;
	class FB2MessageInfoCommand;
	
	struct b2network_Message
	{
	};

	typedef b2network_Message Message;
	typedef std::shared_ptr<Message> MessagePtr;
}

typedef std::shared_ptr<b2network::FB2MessageInfo> FB2MessageInfoPtr;
typedef std::function<void(FB2MessageInfoPtr)> FB2MessageCallback;

typedef std::shared_ptr<b2network::FB2MessageInfoNotify> FB2MessageInfoNotifyPtr;
typedef std::shared_ptr<b2network::FB2MessageInfoCommand> FB2MessageInfoCommandPtr;

namespace b2network
{
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

		std::string& GetType() {
			return MessageInfoType;
		}

		bool IsSuccess() {
			return (ErrorCode == B2RESULT_SUCCESS);
		}
		EB2ResultCode GetErrorCode() {
			return ErrorCode;
		}
		void SetErrorCode(int InErrorCode) {
			ErrorCode = static_cast<EB2ResultCode>(InErrorCode);
		};

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
		virtual FB2MessageCallback GetSuccessCallback() override {
			return nullptr;
		}
		virtual FB2MessageCallback GetFailCallback() override {
			return nullptr;
		}
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
		virtual FB2MessageCallback GetSuccessCallback() override {
			return nullptr;
		}
		virtual FB2MessageCallback GetFailCallback() override {
			return nullptr;
		}
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
	DECLARE_DF_FIELD(0, TArray<B2FindAccountQueryPtr>, query_parameters)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountItem)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountCostume)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMasterData)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetLatestEventData)
	DECLARE_DF_FIELD(0, int64, last_update_time)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSignUp)
	DECLARE_DF_FIELD(0, FString, name)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellItem)
	DECLARE_DF_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConvertData)
	DECLARE_DF_FIELD(0, int32, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConvertItem)
	DECLARE_DF_FIELD(0, int32, item_id)
	DECLARE_DF_FIELD(1, int32, convert_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellConsumableItem)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, sell_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipItem)
	DECLARE_DF_FIELD(0, int32, preset_id)
	DECLARE_DF_FIELD(1, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipItem)
	DECLARE_DF_FIELD(0, int32, preset_id)
	DECLARE_DF_FIELD(1, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipCostume)
	DECLARE_DF_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipCostume)
	DECLARE_DF_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceCostume)
	DECLARE_DF_FIELD(0, int64, costume_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleCostumes)
	DECLARE_DF_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockCostumes)
	DECLARE_DF_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockCostumes)
	DECLARE_DF_FIELD(0, TArray<int64>, costume_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockItem)
	DECLARE_DF_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockItem)
	DECLARE_DF_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExtendInventory)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, extend_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelupItem)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, TArray<int64>, material_item_ids)
	DECLARE_DF_FIELD(2, bool, auto_levelup)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceItem)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, scroll_item_id)
	DECLARE_DF_FIELD(2, bool, use_protection)
	DECLARE_DF_FIELD(3, int32, protection_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleItems)
	DECLARE_DF_FIELD(0, TArray<int64>, item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExchangeItemEnhanceLevel)
	DECLARE_DF_FIELD(0, int64, item_id_1)
	DECLARE_DF_FIELD(1, int64, item_id_2)
	DECLARE_DF_FIELD(2, int32, price_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenSealSlotItem)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, seal_slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCarveSealItem)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, seal_slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSurpassItem)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpgradeItem)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestComposeItem)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int64, sub_item_id)
	DECLARE_DF_FIELD(2, int32, current_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeItemCharacterType)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetChangeItemCharacterTypeCost)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_DF_FIELD(2, int32, tag_character_type)
	DECLARE_DF_FIELD(3, int32, selected_option_flags)
	DECLARE_DF_FIELD(4, int32, stage_repeat_mode)
	DECLARE_DF_FIELD(5, bool, boost_mode)
	DECLARE_DF_FIELD(6, int32, tutorial_id)
	DECLARE_DF_FIELD(7, int32, main_character_preset_id)
	DECLARE_DF_FIELD(8, int32, tag_character_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestClearStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_DF_FIELD(1, FString, token)
	DECLARE_DF_FIELD(2, int32, main_character_type)
	DECLARE_DF_FIELD(3, int32, tag_character_type)
	DECLARE_DF_FIELD(4, int32, clear_time_second)
	DECLARE_DF_FIELD(5, TArray<B2KillMonsterInfoPtr>, kill_monster_infos)
	DECLARE_DF_FIELD(6, int32, total_monster_kill_count)
	DECLARE_DF_FIELD(7, bool, is_all_survival)
	DECLARE_DF_FIELD(8, int32, qte_start_count)
	DECLARE_DF_FIELD(9, int32, qte_finish_count)
	DECLARE_DF_FIELD(10, B2ItemSpotSaleOptionPtr, item_sale_option)
	DECLARE_DF_FIELD(11, B2AetherSpotSaleOptionPtr, aether_sale_option)
	DECLARE_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_DF_FIELD(2, int32, tag_character_type)
	DECLARE_DF_FIELD(3, int32, main_character_preset_id)
	DECLARE_DF_FIELD(4, int32, tag_character_preset_id)
	DECLARE_DF_FIELD(5, int32, repeat_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFailStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_DF_FIELD(1, FString, token)
	DECLARE_DF_FIELD(2, int32, reason)
	DECLARE_DF_FIELD(3, int32, play_time_second)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResurrection)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_DF_FIELD(1, int32, selected_buff_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckBladePointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemSurpassCost)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemUpgradeCost)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemComposeCost)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int32, current_preset_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetActInfo)
	DECLARE_DF_FIELD(0, int32, difficulty)
	DECLARE_DF_FIELD(1, int32, act_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetStageInfo)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelUpSkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, skill_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetSkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetUsingSkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, slot)
	DECLARE_DF_FIELD(2, int32, using_skill_id_1)
	DECLARE_DF_FIELD(3, int32, using_skill_id_2)
	DECLARE_DF_FIELD(4, int32, using_skill_id_3)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuySkillPoint)
	DECLARE_DF_FIELD(0, TArray<B2BuySkillPointRequestInfoPtr>, buy_infos)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveNickname)
	DECLARE_DF_FIELD(0, FString, nickname)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateNickname)
	DECLARE_DF_FIELD(0, FString, nickname)
	DECLARE_DF_FIELD(1, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetServerVersion)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD();

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveCharacterName)
	DECLARE_DF_FIELD(0, int32, character_type);
	DECLARE_DF_FIELD(1, FString, name);
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveActRepeatClearReward)
	DECLARE_DF_FIELD(0, int32, difficulty)
	DECLARE_DF_FIELD(1, int32, act_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveActClearReward)
	DECLARE_DF_FIELD(0, int32, difficulty)
	DECLARE_DF_FIELD(1, int32, act_id)
	DECLARE_DF_FIELD(2, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetTeamBattleStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAssaultBattleStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateTeamBattleEntry)
	DECLARE_DF_FIELD(0, B2TeamBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceTeamBattleFormation)
	DECLARE_DF_FIELD(0, int32, formation_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTeamMatchmaking)
	DECLARE_DF_FIELD(0, bool, use_additional_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTeamMatchResult)
	DECLARE_DF_FIELD(0, FString, match_token)
	DECLARE_DF_FIELD(1, int32, match_result)
	DECLARE_DF_FIELD(2, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRank)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceRankNode)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMailList)
	DECLARE_DF_FIELD(0, int32, mail_category)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenMail)
	DECLARE_DF_FIELD(0, int64, mail_id)
	DECLARE_DF_FIELD(1, int32, lottery_character_type)
	DECLARE_DF_FIELD(2, int32, lottery_item_template_id)
	DECLARE_DF_FIELD(3, int32, lottery_aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenAllMail)
	DECLARE_DF_FIELD(0, int32, mail_category)
	DECLARE_DF_FIELD(1, int64, start_id)
	DECLARE_DF_FIELD(2, int64, end_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetNewMailCount)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskFriend)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptAskFriend)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRejectAskFriend)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFriendList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelAskFriend)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDeleteFriend)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAllSendAskFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAllReceiveAskFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSendSocialPoint)
	DECLARE_DF_FIELD(0, TArray<int64>, account_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRecommendFriend)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFindUser)
	DECLARE_DF_FIELD(0, FString, nickname)
	DECLARE_DF_FIELD(1, bool, guild_invite_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestConfirmNewFriend)
	DECLARE_DF_FIELD(0, bool, is_all_confirm)				// true 면 모든 미확인 대상, false : 지정된 대상.	
		DECLARE_DF_FIELD(1, TArray<int64>, friend_ids)			// 친구 확인 목록 ( case is_all_confirm = false ) 
		DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteInfo)
	DECLARE_DF_FIELD(0, int32, platform_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteCountUp)
	DECLARE_DF_FIELD(0, int32, platform_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendJoinerCountUp)
	DECLARE_DF_FIELD(0, int32, platform_type)
	DECLARE_DF_FIELD(1, int32, joiner_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPlatformFriendInviteReward)
	DECLARE_DF_FIELD(0, int32, platform_type)
	DECLARE_DF_FIELD(1, int32, reward_type)
	DECLARE_DF_FIELD(2, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetQuest)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptQuest)
	DECLARE_DF_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveQuestReward)
	DECLARE_DF_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetCounterDungeon)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartCounterDungeon)
	DECLARE_DF_FIELD(0, int32, dungeon_id)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishCounterDungeon)
	DECLARE_DF_FIELD(0, int32, dungeon_id)
	DECLARE_DF_FIELD(1, FString, play_token)
	DECLARE_DF_FIELD(2, bool, is_success)
	DECLARE_DF_FIELD(3, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepCounterDungeon)
	DECLARE_DF_FIELD(0, int32, dungeon_id)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyCounterDungeonTicket)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCounterDungeonWeeklyReward)
	DECLARE_DF_FIELD(0, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDimension)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartDimension)
	DECLARE_DF_FIELD(0, int32, difficulty_level)
	DECLARE_DF_FIELD(1, int32, dungeon_id)
	DECLARE_DF_FIELD(2, int32, main_character_type)
	DECLARE_DF_FIELD(3, int32, total_power)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishDimension)
	DECLARE_DF_FIELD(0, int32, difficulty_level)
	DECLARE_DF_FIELD(1, FString, play_token)
	DECLARE_DF_FIELD(2, int32, play_result)
	DECLARE_DF_FIELD(3, int32, play_time_millis)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCreateGuild)
	DECLARE_DF_FIELD(0, FString, guild_name)
	DECLARE_DF_FIELD(1, int32, join_option)
	DECLARE_DF_FIELD(2, int32, mark_index)
	DECLARE_DF_FIELD(3, int32, mark_color)
	DECLARE_DF_FIELD(4, int32, mark_bg_index)
	DECLARE_DF_FIELD(5, int32, mark_bg_color)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskJoinGuild)
	DECLARE_DF_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLeaveGuild)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildDetailInfo)
	DECLARE_DF_FIELD(0, int64, guild_id)
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
	DECLARE_DF_FIELD(0, bool, need_friend_list)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestInviteGuild)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildInviteList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRejectInviteGuild)
	DECLARE_DF_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAcceptInviteGuild)
	DECLARE_DF_FIELD(0, int64, guild_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFindGuild)
	DECLARE_DF_FIELD(0, FString, guild_name)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestObtainGuildMaster)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestHandOverGuildMaster)
	DECLARE_DF_FIELD(0, int64, account_id);
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildRankingList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAppointSubGuildMaster)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismissSubGuildMaster)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKickGuildMember)
	DECLARE_DF_FIELD(0, int64, account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateGuild)
	DECLARE_DF_FIELD(0, int32, join_option)
	DECLARE_DF_FIELD(1, int32, mark_index)
	DECLARE_DF_FIELD(2, int32, mark_color)
	DECLARE_DF_FIELD(3, int32, mark_bg_index)
	DECLARE_DF_FIELD(4, int32, mark_bg_color)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateGuildNotice)
	DECLARE_DF_FIELD(0, FString, notice)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelUpGuildSkill)
	DECLARE_DF_FIELD(0, int32, skill_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyGuildSkill)
	DECLARE_DF_FIELD(0, int32, skill_id)
	DECLARE_DF_FIELD(1, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMission)
	DECLARE_DF_FIELD(0, int32, mission_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveMissionReward)
	DECLARE_DF_FIELD(0, int32, mission_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveCompleteMissionReward)
	DECLARE_DF_FIELD(0, int32, mission_type)
	DECLARE_DF_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetWing)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceWing)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpgradeWing)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceWingOption)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, option_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMissionAlarmNotify)
	DECLARE_DF_FIELD(0, int32, completed_quest_id)
	DECLARE_DF_FIELD(1, int32, completed_quest_complete_step)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartHeroTower)
	DECLARE_DF_FIELD(0, int32, floor)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_DF_FIELD(2, int32, tag_character_type)
	DECLARE_DF_FIELD(3, FString, play_token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishHeroTower)
	DECLARE_DF_FIELD(0, int32, floor)
	DECLARE_DF_FIELD(1, FString, play_token)
	DECLARE_DF_FIELD(2, int32, clear_time_sec)
	DECLARE_DF_FIELD(3, bool, is_success)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAbandonHeroTower)
	DECLARE_DF_FIELD(0, int32, floor)
	DECLARE_DF_FIELD(1, FString, play_token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceRelic)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, relic_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPromotionRelic)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, relic_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountRelic)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSweepHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyHeroTowerTicket)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDrawShopLottery)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, draw_price_type)
	DECLARE_DF_FIELD(2, bool, continuous_draw)
	DECLARE_DF_FIELD(3, bool, is_draw_again)
	DECLARE_DF_FIELD(4, FString, last_lottery_key)
	DECLARE_DF_FIELD(5, bool, is_free_lottery)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardShopMileage)
	DECLARE_DF_FIELD(0, int32, shop_mileage_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetLotteryShop)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyShopProduct)
	DECLARE_DF_FIELD(0, int32, product_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPurchaseCostume)
	DECLARE_DF_FIELD(0, int32, product_id)
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
	DECLARE_DF_FIELD(0, int32, id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetCollectionItem)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, equip_category)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardCollectionItem)
	DECLARE_DF_FIELD(0, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardCollectionSetItem)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, grade)
	DECLARE_DF_FIELD(2, int32, group_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMarketProduct)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPreparePurchase)
	DECLARE_DF_FIELD(0, int32, product_id)
	DECLARE_DF_FIELD(1, FString, market_product_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestVerifyPurchase)
	DECLARE_DF_FIELD(0, FString, market_product_id)
	DECLARE_DF_FIELD(1, FString, developer_payload)
	DECLARE_DF_FIELD(2, FString, purchase_data)
	DECLARE_DF_FIELD(3, FString, purchase_signature)
	DECLARE_DF_FIELD(4, bool, is_restore)
	DECLARE_DF_FIELD(5, FString, currency_code)
	DECLARE_DF_FIELD(6, float, price)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelPurchase)
	DECLARE_DF_FIELD(0, FString, market_product_id)
	DECLARE_DF_FIELD(1, FString, developer_payload)
	DECLARE_DF_FIELD(2, int32, cancel_reason)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishPurchase)
	DECLARE_DF_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRaidResult)
	DECLARE_DF_FIELD(0, FString, raid_token)
	DECLARE_DF_FIELD(1, int32, ranking)
	DECLARE_DF_FIELD(2, bool, is_bonus_player)
	DECLARE_DF_FIELD(3, bool, is_reward)
	DECLARE_DF_FIELD(4, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDailyAttendance)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEventAttendance)
	DECLARE_DF_FIELD(0, int32, eventId)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBeginnerAttendance)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateDailyPlayTime)
	DECLARE_DF_FIELD(0, int32, play_time_in_sec)
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
	DECLARE_DF_FIELD(0, int64, target_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestFinishGuildBattle)
	DECLARE_DF_FIELD(0, int64, target_account_id)
	DECLARE_DF_FIELD(1, FString, battle_token)
	DECLARE_DF_FIELD(2, int32, result)
	DECLARE_DF_FIELD(3, int32, round_win_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildBattleEntry)
	DECLARE_DF_FIELD(0, int32, slot_1)
	DECLARE_DF_FIELD(1, int32, slot_2)
	DECLARE_DF_FIELD(2, int32, slot_3)
	DECLARE_DF_FIELD(3, int32, slot_4)
	DECLARE_DF_FIELD(4, int32, slot_5)
	DECLARE_DF_FIELD(5, int32, slot_6)
	DECLARE_DF_FIELD(6, int32, slot_7)
	DECLARE_DF_FIELD(7, int32, slot_8)
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
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetHelmetVisible)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCostumeVisible)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, costume_type)
	DECLARE_DF_FIELD(2, bool, is_visible)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetRaid)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRaidResurrection)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_DF_FIELD(1, int, selected_buff_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeQuestId)
	DECLARE_DF_FIELD(0, int32, slot_type)
	DECLARE_DF_FIELD(1, int32, quest_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestChangeQuestCount)
	DECLARE_DF_FIELD(0, int32, slot_type)
	DECLARE_DF_FIELD(1, int32, count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultMatchmaking)
	DECLARE_DF_FIELD(0, int32, battle_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCancelAssaultMatchmaking)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultResult)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_DF_FIELD(1, int32, match_result)
	DECLARE_DF_FIELD(2, bool, is_mvp_player)
	DECLARE_DF_FIELD(3, bool, is_ghost_user)
	DECLARE_DF_FIELD(4, B2AssaultBattleInfoPtr, battle_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAssaultResurrection)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckPvPMatchPointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckTeamMatchPointCharge)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFriendRankings)
	DECLARE_DF_FIELD(0, int32, opponent_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemRandomOption)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGenerateItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, price_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestApplyItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDiscardItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckRedDot)
	DECLARE_DF_FIELD(0, TArray<int32>, red_dot_hint)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseDarkCrystalPaid)
	DECLARE_DF_FIELD(0, int32, dark_crystal_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, slot_num)
	DECLARE_DF_FIELD(2, int32, random_option_id)
	DECLARE_DF_FIELD(3, float, random_option_value)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyItemNewFlag)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyCostumeNewFlag)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, inventory_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestStartRaid)
	DECLARE_DF_FIELD(0, int32, battle_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetContentsOpenStatus)
	DECLARE_DF_FIELD(0, int32, link_key)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelRewardBoxInfo)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelRewardBoxGainableList)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_DF_FIELD(1, int32, grade)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestOpenDuelRewardBox)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveAssaultMvpReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountAether)
	DECLARE_DF_FIELD(0, int32, aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMountAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnmountAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSelectAetherDefenseOption)
	DECLARE_DF_FIELD(0, int32, set_option_id)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSelectAetherUnitySkillOption)
	DECLARE_DF_FIELD(0, int32, set_option_id)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestExtendAetherInventory)
	DECLARE_DF_FIELD(0, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEnhanceAether)
	DECLARE_DF_FIELD(0, int64, aether_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, aether_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestModifyAetherNewFlag)
	DECLARE_DF_FIELD(0, int32, aether_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetTutorialProgress)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, bool, is_item_prepared)
	DECLARE_DF_FIELD(2, bool, is_lottery_drawn)
	DECLARE_DF_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUpdateTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSkipTutorialProgress)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDrawTutorialItem)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareTutorial)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveUnitySkillReward)
	DECLARE_DF_FIELD(0, int32, main_character_type)
	DECLARE_DF_FIELD(1, int32, unity_character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetUnitySkillMission)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAwakenUnitySkillMission)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, mission_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAwakenUnitySkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDuelModeInfo)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetChallengeModeInfo)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetHallOfFame)
	DECLARE_DF_FIELD(0, int32, mode)
	DECLARE_DF_FIELD(1, bool, need_user_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPraiseTargetRanker)
	DECLARE_DF_FIELD(0, int32, mode)
	DECLARE_DF_FIELD(1, int64, account_id)
	DECLARE_DF_FIELD(2, int32, ranking)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveRestReward)
	DECLARE_DF_FIELD(0, int32, reward_id)
	DECLARE_DF_FIELD(1, bool, isDouble)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckModeOpen)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveTutorialReward)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckReachPower)
	DECLARE_DF_FIELD(0, TArray<B2CharacterPowerPtr>, character_power)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTrackingPowerDetail)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, item_preset_id)
	DECLARE_DF_FIELD(2, int32, skill_slot)
	DECLARE_DF_FIELD(3, TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoMessageProcessing)
	DECLARE_DF_FIELD(0, FString, message_box_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoCouponUse)
	DECLARE_DF_FIELD(0, FString, coupon_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoAccountConversionPrepare)
	DECLARE_DF_FIELD(0, FString, idp_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestKakaoAccountConversionComplete)
	DECLARE_DF_FIELD(0, FString, idp_code)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetDonationEvent)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeDonation)
	DECLARE_DF_FIELD(0, int32, donation_id)
	DECLARE_DF_FIELD(1, bool, is_bundle)
	DECLARE_DF_FIELD(2, bool, is_again)
	DECLARE_DF_FIELD(3, bool, is_free)
	DECLARE_DF_FIELD(4, FString, last_donation_key)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDonationPointReward)
	DECLARE_DF_FIELD(0, int32, donation_id)
	DECLARE_DF_FIELD(1, int32, step)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFairy)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLevelupFairy)
	DECLARE_DF_FIELD(0, int32, fairy_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBlessFairy)
	DECLARE_DF_FIELD(0, int32, fairy_type)
	DECLARE_DF_FIELD(1, int32, bless_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveFairyGift)
	DECLARE_DF_FIELD(0, int32, fairy_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestCheckChargePoints)
	DECLARE_DF_FIELD(0, bool, check_blade_point)
	DECLARE_DF_FIELD(1, bool, check_pvp_match_point)
	DECLARE_DF_FIELD(2, bool, check_team_match_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetFeaturedMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetEvent)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceivePointEventReward)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveLevelUpEventReward)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetSeasonMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardSeasonMission)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, day)
	DECLARE_DF_FIELD(2, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAttendSeasonMission)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveStageClearEventReward)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyHotTimeBuff)
	DECLARE_DF_FIELD(0, int32, buff_type)
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
	DECLARE_DF_FIELD(0, int32, package_id)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_DF_FIELD(2, int32, level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardReachLevelPackage)
	DECLARE_DF_FIELD(0, int32, package_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGiveUpQuest)
	DECLARE_DF_FIELD(0, int32, slot)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetContentsModeState)
	DECLARE_DF_FIELD(0, int32, mode)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardPointShopEvent)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_DF_FIELD(2, int32, buy_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReportUser)
	DECLARE_DF_FIELD(0, int64, target_account_id)
	DECLARE_DF_FIELD(1, int32, mode_type)
	DECLARE_DF_FIELD(2, int32, reason_type)
	DECLARE_DF_FIELD(3, FString, explain)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBlockChat)
	DECLARE_DF_FIELD(0, int64, block_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnblockChat)
	DECLARE_DF_FIELD(0, int64, unblock_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetBlockChatList)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestClientStateReport)
	DECLARE_DF_FIELD(0, FString, category)
	DECLARE_DF_FIELD(1, FString, event)
	DECLARE_DF_FIELD(2, FString, label)
	DECLARE_DF_FIELD(3, FString, detail_info)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardTenLotteryEvent)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestReceiveGemPurchaseEventReward)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardSpendGemEvent)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardTenLotterySavingsEvent)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRollDiceEvent)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, bool, is_free_dice)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestBuyDiceEventPoint)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRewardDiceEventMileage)
	DECLARE_DF_FIELD(0, int32, event_id)
	DECLARE_DF_FIELD(1, int32, reward_index)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	// REPLACEMENT POINT: MESSAGE_INFO HEADER REQUEST
	// NEVER DELETE LINE ABOVE

	// for console command

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDeleteAccount)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddItemToInventory)
	DECLARE_DF_FIELD(0, int32, item_template_id)
	DECLARE_DF_FIELD(1, int32, item_quality)
	DECLARE_DF_FIELD(2, int32, item_amount)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddCostumeToInventory)
	DECLARE_DF_FIELD(0, int32, template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddTotemToInventory)
	DECLARE_DF_FIELD(0, int32, template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseCashPaid)
	DECLARE_DF_FIELD(0, int32, cash_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseMoney)
	DECLARE_DF_FIELD(0, int32, money)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseSocialPoint)
	DECLARE_DF_FIELD(0, int32, social_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetItemLevelup)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, level)
	DECLARE_DF_FIELD(2, float, exp)
	DECLARE_DF_FIELD(3, int32, enhance_level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterLevelExp)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, level)
	DECLARE_DF_FIELD(2, float, exp)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterRankInfo)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, rank)
	DECLARE_DF_FIELD(2, int32, nodeIndex)
	DECLARE_DF_FIELD(3, bool, nodeState)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetCharacterRelic)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, relic_id)
	DECLARE_DF_FIELD(2, int32, grade)
	DECLARE_DF_FIELD(3, int32, enhance_level)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetBladePoint)
	DECLARE_DF_FIELD(0, int32, blade_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseRankPromotionPoint)
	DECLARE_DF_FIELD(0, int32, rank_promotion_point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddMail)
	DECLARE_DF_FIELD(0, int32, mail_category)
	DECLARE_DF_FIELD(1, int32, attach_type)
	DECLARE_DF_FIELD(2, int32, source_type)
	DECLARE_DF_FIELD(3, int32, sender_type)
	DECLARE_DF_FIELD(4, int32, amount)
	DECLARE_DF_FIELD(5, int32, item_template_id)
	DECLARE_DF_FIELD(6, int32, general_lottery_id)
	DECLARE_DF_FIELD(7, int32, fixed_grade_lottery_id)
	DECLARE_DF_FIELD(8, int32, selective_lottery_id)
	DECLARE_DF_FIELD(9, int32, selective_item_lottery_id)
	DECLARE_DF_FIELD(10, int32, prefix_selective_lottery_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeRanking)
	DECLARE_DF_FIELD(0, int32, match_type)
	DECLARE_DF_FIELD(1, int32, ranking)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceClearStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetQuestState)
	DECLARE_DF_FIELD(0, int32, quest_id)
	DECLARE_DF_FIELD(1, int32, quest_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestIncreaseHeroPiece)
	DECLARE_DF_FIELD(0, int32, hero_piece)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceClearHeroTower)
	DECLARE_DF_FIELD(0, int32, floor)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceStartCounterDungeon)
	DECLARE_DF_FIELD(0, int32, dungeon_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareGuild)
	DECLARE_DF_FIELD(0, int32, member_count)
	DECLARE_DF_FIELD(1, int32, add_guild_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareSweepHeroTower)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildBattleState)
	DECLARE_DF_FIELD(0, bool, is_new_season)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetAttendanceTime)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetEventAttendance)
	DECLARE_DF_FIELD(0, bool, is_only_attendance_time)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetDailyPlayTimeRecord)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestResetTeamMatchPoint)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestPrepareFriend)
	DECLARE_DF_FIELD(0, int32, friend_count)
	DECLARE_DF_FIELD(1, int32, job_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetGuildLevelExp)
	DECLARE_DF_FIELD(0, int32, level)
	DECLARE_DF_FIELD(1, int32, exp)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAddAetherToInventory)
	DECLARE_DF_FIELD(0, int32, aether_type)
	DECLARE_DF_FIELD(1, int32, slot)
	DECLARE_DF_FIELD(2, int32, grade)
	DECLARE_DF_FIELD(3, int32, tier_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestForceUpdateAether)
	DECLARE_DF_FIELD(0, int64, aether_id)
	DECLARE_DF_FIELD(1, int32, type)
	DECLARE_DF_FIELD(2, int32, level)
	DECLARE_DF_FIELD(3, int32, grade)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetMatchSeasonReward)
	DECLARE_DF_FIELD(0, int32, match_type)
	DECLARE_DF_FIELD(1, int32, ranking)
	DECLARE_DF_FIELD(2, int32, point)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetStageBoostCount)
	DECLARE_DF_FIELD(0, int32, stage_boost_ticket)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSkipMatchTutorial)
	DECLARE_DF_FIELD(0, int32, match_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetDebugMode)
	DECLARE_DF_FIELD(0, bool, skip_keep_alive_check)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestServerCmd)
	DECLARE_DF_FIELD(0, FString, command)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestTrace)
	DECLARE_DF_FIELD(0, FString, tag)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class B2NETWORK_API FB2MessageInfoResponse : public FB2MessageInfo
	{
	public:
		FB2MessageInfoResponse(std::string className) : FB2MessageInfo(className) {}

		virtual void DoCallback(FB2MessageInfoPtr InParam) override {};
		virtual FB2MessageCallback GetSuccessCallback() override {
			return nullptr;
		}
		virtual FB2MessageCallback GetFailCallback() override {
			return nullptr;
		}
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
	DECLARE_DF_FIELD(0, B2AccountInfoPtr, account)
	DECLARE_DF_FIELD(1, TArray<B2CharacterServerInfoPtr>, characters)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, equips)
	DECLARE_DF_FIELD(3, TArray<B2CostumeServerInfoPtr>, equip_costumes)
	DECLARE_DF_FIELD(4, TArray<B2TotemServerInfoPtr>, equip_totems)
	DECLARE_DF_FIELD(5, TArray<B2ItemPresetPtr>, item_presets)
	DECLARE_DF_FIELD(6, FString, guild_name)
	DECLARE_DF_FIELD(7, int32, mark_index)
	DECLARE_DF_FIELD(8, int32, mark_color)
	DECLARE_DF_FIELD(9, int32, mark_bg_index)
	DECLARE_DF_FIELD(10, int32, mark_bg_color)
	DECLARE_DF_FIELD(11, int32, my_rank)
	DECLARE_DF_FIELD(12, TArray<B2RelicPtr>, relics)
	DECLARE_DF_FIELD(13, TArray<B2WingOptionInfoPtr>, wing_options)
	DECLARE_DF_FIELD(14, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_DF_FIELD(15, TArray<B2EventAttendanceStatusPtr>, event_attendance_statuses)
	DECLARE_DF_FIELD(16, int32, aether_inventory_slot_count)
	DECLARE_DF_FIELD(17, TArray<B2AetherServerInfoPtr>, mount_aethers)
	DECLARE_DF_FIELD(18, TArray<B2FairyStatusPtr>, fairies)
	DECLARE_DF_FIELD(19, bool, is_all_tutorial_finished)
	DECLARE_DF_FIELD(20, TArray<B2UnitySkillMissionPtr>, unitySkills)
	DECLARE_DF_FIELD(21, TArray<B2RollingBannerPtr>, rolling_banners)
	DECLARE_DF_FIELD(22, B2BeginnerAttendanceStatusPtr, beginner_attendance_status)
	DECLARE_DF_FIELD(23, TArray<B2UnitySkillAwakenMissionPtr>, unity_awaken_missions)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(24)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindAccount)
	DECLARE_DF_FIELD(0, TArray<B2FindAccountInfoPtr>, account_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountItem)
	DECLARE_DF_FIELD(0, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountCostume)
	DECLARE_DF_FIELD(0, TArray<B2CostumeServerInfoPtr>, costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMasterData)
	DECLARE_DF_FIELD(0, B2ConfigDataPtr, config_data)
	DECLARE_DF_FIELD(1, TArray<B2mdSkillInfoPtr>, md_skill_infos)
	DECLARE_DF_FIELD(2, TArray<B2mdSkillOptionPtr>, md_skill_options)
	DECLARE_DF_FIELD(3, TArray<B2mdItemLevelupExpFactorPtr>, md_item_levelup_exp_factors)
	DECLARE_DF_FIELD(4, TArray<B2mdItemOptionAddValuePtr>, md_item_option_add_values)
	DECLARE_DF_FIELD(5, TArray<B2mdItemLevelupCostPtr>, md_item_levelup_costs)
	DECLARE_DF_FIELD(6, TArray<B2mdStageInfoPtr>, md_stage_infos)
	DECLARE_DF_FIELD(7, TArray<B2mdResurrectionInfoPtr>, md_resurrection_infos)
	DECLARE_DF_FIELD(8, TArray<B2mdTeamBattleFormationInfoPtr>, md_formation_infos)
	DECLARE_DF_FIELD(9, TArray<B2mdRankInfoPtr>, md_rank_infos)
	DECLARE_DF_FIELD(10, TArray<B2mdDailyMatchRewardPtr>, md_daily_match_rewards)
	DECLARE_DF_FIELD(11, TArray<B2mdMatchRankingRewardPtr>, md_pvp_ranking_rewards)
	DECLARE_DF_FIELD(12, TArray<B2mdMatchRankingRewardPtr>, md_team_ranking_rewards)
	DECLARE_DF_FIELD(13, TArray<B2mdCounterDungeonPtr>, md_counter_dungeons)
	DECLARE_DF_FIELD(14, TArray<B2mdCounterDungeonPeriodPtr>, md_counter_dungeon_periods)
	DECLARE_DF_FIELD(15, TArray<B2mdCounterDungeonWeekRewardPtr>, md_counter_dungeon_week_rewards)
	DECLARE_DF_FIELD(16, TArray<B2mdMainQuestInfoPtr>, md_main_quest_infos)
	DECLARE_DF_FIELD(17, TArray<B2mdDailyQuestInfoPtr>, md_daily_quest_infos)
	DECLARE_DF_FIELD(18, TArray<B2mdRewardInfoPtr>, md_reward_infos)
	DECLARE_DF_FIELD(19, TArray<B2mdMissionInfoPtr>, md_mission_infos)
	DECLARE_DF_FIELD(20, TArray<B2mdSerialMissionInfoPtr>, md_serial_mission_infos)
	DECLARE_DF_FIELD(21, TArray<B2mdMissionCompletePointInfoPtr>, md_mission_complete_point_infos)
	DECLARE_DF_FIELD(22, TArray<B2mdWingEnhancePtr>, md_wing_enhance_infos)
	DECLARE_DF_FIELD(23, TArray<B2mdWingUpgradePtr>, md_wing_upgrade_infos)
	DECLARE_DF_FIELD(24, TArray<B2mdWingOptionValuePtr>, md_wing_option_value_infos)
	DECLARE_DF_FIELD(25, TArray<B2mdWingOptionOpenPtr>, md_wing_option_open_infos)
	DECLARE_DF_FIELD(26, TArray<B2mdWingOptionMaxLevelPtr>, md_wing_option_max_level_infos)
	DECLARE_DF_FIELD(27, TArray<B2mdHeroTowerFloorPtr>, md_hero_tower_floors)
	DECLARE_DF_FIELD(28, TArray<B2mdRelicInfoPtr>, md_relic_infos)
	DECLARE_DF_FIELD(29, TArray<B2mdRelicGradeInfoPtr>, md_relic_grade_infos)
	DECLARE_DF_FIELD(30, TArray<B2mdRelicLevelInfoPtr>, md_relic_level_infos)
	DECLARE_DF_FIELD(31, TArray<B2mdShopLotteryPricePtr>, md_shop_lottery_prices)
	DECLARE_DF_FIELD(32, TArray<B2mdShopLotteryPossibleGradePtr>, md_shop_lottery_possible_grades)
	DECLARE_DF_FIELD(33, TArray<B2mdShopLotterySpecialGradePtr>, md_shop_lottery_special_grades)
	DECLARE_DF_FIELD(34, TArray<B2mdShopProductPtr>, md_shop_products)
	DECLARE_DF_FIELD(35, TArray<B2mdShopPackagePtr>, md_shop_packages)
	DECLARE_DF_FIELD(36, TArray<B2mdShopMileageRewardPtr>, md_shop_mileage_rewards)
	DECLARE_DF_FIELD(37, TArray<B2mdShopLotteryDailyLimitPtr>, md_shop_lottery_daily_limits)
	DECLARE_DF_FIELD(38, TArray<B2mdCollectionRewardPtr>, md_collection_rewards)
	DECLARE_DF_FIELD(39, TArray<B2mdDailyAttendanceInfoPtr>, md_daily_atttendances)
	DECLARE_DF_FIELD(40, TArray<B2mdGuildBattleResultRewardPtr>, md_guild_battle_result_rewards)
	DECLARE_DF_FIELD(41, TArray<B2mdGuildBattleSeasonRewardPtr>, md_guild_battle_season_rewards)
	DECLARE_DF_FIELD(42, TArray<B2mdRaidInfoPtr>, md_raid_infos)
	DECLARE_DF_FIELD(43, TArray<B2mdMonsterInfoPtr>, md_monster_infos)
	DECLARE_DF_FIELD(44, TArray<B2mdGeneralLotteryPtr>, md_general_lotteries)
	DECLARE_DF_FIELD(45, TArray<B2mdSelectiveLotteryPtr>, md_selective_lotteries)
	DECLARE_DF_FIELD(46, TArray<B2mdFixedGradeLotteryPtr>, md_fixed_grade_lotteries)
	DECLARE_DF_FIELD(47, TArray<B2mdItemEnhanceCostPtr>, md_item_enhance_costs)
	DECLARE_DF_FIELD(48, TArray<B2mdItemEnhanceEffectInfoPtr>, md_item_enhance_effect_infos)
	DECLARE_DF_FIELD(49, TArray<B2mdAdditionalMatchCostPtr>, md_additional_match_cost_infos)
	DECLARE_DF_FIELD(50, TArray<B2mdGuildSkillPtr>, md_guild_skills)
	DECLARE_DF_FIELD(51, TArray<B2mdGuildExpPtr>, md_guild_exps)
	DECLARE_DF_FIELD(52, TArray<B2mdStageFixedDropPtr>, md_stage_fixed_drops)
	DECLARE_DF_FIELD(53, TArray<B2mdAssaultVictoryRewardInfoPtr>, md_assault_victory_reward_infos)
	DECLARE_DF_FIELD(54, TArray<B2mdAssaultMvpRewardInfoPtr>, md_assault_mvp_reward_infos)
	DECLARE_DF_FIELD(55, TArray<B2mdEventAttendanceRewardPtr>, md_event_attendance_rewards)
	DECLARE_DF_FIELD(56, TArray<B2mdDailyPlayTimeRewardPtr>, md_daily_play_time_rewards)
	DECLARE_DF_FIELD(57, TArray<B2mdMatchRankingRewardPtr>, md_assault_ranking_rewards)
	DECLARE_DF_FIELD(58, TArray<B2mdTutorialInfoPtr>, md_tutorial_infos)
	DECLARE_DF_FIELD(59, TArray<B2mdDuelModeRewardPtr>, md_duel_mode_reward)
	DECLARE_DF_FIELD(60, TArray<B2mdRaidExpPtr>, md_raid_exp)
	DECLARE_DF_FIELD(61, TArray<B2mdAetherOptionInfoPtr>, md_aether_option_infos)
	DECLARE_DF_FIELD(62, TArray<B2mdAetherSlotInfoPtr>, md_aether_slot_infos)
	DECLARE_DF_FIELD(63, TArray<B2mdAetherEnhanceInfoPtr>, md_aether_enhance_infos)
	DECLARE_DF_FIELD(64, TArray<B2mdAetherEnhanceOptionInfoPtr>, md_aether_enhance_option_infos)
	DECLARE_DF_FIELD(65, TArray<B2mdAetherSetOptionInfoPtr>, md_aether_set_option_infos)
	DECLARE_DF_FIELD(66, TArray<B2mdGuildSupportItemPtr>, md_guild_support_items)
	DECLARE_DF_FIELD(67, TArray<B2mdGuildBattleScheduleInfoPtr>, md_guild_battle_schedule_infos)
	DECLARE_DF_FIELD(68, TArray<B2mdUnitySkillMissionPtr>, md_unity_skill_missions)
	DECLARE_DF_FIELD(69, TArray<B2mdRestRewardPtr>, md_rest_rewards)
	DECLARE_DF_FIELD(70, TArray<B2mdGuildMercenaryPtr>, md_guild_mercenaries)
	DECLARE_DF_FIELD(71, TArray<B2mdGuildMercenarySkillPtr>, md_guild_mercenary_skills)
	DECLARE_DF_FIELD(72, TArray<B2mdGuildMercenaryMasteryPtr>, md_guild_mercenary_masteries)
	DECLARE_DF_FIELD(73, TArray<B2mdGuildMercenaryDonationPtr>, md_guild_mercenary_donations)
	DECLARE_DF_FIELD(74, TArray<B2mdItemInfoPtr>, md_item_infos)
	DECLARE_DF_FIELD(75, TArray<B2mdItemSetPtr>, md_item_set_infos)
	DECLARE_DF_FIELD(76, TArray<B2mdPointEventPtr>, md_point_events)
	DECLARE_DF_FIELD(77, TArray<B2mdPointEventRewardPtr>, md_point_event_rewards)
	DECLARE_DF_FIELD(78, TArray<B2mdLevelUpEventPtr>, md_level_events)
	DECLARE_DF_FIELD(79, TArray<B2mdSeasonMissionEventPtr>, md_season_mission_events)
	DECLARE_DF_FIELD(80, TArray<B2mdKakaoFriendInviteRewardPtr>, md_kakao_friend_invite_rewards)
	DECLARE_DF_FIELD(81, TArray<B2mdHotTimeProductPtr>, md_hot_time_products)
	DECLARE_DF_FIELD(82, TArray<B2mdShopCashBonusPtr>, md_shop_cash_bonus_infos)
	DECLARE_DF_FIELD(83, TArray<B2mdShopFundsSalePtr>, md_shop_funds_sale_infos)
	DECLARE_DF_FIELD(84, TArray<B2mdShopLotterySalePtr>, md_shop_lottery_sale_infos)
	DECLARE_DF_FIELD(85, TArray<B2mdShopPackageProductPtr>, md_shop_package_products)
	DECLARE_DF_FIELD(86, TArray<B2mdSkillLevelUpPtr>, md_skill_levelups)
	DECLARE_DF_FIELD(87, TArray<B2mdDuelModeSettleSchedulePtr>, md_duel_mode_settle_schedules)
	DECLARE_DF_FIELD(88, TArray<B2mdShopLevelUpPackagePtr>, md_shop_level_up_packages)
	DECLARE_DF_FIELD(89, TArray<B2mdShopReachLevelPackagePtr>, md_shop_reach_level_packages)
	DECLARE_DF_FIELD(90, TArray<B2mdShopFlatRatePackageBonusPtr>, md_shop_flat_rate_package_bonus)
	DECLARE_DF_FIELD(91, TArray<B2mdShopPackageTabPtr>, md_shop_package_tab_infos)
	DECLARE_DF_FIELD(92, TArray<B2mdUnitySkillOptionPtr>, md_unity_skill_options)
	DECLARE_DF_FIELD(93, TArray<B2mdStageClearEventPtr>, md_stage_clear_events)
	DECLARE_DF_FIELD(94, TArray<B2mdWebUrlPtr>, md_web_urls)
	DECLARE_DF_FIELD(95, TArray<B2mdTenLotteryEventPtr>, md_ten_lottery_events)
	DECLARE_DF_FIELD(96, TArray<B2mdMagicShopProductPtr>, md_magic_shop_product)
	DECLARE_DF_FIELD(97, TArray<B2mdMagicShopRenewalCostPtr>, md_magic_shop_renewal_cost)
	DECLARE_DF_FIELD(98, TArray<B2mdGemPurchaseEventRewardPtr>, md_gem_purchase_event_rewards)
	DECLARE_DF_FIELD(99, TArray<B2mdSpendGemEventPtr>, md_spend_gem_event_rewards)
	DECLARE_DF_FIELD(100, TArray<B2mdEventAttendancePtr>, md_event_attendances)
	DECLARE_DF_FIELD(101, TArray<B2mdEventInfoPtr>, md_event_infos)
	DECLARE_DF_FIELD(102, TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
	DECLARE_DF_FIELD(103, TArray<B2mdTenLotterySavingsEventPtr>, md_ten_lottery_savings_events)
	DECLARE_DF_FIELD(104, TArray<B2mdItemLevelupFactorPtr>, md_item_levelup_factors)
	DECLARE_DF_FIELD(105, TArray<B2mdPrefixSelectiveLotteryPtr>, md_prefix_selective_lotteries)
	DECLARE_DF_FIELD(106, TArray<B2mdSelectItemOptionPtr>, md_select_item_option)
	DECLARE_DF_FIELD(107, TArray<B2mdDiceEventMissionPtr>, md_dice_event_missions)
	DECLARE_DF_FIELD(108, TArray<B2mdDiceEventCellPtr>, md_dice_event_cells)
	DECLARE_DF_FIELD(109, TArray<B2mdDiceEventFinishRewardPtr>, md_dice_event_finish_rewards)
	DECLARE_DF_FIELD(110, TArray<B2mdItemSurpassLevelPtr>, md_item_surpass_levels)
	DECLARE_DF_FIELD(111, TArray<B2mdDiceEventBuyPointPtr>, md_dice_event_buy_points)
	DECLARE_DF_FIELD(112, TArray<B2mdDiceEventMileagePtr>, md_dice_event_mileages)
	DECLARE_DF_FIELD(113, TArray<B2mdItemDismantleInfoPtr>, md_item_dismantle_infos)
	DECLARE_DF_FIELD(114, TArray<B2mdFairyInfoPtr>, md_fairy_infos)
	DECLARE_DF_FIELD(115, TArray<B2mdFairyLevelupCostPtr>, md_fairy_levelup_costs)
	DECLARE_DF_FIELD(116, TArray<B2mdFairyOpenInfoPtr>, md_fairy_open_infos)
	DECLARE_DF_FIELD(117, TArray<B2mdCostumeInfoPtr>, md_costume_infos)
	DECLARE_DF_FIELD(118, TArray<B2mdDonationPointRankingRewardInfoPtr>, md_donation_point_ranking_reward_infos)
	DECLARE_DF_FIELD(119, TArray<B2mdDonationBundleFixedRewardInfoPtr>, md_donation_bundle_fixed_reward_infos)
	DECLARE_DF_FIELD(120, TArray<B2mdDonationPointStepRewardInfoPtr>, md_donation_point_step_reward_infos)
	DECLARE_DF_FIELD(121, TArray<B2mdUnitySkillAwakenMissionPtr>, md_unity_skill_awaken_missions)
	DECLARE_DF_FIELD(122, B2mdExtendMessagePtr, md_extend_massages)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(123)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetLatestEventData)
	DECLARE_DF_FIELD(0, int64, last_update_time)
	DECLARE_DF_FIELD(1, TArray<B2mdEventInfoPtr>, md_event_infos)
	DECLARE_DF_FIELD(2, TArray<B2mdEventAttendancePtr>, md_event_attendances)
	DECLARE_DF_FIELD(3, TArray<B2mdHotTimeEventPtr>, md_hot_time_events)
	DECLARE_DF_FIELD(4, TArray<B2mdDonationEventInfoPtr>, md_donation_event_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSignUp)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellItem)
	DECLARE_DF_FIELD(0, int32, money_total)
	DECLARE_DF_FIELD(1, TArray<int64>, sell_item_ids)
	DECLARE_DF_FIELD(2, int32, sell_price)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellConsumableItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, B2ItemServerInfoPtr, updated_item)
	DECLARE_DF_FIELD(2, int64, deleted_item_id)
	DECLARE_DF_FIELD(3, int32, sell_price)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConvertData)
	DECLARE_DF_FIELD(0, int32, templateId)
	DECLARE_DF_FIELD(1, int32, exchange_remain_count_gold)
	DECLARE_DF_FIELD(2, int32, exchange_remain_count_gem)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConvertItem)
	//DECLARE_DF_FIELD(0, b2::protocol::commondata::ExchangePriceType, exchangetype)
	DECLARE_DF_FIELD(0, int32, exchangetype)
	DECLARE_DF_FIELD(1, int32, currentGold)
	DECLARE_DF_FIELD(2, int32, currentCash)
	DECLARE_DF_FIELD(3, B2ItemServerInfoPtr, exchangeItem)
	DECLARE_DF_FIELD(4, int32, exchangeItemAmount)
	DECLARE_DF_FIELD(5, int32, costCash)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipItem)
	DECLARE_DF_FIELD(0, int32, preset_id)
	DECLARE_DF_FIELD(1, TArray<int64>, equiped_item_ids)
	DECLARE_DF_FIELD(2, TArray<int64>, unequiped_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipItem)
	DECLARE_DF_FIELD(0, int32, preset_id)
	DECLARE_DF_FIELD(1, TArray<int64>, unequiped_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipCostume)
	DECLARE_DF_FIELD(0, TArray<int64>, equiped_costume_ids)
	DECLARE_DF_FIELD(1, TArray<int64>, unequiped_costume_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipCostume)
	DECLARE_DF_FIELD(0, TArray<int64>, unequiped_costume_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceCostume)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, B2CostumeServerInfoPtr, enhanced_costume)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_material_items)
	DECLARE_DF_FIELD(3, TArray<int64>, deleted_material_item_ids)
	DECLARE_DF_FIELD(4, int32, current_enhance_level)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleCostumes)
	DECLARE_DF_FIELD(0, TArray<int64>, dismantled_costume_ids)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, extracted_items)
	DECLARE_DF_FIELD(2, TArray<int32>, extracted_item_change_amount)
	DECLARE_DF_FIELD(3, B2ItemServerInfoPtr, restore_item)
	DECLARE_DF_FIELD(4, int32, restore_items_change_amount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockCostumes)
	DECLARE_DF_FIELD(0, TArray<B2CostumeServerInfoPtr>, locked_costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockCostumes)
	DECLARE_DF_FIELD(0, TArray<B2CostumeServerInfoPtr>, unlocked_costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockItem)
	DECLARE_DF_FIELD(0, TArray<B2ItemServerInfoPtr>, locked_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockItem)
	DECLARE_DF_FIELD(0, TArray<B2ItemServerInfoPtr>, unlocked_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExtendInventory)
	DECLARE_DF_FIELD(0, B2CharacterServerInfoPtr, character)
	DECLARE_DF_FIELD(1, int32, cash)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelupItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, int32, money_cost)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, main_item)
	DECLARE_DF_FIELD(3, TArray<int64>, item_ids)
	DECLARE_DF_FIELD(4, bool, jackpoted)
	DECLARE_DF_FIELD(5, bool, great_jackpoted)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, B2ItemServerInfoPtr, enhanced_item)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_enhance_scroll_items)
	DECLARE_DF_FIELD(3, TArray<int64>, deleted_enhance_scroll_item_ids)
	DECLARE_DF_FIELD(4, TArray<B2ItemServerInfoPtr>, changed_protection_scroll_items)
	DECLARE_DF_FIELD(5, TArray<int64>, deleted_protection_scroll_item_ids)
	DECLARE_DF_FIELD(6, int32, changed_enhance_level)
	DECLARE_DF_FIELD(7, bool, is_success)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleItems)
	DECLARE_DF_FIELD(0, TArray<int64>, dismantled_item_ids)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, extracted_items)
	DECLARE_DF_FIELD(2, TArray<int32>, extracted_item_change_amount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExchangeItemEnhanceLevel)
	DECLARE_DF_FIELD(0, int32, current_dark_crystal)
	DECLARE_DF_FIELD(1, int32, current_gem)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, item_data_1)
	DECLARE_DF_FIELD(3, B2ItemServerInfoPtr, item_data_2)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenSealSlotItem)
	DECLARE_DF_FIELD(0, int32, current_gold)
	DECLARE_DF_FIELD(1, int32, current_gem)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, slot_opened_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCarveSealItem)
	DECLARE_DF_FIELD(0, int32, current_gold)
	DECLARE_DF_FIELD(1, int32, current_gem)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, sealed_item)
	DECLARE_DF_FIELD(3, TArray<B2ItemServerInfoPtr>, updated_material_items)
	DECLARE_DF_FIELD(4, TArray<int64>, deleted_material_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSurpassItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, int32, money_cost)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, main_item)
	DECLARE_DF_FIELD(3, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpgradeItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, int32, money_cost)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, main_item)
	DECLARE_DF_FIELD(3, TArray<B2ItemServerInfoPtr>, upgrade_stone_items)
	DECLARE_DF_FIELD(4, int32, use_upgrade_stone_count)
	DECLARE_DF_FIELD(5, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseComposeItem)
	DECLARE_DF_FIELD(0, int32, current_money)
	DECLARE_DF_FIELD(1, int32, money_cost)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, new_composed_item)
	DECLARE_DF_FIELD(3, int64, deleted_main_item_id)
	DECLARE_DF_FIELD(4, int64, deleted_sub_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseChangeItemCharacterType)
	DECLARE_DF_FIELD(0, B2ItemServerInfoPtr, updated_item)
	DECLARE_DF_FIELD(1, int32, current_dark_crystal)
	DECLARE_DF_FIELD(2, int32, dark_crystal_cost)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetChangeItemCharacterTypeCost)
	DECLARE_DF_FIELD(0, int32, dark_crystal_cost)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, change_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartStage)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_DF_FIELD(1, int32, drop_gold)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, drop_items)
	DECLARE_DF_FIELD(3, int32, drop_gold_by_gold_monster)
	DECLARE_DF_FIELD(4, int32, max_resurrection_count)
	DECLARE_DF_FIELD(5, int32, need_cash_for_resurrection)
	DECLARE_DF_FIELD(6, B2AccountInfoPtr, account);
	DECLARE_DF_FIELD(7, int32, selected_option_flags);
	DECLARE_DF_FIELD(8, TArray<B2DropDataPtr>, drop_datas);
	DECLARE_DF_FIELD(9, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
	DECLARE_DF_FIELD(10, int32, quest_item_id);
	DECLARE_DF_FIELD(11, int32, quest_item_count);
	DECLARE_DF_FIELD(12, int32, current_aether_count);
	DECLARE_DF_FIELD(13, int32, current_aether_max_count);
	DECLARE_DF_FIELD(14, int32, qte_bonus_attack);
	DECLARE_DF_FIELD(15, int32, qte_bonus_finish);
	DECLARE_DF_FIELD(16, int32, stage_boost_ticket);
	DECLARE_DF_FIELD(17, TArray<B2GuildSkillPtr>, guild_skills);
	DECLARE_DF_FIELD(18, int32, gold_buff_rate)
	DECLARE_DF_FIELD(19, int32, exp_buff_rate)
	DECLARE_DF_FIELD(20, int32, equip_item_gold_rate)
	DECLARE_DF_FIELD(21, int32, equip_item_exp_rate)
	DECLARE_DF_FIELD(22, int32, item_set_gold_rate)
	DECLARE_DF_FIELD(23, int32, item_set_exp_rate)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(24)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseClearStage)
	DECLARE_DF_FIELD(0, B2AccountInfoPtr, account)
	DECLARE_DF_FIELD(1, TArray<B2CharacterServerInfoPtr>, characters)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(3, int32, clear_grade)
	DECLARE_DF_FIELD(4, int32, total_added_drop_gold)
	DECLARE_DF_FIELD(5, int32, total_added_reward_gold)
	DECLARE_DF_FIELD(6, int32, total_added_qte_gold)
	DECLARE_DF_FIELD(7, int32, reward_exp)
	DECLARE_DF_FIELD(8, int32, quest_item_id);
	DECLARE_DF_FIELD(9, int32, quest_item_count);
	DECLARE_DF_FIELD(10, bool, is_level_up_main_character);
	DECLARE_DF_FIELD(11, bool, is_level_up_tag_character);
	DECLARE_DF_FIELD(12, int32, clear_mask);
	DECLARE_DF_FIELD(13, TArray<B2AetherServerInfoPtr>, aethers)
	DECLARE_DF_FIELD(14, TArray<int32>, reward_golds);
	DECLARE_DF_FIELD(15, TArray<B2DropDataPtr>, fixed_items);
	DECLARE_DF_FIELD(16, TArray<B2ItemServerInfoPtr>, update_items)
	DECLARE_DF_FIELD(17, TArray<B2AetherServerInfoPtr>, update_aethers)
	DECLARE_DF_FIELD(18, int32, multiple_value);
	DECLARE_DF_FIELD(19, TArray<B2RelicPtr>, created_relics)
	DECLARE_DF_FIELD(20, int32, total_added_gold)
	DECLARE_DF_FIELD(21, int32, gold_buff_rate)
	DECLARE_DF_FIELD(22, int32, exp_buff_rate)
	DECLARE_DF_FIELD(23, int32, equip_item_gold_rate)
	DECLARE_DF_FIELD(24, int32, equip_item_exp_rate)
	DECLARE_DF_FIELD(25, int32, item_set_gold_rate)
	DECLARE_DF_FIELD(26, int32, item_set_exp_rate)
	DECLARE_DF_FIELD(27, TArray<B2DropDataPtr>, material_items);
	DECLARE_DF_FIELD(28, TArray<B2SoldItemInfoPtr>, sold_items);
	DECLARE_DF_FIELD(29, TArray<B2SoldAetherInfoPtr>, sold_aethers);
	DECLARE_DF_FIELD(30, int32, total_added_sales_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(31)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepStage)
	DECLARE_DF_FIELD(0, int32, stage_id)
	DECLARE_DF_FIELD(1, int32, repeat_count)
	DECLARE_DF_FIELD(2, int32, stage_sweep_ticket)
	DECLARE_DF_FIELD(3, B2AccountInfoPtr, account)
	DECLARE_DF_FIELD(4, TArray<B2CharacterServerInfoPtr>, characters)
	DECLARE_DF_FIELD(5, bool, is_level_up_main_character)
	DECLARE_DF_FIELD(6, bool, is_level_up_tag_character)
	DECLARE_DF_FIELD(7, int32, reward_exp)
	DECLARE_DF_FIELD(8, int32, total_added_drop_gold)
	DECLARE_DF_FIELD(9, int32, total_added_reward_gold)
	DECLARE_DF_FIELD(10, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(11, TArray<B2AetherServerInfoPtr>, aethers)
	DECLARE_DF_FIELD(12, TArray<int32>, reward_golds)
	DECLARE_DF_FIELD(13, TArray<B2DropDataPtr>, material_items)
	DECLARE_DF_FIELD(14, int32, quest_item_id)
	DECLARE_DF_FIELD(15, int32, quest_item_count)
	DECLARE_DF_FIELD(16, TArray<B2DropDataPtr>, fixed_items)
	DECLARE_DF_FIELD(17, TArray<B2RelicPtr>, created_relics)
	DECLARE_DF_FIELD(18, TArray<B2ItemServerInfoPtr>, update_items)
	DECLARE_DF_FIELD(19, int32, total_added_gold)
	DECLARE_DF_FIELD(20, int32, gold_buff_rate)
	DECLARE_DF_FIELD(21, int32, exp_buff_rate)
	DECLARE_DF_FIELD(22, int32, equip_item_gold_rate)
	DECLARE_DF_FIELD(23, int32, equip_item_exp_rate)
	DECLARE_DF_FIELD(24, int32, item_set_gold_rate)
	DECLARE_DF_FIELD(25, int32, item_set_exp_rate)
	DECLARE_DF_FIELD(26, TArray<B2InventoryCountServerInfoPtr>, not_enough_inventories)
	DECLARE_DF_FIELD(27, int32, current_aether_count)
	DECLARE_DF_FIELD(28, int32, current_aether_max_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(29)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFailStage)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResurrection)
	DECLARE_DF_FIELD(0, int32, max_resurrection_count)
	DECLARE_DF_FIELD(1, int32, resurrection_count)
	DECLARE_DF_FIELD(2, int32, used_cash)
	DECLARE_DF_FIELD(3, int32, total_cash)
	DECLARE_DF_FIELD(4, int32, next_resurrection_gem_cost)
	DECLARE_DF_FIELD(5, int32, selected_count_resurrection_buff1)
	DECLARE_DF_FIELD(6, int32, selected_count_resurrection_buff2)
	DECLARE_DF_FIELD(7, int32, selected_count_resurrection_buff3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckBladePointCharge)
	DECLARE_DF_FIELD(0, int32, blade_point)
	DECLARE_DF_FIELD(1, int32, blade_point_max)
	DECLARE_DF_FIELD(2, int32, blade_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemSurpassCost)
	DECLARE_DF_FIELD(0, int32, need_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemUpgradeCost)
	DECLARE_DF_FIELD(0, int32, need_gold)
	DECLARE_DF_FIELD(1, int32, need_upgrade_stone_template_id)
	DECLARE_DF_FIELD(2, int32, need_upgrade_stone_count)
	DECLARE_DF_FIELD(3, B2ItemServerInfoPtr, next_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemComposeCost)
	DECLARE_DF_FIELD(0, int32, need_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetActInfo)
	DECLARE_DF_FIELD(0, TArray<B2StageClearInfoPtr>, stage_clear_infos)
	DECLARE_DF_FIELD(1, TArray<B2ActRewardInfoPtr>, reward_infos)
	DECLARE_DF_FIELD(2, TArray<B2ActIdentifierPtr>, has_reward_acts)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetStageInfo)
	DECLARE_DF_FIELD(0, TArray<B2StageClearRewardItemInfoPtr>, reward_item_infos)
	DECLARE_DF_FIELD(1, int32, auto_repeat_battle_cost)
	DECLARE_DF_FIELD(2, int32, need_blade_point)
	DECLARE_DF_FIELD(3, bool, enable_auto_repeat_battle)
	DECLARE_DF_FIELD(4, int32, left_buff_ticket_exp)
	DECLARE_DF_FIELD(5, int32, left_buff_ticket_gold)
	DECLARE_DF_FIELD(6, int32, left_buff_ticket_autoskill)
	DECLARE_DF_FIELD(7, int32, left_buff_ticket_boost)
	DECLARE_DF_FIELD(8, int32, stage_boost_ticket)
	DECLARE_DF_FIELD(9, int32, best_clear_mask)
	DECLARE_DF_FIELD(10, TArray<B2StageClearRewardAetherInfoPtr>, reward_aether_infos)
	DECLARE_DF_FIELD(11, TArray<B2DropDataPtr>, material_items);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoContentsModeStateNotify)
	DECLARE_DF_FIELD(0, int32, mode)				// B2ContentsMode
		DECLARE_DF_FIELD(1, int32, prev_state)			// B2ContentsModeState
		DECLARE_DF_FIELD(2, int32, current_state)		// B2ContentsModeState
		DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetMatchStatus)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoResponseGetMatchStatus)
	DECLARE_DF_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(1, int32, from_ranking)
	DECLARE_DF_FIELD(2, int32, total_ranker_count)
	DECLARE_DF_FIELD(3, B2MatchStatusPtr, match_status)
	DECLARE_DF_FIELD(4, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(5, int32, main_character_type)
	DECLARE_DF_FIELD(6, int32, tag_character_type)
	DECLARE_DF_FIELD(7, int32, total_power)
	DECLARE_DF_FIELD(8, B2RankingRewardPtr, ranking_reward)
	DECLARE_DF_FIELD(9, TArray<B2HallOfFamePtr>, hall_of_fames)
	DECLARE_DF_FIELD(10, int64, settlement_time)
	DECLARE_DF_FIELD(11, int32, settlement_period_hour)
	DECLARE_DF_FIELD(12, bool, today_reward)
	DECLARE_DF_FIELD(13, bool, last_reward)
	DECLARE_DF_FIELD(14, int32, today_participation_count)
	DECLARE_DF_FIELD(15, int32, current_gold)
	DECLARE_DF_FIELD(16, int32, current_bladepoint)
	DECLARE_DF_FIELD(17, int32, current_gem)
	DECLARE_DF_FIELD(18, int32, match_point)
	DECLARE_DF_FIELD(19, int32, additional_match_count)
	DECLARE_DF_FIELD(20, int32, match_point_next_charge_second)
	DECLARE_DF_FIELD(21, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(22)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMatchmakingRequest)
	DECLARE_DF_FIELD(0, int32, match_type)
	DECLARE_DF_FIELD(1, int32, main_character_type)
	DECLARE_DF_FIELD(2, int32, tag_character_type)
	DECLARE_DF_FIELD(3, bool, use_additional_count)
	DECLARE_DF_FIELD(4, FString, room_seed)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoMatchmakingResponse)
	DECLARE_DF_FIELD(0, B2MatchInfoPtr, match_info)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2RoomLocationPtr, room_location)
	DECLARE_DF_FIELD(3, int32, current_gem)
	DECLARE_DF_FIELD(4, int32, addadditional_match_count)
	DECLARE_DF_FIELD(5, int32, match_point)
	DECLARE_DF_FIELD(6, B2MatcherPtr, ai_opponent)
	DECLARE_DF_FIELD(7, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRaidMatchmakingRequest)
	DECLARE_DF_FIELD(0, int32, raid_req_type)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_DF_FIELD(2, int32, raid_type)
	DECLARE_DF_FIELD(3, int32, raid_step)
	DECLARE_DF_FIELD(4, B2EndpointPtr, channel_endpoint)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRaidMatchmakingResponse)
	DECLARE_DF_FIELD(0, int32, req_type)
	DECLARE_DF_FIELD(1, FString, raid_token)
	DECLARE_DF_FIELD(2, int32, room_type)
	DECLARE_DF_FIELD(3, int32, room_access)
	DECLARE_DF_FIELD(4, B2EndpointPtr, channel_endpoint)
	DECLARE_DF_FIELD(5, B2RoomLocationPtr, room_location)
	DECLARE_DF_FIELD(6, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_DF_FIELD(7, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
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
	DECLARE_DF_FIELD(0, FString, inviter)
	DECLARE_DF_FIELD(1, FString, invitation_key)
	DECLARE_DF_FIELD(2, B2RoomLocationPtr, room_location)
	DECLARE_DF_FIELD(3, int32, map_id)
	DECLARE_DF_FIELD(4, int32, difficulty)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoMatchResultRequest)
	DECLARE_DF_FIELD(0, B2MatchResultPtr, result)
	DECLARE_DF_FIELD(1, int32, match_type)
	DECLARE_DF_FIELD(2, B2RoomCharacterPtr, my_main_character)
	DECLARE_DF_FIELD(3, B2RoomCharacterPtr, my_tag_character)
	DECLARE_DF_FIELD(4, B2RoomCharacterPtr, opponent_main_character)
	DECLARE_DF_FIELD(5, B2RoomCharacterPtr, opponent_tag_character)
	DECLARE_DF_FIELD(6, FString, room_id)
	DECLARE_DF_FIELD(7, bool, is_give_up)
	DECLARE_B2MESSAGEINFO_REQUEST_END(8)


	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoMatchResultResponse)
	DECLARE_DF_FIELD(0, B2MatchStatusPtr, status)
	DECLARE_DF_FIELD(1, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(2, B2MatchRankerPtr, prev_ranker)
	DECLARE_DF_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_DF_FIELD(4, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoInvitationRequest)
	DECLARE_DF_FIELD(0, FString, invitee)
	DECLARE_DF_FIELD(1, FString, room_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoInvitationResponse)
	DECLARE_DF_FIELD(0, FString, invitee)
	DECLARE_DF_FIELD(1, FString, invitation_ref)
	DECLARE_DF_FIELD(2, B2RoomLocationPtr, room_location)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoInvitationNotify)
	DECLARE_DF_FIELD(0, int32, invitation_type)
	DECLARE_DF_FIELD(1, FString, inviter)
	DECLARE_DF_FIELD(2, FString, invitation_ref)
	DECLARE_DF_FIELD(3, B2RoomLocationPtr, room_location)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoAcceptInvitationRequest)
	DECLARE_DF_FIELD(0, int32, invitation_type)
	DECLARE_DF_FIELD(1, FString, inviter)
	DECLARE_DF_FIELD(2, FString, invitation_ref)
	DECLARE_DF_FIELD(3, FString, invitee)
	DECLARE_DF_FIELD(4, B2RoomLocationPtr, room_location)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoAcceptInvitationResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoAcceptInvitationNotify)
	DECLARE_DF_FIELD(0, int32, invitation_type)
	DECLARE_DF_FIELD(1, FString, invitee)
	DECLARE_DF_FIELD(2, FString, invitation_ref)
	DECLARE_DF_FIELD(3, B2RoomLocationPtr, room_location)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoDeclineInvitationRequest)
	DECLARE_DF_FIELD(0, int32, invitation_type)
	DECLARE_DF_FIELD(1, FString, inviter)
	DECLARE_DF_FIELD(2, FString, invitation_ref)
	DECLARE_DF_FIELD(3, FString, invitee)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoDeclineInvitationResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoDeclineInvitationNotify)
	DECLARE_DF_FIELD(0, int32, invitation_type)
	DECLARE_DF_FIELD(1, FString, invitee)
	DECLARE_DF_FIELD(2, FString, invitation_ref)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomJoinRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, int32, room_access)
	DECLARE_DF_FIELD(3, TArray<B2RoomPropertyPtr>, properties)
	DECLARE_DF_FIELD(4, TArray<B2RoomCharacterPtr>, room_characters)
	DECLARE_DF_FIELD(5, TArray<B2PresetNumPtr>, character_presets)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomJoinResponse)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2RoomParticipantPtr, host)
	DECLARE_DF_FIELD(3, TArray<B2RoomParticipantPtr>, participants)
	DECLARE_DF_FIELD(4, TArray<B2RoomPropertyPtr>, properties)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomLeaveRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomLeaveResponse)
	DECLARE_DF_FIELD(0, TArray<FString>, room_id_list)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomKickRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int64, kick_account_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomKickResponse)
	DECLARE_DF_FIELD(0, int64, kicked_account_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_COMMAND(FB2MessageInfoRoomMessageCommand)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, receiver_id)
	DECLARE_DF_FIELD(2, FString, message)
	DECLARE_B2MESSAGEINFO_COMMAND_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomParticipantJoinNotify)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2RoomParticipantPtr, joiner)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomParticipantLeaveNotify)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2RoomParticipantPtr, leaver)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomHostChangedNotify)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, int32, host_id)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomMessageNotify)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2RoomParticipantPtr, sender)
	DECLARE_DF_FIELD(3, FString, message)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomHeartbeatNotify)
	DECLARE_B2MESSAGEINFO_NOTIFY_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomSetPropertyRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, TArray<B2RoomPropertyPtr>, properties)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomSetPropertyResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoRoomPropertyUpdatedNotify)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, TArray<B2RoomPropertyPtr>, properties)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomInviteRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, FString, invitee)
	DECLARE_DF_FIELD(2, int32, invitation_number)
	DECLARE_DF_FIELD(3, int32, map_id)
	DECLARE_DF_FIELD(4, int32, difficulty)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomInviteResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomCloseRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, map_id)
	DECLARE_DF_FIELD(2, int32, reason)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomLeaveResultRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int64, leaved_account_id)
	DECLARE_DF_FIELD(2, FString, match_token)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomCloseResponse)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, FString, token)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomLeaveResultResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRoomAccessControlRequest)
	DECLARE_DF_FIELD(0, FString, room_id)
	DECLARE_DF_FIELD(1, int32, access)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoRoomAccessControlResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSetRoomDebugMode)
	DECLARE_DF_FIELD(0, bool, skip_room_heart_beat_check)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetRoomDebugMode)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoReportUserViaChannelRequest)
	DECLARE_DF_FIELD(0, int64, target_account_id)
	DECLARE_DF_FIELD(1, int32, mode_type)
	DECLARE_DF_FIELD(2, int32, reason_type)
	DECLARE_DF_FIELD(3, FString, explain)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoReportUserViaChannelResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoWorldJoinRequest)
	DECLARE_DF_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoWorldJoinResponse)
	DECLARE_DF_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoWorldLeaveRequest)
	DECLARE_DF_FIELD(0, FString, world_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoWorldLeaveResponse)
	DECLARE_DF_FIELD(0, TArray<FString>, world_id_list)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelUpSkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, left_skill_point)
	DECLARE_DF_FIELD(2, B2SkillServerInfoPtr, skill)
	DECLARE_DF_FIELD(3, int32, current_gold)
	DECLARE_DF_FIELD(4, TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetSkill)
	DECLARE_DF_FIELD(0, int32, current_gem)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_DF_FIELD(2, int32, left_skill_point)
	DECLARE_DF_FIELD(3, TArray<B2SkillServerInfoPtr>, skills)
	DECLARE_DF_FIELD(4, TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetUsingSkill)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, B2SkillQuickSlotPtr, skill_quick_slot)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuySkillPoint)
	DECLARE_DF_FIELD(0, int32, left_gem)
	DECLARE_DF_FIELD(1, TArray<B2BuySkillPointResponseInfoPtr>, buy_results)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveNickname)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateNickname)
	DECLARE_DF_FIELD(0, FString, old_nickname)
	DECLARE_DF_FIELD(1, FString, new_nickname)
	DECLARE_DF_FIELD(2, B2ItemServerInfoPtr, updated_item)
	DECLARE_DF_FIELD(3, int64, deleted_item_id)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetServerVersion)
	DECLARE_DF_FIELD(0, FString, version)
	DECLARE_DF_FIELD(1, FString, stage)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveCharacterName)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveActRepeatClearReward)
	DECLARE_DF_FIELD(0, int32, clear_count)
	DECLARE_DF_FIELD(1, int32, next_require_clear_count)
	DECLARE_DF_FIELD(2, int32, next_reward_gold)
	DECLARE_DF_FIELD(3, int32, rewarded_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveActClearReward)
	DECLARE_DF_FIELD(0, int32, reward_index)
	DECLARE_DF_FIELD(1, int32, reward_gold)		// @NOTE 삭제 예정
		DECLARE_DF_FIELD(2, int32, reward_gem)			// @NOTE 삭제 예정
		DECLARE_DF_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetTeamBattleStatus)
	DECLARE_DF_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(1, int32, from_ranking)
	DECLARE_DF_FIELD(2, int32, total_ranker_count)
	DECLARE_DF_FIELD(3, B2MatchStatusPtr, match_status)
	DECLARE_DF_FIELD(4, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(5, TArray<B2BattleFormationPtr>, formations)
	DECLARE_DF_FIELD(6, B2TeamBattleEntryPtr, entry)
	DECLARE_DF_FIELD(7, B2RankingRewardPtr, ranking_reward)
	DECLARE_DF_FIELD(8, TArray<B2HallOfFamePtr>, hall_of_fames)
	DECLARE_DF_FIELD(9, int64, settlement_time)
	DECLARE_DF_FIELD(10, int32, settlement_period_hour)
	DECLARE_DF_FIELD(11, bool, today_reward)
	DECLARE_DF_FIELD(12, bool, last_reward)
	DECLARE_DF_FIELD(13, int32, today_participation_count)
	DECLARE_DF_FIELD(14, int32, current_gold)
	DECLARE_DF_FIELD(15, int32, current_bladepoint)
	DECLARE_DF_FIELD(16, int32, current_gem)
	DECLARE_DF_FIELD(17, int32, match_point)
	DECLARE_DF_FIELD(18, int32, additional_match_count)
	DECLARE_DF_FIELD(19, int32, match_point_next_charge_second)
	DECLARE_DF_FIELD(20, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(21)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAssaultBattleStatus)
	DECLARE_DF_FIELD(0, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(1, int32, from_ranking)
	DECLARE_DF_FIELD(2, int32, total_ranker_count)
	DECLARE_DF_FIELD(3, B2MatchStatusPtr, match_status)
	DECLARE_DF_FIELD(4, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(5, int32, battle_character_type)
	DECLARE_DF_FIELD(6, int32, total_power)
	DECLARE_DF_FIELD(7, B2RankingRewardPtr, ranking_reward)
	DECLARE_DF_FIELD(8, TArray<B2HallOfFamePtr>, hall_of_fames)
	DECLARE_DF_FIELD(9, int64, settlement_time)
	DECLARE_DF_FIELD(10, int32, settlement_period_hour)
	DECLARE_DF_FIELD(11, bool, today_reward)
	DECLARE_DF_FIELD(12, bool, last_reward)
	DECLARE_DF_FIELD(13, int32, today_participation_count)
	DECLARE_DF_FIELD(14, int32, current_gold)
	DECLARE_DF_FIELD(15, int32, current_bladepoint)
	DECLARE_DF_FIELD(16, int32, current_gem)
	DECLARE_DF_FIELD(17, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_DF_FIELD(18, int32, mvp_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(19)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateTeamBattleEntry)
	DECLARE_DF_FIELD(0, B2TeamBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceTeamBattleFormation)
	DECLARE_DF_FIELD(0, B2BattleFormationPtr, formation)
	DECLARE_DF_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTeamMatchmaking)
	DECLARE_DF_FIELD(0, FString, match_token)
	DECLARE_DF_FIELD(1, B2TeamPtr, opponent_team)
	DECLARE_DF_FIELD(2, int32, current_gem)
	DECLARE_DF_FIELD(3, int32, addadditional_match_count)
	DECLARE_DF_FIELD(4, int32, match_point)
	DECLARE_DF_FIELD(5, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTeamMatchResult)
	DECLARE_DF_FIELD(0, B2MatchStatusPtr, match_status)
	DECLARE_DF_FIELD(1, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(2, B2MatchRankerPtr, prev_ranker)
	DECLARE_DF_FIELD(3, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_DF_FIELD(4, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_COMMAND(FB2MessageInfoChatCommand)
	DECLARE_DF_FIELD(0, int32, type)
	DECLARE_DF_FIELD(1, FString, to)
	DECLARE_DF_FIELD(2, int32, format)
	DECLARE_DF_FIELD(3, FString, body)
	DECLARE_DF_FIELD(4, int32, character_level_max)
	DECLARE_B2MESSAGEINFO_COMMAND_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoChatNotify)
	DECLARE_DF_FIELD(0, int32, type)
	DECLARE_DF_FIELD(1, FString, from)
	DECLARE_DF_FIELD(2, int32, format)
	DECLARE_DF_FIELD(3, FString, body)
	DECLARE_DF_FIELD(4, B2GuildInfoPtr, guild)
	DECLARE_DF_FIELD(5, int32, notice_type)
	DECLARE_DF_FIELD(6, TArray<FString>, notice_args)
	DECLARE_DF_FIELD(7, int64, account_id)
	DECLARE_DF_FIELD(8, int32, character_level_max)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoAdminMessageNotify)
	DECLARE_DF_FIELD(0, TArray<B2AdminMessagePtr>, messages)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildChat)
	DECLARE_DF_FIELD(0, int32, format)
	DECLARE_DF_FIELD(1, FString, body)
	DECLARE_DF_FIELD(2, int32, character_level_max)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildChat)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetGuildChat)
	DECLARE_DF_FIELD(0, int32, chat_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildChat)
	DECLARE_DF_FIELD(0, TArray<B2GuildChatDataPtr>, chat_list)
	DECLARE_DF_FIELD(1, int64, next_ask_support_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestAskGuildSupport)
	DECLARE_DF_FIELD(0, int32, item_template_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAskGuildSupport)
	DECLARE_DF_FIELD(0, int64, next_ask_support_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildSupport)
	DECLARE_DF_FIELD(0, int64, target_account_id)
	DECLARE_DF_FIELD(1, int64, chat_id)
	DECLARE_DF_FIELD(2, int32, count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildSupport)
	DECLARE_DF_FIELD(0, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGuildDonation)
	DECLARE_DF_FIELD(0, int32, mercenary_id)
	DECLARE_DF_FIELD(1, int32, donation_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildDonation)
	DECLARE_DF_FIELD(0, B2LightAccountPtr, light_account)
	DECLARE_DF_FIELD(1, B2GuildMemberPtr, guild_member)
	DECLARE_DF_FIELD(2, B2GuildMercenaryPtr, mercenary)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoGuildChatNotify)
	DECLARE_DF_FIELD(0, TArray<B2GuildChatDataPtr>, chat_list)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRank)
	DECLARE_DF_FIELD(0, TArray<B2RankInfoPtr>, rankInfos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceRankNode)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, rank)
	DECLARE_DF_FIELD(2, int32, rank_node_index)
	DECLARE_DF_FIELD(3, bool, rank_node_state)
	DECLARE_DF_FIELD(4, TArray<B2ItemServerInfoPtr>, changed_rank_promotion_points)
	DECLARE_DF_FIELD(5, TArray<int64>, deleted_rank_promotion_point_ids)
	DECLARE_DF_FIELD(6, int32, current_gold)
	DECLARE_DF_FIELD(7, bool, is_success)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMailList)
	DECLARE_DF_FIELD(0, TArray<B2MailInfoPtr>, mail_infos);
	DECLARE_DF_FIELD(1, int32, new_present_mail_count);
	DECLARE_DF_FIELD(2, int32, new_bladepoint_mail_count);
	DECLARE_DF_FIELD(3, int32, new_socialpoint_mail_count);
	DECLARE_DF_FIELD(4, int32, new_lottery_mail_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenMail)
	DECLARE_DF_FIELD(0, int64, mail_id);
	DECLARE_DF_FIELD(1, B2MailItemInfoPtr, mail_item_info);
	DECLARE_DF_FIELD(2, int32, gold);
	DECLARE_DF_FIELD(3, int32, gem);
	DECLARE_DF_FIELD(4, int32, blade_point);
	DECLARE_DF_FIELD(5, int32, social_point);
	DECLARE_DF_FIELD(6, int32, dark_crystal);
	DECLARE_DF_FIELD(7, int32, stage_boost_ticket);
	DECLARE_DF_FIELD(8, int32, stage_auto_clear_ticket);
	DECLARE_DF_FIELD(9, int32, hot_time_gold_buff_ticket);
	DECLARE_DF_FIELD(10, int32, hot_time_exp_buff_ticket);
	DECLARE_DF_FIELD(11, int32, shop_ten_lottery_ticket);
	DECLARE_DF_FIELD(12, int32, fame_point);
	DECLARE_DF_FIELD(13, B2AetherServerInfoPtr, aether);
	DECLARE_DF_FIELD(14, B2TotemServerInfoPtr, totem);
	DECLARE_DF_FIELD(15, int32, exceed_inven_character_type);
	DECLARE_DF_FIELD(16, int32, current_aether_count);
	DECLARE_DF_FIELD(17, int32, aether_inventory_max_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(18)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenAllMail)
	DECLARE_DF_FIELD(0, TArray<int64>, mail_ids);
	DECLARE_DF_FIELD(1, int32, gold);
	DECLARE_DF_FIELD(2, int32, gem);
	DECLARE_DF_FIELD(3, int32, blade_point);
	DECLARE_DF_FIELD(4, int32, social_point);
	DECLARE_DF_FIELD(5, TArray<B2MailItemInfoPtr>, item_infos);
	DECLARE_DF_FIELD(6, int32, dark_crystal);
	DECLARE_DF_FIELD(7, int32, stage_boost_ticket);
	DECLARE_DF_FIELD(8, int32, hot_time_gold_buff_ticket);
	DECLARE_DF_FIELD(9, int32, hot_time_exp_buff_ticket);
	DECLARE_DF_FIELD(10, int32, shop_ten_lottery_ticket);
	DECLARE_DF_FIELD(11, int32, fame_point);
	DECLARE_DF_FIELD(12, int32, stage_auto_clear_ticket);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(13)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetNewMailCount)
	DECLARE_DF_FIELD(0, int32, new_mail_all);
	DECLARE_DF_FIELD(1, int32, new_present_mail_count);
	DECLARE_DF_FIELD(2, int32, new_bladepoint_mail_count);
	DECLARE_DF_FIELD(3, int32, new_socialpoint_mail_count);
	DECLARE_DF_FIELD(4, int32, new_lottery_mail_count);
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
	DECLARE_DF_FIELD(0, TArray<B2FriendPtr>, friends);
	DECLARE_DF_FIELD(1, int32, left_delete_friend_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelAskFriend)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteFriend)
	DECLARE_DF_FIELD(0, int32, left_delete_friend_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAllSendAskFriend)
	DECLARE_DF_FIELD(0, TArray<B2UserPtr>, acceptors);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAllReceiveAskFriend)
	DECLARE_DF_FIELD(0, TArray<B2UserPtr>, requestors);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSendSocialPoint)
	DECLARE_DF_FIELD(0, int32, current_social_point);
	DECLARE_DF_FIELD(1, TArray<int64>, success_account_ids);
	DECLARE_DF_FIELD(2, int64, fail_account_id);
	DECLARE_DF_FIELD(3, int32, fail_error_code);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRecommendFriend)
	DECLARE_DF_FIELD(0, TArray<B2UserPtr>, recommended_users);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindUser)
	DECLARE_DF_FIELD(0, B2UserPtr, user);
	DECLARE_DF_FIELD(1, int32, guild_invite_state);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseConfirmNewFriend)
	DECLARE_DF_FIELD(0, TArray<long>, applied_friend_ids);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteInfo)
	DECLARE_DF_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteCountUp)
	DECLARE_DF_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendJoinerCountUp)
	DECLARE_DF_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePlatformFriendInviteReward)
	DECLARE_DF_FIELD(0, B2FriendInviteRewardInfoPtr, invite_reward_info)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetQuest)
	DECLARE_DF_FIELD(0, TArray<B2QuestInfoPtr>, quest_infos);
	DECLARE_DF_FIELD(1, bool, daily_quest_initialized);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAcceptQuest)
	DECLARE_DF_FIELD(0, B2QuestInfoPtr, quest_info);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveQuestReward)
	DECLARE_DF_FIELD(0, int32, received_reward_slot);
	DECLARE_DF_FIELD(1, B2QuestInfoPtr, next_quest_info);
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoQuestAlarmNotify)
	DECLARE_DF_FIELD(0, int32, completed_slot);
	DECLARE_DF_FIELD(1, B2QuestInfoPtr, next_quest_info);
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_NOTIFY_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCounterDungeon)
	DECLARE_DF_FIELD(0, TArray<int32>, opened_dungeon_types);
	DECLARE_DF_FIELD(1, int32, left_ticket);
	DECLARE_DF_FIELD(2, int32, weekly_clear_count);
	DECLARE_DF_FIELD(3, int32, received_weekly_reward);
	DECLARE_DF_FIELD(4, int32, next_buy_ticket_gem_cost);
	DECLARE_DF_FIELD(5, int32, weekly_reset_day_of_week);
	DECLARE_DF_FIELD(6, int32, main_character_type);
	DECLARE_DF_FIELD(7, int32, buy_ticket_count);
	DECLARE_DF_FIELD(8, int32, buy_ticket_count_max);
	DECLARE_DF_FIELD(9, int32, highest_difficulty_level);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartCounterDungeon)
	DECLARE_DF_FIELD(0, FString, play_token);
	DECLARE_DF_FIELD(1, int32, drop_gold);
	DECLARE_DF_FIELD(2, TArray<B2GuildSkillPtr>, guild_skills);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishCounterDungeon)
	DECLARE_DF_FIELD(0, int32, current_gold);
	DECLARE_DF_FIELD(1, int32, current_gem);
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_DF_FIELD(3, int32, left_ticket);
	DECLARE_DF_FIELD(4, int32, weekly_clear_count);
	DECLARE_DF_FIELD(5, int32, next_buy_ticket_gem_cost);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepCounterDungeon)
	DECLARE_DF_FIELD(0, int32, dungeon_id);
	DECLARE_DF_FIELD(1, int32, current_gold);
	DECLARE_DF_FIELD(2, int32, current_gem);
	DECLARE_DF_FIELD(3, TArray<B2RewardPtr>, received_rewards);
	DECLARE_DF_FIELD(4, int32, left_ticket);
	DECLARE_DF_FIELD(5, int32, weekly_clear_count);
	DECLARE_DF_FIELD(6, int32, next_buy_ticket_gem_cost);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyCounterDungeonTicket)
	DECLARE_DF_FIELD(0, int32, current_gem);
	DECLARE_DF_FIELD(1, int32, left_ticket);
	DECLARE_DF_FIELD(2, int32, buy_ticket_count);
	DECLARE_DF_FIELD(3, int32, buy_ticket_count_max);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCounterDungeonWeeklyReward)
	DECLARE_DF_FIELD(0, int32, current_gold);
	DECLARE_DF_FIELD(1, int32, current_gem);
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_DF_FIELD(3, int32, received_weekly_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDimension)
	DECLARE_DF_FIELD(0, B2DimensionStatusPtr, dimension_status);
	DECLARE_DF_FIELD(1, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(2, int32, from_ranking)
	DECLARE_DF_FIELD(3, int32, total_ranker_count)
	DECLARE_DF_FIELD(4, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(5, B2DimensionRankingRewardPtr, ranking_reward)
	DECLARE_DF_FIELD(6, int64, settlement_time)
	DECLARE_DF_FIELD(7, int32, settlement_period_hour)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartDimension)
	DECLARE_DF_FIELD(0, FString, play_token);
	DECLARE_DF_FIELD(1, TArray<B2GuildSkillPtr>, guild_skills);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishDimension)
	DECLARE_DF_FIELD(0, int32, play_result);
	DECLARE_DF_FIELD(1, B2DimensionStatusPtr, dimension_status);
	DECLARE_DF_FIELD(2, bool, is_new_record);
	DECLARE_DF_FIELD(3, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_DF_FIELD(4, TArray<B2DropDataPtr>, fixed_items);
	DECLARE_DF_FIELD(5, TArray<B2ItemServerInfoPtr>, update_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCreateGuild)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAskJoinGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLeaveGuild)
	DECLARE_DF_FIELD(0, int64, leave_guild_time);
	DECLARE_DF_FIELD(1, int64, guild_id);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildDetailInfo)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild)
	DECLARE_DF_FIELD(1, TArray<B2GuildMemberPtr>, guild_members)
	DECLARE_DF_FIELD(2, B2GuildBattleStatePtr, guild_battle_state)
	DECLARE_DF_FIELD(3, B2GuildBattleEntryPtr, entry)
	DECLARE_DF_FIELD(4, bool, has_turn_reward)
	DECLARE_DF_FIELD(5, TArray<B2GuildMercenaryPtr>, mercenaries)
	DECLARE_DF_FIELD(6, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_DF_FIELD(7, bool, joined_guild_battle_member)
	DECLARE_DF_FIELD(8, TArray<B2CharacterPowerPtr>, character_powers)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildMember)
	DECLARE_DF_FIELD(0, TArray<B2GuildMemberPtr>, guild_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRecommendGuild)
	DECLARE_DF_FIELD(0, TArray<B2GuildPtr>, guilds)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildMemberCandidates)
	DECLARE_DF_FIELD(0, TArray<B2GuildMemberCandidatePtr>, candidates)
	DECLARE_DF_FIELD(1, bool, need_friend_list)
	DECLARE_DF_FIELD(2, TArray<B2FriendPtr>, friends)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildInviteList)
	DECLARE_DF_FIELD(0, TArray<B2GuildInvitePtr>, invites)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRejectInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAcceptInviteGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFindGuild)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseObtainGuildMaster)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild)
	DECLARE_DF_FIELD(1, TArray<B2GuildMemberPtr>, changed_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseHandOverGuildMaster)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild)
	DECLARE_DF_FIELD(1, TArray<B2GuildMemberPtr>, changed_members)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildRankingList)
	DECLARE_DF_FIELD(0, TArray<B2GuildRankingPtr>, guild_rankings)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAppointSubGuildMaster)
	DECLARE_DF_FIELD(0, B2GuildMemberPtr, changed_member)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismissSubGuildMaster)
	DECLARE_DF_FIELD(0, B2GuildMemberPtr, changed_member)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKickGuildMember)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateGuild)
	DECLARE_DF_FIELD(0, int32, join_option)
	DECLARE_DF_FIELD(1, int32, mark_index)
	DECLARE_DF_FIELD(2, int32, mark_color)
	DECLARE_DF_FIELD(3, int32, mark_bg_index)
	DECLARE_DF_FIELD(4, int32, mark_bg_color)
	DECLARE_DF_FIELD(5, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateGuildNotice)
	DECLARE_DF_FIELD(0, FString, notice)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelUpGuildSkill)
	DECLARE_DF_FIELD(0, B2GuildSkillPtr, skill)
	DECLARE_DF_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyGuildSkill)
	DECLARE_DF_FIELD(0, B2GuildSkillPtr, skill)
	DECLARE_DF_FIELD(1, int32, current_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMission)
	DECLARE_DF_FIELD(0, TArray<B2MissionInfoPtr>, mission_infos);
	DECLARE_DF_FIELD(1, TArray<B2MissionCompletePointInfoPtr>, mission_complete_point_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveMissionReward)
	DECLARE_DF_FIELD(0, B2MissionInfoPtr, received_mission_info);
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveCompleteMissionReward)
	DECLARE_DF_FIELD(0, int32, received_mission_type);
	DECLARE_DF_FIELD(1, int32, received_reward_index);
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, received_rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetWing)
	DECLARE_DF_FIELD(0, int32, character_type);
	DECLARE_DF_FIELD(1, int32, grade);
	DECLARE_DF_FIELD(2, int32, level);
	DECLARE_DF_FIELD(3, int32, enhance_point);
	DECLARE_DF_FIELD(4, int32, upgrade_fail_point);
	DECLARE_DF_FIELD(5, TArray<B2WingOptionInfoPtr>, wing_option_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceWing)
	DECLARE_DF_FIELD(0, int32, character_type);
	DECLARE_DF_FIELD(1, int32, level);
	DECLARE_DF_FIELD(2, int32, enhance_point);
	DECLARE_DF_FIELD(3, int32, current_money);
	DECLARE_DF_FIELD(4, int32, material_item_count);
	DECLARE_DF_FIELD(5, TArray<B2ItemServerInfoPtr>, changed_material_items)
	DECLARE_DF_FIELD(6, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpgradeWing)
	DECLARE_DF_FIELD(0, int32, character_type);
	DECLARE_DF_FIELD(1, bool, isUpgradeSuccess);
	DECLARE_DF_FIELD(2, int32, level);
	DECLARE_DF_FIELD(3, int32, grade);
	DECLARE_DF_FIELD(4, int32, current_money);
	DECLARE_DF_FIELD(5, int32, fail_point);
	DECLARE_DF_FIELD(6, int32, material_item_count);
	DECLARE_DF_FIELD(7, TArray<B2ItemServerInfoPtr>, changed_material_items)
	DECLARE_DF_FIELD(8, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceWingOption)
	DECLARE_DF_FIELD(0, int32, character_type);
	DECLARE_DF_FIELD(1, B2WingOptionInfoPtr, enhanced_wing_option);
	DECLARE_DF_FIELD(2, int32, material_item_count);
	DECLARE_DF_FIELD(3, TArray<B2ItemServerInfoPtr>, changed_material_items)
	DECLARE_DF_FIELD(4, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetHeroTower)
	DECLARE_DF_FIELD(0, int32, start_floor)
	DECLARE_DF_FIELD(1, int32, max_clear_floor)
	DECLARE_DF_FIELD(2, int32, main_character_type)
	DECLARE_DF_FIELD(3, int32, tag_character_type)
	DECLARE_DF_FIELD(4, int32, daily_try_count)
	DECLARE_DF_FIELD(5, int32, current_free_ticket)				// 현재 무료 입장 가능 횟수
		DECLARE_DF_FIELD(6, int32, current_buy_ticket)					// 현재 구매된 유료 입장 횟수
		DECLARE_DF_FIELD(7, int32, possible_try_count)					// 입장 가능한 횟수
		DECLARE_DF_FIELD(8, B2ItemServerInfoPtr, current_hero_piece)
	DECLARE_DF_FIELD(9, TArray<B2RelicPtr>, relics)
	DECLARE_DF_FIELD(10, bool, is_sweep)
	DECLARE_DF_FIELD(11, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(12, int32, from_ranking)
	DECLARE_DF_FIELD(13, int32, total_ranker_count)
	DECLARE_DF_FIELD(14, B2RankingPtr, ranking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(15)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartHeroTower)
	DECLARE_DF_FIELD(0, FString, play_token)
	DECLARE_DF_FIELD(1, int32, left_free_ticket)
	DECLARE_DF_FIELD(2, int32, daily_enter_count)
	DECLARE_DF_FIELD(3, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishHeroTower)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, rewards)
	DECLARE_DF_FIELD(1, int32, next_floor)
	DECLARE_DF_FIELD(2, FString, play_token)
	DECLARE_DF_FIELD(3, bool, is_continue)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAbandonHeroTower)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceRelic)
	DECLARE_DF_FIELD(0, B2RelicPtr, relic)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_hero_piece_items)
	DECLARE_DF_FIELD(2, TArray<int64>, deleted_hero_piece_item_ids)
	DECLARE_DF_FIELD(3, int32, current_money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePromotionRelic)
	DECLARE_DF_FIELD(0, B2RelicPtr, relic)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_boss_piece_items)
	DECLARE_DF_FIELD(2, TArray<int64>, deleted_boss_piece_item_ids)
	DECLARE_DF_FIELD(3, int32, current_money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountRelic)
	DECLARE_DF_FIELD(0, TArray<B2RelicPtr>, relics)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSweepHeroTower)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyHeroTowerTicket)
	DECLARE_DF_FIELD(0, int32, current_cash)
	DECLARE_DF_FIELD(1, int32, current_free_ticket)
	DECLARE_DF_FIELD(2, int32, current_buy_ticket)
	DECLARE_DF_FIELD(3, int32, possible_try_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDrawShopLottery)
	DECLARE_DF_FIELD(0, bool, free_lottery)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(2, int32, shop_ten_lottery_ticket)
	DECLARE_DF_FIELD(3, int32, gem)
	DECLARE_DF_FIELD(4, int32, social_point)
	DECLARE_DF_FIELD(5, int32, mileage)
	DECLARE_DF_FIELD(6, int64, next_free_time)
	DECLARE_DF_FIELD(7, int32, daily_lottery_count)
	DECLARE_DF_FIELD(8, FString, last_lottery_key)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardShopMileage)
	DECLARE_DF_FIELD(0, int32, shop_mileage_type)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, received_rewards);
	DECLARE_DF_FIELD(2, int32, received_reward_index)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetLotteryShop)
	DECLARE_DF_FIELD(0, int32, mileage)
	DECLARE_DF_FIELD(1, int32, reward_index);
	DECLARE_DF_FIELD(2, int32, shop_ten_lottery_ticket);
	DECLARE_DF_FIELD(3, TArray<B2ShopLotteryPtr>, shop_lotteries)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyShopProduct)
	DECLARE_DF_FIELD(0, int32, buy_price_type)
	DECLARE_DF_FIELD(1, int32, price)
	DECLARE_DF_FIELD(2, B2BuyShopResultInfoPtr, buy_result_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePurchaseCostume)
	DECLARE_DF_FIELD(0, int32, buy_price_type)
	DECLARE_DF_FIELD(1, int32, price)
	DECLARE_DF_FIELD(2, TArray<B2CostumeServerInfoPtr>, costumes)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGeneralShop)
	DECLARE_DF_FIELD(0, TArray<B2ShopMileagePtr>, shop_mileages)
	DECLARE_DF_FIELD(1, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
	DECLARE_DF_FIELD(2, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCostumeShop)
	DECLARE_DF_FIELD(0, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
	DECLARE_DF_FIELD(1, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMagicShop)
	DECLARE_DF_FIELD(0, int32, renewal_count)
	DECLARE_DF_FIELD(1, int64, next_free_renewal_time)
	DECLARE_DF_FIELD(2, bool, is_free_renewal)
	DECLARE_DF_FIELD(3, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRenewalMagicShop)
	DECLARE_DF_FIELD(0, int32, renewal_count)
	DECLARE_DF_FIELD(1, int64, next_free_renewal_time)
	DECLARE_DF_FIELD(2, int32, current_gem)
	DECLARE_DF_FIELD(3, bool, is_free_renewal)
	DECLARE_DF_FIELD(4, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePurchaseMagicShop)
	DECLARE_DF_FIELD(0, int32, renewal_count)
	DECLARE_DF_FIELD(1, int64, next_free_renewal_time)
	DECLARE_DF_FIELD(2, int32, buy_price_type)
	DECLARE_DF_FIELD(3, int32, price)
	DECLARE_DF_FIELD(4, bool, is_free_renewal)
	DECLARE_DF_FIELD(5, TArray<B2MagicShopPtr>, magic_shops)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetCollectionItem)
	DECLARE_DF_FIELD(0, TArray<B2CollectionItemPtr>, collection_items)
	DECLARE_DF_FIELD(1, TArray<B2CollectionSetItemPtr>, reward_collection_items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardCollectionItem)
	DECLARE_DF_FIELD(0, int32, item_template_id)
	DECLARE_DF_FIELD(1, B2RewardPtr, received_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardCollectionSetItem)
	DECLARE_DF_FIELD(0, int32, gropup_id)
	DECLARE_DF_FIELD(1, int32, grade)
	DECLARE_DF_FIELD(2, int32, character_type)
	DECLARE_DF_FIELD(3, B2RewardPtr, received_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetMarketProduct)
	DECLARE_DF_FIELD(0, TArray<B2mdMarketProductPtr>, md_market_products)
	DECLARE_DF_FIELD(1, TArray<B2MarketPurchaseInfoPtr>, purchase_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePreparePurchase)
	DECLARE_DF_FIELD(0, B2MarketPurchaseInfoPtr, purchase_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseVerifyPurchase)
	DECLARE_DF_FIELD(0, int32, product_id)
	DECLARE_DF_FIELD(1, FString, market_product_id)
	DECLARE_DF_FIELD(2, FString, developer_payload)
	DECLARE_DF_FIELD(3, B2BuyShopResultInfoPtr, buy_result_info)
	DECLARE_DF_FIELD(4, int32, receive_error_code)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelPurchase)
	DECLARE_DF_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishPurchase)
	DECLARE_DF_FIELD(0, FString, developer_payload)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRaidResult)
	DECLARE_DF_FIELD(0, TArray<B2RaidRankingRewardPtr>, raid_rewards)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(2, TArray<B2LightCharacterPtr>, light_characters)
	DECLARE_DF_FIELD(3, B2RewardPtr, step_clear_reward)
	DECLARE_DF_FIELD(4, B2RewardPtr, main_reward)
	DECLARE_DF_FIELD(5, B2RewardPtr, main_bonus_reward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDailyAttendance)
	DECLARE_DF_FIELD(0, bool, is_attendance)
	DECLARE_DF_FIELD(1, int32, attendance_day)
	DECLARE_DF_FIELD(2, int32, attendance_type)
	DECLARE_DF_FIELD(3, B2RewardPtr, reward)
	DECLARE_DF_FIELD(4, int64, next_attendance_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEventAttendance)
	DECLARE_DF_FIELD(0, int32, eventId)
	DECLARE_DF_FIELD(1, int32, attendance_day)
	DECLARE_DF_FIELD(2, B2RewardPtr, reward)
	DECLARE_DF_FIELD(3, int64, next_attendance_time)
	DECLARE_DF_FIELD(4, int32, bonus_type)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBeginnerAttendance)
	DECLARE_DF_FIELD(0, int32, attendance_day)
	DECLARE_DF_FIELD(1, B2RewardPtr, reward)
	DECLARE_DF_FIELD(2, int64, next_attendance_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateDailyPlayTime)
	DECLARE_DF_FIELD(0, int32, daily_play_time_in_sec)
	DECLARE_DF_FIELD(1, int32, daily_play_time_reward_index)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveDailyPlayTimeReward)
	DECLARE_DF_FIELD(0, int32, daily_play_time_in_sec)
	DECLARE_DF_FIELD(1, int32, daily_play_time_reward_index)
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseJoinGuildBattle)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetGuildBattle)
	DECLARE_DF_FIELD(0, int64, battle_end_time)
	DECLARE_DF_FIELD(1, B2GuildBattleGuildPtr, my_guild)
	DECLARE_DF_FIELD(2, B2GuildBattleGuildPtr, target_guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartGuildBattle)
	DECLARE_DF_FIELD(0, FString, battle_token)
	DECLARE_DF_FIELD(1, B2FindAccountInfoPtr, target)
	DECLARE_DF_FIELD(2, int32, target_left_medal)
	DECLARE_DF_FIELD(3, B2FindAccountInfoPtr, my_info)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseFinishGuildBattle)
	DECLARE_DF_FIELD(0, int32, battle_result)
	DECLARE_DF_FIELD(1, int32, obtain_medal)
	DECLARE_DF_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildBattleEntry)
	DECLARE_DF_FIELD(0, B2GuildBattleEntryPtr, entry)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleTurnResult)
	DECLARE_DF_FIELD(0, int64, mvp_account_id)
	DECLARE_DF_FIELD(1, int32, battle_result)
	DECLARE_DF_FIELD(2, int32, increased_exp)
	DECLARE_DF_FIELD(3, B2GuildBattleGuildPtr, my_guild)
	DECLARE_DF_FIELD(4, B2GuildBattleGuildPtr, target_guild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleTurnReward)
	DECLARE_DF_FIELD(0, int32, battle_result)
	DECLARE_DF_FIELD(1, int32, guild_medal)
	DECLARE_DF_FIELD(2, int32, reward_grade)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleSeasonReward)
	DECLARE_DF_FIELD(0, int32, season_ranking)
	DECLARE_DF_FIELD(1, int32, season_ranking_total_count)
	DECLARE_DF_FIELD(2, int32, gem)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGuildBattleHistory)
	DECLARE_DF_FIELD(0, TArray<B2GuildBattleHistoryPtr>, battle_histories)
	DECLARE_DF_FIELD(1, B2GuildBattleRecordPtr, season_record)
	DECLARE_DF_FIELD(2, B2GuildBattleRecordPtr, best_season_record)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetWingVisible)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetHelmetVisible)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCostumeVisible)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, costume_type)
	DECLARE_DF_FIELD(2, bool, is_visible)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetRaid)
	DECLARE_DF_FIELD(0, int64, penalty_remove_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRaidResurrection)
	DECLARE_DF_FIELD(0, int32, resurrection_count)
	DECLARE_DF_FIELD(1, int32, used_gem)
	DECLARE_DF_FIELD(2, int32, total_gem)
	DECLARE_DF_FIELD(3, int32, next_resurrection_gem_cost)
	DECLARE_DF_FIELD(4, int32, selected_count_resurrection_buff1)
	DECLARE_DF_FIELD(5, int32, selected_count_resurrection_buff2)
	DECLARE_DF_FIELD(6, int32, selected_count_resurrection_buff3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)


	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultMatchmaking)
	DECLARE_DF_FIELD(0, FString, token)
	DECLARE_DF_FIELD(1, int32, room_type)
	DECLARE_DF_FIELD(2, B2EndpointPtr, channel_endpoint)
	DECLARE_DF_FIELD(3, TArray<B2GuildSkillPtr>, guild_skills)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCancelAssaultMatchmaking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultResult)
	DECLARE_DF_FIELD(0, B2MatchStatusPtr, match_status)
	DECLARE_DF_FIELD(1, B2RankingPtr, ranking)
	DECLARE_DF_FIELD(2, B2MatchRankerPtr, prev_ranker)
	DECLARE_DF_FIELD(3, TArray<B2RewardPtr>, victory_rewards)
	DECLARE_DF_FIELD(4, int32, increased_mvp_point)
	DECLARE_DF_FIELD(5, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAssaultResurrection)
	DECLARE_DF_FIELD(0, int32, resurrection_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckPvPMatchPointCharge)
	DECLARE_DF_FIELD(0, int32, match_point)
	DECLARE_DF_FIELD(1, int32, match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckTeamMatchPointCharge)
	DECLARE_DF_FIELD(0, int32, match_point)
	DECLARE_DF_FIELD(1, int32, match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFriendRankings)
	DECLARE_DF_FIELD(0, int32, opponent_type)
	DECLARE_DF_FIELD(1, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, random_option_id1)
	DECLARE_DF_FIELD(2, int32, random_option_id2)
	DECLARE_DF_FIELD(3, int32, random_option_id3)
	DECLARE_DF_FIELD(4, float, random_option_value1)
	DECLARE_DF_FIELD(5, float, random_option_value2)
	DECLARE_DF_FIELD(6, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGenerateItemRandomOption)
	DECLARE_DF_FIELD(0, int32, current_dark_crystal)
	DECLARE_DF_FIELD(1, int32, current_cash)
	DECLARE_DF_FIELD(2, int32, random_option_id1)
	DECLARE_DF_FIELD(3, int32, random_option_id2)
	DECLARE_DF_FIELD(4, int32, random_option_id3)
	DECLARE_DF_FIELD(5, float, random_option_value1)
	DECLARE_DF_FIELD(6, float, random_option_value2)
	DECLARE_DF_FIELD(7, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseApplyItemRandomOption)
	DECLARE_DF_FIELD(0, B2ItemServerInfoPtr, item_data)
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
			DECLARE_DF_FIELD(0, bool, has_new_equipment)
			DECLARE_DF_FIELD(1, bool, has_new_collection)
			DECLARE_DF_FIELD(2, bool, has_mission_to_reward)
			DECLARE_DF_FIELD(3, bool, has_mission_mileage_reward)
			DECLARE_DF_FIELD(4, bool, has_serial_mission_to_reward)
			DECLARE_DF_FIELD(5, bool, has_friend_to_send_point)
			DECLARE_DF_FIELD(6, bool, has_friend_request)
			DECLARE_DF_FIELD(7, bool, has_adventure_mileage_reward)
			DECLARE_DF_FIELD(8, int32, counter_dungeon_weekly_clear_count)
			DECLARE_DF_FIELD(9, int32, counter_dungeon_weekly_reward_received)
			DECLARE_DF_FIELD(10, int32, tickets_for_counter_dungeon)
			DECLARE_DF_FIELD(11, int32, used_tickets_for_hero_tower)
			DECLARE_DF_FIELD(12, int32, tickets_for_raid)
			DECLARE_DF_FIELD(13, bool, has_one_vs_one_reward)
			DECLARE_DF_FIELD(14, bool, has_team_battle_reward)
			DECLARE_DF_FIELD(15, bool, has_team_assault_reward)
			DECLARE_DF_FIELD(16, int32, team_assault_mvp_point)
			DECLARE_DF_FIELD(17, int64, remaining_free_item_time)
			DECLARE_DF_FIELD(18, bool, has_shop_item_mileage_reward)
			DECLARE_DF_FIELD(19, bool, has_shop_cash_mileage_reward)
			DECLARE_DF_FIELD(20, int32, unread_mail_count)
			DECLARE_DF_FIELD(21, bool, has_present_mail)
			DECLARE_DF_FIELD(22, bool, has_blade_point_mail)
			DECLARE_DF_FIELD(23, bool, has_social_point_mail)
			DECLARE_DF_FIELD(24, bool, has_lottery_mail)
			DECLARE_DF_FIELD(25, bool, has_item_random_option_change)
		DECLARE_B2MESSAGEINFO_REQUEST_END(26)
	*/

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseDarkCrystalPaid)
	DECLARE_DF_FIELD(0, int32, dark_crystal_paid)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyItemRandomOption)
	DECLARE_DF_FIELD(0, int64, item_id)
	DECLARE_DF_FIELD(1, int32, random_option_id1)
	DECLARE_DF_FIELD(2, int32, random_option_id2)
	DECLARE_DF_FIELD(3, int32, random_option_id3)
	DECLARE_DF_FIELD(4, float, random_option_value1)
	DECLARE_DF_FIELD(5, float, random_option_value2)
	DECLARE_DF_FIELD(6, float, random_option_value3)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyItemNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyCostumeNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseStartRaid)
	DECLARE_DF_FIELD(0, TArray<B2InventoryCountServerInfoPtr>, inventory_count_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_NOTIFY(FB2MessageInfoSessionClosedNotify)
	DECLARE_DF_FIELD(0, int32, reason)
	DECLARE_B2MESSAGEINFO_NOTIFY_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetContentsOpenStatus)
	DECLARE_DF_FIELD(0, TArray<B2ContentsOpenStatusPtr>, contents)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelRewardBoxInfo)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_DF_FIELD(1, TArray<B2DuelRewardBoxInfoResponseRewardBoxPtr>, reward_boxes)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelRewardBoxGainableList)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_DF_FIELD(1, int32, grade)
	DECLARE_DF_FIELD(2, TArray<B2DuelRewardBoxGainableListResponseRewardItemPtr>, reward_items)
	DECLARE_DF_FIELD(3, int32, reward_item_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseOpenDuelRewardBox)
	DECLARE_DF_FIELD(0, int32, duel_mode)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, received_rewards)
	DECLARE_DF_FIELD(2, B2DuelRewardBoxStatusPtr, reward_box_status)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveAssaultMvpReward)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, received_rewards)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountAether)
	DECLARE_DF_FIELD(0, TArray<B2AetherServerInfoPtr>, aethers)
	DECLARE_DF_FIELD(1, TArray<B2CharacterSetOptionInfoPtr>, current_character_set_option_Infos)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, dismantle_aether_Ids)
	DECLARE_DF_FIELD(1, B2ItemServerInfoPtr, current_aether_piece)
	DECLARE_DF_FIELD(2, int32, obtain_aether_piece_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMountAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, mounted_aether_ids)
	DECLARE_DF_FIELD(1, TArray<int64>, unmounted_aether_ids)
	DECLARE_DF_FIELD(2, int32, selected_offense_set_option_id)
	DECLARE_DF_FIELD(3, int32, selected_defense_set_option_id)
	DECLARE_DF_FIELD(4, int32, selected_unity_skill_set_option_id)
	DECLARE_DF_FIELD(5, TArray<int32>, defense_set_option_ids)
	DECLARE_DF_FIELD(6, TArray<int32>, unity_skill_set_option_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnmountAethers)
	DECLARE_DF_FIELD(0, TArray<int64>, unmounted_aether_ids)
	DECLARE_DF_FIELD(1, int32, selected_offense_set_option_id)
	DECLARE_DF_FIELD(2, int32, selected_defense_set_option_id)
	DECLARE_DF_FIELD(3, int32, selected_unity_skill_set_option_id)
	DECLARE_DF_FIELD(4, TArray<int32>, defense_set_option_ids)
	DECLARE_DF_FIELD(5, TArray<int32>, unity_skill_set_option_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSelectAetherDefenseOption)
	DECLARE_DF_FIELD(0, int32, selected_defense_set_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSelectAetherUnitySkillOption)
	DECLARE_DF_FIELD(0, int32, selected_unity_skill_set_option_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockAethers)
	DECLARE_DF_FIELD(0, TArray<B2AetherServerInfoPtr>, locked_aethers)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockAethers)
	DECLARE_DF_FIELD(0, TArray<B2AetherServerInfoPtr>, unlocked_aethers)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseExtendAetherInventory)
	DECLARE_DF_FIELD(0, B2AccountInfoPtr, account)
	DECLARE_DF_FIELD(1, int32, current_slot_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEnhanceAether)
	DECLARE_DF_FIELD(0, B2AetherServerInfoPtr, aether)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, changed_aether_piece_items)
	DECLARE_DF_FIELD(2, TArray<int64>, deleted_aether_piece_ids)
	DECLARE_DF_FIELD(3, bool, is_success)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellAethers)
	DECLARE_DF_FIELD(0, int32, current_gold)
	DECLARE_DF_FIELD(1, TArray<int64>, sell_aether_ids)
	DECLARE_DF_FIELD(2, int32, obtain_gold)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseModifyAetherNewFlag)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetTutorialProgress)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, bool, is_item_prepared)
	DECLARE_DF_FIELD(2, bool, is_lottery_drawn)
	DECLARE_DF_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUpdateTutorialProgress)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, bool, is_item_prepared)
	DECLARE_DF_FIELD(2, bool, is_lottery_drawn)
	DECLARE_DF_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSkipTutorialProgress)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, bool, is_item_prepared)
	DECLARE_DF_FIELD(2, bool, is_lottery_drawn)
	DECLARE_DF_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDrawTutorialItem)
	DECLARE_DF_FIELD(0, bool, free_lottery)
	DECLARE_DF_FIELD(1, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(2, int32, shop_ten_lottery_ticket)
	DECLARE_DF_FIELD(3, int32, gem)
	DECLARE_DF_FIELD(4, int32, social_point)
	DECLARE_DF_FIELD(5, int32, mileage)
	DECLARE_DF_FIELD(6, int64, next_free_time)
	DECLARE_DF_FIELD(7, int32, daily_lottery_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareTutorial)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, int32, current_cash)
	DECLARE_DF_FIELD(2, int32, current_gold)
	DECLARE_DF_FIELD(3, TArray<B2ItemServerInfoPtr>, added_item_list)
	DECLARE_DF_FIELD(4, TArray<B2AetherServerInfoPtr>, added_aether_list)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetUnitySkillMission)
	DECLARE_DF_FIELD(0, TArray<B2UnitySkillMissionPtr>, missions)
	DECLARE_DF_FIELD(1, TArray<B2UnitySkillAwakenMissionPtr>, awaken_missions)
	DECLARE_DF_FIELD(2, bool, all_mission_completed)
	DECLARE_DF_FIELD(3, bool, all_awaken_mission_completed)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveUnitySkillReward)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward)
	DECLARE_DF_FIELD(1, B2UnitySkillMissionPtr, next_mission)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAwakenUnitySkillMission)
	DECLARE_DF_FIELD(0, B2UnitySkillAwakenMissionPtr, awaken_mission)
	DECLARE_DF_FIELD(1, int32, current_money)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_material_items)
	DECLARE_DF_FIELD(3, TArray<int64>, deleted_item_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAwakenUnitySkill)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDuelModeInfo)
	DECLARE_DF_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos)
	DECLARE_DF_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetChallengeModeInfo)
	DECLARE_DF_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos)
	DECLARE_DF_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
	DECLARE_DF_FIELD(2, int32, hero_tower_ticket_count)
	DECLARE_DF_FIELD(3, int32, counter_dungeon_left_ticket_count)
	DECLARE_DF_FIELD(4, int32, dimension_daily_clear_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetHallOfFame)
	DECLARE_DF_FIELD(0, TArray<B2HallOfFamePtr>, basics)
	DECLARE_DF_FIELD(1, TArray<B2HallOfFameUserPtr>, rankers)
	DECLARE_DF_FIELD(2, TArray<B2HallOfFameRewardPtr>, ranker_rewards)
	DECLARE_DF_FIELD(3, TArray<B2HallOfFamePraisePtr>, left_praise_counts)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePraiseTargetRanker)
	DECLARE_DF_FIELD(0, TArray<B2HallOfFamePtr>, basics)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_DF_FIELD(2, B2HallOfFamePraisePtr, left_praise_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveRestReward)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward)
	DECLARE_DF_FIELD(1, int32, used_cash)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckModeOpen)
	DECLARE_DF_FIELD(0, TArray<B2MatchOpenPeriodInfoPtr>, raid_open_period_infos)
	DECLARE_DF_FIELD(1, TArray<B2MatchOpenPeriodInfoPtr>, assault_open_period_infos)
	DECLARE_DF_FIELD(2, int32, guild_battle_state)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveTutorialReward)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, tutorial_rewards)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckReachPower)
	DECLARE_DF_FIELD(0, TArray<B2CharacterPowerPtr>, character_power)
	DECLARE_DF_FIELD(1, TArray<int32>, required_tracking_targets)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTrackingPowerDetail)
	DECLARE_DF_FIELD(0, int32, character_type)
	DECLARE_DF_FIELD(1, int32, item_preset_id)
	DECLARE_DF_FIELD(2, int32, skill_slot)
	DECLARE_DF_FIELD(3, TArray<B2CharacterPowerSpecificPtr>, specific_power_values)
	DECLARE_DF_FIELD(4, int32, different_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoMessageProcessing)
	DECLARE_DF_FIELD(0, FString, message_box_id)
	DECLARE_DF_FIELD(1, int32, processing_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoCouponUse)
	DECLARE_DF_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoAccountConversionPrepare)
	DECLARE_DF_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseKakaoAccountConversionComplete)
	DECLARE_DF_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetDonationEvent)
	DECLARE_DF_FIELD(0, int32, donation_id)
	DECLARE_DF_FIELD(1, int32, free_donation_count)
	DECLARE_DF_FIELD(2, int32, point)
	DECLARE_DF_FIELD(3, int32, whole_step)
	DECLARE_DF_FIELD(4, TArray<B2DonationPointRewardStatePtr>, point_reward_state)
	DECLARE_DF_FIELD(5, bool, settled)
	DECLARE_DF_FIELD(6, TArray<B2MatchRankerPtr>, rankers)
	DECLARE_DF_FIELD(7, int32, from_ranking)
	DECLARE_DF_FIELD(8, int32, total_ranker_count)
	DECLARE_DF_FIELD(9, B2RankingPtr, ranking)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMakeDonation)
	DECLARE_DF_FIELD(0, bool, is_free_donation)
	DECLARE_DF_FIELD(1, int32, free_donation_count)
	DECLARE_DF_FIELD(2, TArray<B2DonationPresentPtr>, donation_presents)
	DECLARE_DF_FIELD(3, int32, decrease_gem)
	DECLARE_DF_FIELD(4, int32, current_gem)
	DECLARE_DF_FIELD(5, int32, decrease_gold)
	DECLARE_DF_FIELD(6, int32, current_gold)
	DECLARE_DF_FIELD(7, int32, obtain_point)
	DECLARE_DF_FIELD(8, int32, current_point)
	DECLARE_DF_FIELD(9, FString, last_donation_key)
	DECLARE_DF_FIELD(10, B2RawRewardPtr, bundled_fixed_reward)
	DECLARE_DF_FIELD(11, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_DF_FIELD(12, int32, present_gem)
	DECLARE_DF_FIELD(13, int32, present_blade_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(14)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDonationPointReward)
	DECLARE_DF_FIELD(0, TArray<B2RawRewardPtr>, rewards)
	DECLARE_DF_FIELD(1, TArray<B2DonationPointRewardStatePtr>, point_reward_state)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFairy)
	DECLARE_DF_FIELD(0, TArray<B2FairyStatusPtr>, fairies)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLevelupFairy)
	DECLARE_DF_FIELD(0, B2FairyStatusPtr, updated_fairy)
	DECLARE_DF_FIELD(1, B2ItemServerInfoPtr, changed_material_item)
	DECLARE_DF_FIELD(2, int64, deleted_item_id)
	DECLARE_DF_FIELD(3, bool, is_levelup)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBlessFairy)
	DECLARE_DF_FIELD(0, B2FairyStatusPtr, updated_fairy)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveFairyGift)
	DECLARE_DF_FIELD(0, bool, jackpoted)
	DECLARE_DF_FIELD(1, int32, multiple_value)
	DECLARE_DF_FIELD(2, int32, fairy_type)
	DECLARE_DF_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_DF_FIELD(4, int64, next_fairy_gift_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseCheckChargePoints)
	DECLARE_DF_FIELD(0, int32, blade_point)
	DECLARE_DF_FIELD(1, int32, blade_point_max)
	DECLARE_DF_FIELD(2, int32, blade_point_charge_lefttime_second)
	DECLARE_DF_FIELD(3, int32, pvp_match_point)
	DECLARE_DF_FIELD(4, int32, pvp_match_point_charge_lefttime_second)
	DECLARE_DF_FIELD(5, int32, team_match_point)
	DECLARE_DF_FIELD(6, int32, team_match_point_charge_lefttime_second)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetFeaturedMission)
	DECLARE_DF_FIELD(0, TArray<B2MissionInfoPtr>, mission_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetEvent)
	DECLARE_DF_FIELD(0, TArray<B2PointEventStatusPtr>, point_event_statuses);
	DECLARE_DF_FIELD(1, TArray<B2LevelUpEventInfoPtr>, level_up_event_infos);
	DECLARE_DF_FIELD(2, TArray<B2StageClearEventStatusPtr>, stage_clear_event_statuses);
	DECLARE_DF_FIELD(3, TArray<B2PointShopEventStatusPtr>, point_shop_event_statuses);
	DECLARE_DF_FIELD(4, TArray<B2TenLotteryEventStatusPtr>, ten_lottery_event_statuses);
	DECLARE_DF_FIELD(5, TArray<B2GemPurchaseEventStatusPtr>, gem_purchase_event_statuses);
	DECLARE_DF_FIELD(6, TArray<B2SpendGemEventStatusPtr>, spend_gem_event_statuses);
	DECLARE_DF_FIELD(7, TArray<B2TenLotterySavingsStatusPtr>, ten_lottery_savings_event_statuses);
	DECLARE_DF_FIELD(8, TArray<B2DiceEventStatusPtr>, dice_event_statuses);
	DECLARE_DF_FIELD(9, TArray<B2PointShopEventRewardHistoryPtr>, point_shop_event_reward_histories);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceivePointEventReward)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveLevelUpEventReward)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetSeasonMission)
	DECLARE_DF_FIELD(0, TArray<B2SeasonMissionInfoPtr>, missions);
	DECLARE_DF_FIELD(1, TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardSeasonMission)
	DECLARE_DF_FIELD(0, B2SeasonMissionInfoPtr, reward_mission);
	DECLARE_DF_FIELD(1, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAttendSeasonMission)
	DECLARE_DF_FIELD(0, TArray<B2SeasonMissionEventStatusPtr>, event_statuses);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveStageClearEventReward)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyHotTimeBuff)
	DECLARE_DF_FIELD(0, int32, buff_type);
	DECLARE_DF_FIELD(1, int64, buff_end_time);
	DECLARE_DF_FIELD(2, int32, buff_ticket);
	DECLARE_DF_FIELD(3, int32, current_gem);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardFlatRatePackageBonus)
	DECLARE_DF_FIELD(0, B2RewardPtr, received_reward);
	DECLARE_DF_FIELD(1, B2FlatRatePackageBonusStatePtr, next_bonus_state);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetPackageShop)
	DECLARE_DF_FIELD(0, TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
	DECLARE_DF_FIELD(1, B2FlatRatePackageBonusStatePtr, flat_rate_package_bonus_state);
	DECLARE_DF_FIELD(2, TArray<B2LevelUpPackageMissionPtr>, level_up_package_missions);
	DECLARE_DF_FIELD(3, TArray<B2ReachLevelPackageMissionPtr>, reach_level_package_missions);
	DECLARE_DF_FIELD(4, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
	DECLARE_DF_FIELD(5, TArray<B2ShopProductMarkPtr>, shop_product_marks)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetPeriodPackageState)
	DECLARE_DF_FIELD(0, TArray<B2PeriodicPackageStatePtr>, periodic_package_states);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardLevelUpPackage)
	DECLARE_DF_FIELD(0, B2RewardPtr, received_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardReachLevelPackage)
	DECLARE_DF_FIELD(0, B2RewardPtr, received_reward);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGiveUpQuest)
	DECLARE_DF_FIELD(0, B2QuestInfoPtr, next_quest_info);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetContentsModeState)
	DECLARE_DF_FIELD(0, TArray<B2ContentsModeInfoPtr>, mode_infos);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardPointShopEvent)
	DECLARE_DF_FIELD(0, B2RewardPtr, reward);
	DECLARE_DF_FIELD(1, B2PointShopEventStatusPtr, point_shop_event_status);
	DECLARE_DF_FIELD(2, int32, index);
	DECLARE_DF_FIELD(3, int32, total_reward_count);	// 횟수 제한이 있는 경우만 누적카운트 유효
	DECLARE_DF_FIELD(4, int32, buy_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReportUser)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBlockChat)
	DECLARE_DF_FIELD(0, B2BlockChatUserPtr, blocked_user);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnblockChat)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetBlockChatList)
	DECLARE_DF_FIELD(0, TArray<B2BlockChatUserPtr>, blocked_users);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseClientStateReport)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardTenLotteryEvent)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, rewards);
	DECLARE_DF_FIELD(1, int32, reward_index);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseReceiveGemPurchaseEventReward)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardSpendGemEvent)
	DECLARE_DF_FIELD(0, TArray<B2RewardPtr>, rewards);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardTenLotterySavingsEvent)
	DECLARE_DF_FIELD(0, int32, current_point)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRollDiceEvent)
	DECLARE_DF_FIELD(0, B2RewardPtr, cell_reward)
	DECLARE_DF_FIELD(1, B2RewardPtr, finish_reward)
	DECLARE_DF_FIELD(2, int32, current_position)
	DECLARE_DF_FIELD(3, int32, dice_number)
	DECLARE_DF_FIELD(4, int32, dice_cell_type)
	DECLARE_DF_FIELD(5, bool, is_finish)
	DECLARE_DF_FIELD(6, int32, reward_count_times)
	DECLARE_DF_FIELD(7, int32, free_dice_count)
	DECLARE_DF_FIELD(8, int32, finish_count)
	DECLARE_DF_FIELD(9, int32, roll_dice_count)
	DECLARE_DF_FIELD(10, int32, current_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseBuyDiceEventPoint)
	DECLARE_DF_FIELD(0, int32, current_gem)
	DECLARE_DF_FIELD(1, int32, buy_count)
	DECLARE_DF_FIELD(2, int32, point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRewardDiceEventMileage)
	DECLARE_DF_FIELD(0, int32, reward_value)
	DECLARE_DF_FIELD(1, TArray<B2RewardPtr>, rewards)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	// REPLACEMENT POINT: MESSAGE_INFO HEADER RESPONSE
	// NEVER DELETE LINE ABOVE

	// for console command

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDeleteAccount)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddItemToInventory)
	DECLARE_DF_FIELD(0, TArray<B2ItemServerInfoPtr>, items)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddCostumeToInventory)
	DECLARE_DF_FIELD(0, B2CostumeServerInfoPtr, costume)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddTotemToInventory)
	DECLARE_DF_FIELD(0, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseCashPaid)
	DECLARE_DF_FIELD(0, int32, cash_paid)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseMoney)
	DECLARE_DF_FIELD(0, int32, money)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseSocialPoint)
	DECLARE_DF_FIELD(0, int32, current_social_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetItemLevelup)
	DECLARE_DF_FIELD(0, B2ItemServerInfoPtr, item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterLevelExp)
	DECLARE_DF_FIELD(0, B2CharacterServerInfoPtr, character)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterRankInfo)
	DECLARE_DF_FIELD(0, B2CharacterServerInfoPtr, character)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetCharacterRelic)
	DECLARE_DF_FIELD(0, B2RelicPtr, relic)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetBladePoint)
	DECLARE_DF_FIELD(0, int32, blade_point)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseIncreaseRankPromotionPoint)
	DECLARE_DF_FIELD(0, int32, rank_promotion_point)
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
	DECLARE_DF_FIELD(0, int32, current_hero_piece)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceClearHeroTower)
	DECLARE_DF_FIELD(0, int32, start_floor)
	DECLARE_DF_FIELD(1, int32, max_clear_floor)
	DECLARE_DF_FIELD(2, int32, main_character_type)
	DECLARE_DF_FIELD(3, int32, tag_character_type)
	DECLARE_DF_FIELD(4, int32, daily_try_count)
	DECLARE_DF_FIELD(5, int32, daily_max_free_try_count)
	DECLARE_DF_FIELD(6, int32, daily_left_free_try_count)
	DECLARE_DF_FIELD(7, int32, current_hero_piece)
	DECLARE_DF_FIELD(8, bool, is_sweep)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceStartCounterDungeon)
	DECLARE_DF_FIELD(0, FString, play_token);
	DECLARE_DF_FIELD(1, int32, drop_gold);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareSweepHeroTower)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponsePrepareGuild)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildBattleState)
	DECLARE_DF_FIELD(0, B2GuildBattleStatePtr, state);
	DECLARE_DF_FIELD(1, FString, state_message);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetAttendanceTime)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetEventAttendance)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseResetDailyPlayTimeRecord)
	DECLARE_DF_FIELD(0, int32, daily_play_time_in_sec)
	DECLARE_DF_FIELD(1, int32, daily_play_time_reward_index)
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
	DECLARE_DF_FIELD(0, int32, new_friend_count);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetGuildLevelExp)
	DECLARE_DF_FIELD(0, B2GuildPtr, guild);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseAddAetherToInventory)
	DECLARE_DF_FIELD(0, B2AetherServerInfoPtr, aether)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseForceUpdateAether)
	DECLARE_DF_FIELD(0, B2AetherServerInfoPtr, updated_aether)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetMatchSeasonReward)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetTutorialProgress)
	DECLARE_DF_FIELD(0, int32, tutorial_id)
	DECLARE_DF_FIELD(1, bool, is_item_prepared)
	DECLARE_DF_FIELD(2, bool, is_lottery_drawn)
	DECLARE_DF_FIELD(3, bool, is_finished)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetStageBoostCount)
	DECLARE_DF_FIELD(0, int32, stage_boost_ticket)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSkipMatchTutorial)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSetDebugMode)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseServerCmd)
	DECLARE_DF_FIELD(0, FString, result_message)
	DECLARE_DF_FIELD(1, bool, need_restart)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseTrace)
	DECLARE_DF_FIELD(0, FString, tag)
	DECLARE_DF_FIELD(1, int64, client_send_time)
	DECLARE_DF_FIELD(2, int64, session_recv_time)
	DECLARE_DF_FIELD(3, int64, app_recv_time)
	DECLARE_DF_FIELD(4, int64, db_request_time)
	DECLARE_DF_FIELD(5, int64, db_response_time)
	DECLARE_DF_FIELD(6, int64, app_send_time)
	DECLARE_DF_FIELD(7, int64, session_send_time)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(8)

	// totem
	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetAccountTotem)
	DECLARE_B2MESSAGEINFO_REQUEST_END_NO_FIELD()

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetAccountTotem)
	DECLARE_DF_FIELD(0, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestEquipTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseEquipTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, equiped_totem_ids)
	DECLARE_DF_FIELD(1, TArray<int64>, unequiped_totem_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnequipTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnequipTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, unequiped_totem_ids)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestLockTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseLockTotems)
	DECLARE_DF_FIELD(0, TArray<B2TotemServerInfoPtr>, locked_totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnlockTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnlockTotems)
	DECLARE_DF_FIELD(0, TArray<B2TotemServerInfoPtr>, unlocked_totems)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestSellTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseSellTotems)
	DECLARE_DF_FIELD(0, int32, current_gold)
	DECLARE_DF_FIELD(1, TArray<int64>, sell_totem_ids)
	DECLARE_DF_FIELD(2, int32, obtain_gold)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDismantleTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, totem_ids)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseDismantleTotems)
	DECLARE_DF_FIELD(0, TArray<int64>, dismantle_totem_ids)
	DECLARE_DF_FIELD(1, B2ItemServerInfoPtr, current_dimensional_piece)
	DECLARE_DF_FIELD(2, int32, obtain_dimensional_piece_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestRefineTotem)
	DECLARE_DF_FIELD(0, int64, totem_id)
	DECLARE_DF_FIELD(1, int32, option_id)
	DECLARE_DF_FIELD(2, int32, cost_type)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseRefineTotem)
	DECLARE_DF_FIELD(0, int32, current_gold)
	DECLARE_DF_FIELD(1, B2TotemServerInfoPtr, refined_totem)
	DECLARE_DF_FIELD(2, TArray<B2ItemServerInfoPtr>, changed_dimensional_piece_items)
	DECLARE_DF_FIELD(3, TArray<int64>, deleted_dimensional_piece_ids)
	DECLARE_DF_FIELD(4, bool, is_success)
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
	DECLARE_DF_FIELD(0, int32, channel_number)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoChangeChatChannelRequest)
	DECLARE_DF_FIELD(0, int32, channel_number)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoChangeChatChannelResponse)
	DECLARE_B2MESSAGEINFO_RESPONSE_END_NO_FIELD()


	//////////////////////////////////////////////////////////////////////////
	// item random quality
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGetItemRandomQuality)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGetItemRandomQuality)
	DECLARE_DF_FIELD(0, TArray<B2ItemServerInfoPtr>, main_item_infos)
	DECLARE_DF_FIELD(1, int64, item_id)
	DECLARE_DF_FIELD(2, int32, quality)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestGenerateItemRandomQuality)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int64, material_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseGenerateItemRandomQuality)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_DF_FIELD(1, int64, material_item_id)
	DECLARE_DF_FIELD(2, int32, quality)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestApplyItemRandomQuality)
	DECLARE_DF_FIELD(0, int64, main_item_id)
	DECLARE_B2MESSAGEINFO_REQUEST_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseApplyItemRandomQuality)
	DECLARE_DF_FIELD(0, B2ItemServerInfoPtr, main_item)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestDiscardItemRandomQuality)
	DECLARE_DF_FIELD(0, int64, main_item_id)
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
	DECLARE_DF_FIELD(0, TArray<B2FactoryPeriodicStatusPtr>, factory_periodic_status)
	DECLARE_DF_FIELD(1, TArray<B2FactoryLimitStatusPtr>, limit_status)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestMakeFactoryItem)
	DECLARE_DF_FIELD(0, int64, factory_id)
	DECLARE_DF_FIELD(1, int64, make_count)
	DECLARE_DF_FIELD(2, int64, main_item_id)
	DECLARE_DF_FIELD(3, int64, material_item_id1)
	DECLARE_DF_FIELD(4, int64, material_item_id2)
	DECLARE_DF_FIELD(5, int64, material_item_id3)
	DECLARE_DF_FIELD(6, int64, material_item_id4)
	DECLARE_DF_FIELD(7, int64, main_item_template_id)
	DECLARE_DF_FIELD(8, int64, material_item_template_id1)
	DECLARE_DF_FIELD(9, int64, material_item_template_id2)
	DECLARE_DF_FIELD(10, int64, material_item_template_id3)
	DECLARE_DF_FIELD(11, int64, material_item_template_id4)
	DECLARE_B2MESSAGEINFO_REQUEST_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseMakeFactoryItem)
	DECLARE_DF_FIELD(0, int64, factory_id)
	DECLARE_DF_FIELD(1, int64, main_item_id)
	DECLARE_DF_FIELD(2, int64, material_item_id1)
	DECLARE_DF_FIELD(3, int64, material_item_id2)
	DECLARE_DF_FIELD(4, int64, material_item_id3)
	DECLARE_DF_FIELD(5, int64, material_item_id4)
	DECLARE_DF_FIELD(6, TArray<B2FactoryResultPtr>, make_result)
	DECLARE_DF_FIELD(7, int64, left_main_item_count)
	DECLARE_DF_FIELD(8, int64, left_material_item_count1)
	DECLARE_DF_FIELD(9, int64, left_material_item_count2)
	DECLARE_DF_FIELD(10, int64, left_material_item_count3)
	DECLARE_DF_FIELD(11, int64, left_material_item_count4)
	DECLARE_DF_FIELD(12, int64, current_gold)
	DECLARE_DF_FIELD(13, int64, current_gem)
	DECLARE_DF_FIELD(14, int64, current_dark_crystal)
	DECLARE_DF_FIELD(15, int64, cost_gold)
	DECLARE_DF_FIELD(16, int64, cost_gem)
	DECLARE_DF_FIELD(17, int64, cost_dark_crystal)
	DECLARE_DF_FIELD(18, int64, left_count)
	DECLARE_B2MESSAGEINFO_RESPONSE_END(19)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_REQUEST(FB2MessageInfoRequestUnsealBox)
	DECLARE_DF_FIELD(0, int64, seal_box_id)
	DECLARE_DF_FIELD(1, int32, character_type)
	DECLARE_DF_FIELD(2, int64, unseal_count)
	DECLARE_B2MESSAGEINFO_REQUEST_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_B2MESSAGEINFO_RESPONSE(FB2MessageInfoResponseUnsealBox)
	DECLARE_DF_FIELD(0, TArray<B2UnsealBoxRewardPtr>, rewards)
	DECLARE_DF_FIELD(1, int64, seal_box_id)
	DECLARE_DF_FIELD(2, int64, left_amount)
	DECLARE_DF_FIELD(3, int32, gold);
	DECLARE_DF_FIELD(4, int32, gem);
	DECLARE_DF_FIELD(5, int32, blade_point);
	DECLARE_DF_FIELD(6, int32, social_point);
	DECLARE_DF_FIELD(7, int32, dark_crystal);
	DECLARE_DF_FIELD(8, int32, stage_boost_ticket);
	DECLARE_DF_FIELD(9, int32, stage_auto_clear_ticket);
	DECLARE_DF_FIELD(10, int32, shop_ten_lottery_ticket);
	DECLARE_DF_FIELD(11, int32, fame_point);
	DECLARE_B2MESSAGEINFO_RESPONSE_END(12)
};
