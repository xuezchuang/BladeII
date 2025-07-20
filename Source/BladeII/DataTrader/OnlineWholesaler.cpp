
//#include "B2NetworkBridge.h"
#include "OnlineWholesaler.h"
//#include "Message.h"
#include "Event.h"
#include "InternalEvents.h"
//#include "HandlerBank.h"
//#include "B2StageManager.h"
#include "B2PCClassInfo.h"
//#include "B2LobbyInventory.h"


#include "BladeIIGameImpl.h"

namespace data_trader
{
	ExecHandler&	ExecHandler::GetInstance()
	{
		static ExecHandler	inst;
		return inst;
	}

	bool ExecHandler::ExecuteShellCommandAccount(const TCHAR* Cmd)
	{
		if (FParse::Command(&Cmd, TEXT("CreateAccount")))
		{
			FString NickName;
			FParse::Token(Cmd, NickName, true);

			RequestGiveNicknameClass<const FString&>::GetInstance().Signal(NickName);

			return true;
		}
		/*else if (FParse::Command(&Cmd, TEXT("GetAccount")))
		{
			RequestGetAccountClass<>::GetInstance().Signal();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("FindAccountById")))
		{
			FString AccountIdString, CharacterTypeString1, CharacterTypeString2, CharacterTypeString3;
			FParse::Token(Cmd, AccountIdString, true);
			FParse::Token(Cmd, CharacterTypeString1, true);
			FParse::Token(Cmd, CharacterTypeString2, true);
			FParse::Token(Cmd, CharacterTypeString3, true);

			int64 AccountID = FCString::Atoi64(*AccountIdString);
			int32 CharacterType1 = FCString::Atoi(*CharacterTypeString1);
			int32 CharacterType2 = FCString::Atoi(*CharacterTypeString2);
			int32 CharacterType3 = FCString::Atoi(*CharacterTypeString3);

			b2network::B2FindAccountQueryPtr query = std::make_shared<b2network::B2FindAccountQuery>();
			query->account_id = AccountID;
			query->nickname.Empty();
			if (CharacterType1 > 0) query->character_types.Emplace(CharacterType1);
			if (CharacterType2 > 0) query->character_types.Emplace(CharacterType2);
			if (CharacterType3 > 0) query->character_types.Emplace(CharacterType3);

			TArray<b2network::B2FindAccountQueryPtr> queryList;
			queryList.Emplace(query);


			auto EmptyMessage = EmptyMessagePtr(new data_trader::FindAccountMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFindAccount>(messageInfo);

				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseFindAccount::type_list>::Result;
				my_tuple box;
				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseFindAccount");
				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

				UE_LOG(LogBladeII, Log, TEXT("Complete FindAccountById."));
				for (auto accountInfo : Message->account_infos)
				{
					UE_LOG(LogBladeII, Log, TEXT("       - nickname(%s)"), *accountInfo->nickname);
				}
			},
				[](FB2MessageInfoPtr messageInfo)
			{
			}
			));

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFindAccount>(EmptyMessage, queryList);
			Message->SendMessage();
			return true;
		}*/
		//else if (FParse::Command(&Cmd, TEXT("FindAccountByName")))
		//{
		//	FString NicknameString, CharacterTypeString1, CharacterTypeString2, CharacterTypeString3;
		//	FParse::Token(Cmd, NicknameString, true);
		//	FParse::Token(Cmd, CharacterTypeString1, true);
		//	FParse::Token(Cmd, CharacterTypeString2, true);
		//	FParse::Token(Cmd, CharacterTypeString3, true);

		//	int32 CharacterType1 = FCString::Atoi(*CharacterTypeString1);
		//	int32 CharacterType2 = FCString::Atoi(*CharacterTypeString2);
		//	int32 CharacterType3 = FCString::Atoi(*CharacterTypeString3);

		//	b2network::B2FindAccountQueryPtr query = std::make_shared<b2network::B2FindAccountQuery>();
		//	query->account_id = 0;
		//	query->nickname = NicknameString;
		//	if (CharacterType1 > 0) query->character_types.Emplace(CharacterType1);
		//	if (CharacterType2 > 0) query->character_types.Emplace(CharacterType2);
		//	if (CharacterType3 > 0) query->character_types.Emplace(CharacterType3);

		//	TArray<b2network::B2FindAccountQueryPtr> queryList;
		//	queryList.Emplace(query);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FindAccountMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFindAccount>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseFindAccount::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseFindAccount");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		//UE_LOG(LogBladeII, Log, TEXT("Complete FindAccountByName."));
		//		//for (auto accountInfo : Message->account_infos)
		//		//{
		//		//	UE_LOG(LogBladeII, Log, TEXT("       - nickname(%s)"), *accountInfo->nickname);
		//		//}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFindAccount>(EmptyMessage, queryList);
		//	Message->SendMessage();
		//	return true;
		//}
//		else if (FParse::Command(&Cmd, TEXT("GetAccountItem")))
//		{
//			FString CharacterTypeString, InventoryTypeString;
//			FParse::Token(Cmd, CharacterTypeString, true);
//			FParse::Token(Cmd, InventoryTypeString, true);
//
//			int32 CharacterType = FCString::Atoi(*CharacterTypeString);
//			int32 InventoryType = FCString::Atoi(*InventoryTypeString);
//
//			RequestGetAccountItemClass<int32, int32>::GetInstance().Signal(CharacterType, InventoryType);
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("GetSV")))
//		{
//			RequestGetServerVersionClass<>::GetInstance().Signal();
//			return true;
//		}
//		//////////////////////////////////////////////////////////////////////////
//		// for console command
//		else if (FParse::Command(&Cmd, TEXT("DeleteAccount")))		// DeleteAccount
//		{
//			UE_LOG(LogBladeII, Log, TEXT("DeleteAccount."));
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::DeleteAccountMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDeleteAccount>(messageInfo);
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete DeleteAccount."));
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDeleteAccount>(EmptyMessage);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetDevItem")))
//		{
//			for (int32 i = 0; i < 17; ++i)
//			{
//				if (i == 3 || i == 4 || i == 8 || i == 9)
//					continue;
//
//				ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d 1 100"), 8000000 + i));
//			}
//
//			ExecuteShellCommand(TEXT("IncGold 1000000"));
//			ExecuteShellCommand(TEXT("IncGem 1000000"));
//			ExecuteShellCommand(TEXT("IncDarkCrystal 1000000"));
//			ExecuteShellCommand(TEXT("IncRankPromotionPoint 100000"));
//			ExecuteShellCommand(TEXT("IncSocialPoint 1000"));
//			ExecuteShellCommand(TEXT("SetBladePoint 120"));
//
//			ExecuteShellCommand(TEXT("GetAccount"));
//		}
//		else if (FParse::Command(&Cmd, TEXT("IncGem")))		// IncGem Gem
//		{
//			FString CashString;
//			FParse::Token(Cmd, CashString, true);
//
//			int32 Cash = FCString::Atoi(*CashString);
//
//			UE_LOG(LogBladeII, Log, TEXT("IncreaseGem : Gem(%d)"), Cash);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseGemMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseCashPaid>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseIncreaseCashPaid::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseIncreaseCashPaid");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete increase gem. current gem(%d)"), Message->cash_paid);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseCashPaid>(EmptyMessage, Cash);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("IncGold")))		// IncGold Gold
//		{
//			FString GoldString;
//			FParse::Token(Cmd, GoldString, true);
//
//			int32 Gold = FCString::Atoi(*GoldString);
//
//			UE_LOG(LogBladeII, Log, TEXT("IncreaseGold : Gold(%d)"), Gold);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseGoldMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseMoney>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseIncreaseMoney::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseIncreaseMoney");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete increase gold. current gold(%d)"), Message->money);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseMoney>(EmptyMessage, Gold);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetCharLevelExp")))		// SetCharLevelExp CharacterType(1~4) level exp
//		{
//			FString CharacterTypeIDString, LevelString, ExpString;
//			FParse::Token(Cmd, CharacterTypeIDString, true);
//			FParse::Token(Cmd, LevelString, true);
//			FParse::Token(Cmd, ExpString, true);
//
//			int32 CharacterType = FCString::Atoi(*CharacterTypeIDString);
//			int32 Level = FCString::Atoi(*LevelString);
//			float Exp = FCString::Atof(*ExpString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetCharacterLevelExp : CharacterType(%d), Level(%d), Exp(%.2f)"), CharacterType, Level, Exp);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetCharacterLevelExpMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetCharacterLevelExp>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetCharacterLevelExp::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetCharacterLevelExp");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set character level and exp. current level(%d), exp(%d)"),
//					Message->character->level, Message->character->exp);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetCharacterLevelExp>(EmptyMessage, CharacterType, Level, Exp);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetCharRankInfo")))		// SetCharRankInfo CharacterType(1~4) rank nodeIndex nodeState(bool)
//		{
//			FString CharacterTypeIDString, rankString, nodeIndexString, nodeStateString;
//			FParse::Token(Cmd, CharacterTypeIDString, true);
//			FParse::Token(Cmd, rankString, true);
//			FParse::Token(Cmd, nodeIndexString, true);
//			FParse::Token(Cmd, nodeStateString, true);
//
//			int32 CharacterType = FCString::Atoi(*CharacterTypeIDString);
//			int32 Rank = FCString::Atoi(*rankString);
//			int32 NodeIndex = FCString::Atof(*nodeIndexString);
//			bool NodeState = FCString::ToBool(*nodeStateString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetCharacterRankInfo : CharacterType(%d), Rank(%d), NodeIndex(%d), NodeState(%d)"), CharacterType, Rank, NodeIndex, NodeState);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetCharacterRankInfoMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetCharacterRankInfo>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetCharacterRankInfo::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetCharacterRankInfo");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set character rankInfo. current Rank(%d), NodeIndex(%d), NodeState(%d)"),
//					Message->character->rank, Message->character->rank_node_index, Message->character->rank_node_state);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetCharacterRankInfo>(EmptyMessage, CharacterType, Rank, NodeIndex, NodeState);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("GetDonationEvent")))
//		{
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetDonationEventMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetDonationEvent>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetDonationEvent>(EmptyMessage);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("MakeDonation")))
//		{
//			FString DonationIdString, IsBundleString, IsAgainString, IsFreeString, LastDonationKeyString;
//			FParse::Token(Cmd, DonationIdString, true);
//			FParse::Token(Cmd, IsBundleString, true);
//			FParse::Token(Cmd, IsAgainString, true);
//			FParse::Token(Cmd, IsFreeString, true);
//			FParse::Token(Cmd, LastDonationKeyString, true);
//
//			int32 DonationId = FCString::Atoi(*DonationIdString);
//			bool IsBundle = FCString::ToBool(*IsBundleString);
//			bool IsAgain = FCString::ToBool(*IsAgainString);
//			bool IsFree = FCString::ToBool(*IsFreeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("MakeDonation : DonationId(%d), IsBundle(%d), IsAgain(%d), IsFree(%d), LastDonationKey(%s)"), DonationId, IsBundle, IsAgain, IsFree, *LastDonationKeyString);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::MakeDonationMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseMakeDonation>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestMakeDonation>(EmptyMessage, DonationId, IsBundle, IsAgain, IsFree, LastDonationKeyString);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("DonationPointReward")))
//		{
//			FString DonationIdString, StepString;
//			FParse::Token(Cmd, DonationIdString, true);
//			FParse::Token(Cmd, StepString, true);
//
//			int32 DonationId = FCString::Atoi(*DonationIdString);
//			int32 Step = FCString::Atoi(*StepString);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::DonationPointRewardMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDonationPointReward>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDonationPointReward>(EmptyMessage, DonationId, Step);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("GetFairy")))
//		{	
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetFairyMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetFairy>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetFairy>(EmptyMessage);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("LevelupFairy")))		// LevelupFairy FairyType
//		{
//			FString FairyTypeString;
//			FParse::Token(Cmd, FairyTypeString, true);
//
//			int32 FairyType = FCString::Atoi(*FairyTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("LevelupFairy : FairyType(%d)"), FairyType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::LevelupFairyMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLevelupFairy>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseLevelupFairy::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseLevelupFairy");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete levelup fairy. current FairyType(%d)"), Message->updated_fairy->fairy_type);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLevelupFairy>(EmptyMessage, FairyType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("BlessFairy")))		// BlessFairy FairyType BlessOptionId
//		{
//			FString FairyTypeString, BlessOptionIdString;
//			FParse::Token(Cmd, FairyTypeString, true);
//			FParse::Token(Cmd, BlessOptionIdString, true);
//
//			int32 FairyType = FCString::Atoi(*FairyTypeString);
//			int32 BlessOptionId = FCString::Atoi(*BlessOptionIdString);
//
//			UE_LOG(LogBladeII, Log, TEXT("BlessFairy : FairyType(%d), BlessOptionId(%d)"), FairyType, BlessOptionId);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::BlessFairyMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBlessFairy>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseBlessFairy::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseBlessFairy");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete bless fairy. current FairyType(%d)"), Message->updated_fairy->fairy_type);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBlessFairy>(EmptyMessage, FairyType, BlessOptionId);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ReceiveFairyGift")))		// ReceiveFairyGift FairyType
//		{
//			FString FairyTypeString;
//			FParse::Token(Cmd, FairyTypeString, true);
//
//			int32 FairyType = FCString::Atoi(*FairyTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("ReceiveFairyGift : FairyType(%d)"), FairyType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceiveFairyGiftMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceiveFairyGift>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseReceiveFairyGift::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseReceiveFairyGift");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete receive fairy. fairyType(%d)"), Message->fairy_type);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceiveFairyGift>(EmptyMessage, FairyType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetFairyGiftTime")))
//		{
//			FString CommandStr(FString::Printf(TEXT("SetFairyGiftTime %s"), Cmd));
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ServerCmdMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseServerCmd>(messageInfo);
//				RequestGetAccountClass<>::GetInstance().Signal();
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestServerCmd>(EmptyMessage, CommandStr);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ResetFairyGiftTime")))
//		{
//			FString CommandStr(FString::Printf(TEXT("ResetFairyGiftTime")));
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ServerCmdMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseServerCmd>(messageInfo);
//				RequestGetAccountClass<>::GetInstance().Signal();
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestServerCmd>(EmptyMessage, CommandStr);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetJackpotFairy")))
//		{
//			FString CommandStr(FString::Printf(TEXT("SetJackpotFairy %s"), Cmd));
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ServerCmdMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseServerCmd>(messageInfo);
//				DeliveryJackpotFairyClass<FB2ServerCmdPtr>::GetInstance().Signal(Message);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseServerCmd>(messageInfo);
//				DeliveryJackpotFairyClass<FB2ServerCmdPtr>::GetInstance().Signal(Message);
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestServerCmd>(EmptyMessage, CommandStr);
//			Message->SendMessage();
//			return true;			
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetCharRelic")))		// SetCharRelic CharacterType(1~4) relicId grade enhanceLevel
//		{
//			FString CharacterTypeIDString, RelicIdString, GradeString, EnhanceLevelString;
//			FParse::Token(Cmd, CharacterTypeIDString, true);
//			FParse::Token(Cmd, RelicIdString, true);
//			FParse::Token(Cmd, GradeString, true);
//			FParse::Token(Cmd, EnhanceLevelString, true);
//
//			int32 CharacterType = FCString::Atoi(*CharacterTypeIDString);
//			int32 RelicId = FCString::Atoi(*RelicIdString);
//			int32 Grade = FCString::Atoi(*GradeString);
//			int32 EnhanceLevel = FCString::Atoi(*EnhanceLevelString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetCharacterRelic : CharacterType(%d), RelicId(%d), Grade(%d), EnhanceLevel(%d)"), CharacterType, RelicId, Grade, EnhanceLevel);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetCharacterRelicMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetCharacterRelic>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetCharacterRelic::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetCharacterRelic");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set character relic. current RelicId(%d), Grade(%d), EnhanceLevel(%d)"),
//					Message->relic->relic_id, Message->relic->grade, Message->relic->enhance_level);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetCharacterRelic>(EmptyMessage, CharacterType, RelicId, Grade, EnhanceLevel);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetBladePoint")))		// SetBladePoint BladePoint
//		{
//			FString BladePointString; 
//			FParse::Token(Cmd, BladePointString, true);
//
//			int32 BladePoint = FCString::Atoi(*BladePointString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetBladePoint : BladePoint(%d)"), BladePoint);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetBladePointMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetBladePoint>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetBladePoint::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetBladePoint");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set bladepoint. current baldepoint(%d)"), Message->blade_point);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetBladePoint>(EmptyMessage, BladePoint);
//			Message->SendMessage();
//			return true;
//		}
///*		
//		else if (FParse::Command(&Cmd, TEXT("ExChangeSystem")))		
//		{
//			FString id_string ,exchange_type_string  ;
//			FParse::Token(Cmd, id_string, true);
//			FParse::Token(Cmd, exchange_type_string, true);
//
//			int32 nId = FCString::Atoi(*id_string);
//			int32 nExchange_type = FCString::Atoi(*exchange_type_string);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ExChangeSystemMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoRequestExChangeSystem>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoRequestExChangeSystem::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoRequestExChangeSystem");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestExChangeSystem>(EmptyMessage, nId, nExchange_type);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckExChangeCount")))
//		{
//			FString templateidString ;
//			FParse::Token(Cmd, templateidString, true);
//			int32 templateid = FCString::Atoi(*templateidString);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::CheckExChangeCountMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoRequestCheckExChangeCount>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoRequestCheckExChangeCount::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoRequestCheckExChangeCount");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCheckExChangeCount>(EmptyMessage, templateid);
//			Message->SendMessage();
//			return true;
//		}
//*/
//		else if (FParse::Command(&Cmd, TEXT("CheckRedDot")))
//		{
//			// Server Type
//			FString hintString;
//			TArray<int32> hintList;
//			bool parseResult = FParse::Token(Cmd, hintString, true);
//			while (parseResult) {
//				// B2RedDotHintType in B2MessageInfoCommonStruct.h
//				int32 hintValue = FCString::Atoi(*hintString);
//				hintList.Emplace(hintValue);
//				parseResult = FParse::Token(Cmd, hintString, true);
//			}
//
//			//
//			// if you know which hints to be used, 
//			// you can just use B2RedDotHintType defined in b2network::B2RedDotHintType::<hint name> like following:
//			//
//			// hintList.Emplace(b2network::B2RedDotHintType::RED_DOT_EQUIP);
//			// hintList.Emplace(b2network::B2RedDotHintType::RED_DOT_MISSION);
//			// hintList.Emplace(b2network::B2RedDotHintType::RED_DOT_MAIL);
//			//
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::CheckRedDotMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCheckRedDot>(messageInfo);
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete check lobby alarm mark."));
//				// B2RedDotHintType2PbRedDotHint(hint) to see types and enumeration value mapping.
//				// equip
//				if (Message->is_valid_equip)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[EQUIP] has_new_equipment : %s"), Message->has_new_equipment ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[AETHER] has_new_aether : %s"), Message->has_new_aether ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_EQIP was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// collection
//				if (Message->is_valid_collection)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[COLLECTION] has_new_collection : %s"), Message->has_new_collection ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_COLLECTION was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// mission
//				if (Message->is_valid_mission)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[MISSION] has_mission_to_reward : %s"), Message->has_mission_to_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[MISSION] has_mission_mileage_reward : %s"), Message->has_mission_mileage_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[MISSION] has_serial_mission_to_reward : %s"), Message->has_serial_mission_to_reward ? TEXT("true") : TEXT("false"));
//				}
//				else
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_MISSION was not passed or any other hint(s) has been passed without it."));
//
//				// friend
//				if (Message->is_valid_friend)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[FRIEND] has_new_friend : %s"), Message->has_new_friend ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[FRIEND] has_friend_request : %s"), Message->has_friend_request ? TEXT("true") : TEXT("false"));
//				}
//				else
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_FRIEND was not passed or any other hint(s) has been passed without it."));
//
//				// adventure
//				if (Message->is_valid_adventure)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[ADVENTURE] has_adventure_mileage_reward : %s"), Message->has_adventure_mileage_reward ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_ADVENTURE was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// dungeon
//				if (Message->is_valid_dungeon)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[DUNGEON] counter_dungeon_weekly_clear_count : %d"), Message->counter_dungeon_weekly_clear_count);
//					UE_LOG(LogBladeII, Log, TEXT("[DUNGEON] counter_dungeon_weekly_reward_received : %d"), Message->counter_dungeon_weekly_reward_received);
//					UE_LOG(LogBladeII, Log, TEXT("[DUNGEON] tickets_for_counter_dungeon : %d"), Message->tickets_for_counter_dungeon);
//					UE_LOG(LogBladeII, Log, TEXT("[DUNGEON] tickets_for_hero_tower : %d"), Message->tickets_for_hero_tower);
//					UE_LOG(LogBladeII, Log, TEXT("[DUNGEON] used_tickets_for_hero_tower : %d"), Message->used_tickets_for_hero_tower);
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_DUNGEON was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// pvp
//				if (Message->is_valid_pvp)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[PVP] has_one_vs_one_reward : %s"), Message->has_one_vs_one_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[PVP] has_team_battle_reward : %s"), Message->has_team_battle_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[PVP] has_team_assault_reward : %s"), Message->has_team_assault_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[PVP] team_assault_mvp_point : %d"), Message->team_assault_mvp_point);
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_PVP was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// guild
//				UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_GUILD was not passed or any other hint(s) has been passed without it."));
//
//				// shop
//				if (Message->is_valid_shop)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[SHOP] remaining_free_item_time : %d"), Message->remaining_free_item_time);
//					UE_LOG(LogBladeII, Log, TEXT("[SHOP] has_shop_item_mileage_reward : %s"), Message->has_shop_item_mileage_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[SHOP] has_shop_cash_mileage_reward : %s"), Message->has_shop_cash_mileage_reward ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_SHOP was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// mail
//				if (Message->is_valid_mail)
//				{
//					UE_LOG(LogBladeII, Log, TEXT("[MAIL] unread_mail_count : %d"), Message->unread_mail_count);
//					UE_LOG(LogBladeII, Log, TEXT("[MAIL] has_present_mail : %s"), Message->has_present_mail ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[MAIL] has_blade_point_mail : %s"), Message->has_blade_point_mail ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[MAIL] has_social_point_mail : %s"), Message->has_social_point_mail ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[MAIL] has_lottery_mail : %s"), Message->has_lottery_mail ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_MAIL was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// event
//				if (Message->is_valid_event)
//				{ 
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_level_up_reward : %s"), Message->has_event_level_up_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_point_reward : %s"), Message->has_event_point_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_season_mission_reward : %s"), Message->has_event_season_mission_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_stage_clear_reward : %s"), Message->has_event_stage_clear_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_point_shop_reward : %s"), Message->has_event_point_shop_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_gem_purchase_reward : %s"), Message->has_event_gem_purchase_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_ten_lottery_reward : %s"), Message->has_event_ten_lottery_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_spend_gem_reward : %s"), Message->has_event_spend_gem_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[EVENT] has_event_ten_lottery_savings_reward : %s"), Message->has_event_ten_lottery_savings_reward ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_EVENT was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// shop package
//				if (Message->is_valid_shop_package)
//				{ 
//					UE_LOG(LogBladeII, Log, TEXT("[PACKAGE] has_shop_package_flat_rate_reward : %s"), Message->has_shop_package_flat_rate_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[PACKAGE] has_shop_package_level_up_reward : %s"), Message->has_shop_package_level_up_reward ? TEXT("true") : TEXT("false"));
//					UE_LOG(LogBladeII, Log, TEXT("[PACKAGE] has_shop_package_reach_level_reward : %s"), Message->has_shop_package_reach_level_reward ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_SHOP_PACKAGE was not passed or any other hint(s) has been passed without it."));
//				}
//
//				// option
//				// etc
//				if (Message->is_valid_etc)
//				{ 
//					UE_LOG(LogBladeII, Log, TEXT("[ETC] has_item_random_option_change : %s"), Message->has_item_random_option_change ? TEXT("true") : TEXT("false"));
//				}
//				else
//				{
//					UE_LOG(LogBladeII, Log, TEXT("--- hint RED_DOT_ETC was not passed or any other hint(s) has been passed without it."));
//				}
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCheckRedDot>(EmptyMessage, hintList);
//			Message->SendMessage();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckChargePoints")))
//		{
//			FString CheckBladePointString, CheckPvPMatchPointString, CheckTeamMatchPointString;
//			FParse::Token(Cmd, CheckBladePointString, true);
//			FParse::Token(Cmd, CheckPvPMatchPointString, true);
//			FParse::Token(Cmd, CheckTeamMatchPointString, true);
//
//			bool checkBladePoint = FCString::Atoi64(*CheckBladePointString) == 1 ? true : false;
//			bool checkPvPMatchPoint = FCString::Atoi64(*CheckPvPMatchPointString) == 1 ? true : false;
//			bool checkTeamMatchPoint = FCString::Atoi64(*CheckTeamMatchPointString) == 1 ? true : false;
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::CheckChargePointsMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCheckChargePoints>(messageInfo);
//
//				UE_LOG(LogBladeII, Log, TEXT("-- blade_point: %d"), Message->blade_point);
//				UE_LOG(LogBladeII, Log, TEXT("-- blade_point_max: %d"), Message->blade_point_max);
//				UE_LOG(LogBladeII, Log, TEXT("-- blade_point_charge_lefttime_second: %d"), Message->blade_point_charge_lefttime_second);
//				UE_LOG(LogBladeII, Log, TEXT("-- pvp_match_point: %d"), Message->pvp_match_point);
//				UE_LOG(LogBladeII, Log, TEXT("-- pvp_match_point_charge_lefttime_second: %d"), Message->pvp_match_point_charge_lefttime_second);
//				UE_LOG(LogBladeII, Log, TEXT("-- team_match_point: %d"), Message->team_match_point);
//				UE_LOG(LogBladeII, Log, TEXT("-- team_match_point_charge_lefttime_second: %d"), Message->team_match_point_charge_lefttime_second);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCheckChargePoints>(EmptyMessage, checkBladePoint, checkPvPMatchPoint, checkTeamMatchPoint);
//			Message->SendMessage();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckBladePointCharge")))		// CheckBladePointCharge
//		{
//			UE_LOG(LogBladeII, Log, TEXT("CheckBladePointCharge"));
//
//			RequestCheckBladePointChargeClass<>::GetInstance().Signal();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("Nickname")))
//		{
//			FString Nickname;
//			FParse::Token(Cmd, Nickname, true);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::GiveNicknameMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGiveNickname>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//			}));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGiveNickname>(
//				EmptyMessage, Nickname);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ChangeNickname")))
//		{
//			FString Nickname, ItemTemplateIDString;
//			FParse::Token(Cmd, Nickname, true);
//			FParse::Token(Cmd, ItemTemplateIDString, true);
//
//			int32 ItemTemplateID = FCString::Atoi(*ItemTemplateIDString);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::UpdateNicknameMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpdateNickname>(messageInfo);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//			}));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpdateNickname>(EmptyMessage, Nickname, ItemTemplateID);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("IncRankPromotionPoint")))		// IncRankPromotionPoint RankPromotionPoint
//		{
//			FString rankPromotionPointString;
//			FParse::Token(Cmd, rankPromotionPointString, true);
//
//			int32 rankPromotionPoint = FCString::Atoi(*rankPromotionPointString);
//
//			UE_LOG(LogBladeII, Log, TEXT("IncreaseRankPromotionPoint : RankPromotionPoint(%d)"), rankPromotionPoint);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseGoldMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseRankPromotionPoint>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseIncreaseRankPromotionPoint::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseIncreaseRankPromotionPoint");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete increase rankPromotionPoint. current rank promotion point (%d)"), Message->rank_promotion_point);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseRankPromotionPoint>(EmptyMessage, rankPromotionPoint);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("IncSocialPoint")))
//		{
//			FString SocialPointString;
//			FParse::Token(Cmd, SocialPointString, true);
//
//			int32 SocialPoint = FCString::Atoi(*SocialPointString);
//
//			UE_LOG(LogBladeII, Log, TEXT("IncreaseSocialPoint : SocialPoint(%d)"), SocialPoint);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseSocialPointMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseSocialPoint>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseIncreaseSocialPoint::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseIncreaseSocialPoint");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete increase socialpoint. current socialpoint(%d)"), Message->current_social_point);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseSocialPoint>(EmptyMessage, SocialPoint);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("PrepareGuild")))
//		{
//			FString MemberCountString, AddGuildCountString;
//			FParse::Token(Cmd, MemberCountString, true);
//			FParse::Token(Cmd, AddGuildCountString, true);
//
//			int32 MemberCount = FCString::Atoi(*MemberCountString);
//			int32 AddGuildCount = FCString::Atoi(*AddGuildCountString);
//
//			UE_LOG(LogBladeII, Log, TEXT("PrepareGuild : MemberCount(%d), AddGuildCount(%d)"), MemberCount, AddGuildCount);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::PrepareGuildMessage(
//				[](FB2MessageInfoPtr messageInfo)
//				{
//					auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePrepareGuild>(messageInfo);
//					UE_LOG(LogBladeII, Log, TEXT("Complete prepare guild."));
//				},
//				[](FB2MessageInfoPtr messageInfo)
//				{
//					b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//					// TODO error code
//				}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPrepareGuild>(EmptyMessage, MemberCount, AddGuildCount);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("GuildBattleState")))
//		{
//			FString NewSeasonString;
//			FParse::Token(Cmd, NewSeasonString, true);
//
//			bool NewSeason = FCString::ToBool(*NewSeasonString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetGBState : NewSeason(%d)"), NewSeason);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetGuildBattleStateMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetGuildBattleState>(messageInfo);
//				UE_LOG(LogBladeII, Log, TEXT("Complete set guildbattle state. season(%d) turn(%d) state(%d) msg(%s)"),
//					Message->state->season_id, Message->state->turn, Message->state->current_state, *Message->state_message);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetGuildBattleState>(messageInfo);
//				UE_LOG(LogBladeII, Log, TEXT("Error set guildbattle state. errorCode(%d) msg(%s)"),
//					(int)messageInfo->GetErrorCode(), *Message->state_message);
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetGuildBattleState>(EmptyMessage, NewSeason);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckPvPMatchPointCharge")))
//		{
//			UE_LOG(LogBladeII, Log, TEXT("CheckPvPMatchPointCharge"));
//
//			RequestCheckPvPMatchPointChargeClass<>::GetInstance().Signal();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckTeamMatchPointCharge")))
//		{
//			UE_LOG(LogBladeII, Log, TEXT("CheckTeamMatchPointCharge"));
//
//			RequestCheckTeamMatchPointChargeClass<>::GetInstance().Signal();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("GetAccountAether")))
//		{
//			FString AetherTypeString;
//			FParse::Token(Cmd, AetherTypeString, true);
//
//			int32 AetherType = FCString::Atoi(*AetherTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("GetAccountAether : AetherType(%d)"), AetherType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetAccountAetherMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetAccountAether>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseGetAccountAether::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseGetAccountAether");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				for (b2network::B2AetherServerInfoPtr aether : Message->aethers)
//				{ 
//					UE_LOG(LogBladeII, Log, TEXT("Complete get account aether. current AetherType(%d)"), aether->type);
//				}
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetAccountAether>(EmptyMessage, AetherType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("DismantleAethers")))
//		{
//			FString AetherIdString;
//			FParse::Token(Cmd, AetherIdString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//
//			UE_LOG(LogBladeII, Log, TEXT("AddAetherToInventory : AetherType(%I64d)"), AetherId);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::DismantleAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDismantleAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseDismantleAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseDismantleAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete dismantle aethers. current obtainAetherPiece(%d), currentAetherPiece(%I64d)"), Message->obtain_aether_piece_count, Message->current_aether_piece->amount);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDismantleAethers>(EmptyMessage, AetherIds);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SellAethers")))
//		{
//			FString AetherIdString;
//			FParse::Token(Cmd, AetherIdString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SellAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSellAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSellAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSellAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete sell aethers. current obtainGold(%d), currentGold(%I64d)"), Message->obtain_gold, Message->current_gold);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSellAethers>(EmptyMessage, AetherIds);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("MountAethers")))
//		{
//			FString AetherIdString, CharacterTypeString;
//			FParse::Token(Cmd, AetherIdString, true);
//			FParse::Token(Cmd, CharacterTypeString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//			int32 CharacterType = FCString::Atoi(*CharacterTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("MountAethers : AetherType(%I64d), CharacterTypeString(%d)"), AetherId, CharacterType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::MountAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseMountAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseMountAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseMountAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				FString mountAetherIds, unmountAetherIds;
//				for (auto aetherId : Message->mounted_aether_ids)
//				{
//					mountAetherIds.Append(FString::Printf(TEXT(" %d"), aetherId));
//				}
//				for (auto aetherId : Message->unmounted_aether_ids)
//				{
//					unmountAetherIds.Append(FString::Printf(TEXT(" %d"), aetherId));
//				}
//				UE_LOG(LogBladeII, Log, TEXT("Complete mount aethers. current mountedAetherIds(%s), unmountedAetherIds(%s)"), *mountAetherIds, *unmountAetherIds);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestMountAethers>(EmptyMessage, AetherIds, CharacterType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("UnmountAethers")))
//		{
//			FString AetherIdString, CharacterTypeString;
//			FParse::Token(Cmd, AetherIdString, true);
//			FParse::Token(Cmd, CharacterTypeString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//			int32 CharacterType = FCString::Atoi(*CharacterTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("MountAethers : AetherType(%I64d), CharacterTypeString(%d)"), AetherId, CharacterType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::UnmountAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnmountAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseUnmountAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseUnmountAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				FString unmountAetherIds;
//				for (auto aetherId : Message->unmounted_aether_ids)
//				{
//					unmountAetherIds.Append(FString::Printf(TEXT(" %d"), aetherId));
//				}
//				UE_LOG(LogBladeII, Log, TEXT("Complete unmount aethers. current unmountedAetherIds(%s)"), *unmountAetherIds);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnmountAethers>(EmptyMessage, AetherIds, CharacterType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("LockAethers")))
//		{
//			FString AetherIdString, CharacterTypeString;
//			FParse::Token(Cmd, AetherIdString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//
//			UE_LOG(LogBladeII, Log, TEXT("LockAethers : AetherType(%I64d)"), AetherId);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::LockAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLockAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseLockAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseLockAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				FString aetherIds;
//				for (b2network::B2AetherServerInfoPtr aether : Message->locked_aethers)
//				{
//					aetherIds.Append(FString::Printf(TEXT(" %d"), aether->id));
//				}
//				UE_LOG(LogBladeII, Log, TEXT("Complete lock aethers. current aetherIds(%s)"), *aetherIds);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLockAethers>(EmptyMessage, AetherIds);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("UnlockAethers")))
//		{
//			FString AetherIdString, CharacterTypeString;
//			FParse::Token(Cmd, AetherIdString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//
//			UE_LOG(LogBladeII, Log, TEXT("UnlockAethers : AetherType(%I64d)"), AetherId);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::LockAethersMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnlockAethers>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseUnlockAethers::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseUnlockAethers");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				FString aetherIds;
//				for (b2network::B2AetherServerInfoPtr aether : Message->unlocked_aethers)
//				{
//					aetherIds.Append(FString::Printf(TEXT(" %d"), aether->id));
//				}
//				UE_LOG(LogBladeII, Log, TEXT("Complete unlock aethers. current aetherIds(%s)"), *aetherIds);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			TArray<int64> AetherIds;
//			AetherIds.Emplace(AetherId);
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnlockAethers>(EmptyMessage, AetherIds);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ExtendAetherInventory")))
//		{
//			FString BuyCountString;
//			FParse::Token(Cmd, BuyCountString, true);
//
//			int32 BuyCount = FCString::Atoi(*BuyCountString);
//
//			UE_LOG(LogBladeII, Log, TEXT("ExtendAetherInventory : BuyCount(%d)"), BuyCount);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ExtendAetherInventoryMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseExtendAetherInventory>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseExtendAetherInventory::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseExtendAetherInventory");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete extend aether inventory. current cash(%d) currentSlotCount(%d)"), Message->account->cash, Message->current_slot_count);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestExtendAetherInventory>(EmptyMessage, BuyCount);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("EnhanceAether")))
//		{
//			FString AetherIdString;
//			FParse::Token(Cmd, AetherIdString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//
//			UE_LOG(LogBladeII, Log, TEXT("EnhanceAether : AetherId(%d)"), AetherId);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::EnhanceAetherMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEnhanceAether>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseEnhanceAether::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseEnhanceAether");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete enhance aether. current level(%d) isSuccess(%d)"), Message->aether->level , Message->is_success);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEnhanceAether>(EmptyMessage, AetherId);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ModifyAetherNewFlag")))
//		{
//			FString aetherTypeString;
//			FParse::Token(Cmd, aetherTypeString, true);
//
//			int32 AetherType = FCString::Atoi(*aetherTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("ModifyAetherNewFlag : AetherType(%d)"), AetherType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ModifyAetherNewFlagMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseModifyAetherNewFlag>(messageInfo);
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete ModifyAetherNewFlag"));
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestModifyAetherNewFlag>(EmptyMessage, AetherType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SelectAetherDefenseOption")))
//		{
//			FString setOptionIdString, characterTypeString;
//			FParse::Token(Cmd, setOptionIdString, true);
//			FParse::Token(Cmd, characterTypeString, true);
//
//			int32 SetOptionId = FCString::Atoi(*setOptionIdString);
//			int32 CharacterType = FCString::Atoi(*characterTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SelectAetherDefenseOption : SetOptionId(%d), CharacterType(%d)"), SetOptionId, CharacterType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SelectAetherDefenseOptionMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSelectAetherDefenseOption>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSelectAetherDefenseOption::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSelectAetherDefenseOption");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete SelectAetherDefenseOption"));
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSelectAetherDefenseOption>(EmptyMessage, SetOptionId, CharacterType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SelectAetherUnitySkillOption")))
//		{
//			FString setOptionIdString, characterTypeString;
//			FParse::Token(Cmd, setOptionIdString, true);
//			FParse::Token(Cmd, characterTypeString, true);
//
//			int32 SetOptionId = FCString::Atoi(*setOptionIdString);
//			int32 CharacterType = FCString::Atoi(*characterTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SelectAetherUnitySkillOption : SetOptionId(%d), CharacterType(%d)"), SetOptionId, CharacterType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SelectAetherUnitySkillOptionMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSelectAetherUnitySkillOption>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSelectAetherUnitySkillOption::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSelectAetherUnitySkillOption");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete SelectAetherUnitySkillOption"));
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSelectAetherUnitySkillOption>(EmptyMessage, SetOptionId, CharacterType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("AddAetherToInventory")))
//		{
//			FString AetherTypeString, SlotString, GradeString, TierTypeString;
//			FParse::Token(Cmd, AetherTypeString, true);
//			FParse::Token(Cmd, SlotString, true);
//			FParse::Token(Cmd, GradeString, true);
//			FParse::Token(Cmd, TierTypeString, true);
//
//			int32 AetherType = FCString::Atoi(*AetherTypeString);
//			int32 Slot = FCString::Atoi(*SlotString);
//			int32 Grade = FCString::Atoi(*GradeString);
//			int32 TierType = FCString::Atoi(*TierTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("AddAetherToInventory : AetherType(%d), Slot(%d), Grade(%d), TierType(%d)"), AetherType, Slot, Grade, TierType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::AddAetherToInventoryMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAddAetherToInventory>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseAddAetherToInventory::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseAddAetherToInventory");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete add aether to inventory. current AetherType(%d), Slot(%d), Grade(%d), TierType(%d)"),
//					Message->aether->type, Message->aether->slot, Message->aether->grade, Message->aether->tier);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAddAetherToInventory>(EmptyMessage, AetherType, Slot, Grade, TierType);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("AllAddAetherToInventory")))
//		{
//			FString GradeString, TierTypeString;
//			FParse::Token(Cmd, GradeString, true);
//			FParse::Token(Cmd, TierTypeString, true);
//
//			int32 Grade = FCString::Atoi(*GradeString);
//			int32 TierType = FCString::Atoi(*TierTypeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("AddAetherToInventory : Grade(%d), TierType(%d)"), Grade, TierType);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::AddAetherToInventoryMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAddAetherToInventory>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseAddAetherToInventory::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseAddAetherToInventory");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete add aether to inventory. current AetherType(%d), Slot(%d), Grade(%d), TierType(%d)"),
//					Message->aether->type, Message->aether->slot, Message->aether->grade, Message->aether->tier);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			for (int AetherType = 1; AetherType <= 10 ; AetherType++)
//			{
//				for (int Slot = 1; Slot <= 6; Slot++)
//				{
//					auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAddAetherToInventory>(EmptyMessage, AetherType, Slot, Grade, TierType);
//					Message->SendMessage();
//				}
//			}
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("ForceUpdateAether")))
//		{
//			FString AetherIdString, AetherTypeString, LevelString, GradeString;
//			FParse::Token(Cmd, AetherIdString, true);
//			FParse::Token(Cmd, AetherTypeString, true);
//			FParse::Token(Cmd, LevelString, true);
//			FParse::Token(Cmd, GradeString, true);
//
//			int64 AetherId = FCString::Atoi64(*AetherIdString);
//			int32 AetherType = FCString::Atoi(*AetherTypeString);
//			int32 Level = FCString::Atoi(*LevelString);
//			int32 Grade = FCString::Atoi(*GradeString);
//
//			UE_LOG(LogBladeII, Log, TEXT("AddAetherToInventory : AetherId(%d), AetherType(%d), Level(%d), Grade(%d)"), AetherId, AetherType, Level, Grade);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ForceUpdateAetherMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseForceUpdateAether>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseForceUpdateAether::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseForceUpdateAether");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete force update aether. current AetherId(%d), AetherType(%d), Level(%d), Grade(%d)"),
//					Message->updated_aether->id, Message->updated_aether->type, Message->updated_aether->level, Message->updated_aether->grade);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestForceUpdateAether>(EmptyMessage, AetherId, AetherType, Level, Grade);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetGuildLevel")))
//		{
//			FString LevelString, ExpString;
//			FParse::Token(Cmd, LevelString, true);
//			FParse::Token(Cmd, ExpString, true);
//
//			int32 GuildLevel = FCString::Atoi(*LevelString);
//			int32 GuildExp = FCString::Atoi(*ExpString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetGuildLevel : GuildLevel(%d), GuildExp(%d)"), GuildLevel, GuildExp);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetGuildLevelExpMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetGuildLevelExp>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetGuildLevelExp::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetGuildLevelExp");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set guild level. current GuildLevel(%d), GuildExp(%d)"), Message->guild->level, Message->guild->exp);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetGuildLevelExp>(EmptyMessage, GuildLevel, GuildExp);
//			Message->SendMessage();
//			return true;
//		}
//
//		else if (FParse::Command(&Cmd, TEXT("ReceiveRestReward")))
//		{
//			FString rewardIdString, isDoubleString;
//			FParse::Token(Cmd, rewardIdString, true);
//			FParse::Token(Cmd, isDoubleString, true);
//
//			int32 rewardId = FCString::Atoi(*rewardIdString);
//			bool isDouble = (FCString::Atoi(*isDoubleString) != 0) ? true : false;
//
//			UE_LOG(LogBladeII, Log, TEXT("ReceiveRestReward : RewardID(%d), IsDouble(%d)"), rewardId, isDouble);
//
//			RequestReceiveRestRewardClass<int32, bool>::GetInstance().Signal(rewardId, isDouble);
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("CheckModeOpen")))
//		{
//			RequestCheckModeOpenClass<>::GetInstance().Signal();
//
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetStageBoostCount")))		// SetStageBoostCount count
//		{
//			FString CountValueString;
//			FParse::Token(Cmd, CountValueString, true);
//
//			int32 countValue = FCString::Atoi(*CountValueString);
//
//			UE_LOG(LogBladeII, Log, TEXT("SetStageBoostCount : count(%d)"), countValue);
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::SetStageBoostCountMessage(
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetStageBoostCount>(messageInfo);
//
//				using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetStageBoostCount::type_list>::Result;
//				my_tuple box;
//				const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetStageBoostCount");
//				CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
//
//				UE_LOG(LogBladeII, Log, TEXT("Complete set stageBoostTicket. current stageBoostTicket(%d)"), Message->stage_boost_ticket);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetStageBoostCount>(EmptyMessage, countValue);
//			Message->SendMessage();
//			return true;
//		}
//		else if (FParse::Command(&Cmd, TEXT("SetUltimate")))
//		{
//			// ?�에 ?�라미터가 존재?�다�? 컴뱃 계수 ?�용
//			FString CombatScaleString;
//			FParse::Token(Cmd, CombatScaleString, true);
//			
//			float CombatScale = 1.0f;
//			if(CombatScaleString.IsEmpty() == false)
//			{
//				CombatScale = FCString::Atof(*CombatScaleString);
//				CombatScale = FMath::Clamp(CombatScale, 1.0f, 10.0f);
//			}
//
//			auto EmptyMessage = EmptyMessagePtr(new data_trader::ServerCmdMessage(
//				[CombatScale](FB2MessageInfoPtr messageInfo)
//			{
//				// SetUltimate 치트?�에 ?�버가 ?�상?�으�? ?�답?? ?�을 경우 계수�? ?�용?�다.
//				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseServerCmd>(messageInfo);
//				UpdateCombatStatScaleClass<const float>::GetInstance().Signal(CombatScale);
//			},
//				[](FB2MessageInfoPtr messageInfo)
//			{
//				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
//				// TODO error code
//			}
//			));
//
//			FString CommandUltimateStr(TEXT("SetUltimate"));
//			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestServerCmd>(EmptyMessage, CommandUltimateStr);
//			Message->SendMessage();
//
//			return true;
//		}
		return false;
	}

	bool ExecHandler::ExecuteShellCommandNetWork(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("ResetNetwork")))
		//{
		//	FB2NetworkBridge::GetInstance()->ResetNetwork();
		//	return true;
		//}
  //      else if (FParse::Command(&Cmd, TEXT("ConfigNetwork")))
  //      {
  //          if (FParse::Command(&Cmd, TEXT("ResponseOrder")))
  //          {
  //              if (FParse::Command(&Cmd, TEXT("On")))
  //              {
  //                  FB2NetworkBridge::GetInstance()->Configure("ResponseOrder", true);
  //                  return true;
  //              }
  //              else if (FParse::Command(&Cmd, TEXT("Off")))
  //              {
  //                  FB2NetworkBridge::GetInstance()->Configure("ResponseOrder", false);
  //                  return true;
  //              }
  //          }
  //      }
		//else if (FParse::Command(&Cmd, TEXT("DumpNetwork")))
		//{
		//	FB2NetworkBridge::GetInstance()->Dump();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("TestSodium")))
		//{
		//	FB2NetworkBridge::GetInstance()->TestSodium();
		//	return true;
		//}
		//
		return false;
	}

	bool ExecHandler::ExecuteShellCommandEtc(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("PrintDeviceID")))
		//{
		//	FString uniqueDeviceId = FGenericPlatformMisc::GetDeviceId();
		//	UE_LOG(LogBladeII, Log, TEXT("PrintDeviceID : %s"), *uniqueDeviceId);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetMasterData")))
		//{
		//	RequestGetMasterDataClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetLatestEventData")))
		//{
		//	FString UpdateTimeString;
		//	FParse::Token(Cmd, UpdateTimeString, true);
		//	int32 UpdateTime = FCString::Atoi64(*UpdateTimeString);

		//	RequestGetLatestEventDataClass<int64>::GetInstance().Signal(UpdateTime);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetDebugMode")))
		//{
		//	FString isDebugString, SessionPeriodString, ChannelPeriodString;
		//	FParse::Token(Cmd, isDebugString, true);
		//	FParse::Token(Cmd, SessionPeriodString, true);
		//	FParse::Token(Cmd, ChannelPeriodString, true);

		//	UE_LOG(LogBladeII, Log, TEXT("SetDebugMode... %s"), *isDebugString);

		//	bool isDeubg = FCString::ToBool(*isDebugString);
		//	int SessionPeriod = FCString::Atoi(*SessionPeriodString);
		//	int ChannelPeriod = FCString::Atoi(*ChannelPeriodString);

		//	// to session
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetDebugModeMessage(
		//		[isDeubg](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetDebugMode>(messageInfo);
		//		UE_LOG(LogBladeII, Log, TEXT("SetDebugMode to session... isSuccess(%s)"), Message->IsSuccess() ? TEXT("True") : TEXT("False"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetDebugMode>(EmptyMessage, isDeubg);
		//	Message->SendMessage();

		//	// to channel
		//	if (!FB2NetworkBridge::GetInstance()->IsChannelDisconnected())
		//	{
		//		auto EmptyMessageC = EmptyMessagePtr(new data_trader::SetRoomDebugModeMessage(
		//			[](FB2MessageInfoPtr messageInfoC)
		//		{
		//			auto MessageC = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetRoomDebugMode>(messageInfoC);
		//			UE_LOG(LogBladeII, Log, TEXT("SetDebugMode to channel... isSuccess(%s)"), MessageC->IsSuccess() ? TEXT("True") : TEXT("False"));
		//		},
		//			[](FB2MessageInfoPtr messageInfoC)
		//		{
		//		}));

		//		auto MessageC = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetRoomDebugMode>(EmptyMessageC, isDeubg);
		//		MessageC->SendMessage();
		//	}

		//	// to client
		//	if (SessionPeriod > 0 && ChannelPeriod > 0)
		//	{
		//		FB2NetworkBridge::GetInstance()->SetHeartbeatPeriod(SessionPeriod, ChannelPeriod);
		//	}

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("Invite")))
		//{
		//	FString Nickname;
		//	FParse::Token(Cmd, Nickname, true);

		//	UE_LOG(LogBladeII, Log, TEXT("Inviting... %s"), *Nickname);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::InvitationMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoInvitationResponse>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoInvitationResponse::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoInvitationResponse");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoInvitationRequest>(EmptyMessage, Nickname, FString(""));
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AcceptInvite")))
		//{
		//	FString Inviter;
		//	FString InvitationRef;
		//	FString Invitee;
		//	FString ChannelHost;
		//	FString ChannelPort;
		//	FString RoomId;

		//	FParse::Token(Cmd, Inviter, true);
		//	FParse::Token(Cmd, InvitationRef, true);
		//	FParse::Token(Cmd, Invitee, true);
		//	FParse::Token(Cmd, ChannelHost, true);
		//	FParse::Token(Cmd, ChannelPort, true);
		//	FParse::Token(Cmd, RoomId, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AcceptInvitationMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoAcceptInvitationResponse>(messageInfo);
		//		if (Message->IsSuccess())
		//		{
		//		}
		//		else
		//		{
		//		}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		// TODO error handling
		//	}));

		//	auto Endpoint = std::make_shared<b2network::B2Endpoint>();
		//	Endpoint->host = ChannelHost;
		//	Endpoint->port = FCString::Atoi(*ChannelPort);

		//	auto RoomLocation = std::make_shared<b2network::B2RoomLocation>();
		//	RoomLocation->channel_endpoint = Endpoint;
		//	RoomLocation->room_id = RoomId;

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoAcceptInvitationRequest>(
		//		EmptyMessage,
		//		(int32)b2::protocol::commondata::Invitation::PVP_INVITE,
		//		Inviter,
		//		InvitationRef,
		//		Invitee,
		//		RoomLocation);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DeclineInvite")))
		//{
		//	FString Inviter;
		//	FString InvitationRef;
		//	FString Invitee;

		//	FParse::Token(Cmd, Inviter, true);
		//	FParse::Token(Cmd, InvitationRef, true);
		//	FParse::Token(Cmd, Invitee, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DeclineInvitationMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoDeclineInvitationResponse>(messageInfo);
		//		if (Message->IsSuccess())
		//		{
		//		}
		//		else
		//		{
		//		}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoDeclineInvitationRequest>(
		//		EmptyMessage,
		//		(int32)b2::protocol::commondata::Invitation::PVP_INVITE,
		//		Inviter,
		//		InvitationRef,
		//		Invitee);
		//	Message->SendMessage();
		//	return true;
		//}
  //      else if (FParse::Command(&Cmd, TEXT("RaidInvite")))
  //      {
  //          // TODO
  //          return true;
  //      }
		//else if (FParse::Command(&Cmd, TEXT("StartRaid")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("StartRaid : CharacterType(%d)"), characterType);

		//	RequestStartRaidClass<int32>::GetInstance().Signal(characterType);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("Notice")))
		//{
		//	FString BodyString;
		//	FParse::Token(Cmd, BodyString, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ChatCommand(nullptr, nullptr));

		//	int32 Type = b2::protocol::session::ChatType::NOTICE;
		//	FString To = "";
		//	int32 Format = 0;
		//	int32 CharacterLevelMax = 0;

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoChatCommand>(
		//		EmptyMessage, Type, To, Format, BodyString, CharacterLevelMax);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("Chat")))
		//{
		//	FString ToString;
		//	FString BodyString;
		//	FParse::Token(Cmd, ToString, true);
		//	FParse::Token(Cmd, BodyString, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ChatCommand(nullptr, nullptr));

		//	int32 Type = b2::protocol::session::ChatType::ONE_TO_ONE;
		//	FString To = ToString;
		//	int32 Format = 0;
		//	int32 CharacterLevelMax = 0;

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoChatCommand>(
		//		EmptyMessage, Type, To, Format, BodyString, CharacterLevelMax);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildChat")))
		//{
		//	FString BodyString;
		//	FParse::Token(Cmd, BodyString, true);

		//	UE_LOG(LogBladeII, Log, TEXT("GuildChat... %s"), *BodyString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildChatMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildChat>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	int32 Format = 0;
		//	int32 CharacterLevelMax = 0;
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildChat>(
		//		EmptyMessage, Format, BodyString, CharacterLevelMax);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildChat")))
		//{
		//	FString ChatIDString;
		//	FParse::Token(Cmd, ChatIDString, true);
		//	int32 ChatID = FCString::Atoi(*ChatIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetGuildChat... %d"), ChatID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildChatMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildChat>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	int32 Format = 0;
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildChat>(
		//		EmptyMessage, ChatID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AskGuildSupport")))
		//{
		//	FString ItemTemplateIDString;
		//	FParse::Token(Cmd, ItemTemplateIDString, true);
		//	int32 ItemTemplateID = FCString::Atoi(*ItemTemplateIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("AskGuildSupport... %d"), ItemTemplateID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AskGuildSupportMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAskGuildSupport>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAskGuildSupport>(
		//		EmptyMessage, ItemTemplateID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildSupport")))
		//{
		//	FString AccountIDString, ChatIDString, CountString;
		//	FParse::Token(Cmd, AccountIDString, true);
		//	FParse::Token(Cmd, ChatIDString, true);
		//	FParse::Token(Cmd, CountString, true);
		//	int64 AccountID = FCString::Atoi64(*AccountIDString);
		//	int64 ChatID = FCString::Atoi64(*ChatIDString);
		//	int32 Count = FCString::Atoi(*CountString);

		//	UE_LOG(LogBladeII, Log, TEXT("GuildSupport... TargetAccount(%I64d), ChatID(%I64d), Count(%d)"), AccountID, ChatID, Count);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildSupportMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildSupport>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildSupport>(
		//		EmptyMessage, AccountID, ChatID, Count);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetEvent")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetEventMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetEvent>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetEvent>(
		//		EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PointEventReward")))
		//{
		//	FString eventIdString, rewardIndexString;
		//	FParse::Token(Cmd, eventIdString, true);
		//	FParse::Token(Cmd, rewardIndexString, true);
		//	int32 eventId = FCString::Atoi(*eventIdString);
		//	int32 rewardIndex = FCString::Atoi(*rewardIndexString);

		//	UE_LOG(LogBladeII, Log, TEXT("PointEventReward... eventId(%d), rewardIndex(%d)"), eventId, rewardIndex);


		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceivePointEventRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceivePointEventReward>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceivePointEventReward>(
		//		EmptyMessage, eventId, rewardIndex);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LevelUpEventReward")))
		//{
		//	FString eventIdString, indexString;
		//	FParse::Token(Cmd, eventIdString, true);
		//	FParse::Token(Cmd, indexString, true);
		//	int32 eventId = FCString::Atoi(*eventIdString);
		//	int32 index = FCString::Atoi(*indexString);

		//	UE_LOG(LogBladeII, Log, TEXT("PointEventReward... eventId(%d), index(%d)"), eventId, index);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceivePointEventRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceivePointEventReward>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceivePointEventReward>(
		//		EmptyMessage, eventId, index);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetSeasonMission")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetSeasonMissionMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetSeasonMission>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetSeasonMission>(
		//		EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("SeasonMissionReward")))
		//{
		//	FString eventIdString, dayString, indexString;
		//	FParse::Token(Cmd, eventIdString, true);
		//	FParse::Token(Cmd, dayString, true);
		//	FParse::Token(Cmd, indexString, true);
		//	int32 eventId = FCString::Atoi(*eventIdString);
		//	int32 day = FCString::Atoi(*eventIdString);
		//	int32 index = FCString::Atoi(*indexString);

		//	UE_LOG(LogBladeII, Log, TEXT("SeasonMissionReward... eventId(%d), day(%d), index(%d)"), eventId, day, index);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardSeasonMissionMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardSeasonMission>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardSeasonMission>(
		//		EmptyMessage, eventId, day, index);
		//	Message->SendMessage();
		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("AttendSeasonMission")))
		//{
		//	RequestAttendSeasonMissionClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("Trace")))
		//{
		//	FString Tag;
		//	FParse::Token(Cmd, Tag, true);

		//	UE_LOG(LogBladeII, Log, TEXT("TraceRequest... tag(%s)"), *Tag);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::TraceMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseTrace>(messageInfo);

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//	}));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestTrace>(EmptyMessage, Tag);
		//	Message->SendMessage();
		//	return true;
		//}
		//
		return false;
	}

	bool ExecHandler::ExecuteShellCommandItem(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("AddItem")))		// AddItem ItemRefID ItemQuality(1 : normal, 2 : medium, 3 : high, 4 : highest) ItemAmount
		//{
		//	FString ItemIDString, ItemQualityString, ItemAmountString;
		//	FParse::Token(Cmd, ItemIDString, true);
		//	FParse::Token(Cmd, ItemQualityString, true);
		//	FParse::Token(Cmd, ItemAmountString, true);

		//	int32 ItemRefID = FCString::Atoi(*ItemIDString);
		//	int32 ItemQuality = FCString::Atoi(*ItemQualityString);
		//	int32 ItemAmount = FCString::Atoi(*ItemAmountString);
		//	ItemQuality = FMath::Clamp(ItemQuality, 1, 4);
		//	if (ItemAmount <= 0) {
		//		ItemAmount = 1;
		//	}

		//	UE_LOG(LogBladeII, Log, TEXT("AddItemToInventory : ItemRefID(%d), ItemQuality(%d), ItemAmount(%d)"), ItemRefID, ItemQuality, ItemAmount);

		//	RequestAddItemClass<int32, int32, int32>::GetInstance().Signal(ItemRefID, ItemQuality, ItemAmount);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AddDemoItem")))		// AddDemoItem DemoSetIndex(1~10)
		//{
		//	FString DemoSetIndexString;
		//	FParse::Token(Cmd, DemoSetIndexString, true);

		//	int32 DemoSetIndex = FCString::Atoi(*DemoSetIndexString);
		//	if (DemoSetIndex <= 0 || DemoSetIndex > 12) {
		//		DemoSetIndex = 1;
		//	}

		//	UE_LOG(LogBladeII, Log, TEXT("AddDemoItem. index(%d)"), DemoSetIndex);

		//	RequestAddItemClass<int32, int32, int32>::GetInstance().Signal(DemoSetIndex, 1, 1);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetItemLevelup")))		// SetItemLevelup ItemID level exp
		//{
		//	FString ItemUIDString, LevelString, ExpString, EnhanceLevelString;
		//	FParse::Token(Cmd, ItemUIDString, true);
		//	FParse::Token(Cmd, LevelString, true);
		//	FParse::Token(Cmd, ExpString, true);
		//	FParse::Token(Cmd, EnhanceLevelString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);
		//	int32 ItemLevel = FCString::Atoi(*LevelString);
		//	float ItemExp = FCString::Atof(*ExpString);
		//	int32 ItemEnhanceLevel = FCString::Atoi(*EnhanceLevelString);

		//	UE_LOG(LogBladeII, Log, TEXT("SetItemLevelup : Item(%I64d), Level(%d), Exp(%.2f), EnhanceLevel(%d)"), ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetItemLevelupMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetItemLevelup>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetItemLevelup::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetItemLevelup");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete set item levelup. current level(%d), exp(%.2f), EnhanceLevel(%d)"),
		//			Message->item->level, Message->item->exp, Message->item->enhance_level);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetItemLevelup>(EmptyMessage, ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetItemLevelupEquipPlace")))
		//{
		//	FString PlaceTypeString, LevelString, ExpString, EnhanceLevelString;
		//	FParse::Token(Cmd, PlaceTypeString, true);
		//	FParse::Token(Cmd, LevelString, true);
		//	FParse::Token(Cmd, ExpString, true);
		//	FParse::Token(Cmd, EnhanceLevelString, true);

		//	int32 PlaceType = FCString::Atoi(*PlaceTypeString);
		//	int32 ItemLevel = FCString::Atoi(*LevelString);
		//	float ItemExp = FCString::Atof(*ExpString);
		//	int32 ItemEnhanceLevel = FCString::Atoi(*EnhanceLevelString);

		//	if (PlaceType >= (int32)EItemEquipPlace::EIEP_End)
		//		return false;

		//	FB2Item EquipPlaceItem;
		//	UB2LobbyInventory::GetEquippedItemAtPlace(EquipPlaceItem, (EItemEquipPlace)PlaceType);

		//	if (EquipPlaceItem.IsEmpty())
		//		return false;

		//	int64 ItemUID = EquipPlaceItem.InstanceUID;

		//	UE_LOG(LogBladeII, Log, TEXT("SetItemLevelup : PlaceType(%d), ItemUID(%I64d), Level(%d), Exp(%.2f), EnhanceLevel(%d)"), PlaceType, ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetItemLevelupMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetItemLevelup>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetItemLevelup::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetItemLevelup");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete set item levelup. current level(%d), exp(%.2f), EnhanceLevel(%d)"),
		//			Message->item->level, Message->item->exp, Message->item->enhance_level);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetItemLevelup>(EmptyMessage, ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetItemLevelupEquipPlaceAll")))
		//{
		//	FString LevelString, ExpString, EnhanceLevelString;
		//	FParse::Token(Cmd, LevelString, true);
		//	FParse::Token(Cmd, ExpString, true);
		//	FParse::Token(Cmd, EnhanceLevelString, true);

		//	int32 ItemLevel = FCString::Atoi(*LevelString);
		//	float ItemExp = FCString::Atof(*ExpString);
		//	int32 ItemEnhanceLevel = FCString::Atoi(*EnhanceLevelString);

		//	for (int32 i = 0; i < (int32)EItemEquipPlace::EIEP_End; ++i)
		//	{
		//		FB2Item EquipPlaceItem;
		//		UB2LobbyInventory::GetEquippedItemAtPlace(EquipPlaceItem, (EItemEquipPlace)i);

		//		if (EquipPlaceItem.IsEmpty())
		//			continue;

		//		int64 ItemUID = EquipPlaceItem.InstanceUID;

		//		UE_LOG(LogBladeII, Log, TEXT("SetItemLevelup : PlaceType(%d), ItemUID(%I64d), Level(%d), Exp(%.2f), EnhanceLevel(%d)"), 
		//														i, ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);

		//		auto EmptyMessage = EmptyMessagePtr(new data_trader::SetItemLevelupMessage(
		//			[](FB2MessageInfoPtr messageInfo)
		//		{
		//			auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetItemLevelup>(messageInfo);

		//			using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetItemLevelup::type_list>::Result;
		//			my_tuple box;
		//			const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetItemLevelup");
		//			CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//			UE_LOG(LogBladeII, Log, TEXT("Complete set item levelup. current level(%d), exp(%.2f), EnhanceLevel(%d)"),
		//				Message->item->level, Message->item->exp, Message->item->enhance_level);
		//		},
		//			[](FB2MessageInfoPtr messageInfo)
		//		{
		//			b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//			// TODO error code
		//		}
		//		));

		//		auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetItemLevelup>(EmptyMessage, ItemUID, ItemLevel, ItemExp, ItemEnhanceLevel);
		//		Message->SendMessage();
		//	}
		//	
		//	return true;
		//}
		////////////////////////////////////////////////////////////////////////////
		//// for normal protocol
		//else if (FParse::Command(&Cmd, TEXT("DismantleItem")))		// DismantleItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("DismantleItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestDismantleItemsClass<const TArray<int64>&>::GetInstance().Signal(ItemUIDList);

		//	return true;
		//}
		////////////////////////////////////////////////////////////////////////////
		//// for normal protocol
		//else if (FParse::Command(&Cmd, TEXT("SellItem")))		// SellItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("SellItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestSellItemClass<const TArray<int64>&>::GetInstance().Signal(ItemUIDList);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SellConsumableItem")))		// SellConsumableItem ItemUID, SellCount
		//{
		//	FString ItemUIDString, SellCountString;
		//	FParse::Token(Cmd, ItemUIDString, true);
		//	FParse::Token(Cmd, SellCountString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);
		//	int32 SellCount = FCString::Atoi64(*SellCountString);

		//	UE_LOG(LogBladeII, Log, TEXT("SellItem : ItemUID(%I64d), SellCount(%d)"), ItemUID, SellCount);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SellConsumableItemMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSellConsumableItem> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	RequestSellConsumableItemClass<int64, int32>::GetInstance().Signal(ItemUID, SellCount);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SellItemAll")))		// SellItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	TArray<int64> ItemUIDList;
		//	auto& DataStore = BladeIIGameImpl::GetLocalCharacterData();

		//	for (auto& ItemContainer : DataStore.GetUserAllItems().CharacterItemContainers)
		//	{
		//		ItemContainer.StoredAccessries.GetKeys(ItemUIDList);
		//		ItemContainer.StoredProtections.GetKeys(ItemUIDList);
		//		ItemContainer.StoredWeapons.GetKeys(ItemUIDList);
		//	}

		//	RequestSellItemClass<const TArray<int64>&>::GetInstance().Signal(ItemUIDList);

		//	return true;
		//}
		//
		//else if (FParse::Command(&Cmd, TEXT("ConvertData")))		// ConvertItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int32 templateId = FCString::Atoi(*ItemUIDString);

		//	RequestConvertDataClass<int32>::GetInstance().Signal(templateId);
		//	return true;
		//}
		//
		//else if (FParse::Command(&Cmd, TEXT("EquipItem")))		// EquipItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("EquipItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestEquipItemClass<int32, const TArray<int64>&>::GetInstance().Signal(0, ItemUIDList);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnequipItem")))		// UnequipItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("UnequipItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestUnequipItemClass<int32, const TArray<int64>&>::GetInstance().Signal(0, ItemUIDList);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LockItem")))		// LockItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("LockItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestLockItemClass<const TArray<int64>&>::GetInstance().Signal(ItemUIDList);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnlockItem")))		// UnlockItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("UnlockItem : ItemUID(%I64d)"), ItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(ItemUID);

		//	RequestUnlockItemClass<const TArray<int64>&>::GetInstance().Signal(ItemUIDList);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ExtendInventory")))		// ExtendInventory B2CharacterType(1~4)
		//{
		//	FString CharacterTypeIDString, InventoryTypeIDString;
		//	FParse::Token(Cmd, CharacterTypeIDString, true);
		//	
		//	int32 CharacterType = FCString::Atoi(*CharacterTypeIDString);
		//	
		//	UE_LOG(LogBladeII, Log, TEXT("ExtendInventory : CharacterType(%d)"), CharacterType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ExtendInventoryMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseExtendInventory>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseExtendInventory::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseExtendInventory");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete extend inventory. current cash(%d), each inventories extend count : %d"),
		//			Message->cash, Message->character->inventory_item_buy_count);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestExtendInventory>(EmptyMessage, CharacterType, 1);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceItem")))		// EnhanceItem MainItemUID EnhanceScrollTID UseProtectionScroll ProtectionScrollTID
		//{
		//	FString MainItemUIDString, EnhanceScrollTIDString, UseProtectionScrollString, ProtectionScrollTIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, EnhanceScrollTIDString, true);
		//	FParse::Token(Cmd, UseProtectionScrollString, true);
		//	FParse::Token(Cmd, ProtectionScrollTIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int32 EnhanceScrollTID = FCString::Atoi(*EnhanceScrollTIDString);
		//	bool UseProtectionScroll = (FCString::Atoi(*UseProtectionScrollString) != 0) ? true : false;
		//	int32 ProtectionScrollTID = FCString::Atoi(*ProtectionScrollTIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("EnhanceItem : MainItemUID(%I64d), EnhanceScrollTID(%d), UseProtectionScroll(%d), ProtectionScrollTID(%d)"),
		//		MainItemUID, EnhanceScrollTID, UseProtectionScroll, ProtectionScrollTID);

		//	RequestEnhanceItemClass<int64, int32, bool, int32>::GetInstance().Signal(MainItemUID, EnhanceScrollTID, UseProtectionScroll, ProtectionScrollTID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ExchangeItemEnhanceLevel")))		// ExchangeItemEnhanceLevel ItemId1 ItemId2 PriceType
		//{
		//	FString ItemId1String, ItemId2String, PriceTypeString;
		//	FParse::Token(Cmd, ItemId1String, true);
		//	FParse::Token(Cmd, ItemId2String, true);
		//	FParse::Token(Cmd, PriceTypeString, true);

		//	int64 ItemId1 = FCString::Atoi64(*ItemId1String);
		//	int64 ItemId2 = FCString::Atoi64(*ItemId2String);
		//	int32 PriceType = FCString::Atoi(*PriceTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("ExchangeItemEnhanceLevel : ItemId1(%I64d), ItemId2(%I64d), PriceType(%d)"), ItemId1, ItemId2, PriceType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ExchangeItemEnhanceLevelMessage(
		//		[ItemId1, ItemId2, PriceType](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseExchangeItemEnhanceLevel>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseExchangeItemEnhanceLevel::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseExchangeItemEnhanceLevel");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete exchange item enhance level. dark crystal: %d, gem: %d, item 1(id: %I64d, enhance level: %d), item 2(id: %I64d, enhance level: %d)"),
		//			Message->current_dark_crystal, Message->current_gem, 
		//			Message->item_data_1->id, Message->item_data_1->enhance_level, 
		//			Message->item_data_2->id, Message->item_data_2->enhance_level
		//		);
		//	},
		//		[ItemId1, ItemId2, PriceType](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestExchangeItemEnhanceLevel>(EmptyMessage, ItemId1, ItemId2, PriceType);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("OpenSealSlotItem")))		// OpenSealSlotItem ItemId SealSlot OpenPriceType
		//{
		//	FString ItemIdString, SealSlotString;
		//	FParse::Token(Cmd, ItemIdString, true);
		//	FParse::Token(Cmd, SealSlotString, true);

		//	int64 ItemId = FCString::Atoi64(*ItemIdString);
		//	int32 SealSlot = FCString::Atoi(*SealSlotString);

		//	UE_LOG(LogBladeII, Log, TEXT("OpenSealSlotItem : ItemId(%I64d), SealSlot(%d)"), ItemId, SealSlot);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::OpenSealSlotItemMessage(
		//		[ItemId, SealSlot](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseOpenSealSlotItem>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseOpenSealSlotItem::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseOpenSealSlotItem");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete open seal slot item. gold: %d, gem: %d, item (id: %I64d, open seal slot count: %d)"),
		//			Message->current_gold, Message->current_gem,
		//			Message->slot_opened_item->id, Message->slot_opened_item->open_seal_slot_count
		//		);
		//	},
		//		[ItemId, SealSlot](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestOpenSealSlotItem>(EmptyMessage, ItemId, SealSlot);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("CarveSealItem")))		// CarveSealItem ItemId SealSlot PriceType
		//{
		//	FString ItemIdString, SealSlotString;
		//	FParse::Token(Cmd, ItemIdString, true);
		//	FParse::Token(Cmd, SealSlotString, true);

		//	int64 ItemId = FCString::Atoi64(*ItemIdString);
		//	int32 SealSlot = FCString::Atoi(*SealSlotString);

		//	UE_LOG(LogBladeII, Log, TEXT("CarveSealItem : ItemId(%I64d), SealSlot(%d)"), ItemId, SealSlot);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CarveSealItemMessage(
		//		[ItemId, SealSlot](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCarveSealItem>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseCarveSealItem::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseCarveSealItem");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete open seal slot item. gold: %d, gem: %d, item (id: %I64d)"),
		//			Message->current_gold, Message->current_gem,
		//			Message->sealed_item->id
		//		);
		//	},
		//		[ItemId, SealSlot](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestOpenSealSlotItem>(EmptyMessage, ItemId, SealSlot);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LevelupItem")))		// LevelupItem MainItemUID MaterialItemUID
		//{
		//	FString MainItemUIDString, MaterialItemUIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, MaterialItemUIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int64 MaterialItemUID = FCString::Atoi64(*MaterialItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("LevelupItem : MainItemUID(%I64d), MaterialItemUID(%I64d)"), MainItemUID, MaterialItemUID);

		//	TArray<int64> ItemUIDList;
		//	ItemUIDList.Emplace(MaterialItemUID);

		//	RequestLevelupItemClass<int64, const TArray<int64>&, bool>::GetInstance().Signal(MainItemUID, ItemUIDList, false);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SurpassItem")))		// SurpassItem MainItemUID MaterialItemUID
		//{
		//	FString MainItemUIDString, MaterialItemUIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, MaterialItemUIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int64 MaterialItemUID = FCString::Atoi64(*MaterialItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("SurpassItem : MainItemUID(%I64d), MaterialItemUID(%I64d)"), MainItemUID, MaterialItemUID);

		//	RequestSurpassItemClass<int64, int64>::GetInstance().Signal(MainItemUID, MaterialItemUID);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpgradeItem")))		// UpgradeItem ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("UpgradeItem : ItemUID(%I64d)"), ItemUID);

		//	RequestUpgradeItemClass<int64>::GetInstance().Signal(ItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ComposeItem")))		// ComposeItem MainItemUID SubItemUID PresetID
		//{
		//	FString MainItemUIDString, SubItemUIDString, PresetIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, SubItemUIDString, true);
		//	FParse::Token(Cmd, PresetIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int64 SubItemUID = FCString::Atoi64(*SubItemUIDString);
		//	int32 PresetID = FCString::Atoi(*PresetIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("ComposeItem : MainItemUID(%I64d), SubItemUID(%I64d), PresetID(%d)"), MainItemUID, SubItemUID, PresetID);

		//	RequestComposeItemClass<int64, int64, int32>::GetInstance().Signal(MainItemUID, SubItemUID, PresetID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ChangeItemCharacter")))		// ChangeItemCharacter MainItemUID CharacterType
		//{
		//	FString MainItemUIDString, CharacterTypeString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("ChangeItemCharacter : MainItemUID(%I64d), CharacterType(%d)"), MainItemUID, CharacterType);

		//	RequestChangeItemCharacterTypeClass<int64, int32>::GetInstance().Signal(MainItemUID, CharacterType);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetChangeItemCharacterCost")))		// GetChangeItemCharacterCost MainItemUID
		//{
		//	FString MainItemUIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetChangeItemCharacterCost : MainItemUID(%I64d)"), MainItemUID);

		//	RequestGetChangeItemCharacterTypeCostClass<int64>::GetInstance().Signal(MainItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetItemSurpassCost")))		// GetItemSurpassCost MainItemUID
		//{
		//	FString MainItemUIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetItemSurpassCost : MainItemUID(%I64d)"), MainItemUID);

		//	RequestGetItemSurpassCostClass<int64>::GetInstance().Signal(MainItemUID);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetItemUpgradeCost")))		// GetItemUpgradeCost ItemUID
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetItemUpgradeCost : ItemUID(%I64d)"), ItemUID);

		//	RequestGetItemUpgradeCostClass<int64>::GetInstance().Signal(ItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetItemComposeCost")))		// GetItemComposeCost MainItemUID PresetID
		//{
		//	FString MainItemUIDString, PresetIDString;
		//	FParse::Token(Cmd, MainItemUIDString, true);
		//	FParse::Token(Cmd, PresetIDString, true);

		//	int64 MainItemUID = FCString::Atoi64(*MainItemUIDString);
		//	int64 PresetID = FCString::Atoi(*PresetIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetItemComposeCost : MainItemUID(%I64d) PresetID(%d)"), MainItemUID, PresetID);

		//	RequestGetItemComposeCostClass<int64, int32>::GetInstance().Signal(MainItemUID, PresetID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetItemRandomOption")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetItemRandomOptionMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetItemRandomOption>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseGetItemRandomOption::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseGetItemRandomOption");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete get random item option. item id(%I64d), option1(%d, %f), option2(%d, %f), option3(%d, %f)"), 
		//			Message->item_id, 
		//			Message->random_option_id1, Message->random_option_value1, 
		//			Message->random_option_id2, Message->random_option_value2, 
		//			Message->random_option_id3, Message->random_option_value3);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetItemRandomOption>(EmptyMessage);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GenerateItemRandomOption")))
		//{
		//	FString ItemUIDString, BuyPriceTypeString;
		//	FParse::Token(Cmd, ItemUIDString, true);
		//	FParse::Token(Cmd, BuyPriceTypeString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);
		//	int32 BuyPriceType = FCString::Atoi(*BuyPriceTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("GenerateItemRandomOption : ItemUID(%I64d), BuyPriceType(%d)"), ItemUID, BuyPriceType);

		//	RequestGenerateItemRandomOptionClass<int64, int32>::GetInstance().Signal(ItemUID, BuyPriceType);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ApplyItemRandomOption")))
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("ApplyItemRandomOption : ItemUID(%I64d)"), ItemUID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ApplyItemRandomOptionMessage(
		//		[ItemUID](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseApplyItemRandomOption>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseApplyItemRandomOption::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseApplyItemRandomOption");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete apply random item option. item id(%I64d), option1(%d, %f), option2(%d, %f), option3(%d, %f)"),
		//			Message->item_data->id,
		//			Message->item_data->random_option_id1, Message->item_data->random_option_value1,
		//			Message->item_data->random_option_id2, Message->item_data->random_option_value2,
		//			Message->item_data->random_option_id3, Message->item_data->random_option_value3);
		//	},
		//		[ItemUID](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestApplyItemRandomOption>(EmptyMessage, ItemUID);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DiscardItemRandomOption")))
		//{
		//	// TODO DISCARDITEMRANDOMOPTION: DO SOMETHING YOU WANT
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	// TODO DISCARDITEMRANDOMOPTION: LOG AS YOU WANT
		//	UE_LOG(LogBladeII, Log, TEXT("DiscardItemRandomOption : ItemUID(%I64d)"), ItemUID);


		//	// TODO DISCARDITEMRANDOMOPTION: SIGNAL TO REQUEST INSTANCE
		//	RequestDiscardItemRandomOptionClass<int64>::GetInstance().Signal(ItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetItemRandomQuality")))
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetItemRandomQuality : ItemUID(%I64d)"), ItemUID);


		//	// TODO DISCARDITEMRANDOMOPTION: SIGNAL TO REQUEST INSTANCE
		//	RequestGetItemRandomQualityClass<int64>::GetInstance().Signal(ItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GenerateItemRandomQuality")))
		//{
		//	FString ItemUIDString, MaterialItemIDString, QualityString;
		//	FParse::Token(Cmd, ItemUIDString, true);
		//	FParse::Token(Cmd, MaterialItemIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);
		//	int64 MaterialItemID = FCString::Atoi64(*MaterialItemIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GenerateItemRandomOption : ItemUID(%I64d), Material ItemID(%I64d)"), ItemUID, MaterialItemID);

		//	RequestGenerateItemRandomQualityClass<int64, int64>::GetInstance().Signal(ItemUID, MaterialItemID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ApplyItemRandomQuality")))
		//{
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("ApplyItemRandomOption : ItemUID(%I64d)"), ItemUID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ApplyItemRandomQualityMessage(
		//		[ItemUID](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseApplyItemRandomQuality>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseApplyItemRandomQuality::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseApplyItemRandomQuality");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete apply random item Quality. item id(%I64d)"),
		//			Message->main_item->id);
		//	},
		//		[ItemUID](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestApplyItemRandomQuality>(EmptyMessage, ItemUID);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DiscardItemRandomQuality")))
		//{
		//	// TODO DISCARDITEMRANDOMOPTION: DO SOMETHING YOU WANT
		//	FString ItemUIDString;
		//	FParse::Token(Cmd, ItemUIDString, true);

		//	int64 ItemUID = FCString::Atoi64(*ItemUIDString);

		//	// TODO DISCARDITEMRANDOMOPTION: LOG AS YOU WANT
		//	UE_LOG(LogBladeII, Log, TEXT("DiscardItemRandomQuality : ItemUID(%I64d)"), ItemUID);


		//	// TODO DISCARDITEMRANDOMOPTION: SIGNAL TO REQUEST INSTANCE
		//	RequestDiscardItemRandomQualityClass<int64>::GetInstance().Signal(ItemUID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("IncDarkCrystal")))
		//{
		//	FString DarkCrystalString;
		//	FParse::Token(Cmd, DarkCrystalString, true);

		//	int32 DarkCrystal = FCString::Atoi(*DarkCrystalString);

		//	UE_LOG(LogBladeII, Log, TEXT("IncreaseDarkCrystalPaid : DarkCrystal(%d)"), DarkCrystal);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseDarkCrystalPaidMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseDarkCrystalPaid>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseIncreaseDarkCrystalPaid::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseIncreaseDarkCrystalPaid");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete increase dark crystal. current dark crystal(%d)"), Message->dark_crystal_paid);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseDarkCrystalPaid>(EmptyMessage, DarkCrystal);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ModifyItemRandomOption")))
		//{
		//	FString itemUIDString, slotNumString, optionIdString, optionValueString;
		//	FParse::Token(Cmd, itemUIDString, true);
		//	FParse::Token(Cmd, slotNumString, true);
		//	FParse::Token(Cmd, optionIdString, true);
		//	FParse::Token(Cmd, optionValueString, true);

		//	int64 itemUID = FCString::Atoi64(*itemUIDString);
		//	int32 slotNum = FCString::Atoi(*slotNumString);
		//	int32 optionId = FCString::Atoi(*optionIdString);
		//	float optionValue = FCString::Atoi(*optionValueString);

		//	UE_LOG(LogBladeII, Log, TEXT("IncreaseDarkCrystalPaid : itemUID(%I64d), slotNum(%d), optionId(%d), optionValue(%f)"), 
		//		itemUID, slotNum, optionId, optionValue);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ModifyItemRandomOptionMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseModifyItemRandomOption>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseModifyItemRandomOption::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseModifyItemRandomOption");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete increase dark crystal. item id(%I64d), option1(%d, %f), option2(%d,%f), option3(%d,%f)"), 
		//			Message->item_id, Message->random_option_id1, Message->random_option_value1, 
		//			Message->random_option_id2, Message->random_option_value2, 
		//			Message->random_option_id3, Message->random_option_value3);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestModifyItemRandomOption>(EmptyMessage, itemUID, slotNum, optionId, optionValue);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ModifyItemNewFlag")))
		//{
		//	FString characterTypeString, inventoryTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, inventoryTypeString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 inventoryType = FCString::Atoi(*inventoryTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("ModifyItemNewFlag : characterType(%d), inventoryType(%d)"),
		//		characterType, inventoryType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ModifyItemNewFlagMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseModifyItemNewFlag>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete ModifyItemNewFlag"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestModifyItemNewFlag>(EmptyMessage, characterType, inventoryType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ModifyCostumeNewFlag")))
		//{
		//	FString characterTypeString, inventoryTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, inventoryTypeString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 inventoryType = FCString::Atoi(*inventoryTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("ModifyCostumeNewFlag : characterType(%d), inventoryType(%d)"),
		//		characterType, inventoryType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ModifyCostumeNewFlagMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseModifyCostumeNewFlag>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete ModifyCostumeNewFlag"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestModifyCostumeNewFlag>(EmptyMessage, characterType, inventoryType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetContentsOpenStatus")))
		//{
		//	// TODO GetContentsOpenStatus: DO SOMETHING YOU WANT
		//	FString linkKeyString;
		//	FParse::Token(Cmd, linkKeyString, true);

		//	int32 linkKey = FCString::Atoi(*linkKeyString);

		//	// TODO GetContentsOpenStatus: LOG AS YOU WANT
		//	UE_LOG(LogBladeII, Log, TEXT("GetContentsOpenStatus : linkKey(%d)"), linkKey);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetContentsOpenStatusMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetContentsOpenStatus>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseGetContentsOpenStatus::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseGetContentsOpenStatus");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("GetContentsOpenStatus: total count (%d)"), Message->contents.Num());
		//		for (b2network::B2ContentsOpenStatusPtr status : Message->contents)
		//		{
		//			UE_LOG(LogBladeII, Log, TEXT("--- mode id (%d), opened (%s), stage id (%d), tutorial id (%d)"), 
		//				status->mode_id, status->opened ? TEXT("True") : TEXT("False"), status->stage_id, status->tutorial_id);
		//		}

		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetContentsOpenStatus>(EmptyMessage, linkKey);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetDuelRewardBoxInfo")))
		//{
		//	FString duelModeString;
		//	FParse::Token(Cmd, duelModeString, true);

		//	int32 duelMode = FCString::Atoi(*duelModeString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetDuelRewardBoxInfo : duelMode(%d)"), duelMode);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetDuelRewardBoxInfoMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetDuelRewardBoxInfo>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetDuelRewardBoxInfo>(EmptyMessage, duelMode);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetDuelRewardBoxGainableList")))
		//{
		//	FString duelModeString, gradeString;
		//	FParse::Token(Cmd, duelModeString, true);
		//	FParse::Token(Cmd, gradeString, true);

		//	int32 duelMode = FCString::Atoi(*duelModeString);
		//	int32 grade = FCString::Atoi(*gradeString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetDuelRewardBoxGainableList : duelMode(%d), grade(%d)"), duelMode, grade);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetDuelRewardBoxGainableListMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetDuelRewardBoxGainableList>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetDuelRewardBoxGainableList>(EmptyMessage, duelMode, grade);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("OpenDuelRewardBox")))
		//{
		//	FString duelModeString;
		//	FParse::Token(Cmd, duelModeString, true);

		//	int32 duelMode = FCString::Atoi(*duelModeString);

		//	UE_LOG(LogBladeII, Log, TEXT("OpenDuelRewardBox : duelMode(%d)"), duelMode);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::OpenDuelRewardBoxMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseOpenDuelRewardBox>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestOpenDuelRewardBox>(EmptyMessage, duelMode);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EventAttendance")))
		//{
		//	FString eventIdString;
		//	FParse::Token(Cmd, eventIdString, true);

		//	int32 eventId = FCString::Atoi(*eventIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("EventAttendance : eventId(%d)"), eventId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EventAttendanceMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEventAttendance>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEventAttendance>(EmptyMessage, eventId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("BeginnerAttendance")))
		//{

		//	UE_LOG(LogBladeII, Log, TEXT("BeginnerAttendance"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BeginnerAttendanceMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBeginnerAttendance>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBeginnerAttendance>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpdateDailyPlayTime")))
		//{
		//	FString playTimeInSecString;
		//	FParse::Token(Cmd, playTimeInSecString, true);

		//	int32 playTimeInSec = FCString::Atoi(*playTimeInSecString);

		//	UE_LOG(LogBladeII, Log, TEXT("UpdateDailyPlayTime : playTimeInSec(%d)"), playTimeInSec);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UpdateDailyPlayTimeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpdateDailyPlayTime>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpdateDailyPlayTime>(EmptyMessage, playTimeInSec);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ReceiveDailyPlayTimeReward")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("ReceiveDailyPlayTimeReward"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceiveDailyPlayTimeRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceiveDailyPlayTimeReward>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceiveDailyPlayTimeReward>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("GetTutorialProgress")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("GetTutorialProgress :"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetTutorialProgressMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetTutorialProgress>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseGetTutorialProgress::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseGetTutorialProgress");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Current tutorial progress. totorial id (%d), prepared(%s), lottery drawn(%s), finished(%s)"),
		//			Message->tutorial_id, 
		//			Message->is_item_prepared ? TEXT("TRUE") : TEXT("FALSE"), 
		//			Message->is_lottery_drawn ? TEXT("TRUE") : TEXT("FALSE"), 
		//			Message->is_finished ? TEXT("TRUE") : TEXT("FALSE"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetTutorialProgress>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetTutorialProgress")))
		//{
		//	FString tutorialIdString, isPreparedString, isDrawnString, isFinishedString;
		//	FParse::Token(Cmd, tutorialIdString, true);
		//	FParse::Token(Cmd, isPreparedString, true);
		//	FParse::Token(Cmd, isDrawnString, true);
		//	FParse::Token(Cmd, isFinishedString, true);

		//	int32 tutorialId = FCString::Atoi64(*tutorialIdString);
		//	bool prepared = (FCString::Atoi(*isPreparedString) != 0) ? true : false;
		//	bool drawn = (FCString::Atoi(*isDrawnString) != 0) ? true : false;
		//	bool finished = (FCString::Atoi(*isFinishedString) != 0) ? true : false;

		//	UE_LOG(LogBladeII, Log, TEXT("SetTutorialProgress : tutorial id(%d), is prepared(%s), is lottery drwan(%s), is finished(%s)"),
		//		tutorialId, 
		//		prepared ? TEXT("TRUE") : TEXT("FALSE"), 
		//		drawn ? TEXT("TRUE") : TEXT("FALSE"), 
		//		finished ? TEXT("TRUE") : TEXT("FALSE"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetTutorialProgressMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetTutorialProgress>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetTutorialProgress::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetTutorialProgress");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Tutorial was set to tutorial id(%d), is prepared(%s), is lottery drwan(%s), is finished(%s)"),
		//			Message->tutorial_id, 
		//			Message->is_item_prepared ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_lottery_drawn ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_finished ? TEXT("TRUE") : TEXT("FALSE")
		//			);

		//		DeliverySetTutorialProgressClass<FB2ResponseSetTutorialProgressPtr>::GetInstance().Signal(Message);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();

		//		UE_LOG(LogBladeII, Log, TEXT("*** ERROR: Updated tutorial progress. errorCode %d"), (int)messageInfo->GetErrorCode());
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetTutorialProgress>(EmptyMessage, tutorialId, prepared, drawn, finished);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpdateTutorialProgress")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("UpdateTutorialProgress :"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UpdateTutorialProgressMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpdateTutorialProgress>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseUpdateTutorialProgress::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseUpdateTutorialProgress");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Updated tutorial progress. totorial id (%d), prepared(%s), lottery drawn(%s), finished(%s)"),
		//			Message->tutorial_id,
		//			Message->is_item_prepared ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_lottery_drawn ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_finished ? TEXT("TRUE") : TEXT("FALSE"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//		UE_LOG(LogBladeII, Log, TEXT("*** ERROR: Updated tutorial progress. errorCode %d"), (int)messageInfo->GetErrorCode());
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpdateTutorialProgress>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SkipTutorialProgress")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("SkipTutorialProgress :"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetTutorialProgressMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSkipTutorialProgress>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSkipTutorialProgress::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSkipTutorialProgress");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Skipped tutorial progress. totorial id (%d), prepared(%s), lottery drawn(%s), finished(%s)"),
		//			Message->tutorial_id,
		//			Message->is_item_prepared ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_lottery_drawn ? TEXT("TRUE") : TEXT("FALSE"),
		//			Message->is_finished ? TEXT("TRUE") : TEXT("FALSE"));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSkipTutorialProgress>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DrawTutorialItem")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	
		//	UE_LOG(LogBladeII, Log, TEXT("DrawTutorialItem"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DrawTutorialItemMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDrawTutorialItem>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDrawTutorialItem>(EmptyMessage, characterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PrepareTutorial")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("PrepareTutorial"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PrepareTutorialMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePrepareTutorial>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPrepareTutorial>(EmptyMessage, characterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GiveTutorialReward")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("GiveTutorialReward"));

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GiveTutorialRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGiveTutorialReward>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGiveTutorialReward>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("CheckReachPower")))
		//{
		//	FString gladiatorPowerString, assassinPowerString, wizardPowerString, fighterPowerString;
		//	FParse::Token(Cmd, gladiatorPowerString, true);
		//	FParse::Token(Cmd, assassinPowerString, true);
		//	FParse::Token(Cmd, wizardPowerString, true);
		//	FParse::Token(Cmd, fighterPowerString, true);

		//	int32 powers[4];
		//	powers[0] = FCString::Atoi64(*gladiatorPowerString);
		//	powers[1] = FCString::Atoi64(*assassinPowerString);
		//	powers[2] = FCString::Atoi64(*wizardPowerString);
		//	powers[3] = FCString::Atoi64(*fighterPowerString);

		//	UE_LOG(LogBladeII, Log, TEXT("CheckReachPower : gladiator(%d), assassin(%d), wizard(%d), fighter(%d)"), powers[0], powers[1], powers[2], powers[3]);

		//	TArray<b2network::B2CharacterPowerPtr> characterList;
		//	for (int i = 1; i <= 4; i ++)
		//	{
		//		// 0 ?? ?�라미터�?? 받을 경우 request packet ?? ?��? ?�는??.
		//		// ?�제 ?�라?�언?�에?? ?�용 ?? ?�에?? 변�?? ?? 캐릭?�만 ?�는??.
		//		if (powers[i - 1] == 0)
		//			continue;

		//		b2network::B2CharacterPowerPtr character_power = std::make_shared<b2network::B2CharacterPower>();
		//		character_power->character_type = i;
		//		character_power->total_power = powers[i - 1];
		//		characterList.Emplace(character_power);
		//	}

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CheckReachPowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCheckReachPower>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCheckReachPower>(EmptyMessage, characterList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("TrackingPowerDetail")))
		//{
		//	FString characterTypeString, itemPresetString, skillSlotString, valueString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, itemPresetString, true);
		//	FParse::Token(Cmd, skillSlotString, true);
		//	FParse::Token(Cmd, valueString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 itemPresetId = FCString::Atoi(*itemPresetString);
		//	int32 skillSlot = FCString::Atoi(*skillSlotString);
		//	int32 powerUnitValue = FCString::Atoi(*valueString);

		//	UE_LOG(LogBladeII, Log, TEXT("TrackingPowerDetail : characterType(%d), itemPresetId(%d), skillSlot(%d), powerUnitValue(%d)"), characterType, itemPresetId, skillSlot, powerUnitValue);

		//	int32 totalPower = 0;
		//	TArray<b2network::B2CharacterPowerSpecificPtr> powerSpecifics;
		//	for (int category = b2network::B2PowerCategory::STAT_POWER; category <= b2network::B2PowerCategory::FAIRY_POWER; category++)
		//	{
		//		int powerValue = powerUnitValue + category;
		//		b2network::B2CharacterPowerSpecificPtr specific_power = std::make_shared<b2network::B2CharacterPowerSpecific>();
		//		specific_power->category = category;
		//		specific_power->power = powerValue;
		//		powerSpecifics.Emplace(specific_power);

		//		totalPower += powerValue;
		//	}

		//	b2network::B2CharacterPowerSpecificPtr totoal_power = std::make_shared<b2network::B2CharacterPowerSpecific>();
		//	totoal_power->category = b2network::B2PowerCategory::TOTAL_POWER;
		//	totoal_power->power = totalPower;
		//	powerSpecifics.Emplace(totoal_power);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::TrackingPowerDetailMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseTrackingPowerDetail>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestTrackingPowerDetail>(EmptyMessage, characterType, itemPresetId, skillSlot, powerSpecifics);
		//	Message->SendMessage();
		//	return true;
		//}
		//// REPLACEMENT POINT: SPECIAL ONLINE_WHOLESALER CPP REQUEST
		//// NEVER DELETE LINE ABOVE

		return false;
	}

	bool ExecHandler::ExecuteShellCommandMatch(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("MakePvPRanking")))		// MakePvPRanking ranking
		//{
		//	FString RankingString;
		//	FParse::Token(Cmd, RankingString, true);

		//	int32 Ranking = FCString::Atoi(*RankingString);

		//	UE_LOG(LogBladeII, Log, TEXT("MakePvPRanking : Ranking(%d)"), Ranking);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::MakeRankingMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseMakeRanking>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete make pvp ranking."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestMakeRanking>(EmptyMessage, b2network::B2MatchInfoType::BMIT_PLAYER, Ranking);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("MakeTeamRanking")))		// MakeTeamRanking ranking
		//{
		//	FString RankingString;
		//	FParse::Token(Cmd, RankingString, true);

		//	int32 Ranking = FCString::Atoi(*RankingString);

		//	UE_LOG(LogBladeII, Log, TEXT("MakeTeamRanking : Ranking(%d)"), Ranking);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::MakeRankingMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseMakeRanking>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete make team ranking."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestMakeRanking>(EmptyMessage, b2network::B2MatchInfoType::BMIT_TEAM, Ranking);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("JoinPvp")))
		//{
		//	FString MainCharacterString, TagCharacterString, useAdditionalCountString, RoomSeedString;

		//	FParse::Token(Cmd, MainCharacterString, true);
		//	FParse::Token(Cmd, TagCharacterString, true);
		//	FParse::Token(Cmd, useAdditionalCountString, true);
		//	FParse::Token(Cmd, RoomSeedString, true);

		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterString);
		//	bool useAdditionalCount = (FCString::Atoi(*useAdditionalCountString) != 0) ? true : false;

		//	RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance()
		//		.Signal(b2network::B2MatchMakingType::BMMT_PVP_RANK,
		//		MainCharacterType, TagCharacterType, useAdditionalCount, RoomSeedString);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("JoinRaid")))
		//{
		//	FString MainCharacterString, TagCharacterString, RoomSeedString;

		//	FParse::Token(Cmd, MainCharacterString, true);
		//	FParse::Token(Cmd, TagCharacterString, true);
		//	FParse::Token(Cmd, RoomSeedString, true);

		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterString);

		//	RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance()
		//		.Signal(b2network::B2MatchMakingType::BMMT_RAID_RANK,
		//		MainCharacterType, TagCharacterType, false, RoomSeedString);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("JoinAssault")))
		//{
		//	FString MainCharacterString, TagCharacterString, RoomSeedString;

		//	FParse::Token(Cmd, MainCharacterString, true);
		//	FParse::Token(Cmd, TagCharacterString, true);
		//	FParse::Token(Cmd, RoomSeedString, true);

		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterString);

		//	RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance()
		//		.Signal(b2network::B2MatchMakingType::BMMT_ASSAULT_RANK,
		//		MainCharacterType, TagCharacterType, false, RoomSeedString);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("JoinRoom")))
		//{
		//	FString MainCharacterString, TagCharacterString;
		//	FParse::Token(Cmd, MainCharacterString, true);
		//	FParse::Token(Cmd, TagCharacterString, true);

		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterString);

		//	RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance()
		//		.Signal(b2network::B2MatchMakingType::BMMT_PVP_RANK,
		//		MainCharacterType, TagCharacterType, false, "");

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LeaveRoom")))
		//{
		//	// TODO fix
		//	RequestLeaveRoomClass<const FString&>::GetInstance().Signal("");
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RoomKick")))
		//{
		//	FString RoomIdString, AccountIdString;
		//	FParse::Token(Cmd, RoomIdString, true);
		//	FParse::Token(Cmd, AccountIdString, true);

		//	int64 kickAccountId = FCString::Atoi64(*AccountIdString); 

		//	RequestRoomKickClass<const FString&, int64>::GetInstance().Signal(RoomIdString, kickAccountId);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RoomMessage")))
		//{
		//	FString RoomIDString, ReceiverIdString, MessageString;
		//	FParse::Token(Cmd, RoomIDString, true);
		//	FParse::Token(Cmd, ReceiverIdString, true);
		//	FParse::Token(Cmd, MessageString, true);

		//	int32 ReceiverId = FCString::Atoi(*ReceiverIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("Sending RoomMessage : RoomID(%s) Message(size=%d)"),
		//		*RoomIDString, MessageString.Len());

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RoomMessageCommand(nullptr, nullptr));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRoomMessageCommand>(EmptyMessage, RoomIDString, ReceiverId, MessageString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetMatchStatus")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("GetMatchStatus."));

		//	RequestGetMatchStatusClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetTeamBattleStatus")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("GetTeamBattleStatus."));

		//	RequestGetTeamBattleStatusClass<>::GetInstance().Signal();
		//	return true;

		//}
		//else if (FParse::Command(&Cmd, TEXT("UpdateTeamBattleEntry")))			// UpdateTeamBattleEntry formationType characterType1 characterType2 characterType3
		//{
		//	FString formationTypeString, characterTypeString[3];

		//	FParse::Token(Cmd, formationTypeString, true);
		//	FParse::Token(Cmd, characterTypeString[0], true);
		//	FParse::Token(Cmd, characterTypeString[1], true);
		//	FParse::Token(Cmd, characterTypeString[2], true);

		//	int32 formationType = FCString::Atoi(*formationTypeString);
		//	int32 characterType[3] = {
		//		FCString::Atoi(*characterTypeString[0]),
		//		FCString::Atoi(*characterTypeString[1]),
		//		FCString::Atoi(*characterTypeString[2]),
		//	};

		//	UE_LOG(LogBladeII, Log, TEXT("GetTeamBattleStatus. FormationType(%d), CharacterSlot(%d, %d, %d)"),
		//		formationType, characterType[0], characterType[1], characterType[2]);

		//	b2network::B2TeamBattleEntryPtr entryPtr = std::make_shared<b2network::B2TeamBattleEntry>();
		//	entryPtr->formation_type = formationType;
		//	entryPtr->character_slot_1 = characterType[0];
		//	entryPtr->character_slot_2 = characterType[1];
		//	entryPtr->character_slot_3 = characterType[2];

		//	RequestUpdateTeamBattleEntryClass<b2network::B2TeamBattleEntryPtr>::GetInstance().Signal(entryPtr);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceTeamBattleFormation")))			// EnhanceTeamBattleFormation formationType
		//{
		//	FString formationTypeString;

		//	FParse::Token(Cmd, formationTypeString, true);

		//	int32 formationType = FCString::Atoi(*formationTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("EnhanceTeamBattleFormation. FormationType(%d)"), formationType);

		//	RequestEnhanceTeamBattleFormationClass<int32>::GetInstance().Signal(formationType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("TeamMatchmaking")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("TeamMatchmaking."));

		//	RequestTeamMatchmakingClass<bool>::GetInstance().Signal(false);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("TeamMatchResult")))		// TeamMatchResult token result(0:win, 1:lose, 2:draw)
		//{
		//	FString matchTokenString, matchResultString;

		//	FParse::Token(Cmd, matchResultString, true);

		//	int32 matchResult = FCString::Atoi64(*matchResultString);

		//	UE_LOG(LogBladeII, Log, TEXT("TeamMatchResult. token(%s), result(%d)"), *matchTokenString, matchResult);

		//	RequestTeamMatchResultClass<const FString&, int32, bool>::GetInstance().Signal(matchTokenString, matchResult, false);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RaidResult")))	
		//{
		//	FString tokenString, rankingString, isBonusString, isRewardString;

		//	FParse::Token(Cmd, tokenString, true);
		//	FParse::Token(Cmd, rankingString, true);
		//	FParse::Token(Cmd, isBonusString, true);
		//	FParse::Token(Cmd, isRewardString, true);

		//	int32 ranking = FCString::Atoi(*rankingString);
		//	bool isBonus = (FCString::Atoi(*isBonusString) != 0) ? true : false;
		//	bool isReward = (FCString::Atoi(*isRewardString) != 0) ? true : false;

		//	UE_LOG(LogBladeII, Log, TEXT("RaidResult. token(%s), ranking(%d), contributionMin(%d), contributionMax(%d)"), *tokenString, ranking, isBonus);

		//	RequestRaidResultClass<const FString&, int32, bool, bool, bool>::GetInstance().Signal(tokenString, ranking, isBonus, isReward, false);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetRaid")))
		//{
		//	RequestGetRaidClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RaidResurrection")))
		//{
		//	FString tokenString, buffIndexString;

		//	FParse::Token(Cmd, tokenString, true);
		//	FParse::Token(Cmd, buffIndexString, true);			

		//	int32 buffIndex = FCString::Atoi(*buffIndexString);			

		//	UE_LOG(LogBladeII, Log, TEXT("RaidResurrection. token(%s), buffIndex(%d)"), *tokenString, buffIndex);

		//	RequestRaidResurrectionClass<const FString&, int32>::GetInstance().Signal(tokenString, buffIndex);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ResetTeamMatchPoint")))
		//{
		//	RequestResetTeamMatchPointClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AssaultMatchmaking")))
		//{
		//	FString characterTypeString;

		//	FParse::Token(Cmd, characterTypeString, true);
		//	
		//	int32 characterType = FCString::Atoi(*characterTypeString);			

		//	RequestAssaultMatchmakingClass<int32>::GetInstance().Signal(characterType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AssaultResult")))
		//{
		//	FString tokenString, matchResultString, mvpPlayerString, ghostUserString;

		//	FParse::Token(Cmd, tokenString, true);
		//	FParse::Token(Cmd, matchResultString, true);
		//	FParse::Token(Cmd, mvpPlayerString, true);
		//	FParse::Token(Cmd, ghostUserString, true);
		//	
		//	int32 matchResult = FCString::Atoi(*matchResultString);			
		//	bool isMvpPlayer = (FCString::Atoi(*mvpPlayerString) != 0) ? true : false;
		//	bool isGhostUser = (FCString::Atoi(*ghostUserString) != 0) ? true : false;
		//	
		//	auto battleInfo = std::make_shared<b2network::B2AssaultBattleInfo>();
		//	battleInfo->kill_count = 30;
		//	battleInfo->death_count = 10;
		//	battleInfo->damage = 1000;

		//	RequestAssaultResultClass<const FString&, int32, bool, bool, b2network::B2AssaultBattleInfoPtr>::GetInstance().Signal(tokenString, matchResult, isMvpPlayer, isGhostUser, battleInfo);
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetAssaultBattleStatus")))
		//{
		//	RequestGetAssaultBattleStatusClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ReceiveAssaultMvpReward")))
		//{
		//	RequestReceiveAssaultMvpRewardClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RoomLeaveResult")))
		//{
		//	FString tokenString, accountIdString, matchTokenString;

		//	FParse::Token(Cmd, tokenString, true);
		//	FParse::Token(Cmd, accountIdString, true);
		//	FParse::Token(Cmd, matchTokenString, true);
		//	
		//	int64 accountId = FCString::Atoi64(*accountIdString);			
		//	RequestRoomLeaveResultClass<const FString&, int64, const FString&>::GetInstance().Signal(tokenString, accountId, matchTokenString);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetDuelModeInfo")))
		//{
		//	RequestGetDuelModeInfoClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetChallengeModeInfo")))
		//{
		//	RequestGetChallengeModeInfoClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetHallOfFame")))
		//{
		//	FString modeString, needUserInfoString;

		//	FParse::Token(Cmd, modeString, true);
		//	FParse::Token(Cmd, needUserInfoString, true);

		//	int32 mode = FCString::Atoi(*modeString);
		//	bool needUserInfo = FCString::ToBool(*needUserInfoString);

		//	RequestGetHallOfFameClass<int32, bool>::GetInstance().Signal(mode, needUserInfo);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ClientStateReport")))
		//{
		//	FString categoryName = "TEST";
		//	FString eventName = "Disconnect";
		//	FString lableName = "UserPause";
		//	FString detailInfo = "{ \"mode\": PVP, \"os\": \"android\", \"network\": \"wifi\" }";

		//	RequestClientStateReportClass<const FString&, const FString&, const FString&, const FString&>::GetInstance().Signal(categoryName, eventName, lableName, detailInfo);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PraiseTargetRanker")))
		//{
		//	FString modeString, accountIdString, rankingString;

		//	FParse::Token(Cmd, modeString, true);
		//	FParse::Token(Cmd, accountIdString, true);
		//	FParse::Token(Cmd, rankingString, true);

		//	int32 mode = FCString::Atoi(*modeString);
		//	int64 accountId = FCString::Atoi64(*accountIdString);
		//	int32 ranking = FCString::Atoi(*rankingString);

		//	RequestPraiseTargetRankerClass<int32, int64, int32>::GetInstance().Signal(mode, accountId, ranking);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetMatchRankingReward")))		// SetMatchRankingReward match_type ranking point
		//{
		//	FString MatchTypeString, RankingString, PointString;
		//	FParse::Token(Cmd, MatchTypeString, true);
		//	FParse::Token(Cmd, RankingString, true);
		//	FParse::Token(Cmd, PointString, true);

		//	int32 MatchType = FCString::Atoi(*MatchTypeString);
		//	int32 Ranking = FCString::Atoi(*RankingString);
		//	int32 Point = FCString::Atoi(*PointString);

		//	UE_LOG(LogBladeII, Log, TEXT("SetMatchRankingReward : MatchType(%d), Ranking(%d), Point(%d)"), MatchType, Ranking, Point);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetMatchSeasonRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetMatchSeasonReward>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete SetMatchRankingReward."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetMatchSeasonReward>(EmptyMessage, MatchType, Ranking, Point);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SkipMatchTutorial")))
		//{
		//	FString MatchTypeString;
		//	FParse::Token(Cmd, MatchTypeString, true);
		//	int32 MatchType = FCString::Atoi(*MatchTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("SkilMatchTutorial : MatchType(%d)"), MatchType);
		//	RequestSkipMatchTutorialClass<int32>::GetInstance().Signal(MatchType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetContentsModeState")))		// GetContentsModeState mode
		//{
		//	FString ModeString;
		//	FParse::Token(Cmd, ModeString, true);

		//	int32 Mode = FCString::Atoi(*ModeString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetContentsModeState : Mode(%d)"), Mode);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetContentsModeStateMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetContentsModeState>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete GetContentsModeState."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetContentsModeState>(EmptyMessage, Mode);
		//	Message->SendMessage();
		//	return true;
		//}
		return false;
	}

	bool ExecHandler::ExecuteShellCommandStage(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("OpenStage")))		// OpenStage actId stageId difficulty
		//{
		//	FString DifficultyString, ActIDString, StageIDString;

		//	FParse::Token(Cmd, ActIDString, true);
		//	FParse::Token(Cmd, StageIDString, true);
		//	FParse::Token(Cmd, DifficultyString, true);

		//	int32 ChapterNum = FCString::Atoi(*ActIDString);
		//	int32 SubChapterNum = FCString::Atoi(*StageIDString);
		//	int32 Difficulty = FCString::Atoi(*DifficultyString);

		//	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
		//	int32 ClientStageId = StageDataStore.GetClientStageIdOfChapterNums(ChapterNum, SubChapterNum);
		//	FServerStageID ServerStageID = StageDataStore.GetServerStageID(ClientStageId, SvrToCliStageDifficulty(Difficulty));
		//	UE_LOG(LogBladeII, Log, TEXT("OpenStage : ActID(%d), ClientSubStageID(%d), Difficulty(%d) => ServerStageID(%d)"), ChapterNum, SubChapterNum, Difficulty, ServerStageID.Id);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ForceClearStageMessage(
		//		[ServerStageID](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseForceClearStage>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete open stage."));
		//		//StageDataStore.SetDirtyActData(ServerStageID);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestForceClearStage>(EmptyMessage, ServerStageID.Id);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("StartStage")))		// StartStage StageID(1~) B2CharacterType(1~4) B2CharacterType(1~4) B2StageBuffOptionFlags(1,2,4,8 bit composite) TutorialId(1600)
		//{
		//	FString StageIDString, MainCharacterTypeIDString, TagCharacterTypeIDString, OptionFlagString, TutorialIdString, MainPresetIdString, TagPresetIdString;
		//	FParse::Token(Cmd, StageIDString, true);
		//	FParse::Token(Cmd, MainCharacterTypeIDString, true);
		//	FParse::Token(Cmd, TagCharacterTypeIDString, true);
		//	FParse::Token(Cmd, OptionFlagString, true);
		//	FParse::Token(Cmd, TutorialIdString, true);
		//	FParse::Token(Cmd, MainPresetIdString, true);
		//	FParse::Token(Cmd, TagPresetIdString, true);

		//	int32 StageID = FCString::Atoi(*StageIDString);
		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterTypeIDString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterTypeIDString);
		//	int32 OptionFlag = FCString::Atoi(*OptionFlagString);
		//	int32 TutorialId = TutorialIdString == "" ? 0 : FCString::Atoi(*TutorialIdString);
		//	int32 MainPresetId = FCString::Atoi(*MainPresetIdString);
		//	int32 TagPresetId = FCString::Atoi(*TagPresetIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("StartStage : StageID(%d), MainCharacter(%d), TagCharacter(%d), Option(%d), TutorialId(%d), MainPresetId(%d), TagPresetId(%d)"),
		//		StageID, MainCharacterType, TagCharacterType, OptionFlag, TutorialId, MainPresetId, TagPresetId);

		//	TArray<EStageBuffType> StageBuffs;
		//	SvrToCliStageBuffTypeComposit(OptionFlag, StageBuffs);
		//	AB2StageManager::GetCacheStageKeepEssentialData().RequestStartStage(StageID, EStageDifficulty::ESD_Normal, SvrToCliPCClassType(MainCharacterType), SvrToCliPCClassType(TagCharacterType), StageBuffs, FRepeatBattleStateSet(), false, TutorialId, MainPresetId, TagPresetId);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ClearStage")))		// ClearStage StageID(1~) Token B2CharacterType(1~4) B2CharacterType(1~4) ClearTimeSecond IsAllSurvival QteStartCount QteFinishCount
		//{
		//	FString StageIDString, TokenString, MainCharacterTypeIDString, TagCharacterTypeIDString, ClearTimeString, IsAllSurvival, QteStartCountString, QteFinishCountString;
		//	FParse::Token(Cmd, StageIDString, true);
		//	FParse::Token(Cmd, TokenString, true);
		//	FParse::Token(Cmd, MainCharacterTypeIDString, true);
		//	FParse::Token(Cmd, TagCharacterTypeIDString, true);
		//	FParse::Token(Cmd, ClearTimeString, true);
		//	FParse::Token(Cmd, IsAllSurvival, true);
		//	FParse::Token(Cmd, QteStartCountString, true);
		//	FParse::Token(Cmd, QteFinishCountString, true);

		//	int32 StageID = FCString::Atoi(*StageIDString);
		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterTypeIDString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterTypeIDString);
		//	int32 ClearTimeSecond = FCString::Atoi(*ClearTimeString);
		//	bool bIsAllSurvival = (FCString::Atoi(*IsAllSurvival) != 0) ? true : false;
		//	int32 monsterKillCount = 0;
		//	int32 qteStartCount = FCString::Atoi(*QteStartCountString);;
		//	int32 qteFinishCount = FCString::Atoi(*QteFinishCountString);;

		//	UE_LOG(LogBladeII, Log, TEXT("ClearStage : StageID(%d), Token(%s), MainCharacter(%d), TagCharacter(%d), ClearTime(%d), bIsAllSurvival(%d), qteStartCount(%d), qteFinishCount(%d)"),
		//		StageID, *TokenString, MainCharacterType, TagCharacterType, ClearTimeSecond, bIsAllSurvival, qteStartCount, qteFinishCount);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ClearStageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseClearStage>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseClearStage::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseClearStage");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		int32 clearGrade = Message->clear_grade;
		//		int32 clearMask = Message->clear_mask;
		//		UE_LOG(LogBladeII, Log, TEXT("Complete clear stage. clearGrade(%d), clearMask(0x04%x)"), clearGrade, clearMask);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	}
		//	));

		//	TArray<b2network::B2KillMonsterInfoPtr> killMonsterInfos;
		//	auto b2SpotSaleOptionPtr = std::make_shared<b2network::B2ItemSpotSaleOption>();
		//	b2SpotSaleOptionPtr->item_grade = 0;

		//	auto b2AetherSpotSaleOptionPtr = std::make_shared<b2network::B2AetherSpotSaleOption>();
		//	b2AetherSpotSaleOptionPtr->aether_grade = 0;
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestClearStage>(EmptyMessage, StageID, TokenString, 
		//		MainCharacterType, TagCharacterType, ClearTimeSecond, killMonsterInfos, monsterKillCount, bIsAllSurvival, qteStartCount, qteFinishCount, b2SpotSaleOptionPtr, b2AetherSpotSaleOptionPtr);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FailStage")))		// FailStage StageID(1~) Token B2StageFailReason(1~2) play_time_second(1~)
		//{
		//	FString StageIDString, TokenString, FailReasonString, PlayTimeSecondString;
		//	FParse::Token(Cmd, StageIDString, true);
		//	FParse::Token(Cmd, TokenString, true);
		//	FParse::Token(Cmd, FailReasonString, true);
		//	FParse::Token(Cmd, PlayTimeSecondString, true);

		//	int32 StageID = FCString::Atoi(*StageIDString);
		//	int32 FailReason = FCString::Atoi(*FailReasonString);
		//	int32 PlayTimeSecond = FCString::Atoi(*PlayTimeSecondString);

		//	UE_LOG(LogBladeII, Log, TEXT("StartStage : StageID(%d), Token(%s), FailReason(%d), PlayTimeSecond(%d)"),
		//		StageID, *TokenString, FailReason, PlayTimeSecond);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FailStageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFailStage>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFailStage>(EmptyMessage, StageID, TokenString,
		//		FailReason, PlayTimeSecond);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SweepStage")))		// SweepStage StageID(1~) B2CharacterType(1~4) B2CharacterType(1~4)
		//{
		//	FString StageIDString, MainCharacterTypeIDString, TagCharacterTypeIDString, MainPresetIdString, TagPresetIdString, RepeatCountString;
		//	FParse::Token(Cmd, StageIDString, true);
		//	FParse::Token(Cmd, MainCharacterTypeIDString, true);
		//	FParse::Token(Cmd, TagCharacterTypeIDString, true);
		//	FParse::Token(Cmd, MainPresetIdString, true);
		//	FParse::Token(Cmd, TagPresetIdString, true);
		//	FParse::Token(Cmd, RepeatCountString, true);

		//	int32 StageID = FCString::Atoi(*StageIDString);
		//	int32 MainCharacterType = FCString::Atoi(*MainCharacterTypeIDString);
		//	int32 TagCharacterType = FCString::Atoi(*TagCharacterTypeIDString);
		//	int32 MainPresetId = FCString::Atoi(*MainPresetIdString);
		//	int32 TagPresetId = FCString::Atoi(*TagPresetIdString);
		//	int32 RepeatCount = FCString::Atoi(*RepeatCountString);

		//	UE_LOG(LogBladeII, Log, TEXT("SweepStage : StageID(%d), MainCharacter(%d), TagCharacter(%d), MainPresetId(%d), TagPresetId(%d), RepeatCount(%d)"),
		//		StageID, MainCharacterType, TagCharacterType, MainPresetId, TagPresetId, RepeatCount);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SweepStageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSweepStage>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSweepStage>(EmptyMessage, StageID, MainCharacterType, TagCharacterType, MainPresetId, TagPresetId, RepeatCount);
		//	Message->SendMessage();
		//	return true;

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("Resurrection")))		// Resurrection Token
		//{
		//	FString TokenString;
		//	FParse::Token(Cmd, TokenString, true);

		//	UE_LOG(LogBladeII, Log, TEXT("Resurrection : Token(%s)"), *TokenString);

		//	RequestResurrectionClass<const FString&, int32>::GetInstance().Signal(TokenString, 1);

		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("GetActInfo")))		// GetActInfo ActID
		//{
		//	FString DifficultyString, ActIDString;
		//	FParse::Token(Cmd, DifficultyString, true);
		//	FParse::Token(Cmd, ActIDString, true);

		//	int32 Difficulty = FCString::Atoi(*DifficultyString);
		//	int32 ActID = FCString::Atoi(*ActIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetActInfo : Difficulty(%d), ActID(%d)"), Difficulty, ActID);

		//	RequestGetActInfoClass<int32, int32>::GetInstance().Signal(Difficulty, ActID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetStageInfo")))		// GetStageInfo StageID
		//{
		//	FString StageIDString;
		//	FParse::Token(Cmd, StageIDString, true);

		//	FServerStageID ServerStageID = FServerStageID(FCString::Atoi64(*StageIDString));

		//	UE_LOG(LogBladeII, Log, TEXT("GetStageInfo : StageID(%d)"), ServerStageID.Id);

		//	RequestGetStageInfoClass<FServerStageID>::GetInstance().Signal(ServerStageID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ReceiveActClearReward")))
		//{
		//	FString difficultyString, actIdString, rewardIndexString;

		//	FParse::Token(Cmd, difficultyString, true);
		//	FParse::Token(Cmd, actIdString, true);
		//	FParse::Token(Cmd, rewardIndexString, true);

		//	int32 difficulty = FCString::Atoi(*difficultyString);
		//	int32 actId = FCString::Atoi(*actIdString);
		//	int32 rewardIndex = FCString::Atoi(*rewardIndexString);

		//	UE_LOG(LogBladeII, Log, TEXT("ReceiveActClearReward : Difficulty(%d), ActID(%d), RewardIndex(%d)"), difficulty, actId, rewardIndex);

		//	RequestReceiveActClearRewardClass<int32, int32, int32>::GetInstance().Signal(difficulty, actId, rewardIndex);
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandHeroTower(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("IncHeroPiece")))
		//{
		//	FString HeroPieceString;
		//	FParse::Token(Cmd, HeroPieceString, true);

		//	int32 HeroPiece = FCString::Atoi(*HeroPieceString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::IncreaseHeroPieceMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseIncreaseHeroPiece> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestIncreaseHeroPiece>(EmptyMessage, HeroPiece);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ClearHeroTower")))
		//{
		//	FString FloorString;
		//	FParse::Token(Cmd, FloorString, true);

		//	int32 Floor = FCString::Atoi(*FloorString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ForceClearHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseForceClearHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestForceClearHeroTower>(EmptyMessage, Floor);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetHeroTower")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetHeroTower>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("StartHeroTower")))
		//{
		//	FString FloorString, UseGemString, PlayTokenString;
		//	FParse::Token(Cmd, FloorString, true);
		//	FParse::Token(Cmd, PlayTokenString, true);

		//	int32 Floor = FCString::Atoi(*FloorString);
		//	int32 MainCharacterType = b2network::B2CharacterType::Gladiator;
		//	int32 TagCharacterType = b2network::B2CharacterType::Assassin;
		//	
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::StartHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseStartHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestStartHeroTower>(EmptyMessage, Floor, MainCharacterType, TagCharacterType, PlayTokenString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FinishHeroTower")))
		//{
		//	FString FloorString, PlayTokenString, ClearTimeSecString, IsSuccessString;
		//	FParse::Token(Cmd, FloorString, true);
		//	FParse::Token(Cmd, PlayTokenString, true);
		//	FParse::Token(Cmd, ClearTimeSecString, true);
		//	FParse::Token(Cmd, IsSuccessString, true);

		//	int32 Floor = FCString::Atoi(*FloorString);
		//	int32 ClearTimeSec = FCString::Atoi(*ClearTimeSecString);
		//	bool IsSuccess = (FCString::Atoi(*IsSuccessString) != 0) ? true : false;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FinishHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFinishHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFinishHeroTower>(EmptyMessage, Floor, PlayTokenString, ClearTimeSec, IsSuccess);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AbandonHeroTower")))
		//{
		//	FString FloorString, PlayTokenString;
		//	FParse::Token(Cmd, FloorString, true);
		//	FParse::Token(Cmd, PlayTokenString, true);

		//	int32 Floor = FCString::Atoi(*FloorString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AbandonHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAbandonHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAbandonHeroTower>(EmptyMessage, Floor, PlayTokenString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceRelic")))
		//{
		//	FString CharacterTypeString, RelicIdString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, RelicIdString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 RelicId = FCString::Atoi(*RelicIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EnhanceRelicMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEnhanceRelic> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEnhanceRelic>(EmptyMessage, CharacterType, RelicId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PromotionRelic")))
		//{
		//	FString CharacterTypeString, RelicIdString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, RelicIdString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 RelicId = FCString::Atoi(*RelicIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PromotionRelicMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePromotionRelic> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPromotionRelic>(EmptyMessage, CharacterType, RelicId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetAccountRelic")))
		//{
		//	FString CharacterTypeString;
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	RequestGetAccountRelicClass<int32>::GetInstance().Signal(CharacterType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SweepHeroTower")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SweepHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSweepHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSweepHeroTower>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("BuyHeroTowerTicket")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BuyHeroTowerTicketMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBuyHeroTowerTicket> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBuyHeroTowerTicket>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PrepareSweepHeroTower")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PrepareSweepHeroTowerMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePrepareSweepHeroTower> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPrepareSweepHeroTower>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandCounter(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("ForceCounter")))
		//{
		//	FString DungeonIdString;
		//	FParse::Token(Cmd, DungeonIdString, true);

		//	FServerStageID ServerStageId = FServerStageID(FCString::Atoi(*DungeonIdString));

		//	FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
		//	StageData.PrepareForceStartCounterDungeon(ServerStageId);

		//	RequestForceStartCounterDungeonClass<FServerStageID>::GetInstance().Signal(ServerStageId);
		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("GetCounterDungeon")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetCounterDungeonMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetCounterDungeon>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetCounterDungeon>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("StartCounterDungeon")))
		//{
		//	FString DungeonIdString, CharacterTypeString;
		//	FParse::Token(Cmd, DungeonIdString, true);
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int32 DungeonID = FCString::Atoi(*DungeonIdString);
		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::StartCounterDungeonMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseStartCounterDungeon>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestStartCounterDungeon>(EmptyMessage, DungeonID, CharacterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FinishCounterDungeon")))
		//{
		//	FString DungeonIdString, PlayTokenString, IsSuccessString;
		//	FParse::Token(Cmd, DungeonIdString, true);
		//	FParse::Token(Cmd, PlayTokenString, true);
		//	FParse::Token(Cmd, IsSuccessString, true);

		//	int32 DungeonID = FCString::Atoi(*DungeonIdString);
		//	int32 IsSuccessValue = FCString::Atoi(*IsSuccessString);
		//	bool IsSuccess = (IsSuccessValue != 0) ? true : false;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FinishCounterDungeonMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFinishCounterDungeon>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFinishCounterDungeon>(EmptyMessage, DungeonID, PlayTokenString, IsSuccess, false);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SweepCounterDungeon")))
		//{
		//	FString DungeonIdString, CharacterTypeString;
		//	FParse::Token(Cmd, DungeonIdString, true);
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int32 DungeonID = FCString::Atoi(*DungeonIdString);
		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SweepCounterDungeonMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSweepCounterDungeon>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSweepCounterDungeon>(EmptyMessage, DungeonID, CharacterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("BuyCounterDungeonTicket")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BuyCounterDungeonTicketMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBuyCounterDungeonTicket>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBuyCounterDungeonTicket>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("CounterDungeonWeeklyReward")))
		//{
		//	FString ClearCountString;
		//	FParse::Token(Cmd, ClearCountString, true);

		//	int32 ClearCount = FCString::Atoi(*ClearCountString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CounterDungeonWeeklyRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCounterDungeonWeeklyReward>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCounterDungeonWeeklyReward>(EmptyMessage, ClearCount);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}
	bool ExecHandler::ExecuteShellCommandDimension(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("GetDimension")))
		//{
		//	RequestGetDimensionClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("StartDimension")))
		//{
		//	FString DifficultyLevelString, DungeonIdString, CharacterTypeString, TotalPowerString;
		//	FParse::Token(Cmd, DifficultyLevelString, true);
		//	FParse::Token(Cmd, DungeonIdString, true);
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, TotalPowerString, true);

		//	int32 difficultyLevel = FCString::Atoi(*DifficultyLevelString);
		//	int32 dungeonID = FCString::Atoi(*DungeonIdString);
		//	int32 characterType = FCString::Atoi(*CharacterTypeString);
		//	int32 totalPower = FCString::Atoi(*TotalPowerString);

		//	RequestStartDimensionClass<int32, int32, int32, int32>::GetInstance().Signal(difficultyLevel, dungeonID, characterType, totalPower);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FinishDimension")))
		//{
		//	FString DifficultyLevelString, PlayTokenString, PlayResultString, PlayTimeMillisString;
		//	FParse::Token(Cmd, DifficultyLevelString, true);
		//	FParse::Token(Cmd, PlayTokenString, true);
		//	FParse::Token(Cmd, PlayResultString, true);
		//	FParse::Token(Cmd, PlayTimeMillisString, true);

		//	int32 difficultyLevel = FCString::Atoi(*DifficultyLevelString);
		//	int32 playResult = FCString::Atoi(*PlayResultString);
		//	int32 playTimeMillis = FCString::Atoi(*PlayTimeMillisString);

		//	RequestFinishDimensionClass<int32, const FString&, int32, int32>::GetInstance().Signal(difficultyLevel, PlayTokenString, playResult, playTimeMillis);
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandCharacter(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("EnhanceRankNode")))
		//{
		//	FString characterTypeString;

		//	FParse::Token(Cmd, characterTypeString, true);
		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("EnhanceRankNode : CharacterType(%I64d)"), characterType);

		//	RequestEnhanceRankNodeClass<int32>::GetInstance().Signal(characterType);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetWing")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetWingMessage(
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetWing>(messageInfo);
		//		//},
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		//})
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetWing>(EmptyMessage, characterType);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceWing")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EnhanceWingMessage(
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEnhanceWing>(messageInfo);
		//		//},
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		//})
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEnhanceWing>(EmptyMessage, characterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpgradeWing")))
		//{
		//	FString characterTypeString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	int32 characterType = FCString::Atoi(*characterTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UpgradeWingMessage(
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpgradeWing>(messageInfo);
		//		//},
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		//})
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpgradeWing>(EmptyMessage, characterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceWingOption")))
		//{
		//	FString characterTypeString, optionIndexString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, optionIndexString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 optionIndex = FCString::Atoi(*optionIndexString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EnhanceWingOptionMessage(
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEnhanceWingOption> (messageInfo);
		//		//},
		//		//[](FB2MessageInfoPtr messageInfo)
		//		//{
		//		//	b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		//})
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEnhanceWingOption>(EmptyMessage, characterType, optionIndex);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetWingVisible")))
		//{
		//	FString characterTypeString, isVisibleString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, isVisibleString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 isVisibleValue = FCString::Atoi(*isVisibleString);

		//	bool isVisible = (isVisibleValue != 0) ? true : false;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetWingVisibleMessage(
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetWingVisible>(EmptyMessage, characterType, isVisible);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetHelmetVisible")))
		//{
		//	FString characterTypeString, isVisibleString;
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, isVisibleString, true);

		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 isVisibleValue = FCString::Atoi(*isVisibleString);

		//	bool isVisible = (isVisibleValue != 0) ? true : false;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetHelmetVisibleMessage(
		//		HandlerBank::GetGlobalHandler(),
		//		HandlerBank::GetGlobalHandler())
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetHelmetVisible>(EmptyMessage, characterType, isVisible);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandSkill(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("LevelUpSkill")))		// LevelUpSkill CharacterType SkillId
		//{
		//	FString CharacterTypeString, SkillIdString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, SkillIdString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 SkillId = FCString::Atoi(*SkillIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("LevelUpSkill : CharacterType(%d), SkillId(%d)"),
		//		CharacterType, SkillId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::LevelUpSkillMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLevelUpSkill>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseLevelUpSkill::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseLevelUpSkill");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete LevelUpSkill. leftSkillPoint(%d), skillId(%d), skillLevel(%d)"),
		//			Message->left_skill_point, Message->skill->skill_id, Message->skill->skill_level);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLevelUpSkill>(
		//		EmptyMessage, CharacterType, SkillId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ResetSkill")))		// ResetSkill CharacterType
		//{
		//	FString CharacterTypeString;
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("ResetSkill : CharacterType(%d)"), CharacterType);

		//	RequestResetSkillClass<int32>::GetInstance().Signal(CharacterType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetUsingSkill")))		// SetUsingSkill CharacterType PresetType SkillID1 SkillID2 SkillID3
		//{
		//	FString CharacterTypeString, PresetTypeString, SkillID1String, SkillID2String, SkillID3String;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, PresetTypeString, true);
		//	FParse::Token(Cmd, SkillID1String, true);
		//	FParse::Token(Cmd, SkillID2String, true);
		//	FParse::Token(Cmd, SkillID3String, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 PresetType = FCString::Atoi(*PresetTypeString);
		//	int32 SkillID1 = FCString::Atoi(*SkillID1String);
		//	int32 SkillID2 = FCString::Atoi(*SkillID2String);
		//	int32 SkillID3 = FCString::Atoi(*SkillID3String);

		//	UE_LOG(LogBladeII, Log, TEXT("ResetSkill : CharacterType(%d), PresetType(%d), SkillID1(%d), SkillID2(%d), SkillID3(%d)"),
		//		CharacterType, PresetType, SkillID1, SkillID2, SkillID3);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetUsingSkillMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetUsingSkill>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseSetUsingSkill::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseSetUsingSkill");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete SetUsingSkill. SkillID1(%d), SkillID2(%d), SkillID3(%d)"),
		//			Message->skill_quick_slot->using_skill_id_1, Message->skill_quick_slot->using_skill_id_2, Message->skill_quick_slot->using_skill_id_3);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetUsingSkill>(
		//		EmptyMessage, CharacterType, PresetType, SkillID1, SkillID2, SkillID3);
		//	Message->SendMessage();
		//	return true;
		//}

		//else if (FParse::Command(&Cmd, TEXT("BuySkillPoint")))		// BuySkillPoint CharacterType PresetId BuyCount
		//{
		//	FString CharacterTypeString, PresetIdString, BuyCountString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, PresetIdString, true);
		//	FParse::Token(Cmd, BuyCountString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 PresetId = FCString::Atoi(*PresetIdString);
		//	int32 BuyCount = FCString::Atoi(*BuyCountString);

		//	UE_LOG(LogBladeII, Log, TEXT("BuySkillPoint : CharacterType(%d), PresetId(%d), BuyCount(%d)"),
		//		CharacterType, PresetId, BuyCount);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BuySkillPointMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBuySkillPoint>(messageInfo);

		//		using my_tuple = TU::MakeTuple<b2network::FB2MessageInfoResponseBuySkillPoint::type_list>::Result;
		//		my_tuple box;
		//		const reflection::ProtoClass* Proto = reflection::ProtoClass::from_name("FB2MessageInfoResponseBuySkillPoint");
		//		CopyFromStruct(box, Proto, Message, Proto->begin(), Proto->end());

		//		UE_LOG(LogBladeII, Log, TEXT("Complete BuySkillPoint. leftGem(%d)"), Message->left_gem);
		//		for (b2network::B2BuySkillPointResponseInfoPtr buyResponseInfo : Message->buy_results)
		//		{
		//			UE_LOG(LogBladeII, Log, TEXT("          characterType(%d), leftSkillPoint(%d), totalBuyCount(%d)"),
		//				buyResponseInfo->character_type, buyResponseInfo->left_skill_point, buyResponseInfo->total_skill_point_buy_count);

		//		}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto buyInfo = std::make_shared<b2network::B2BuySkillPointRequestInfo>();
		//	buyInfo->character_type = CharacterType;
		//	buyInfo->skill_point_buy_count = BuyCount;

		//	TArray<b2network::B2BuySkillPointRequestInfoPtr> buySkillPointRequest;
		//	buySkillPointRequest.Emplace(buyInfo);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBuySkillPoint>(
		//		EmptyMessage, buySkillPointRequest);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandMail(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("GetMailList")))
		//{
		//	FString mailCategoryString;

		//	FParse::Token(Cmd, mailCategoryString, true);
		//	int32 mailCategory = FCString::Atoi(*mailCategoryString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetMailList : MailCategory(%I64d)"), mailCategory);

		//	RequestGetMailListClass<int32>::GetInstance().Signal(mailCategory);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("OpenMail")))
		//{
		//	FString mailIdString;
		//	FString characterTypeString;
		//	FString itemTemplateIdString;
		//	FString aetherTypeString;

		//	FParse::Token(Cmd, mailIdString, true);
		//	FParse::Token(Cmd, characterTypeString, true);
		//	FParse::Token(Cmd, itemTemplateIdString, true);
		//	FParse::Token(Cmd, aetherTypeString, true);

		//	int64 mailId = FCString::Atoi64(*mailIdString);
		//	int32 characterType = FCString::Atoi(*characterTypeString);
		//	int32 itemTemplateId = FCString::Atoi(*itemTemplateIdString);
		//	int32 aetherType = FCString::Atoi(*aetherTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("OpenMail : mailId(%I64d), characterType(%d), itemTemplateId(%d), aetherType(%d)"), mailId, characterType, itemTemplateId, aetherType);

		//	RequestOpenMailClass<int64, int32, int32, int32>::GetInstance().Signal(mailId, characterType, itemTemplateId, aetherType);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("OpenAllMail")))
		//{
		//	FString mailICategoryString, mailStartIdString, mailEndIdString;

		//	FParse::Token(Cmd, mailICategoryString, true);
		//	FParse::Token(Cmd, mailStartIdString, true);
		//	FParse::Token(Cmd, mailEndIdString, true);

		//	int32 mailCategory = FCString::Atoi(*mailICategoryString);
		//	int64 mailStartId = FCString::Atoi(*mailStartIdString);
		//	int64 mailEndId = FCString::Atoi(*mailEndIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("OpenAllMail : mailCategory(%d), startId(%I64d), endId(%I64d)"), mailCategory, mailStartId, mailEndId);

		//	RequestOpenAllMailClass<int32, int64, int64>::GetInstance().Signal(mailCategory, mailStartId, mailEndId);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetNewMailCount")))
		//{
		//	UE_LOG(LogBladeII, Log, TEXT("GetNewMailCount"));

		//	RequestGetNewMailCountClass<>::GetInstance().Signal();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AddMail")))
		//{
		//	FString mailCategoryString, presentAttachTypeString, placeTypeString, senderTypeString;
		//	FString amountString, itemTemplateIdString, generalLotteryIdString, fixedGradeLotteryIdString, selectiveLotteryIdString, selectiveItemLotteryIdString, prefixSelectiveLotteryIdString;

		//	FParse::Token(Cmd, mailCategoryString, true);
		//	FParse::Token(Cmd, presentAttachTypeString, true);
		//	FParse::Token(Cmd, placeTypeString, true);
		//	FParse::Token(Cmd, senderTypeString, true);
		//	FParse::Token(Cmd, amountString, true);

		//	FParse::Token(Cmd, itemTemplateIdString, true);
		//	FParse::Token(Cmd, generalLotteryIdString, true);
		//	FParse::Token(Cmd, fixedGradeLotteryIdString, true);
		//	FParse::Token(Cmd, selectiveLotteryIdString, true);
		//	FParse::Token(Cmd, selectiveItemLotteryIdString, true);
		//	FParse::Token(Cmd, prefixSelectiveLotteryIdString, true);

		//	int32 mailCategory = FCString::Atoi(*mailCategoryString);
		//	int32 presentAttachType = FCString::Atoi(*presentAttachTypeString);
		//	int32 placeType = FCString::Atoi(*placeTypeString);
		//	int32 senderType = FCString::Atoi(*senderTypeString);
		//	int32 amount = FCString::Atoi(*amountString);

		//	int32 itemTemplateId = FCString::Atoi(*itemTemplateIdString);
		//	int32 generalLotteryId = FCString::Atoi(*generalLotteryIdString);
		//	int32 fixedGradeLotteryId = FCString::Atoi(*fixedGradeLotteryIdString);
		//	int32 selectiveLotteryId = FCString::Atoi(*selectiveLotteryIdString);
		//	int32 selectiveItemLotteryId = FCString::Atoi(*selectiveItemLotteryIdString);
		//	int32 prefixSelectiveLotteryId = FCString::Atoi(*prefixSelectiveLotteryIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("AddMail mailCategory(%d), presentAttachType(%d), placeType(%d), senderType(%d) amount(%d), itemTemplateId(%d), generalLotteryId(%d), fixedGradeLotteryId(%d), seleciveLotteryId(%d), selectiveItemLotteryId(%d), prefixSelectiveLotteryId(%d)"),
		//	mailCategory, presentAttachType, placeType, senderType, amount, itemTemplateId, generalLotteryId, fixedGradeLotteryId, selectiveLotteryId, selectiveItemLotteryId, prefixSelectiveLotteryId);

		//	RequestAddMailClass<int32, int32, int32, int32, int32, int32, int32, int32, int32, int32, int32>
		//		::GetInstance().Signal(mailCategory, presentAttachType, placeType, senderType, amount, itemTemplateId, generalLotteryId, fixedGradeLotteryId, selectiveLotteryId, selectiveItemLotteryId, prefixSelectiveLotteryId);

  //          return true;
		//}
  //      else if (FParse::Command(&Cmd, TEXT("AddMailLoop")))
  //      {
  //          FString mailCategoryString, presentAttachTypeString, placeTypeString, senderTypeString;

  //          FString loopCount;

  //          FString amountString, itemTemplateIdString, generalLotteryIdString, fixedGradeLotteryIdString, selectiveLotteryIdString, selectiveItemLotteryIdString, prefixSelectiveLotteryIdString;

  //          FParse::Token(Cmd, mailCategoryString, true);
  //          FParse::Token(Cmd, presentAttachTypeString, true);
  //          FParse::Token(Cmd, placeTypeString, true);
  //          FParse::Token(Cmd, senderTypeString, true);
  //          FParse::Token(Cmd, amountString, true);

  //          FParse::Token(Cmd, loopCount, true);

  //          FParse::Token(Cmd, itemTemplateIdString, true);
  //          FParse::Token(Cmd, generalLotteryIdString, true);
		//	FParse::Token(Cmd, fixedGradeLotteryIdString, true);
  //          FParse::Token(Cmd, selectiveLotteryIdString, true);
  //          FParse::Token(Cmd, selectiveItemLotteryIdString, true);
		//	FParse::Token(Cmd, prefixSelectiveLotteryIdString, true);

  //          int32 mailCategory = FCString::Atoi(*mailCategoryString);
  //          int32 presentAttachType = FCString::Atoi(*presentAttachTypeString);
  //          int32 placeType = FCString::Atoi(*placeTypeString);
  //          int32 senderType = FCString::Atoi(*senderTypeString);
  //          int32 amount = FCString::Atoi(*amountString);

  //          int32 count = FCString::Atoi(*loopCount);

  //          int32 itemTemplateId = FCString::Atoi(*itemTemplateIdString);
  //          int32 generalLotteryId = FCString::Atoi(*generalLotteryIdString);
		//	int32 fixedGradeLotteryId = FCString::Atoi(*fixedGradeLotteryIdString);
  //          int32 selectiveLotteryId = FCString::Atoi(*selectiveLotteryIdString);
  //          int32 selectiveItemLotteryId = FCString::Atoi(*selectiveItemLotteryIdString);
		//	int32 prefixSelectiveLotteryId = FCString::Atoi(*prefixSelectiveLotteryIdString);

  //          //UE_LOG(LogBladeII, Log, TEXT("AddMail mailCategory(%d), presentAttachType(%d), placeType(%d), senderType(%d) amount(%d), loopCount(%d), itemTemplateIdString(%d), generalLotteryId(%d), fixedGradeLotteryId(%d), seleciveLotteryId(%d), selectiveItemLotteryId(%d), prefixSelectiveLotteryId(%d)"),
  //          //    mailCategory, presentAttachType, placeType, senderType, amount,
  //          //    count, itemTemplateId, generalLotteryId, fixedGradeLotteryId, selectiveLotteryId, selectiveItemLotteryId, prefixSelectiveLotteryId);
  //          for (int i = 0; i < count; i++)
  //          {
  //              //RequestAddMailClass<int32, int32, int32, int32, int32, int32, int32, int32, int32, int32, int32>
  //                  //::GetInstance().Signal(mailCategory, presentAttachType, placeType, senderType, amount, itemTemplateId, generalLotteryId, fixedGradeLotteryId, selectiveLotteryId, selectiveItemLotteryId, prefixSelectiveLotteryId);
  //          }

  //          return true;
  //      }

		return false;
	}

	bool ExecHandler::ExecuteShellCommandFriend(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("AskFriend")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAskFriend>(EmptyMessage, AccountId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AcceptAskFriend")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AcceptAskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAcceptAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAcceptAskFriend>(EmptyMessage, AccountId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RejectAskFriend")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RejectAskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRejectAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRejectAskFriend>(EmptyMessage, AccountId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetFriendList")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetFriendListMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetFriendList>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetFriendList>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("CancelAskFriend")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CancelAskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCancelAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCancelAskFriend>(EmptyMessage, AccountId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DeleteFriend")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DeleteFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDeleteFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDeleteFriend>(EmptyMessage, AccountId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetAllSendAskFriend")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetAllSendAskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetAllSendAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetAllSendAskFriend>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetAllReceiveAskFriend")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetAllReceiveAskFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetAllReceiveAskFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetAllReceiveAskFriend>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SendSocialPoint")))
		//{
		//	FString AccountIdString;
		//	FParse::Token(Cmd, AccountIdString, true);
		//	int64 AccountId = FCString::Atoi64(*AccountIdString);

		//	TArray<int64> AccountList;
		//	AccountList.Emplace(AccountId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SendSocialPointMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSendSocialPoint>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSendSocialPoint>(EmptyMessage, AccountList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ConfirmNewFriend")))
		//{
		//	bool isAllConfirm = true;
		//	TArray<int64> friendIdList;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ConfirmNewFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseConfirmNewFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestConfirmNewFriend>(EmptyMessage, isAllConfirm, friendIdList);
		//	Message->SendMessage();
		//	return true;

		//	//RequestConfirmNewFriendClass<bool, TArray<int64>>::GetInstance().Signal(isAllConfirm, friendIdList);
		//	//return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetRecommendFriend")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetRecommendFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetRecommendFriend>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetRecommendFriend>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PlatformFriendInviteInfo")))
		//{
		//	int32 platformType = 1;
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PlatformFriendInviteInfoMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePlatformFriendInviteInfo>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPlatformFriendInviteInfo>(EmptyMessage, platformType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PlatformFriendInviteCountUp")))
		//{
		//	int32 platformType = 1;
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PlatformFriendInviteCountUpMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePlatformFriendInviteCountUp>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPlatformFriendInviteCountUp>(EmptyMessage, platformType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PlatformFriendJoinerCountUp")))
		//{
		//	int32 platformType = 1;

		//	FString joinerCountString;
		//	FParse::Token(Cmd, joinerCountString, true);
		//	int32 joinerCount = FCString::Atoi(*joinerCountString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PlatformFriendJoinerCountUpMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePlatformFriendJoinerCountUp>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPlatformFriendJoinerCountUp>(EmptyMessage, platformType, joinerCount);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PlatformFriendInviteReward")))
		//{
		//	int32 platformType = 1;

		//	FString rewardTypeString, rewardIndexString;
		//	FParse::Token(Cmd, rewardTypeString, true);
		//	FParse::Token(Cmd, rewardIndexString, true);

		//	int32 rewardType = FCString::Atoi(*rewardTypeString);
		//	int32 rewardIndex = FCString::Atoi(*rewardIndexString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PlatformFriendInviteRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePlatformFriendInviteReward>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPlatformFriendInviteReward>(EmptyMessage, platformType, rewardType, rewardIndex);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FindUser")))
		//{
		//	FString Nickname;
		//	FParse::Token(Cmd, Nickname, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FindUserMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFindUser>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFindUser>(EmptyMessage, Nickname, false);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetFriendRankings")))
		//{
		//	FString opponentTypeString;

		//	FParse::Token(Cmd, opponentTypeString, true);
		//	int32 opponentType = FCString::Atoi(*opponentTypeString);

		//	RequestGetFriendRankingsClass<int32>::GetInstance().Signal(opponentType);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PrepareFriend")))
		//{
		//	FString newFriendCountString, jobTypeString;

		//	FParse::Token(Cmd, newFriendCountString, true);
		//	FParse::Token(Cmd, jobTypeString, true);

		//	int32 newFriendCount = FCString::Atoi(*newFriendCountString);
		//	int32 jobType = FCString::Atoi(*jobTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("PrepareFriend : newFriendCount(%d), jobType(%d)"), newFriendCount, jobType);

		//	//RequestjobTypeStringClass<int32, int32>::GetInstance().Signal(newFriendCount, jobType);
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PrepareFriendMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePrepareFriend>(messageInfo);
		//		UE_LOG(LogBladeII, Log, TEXT("Complete prepare friend."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPrepareFriend>(EmptyMessage, newFriendCount, jobType);
		//	Message->SendMessage();
		//	return true;

		//	//RequestPrepareFriendClass<int32, int32>::GetInstance().Signal(newFriendCount, jobType);
		//	//return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandQuest(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("GetQuest")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetQuestMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetQuest>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetQuest>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AcceptQuest")))
		//{
		//	FString questIdString;
		//	FParse::Token(Cmd, questIdString, true);
		//	int32 questSlot = FCString::Atoi(*questIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AcceptQuestMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAcceptQuest>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAcceptQuest>(EmptyMessage, questSlot);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ReceiveQuestReward")))
		//{
		//	FString questIdString;
		//	FParse::Token(Cmd, questIdString, true);
		//	int32 questSlot = FCString::Atoi(*questIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceiveQuestRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceiveQuestReward>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceiveQuestReward>(EmptyMessage, questSlot);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetQuestState")))		// 
		//{
		//	FString QuestIDString, QuestStateString;
		//	FParse::Token(Cmd, QuestIDString, true);
		//	FParse::Token(Cmd, QuestStateString, true);

		//	int32 QuestID = FCString::Atoi(*QuestIDString);
		//	int32 QuestState = FCString::Atoi(*QuestStateString);


		//	UE_LOG(LogBladeII, Log, TEXT("SetQuestState : QuestID(%d), QuestState(%d)"), QuestID, QuestState);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetQuestStateMessage(
		//		[QuestID, QuestState](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetQuestState>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetQuestState>(EmptyMessage, QuestID, QuestState);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ChangeQuestId")))
		//{
		//	FString QuestSlotString, QuestIDString;
		//	FParse::Token(Cmd, QuestSlotString, true);
		//	FParse::Token(Cmd, QuestIDString, true);			

		//	int32 QuestSlot = FCString::Atoi(*QuestSlotString);
		//	int32 QuestID = FCString::Atoi(*QuestIDString);

		//	RequestChangeQuestIdClass<int32, int32>::GetInstance().Signal(QuestSlot, QuestID);
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ChangeQuestCount")))
		//{
		//	FString QuestSlotString, CountString;
		//	FParse::Token(Cmd, QuestSlotString, true);
		//	FParse::Token(Cmd, CountString, true);

		//	int32 QuestSlot = FCString::Atoi(*QuestSlotString);
		//	int32 Count = FCString::Atoi(*CountString);

		//	RequestChangeQuestCountClass<int32, int32>::GetInstance().Signal(QuestSlot, Count);
		//	return true;
		//}


		return false; 
	}

	bool ExecHandler::ExecuteShellCommandGuild(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("CreateGuild")))	// CreateGuild name joinoption
		//{
		//	FString GuildNameString, JoinOptionString;
		//	FParse::Token(Cmd, GuildNameString, true);
		//	FParse::Token(Cmd, JoinOptionString, true);

		//	int32 JoinOption = FCString::Atoi(*JoinOptionString);
		//	int32 MarkIndex = 1;
		//	int32 MarkColor = 1;
		//	int32 MarkBgIndex = 1;
		//	int32 MarkBgColor = 1;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CreateGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCreateGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCreateGuild>(
		//		EmptyMessage, GuildNameString, JoinOption, MarkIndex, MarkColor, MarkBgIndex, MarkBgColor);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AskJoinGuild")))	// AskJoinGuild guild_id
		//{
		//	FString GuildIDString;
		//	FParse::Token(Cmd, GuildIDString, true);

		//	int64 GuildID = FCString::Atoi64(*GuildIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AskJoinGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAskJoinGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAskJoinGuild>(EmptyMessage, GuildID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LeaveGuild")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::LeaveGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLeaveGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLeaveGuild>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildDetailInfo")))	// GetGuildDetailInfo guild_id
		//{
		//	FString GuildIDString;
		//	FParse::Token(Cmd, GuildIDString, true);

		//	int64 GuildID = FCString::Atoi64(*GuildIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildDetailInfoMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildDetailInfo>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildDetailInfo>(EmptyMessage, GuildID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DeleteGuild")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DeleteGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDeleteGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDeleteGuild>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetRecommendGuild")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetRecommendGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetRecommendGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetRecommendGuild>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AppointSubGuildMaster")))
		//{
		//	FString AccountIDString;
		//	FParse::Token(Cmd, AccountIDString, true);

		//	int64 AccountID = FCString::Atoi64(*AccountIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AppointSubGuildMasterMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAppointSubGuildMaster>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAppointSubGuildMaster>(EmptyMessage, AccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DismissSubGuildMaster")))
		//{
		//	FString AccountIDString;
		//	FParse::Token(Cmd, AccountIDString, true);

		//	int64 AccountID = FCString::Atoi64(*AccountIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DismissSubGuildMasterMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDismissSubGuildMaster>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDismissSubGuildMaster>(EmptyMessage, AccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("KickGuildMember")))
		//{
		//	FString AccountIDString;
		//	FParse::Token(Cmd, AccountIDString, true);

		//	int64 AccountID = FCString::Atoi64(*AccountIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::KickGuildMemberMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseKickGuildMember>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestKickGuildMember>(EmptyMessage, AccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpdateGuild")))
		//{
		//	FString JoinOptionString, MarkString, MarkColorString, MarkBgString, MarkBgColorString;
		//	FParse::Token(Cmd, JoinOptionString, true);
		//	FParse::Token(Cmd, MarkString, true);
		//	FParse::Token(Cmd, MarkColorString, true);
		//	FParse::Token(Cmd, MarkBgString, true);
		//	FParse::Token(Cmd, MarkBgColorString, true);

		//	int32 JoinOption = FCString::Atoi(*JoinOptionString);
		//	int32 Mark = FCString::Atoi(*MarkString);
		//	int32 MarkColor = FCString::Atoi(*MarkColorString);
		//	int32 MarkBg = FCString::Atoi(*MarkBgString);
		//	int32 MarkBgColor = FCString::Atoi(*MarkBgColorString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UpdateGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpdateGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpdateGuild>(EmptyMessage, JoinOption, Mark, MarkColor, MarkBg, MarkBgColor);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UpdateGuildNotice")))
		//{
		//	FString NoticeString;
		//	FParse::Token(Cmd, NoticeString, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UpdateGuildNoticeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUpdateGuildNotice>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUpdateGuildNotice>(EmptyMessage, NoticeString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildMemberCandidates")))
		//{
		//	FString NeedFriendString;
		//	FParse::Token(Cmd, NeedFriendString, true);

		//	bool NeedFriend = FCString::ToBool(*NeedFriendString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildMemberCandidatesMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildMemberCandidates>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildMemberCandidates>(EmptyMessage, NeedFriend);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("InviteGuild")))
		//{
		//	FString AccountIDString;
		//	FParse::Token(Cmd, AccountIDString, true);

		//	int64 AccountID = FCString::Atoi64(*AccountIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::InviteGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseInviteGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestInviteGuild>(EmptyMessage, AccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildInviteList")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildInviteListMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildInviteList>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildInviteList>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RejectInviteGuild")))
		//{
		//	FString GuildIDString;
		//	FParse::Token(Cmd, GuildIDString, true);

		//	int64 GuildID = FCString::Atoi64(*GuildIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RejectInviteGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRejectInviteGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRejectInviteGuild>(EmptyMessage, GuildID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AcceptInviteGuild")))
		//{
		//	FString GuildIDString;
		//	FParse::Token(Cmd, GuildIDString, true);

		//	int64 GuildID = FCString::Atoi64(*GuildIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AcceptInviteGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAcceptInviteGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAcceptInviteGuild>(EmptyMessage, GuildID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FindGuild")))
		//{
		//	FString GuildNameString;
		//	FParse::Token(Cmd, GuildNameString, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FindGuildMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFindGuild>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFindGuild>(EmptyMessage, GuildNameString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ObtainGuildMaster")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ObtainGuildMasterMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseObtainGuildMaster>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestObtainGuildMaster>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("HandOverGuildMaster")))
		//{
		//	FString AccountIDString;
		//	FParse::Token(Cmd, AccountIDString, true);

		//	int64 AccountID = FCString::Atoi64(*AccountIDString);
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::HandOverGuildMasterMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseHandOverGuildMaster>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestHandOverGuildMaster>(EmptyMessage, AccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildRankingList")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildRankingListMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildRankingList>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildRankingList>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LevelUpGuildSkill")))
		//{
		//	FString GuildSkillIDString;
		//	FParse::Token(Cmd, GuildSkillIDString, true);

		//	int32 GuildSkillID = FCString::Atoi(*GuildSkillIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::LevelUpGuildSkillMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLevelUpGuildSkill>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLevelUpGuildSkill>(EmptyMessage, GuildSkillID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("BuyGuildSkill")))
		//{
		//	FString GuildSkillIDString;
		//	FString GuildSkillBuyCountString;
		//	FParse::Token(Cmd, GuildSkillIDString, true);
		//	FParse::Token(Cmd, GuildSkillBuyCountString, true);

		//	int32 GuildSkillID = FCString::Atoi(*GuildSkillIDString);
		//	int32 GuildSkillBuyCount = FCString::Atoi(*GuildSkillBuyCountString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BuyGuildSkillMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBuyGuildSkill>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBuyGuildSkill>(EmptyMessage, GuildSkillID, GuildSkillBuyCount);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildDonation")))
		//{
		//	FString MercenaryIDString, DonationTypeString;
		//	FParse::Token(Cmd, MercenaryIDString, true);
		//	FParse::Token(Cmd, DonationTypeString, true);

		//	int32 MercenaryID = FCString::Atoi(*MercenaryIDString);
		//	int32 DonationType = FCString::Atoi(*DonationTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildDonationMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildDonation>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildDonation>(EmptyMessage, MercenaryID, DonationType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildMember")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildMemberMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildMember>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildMember>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandMission(const TCHAR* Cmd)
	{
		/*if (FParse::Command(&Cmd, TEXT("GetMission")))
		{
			FString missionTypeString;
			FParse::Token(Cmd, missionTypeString, true);
			int32 missionType = FCString::Atoi(*missionTypeString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetMissionMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetMission>(messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetMission>(EmptyMessage, missionType);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("GetFeaturedMission")))
		{
			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetFeaturedMissionMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetFeaturedMission>(messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetFeaturedMission>(EmptyMessage);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ReceiveMissionReward")))
		{
			FString missionIdString;
			FParse::Token(Cmd, missionIdString, true);
			int32 missionId = FCString::Atoi(*missionIdString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceiveMissionRewardMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceiveMissionReward>(messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceiveMissionReward>(EmptyMessage, missionId);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ReceiveCompleteMissionReward")))
		{
			FString missionTypeString, rewardIndexString;
			FParse::Token(Cmd, missionTypeString, true);
			FParse::Token(Cmd, rewardIndexString, true);

			int32 missionType = FCString::Atoi(*missionTypeString);
			int32 rewardIndex = FCString::Atoi(*rewardIndexString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::ReceiveCompleteMissionRewardMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseReceiveCompleteMissionReward>(messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestReceiveCompleteMissionReward>(EmptyMessage, missionType, rewardIndex);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("GetUnitySkillMissions")))
		{
			FString characterTypeString;
			FParse::Token(Cmd, characterTypeString, true);

			int32 characterType = FCString::Atoi(*characterTypeString);

			RequestGetUnitySkillMissionClass<int32>::GetInstance().Signal(characterType);
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ReceiveUnitySkillReward")))
		{
			FString mainCharacterTypeString, unityCharacterTypeString;
			FParse::Token(Cmd, mainCharacterTypeString, true);
			FParse::Token(Cmd, unityCharacterTypeString, true);

			int32 mainCharacterType = FCString::Atoi(*mainCharacterTypeString);
			int32 unityCharacterType = FCString::Atoi(*unityCharacterTypeString);

			RequestReceiveUnitySkillRewardClass<int32, int32>::GetInstance().Signal(mainCharacterType, unityCharacterType);
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("AwakenUnitySkillMission")))
		{
			FString characterTypeString, missionIdString;
			FParse::Token(Cmd, characterTypeString, true);
			FParse::Token(Cmd, missionIdString, true);

			int32 characterType = FCString::Atoi(*characterTypeString);
			int32 missionId = FCString::Atoi(*missionIdString);

			RequestAwakenUnitySkillMissionClass<int32, int32>::GetInstance().Signal(characterType, missionId);
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("AwakenUnitySkill")))
		{
			FString characterTypeString;
			FParse::Token(Cmd, characterTypeString, true);

			int32 characterType = FCString::Atoi(*characterTypeString);

			RequestAwakenUnitySkillClass<int32>::GetInstance().Signal(characterType);
			return true;
		}*/


		return false;
	}

	bool ExecHandler::ExecuteShellCommandShop(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("DrawShopLottery")))
		//{
		//	FString CharacterTypeString, EquipCategoryString, DrawPriceTypeString, ContinuousDrawString, isDrawAgainString, lastLotteryKeyString, isFreeLotteryString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, DrawPriceTypeString, true);
		//	FParse::Token(Cmd, ContinuousDrawString, true);
		//	FParse::Token(Cmd, isDrawAgainString, true);
		//	FParse::Token(Cmd, lastLotteryKeyString, true);
		//	FParse::Token(Cmd, isFreeLotteryString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 DrawPriceType = FCString::Atoi(*DrawPriceTypeString);
		//	bool ContinuousDraw = (FCString::Atoi(*ContinuousDrawString) != 0) ? true : false;
		//	bool isDrawAgain = (FCString::Atoi(*isDrawAgainString) != 0) ? true : false;
		//	bool isFreeLottery = (FCString::Atoi(*isFreeLotteryString) != 0) ? true : false;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DrawShopLotteryMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDrawShopLottery> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDrawShopLottery>(EmptyMessage,
		//		CharacterType, DrawPriceType, ContinuousDraw, isDrawAgain, lastLotteryKeyString, isFreeLottery);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RewardShopMileage")))
		//{
		//	FString RewardShopMileageTypeString;
		//	FParse::Token(Cmd, RewardShopMileageTypeString, true);

		//	int32 RewardShopMileageType = FCString::Atoi(*RewardShopMileageTypeString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardShopMileageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardShopMileage> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardShopMileage>(EmptyMessage, RewardShopMileageType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetLotteryShop")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetLotteryShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetLotteryShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetLotteryShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("BuyShopProduct")))
		//{
		//	FString productIdString;
		//	FParse::Token(Cmd, productIdString, true);

		//	int32 productId = FCString::Atoi(*productIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::BuyShopProductMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseBuyShopProduct> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestBuyShopProduct>(EmptyMessage, productId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGeneralShop")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGeneralShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGeneralShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGeneralShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetMagicShop")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetMagicShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetMagicShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetMagicShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RenewalMagicShop")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RenewalMagicShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRenewalMagicShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRenewalMagicShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PurchaseMagicShop")))
		//{
		//	FString idString;
		//	FParse::Token(Cmd, idString, true);

		//	int32 id = FCString::Atoi(*idString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PurchaseMagicShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePurchaseMagicShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPurchaseMagicShop>(EmptyMessage, id);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetMarketProduct")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetMarketProductMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetMarketProduct> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetMarketProduct>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PreparePurchase")))
		//{
		//	FString productIdString, marketProductId;
		//	FParse::Token(Cmd, productIdString, true);
		//	FParse::Token(Cmd, marketProductId, true);

		//	int32 productId = FCString::Atoi(*productIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PreparePurchaseMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePreparePurchase> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPreparePurchase>(EmptyMessage, productId, marketProductId);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("VerifyPurchase")))
		//{
		//	bool isRestore = true;
		//	FString marketProductId, payloadKeyString, purchaseDataString, purchaseSinatureString, currencyCodeString;
		//	FParse::Token(Cmd, marketProductId, true);
		//	FParse::Token(Cmd, payloadKeyString, true);

		//	// fixed test value
		//	purchaseDataString = "eyJvcmRlcklkIjoiR1BBLjMzMjQtMzA1Ni0xOTIzLTU0MDAyIiwicGFja2FnZU5hbWUiOiJjb20ua2FrYW9nYW1lcy5ibGFkZTIiLCJwcm9kdWN0SWQiOiJnZW1fZ2xfY2FzaF8wMDEiLCJwdXJjaGFzZVRpbWUiOjE1MTkyNzU4ODE0NDMsInB1cmNoYXNlU3RhdGUiOjAsImRldmVsb3BlclBheWxvYWQiOiJ1ZTQuZ2VtX2dsX2Nhc2hfMDAxIiwicHVyY2hhc2VUb2tlbiI6Im9waWljb25ob25vY2hlZmViZm5pbW1wbS5BTy1KMU96WjJQc1pFS2J1SFFIaFZ5QWxkb0ZQM0t5RDBBUkVEZlZGSnNUOTg2RDlBOTdmVDhRelRpdHcyZXhFT1hNYTJhYngyU0x1bXNVRkFTbEFMVllBYlJpUTZrdm5OYW5mb1RzNmtobUtrcWlIcmszYVdzUnRwSGxaUHhwOU9VU3B0eWp2d0t0WSJ9";
		//	purchaseSinatureString = "v2rLZTog0hBvCPeqc5CLxIukb0bmv4oZL3RojC4WQZln77wM5Yew1UsD4qFS5277+PsRfTRZIn+j3hs/8m6T1ZehcJjL5EsjchSM+Df5vUfFUhhhuzZUEopucF45R/7exqP6WKtN05UsZ1UF7RkJ2z/6ruaPpGuIZc5aq+eyJMumnTIcnp/sqDzGwiSOFwc5vJlaQORzOUm+tdsxojxONgs/AFV3gFQHtTArx5dP8oiAdnHW/KRyjo2xpSIQpEkT+9bZLrRNTBmwxOmM7Z7Mt4GSZzXvz9Oa0cfHEtaW05pSzwpyQ0RqynjJ6iJMmGNnYTMulIxuPVUl4g7XoL6IvQ==";
		//	currencyCodeString = "KRW";
		//	float price = 1100.0f;

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::VerifyPurchaseMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseVerifyPurchase> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestVerifyPurchase>(EmptyMessage, marketProductId, payloadKeyString, purchaseDataString, purchaseSinatureString, isRestore, currencyCodeString, price);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("CancelPurchase")))
		//{
		//	FString marketProductId, payloadKeyString, cancelReasonString;
		//	FParse::Token(Cmd, marketProductId, true);
		//	FParse::Token(Cmd, payloadKeyString, true);
		//	FParse::Token(Cmd, cancelReasonString, true);

		//	int32 cancelReason = FCString::Atoi(*cancelReasonString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::CancelPurchaseMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseCancelPurchase> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestCancelPurchase>(EmptyMessage, marketProductId, payloadKeyString, cancelReason);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FinishPurchase")))
		//{
		//	FString payloadKeyString;
		//	FParse::Token(Cmd, payloadKeyString, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FinishPurchaseMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFinishPurchase> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFinishPurchase>(EmptyMessage, payloadKeyString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RewardFlatRatePacakgeBonus"))) {
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardFlatRatePackageBonusMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardFlatRatePackageBonus> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardFlatRatePackageBonus>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetPackageShop"))) {
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetPackageShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetPackageShop> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetPackageShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetPeriodPackageState"))) {
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetPeriodPackageStateMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetPeriodPackageState> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetPeriodPackageState>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RewardLevelUpPackage"))) {
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardLevelUpPackageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardLevelUpPackage> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardLevelUpPackage>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RewardReachLevelPackage"))) {
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardReachLevelPackageMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardReachLevelPackage> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);
		//	
		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardReachLevelPackage>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RewardPointShopEvent")))
		//{
		//	FString eventString, indexString, buyCountString;
		//	FParse::Token(Cmd, eventString, true);
		//	FParse::Token(Cmd, indexString, true);
		//	FParse::Token(Cmd, buyCountString, true);

		//	int32 eventId = FCString::Atoi(*eventString);
		//	int32 index = FCString::Atoi(*indexString);
		//	int32 buyCount = FCString::Atoi(*buyCountString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardPointShopEventMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardPointShopEvent> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//	);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardPointShopEvent>(EmptyMessage, eventId, index, buyCount);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandCollection(const TCHAR* Cmd)
	{
		/*if (FParse::Command(&Cmd, TEXT("GetCollectionItem")))
		{
			FString CharacterTypeString, EquipCategoryString;
			FParse::Token(Cmd, CharacterTypeString, true);
			FParse::Token(Cmd, EquipCategoryString, true);

			int32 CharacterType = FCString::Atoi(*CharacterTypeString);
			int32 EquipCategory = FCString::Atoi(*EquipCategoryString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::GetCollectionItemMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetCollectionItem> (messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetCollectionItem>(EmptyMessage, CharacterType, EquipCategory);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("RewardCollectionItem")))
		{
			FString itemTemplateIdString;
			FParse::Token(Cmd, itemTemplateIdString, true);

			int32 itemTemplateId = FCString::Atoi(*itemTemplateIdString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardCollectionItemMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardCollectionItem> (messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardCollectionItem>(EmptyMessage, itemTemplateId);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("RewardCollectionSetItem")))
		{
			FString characterTypeString, gradeString, groupIdString;
			FParse::Token(Cmd, characterTypeString, true);
			FParse::Token(Cmd, gradeString, true);
			FParse::Token(Cmd, groupIdString, true);

			int32 characterType = FCString::Atoi(*characterTypeString);
			int32 grade = FCString::Atoi(*gradeString);
			int32 groupId = FCString::Atoi(*gradeString);

			auto EmptyMessage = EmptyMessagePtr(new data_trader::RewardCollectionSetItemMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRewardCollectionSetItem> (messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRewardCollectionSetItem>(EmptyMessage, characterType, grade, groupId);
			Message->SendMessage();
			return true;
		}*/

		return false;
	}

	bool ExecHandler::ExecuteShellCommandAttendance(const TCHAR* Cmd)
	{

		/*if (FParse::Command(&Cmd, TEXT("DailyAttendance")))
		{
			auto EmptyMessage = EmptyMessagePtr(new data_trader::DailyAttendanceMessage(
				[](FB2MessageInfoPtr messageInfo)
			{
				auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDailyAttendance> (messageInfo);
			},
				[](FB2MessageInfoPtr messageInfo)
			{
				b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
			})
				);

			auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDailyAttendance>(EmptyMessage);
			Message->SendMessage();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ResetAttendanceTime")))
		{
			RequestResetAttendanceTimeClass<>::GetInstance().Signal();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ResetEventAttendance")))
		{
			FString IsOnlyTimeString;
			FParse::Token(Cmd, IsOnlyTimeString, true);

			int32 IsOnlyTimeValue = FCString::Atoi(*IsOnlyTimeString);
			bool IsOnlyTime = (IsOnlyTimeValue != 0) ? true : false;

			RequestResetEventAttendanceClass<bool>::GetInstance().Signal(IsOnlyTime);
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("ResetDailyPlayTimeRecord")))
		{
			RequestResetDailyPlayTimeRecordClass<>::GetInstance().Signal();
			return true;
		}*/

		return false;
	}

	bool ExecHandler::ExecuteShellCommandGuildBattle(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("JoinMyGuildBattle")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::JoinGuildBattleMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseJoinGuildBattle> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestJoinGuildBattle>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetGuildBattle")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetGuildBattleMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetGuildBattle> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetGuildBattle>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("StartGuildBattle")))
		//{
		//	FString TargetAccountIDString;
		//	FParse::Token(Cmd, TargetAccountIDString, true);

		//	int64 TargetAccountID = FCString::Atoi64(*TargetAccountIDString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::StartGuildBattleMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseStartGuildBattle> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestStartGuildBattle>(EmptyMessage, TargetAccountID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("FinishGuildBattle")))
		//{
		//	FString TargetAccountIDString, BattleToken, ResultString, RoundWinCountString;
		//	FParse::Token(Cmd, TargetAccountIDString, true);
		//	FParse::Token(Cmd, BattleToken, true);
		//	FParse::Token(Cmd, ResultString, true);
		//	FParse::Token(Cmd, RoundWinCountString, true);
		//	
		//	int64 TargetAccountID = FCString::Atoi64(*TargetAccountIDString);
		//	int32 Result = FCString::Atoi(*ResultString);
		//	int32 RoundWinCount = FCString::Atoi(*RoundWinCountString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::FinishGuildBattleMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseFinishGuildBattle> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestFinishGuildBattle>(EmptyMessage, TargetAccountID, BattleToken, Result, RoundWinCount);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetMyGuildBattleEntry")))
		//{
		//	FString Entry1String, Entry2String, Entry3String, Entry4String, Entry5String, Entry6String, Entry7String, Entry8String;
		//	FParse::Token(Cmd, Entry1String, true);
		//	FParse::Token(Cmd, Entry2String, true);
		//	FParse::Token(Cmd, Entry3String, true);
		//	FParse::Token(Cmd, Entry4String, true);
		//	FParse::Token(Cmd, Entry5String, true);
		//	FParse::Token(Cmd, Entry6String, true);
		//	FParse::Token(Cmd, Entry7String, true);
		//	FParse::Token(Cmd, Entry8String, true);

		//	int32 Entry1 = FCString::Atoi(*Entry1String);
		//	int32 Entry2 = FCString::Atoi(*Entry2String);
		//	int32 Entry3 = FCString::Atoi(*Entry3String);
		//	int32 Entry4 = FCString::Atoi(*Entry4String);
		//	int32 Entry5 = FCString::Atoi(*Entry5String);
		//	int32 Entry6 = FCString::Atoi(*Entry6String);
		//	int32 Entry7 = FCString::Atoi(*Entry7String);
		//	int32 Entry8 = FCString::Atoi(*Entry8String);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetGuildBattleEntryMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetGuildBattleEntry> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetGuildBattleEntry>(EmptyMessage, 
		//		Entry1, Entry2, Entry3, Entry4, Entry5, Entry6, Entry7, Entry8);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildBattleTurnResult")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildBattleTurnResultMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildBattleTurnResult> (messageInfo);
		//		DeliveryGuildBattleTurnResultClass<FB2ResponseGuildBattleTurnResultPtr>::GetInstance().Signal(Message);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildBattleTurnResult>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildBattleTurnReward")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildBattleTurnRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildBattleTurnReward> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildBattleTurnReward>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildBattleSeasonReward")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildBattleSeasonRewardMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildBattleSeasonReward> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildBattleSeasonReward>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GuildBattleHistory")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GuildBattleHistoryMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGuildBattleHistory> (messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//	})
		//		);

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGuildBattleHistory>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetAccountCostume")))
		//{
		//	FString CharacterTypeString, InventoryTypeString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, InventoryTypeString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 InventoryType = FCString::Atoi(*InventoryTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("GetAccountCostume : CharacterType(%d), InventoryType(%d)"), CharacterType, InventoryType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetAccountCostumeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetAccountCostume>(messageInfo);

		//		for (b2network::B2CostumeServerInfoPtr costume : Message->costumes)
		//		{
		//			UE_LOG(LogBladeII, Log, TEXT("Complete get account costume. current CharacterType(%d), InventoryType(%d)"),
		//				costume->character_type, costume->inventory_type);
		//		}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetAccountCostume>(EmptyMessage, CharacterType, InventoryType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EquipCostume")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("EquipCostume : CostumeID(%I64d)"), CostumeID);

		//	TArray<int64> CostumeUIDList;
		//	CostumeUIDList.Emplace(CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EquipCostumeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEquipCostume>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEquipCostume>(EmptyMessage, CostumeUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnequipCostume")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("UnequipCostume : CostumeID(%I64d)"), CostumeID);

		//	TArray<int64> CostumeUIDList;
		//	CostumeUIDList.Emplace(CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UnequipCostumeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnequipCostume>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnequipCostume>(EmptyMessage, CostumeUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EnhanceCostume")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("EnhanceCostume : CostumeID(%I64d)"), CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EnhanceCostumeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEnhanceCostume>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEnhanceCostume>(EmptyMessage, CostumeID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DismantleCostumes")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("DismantleCostumes : CostumeID(%I64d)"), CostumeID);

		//	TArray<int64> CostumeUIDList;
		//	CostumeUIDList.Emplace(CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DismantleCostumesMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDismantleCostumes>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDismantleCostumes>(EmptyMessage, CostumeUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LockCostumes")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("LockCostumes : CostumeID(%I64d)"), CostumeID);

		//	TArray<int64> CostumeUIDList;
		//	CostumeUIDList.Emplace(CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::LockCostumesMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLockCostumes>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLockCostumes>(EmptyMessage, CostumeUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnlockCostumes")))
		//{
		//	FString CostumeUIDString;
		//	FParse::Token(Cmd, CostumeUIDString, true);

		//	int64 CostumeID = FCString::Atoi64(*CostumeUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("UnlockCostumes : CostumeID(%I64d)"), CostumeID);

		//	TArray<int64> CostumeUIDList;
		//	CostumeUIDList.Emplace(CostumeID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UnlockCostumesMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnlockCostumes>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnlockCostumes>(EmptyMessage, CostumeUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("GetCostumeShop")))
		//{
		//	
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetCostumeShopMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetCostumeShop>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetCostumeShop>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("PurchaseCostume")))
		//{
		//	FString ProductUIDString;
		//	FParse::Token(Cmd, ProductUIDString, true);

		//	int32 ProductID = FCString::Atoi(*ProductUIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("PurchaseCostume : ProductID(%I64d)"), ProductID);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::PurchaseCostumeMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponsePurchaseCostume>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestPurchaseCostume>(EmptyMessage, ProductID);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SetCostumeVisible")))
		//{
		//	FString CharacterTypeString, CostumeTypeString, isVisibleString;
		//	FParse::Token(Cmd, CharacterTypeString, true);
		//	FParse::Token(Cmd, CostumeTypeString, true);
		//	FParse::Token(Cmd, isVisibleString, true);

		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);
		//	int32 CostumeType = FCString::Atoi(*CostumeTypeString);
		//	int32 isVisibleValue = FCString::Atoi(*isVisibleString);

		//	bool isVisible = (isVisibleValue != 0) ? true : false;

		//	UE_LOG(LogBladeII, Log, TEXT("SetCostumeVisible : CharacterType(%d) CostumeType(%d) isVisibleValue(%d)"), CharacterType, CostumeType, isVisibleValue);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SetCostumeVisibleMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSetCostumeVisible>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSetCostumeVisible>(EmptyMessage, CharacterType, CostumeType, isVisible);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if(FParse::Command(&Cmd, TEXT("AddCostume")))		// AddCostume templateId
		//{
		//	FString TemplateIDString;
		//	FParse::Token(Cmd, TemplateIDString, true);

		//	int32 TemplateID = FCString::Atoi(*TemplateIDString);

		//	UE_LOG(LogBladeII, Log, TEXT("AddCostumeToInventory : TemplateID(%d)"), TemplateID);

		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(TemplateID);

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("TestDemoAddCostume")))
		//{
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3000200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3001200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3002200);

		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3100200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3101200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3102200);

		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3200200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3201200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3202200);

		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3300200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3301200);
		//	RequestAddCostumeToInventoryClass<int32>::GetInstance().Signal(3302200);

		//	return true;
		//}
		//// totem
		//else if (FParse::Command(&Cmd, TEXT("GetAccountTotem")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::GetAccountTotemMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseGetAccountTotem>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestGetAccountTotem>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("EquipTotems")))
		//{
		//	FString TotemIdString, CharacterTypeString;
		//	FParse::Token(Cmd, TotemIdString, true);
		//	FParse::Token(Cmd, CharacterTypeString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);
		//	int32 CharacterType = FCString::Atoi(*CharacterTypeString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("EquipTotems : TotemId(%I64d), CharacterType(%d)"), TotemId, CharacterType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::EquipTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseEquipTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestEquipTotems>(EmptyMessage, TotemUIDList, CharacterType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnequipTotems")))
		//{
		//	FString TotemIdString;
		//	FParse::Token(Cmd, TotemIdString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("UnequipTotems : TotemId(%I64d)"), TotemId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UnequipTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnequipTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnequipTotems>(EmptyMessage, TotemUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LockTotems")))
		//{
		//	FString TotemIdString;
		//	FParse::Token(Cmd, TotemIdString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("LockTotems : TotemId(%I64d)"), TotemId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::LockTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseLockTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestLockTotems>(EmptyMessage, TotemUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("UnlockTotems")))
		//{
		//	FString TotemIdString;
		//	FParse::Token(Cmd, TotemIdString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("UnlockTotems : TotemId(%I64d)"), TotemId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::UnlockTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseUnlockTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestUnlockTotems>(EmptyMessage, TotemUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("SellTotems")))
		//{
		//	FString TotemIdString;
		//	FParse::Token(Cmd, TotemIdString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("SellTotems : TotemId(%I64d)"), TotemId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::SellTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseSellTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestSellTotems>(EmptyMessage, TotemUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("DismantleTotems")))
		//{
		//	FString TotemIdString;
		//	FParse::Token(Cmd, TotemIdString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);

		//	TArray<int64> TotemUIDList;
		//	TotemUIDList.Emplace(TotemId);

		//	UE_LOG(LogBladeII, Log, TEXT("DismantleTotems : TotemId(%I64d)"), TotemId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::DismantleTotemsMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseDismantleTotems>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestDismantleTotems>(EmptyMessage, TotemUIDList);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("RefineTotem")))
		//{
		//	FString TotemIdString, OptionIdString, CostTypeString;
		//	FParse::Token(Cmd, TotemIdString, true);
		//	FParse::Token(Cmd, OptionIdString, true);
		//	FParse::Token(Cmd, CostTypeString, true);

		//	int64 TotemId = FCString::Atoi64(*TotemIdString);
		//	int32 OptionId = FCString::Atoi(*OptionIdString);
		//	int32 CostType = FCString::Atoi(*CostTypeString);

		//	UE_LOG(LogBladeII, Log, TEXT("RefineTotem : TotemId(%I64d), OptionId(%d), CostType(%d)"), TotemId, OptionId, CostType);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::RefineTotemMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseRefineTotem>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestRefineTotem>(EmptyMessage, TotemId, OptionId, CostType);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("ModifyTotemNewFlag")))
		//{
		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ModifyTotemNewFlagMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseModifyTotemNewFlag>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestModifyTotemNewFlag>(EmptyMessage);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("AddTotem")))
		//{
		//	FString TemplateIdString;
		//	FParse::Token(Cmd, TemplateIdString, true);

		//	int32 TemplateId = FCString::Atoi64(*TemplateIdString);

		//	UE_LOG(LogBladeII, Log, TEXT("AddTotem : TemplateId(%d)"), TemplateId);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::AddTotemToInventoryMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseAddTotemToInventory>(messageInfo);
		//		if (Message)
		//		{
		//			DeliveryAddTotemToInventoryClass<FB2AddTotemToInventoryPtr>::GetInstance().Signal(Message);
		//		}
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestAddTotemToInventory>(EmptyMessage, TemplateId);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandKakaoPlatform(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("KakaoMessage")))
		//{
		//	FString MessageBoxId;
		//	FParse::Token(Cmd, MessageBoxId, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::KakaoMessageProcessingMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseKakaoMessageProcessing>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestKakaoMessageProcessing>(EmptyMessage, MessageBoxId);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("KakaoCouponUse")))
		//{
		//	FString CouponCode;
		//	FParse::Token(Cmd, CouponCode, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::KakaoCouponUseMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseKakaoCouponUse>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestKakaoCouponUse>(EmptyMessage, CouponCode);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("KakaoAccountConversionPrepare")))
		//{
		//	FString IdpCode;
		//	FParse::Token(Cmd, IdpCode, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::KakaoAccountConversionPrepareMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseKakaoAccountConversionPrepare>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestKakaoAccountConversionPrepare>(EmptyMessage, IdpCode);
		//	Message->SendMessage();

		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("KakaoAccountConversionComplete")))
		//{
		//	FString IdpCode;
		//	FParse::Token(Cmd, IdpCode, true);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::KakaoAccountConversionCompleteMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoResponseKakaoAccountConversionComplete>(messageInfo);
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestKakaoAccountConversionComplete>(EmptyMessage, IdpCode);
		//	Message->SendMessage();

		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandChannelWorld(const TCHAR* Cmd)
	{
		//if (FParse::Command(&Cmd, TEXT("ReportUserChannel")))
		//{
		//	FString TargetAccountIdString, ModeString, ReasonString, Explain;
		//	FParse::Token(Cmd, TargetAccountIdString, true);
		//	FParse::Token(Cmd, ModeString, true);
		//	FParse::Token(Cmd, ReasonString, true);
		//	FParse::Token(Cmd, Explain, true);

		//	int64 TargetAccountId = FCString::Atoi64(*TargetAccountIdString);
		//	int32 ModeType = FCString::Atoi(*ModeString);
		//	int32 ReasonType = FCString::Atoi(*ReasonString);

		//	UE_LOG(LogBladeII, Log, TEXT("ReportUserChannel : TargetAccountId(%I64d), ModeType(%d), ReasonType(%d), Explain(%s)"), 
		//		TargetAccountId, ModeType, ReasonType, *Explain);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::ReportUserViaChannelMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoReportUserViaChannelResponse>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete ReportUserChannel."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoReportUserViaChannelRequest>(EmptyMessage, 
		//		TargetAccountId, ModeType, ReasonType, Explain);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("JoinWorld")))
		//{
		//	FString WorldIdString;
		//	FParse::Token(Cmd, WorldIdString, true);

		//	UE_LOG(LogBladeII, Log, TEXT("JoinWorld : WorldId(%d)"), *WorldIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::WorldJoinMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoWorldJoinResponse>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete WorldJoin."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoWorldJoinRequest>(EmptyMessage, WorldIdString);
		//	Message->SendMessage();
		//	return true;
		//}
		//else if (FParse::Command(&Cmd, TEXT("LeaveWorld")))
		//{
		//	FString WorldIdString;
		//	FParse::Token(Cmd, WorldIdString, true);

		//	UE_LOG(LogBladeII, Log, TEXT("LeaveWorld : WorldId(%d)"), *WorldIdString);

		//	auto EmptyMessage = EmptyMessagePtr(new data_trader::WorldLeaveMessage(
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		auto Message = std::static_pointer_cast<b2network::FB2MessageInfoWorldLeaveResponse>(messageInfo);

		//		UE_LOG(LogBladeII, Log, TEXT("Complete WorldLeave."));
		//	},
		//		[](FB2MessageInfoPtr messageInfo)
		//	{
		//		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		//		// TODO error code
		//	}
		//	));

		//	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoWorldLeaveRequest>(EmptyMessage, WorldIdString);
		//	Message->SendMessage();
		//	return true;
		//}

		return false;
	}

	bool ExecHandler::ExecuteShellCommandServerCmd(const TCHAR* Cmd)
	{
		//FString CommandStr(Cmd);

		//RequestServerCmdClass<const FString&>::GetInstance().Signal(CommandStr);
		return true;
	}

	bool ExecHandler::ExecuteShellCommand(const TCHAR* Cmd)
	{
		/*if (ExecuteShellCommandAccount(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandNetWork(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandEtc(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandItem(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandMatch(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandStage(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandHeroTower(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandCounter(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandDimension(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandCharacter(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandSkill(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandMail(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandFriend(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandQuest(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandGuild(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandMission(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandShop(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandCollection(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandAttendance(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandGuildBattle(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandKakaoPlatform(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandChannelWorld(Cmd))
		{
			return true;
		}
		else if (ExecuteShellCommandServerCmd(Cmd))
		{
			return true;
		}*/

		return false;
	}

	OnlineWholesaler::OnlineWholesaler()
	{
		SubscribeInterested();
	}

	OnlineWholesaler::~OnlineWholesaler()
	{
		Issues.Empty();
	}

	template<typename EmptyMessageType, typename MessageInfoType>
	struct PostMessageHelper
	{
		template<typename... Args>
		PostMessageHelper(Args... args)
		{
			auto EmptyMessage = EmptyMessagePtr(new EmptyMessageType(
				HandlerBank::GetGlobalHandler(),
				HandlerBank::GetGlobalHandler()
				));

			auto Message = FillOutMessageInfo<MessageInfoType>(EmptyMessage, args...);
			Message->SendMessage();
		}
	};

	void OnlineWholesaler::SubscribeInterested()
	{
		//Issues.Add(NoAccountClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		RequestNicknameClass<>::GetInstance().Signal();
		//	}
		//));
		//Issues.Add(RequestGiveNicknameClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& Nickname)
		//	{
		//		PostMessageHelper<GiveNicknameMessage, b2network::FB2MessageInfoRequestGiveNickname>		message(Nickname);
		//	}
		//));
		//Issues.Add(RequestUpdateNicknameClass<const FString&, uint32>::GetInstance().Subscribe2(
		//	[](const FString& Nickname, int32 ItemTemplateID)
		//	{
		//		PostMessageHelper<UpdateNicknameMessage, b2network::FB2MessageInfoRequestUpdateNickname>		message(Nickname, ItemTemplateID);
		//	}
		//));
		//Issues.Add(RequestGetAccountClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetAccountMessage, b2network::FB2MessageInfoRequestGetAccount>	message{};
		//	}
		//));
		//Issues.Add(RequestGetAccountItemClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 inventoryType)
		//	{
		//		PostMessageHelper<GetAccountItemMessage, b2network::FB2MessageInfoRequestGetAccountItem>	message(characterType, inventoryType);
		//	}
		//));
		//Issues.Add(RequestGetServerVersionClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetServerVersionMessage, b2network::FB2MessageInfoRequestGetServerVersion>	message{};
		//}
		//));
		//Issues.Add(RequestGetMasterDataClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetMasterDataMessage, b2network::FB2MessageInfoRequestGetMasterData>	message{};
		//	}
		//));
		//Issues.Add(RequestGetLatestEventDataClass<int64>::GetInstance().Subscribe2(
		//	[](int64 lastUpdateTime)
		//{
		//	PostMessageHelper<GetLatestEventDataMessage, b2network::FB2MessageInfoRequestGetLatestEventData> message(lastUpdateTime);
		//}
		//));
		//Issues.Add(RequestAddItemClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 uid, int32 quality, int32 amount)
		//	{
		//		PostMessageHelper<AddItemMessage, b2network::FB2MessageInfoRequestAddItemToInventory>	message(uid, quality, amount);
		//	}
		//));
		//Issues.Add(RequestAddCostumeToInventoryClass<int32>::GetInstance().Subscribe2(
		//	[](int32 templateId)
		//{
		//	PostMessageHelper<AddCostumeMessage, b2network::FB2MessageInfoRequestAddCostumeToInventory>	message(templateId);
		//}
		//));
		//Issues.Add(RequestDismantleItemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& items)
		//{
		//	PostMessageHelper<DismantleItemsMessage, b2network::FB2MessageInfoRequestDismantleItems>	message(items);
		//}
		//));
		//Issues.Add(RequestSellItemClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& items)
		//{
		//	PostMessageHelper<SellItemMessage, b2network::FB2MessageInfoRequestSellItem>	message(items);
		//}
		//));
		//Issues.Add(RequestSellConsumableItemClass<int64, int32>::GetInstance().Subscribe2(
		//	[](int64 itemId, int32 sellCount)
		//{
		//	PostMessageHelper<SellConsumableItemMessage, b2network::FB2MessageInfoRequestSellConsumableItem>	message(itemId, sellCount);
		//}
		//));
		//Issues.Add(RequestConvertDataClass<int32>::GetInstance().Subscribe2(
		//	[](int32 templateId)
		//{
		//	PostMessageHelper<ConvertDataMessage, b2network::FB2MessageInfoRequestConvertData>	message(templateId);
		//}
		//));
		//Issues.Add(RequestConvertItemClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 itemId, int32 convertType)
		//{
		//	PostMessageHelper<ConvertItemMessage, b2network::FB2MessageInfoRequestConvertItem>	message(itemId, convertType);
		//}
		//));
		//Issues.Add(RequestEquipItemClass<int32, const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](int32 preset_id, const TArray<int64>& items)
		//	{
		//		PostMessageHelper<EquipItemMessage, b2network::FB2MessageInfoRequestEquipItem>		message(preset_id, items);
		//	}
		//));
		//Issues.Add(RequestUnequipItemClass<int32, const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](int32 preset_id, const TArray<int64>& items)
		//	{
		//		PostMessageHelper<UnequipItemMessage, b2network::FB2MessageInfoRequestUnequipItem>	message(preset_id, items);
		//	}
		//));
		//Issues.Add(RequestLockItemClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& items)
		//	{
		//		PostMessageHelper<LockItemMessage, b2network::FB2MessageInfoRequestLockItem>	message(items);
		//	}
		//));
		//Issues.Add(RequestUnlockItemClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& items)
		//	{
		//		PostMessageHelper<UnlockItemMessage, b2network::FB2MessageInfoRequestUnlockItem>	message(items);
		//	}
		//));
		//Issues.Add(RequestEnhanceItemClass<int64, int32, bool, int32>::GetInstance().Subscribe2(
		//	[](int64 main, int32 enhanceScrollTID, bool useProtection, int32 protectionScrollTID)
		//{
		//	PostMessageHelper<EnhanceItemMessage, b2network::FB2MessageInfoRequestEnhanceItem>	message(main, enhanceScrollTID, useProtection, protectionScrollTID);
		//}
		//));
		//Issues.Add(RequestExchangeItemEnhanceLevelClass<int64, int64, int32>::GetInstance().Subscribe2(
		//	[](const int64 item_id_1, const int64 item_id_2, const int32 price_type)
		//{
		//	PostMessageHelper<ExchangeItemEnhanceLevelMessage, b2network::FB2MessageInfoRequestExchangeItemEnhanceLevel> message(item_id_1, item_id_2, price_type);
		//}
		//));
		//Issues.Add(RequestOpenSealSlotItemClass<int64, int32>::GetInstance().Subscribe2(
		//	[](const int64 item_id, const int32 seal_slot)
		//{
		//	PostMessageHelper<OpenSealSlotItemMessage, b2network::FB2MessageInfoRequestOpenSealSlotItem> message(item_id, seal_slot);
		//}
		//));
		//Issues.Add(RequestCarveSealItemClass<int64, int32>::GetInstance().Subscribe2(
		//	[](const int64 item_id, const int32 seal_slot)
		//{
		//	PostMessageHelper<CarveSealItemMessage, b2network::FB2MessageInfoRequestCarveSealItem> message(item_id, seal_slot);
		//}
		//));
		//Issues.Add(RequestLevelupItemClass<int64, const TArray<int64>&, bool>::GetInstance().Subscribe2(
		//	[](int64 main, const TArray<int64>& materials, bool autoEnhance)
		//	{
		//		PostMessageHelper<LevelupItemMessage, b2network::FB2MessageInfoRequestLevelupItem>	message(main, materials, autoEnhance);
		//	}
		//));
		//Issues.Add(RequestGetItemSurpassCostClass<int64>::GetInstance().Subscribe2(
		//	[](int64 uid)
		//	{
		//		PostMessageHelper<GetItemSurpassCostMessage, b2network::FB2MessageInfoRequestGetItemSurpassCost> message(uid);
		//	}
		//));
		//Issues.Add(RequestSurpassItemClass<int64, int64>::GetInstance().Subscribe2(
		//	[](int64 main, int64 material)
		//	{
		//		PostMessageHelper<SurpassItemMessage, b2network::FB2MessageInfoRequestSurpassItem> message(main, material);
		//	}
		//));
		//Issues.Add(RequestGetItemUpgradeCostClass<int64>::GetInstance().Subscribe2(
		//	[](int64 uid)
		//	{
		//		PostMessageHelper<GetItemUpgradeCostMessage, b2network::FB2MessageInfoRequestGetItemUpgradeCost> message(uid);
		//	}
		//));
		//Issues.Add(RequestUpgradeItemClass<int64>::GetInstance().Subscribe2(
		//	[](int64 item)
		//	{
		//		PostMessageHelper<UpgradeItemMessage, b2network::FB2MessageInfoRequestUpgradeItem> message(item);
		//	}
		//));
		//Issues.Add(RequestChangeItemCharacterTypeClass<int64, int32>::GetInstance().Subscribe2(
		//	[](int64 item, int32 characterType)
		//{
		//	PostMessageHelper<ChangeItemCharacterTypeMessage, b2network::FB2MessageInfoRequestChangeItemCharacterType> message(item, characterType);
		//}
		//));
		//Issues.Add(RequestGetChangeItemCharacterTypeCostClass<int64>::GetInstance().Subscribe2(
		//	[](int64 item)
		//{
		//	PostMessageHelper<GetChangeItemCharacterTypeCostMessage, b2network::FB2MessageInfoRequestGetChangeItemCharacterTypeCost> message(item);
		//}
		//));
		//Issues.Add(RequestGetItemComposeCostClass<int64, int32>::GetInstance().Subscribe2(
		//	[](int64 uid, int32 presetId)
		//	{
		//		PostMessageHelper<GetItemComposeCostMessage, b2network::FB2MessageInfoRequestGetItemComposeCost> message(uid, presetId);
		//	}
		//));
		//Issues.Add(RequestComposeItemClass<int64, int64, int32>::GetInstance().Subscribe2(
		//	[](int64 main, int64 sub, int32 presetId)
		//	{
		//		PostMessageHelper<ComposeItemMessage, b2network::FB2MessageInfoRequestComposeItem> message(main, sub, presetId);
		//	}
		//));
		//Issues.Add(RequestExtendInventoryClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 CharacterType, int32 ExtendCount)
		//	{
		//		PostMessageHelper<ExtendInventoryMessage, b2network::FB2MessageInfoRequestExtendInventory> message(CharacterType, ExtendCount);
		//	}
		//));
		//Issues.Add(RequestStartStageClass<FServerStageID, int32, int32, int32, int32, bool, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](FServerStageID stageid, int32 main, int32 tag, int32 option, int32 repeatMode, bool boostMode, int32 tutorialId, int32 mainPresetId, int32 tagPresetId)
		//	{
		//		PostMessageHelper<StartStageMessage, b2network::FB2MessageInfoRequestStartStage> message(stageid.Id, main, tag, option, repeatMode, boostMode, tutorialId, mainPresetId, tagPresetId);
		//	}
		//));
		//Issues.Add(RequestResurrectionClass<const FString&, int32>::GetInstance().Subscribe2(
		//	[](const FString& token, int32 selected_buff_index)
		//	{
		//		PostMessageHelper<ResurrectionMessage, b2network::FB2MessageInfoRequestResurrection>	message(token, selected_buff_index);
		//	}
		//));
		//
		//Issues.Add(RequestClearStageClass<FServerStageID, const FString&, int32, int32, int32, const TArray<b2network::B2KillMonsterInfoPtr>&, int32, bool, int32, int32, b2network::B2ItemSpotSaleOptionPtr, b2network::B2AetherSpotSaleOptionPtr>::GetInstance().Subscribe2(
		//	[](FServerStageID stageid, const FString& token, int32 main, int32 tag, int32 cleartime, const TArray<b2network::B2KillMonsterInfoPtr>& killMonsterInfo, int32 totalMonsterKillCount, bool isAllSurvival, 
		//		int32 qteStartCount, int32 qteFinishCount, b2network::B2ItemSpotSaleOptionPtr b2SpotSaleOptionPtr, b2network::B2AetherSpotSaleOptionPtr b2AetherSpotSaleOptionPtr)
		//	{
		//		PostMessageHelper<ClearStageMessage, b2network::FB2MessageInfoRequestClearStage>	message(stageid.Id, token, main, tag, cleartime, killMonsterInfo, totalMonsterKillCount, isAllSurvival, qteStartCount, qteFinishCount, b2SpotSaleOptionPtr, b2AetherSpotSaleOptionPtr);
		//	}
		//));

		//Issues.Add(RequestFailStageClass<FServerStageID, const FString&, int32, int32>::GetInstance().Subscribe2(
		//	[](FServerStageID stageid, const FString& token, int32 failReason, int32 playTimeSec)
		//	{
		//		PostMessageHelper<FailStageMessage, b2network::FB2MessageInfoRequestFailStage>	message(stageid.Id, token, failReason, playTimeSec);
		//	}
		//));

		//Issues.Add(RequestSweepStageClass<FServerStageID, int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](FServerStageID stageid, int32 main, int32 tag, int32 mainPresetId, int32 tagPresetId, int32 repeatCount)
		//{
		//	PostMessageHelper<SweepStageMessage, b2network::FB2MessageInfoRequestSweepStage> message(stageid.Id, main, tag, mainPresetId, tagPresetId, repeatCount);
		//}
		//));

		//Issues.Add(RequestCheckBladePointChargeClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<CheckBladePointChargeMessage, b2network::FB2MessageInfoRequestCheckBladePointCharge>	message{};
		//	}
		//));
		//Issues.Add(RequestGetActInfoClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 difficulty, int32 actId)
		//	{
		//		PostMessageHelper<GetActInfoMessage, b2network::FB2MessageInfoRequestGetActInfo>	message(difficulty, actId);
		//	}
		//));
		//Issues.Add(RequestGetStageInfoClass<FServerStageID>::GetInstance().Subscribe2(
		//	[](FServerStageID ServerStageId)
		//	{
		//		PostMessageHelper<GetStageInfoMessage, b2network::FB2MessageInfoRequestGetStageInfo>	message(ServerStageId.Id);
		//	}
		//));

		//Issues.Add(RequestRaidMatchmakingClass<int32, int32, int32, int32, b2network::B2EndpointPtr>::GetInstance().Subscribe2(
		//	[](int32 match_type, int32 character_type, int32 raid_type, int32 raid_step, b2network::B2EndpointPtr endpoint)
		//	{
		//		PostMessageHelper<RaidMatchmakingMessage, b2network::FB2MessageInfoRaidMatchmakingRequest> message(match_type, character_type, raid_type, raid_step, endpoint);
		//	}
		//));

		//Issues.Add(RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance().Subscribe2(
		//	[](int32 type, int32 mainCharacter, int32 tagCharacter, bool useAdditionalCount, const FString& roomSeed)
		//	{
		//		PostMessageHelper<MatchmakingMessage, b2network::FB2MessageInfoMatchmakingRequest>	message(type, mainCharacter, tagCharacter, useAdditionalCount, roomSeed);
		//	}
		//));
		//Issues.Add(RequestJoinRoomClass<const FString&, int32, int32, TArray<b2network::B2RoomPropertyPtr>&, TArray<b2network::B2RoomCharacterPtr>&, TArray<b2network::B2PresetNumPtr>&>::GetInstance().Subscribe2(
		//	[](const FString& roomId, int32 roomType, int32 roomAccess, TArray<b2network::B2RoomPropertyPtr>& properties, TArray<b2network::B2RoomCharacterPtr>& characters,
		//		TArray<b2network::B2PresetNumPtr>& presetNums)
		//	{
  //              PostMessageHelper<RoomJoinMessage, b2network::FB2MessageInfoRoomJoinRequest> message(roomId, roomType, roomAccess, properties, characters, presetNums);
		//	}
		//));

		//Issues.Add(RequestRoomPropertyClass<const FString&, TArray<b2network::B2RoomPropertyPtr>&>::GetInstance().Subscribe2(
		//	[](const FString& RoomId, TArray<b2network::B2RoomPropertyPtr>& PropertyList)
		//{
		//	PostMessageHelper<RoomProperty, b2network::FB2MessageInfoRoomSetPropertyRequest> message(RoomId, PropertyList);
		//}
		//));

		//Issues.Add(RequestLeaveRoomClass<const FString&>::GetInstance().Subscribe2(
  //          [](const FString& roomId)
  //          {
  //              PostMessageHelper<RoomLeaveMessage, b2network::FB2MessageInfoRoomLeaveRequest> message(roomId);
  //          }
  //      ));
		//Issues.Add(RequestRoomInvitationClass<const FString&, const FString&, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](const FString& room_id,  const FString& invitee, int32 invitation_number, int32 mapId, int32 difficulty)
		//	{
		//		PostMessageHelper<RoomInvitationMessage, b2network::FB2MessageInfoRoomInviteRequest> message(room_id, invitee, invitation_number, mapId, difficulty);
		//	}	
		//));
		//Issues.Add(RequestMatchResultClass<b2network::B2MatchResult, int32, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr,
		//	b2network::B2RoomCharacterPtr, const FString&, bool>::GetInstance().Subscribe2(
		//	[](const b2network::B2MatchResult& My, int32 matchType, b2network::B2RoomCharacterPtr myMain,
		//		b2network::B2RoomCharacterPtr myTag, b2network::B2RoomCharacterPtr opponentMain, b2network::B2RoomCharacterPtr opponentTag,
		//		const FString& roomId, bool isGiveUp)
		//	{
		//		auto MyResultPtr = std::make_shared<b2network::B2MatchResult>();
		//		*MyResultPtr.get() = My;
		//		PostMessageHelper<MatchResultMessage, b2network::FB2MessageInfoMatchResultRequest> message(MyResultPtr, matchType, myMain, myTag,
		//			opponentMain, opponentTag, roomId, isGiveUp);
		//	}
		//));
		//Issues.Add(SendRoomMessageClass<const FString&, int32, const FString&>::GetInstance().Subscribe2(
		//	[](const FString& roomId, int32 receiverId, const FString& Body)
		//	{
		//		PostMessageHelper<RoomMessageCommand, b2network::FB2MessageInfoRoomMessageCommand> message(roomId, receiverId, Body);
		//	}
		//));

		//Issues.Add(RequestRoomCloseClass<const FString&, int32, int32>::GetInstance().Subscribe2(
		//	[](const FString& roomId, int32 mapId, int32 reason)
		//{
		//	PostMessageHelper<RoomCloseMessage, b2network::FB2MessageInfoRoomCloseRequest> message(roomId, mapId, reason);
		//}
		//));

		//Issues.Add(RequestRoomLeaveResultClass<const FString&, int64, const FString&>::GetInstance().Subscribe2(
		//	[](const FString& roomId, int64 leaveAccountId, const FString& matchToken)
		//{
		//	PostMessageHelper<RoomLeaveResultMessage, b2network::FB2MessageInfoRoomLeaveResultRequest> message(roomId, leaveAccountId, matchToken);
		//}
		//));


		//Issues.Add(RequestRoomAccessControlClass<const FString&, int32>::GetInstance().Subscribe2(
		//	[](const FString& roomId, int32 accessKey)
		//{
		//	PostMessageHelper<RoomAccessControl, b2network::FB2MessageInfoRoomAccessControlRequest> message(roomId, accessKey);
		//}
		//));

		//Issues.Add(SendChatMessageClass<int32, const FString&, int32, const FString&, int32>::GetInstance().Subscribe2(
		//	[](int32 type, const FString& to, int32 format, const FString& body, int32 characterLevelMax)
		//	{
		//		if (type == (int32)EB2ChatType::Guild)
		//		{
		//			PostMessageHelper<GuildChatMessage, b2network::FB2MessageInfoRequestGuildChat> message(format, body, characterLevelMax);
		//		}
		//		else
		//		{
		//			PostMessageHelper<ChatCommand, b2network::FB2MessageInfoChatCommand> message(type, to, format, body, characterLevelMax);
		//		}
		//	}
		//));
		//Issues.Add(RequestLevelUpSkillClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 charType, int32 skillId)
		//	{
		//		PostMessageHelper<LevelUpSkillMessage, b2network::FB2MessageInfoRequestLevelUpSkill> message(charType, skillId);
		//	}
		//));
		//Issues.Add(RequestResetSkillClass<int32>::GetInstance().Subscribe2(
		//	[](int32 charType)
		//	{
		//		PostMessageHelper<ResetSkillMessage, b2network::FB2MessageInfoRequestResetSkill> message(charType);
		//	}
		//));
		//Issues.Add(RequestBuySkillPointClass<const TArray<b2network::B2BuySkillPointRequestInfoPtr>&>::GetInstance().Subscribe2(
		//	[](const TArray<b2network::B2BuySkillPointRequestInfoPtr>& buyInfos)
		//	{
		//		PostMessageHelper<BuySkillPointMessage, b2network::FB2MessageInfoRequestBuySkillPoint> message(buyInfos);
		//	}
		//));
		//Issues.Add(RequestSetUsingSkillClass<int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 charType, int32 presetType, int32 usingSkill1, int32 usingSkill2, int32 usingSkill3)
		//	{
		//		PostMessageHelper<SetUsingSkillMessage, b2network::FB2MessageInfoRequestSetUsingSkill> message(charType, presetType, usingSkill1, usingSkill2, usingSkill3);
		//	}
		//));
		//Issues.Add(RequestReceiveActClearRewardClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 difficulty, int32 actId, int32 rewardIndex)
		//	{
		//		PostMessageHelper<ReceiveActClearRewardMessage, b2network::FB2MessageInfoRequestReceiveActClearReward> message(difficulty, actId, rewardIndex);
		//	}
		//));
		//Issues.Add(RequestGetMatchStatusClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetMatchStatusMessage, b2network::FB2MessageInfoRequestGetMatchStatus> message{};
		//	}
		//));
		//Issues.Add(RequestCancelMatchmakingClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<CancelMatchmakingMessage, b2network::FB2MessageInfoCancelMatchmakingRequest> message{};
		//}
		//));
		//Issues.Add(RequestGetTeamBattleStatusClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetTeamBattleStatusMessage, b2network::FB2MessageInfoRequestGetTeamBattleStatus> message{};
		//	}
		//));
		//Issues.Add(RequestUpdateTeamBattleEntryClass<b2network::B2TeamBattleEntryPtr>::GetInstance().Subscribe2(
		//	[](b2network::B2TeamBattleEntryPtr entryPtr)
		//	{
		//		PostMessageHelper<UpdateTeamBattleEntryMessage, b2network::FB2MessageInfoRequestUpdateTeamBattleEntry> message(entryPtr);
		//	}
		//));
		//Issues.Add(RequestEnhanceTeamBattleFormationClass<int32>::GetInstance().Subscribe2(
		//	[](int32 formationType)
		//	{
		//		PostMessageHelper<EnhanceTeamBattleFormationMessage, b2network::FB2MessageInfoRequestEnhanceTeamBattleFormation> message(formationType);
		//	}
		//));
		//Issues.Add(RequestTeamMatchmakingClass<bool>::GetInstance().Subscribe2(
		//	[](bool useAdditionalCount)
		//	{
		//		PostMessageHelper<TeamMatchmakingMessage, b2network::FB2MessageInfoRequestTeamMatchmaking> message(useAdditionalCount);
		//	}
		//));
		//Issues.Add(RequestTeamMatchResultClass<const FString&, int32, bool>::GetInstance().Subscribe2(
		//	[](const FString& matchToken, int32 matchResult, bool isGiveUp)
		//	{
		//		PostMessageHelper<TeamMatchResultMessage, b2network::FB2MessageInfoRequestTeamMatchResult> message(matchToken, matchResult, isGiveUp);
		//	}
		//));
		//Issues.Add(RequestEnhanceRankNodeClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//{
		//	PostMessageHelper<EnhanceRankNodeMessage, b2network::FB2MessageInfoRequestEnhanceRankNode> message(characterType);
		//}
		//));
		//Issues.Add(RequestSetCharNickNameClass<int32, const FString&>::GetInstance().Subscribe2(
		//	[](int32 PCClassIndex, const FString& InNickName)
		//	{
		//		PostMessageHelper<GiveCharacterNameMessage, b2network::FB2MessageInfoRequestGiveCharacterName> message(PCClassIndex, InNickName);
		//	}
		//));
		//Issues.Add(RequestGetMailListClass<int32>::GetInstance().Subscribe2(
		//	[](int32 mailCategory)
		//	{
		//		PostMessageHelper<GetMailListMessage, b2network::FB2MessageInfoRequestGetMailList> message(mailCategory);
		//	}
		//));
		//Issues.Add(RequestOpenMailClass<int64, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int64 mailId, int32 lottery_character_type, int32 lottery_item_template_id, int32 lottery_aether_type)
		//	{
		//		PostMessageHelper<OpenMailMessage, b2network::FB2MessageInfoRequestOpenMail> message(mailId, lottery_character_type, lottery_item_template_id, lottery_aether_type);
		//	}
		//));
		//Issues.Add(RequestOpenAllMailClass<int32, int64, int64>::GetInstance().Subscribe2(
		//	[](int32 mailCategory, int64 startId, int64 endId)
		//	{
		//		PostMessageHelper<OpenAllMailMessage, b2network::FB2MessageInfoRequestOpenAllMail> message(mailCategory, startId, endId);
		//	}
		//));
		//Issues.Add(RequestGetNewMailCountClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetNewMailCountMessage, b2network::FB2MessageInfoRequestGetNewMailCount> message{};
		//	}
		//));
		//Issues.Add(RequestAddMailClass<int32, int32, int32, int32, int32, int32, int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 mailCategory, int32 presentAttachType, int32 placeType, int32 senderType, int32 amount, int32 itemTemplateId, int32 generalLotteryId, int32 fixedGradeLotteryId, int32 selectiveLotteryId, int32 selectiveItemLotteryId, int32 prefixSelectiveLotteryId)
		//	{
		//		PostMessageHelper<AddMailMessage, b2network::FB2MessageInfoRequestAddMail> message(mailCategory, presentAttachType, placeType, senderType,
		//			amount, itemTemplateId, generalLotteryId, fixedGradeLotteryId, selectiveLotteryId, selectiveItemLotteryId, prefixSelectiveLotteryId);
		//	}
		//));
		//Issues.Add(RequestAskFriendClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<AskFriendMessage, b2network::FB2MessageInfoRequestAskFriend> message(account_id);
		//	}
		//));
		//Issues.Add(RequestAcceptAskFriendClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<AcceptAskFriendMessage, b2network::FB2MessageInfoRequestAcceptAskFriend> message(account_id);
		//	}
		//));
		//Issues.Add(RequestRejectAskFriendClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<RejectAskFriendMessage, b2network::FB2MessageInfoRequestRejectAskFriend> message(account_id);
		//	}
		//));
		//Issues.Add(RequestGetFriendListClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetFriendListMessage, b2network::FB2MessageInfoRequestGetFriendList> message{};
		//	}
		//));
		//Issues.Add(RequestCancelAskFriendClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<CancelAskFriendMessage, b2network::FB2MessageInfoRequestCancelAskFriend> message(account_id);
		//	}
		//));
		//Issues.Add(RequestDeleteFriendClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<DeleteFriendMessage, b2network::FB2MessageInfoRequestDeleteFriend> message(account_id);
		//	}
		//));
		//Issues.Add(RequestGetAllSendAskFriendClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetAllSendAskFriendMessage, b2network::FB2MessageInfoRequestGetAllSendAskFriend> message{};
		//	}
		//));
		//Issues.Add(RequestGetAllReceiveAskFriendClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetAllReceiveAskFriendMessage, b2network::FB2MessageInfoRequestGetAllReceiveAskFriend> message{};
		//	}
		//));
		//Issues.Add(RequestSendSocialPointClass<TArray<int64>>::GetInstance().Subscribe2(
		//	[](TArray<int64> account_ids)
		//	{
		//		PostMessageHelper<SendSocialPointMessage, b2network::FB2MessageInfoRequestSendSocialPoint> message(account_ids);
		//	}
		//));
		//Issues.Add(RequestConfirmNewFriendClass<bool, TArray<int64>>::GetInstance().Subscribe2(
		//	[](bool is_confirm_all, TArray<int64> friend_ids)
		//	{
		//		PostMessageHelper<ConfirmNewFriendMessage, b2network::FB2MessageInfoRequestConfirmNewFriend> message(is_confirm_all, friend_ids);
		//	}
		//));
		//Issues.Add(RequestGetRecommendFriendClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetRecommendFriendMessage, b2network::FB2MessageInfoRequestGetRecommendFriend> message{};
		//	}
		//));
		//Issues.Add(RequestPlatformFriendInviteInfoClass<int32>::GetInstance().Subscribe2(
		//	[](int32 PlatformType)
		//{
		//	PostMessageHelper<PlatformFriendInviteInfoMessage, b2network::FB2MessageInfoRequestPlatformFriendInviteInfo> message(PlatformType);
		//}
		//));
		//Issues.Add(RequestPlatformFriendInviteCountUpClass<int32>::GetInstance().Subscribe2(
		//	[](int32 PlatformType)
		//{
		//	PostMessageHelper<PlatformFriendInviteCountUpMessage, b2network::FB2MessageInfoRequestPlatformFriendInviteCountUp> message(PlatformType);
		//}
		//));
		//Issues.Add(RequestPlatformFriendJoinerCountUpClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 PlatformType, int32 JoinerCount)
		//{
		//	PostMessageHelper<PlatformFriendJoinerCountUpMessage, b2network::FB2MessageInfoRequestPlatformFriendJoinerCountUp> message(PlatformType, JoinerCount);
		//}
		//));
		//Issues.Add(RequestPlatformFriendInviteRewardClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 PlatformType, int32 RewardType, int32 RewardIndex)
		//{
		//	PostMessageHelper<PlatformFriendInviteRewardMessage, b2network::FB2MessageInfoRequestPlatformFriendInviteReward> message(PlatformType, RewardType, RewardIndex);
		//}
		//));
		//Issues.Add(RequestFindUserClass<const FString&, bool>::GetInstance().Subscribe2(
		//	[](const FString& Nickname, bool guild_invite_state)
		//	{
		//		PostMessageHelper<FindUserMessage, b2network::FB2MessageInfoRequestFindUser>		message(Nickname, guild_invite_state);
		//	}
		//));
		//Issues.Add(RequestGetChatChannelClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetChatChannelMessage, b2network::FB2MessageInfoGetChatChannelRequest> message{};
		//	}
		//));
		//Issues.Add(RequestChangeChatChannelClass<int32>::GetInstance().Subscribe2(
		//	[](int32 RequestChannelNum)
		//	{
		//		PostMessageHelper<ChangeChatChannelMessage, b2network::FB2MessageInfoChangeChatChannelRequest> message{ RequestChannelNum };
		//	}
		//));
		//Issues.Add(RequestGetQuestClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetQuestMessage, b2network::FB2MessageInfoRequestGetQuest> message{};
		//	}
		//));
		//Issues.Add(RequestAcceptQuestClass<int32>::GetInstance().Subscribe2(
		//	[](int32 slot)
		//	{
		//		PostMessageHelper<AcceptQuestMessage, b2network::FB2MessageInfoRequestAcceptQuest> message(slot);
		//	}
		//));
		//Issues.Add(RequestReceiveQuestRewardClass<int32>::GetInstance().Subscribe2(
		//	[](int32 slot)
		//	{
		//		PostMessageHelper<ReceiveQuestRewardMessage, b2network::FB2MessageInfoRequestReceiveQuestReward> message(slot);
		//	}
		//));
		//Issues.Add(RequestGetCounterDungeonClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetCounterDungeonMessage, b2network::FB2MessageInfoRequestGetCounterDungeon> message{};
		//	}
		//));
		//Issues.Add(RequestStartCounterDungeonClass<FServerStageID, int32>::GetInstance().Subscribe2(
		//	[](FServerStageID dungeon_id, int32 main_character_type)
		//	{
		//		PostMessageHelper<StartCounterDungeonMessage, b2network::FB2MessageInfoRequestStartCounterDungeon> message(dungeon_id.Id, main_character_type);
		//	}
		//));
		//Issues.Add(RequestForceStartCounterDungeonClass<FServerStageID>::GetInstance().Subscribe2(
		//	[](FServerStageID dungeon_id)
		//{
		//	PostMessageHelper<ForceStartCounterDungeonMessage, b2network::FB2MessageInfoRequestForceStartCounterDungeon> message(dungeon_id.Id);
		//}
		//));
		//Issues.Add(RequestFinishCounterDungeonClass<FServerStageID, const FString&, bool, bool>::GetInstance().Subscribe2(
		//	[](FServerStageID ServerStageId, const FString& play_token, bool is_success, bool isGiveUp)
		//	{
		//		PostMessageHelper<FinishCounterDungeonMessage, b2network::FB2MessageInfoRequestFinishCounterDungeon> message(ServerStageId.Id, play_token, is_success, isGiveUp);
		//	}
		//));
		//Issues.Add(RequestSweepCounterDungeonClass<FServerStageID, int32>::GetInstance().Subscribe2(
		//	[](FServerStageID dungeon_id, int32 main_character_type)
		//{
		//	PostMessageHelper<SweepCounterDungeonMessage, b2network::FB2MessageInfoRequestSweepCounterDungeon> message(dungeon_id.Id, main_character_type);
		//}
		//));
		//Issues.Add(RequestBuyCounterDungeonTicketClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<BuyCounterDungeonTicketMessage, b2network::FB2MessageInfoRequestBuyCounterDungeonTicket> message{};
		//	}
		//));
		//Issues.Add(RequestCounterDungeonWeeklyRewardClass<int32>::GetInstance().Subscribe2(
		//	[](int32 clear_count)
		//	{
		//		PostMessageHelper<CounterDungeonWeeklyRewardMessage, b2network::FB2MessageInfoRequestCounterDungeonWeeklyReward> message(clear_count);
		//	}
		//));
		//Issues.Add(RequestGetDimensionClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetDimensionMessage, b2network::FB2MessageInfoRequestGetDimension> message{};
		//	}
		//));
		//Issues.Add(RequestStartDimensionClass<int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 difficultyLevel, int32 dungeonId, int32 MainSvrClass, int32 totalPower)
		//	{
		//		PostMessageHelper<StartDimensionMessage, b2network::FB2MessageInfoRequestStartDimension> message(difficultyLevel, dungeonId, MainSvrClass, totalPower);
		//	}
		//));
		//Issues.Add(RequestFinishDimensionClass<int32, const FString&, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 difficultyLevel, const FString& playToken, int32 playResult, int32 playTimeMillis)
		//	{
		//		PostMessageHelper<FinishDimensionMessage, b2network::FB2MessageInfoRequestFinishDimension> message(difficultyLevel, playToken, playResult, playTimeMillis);
		//	}
		//));
		//Issues.Add(RequestCreateGuildClass<const FString&, int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](const FString& guild_name, int32 join_option, int32 mark_index, int32 mark_color, int32 mark_bg_index, int32 mark_bg_color)
		//	{
		//		PostMessageHelper<CreateGuildMessage, b2network::FB2MessageInfoRequestCreateGuild> message(guild_name, join_option, mark_index, mark_color, mark_bg_index, mark_bg_color);
		//	}
		//));
		//Issues.Add(RequestAskJoinGuildClass<int64>::GetInstance().Subscribe2(
		//	[](int64 guild_id)
		//	{
		//		PostMessageHelper<AskJoinGuildMessage, b2network::FB2MessageInfoRequestAskJoinGuild> message(guild_id);
		//	}
		//));
		//Issues.Add(RequestLeaveGuildClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<LeaveGuildMessage, b2network::FB2MessageInfoRequestLeaveGuild> message{};
		//	}
		//));
		//Issues.Add(RequestGetGuildDetailInfoClass<int64>::GetInstance().Subscribe2(
		//	[](int64 guild_id)
		//	{
		//		PostMessageHelper<GetGuildDetailInfoMessage, b2network::FB2MessageInfoRequestGetGuildDetailInfo> message(guild_id);
		//	}
		//));
		//Issues.Add(RequestDeleteGuildClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<DeleteGuildMessage, b2network::FB2MessageInfoRequestDeleteGuild> message{};
		//	}
		//));
		//Issues.Add(RequestGetRecommendGuildClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetRecommendGuildMessage, b2network::FB2MessageInfoRequestGetRecommendGuild> message{};
		//	}
		//));
		//Issues.Add(RequestAppointSubGuildMasterClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<AppointSubGuildMasterMessage, b2network::FB2MessageInfoRequestAppointSubGuildMaster> message(account_id);
		//	}
		//));
		//Issues.Add(RequestDismissSubGuildMasterClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<DismissSubGuildMasterMessage, b2network::FB2MessageInfoRequestDismissSubGuildMaster> message(account_id);
		//	}
		//));
		//Issues.Add(RequestKickGuildMemberClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<KickGuildMemberMessage, b2network::FB2MessageInfoRequestKickGuildMember> message(account_id);
		//	}
		//));
		//Issues.Add(RequestUpdateGuildClass<int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 join_option, int32 mark_index, int32 mark_color, int32 mark_bg_index, int32 mark_bg_color)
		//	{
		//		PostMessageHelper<UpdateGuildMessage, b2network::FB2MessageInfoRequestUpdateGuild> message(join_option, mark_index, mark_color, mark_bg_index, mark_bg_color);
		//	}
		//));
		//Issues.Add(RequestUpdateGuildNoticeClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& notice)
		//	{
		//		PostMessageHelper<UpdateGuildNoticeMessage, b2network::FB2MessageInfoRequestUpdateGuildNotice> message(notice);
		//	}
		//));
		//Issues.Add(RequestGetGuildMemberCandidatesClass<bool>::GetInstance().Subscribe2(
		//	[](bool needFriend)
		//	{
		//		PostMessageHelper<GetGuildMemberCandidatesMessage, b2network::FB2MessageInfoRequestGetGuildMemberCandidates> message(needFriend);
		//	}
		//));
		//Issues.Add(RequestInviteGuildClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//	{
		//		PostMessageHelper<InviteGuildMessage, b2network::FB2MessageInfoRequestInviteGuild> message(account_id);
		//	}
		//));
		//Issues.Add(RequestGetGuildInviteListClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetGuildInviteListMessage, b2network::FB2MessageInfoRequestGetGuildInviteList> message{};
		//	}
		//));
		//Issues.Add(RequestRejectInviteGuildClass<int64>::GetInstance().Subscribe2(
		//	[](int64 guild_id)
		//	{
		//		PostMessageHelper<RejectInviteGuildMessage, b2network::FB2MessageInfoRequestRejectInviteGuild> message(guild_id);
		//	}
		//));
		//Issues.Add(RequestAcceptInviteGuildClass<int64>::GetInstance().Subscribe2(
		//	[](int64 guild_id)
		//	{
		//		PostMessageHelper<AcceptInviteGuildMessage, b2network::FB2MessageInfoRequestAcceptInviteGuild> message(guild_id);
		//	}
		//));
		//Issues.Add(RequestFindGuildClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& guild_name)
		//	{
		//		PostMessageHelper<FindGuildMessage, b2network::FB2MessageInfoRequestFindGuild> message(guild_name);
		//	}
		//));
		//Issues.Add(RequestObtainGuildMasterClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<ObtainGuildMasterMessage, b2network::FB2MessageInfoRequestObtainGuildMaster> message{};
		//	}
		//));
		//Issues.Add(RequestHandOverGuildMasterClass<int64>::GetInstance().Subscribe2(
		//	[](int64 account_id)
		//{
		//	PostMessageHelper<HandOverGuildMasterMessage, b2network::FB2MessageInfoRequestHandOverGuildMaster> message(account_id);
		//}
		//));
		//Issues.Add(RequestGetGuildRankingListClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetGuildRankingListMessage, b2network::FB2MessageInfoRequestGetGuildRankingList> message{};
		//	}
		//));
		//Issues.Add(RequestLevelUpGuildSkillClass<int32>::GetInstance().Subscribe2(
		//	[](int32 skill_id)
		//	{
		//		PostMessageHelper<LevelUpGuildSkillMessage, b2network::FB2MessageInfoRequestLevelUpGuildSkill> message(skill_id);
		//	}
		//));
		//Issues.Add(RequestBuyGuildSkillClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 skill_id, int32 buy_count)
		//	{
		//		PostMessageHelper<BuyGuildSkillMessage, b2network::FB2MessageInfoRequestBuyGuildSkill> message(skill_id, buy_count);
		//	}
		//));
		//Issues.Add(RequestGuildDonationClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 mercenary_id, int32 donation_type)
		//	{
		//		PostMessageHelper<GuildDonationMessage, b2network::FB2MessageInfoRequestGuildDonation> message(mercenary_id, donation_type);
		//	}
		//));
		//Issues.Add(RequestGetMissionClass<int32>::GetInstance().Subscribe2(
		//	[](int32 missionType)
		//	{
		//		PostMessageHelper<GetMissionMessage, b2network::FB2MessageInfoRequestGetMission> message(missionType);
		//	}
		//));
		//Issues.Add(RequestGetFeaturedMissionClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetFeaturedMissionMessage, b2network::FB2MessageInfoRequestGetFeaturedMission> message{};
		//	}
		//));
		//Issues.Add(RequestReceiveMissionRewardClass<int32>::GetInstance().Subscribe2(
		//	[](int32 missionId)
		//	{
		//		PostMessageHelper<ReceiveMissionRewardMessage, b2network::FB2MessageInfoRequestReceiveMissionReward> message(missionId);
		//	}
		//));
		//Issues.Add(RequestReceiveCompleteMissionRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 missionType, int32 rewardIndex)
		//	{
		//		PostMessageHelper<ReceiveCompleteMissionRewardMessage, b2network::FB2MessageInfoRequestReceiveCompleteMissionReward> message(missionType, rewardIndex);
		//	}
		//));
		//Issues.Add(RequestGetWingClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//	{
		//		PostMessageHelper<GetWingMessage, b2network::FB2MessageInfoRequestGetWing> message(characterType);
		//	}
		//));
		//Issues.Add(RequestEnhanceWingClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//	{
		//		PostMessageHelper<EnhanceWingMessage, b2network::FB2MessageInfoRequestEnhanceWing> message(characterType);
		//	}
		//));
		//Issues.Add(RequestUpgradeWingClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//	{
		//		PostMessageHelper<UpgradeWingMessage, b2network::FB2MessageInfoRequestUpgradeWing> message(characterType);
		//	}
		//));
		//Issues.Add(RequestEnhanceWingOptionClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 optionIndex)
		//	{
		//		PostMessageHelper<EnhanceWingOptionMessage, b2network::FB2MessageInfoRequestEnhanceWingOption> message(characterType, optionIndex);
		//	}
		//));
		//Issues.Add(RequestGetDonationEventClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetDonationEventMessage, b2network::FB2MessageInfoRequestGetDonationEvent> message{};
		//	}	
		//));
		//Issues.Add(RequestMakeDonationClass<int32, bool, bool, bool, const FString&>::GetInstance().Subscribe2(
		//	[](int32 donationId, bool isBundle, bool isAgain, bool isFree, const FString& lastDonationKey)
		//	{
		//		PostMessageHelper<MakeDonationMessage, b2network::FB2MessageInfoRequestMakeDonation> message(donationId, isBundle, isAgain, isFree, lastDonationKey);
		//	}
		//));
		//Issues.Add(RequestDonationPointRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 donationId, int32 step)
		//	{
		//		PostMessageHelper<DonationPointRewardMessage, b2network::FB2MessageInfoRequestDonationPointReward> message(donationId, step);
		//	}
		//));
		//Issues.Add(RequestGetFairyClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetFairyMessage, b2network::FB2MessageInfoRequestGetFairy> message{};
		//}
		//));
		//Issues.Add(RequestLevelupFairyClass<int32>::GetInstance().Subscribe2(
		//	[](int32 fairyType)
		//{
		//	PostMessageHelper<LevelupFairyMessage, b2network::FB2MessageInfoRequestLevelupFairy> message(fairyType);
		//}
		//));
		//Issues.Add(RequestBlessFairyClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 fairyType, int32 blessOptionId)
		//{
		//	PostMessageHelper<BlessFairyMessage, b2network::FB2MessageInfoRequestBlessFairy> message(fairyType, blessOptionId);
		//}
		//));
		//Issues.Add(RequestReceiveFairyGiftClass<int32>::GetInstance().Subscribe2(
		//	[](int32 fairyType)
		//{
		//	PostMessageHelper<ReceiveFairyGiftMessage, b2network::FB2MessageInfoRequestReceiveFairyGift> message(fairyType);
		//}
		//));
		//Issues.Add(RequestGetHeroTowerClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetHeroTowerMessage, b2network::FB2MessageInfoRequestGetHeroTower> message{};
		//	}
		//));
		//Issues.Add(RequestStartHeroTowerClass<int32, int32, int32, const FString&>::GetInstance().Subscribe2(
		//	[](int32 floor, int32 main_character_type, int32 tag_character_type, const FString& play_token)
		//	{
		//		PostMessageHelper<StartHeroTowerMessage, b2network::FB2MessageInfoRequestStartHeroTower> message(floor, main_character_type, tag_character_type, play_token);
		//	}
		//));
		//Issues.Add(RequestFinishHeroTowerClass<int32, const FString&, int32, bool>::GetInstance().Subscribe2(
		//	[](int32 floor, const FString& play_token, int32 clear_time_sec, bool is_success)
		//	{
		//		PostMessageHelper<FinishHeroTowerMessage, b2network::FB2MessageInfoRequestFinishHeroTower> message(floor, play_token, clear_time_sec, is_success);
		//	}
		//));
		//Issues.Add(RequestAbandonHeroTowerClass<int32, const FString&>::GetInstance().Subscribe2(
		//	[](int32 floor, const FString& play_token)
		//{
		//	PostMessageHelper<AbandonHeroTowerMessage, b2network::FB2MessageInfoRequestAbandonHeroTower> message(floor, play_token);
		//}
		//));
		//Issues.Add(RequestSweepHeroTowerClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<SweepHeroTowerMessage, b2network::FB2MessageInfoRequestSweepHeroTower> message{};
		//	}
		//));
		//Issues.Add(RequestBuyHeroTowerTicketClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<BuyHeroTowerTicketMessage, b2network::FB2MessageInfoRequestBuyHeroTowerTicket> message{};
		//}
		//));
		//Issues.Add(RequestEnhanceRelicClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 character_type, int32 relic_id)
		//	{
		//		PostMessageHelper<EnhanceRelicMessage, b2network::FB2MessageInfoRequestEnhanceRelic> message(character_type, relic_id);
		//	}
		//));
		//Issues.Add(RequestPromotionRelicClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 character_type, int32 relic_id)
		//	{
		//		PostMessageHelper<PromotionRelicMessage, b2network::FB2MessageInfoRequestPromotionRelic> message(character_type, relic_id);
		//	}
		//));
		//Issues.Add(RequestGetAccountRelicClass<int32>::GetInstance().Subscribe2(
		//	[](int32 character_type)
		//	{
		//		PostMessageHelper<GetAccountRelicMessage, b2network::FB2MessageInfoRequestGetAccountRelic> message(character_type);
		//	}
		//));
		//Issues.Add(RequestDrawShopLotteryClass < int32, int32, bool, bool, const FString&, bool > ::GetInstance().Subscribe2(
		//	[](int32 character_type, int32 draw_price_type, bool continuous_draw, bool isDrawAgain, const FString& lastLotteryKey, const bool isFreeLottery)
		//	{
		//		PostMessageHelper<DrawShopLotteryMessage, b2network::FB2MessageInfoRequestDrawShopLottery> message(character_type,
		//			draw_price_type, continuous_draw, isDrawAgain, lastLotteryKey, isFreeLottery);
		//	}
		//));
		//Issues.Add(RequestRewardShopMileageClass<int32>::GetInstance().Subscribe2(
		//	[](int32 shop_mileage_type)
		//	{
		//		PostMessageHelper<RewardShopMileageMessage, b2network::FB2MessageInfoRequestRewardShopMileage> message(shop_mileage_type);
		//	}
		//));
		//Issues.Add(RequestGetLotteryShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetLotteryShopMessage, b2network::FB2MessageInfoRequestGetLotteryShop> message{};
		//	}
		//));
		//Issues.Add(RequestBuyShopProductClass<int32>::GetInstance().Subscribe2(
		//	[](int32 productId)
		//	{
		//		PostMessageHelper<BuyShopProductMessage, b2network::FB2MessageInfoRequestBuyShopProduct> message(productId);
		//	}
		//));
		//Issues.Add(RequestGetGeneralShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetGeneralShopMessage, b2network::FB2MessageInfoRequestGetGeneralShop> message{};
		//	}
		//));
		//Issues.Add(RequestGetMagicShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetMagicShopMessage, b2network::FB2MessageInfoRequestGetMagicShop> message{};
		//}
		//));
		//Issues.Add(RequestRenewalMagicShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<RenewalMagicShopMessage, b2network::FB2MessageInfoRequestRenewalMagicShop> message{};
		//}
		//));
		//Issues.Add(RequestPurchaseMagicShopClass<int32>::GetInstance().Subscribe2(
		//	[](int32 magicShopItemId)
		//{
		//	PostMessageHelper<PurchaseMagicShopMessage, b2network::FB2MessageInfoRequestPurchaseMagicShop> message(magicShopItemId);
		//}
		//));
		//Issues.Add(RequestGetMarketProductClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetMarketProductMessage, b2network::FB2MessageInfoRequestGetMarketProduct> message{};
		//	}
		//));
		//Issues.Add(RequestPreparePurchaseClass<int32, const FString&>::GetInstance().Subscribe2(
		//	[](int32 product_id, const FString& market_product_id)
		//{
		//	PostMessageHelper<PreparePurchaseMessage, b2network::FB2MessageInfoRequestPreparePurchase> message(product_id, market_product_id);
		//}
		//));
		//Issues.Add(RequestVerifyPurchaseClass<const FString&, const FString&, const FString&, const FString&, bool, const FString&, float>::GetInstance().Subscribe2(
		//	[](const FString& market_product_id, const FString& developer_payload, const FString& purchase_data, const FString& purchase_signature, bool is_restore, const FString& currencyCode, float price)
		//{
		//	PostMessageHelper<VerifyPurchaseMessage, b2network::FB2MessageInfoRequestVerifyPurchase> message(market_product_id, developer_payload, purchase_data, purchase_signature, is_restore, currencyCode, price);
		//}
		//));
		//Issues.Add(RequestCancelPurchaseClass<const FString&, const FString&, int32>::GetInstance().Subscribe2(
		//	[](const FString& market_product_id, const FString& developer_payload, int32 cancel_reason)
		//{
		//	PostMessageHelper<CancelPurchaseMessage, b2network::FB2MessageInfoRequestCancelPurchase> message(market_product_id, developer_payload, cancel_reason);
		//}
		//));
		//Issues.Add(RequestFinishPurchaseClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& developer_payload)
		//{
		//	PostMessageHelper<FinishPurchaseMessage, b2network::FB2MessageInfoRequestFinishPurchase> message(developer_payload);
		//}
		//));
		//Issues.Add(RequestGetCollectionItemClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 equipCategory)
		//	{
		//		PostMessageHelper<GetCollectionItemMessage, b2network::FB2MessageInfoRequestGetCollectionItem> message(characterType, equipCategory);
		//	}
		//));
		//Issues.Add(RequestRewardCollectionItemClass<int32>::GetInstance().Subscribe2(
		//	[](int32 itemTemplateId)
		//	{
		//		PostMessageHelper<RewardCollectionItemMessage, b2network::FB2MessageInfoRequestRewardCollectionItem> message(itemTemplateId);
		//	}
		//));
		//Issues.Add(RequestRewardCollectionSetItemClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 grade, int32 groupId)
		//	{
		//		PostMessageHelper<RewardCollectionSetItemMessage, b2network::FB2MessageInfoRequestRewardCollectionSetItem> message(characterType, grade, groupId);
		//	}
		//));
		//Issues.Add(RequestRaidResultClass<const FString&, int32, bool, bool, bool>::GetInstance().Subscribe2(
		//	[](const FString& token, int32 ranking, bool isGetBonus, bool isReward, bool isGiveUp)
		//	{
		//		PostMessageHelper<RaidResultMessage, b2network::FB2MessageInfoRequestRaidResult> message(token, ranking, isGetBonus, isReward, isGiveUp);
		//	}
		//));
		//Issues.Add(RequestDailyAttendanceClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<DailyAttendanceMessage, b2network::FB2MessageInfoRequestDailyAttendance> message{}; 
		//	}
		//));
		//Issues.Add(RequestJoinGuildBattleClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<JoinGuildBattleMessage, b2network::FB2MessageInfoRequestJoinGuildBattle> message{};
		//	}
		//));
		//Issues.Add(RequestGetGuildBattleClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetGuildBattleMessage, b2network::FB2MessageInfoRequestGetGuildBattle> message{};
		//	}
		//));
		//Issues.Add(RequestStartGuildBattleClass<int64>::GetInstance().Subscribe2(
		//	[](int64 target_account_id)
		//	{
		//		PostMessageHelper<StartGuildBattleMessage, b2network::FB2MessageInfoRequestStartGuildBattle> message(target_account_id);
		//	}
		//));
		//Issues.Add(RequestFinishGuildBattleClass<int64, const FString&, int32, int32>::GetInstance().Subscribe2(
		//	[](int64 target_account_id, const FString& battle_token, int32 result, int32 round_win_count)
		//	{
		//		PostMessageHelper<FinishGuildBattleMessage, b2network::FB2MessageInfoRequestFinishGuildBattle> message(target_account_id, battle_token, result, round_win_count);
		//	}
		//));
		//Issues.Add(RequestSetGuildBattleEntryClass<int32, int32, int32, int32, int32, int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 entry1, int32 entry2, int32 entry3, int32 entry4, int32 entry5, int32 entry6, int32 entry7, int32 entry8)
		//	{
		//		PostMessageHelper<SetGuildBattleEntryMessage, b2network::FB2MessageInfoRequestSetGuildBattleEntry> message(
		//			entry1, entry2, entry3, entry4, entry5, entry6, entry7, entry8);
		//	}
		//));
		//Issues.Add(RequestGuildBattleTurnResultClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GuildBattleTurnResultMessage, b2network::FB2MessageInfoRequestGuildBattleTurnResult> message{};
		//	}
		//));
		//Issues.Add(RequestGuildBattleTurnRewardClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GuildBattleTurnRewardMessage, b2network::FB2MessageInfoRequestGuildBattleTurnReward> message{};
		//	}
		//));
		//Issues.Add(RequestGuildBattleSeasonRewardClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GuildBattleSeasonRewardMessage, b2network::FB2MessageInfoRequestGuildBattleSeasonReward> message{};
		//	}
		//));
		//Issues.Add(RequestGuildBattleHistoryClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GuildBattleHistoryMessage, b2network::FB2MessageInfoRequestGuildBattleHistory> message{};
		//	}
		//));
		//Issues.Add(RequestSetWingVisibleClass<int32, bool>::GetInstance().Subscribe2(
		//	[](int32 character_type, bool isVisible)
		//	{
		//		PostMessageHelper<SetWingVisibleMessage, b2network::FB2MessageInfoRequestSetWingVisible> message(character_type, isVisible);
		//	}
		//));
		//Issues.Add(RequestSetHelmetVisibleClass<int32, bool>::GetInstance().Subscribe2(
		//	[](int32 character_type, bool isVisible)
		//{
		//	PostMessageHelper<SetHelmetVisibleMessage, b2network::FB2MessageInfoRequestSetHelmetVisible> message(character_type, isVisible);
		//}
		//));
		//Issues.Add(RequestGetRaidClass<>::GetInstance().Subscribe2(
		//	[]()
		//	{
		//		PostMessageHelper<GetRaidMessage, b2network::FB2MessageInfoRequestGetRaid> message{};
		//	}
		//));
		//Issues.Add(RequestRaidResurrectionClass<const FString&, int32>::GetInstance().Subscribe2(
		//	[](const FString& token, int32 selectedBuffIndex)
		//{
		//	PostMessageHelper<RaidResurrectionMessage, b2network::FB2MessageInfoRequestRaidResurrection> message(token, selectedBuffIndex);
		//} 
		//));
		//Issues.Add(RequestResetAttendanceTimeClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ResetAttendanceTimeMessage, b2network::FB2MessageInfoRequestResetAttendanceTime> message{};
		//}
		//));
		//Issues.Add(RequestResetEventAttendanceClass<bool>::GetInstance().Subscribe2(
		//	[](bool isOnlyAttendanceTime)
		//{
		//	PostMessageHelper<ResetEventAttendanceMessage, b2network::FB2MessageInfoRequestResetEventAttendance> message(isOnlyAttendanceTime);
		//}
		//));
		//Issues.Add(RequestResetDailyPlayTimeRecordClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ResetDailyPlayTimeRecordMessage, b2network::FB2MessageInfoRequestResetDailyPlayTimeRecord> message{};
		//}
		//));
		//Issues.Add(RequestResetTeamMatchPointClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ResetTeamMatchPointMessage, b2network::FB2MessageInfoRequestResetTeamMatchPoint> message{};
		//}
		//));
		//Issues.Add(RequestChangeQuestIdClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 quest_slot, int32 quest_id)
		//{
		//	PostMessageHelper<ChangeQuestIdMessage, b2network::FB2MessageInfoRequestChangeQuestId> message(quest_slot, quest_id);
		//}
		//));
		//Issues.Add(RequestChangeQuestCountClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 quest_slot, int32 count)
		//{
		//	PostMessageHelper<ChangeQuestCountMessage, b2network::FB2MessageInfoRequestChangeQuestCount> message(quest_slot, count);
		//}
		//));
		//Issues.Add(RequestAssaultMatchmakingClass<int32>::GetInstance().Subscribe2(
		//	[](int32 character_type)
		//{
		//	PostMessageHelper<AssaultMatchmakingMessage, b2network::FB2MessageInfoRequestAssaultMatchmaking> message(character_type);
		//}
		//));
		//Issues.Add(RequestAssaultResultClass<const FString&, int32, bool, bool, b2network::B2AssaultBattleInfoPtr>::GetInstance().Subscribe2(
		//	[](const FString token, int32 matchResult, bool isMvpPlayer, bool isGhostUser, b2network::B2AssaultBattleInfoPtr battleInfo)
		//{
		//	PostMessageHelper<AssaultResultMessage, b2network::FB2MessageInfoRequestAssaultResult> message(token, matchResult, isMvpPlayer, isGhostUser, battleInfo);
		//}
		//));
		//Issues.Add(RequestGetAssaultBattleStatusClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetAssaultBattleStatusMessage, b2network::FB2MessageInfoRequestGetAssaultBattleStatus> message{};
		//}
		//));
		//Issues.Add(RequestGetFriendRankingsClass<int32>::GetInstance().Subscribe2(
		//	[](int32 opponentType)
		//{
		//	PostMessageHelper<GetFriendRankingsMessage, b2network::FB2MessageInfoRequestGetFriendRankings> message(opponentType);
		//}
		//));
		//Issues.Add(RequestGetItemRandomOptionClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetItemRandomOptionMessage, b2network::FB2MessageInfoRequestGetItemRandomOption> message{};
		//}
		//));
		//Issues.Add(RequestGenerateItemRandomOptionClass<int64, int32>::GetInstance().Subscribe2(
		//	[](int64 itemId, int32 buyPriceType)
		//{
		//	PostMessageHelper<GenerateItemRandomOptionMessage, b2network::FB2MessageInfoRequestGenerateItemRandomOption> message(itemId, buyPriceType);
		//}
		//));
		//Issues.Add(RequestApplyItemRandomOptionClass<int64>::GetInstance().Subscribe2(
		//	[](int64 ItemId)
		//{
		//	PostMessageHelper<ApplyItemRandomOptionMessage,b2network::FB2MessageInfoRequestApplyItemRandomOption> message(ItemId);
		//}
		//));
		//Issues.Add(RequestDiscardItemRandomOptionClass<int64>::GetInstance().Subscribe2(
		//	[](int64 ItemId)
		//{
		//	PostMessageHelper<DiscardItemRandomOptionMessage,b2network::FB2MessageInfoRequestDiscardItemRandomOption> message(ItemId);
		//}
		//));
		//Issues.Add(RequestGetItemRandomQualityClass<int64>::GetInstance().Subscribe2(
		//	[](int64 ItemId)
		//{
		//	PostMessageHelper<GetItemRandomQualityMessage, b2network::FB2MessageInfoRequestGetItemRandomQuality> message(ItemId);
		//}
		//));
		//Issues.Add(RequestGenerateItemRandomQualityClass<int64, int64>::GetInstance().Subscribe2(
		//	[](int64 itemId, int64 materialItemId)
		//{
		//	PostMessageHelper<GenerateItemRandomQualityMessage, b2network::FB2MessageInfoRequestGenerateItemRandomQuality> message(itemId, materialItemId);
		//}
		//));
		//Issues.Add(RequestApplyItemRandomQualityClass<int64>::GetInstance().Subscribe2(
		//	[](int64 ItemId)
		//{
		//	PostMessageHelper<ApplyItemRandomQualityMessage, b2network::FB2MessageInfoRequestApplyItemRandomQuality> message(ItemId);
		//}
		//));
		//Issues.Add(RequestDiscardItemRandomQualityClass<int64>::GetInstance().Subscribe2(
		//	[](int64 ItemId)
		//{
		//	PostMessageHelper<DiscardItemRandomQualityMessage, b2network::FB2MessageInfoRequestDiscardItemRandomQuality> message(ItemId);
		//}
		//));
		//Issues.Add(RequestCheckRedDotClass<const TArray<int32>&>::GetInstance().Subscribe2(
		//	[](const TArray<int32>& hintList)
		//{
		//	PostMessageHelper<CheckRedDotMessage, b2network::FB2MessageInfoRequestCheckRedDot> message(hintList);
		//}
		//));
		//Issues.Add(RequestCheckChargePointsClass<bool, bool, bool>::GetInstance().Subscribe2(
		//	[](bool CheckBladePoint, bool CheckPvPMatchPoint, bool CheckTeamMatchPoint)
		//{
		//	PostMessageHelper<CheckChargePointsMessage, b2network::FB2MessageInfoRequestCheckChargePoints> message(CheckBladePoint, CheckPvPMatchPoint, CheckTeamMatchPoint);
		//}
		//));
		//Issues.Add(RequestIncreaseDarkCrystalPaidClass<int32>::GetInstance().Subscribe2(
		//	[](int32 DarkCrystalPaid)
		//{
		//	PostMessageHelper<IncreaseDarkCrystalPaidMessage,b2network::FB2MessageInfoRequestIncreaseDarkCrystalPaid> message(DarkCrystalPaid);
		//}
		//));
		//Issues.Add(RequestFindAccountClass<const TArray<b2network::B2FindAccountQueryPtr>&>::GetInstance().Subscribe2(
		//	[](const TArray<b2network::B2FindAccountQueryPtr>& FindAccountInfo)
		//{
		//	PostMessageHelper<FindAccountMessage, b2network::FB2MessageInfoRequestFindAccount> message(FindAccountInfo);
		//}
		//));
		//Issues.Add(RequestCheckPvPMatchPointChargeClass<>::GetInstance().Subscribe2(
		//		[]()
		//{
		//	PostMessageHelper<CheckPvPMatchPointChargeMessage, b2network::FB2MessageInfoRequestCheckPvPMatchPointCharge> message{};
		//}
		//));
		//Issues.Add(RequestCheckTeamMatchPointChargeClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<CheckTeamMatchPointChargeMessage, b2network::FB2MessageInfoRequestCheckTeamMatchPointCharge> message{};
		//}
		//));
		//Issues.Add(RequestModifyItemRandomOptionClass<int64, int32, int32, float>::GetInstance().Subscribe2(
		//	[](int64 ItemId, int32 SlotNum, int32 RandomOptionId, float RandomOptionValue)
		//{
		//	PostMessageHelper<ModifyItemRandomOptionMessage,b2network::FB2MessageInfoRequestModifyItemRandomOption> message(ItemId, SlotNum, RandomOptionId, RandomOptionValue);
		//}
		//));
		//Issues.Add(RequestModifyItemNewFlagClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 CharacterType, int32 InventoryType)
		//{
		//	PostMessageHelper<ModifyItemNewFlagMessage, b2network::FB2MessageInfoRequestModifyItemNewFlag> message(CharacterType, InventoryType);
		//}
		//));
		//Issues.Add(RequestModifyCostumeNewFlagClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 CharacterType, int32 InventoryType)
		//{
		//	PostMessageHelper<ModifyCostumeNewFlagMessage, b2network::FB2MessageInfoRequestModifyCostumeNewFlag> message(CharacterType, InventoryType);
		//}
		//));
		//Issues.Add(RequestGetContentsOpenStatusClass<int32>::GetInstance().Subscribe2(
		//	[](int32 linkKey)
		//{
		//	PostMessageHelper<GetContentsOpenStatusMessage,b2network::FB2MessageInfoRequestGetContentsOpenStatus> message(linkKey);
		//}
		//));
		//Issues.Add(RequestGetDuelRewardBoxInfoClass<int32>::GetInstance().Subscribe2(
		//	[](int32 DuelMode)
		//{
		//	PostMessageHelper<GetDuelRewardBoxInfoMessage, b2network::FB2MessageInfoRequestGetDuelRewardBoxInfo> message(DuelMode);
		//}
		//));
		//Issues.Add(RequestGetDuelRewardBoxGainableListClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 DuelMode, int32 BoxGrade)
		//{
		//	PostMessageHelper<GetDuelRewardBoxGainableListMessage, b2network::FB2MessageInfoRequestGetDuelRewardBoxGainableList> message(DuelMode, BoxGrade);
		//}
		//));
		//Issues.Add(RequestOpenDuelRewardBoxClass<int32>::GetInstance().Subscribe2(
		//	[](int32 DuelMode)
		//{
		//	PostMessageHelper<OpenDuelRewardBoxMessage, b2network::FB2MessageInfoRequestOpenDuelRewardBox> message(DuelMode);
		//}
		//));
		//Issues.Add(RequestEventAttendanceClass<int32>::GetInstance().Subscribe2(
		//	[](int32 eventId)
		//{
		//	PostMessageHelper<EventAttendanceMessage, b2network::FB2MessageInfoRequestEventAttendance> message(eventId);
		//}
		//));
		//Issues.Add(RequestBeginnerAttendanceClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<BeginnerAttendanceMessage, b2network::FB2MessageInfoRequestBeginnerAttendance> message{};
		//}
		//));
		//Issues.Add(RequestUpdateDailyPlayTimeClass<int32>::GetInstance().Subscribe2(
		//	[](int32 PlayTimeInSec)
		//{
		//	PostMessageHelper<UpdateDailyPlayTimeMessage, b2network::FB2MessageInfoRequestUpdateDailyPlayTime> message(PlayTimeInSec);
		//}
		//));
		//Issues.Add(RequestReceiveDailyPlayTimeRewardClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ReceiveDailyPlayTimeRewardMessage, b2network::FB2MessageInfoRequestReceiveDailyPlayTimeReward> message{};
		//}
		//));
		//Issues.Add(RequestAskGuildSupportClass<int32>::GetInstance().Subscribe2(
		//	[](int32 ItemTempleteId)
		//{
		//	PostMessageHelper<AskGuildSupportMessage, b2network::FB2MessageInfoRequestAskGuildSupport> message(ItemTempleteId);
		//}
		//));
		//Issues.Add(RequestGuildSupportClass<int64, int64, int32>::GetInstance().Subscribe2(
		//	[](int64 TargetAccountId, int64 ChatId, int32 Count)
		//{
		//	PostMessageHelper<GuildSupportMessage, b2network::FB2MessageInfoRequestGuildSupport> message(TargetAccountId, ChatId, Count);
		//}
		//));
		//Issues.Add(RequestGetGuildChatClass<int32>::GetInstance().Subscribe2(
		//	[](int32 ChatId)
		//{
		//	PostMessageHelper<GetGuildChatMessage, b2network::FB2MessageInfoRequestGetGuildChat> message(ChatId);
		//}
		//));
		//Issues.Add(RequestReceiveAssaultMvpRewardClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ReceiveAssaultMvpRewardMessage, b2network::FB2MessageInfoRequestReceiveAssaultMvpReward> message{};
		//}
		//));
		//Issues.Add(RequestGetAccountAetherClass<int32>::GetInstance().Subscribe2(
		//	[](int32 aetherType)
		//{
		//	PostMessageHelper<GetAccountAetherMessage, b2network::FB2MessageInfoRequestGetAccountAether> message(aetherType);
		//}
		//));
		//Issues.Add(RequestDismantleAethersClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids)
		//{
		//	PostMessageHelper<DismantleAethersMessage, b2network::FB2MessageInfoRequestDismantleAethers> message(aether_ids);
		//}
		//));
		//Issues.Add(RequestSellAethersClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids)
		//{
		//	PostMessageHelper<SellAethersMessage, b2network::FB2MessageInfoRequestSellAethers> message(aether_ids);
		//}
		//));
		//Issues.Add(RequestMountAethersClass<const TArray<int64>&, int32>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids, int32 characterType)
		//{
		//	PostMessageHelper<MountAethersMessage, b2network::FB2MessageInfoRequestMountAethers> message(aether_ids, characterType);
		//}
		//));
		//Issues.Add(RequestUnmountAethersClass<const TArray<int64>&, int32>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids, int32 characterType)
		//{
		//	PostMessageHelper<UnmountAethersMessage, b2network::FB2MessageInfoRequestUnmountAethers> message(aether_ids, characterType);
		//}
		//));
		//Issues.Add(RequestLockAethersClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids)
		//{
		//	PostMessageHelper<LockAethersMessage, b2network::FB2MessageInfoRequestLockAethers> message(aether_ids);
		//}
		//));
		//Issues.Add(RequestUnlockAethersClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& aether_ids)
		//{
		//	PostMessageHelper<UnlockAethersMessage, b2network::FB2MessageInfoRequestUnlockAethers> message(aether_ids);
		//}
		//));
		//Issues.Add(RequestExtendAetherInventoryClass<int32>::GetInstance().Subscribe2(
		//	[](int32 buyCount)
		//{
		//	PostMessageHelper<ExtendAetherInventoryMessage, b2network::FB2MessageInfoRequestExtendAetherInventory> message(buyCount);
		//}
		//));
		//Issues.Add(RequestEnhanceAetherClass<int64>::GetInstance().Subscribe2(
		//	[](int64 aetherId)
		//{
		//	PostMessageHelper<EnhanceAetherMessage, b2network::FB2MessageInfoRequestEnhanceAether> message(aetherId);
		//}
		//));
		//Issues.Add(RequestModifyAetherNewFlagClass<int32>::GetInstance().Subscribe2(
		//	[](int32 aetherType)
		//{
		//	PostMessageHelper<ModifyAetherNewFlagMessage, b2network::FB2MessageInfoRequestModifyAetherNewFlag> message(aetherType);
		//}
		//));
		//Issues.Add(RequestSelectAetherDefenseOptionClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 setOptionId, int32 characterType)
		//{
		//	PostMessageHelper<SelectAetherDefenseOptionMessage, b2network::FB2MessageInfoRequestSelectAetherDefenseOption> message(setOptionId, characterType);
		//}
		//));
		//Issues.Add(RequestRoomKickClass<const FString&, int64>::GetInstance().Subscribe2(
		//	[](const FString& RoomId, int64 AccountId)
		//{
		//	PostMessageHelper<RoomKickMessage, b2network::FB2MessageInfoRoomKickRequest> message(RoomId, AccountId);
		//}
		//));

		//Issues.Add(RequestGetTutorialProgressClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetTutorialProgressMessage,b2network::FB2MessageInfoRequestGetTutorialProgress> message{};
		//}
		//));
		//Issues.Add(RequestSetTutorialProgressClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<SetTutorialProgressMessage,b2network::FB2MessageInfoRequestSetTutorialProgress> message{};
		//}
		//));
		//Issues.Add(RequestUpdateTutorialProgressClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<UpdateTutorialProgressMessage,b2network::FB2MessageInfoRequestUpdateTutorialProgress> message{};
		//}
		//));
		//Issues.Add(RequestSkipTutorialProgressClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<SkipTutorialProgressMessage,b2network::FB2MessageInfoRequestSkipTutorialProgress> message{};
		//}
		//));
		//Issues.Add(RequestDrawTutorialItemClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//{
		//	PostMessageHelper<DrawTutorialItemMessage,b2network::FB2MessageInfoRequestDrawTutorialItem> message(characterType);
		//}
		//));
		//Issues.Add(RequestPrepareTutorialClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//{
		//	PostMessageHelper<PrepareTutorialMessage,b2network::FB2MessageInfoRequestPrepareTutorial> message(characterType);
		//}
		//));
		//Issues.Add(RequestGetUnitySkillMissionClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//{
		//	PostMessageHelper<GetUnitySkillMissionMessage, b2network::FB2MessageInfoRequestGetUnitySkillMission> message(characterType);
		//}
		//));
		//Issues.Add(RequestAwakenUnitySkillMissionClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 missionId)
		//{
		//	PostMessageHelper<AwakenUnitySkillMissionMessage, b2network::FB2MessageInfoRequestAwakenUnitySkillMission> message(characterType, missionId);
		//}
		//));
		//Issues.Add(RequestAwakenUnitySkillClass<int32>::GetInstance().Subscribe2(
		//	[](int32 characterType)
		//{
		//	PostMessageHelper<AwakenUnitySkillMessage, b2network::FB2MessageInfoRequestAwakenUnitySkill> message(characterType);
		//}
		//));
		//Issues.Add(RequestReceiveUnitySkillRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 mainCharacterType, int32 unityCharacterType)
		//{
		//	PostMessageHelper<ReceiveUnitySkillRewardMessage, b2network::FB2MessageInfoRequestReceiveUnitySkillReward> message(mainCharacterType, unityCharacterType);
		//}
		//));
		//Issues.Add(RequestGetDuelModeInfoClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetDuelModeInfoMessage, b2network::FB2MessageInfoRequestGetDuelModeInfo> message{};
		//}
		//));
		//Issues.Add(RequestGetChallengeModeInfoClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetChallengeModeInfoMessage, b2network::FB2MessageInfoRequestGetChallengeModeInfo> message{};
		//}
		//));
		//Issues.Add(RequestGetHallOfFameClass<int32, bool>::GetInstance().Subscribe2(
		//	[](int32 mode, bool need_user_info)
		//{
		//	PostMessageHelper<GetHallOfFameMessage, b2network::FB2MessageInfoRequestGetHallOfFame> message(mode, need_user_info);
		//}
		//));
		//Issues.Add(RequestPraiseTargetRankerClass<int32, int64, int32>::GetInstance().Subscribe2(
		//	[](int32 mode, int64 account_id, int32 ranking)
		//{
		//	PostMessageHelper<PraiseTargetRankerMessage, b2network::FB2MessageInfoRequestPraiseTargetRanker> message(mode, account_id, ranking);
		//}
		//));

		//Issues.Add(RequestReceiveRestRewardClass<int32, bool>::GetInstance().Subscribe2(
		//	[](int32 rewardId, bool isDoubleReward)
		//{
		//	PostMessageHelper<ReceiveRestRewardMessage, b2network::FB2MessageInfoRequestReceiveRestReward> message(rewardId, isDoubleReward);
		//}
		//));
		//Issues.Add(RequestCheckModeOpenClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<CheckModeOpenMessage, b2network::FB2MessageInfoRequestCheckModeOpen> message{};
		//}
		//));
		//Issues.Add(RequestGiveTutorialRewardClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GiveTutorialRewardMessage,b2network::FB2MessageInfoRequestGiveTutorialReward> message{};
		//}
		//));
		//Issues.Add(RequestCheckReachPowerClass<const TArray<b2network::B2CharacterPowerPtr>&>::GetInstance().Subscribe2(
		//	[](const TArray<b2network::B2CharacterPowerPtr>& characterPower)
		//{
		//	PostMessageHelper<CheckReachPowerMessage,b2network::FB2MessageInfoRequestCheckReachPower> message(characterPower);
		//}
		//));
		//Issues.Add(RequestTrackingPowerDetailClass<int32, int32, int32, const TArray<b2network::B2CharacterPowerSpecificPtr>&>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 itemPresetId, int32 skillSlot, const TArray<b2network::B2CharacterPowerSpecificPtr>& powerSpecifics)
		//{
		//	PostMessageHelper<TrackingPowerDetailMessage, b2network::FB2MessageInfoRequestTrackingPowerDetail> message(characterType, itemPresetId, skillSlot, powerSpecifics);
		//}
		//));
		//Issues.Add(RequestKakaoMessageProcessingClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& MessageBoxId)
		//{
		//	PostMessageHelper<KakaoMessageProcessingMessage, b2network::FB2MessageInfoRequestKakaoMessageProcessing> message(MessageBoxId);
		//}
		//));
		//Issues.Add(RequestKakaoCouponUseClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& CouponCode)
		//{
		//	PostMessageHelper<KakaoCouponUseMessage, b2network::FB2MessageInfoRequestKakaoCouponUse> message(CouponCode);
		//}
		//));
		//Issues.Add(RequestKakaoAccountConversionPrepareClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& IdpCode)
		//{
		//	PostMessageHelper<KakaoAccountConversionPrepareMessage, b2network::FB2MessageInfoRequestKakaoAccountConversionPrepare> message(IdpCode);
		//}
		//));
		//Issues.Add(RequestKakaoAccountConversionCompleteClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& IdpCode)
		//{
		//	PostMessageHelper<KakaoAccountConversionCompleteMessage, b2network::FB2MessageInfoRequestKakaoAccountConversionComplete> message(IdpCode);
		//}
		//));
		//Issues.Add(RequestSkipMatchTutorialClass<int32>::GetInstance().Subscribe2(
		//	[](int32 matchType)
		//{
		//	PostMessageHelper<SkipMatchTutorialMessage, b2network::FB2MessageInfoRequestSkipMatchTutorial> message(matchType);
		//}
		//));
		//Issues.Add(RequestGetGuildMemberClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetGuildMemberMessage, b2network::FB2MessageInfoRequestGetGuildMember> message{};
		//}
		//));
		//Issues.Add(RequestGetEventClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetEventMessage, b2network::FB2MessageInfoRequestGetEvent> message{};
		//}
		//));
		//Issues.Add(RequestReceivePointEventRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 rewardIndex)
		//{
		//	PostMessageHelper<ReceivePointEventRewardMessage, b2network::FB2MessageInfoRequestReceivePointEventReward> message(eventId, rewardIndex);
		//}
		//));
		//Issues.Add(RequestReceiveLevelUpEventRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 index)
		//{
		//	PostMessageHelper<ReceiveLevelUpEventRewardMessage, b2network::FB2MessageInfoRequestReceiveLevelUpEventReward> message(eventId, index);
		//}
		//));
		//Issues.Add(RequestReceiveStageClearEventRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 index)
		//{
		//	PostMessageHelper<ReceiveStageClearEventRewardMessage, b2network::FB2MessageInfoRequestReceiveStageClearEventReward> message(eventId, index);
		//}
		//));
		//Issues.Add(RequestServerCmdClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& command)
		//{
		//	PostMessageHelper<ServerCmdMessage, b2network::FB2MessageInfoRequestServerCmd> message(command);
		//}
		//));
		//Issues.Add(RequestSetDebugModeClass<bool>::GetInstance().Subscribe2(
		//	[](bool isDebug)
		//{
		//	PostMessageHelper<SetDebugModeMessage, b2network::FB2MessageInfoRequestSetDebugMode> message(isDebug);
		//}
		//));
		//Issues.Add(RequestSetRoomDebugModeClass<bool>::GetInstance().Subscribe2(
		//	[](bool isDebug)
		//{
		//	PostMessageHelper<SetRoomDebugModeMessage, b2network::FB2MessageInfoRequestSetRoomDebugMode> message(isDebug);
		//}
		//));
		//Issues.Add(RequestTraceClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& tag)
		//{
		//	PostMessageHelper<TraceMessage, b2network::FB2MessageInfoRequestTrace> message(tag);
		//}
		//));
		//Issues.Add(RequestGetSeasonMissionClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetSeasonMissionMessage, b2network::FB2MessageInfoRequestGetSeasonMission> message{};
		//}
		//));
		//Issues.Add(RequestRewardSeasonMissionClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 day, int32 index)
		//{
		//	PostMessageHelper<RewardSeasonMissionMessage, b2network::FB2MessageInfoRequestRewardSeasonMission> message(eventId, day, index);
		//}
		//));
		//Issues.Add(RequestAttendSeasonMissionClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<AttendSeasonMissionMessage, b2network::FB2MessageInfoRequestAttendSeasonMission> message{};
		//}
		//));
		//Issues.Add(RequestStartRaidClass<int32>::GetInstance().Subscribe2(
		//	[](int32 battleCharacterType)
		//{
		//	PostMessageHelper<StartRaidMessage, b2network::FB2MessageInfoRequestStartRaid> message(battleCharacterType);
		//}
		//));
		//Issues.Add(RequestBuyHotTimeBuffClass<int32>::GetInstance().Subscribe2(
		//	[](int32 buffType)
		//{
		//	PostMessageHelper<BuyHotTimeBuffMessage, b2network::FB2MessageInfoRequestBuyHotTimeBuff> message(buffType);
		//}
		//));
		//Issues.Add(RequestRewardFlatRatePackageBonusClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<RewardFlatRatePackageBonusMessage, b2network::FB2MessageInfoRequestRewardFlatRatePackageBonus> message{};
		//}
		//));
		//Issues.Add(RequestGetPackageShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetPackageShopMessage, b2network::FB2MessageInfoRequestGetPackageShop> message{};
		//}
		//));
		//Issues.Add(RequestGetPeriodPackageStateClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetPeriodPackageStateMessage, b2network::FB2MessageInfoRequestGetPeriodPackageState> message{};
		//}
		//));
		//Issues.Add(RequestRewardLevelUpPackageClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int packageId, int characterType, int level)
		//{
		//	PostMessageHelper<RewardLevelUpPackageMessage, b2network::FB2MessageInfoRequestRewardLevelUpPackage> message(packageId, characterType, level);
		//}
		//));
		//Issues.Add(RequestRewardReachLevelPackageClass<int32>::GetInstance().Subscribe2(
		//	[](int packageId)
		//{
		//	PostMessageHelper<RewardReachLevelPackageMessage, b2network::FB2MessageInfoRequestRewardReachLevelPackage> message(packageId);
		//}
		//));
		//Issues.Add(RequestGiveUpQuestClass<int32>::GetInstance().Subscribe2(
		//	[](int slot)
		//{
		//	PostMessageHelper<GiveUpQuestMessage, b2network::FB2MessageInfoRequestGiveUpQuest> message(slot);
		//}
		//));
		//Issues.Add(RequestGetContentsModeStateClass<int32>::GetInstance().Subscribe2(
		//	[](int mode)
		//{
		//	PostMessageHelper<GetContentsModeStateMessage, b2network::FB2MessageInfoRequestGetContentsModeState> message(mode);
		//}
		//));
		//Issues.Add(RequestRewardPointShopEventClass<int32, int32, int32>::GetInstance().Subscribe2(
		//	[](int eventId, int index, int buyCount)
		//{
		//	PostMessageHelper<RewardPointShopEventMessage, b2network::FB2MessageInfoRequestRewardPointShopEvent> message(eventId, index, buyCount);
		//}
		//));
		//Issues.Add(RequestReportUserClass<int64, int32, int32, const FString&>::GetInstance().Subscribe2(
		//	[](int64 targetAccountId, int modeType, int reasonType, const FString& explain)
		//{
		//	PostMessageHelper<ReportUserMessage, b2network::FB2MessageInfoRequestReportUser> message(targetAccountId, modeType, reasonType, explain);
		//}
		//));
		//Issues.Add(RequestBlockChatClass<int64>::GetInstance().Subscribe2(
		//	[](int64 blockAccountId)
		//{
		//	PostMessageHelper<BlockChatMessage, b2network::FB2MessageInfoRequestBlockChat> message(blockAccountId);
		//}
		//));
		//Issues.Add(RequestUnblockChatClass<int64>::GetInstance().Subscribe2(
		//	[](int64 unblockAccountId)
		//{
		//	PostMessageHelper<UnblockChatMessage, b2network::FB2MessageInfoRequestUnblockChat> message(unblockAccountId);
		//}
		//));
		//Issues.Add(RequestGetBlockChatListClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetBlockChatListMessage, b2network::FB2MessageInfoRequestGetBlockChatList> message{};
		//}
		//));
		//Issues.Add(RequestClientStateReportClass<const FString&, const FString&, const FString&, const FString&>::GetInstance().Subscribe2(
		//	[](const FString& category, const FString& event_name, const FString& label, const FString& detail_info)
		//{
		//	PostMessageHelper<ClientStateReportMessage, b2network::FB2MessageInfoRequestClientStateReport> message(category, event_name, label, detail_info);
		//}
		//));
		//Issues.Add(RequestRewardTenLotteryEventClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 index)
		//{
		//	PostMessageHelper<RewardTenLotteryEventMessage, b2network::FB2MessageInfoRequestRewardTenLotteryEvent> message(eventId, index);
		//}
		//));
		//Issues.Add(RequestRewardSpendGemEventClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 index)
		//{
		//	PostMessageHelper<RewardSpendGemMessage, b2network::FB2MessageInfoRequestRewardSpendGemEvent> message(eventId, index);
		//}
		//));
		//Issues.Add(RequestReceiveGemPurchaseEventRewardClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 eventId, int32 index)
		//{
		//	PostMessageHelper<ReceiveGemPurchaseEventRewardMessage, b2network::FB2MessageInfoRequestReceiveGemPurchaseEventReward> message(eventId, index);
		//}
		//));
		//Issues.Add(RequestRewardTenLotterySavingsEventClass<int32>::GetInstance().Subscribe2(
		//	[](int32 eventId)
		//{
		//	PostMessageHelper<RewardTenLotterySavingsEventMessage, b2network::FB2MessageInfoRequestRewardTenLotterySavingsEvent> message(eventId);
		//}
		//));
		//Issues.Add(RequestInvitationClass<const FString&, const FString&>::GetInstance().Subscribe2(
		//	[](const FString& invitee, const FString& room_id)
		//{
		//	PostMessageHelper<InvitationMessage, b2network::FB2MessageInfoInvitationRequest> message(invitee, room_id);
		//}
		//));
		//Issues.Add(RequestAcceptInvitationClass<const int32, const FString&, const FString&, const FString&, b2network::B2RoomLocationPtr>::GetInstance().Subscribe2(
		//	[](const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee, b2network::B2RoomLocationPtr room_location)
		//{
		//	PostMessageHelper<AcceptInvitationMessage, b2network::FB2MessageInfoAcceptInvitationRequest> message(invitation_type, inviter, invitation_ref, invitee, room_location);
		//}
		//));
		//Issues.Add(RequestDeclineInvitationClass<const int32, const FString&, const FString&, const FString&>::GetInstance().Subscribe2(
		//	[](const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee)
		//{
		//	PostMessageHelper<DeclineInvitationMessage, b2network::FB2MessageInfoDeclineInvitationRequest> message(invitation_type, inviter, invitation_ref, invitee);
		//}
		//));

		//Issues.Add(RequestRollDiceEventClass<const int32, const bool>::GetInstance().Subscribe2(
		//	[](const int32 event_id, const bool is_free_dice)
		//{
		//	PostMessageHelper<RollDiceEventMessage, b2network::FB2MessageInfoRequestRollDiceEvent> message(event_id, is_free_dice);
		//}
		//));
		//Issues.Add(RequestBuyDiceEventPointClass<const int32>::GetInstance().Subscribe2(
		//	[](const int32 event_id)
		//{
		//	PostMessageHelper<BuyDiceEventPointMessage, b2network::FB2MessageInfoRequestBuyDiceEventPoint> message(event_id);
		//}
		//));
		//Issues.Add(RequestRewardDiceEventMileageClass<const int32, const int32>::GetInstance().Subscribe2(
		//	[](const int32 event_id, const int reward_index)
		//{
		//	PostMessageHelper<RewardDiceEventMileageMessage, b2network::FB2MessageInfoRequestRewardDiceEventMileage> message(event_id, reward_index);
		//}
		//));
		//Issues.Add(RequestReportUserViaChannelClass<int64, int32, int32, const FString&>::GetInstance().Subscribe2(
		//	[](const int64 target_account_id, const int32 mode_type, const int32 reason_type, const FString& explain)
		//{
		//	PostMessageHelper<ReportUserViaChannelMessage, b2network::FB2MessageInfoReportUserViaChannelRequest> message(target_account_id, mode_type, reason_type, explain);
		//}
		//));
		//Issues.Add(RequestJoinWorldClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& worldId)
		//{
		//	PostMessageHelper<WorldJoinMessage, b2network::FB2MessageInfoWorldJoinRequest> message(worldId);
		//}
		//));
		//Issues.Add(RequestLeaveWorldClass<const FString&>::GetInstance().Subscribe2(
		//	[](const FString& worldId)
		//{
		//	PostMessageHelper<WorldLeaveMessage, b2network::FB2MessageInfoWorldLeaveRequest> message(worldId);
		//}
		//));

		//Issues.Add(RequestGetAccountCostumeClass<int32, int32>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 inventoryType)
		//{
		//	PostMessageHelper<GetAccountCostumeMessage, b2network::FB2MessageInfoRequestGetAccountCostume> message(characterType, inventoryType);
		//}
		//));
		//Issues.Add(RequestEquipCostumeClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& costumeIds)
		//{
		//	PostMessageHelper<EquipCostumeMessage, b2network::FB2MessageInfoRequestEquipCostume> message(costumeIds);
		//}
		//));
		//Issues.Add(RequestUnequipCostumeClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& costumeIds)
		//{
		//	PostMessageHelper<UnequipCostumeMessage, b2network::FB2MessageInfoRequestUnequipCostume> message(costumeIds);
		//}
		//));
		//Issues.Add(RequestEnhanceCostumeClass<int64>::GetInstance().Subscribe2(
		//	[](int64 costumeId)
		//{
		//	PostMessageHelper<EnhanceCostumeMessage, b2network::FB2MessageInfoRequestEnhanceCostume> message(costumeId);
		//}
		//));
		//Issues.Add(RequestDismantleCostumesClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& costumeIds)
		//{
		//	PostMessageHelper<DismantleCostumesMessage, b2network::FB2MessageInfoRequestDismantleCostumes> message(costumeIds);
		//}
		//));
		//Issues.Add(RequestLockCostumesClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& costumeIds)
		//{
		//	PostMessageHelper<LockCostumesMessage, b2network::FB2MessageInfoRequestLockCostumes> message(costumeIds);
		//}
		//));
		//Issues.Add(RequestUnlockCostumesClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& costumeIds)
		//{
		//	PostMessageHelper<UnlockCostumesMessage, b2network::FB2MessageInfoRequestUnlockCostumes> message(costumeIds);
		//}
		//));
		//Issues.Add(RequestGetCostumeShopClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetCostumeShopMessage, b2network::FB2MessageInfoRequestGetCostumeShop> message{};
		//}
		//));
		//Issues.Add(RequestPurchaseCostumeClass<int32>::GetInstance().Subscribe2(
		//	[](int32 productId)
		//{
		//	PostMessageHelper<PurchaseCostumeMessage, b2network::FB2MessageInfoRequestPurchaseCostume> message(productId);
		//}
		//));
		//Issues.Add(RequestSetCostumeVisibleClass<int32, int32, bool>::GetInstance().Subscribe2(
		//	[](int32 characterType, int32 costumeType, bool isVisible)
		//{
		//	PostMessageHelper<SetCostumeVisibleMessage, b2network::FB2MessageInfoRequestSetCostumeVisible> message(characterType, costumeType, isVisible);
		//}
		//));

		////totem
		//Issues.Add(RequestGetAccountTotemClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetAccountTotemMessage, b2network::FB2MessageInfoRequestGetAccountTotem> message{};
		//}
		//));
		//Issues.Add(RequestEquipTotemsClass<const TArray<int64>&, int32>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds, int32 characterType)
		//{
		//	PostMessageHelper<EquipTotemsMessage, b2network::FB2MessageInfoRequestEquipTotems> message(totemIds, characterType);
		//}
		//));
		//Issues.Add(RequestUnequipTotemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds)
		//{
		//	PostMessageHelper<UnequipTotemsMessage, b2network::FB2MessageInfoRequestUnequipTotems> message(totemIds);
		//}
		//));
		//Issues.Add(RequestLockTotemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds)
		//{
		//	PostMessageHelper<LockTotemsMessage, b2network::FB2MessageInfoRequestLockTotems> message(totemIds);
		//}
		//));
		//Issues.Add(RequestUnlockTotemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds)
		//{
		//	PostMessageHelper<UnlockTotemsMessage, b2network::FB2MessageInfoRequestUnlockTotems> message(totemIds);
		//}
		//));
		//Issues.Add(RequestSellTotemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds)
		//{
		//	PostMessageHelper<SellTotemsMessage, b2network::FB2MessageInfoRequestSellTotems> message(totemIds);
		//}
		//));
		//Issues.Add(RequestDismantleTotemsClass<const TArray<int64>&>::GetInstance().Subscribe2(
		//	[](const TArray<int64>& totemIds)
		//{
		//	PostMessageHelper<DismantleTotemsMessage, b2network::FB2MessageInfoRequestDismantleTotems> message(totemIds);
		//}
		//));
		//Issues.Add(RequestRefineTotemClass<int64, int32, int32>::GetInstance().Subscribe2(
		//	[](int64 totemId, int32 optionId, int32 costType)
		//{
		//	PostMessageHelper<RefineTotemMessage, b2network::FB2MessageInfoRequestRefineTotem> message(totemId, optionId, costType);
		//}
		//));
		//Issues.Add(RequestModifyTotemNewFlagClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<ModifyTotemNewFlagMessage, b2network::FB2MessageInfoRequestModifyTotemNewFlag> message{};
		//}
		//));
		//Issues.Add(RequestAddTotemToInventoryClass<int32>::GetInstance().Subscribe2(
		//	[](int32 templateId)
		//{
		//	PostMessageHelper<AddTotemToInventoryMessage, b2network::FB2MessageInfoRequestAddTotemToInventory> message(templateId);
		//}
		//));

		//Issues.Add(RequestItemForgeListClass<>::GetInstance().Subscribe2(
		//	[]()
		//{
		//	PostMessageHelper<GetItemForgeListMessage, b2network::FB2MessageInfoRequestGetFactory> message{};
		//}
		//));
		//Issues.Add(RequestItemForgeClass<int64, int64, int64, int64, int64, int64, int64, int64, int64, int64, int64, int64>::GetInstance().Subscribe2(
		//	[](int64 ForgeID, int64 Quantity, int64 MainItemUID, int64 MaterialItemUID1, int64 MaterialItemUID2, int64 MaterialItemUID3, int64 MaterialItemUID4, int64 MainItemRefID, int64 MaterialItemRefID1, int64 MaterialItemRefID2, int64 MaterialItemRefID3, int64 MaterialItemRefID4)
		//{
		//	PostMessageHelper<TryItemForgeMessage, b2network::FB2MessageInfoRequestMakeFactoryItem> message(ForgeID, Quantity, MainItemUID, MaterialItemUID1, MaterialItemUID2, MaterialItemUID3, MaterialItemUID4, MainItemRefID, MaterialItemRefID1, MaterialItemRefID2, MaterialItemRefID3, MaterialItemRefID4);
		//}
		//));

		//Issues.Add(RequestUnsealBoxClass<int64, int32, int64>::GetInstance().Subscribe2(
		//	[](int64 sealboxUID, int32 pcClass, int64 unsealAmount)
		//{
		//	PostMessageHelper<UnsealBoxMessage, b2network::FB2MessageInfoRequestUnsealBox> message(sealboxUID, pcClass, unsealAmount);
		//}
		//));


		// REPLACEMENT POINT: ONLINE_WHOLESALER CPP REQUEST
		// NEVER DELETE LINE ABOVE
	}
}
