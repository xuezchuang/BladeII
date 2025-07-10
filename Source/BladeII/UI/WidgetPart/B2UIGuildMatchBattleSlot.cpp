#include "B2UIGuildMatchBattleSlot.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIBattleMain.h"
#include "B2GuildNPCInfo.h"


#include "BladeIIGameImpl.h"

UB2UIGuildMatchBattleSlot::UB2UIGuildMatchBattleSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MercenaryID = INDEX_NONE;
	ConnectedPlayer = nullptr;
	CacheHealth = 0.0f;
	CacheMaxHealth = 0.0f;
	CacheShield = 0.0f;
}

void UB2UIGuildMatchBattleSlot::Init()
{
	Super::Init();
	ConnectedPlayer = nullptr;

	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (GuildDoc)
	{
		GuildDoc->OnViewTargetChanged.AddUObject(this, &UB2UIGuildMatchBattleSlot::OnChangeViewTarget);
	}
}

void UB2UIGuildMatchBattleSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	ConnectedPlayer = nullptr;

	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (GuildDoc)
	{
		GuildDoc->OnViewTargetChanged.RemoveAll(this);
	}
	Super::DestroySelf(InUIManager);
}

void UB2UIGuildMatchBattleSlot::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshPawnStatus();
}

void UB2UIGuildMatchBattleSlot::SeMercenarytData(FGuildMercenaryInfo Info, bool InMyTeam)
{
	SetData(EPCClass::EPC_End, Info.MercenaryLevel, InMyTeam, Info.Spot, Info.FormationNumber);
	MercenaryID = Info.MercenaryID;

	if(TB_Job.IsValid())
		TB_Job->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::NPCName, MercenaryID)));

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();
	if (NPCInfo && IMG_CharacterImage.IsValid())
		IMG_CharacterImage->SetBrushFromMaterial(NPCInfo->GetNPCSlotIcon(MercenaryID));
		
}

void UB2UIGuildMatchBattleSlot::ConnectPawn(ABladeIIPlayer * InPlayer)
{
	ConnectedPlayer = InPlayer;
	SetCamera(false);
	SetDeath(false);
	RefreshPawnStatus();
}

void UB2UIGuildMatchBattleSlot::RefreshPawnStatus()
{
	//if (ConnectedPlayer && ConnectedPlayer->IsValidObj())
	//{
	//	if (ConnectedPlayer->Health != CacheHealth || ConnectedPlayer->GetShield() != CacheShield || ConnectedPlayer->MaxHealth != CacheMaxHealth)
	//	{
	//		CacheHealth		= ConnectedPlayer->Health;
	//		CacheShield		= ConnectedPlayer->GetShield();
	//		CacheMaxHealth	= ConnectedPlayer->MaxHealth; 
	//		SetHpAndShield(CacheHealth, CacheMaxHealth, CacheShield);
	//	}
	//}
}

void UB2UIGuildMatchBattleSlot::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIGuildMatchBattleSlot::OnClickImplement()
{
	//auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	//if (GuildDoc)
	//	GuildGameChangeCameraClass<bool, ABladeIIPlayer*>::GetInstance().Signal(GuildDoc->GetShortCamera(), ConnectedPlayer);
}

void UB2UIGuildMatchBattleSlot::OnChangeViewTarget(UB2UIDocBase * Sender, ABladeIIPlayer * NewTarget, ABladeIIPlayer * OldTarget)
{
	SetCamera(ConnectedPlayer && ConnectedPlayer == NewTarget);
}
