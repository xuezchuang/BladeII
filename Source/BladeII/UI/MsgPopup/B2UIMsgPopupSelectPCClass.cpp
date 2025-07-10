#include "B2UIMsgPopupSelectPCClass.h"
#include "B2UICharacterSelectPage.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"

#include "BladeIIGameImpl.h"

UB2UIMsgPopupSelectPCClass::UB2UIMsgPopupSelectPCClass(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectPCClass = EPCClass::EPC_End;
	RemainAutoCloseSecond = 0;
}

void UB2UIMsgPopupSelectPCClass::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UIMsgPopupSelectPCClass);

	DeliveryStartRaidTicket = DeliveryStartRaidClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryStartRaid)
		Capture->ResponseStartRaid();
	END_CAPTURE_OBJECT()
		);
}

void UB2UIMsgPopupSelectPCClass::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DeliveryStartRaidClass<>::GetInstance().Unsubscribe(DeliveryStartRaidTicket);
}

void UB2UIMsgPopupSelectPCClass::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_AutoClose);

	PCClassList.SetNumUninitialized(4);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, PCClassList[PCClassToInt(EPCClass::EPC_Gladiator)], UIP_Gladiator);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, PCClassList[PCClassToInt(EPCClass::EPC_Assassin)], UIP_Assassin);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, PCClassList[PCClassToInt(EPCClass::EPC_Wizard)], UIP_Wizard);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, PCClassList[PCClassToInt(EPCClass::EPC_Fighter)], UIP_Fighter);

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		auto DocHero = UB2UIDocHelper::GetDocHero(ClassCnt);
		const EPCClass HeroClassEnum = IntToPCClass(ClassCnt);
		if (DocHero)
		{
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(HeroClassEnum);
			PCClassList[ClassCnt]->Init();
			PCClassList[ClassCnt]->SetHeroInfo(HeroClassEnum, HeroLevel, DocHero->GetCombatPower());
			PCClassList[ClassCnt]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {OnClickSelectSlotEntry(InValue); }));
			PCClassList[ClassCnt]->SlotSelect(false);
		}
	}

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ChangeHero")));
}

void UB2UIMsgPopupSelectPCClass::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	EPCClass LastPCClass = EPCClass::EPC_Gladiator;

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		LastPCClass = IntToPCClass(DocBattle->GetCurPCClass());

	PCClassList[PCClassToInt(LastPCClass)]->SlotSelect(true);

	SelectPCClass = LastPCClass;
}

void UB2UIMsgPopupSelectPCClass::OnClickSelectSlotEntry(EPCClass PCClass)
{
	if (SelectPCClass == PCClass)
		return;

	if (PCClassList.IsValidIndex((int32)SelectPCClass))
		PCClassList[(int32)SelectPCClass]->SlotSelect(false);

	SelectPCClass = PCClass;

	if (PCClassList.IsValidIndex((int32)SelectPCClass))
		PCClassList[(int32)SelectPCClass]->SlotSelect(true);
}

void UB2UIMsgPopupSelectPCClass::ResponseStartRaid()
{
	// RequestRaidStart 이름은 레이드 시작이지만, 서버팀에서 프로토콜 재사용으로
	// 레이드 초대시에 어떤 캐릭터를 선택했는지 알리고, 리스폰스를 받은 후에 룸조인.
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		const auto& InvitationInfo = DocRaid->GetRaidInvitationInfo();
		GoRaidGameMode(InvitationInfo.InvitedRoomId, SelectPCClass, InvitationInfo.RaidType, InvitationInfo.RaidStep);
	}
}

void UB2UIMsgPopupSelectPCClass::GoRaidGameMode(const FString& InvitedRoomId, EPCClass ChangeMainPCClass, int32 RaidType, int32 RaidStep)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
		LobbyGameMode->GoRaidGameMode(InvitedRoomId, ChangeMainPCClass, RaidType, RaidStep);

	this->CloseSelf();
}
