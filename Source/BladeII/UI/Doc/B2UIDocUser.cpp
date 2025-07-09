#include "BladeII.h"
#include "Event.h"
//#include "BladeIIPointCharger.h"
//#include "B2LobbyInventory.h"
#include "B2UIDocUser.h"


#include "BladeIIGameImpl.h"

UB2UIDocUser::UB2UIDocUser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = false;
	bAnonymous = false;
	DocType = EUIDocType::User;

#if PLATFORM_WINDOWS
	SetShowVersion(false);
#else
	#if B2_BUILD_LEVEL > B2_BUILD_LEVEL_DEVELOPMENT // !B2_BUILD_LEVEL_QA , !B2_BUILD_LEVEL_LIVE
		SetShowVersion(false);
	#else
		SetShowVersion(true);
	#endif //B2_BUILD_LEVEL >= B2_BUILD_LEVEL_QA

#endif //PLATFORM_WINDOWS
}

UB2UIDocUser::~UB2UIDocUser()
{
	
}

void UB2UIDocUser::Init()
{
	CAPTURE_UOBJECT(UB2UIDocUser);

	//ChangeUserAccountInfoTicket = ChangeUserAccountInfoClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ChangeUserAccountInfo)
	//		Capture->UpdateUserData();
	//	END_CAPTURE_OBJECT()
	//);
	//ChangeStaminaChargeTimeTicket = ChangeStaminaChargeTimeClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ChangeStaminaChargeTime)
	//		Capture->UpdateStaminaData();
	//	END_CAPTURE_OBJECT()
	//);

	UpdateUserData();
}

void UB2UIDocUser::Destroy()
{
	//ChangeUserAccountInfoClass<>::GetInstance().Unsubscribe(ChangeUserAccountInfoTicket);
	//ChangeStaminaChargeTimeClass<>::GetInstance().Unsubscribe(ChangeStaminaChargeTimeTicket);
}

void UB2UIDocUser::UpdateUserData()
{
	//SetUserLevel();
	//SetUserExpCur();
	//SetUserExpMax();
	//SetUserVipLevel();
	//SetUserNickName();
	//SetUserPortrait();
	//	
	//SetSocialPoint(BladeIIGameImpl::GetClientDataStore().GetSocialPoint()); // Current sp anount
	//SetSocialPointMax(BladeIIGameImpl::GetClientDataStore().GetMaxSocialPointValue());
	//SetGold(BladeIIGameImpl::GetClientDataStore().GetGoldAmount()); // Current gold amount
	//SetGem(BladeIIGameImpl::GetClientDataStore().GetGemAmount()); // Current gem amount
	//SetRankPromotionPoint(BladeIIGameImpl::GetClientDataStore().GetRankPromotionPoint()); // Current honer point amount
	//SetAdvancedRankPromotionPoint(BladeIIGameImpl::GetClientDataStore().GetAdvancedRankPromotionPoint()); // Current honer point amount
	//SetDarkCrystal(BladeIIGameImpl::GetClientDataStore().GetDarkCrystal());	// current dark crystal amount
	//SetHeroPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE));
	//SetFamePoint(BladeIIGameImpl::GetClientDataStore().GetFamePoint());
	//SetStageAutoClearTicket(BladeIIGameImpl::GetClientDataStore().GetStageAutoClearTicket());
	//SetDimensionPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE));
	//UpdateStaminaData();
}

void UB2UIDocUser::UpdateStaminaData()
{
	SetStaminaCur(BladeIIGameImpl::GetClientDataStore().GetBladeCurPoint()); // Current stamina amount
	SetStaminaMax(BladeIIGameImpl::GetClientDataStore().GetBladeMaxPoint()); // Max stamina amount
	//SetNextTimeStaminaGet(FBladeIIChargeBladePointCharger::GetBladeIIPointCharger().GetLeftChargeTime());
}
