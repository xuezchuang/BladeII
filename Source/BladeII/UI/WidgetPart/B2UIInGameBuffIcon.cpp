#include "BladeII.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"

UB2UIInGameBuffIcon::UB2UIInGameBuffIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NonScaledSize = FVector2D(40.0f, 40.0f);
}

void UB2UIInGameBuffIcon::Init()
{
	Super::Init();

	BindDocAuto();
	UpdateWidgets();
}

void UB2UIInGameBuffIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_MainIcon);
}

void UB2UIInGameBuffIcon::BindDelegates()
{
	
}

void UB2UIInGameBuffIcon::BindDocAuto()
{
}

void UB2UIInGameBuffIcon::UnbindDoc()
{
}

void UB2UIInGameBuffIcon::UpdateWidgets()
{

}

/*================================================================================
UB2UIInGameBuffIcon_Stage
================================================================================*/

UB2UIInGameBuffIcon_Stage::UB2UIInGameBuffIcon_Stage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyBuffType = EStageBuffType::SBT_End;

	MainIconMaterial_IncExp = NULL;
	MainIconMaterial_IncGold = NULL;
	MainIconMaterial_AutoSkill = NULL;
	MainIconMaterial_BoostGameSpeed = NULL;
}

void UB2UIInGameBuffIcon_Stage::Init()
{
	Super::Init();
}

void UB2UIInGameBuffIcon_Stage::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIInGameBuffIcon_Stage::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIInGameBuffIcon_Stage::BindDocAuto()
{
	Super::BindDocAuto();
}

void UB2UIInGameBuffIcon_Stage::UnbindDoc()
{
	Super::UnbindDoc();
}

void UB2UIInGameBuffIcon_Stage::SetBuffType(EStageBuffType InBuffType)
{
	MyBuffType = InBuffType;

	UpdateWidgets();
}

void UB2UIInGameBuffIcon_Stage::UpdateWidgets()
{
	Super::UpdateWidgets();

	this->SetVisibility((MyBuffType != EStageBuffType::SBT_End) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_MainIcon.IsValid())
	{
		switch (MyBuffType)
		{
		case EStageBuffType::SBT_IncreaseExp: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_IncExp); break;
		case EStageBuffType::SBT_IncreaseGold: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_IncGold); break;
		case EStageBuffType::SBT_AutoSkill: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_AutoSkill); break;
		case EStageBuffType::SBT_BoostGameSpeed: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_BoostGameSpeed); break;
		}
	}
}

/*================================================================================
UB2UIInGameBuffIcon_Resurrect
================================================================================*/

UB2UIInGameBuffIcon_Resurrect::UB2UIInGameBuffIcon_Resurrect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyBuffType = EResurrectBuffType::RBT_None;
	MainIconMaterial_Attack = NULL;
	MainIconMaterial_Defense = NULL;
	MainIconMaterial_Health = NULL;
	TeamSlotIndex = 0;
}

void UB2UIInGameBuffIcon_Resurrect::Init()
{
	Super::Init();

	BindDocByTeamIndex();
}

void UB2UIInGameBuffIcon_Resurrect::CacheAssets()
{
	Super::CacheAssets();;
}

void UB2UIInGameBuffIcon_Resurrect::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIInGameBuffIcon_Resurrect::BindDocByTeamIndex()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;

	switch (TeamSlotIndex)
	{
	case 0:
		DocBattle->OnCurResurrectBuffAttackCount0Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Attack);
		DocBattle->OnCurResurrectBuffDefenseCount0Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Defense);
		DocBattle->OnCurResurrectBuffHealthCount0Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Health);
		break;
	case 1:
		DocBattle->OnCurResurrectBuffAttackCount1Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Attack);
		DocBattle->OnCurResurrectBuffDefenseCount1Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Defense);
		DocBattle->OnCurResurrectBuffHealthCount1Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Health);
		break;
	case 2:
		DocBattle->OnCurResurrectBuffAttackCount2Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Attack);
		DocBattle->OnCurResurrectBuffDefenseCount2Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Defense);
		DocBattle->OnCurResurrectBuffHealthCount2Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Health);
		break;
	case 3:
		DocBattle->OnCurResurrectBuffAttackCount3Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Attack);
		DocBattle->OnCurResurrectBuffDefenseCount3Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Defense);
		DocBattle->OnCurResurrectBuffHealthCount3Changed.AddUObject(this, &UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Health);
		break;
	}
}

void UB2UIInGameBuffIcon_Resurrect::UnbindDocByTeamIndex()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;

	switch (TeamSlotIndex)
	{
	case 0:
		DocBattle->OnCurResurrectBuffAttackCount0Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffDefenseCount0Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffHealthCount0Changed.RemoveAll(this);
		break;
	case 1:
		DocBattle->OnCurResurrectBuffAttackCount1Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffDefenseCount1Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffHealthCount1Changed.RemoveAll(this);
		break;
	case 2:
		DocBattle->OnCurResurrectBuffAttackCount2Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffDefenseCount2Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffHealthCount2Changed.RemoveAll(this);
		break;
	case 3:
		DocBattle->OnCurResurrectBuffAttackCount3Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffDefenseCount3Changed.RemoveAll(this);
		DocBattle->OnCurResurrectBuffHealthCount3Changed.RemoveAll(this);
		break;
	}
}



void UB2UIInGameBuffIcon_Resurrect::SetBuffType(EResurrectBuffType InBuffType)
{
	MyBuffType = InBuffType;
	UpdateWidgets();
}

void UB2UIInGameBuffIcon_Resurrect::SetResurrectAdditionalInfo(int32 InSlotIndex)
{
	if (TeamSlotIndex == InSlotIndex)
		return;

	UnbindDocByTeamIndex();
	TeamSlotIndex = InSlotIndex;
	BindDocByTeamIndex();
}

void UB2UIInGameBuffIcon_Resurrect::UpdateWidgets()
{
	Super::UpdateWidgets();

	// 부활 버프는 한 종류만 선택 가능
	int32 AttackBuffValue, DefenseBuffValue, HealthBuffValue;
	GetBuffData(AttackBuffValue, DefenseBuffValue, HealthBuffValue);
	
	bool bTempAtLeastOneBuffActivated = (AttackBuffValue > 0 || DefenseBuffValue > 0 || HealthBuffValue > 0);
	if (bTempAtLeastOneBuffActivated && (MyBuffType != EResurrectBuffType::RBT_None || MyBuffType != EResurrectBuffType::RBT_End))
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (IMG_MainIcon.IsValid())
		{
			IMG_MainIcon->SetBrushFromMaterial(
				MyBuffType == EResurrectBuffType::RBT_Attack ? MainIconMaterial_Attack :
					(MyBuffType == EResurrectBuffType::RBT_Defense ? MainIconMaterial_Defense : MainIconMaterial_Health)
				);
		}
	}
	else
	{
		// 실제로 Buff 가 없는 채로 생성이 될 수도 있으니 그런 경우 Collapse
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Attack(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Attack);
	}
}

void UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Defense(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Defense);
	}
}

void UB2UIInGameBuffIcon_Resurrect::OnChangedResurrectBuffValue_Health(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Health);
	}
}

void UB2UIInGameBuffIcon_Resurrect::GetBuffData(int32& OutAttackBuffValue, int32& OutDefenseBuffValue, int32& OutHealthBuffValue)
{
	OutAttackBuffValue = 0;
	OutDefenseBuffValue = 0;
	OutHealthBuffValue = 0;

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;
	
	OutAttackBuffValue = DocBattle->GetCurResurrectBuffAttackCountByIndex(TeamSlotIndex);
	OutDefenseBuffValue = DocBattle->GetCurResurrectBuffDefenseCountByIndex(TeamSlotIndex);
	OutHealthBuffValue = DocBattle->GetCurResurrectBuffHealthCountByIndex(TeamSlotIndex);
}

/*================================================================================
UB2UIInGameBuffIcon_Skill
================================================================================*/

EUIUnitedSkillBuffType UnitedSkillBT_from_CharacterBT(EBuffType InType)
{	
	switch (InType)
	{
	case EBuffType::Buff_Shield: return EUIUnitedSkillBuffType::EUUSBT_Team_Shield;
	case EBuffType::Buff_IncreaseCriticalRate: return EUIUnitedSkillBuffType::EUUSBT_Team_IncreaseCriticalRate;
	case EBuffType::Buff_IncreaseCriticalDamage: return EUIUnitedSkillBuffType::EUUSBT_Team_IncreaseCriticalDamage;
	case EBuffType::Buff_DecreaseReceiveDamage: return EUIUnitedSkillBuffType::EUUSBT_Basic_DecreaseDamage;
	case EBuffType::Buff_Hide: return EUIUnitedSkillBuffType::EUUSBT_Basic_Hide;
	case EBuffType::Buff_HealHPByAttack: return EUIUnitedSkillBuffType::EUUSBT_Basic_HealHPByAttack;
	case EBuffType::Buff_ReflectDamage: return EUIUnitedSkillBuffType::EUUSBT_Basic_ReflectDamage;
	case EBuffType::Debuff_Freezing: return EUIUnitedSkillBuffType::EUUSBT_StateDamage_Freezing;
	case EBuffType::Debuff_Stun: return EUIUnitedSkillBuffType::EUUSBT_StateDamage_Stun;
	case EBuffType::Debuff_Burning: return EUIUnitedSkillBuffType::EUUSBT_StateDamage_Burning;
	case EBuffType::Debuff_Poison: return EUIUnitedSkillBuffType::EUUSBT_StateDamage_Poison;
	case EBuffType::Buff_Unbeatable: return EUIUnitedSkillBuffType::EUUSBT_Misc_Unbeatable;
	case EBuffType::Buff_AbnormalStateImmune: return EUIUnitedSkillBuffType::EUUSBT_Team_AbnormalStateImmune;
	case EBuffType::Debuff_Immobilize: return EUIUnitedSkillBuffType::EUUSBT_Character_Restraint;
	}
	return EUIUnitedSkillBuffType::EUUSBT_End;
}

UB2UIInGameBuffIcon_Skill::UB2UIInGameBuffIcon_Skill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BuffType = EUIUnitedSkillBuffType::EUUSBT_End;
}

void UB2UIInGameBuffIcon_Skill::Init()
{
	Super::Init();
}

void UB2UIInGameBuffIcon_Skill::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_RemainingTime);
}

void UB2UIInGameBuffIcon_Skill::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIInGameBuffIcon_Skill::BindDocAuto()
{
	Super::BindDocAuto();
}

void UB2UIInGameBuffIcon_Skill::UnbindDoc()
{
	Super::UnbindDoc();
}

void UB2UIInGameBuffIcon_Skill::SetBuffType(EUIUnitedSkillBuffType InType)
{
	BuffType = InType;
	UpdateWidgets();
}

void UB2UIInGameBuffIcon_Skill::SetRemainigTime(float InTime)
{
	if (TB_RemainingTime.IsValid())
	{
		// 음수에서는 숨기는 걸로 하자.
		TB_RemainingTime->SetVisibility(InTime < 0.0f ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

		TB_RemainingTime->SetText(FText::AsNumber((int32)InTime));
	}
}

void UB2UIInGameBuffIcon_Skill::UpdateWidgets()
{
	Super::UpdateWidgets();

	UMaterialInterface* FoundIconMat = NULL;
	for (FSkillBuffTypeIconMtrlMapping& ThisMapping : IconMtrlMapping)
	{
		if (ThisMapping.BuffType != EUIUnitedSkillBuffType::EUUSBT_End && ThisMapping.BuffType == BuffType)
		{
			FoundIconMat = ThisMapping.IconMtrl;
			break;
		}
	}

	if (IMG_MainIcon.IsValid() && FoundIconMat)
	{
		IMG_MainIcon->SetBrushFromMaterial(FoundIconMat);
	}
}




UB2UIInGameBuffIcon_GuildBuff::UB2UIInGameBuffIcon_GuildBuff(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GuildBuffType = EItemOption::EIO_End;

	MainIconMaterial_Attack = NULL;
	MainIconMaterial_Defense = NULL;
	MainIconMaterial_HP = NULL;
}

void UB2UIInGameBuffIcon_GuildBuff::SetBuffType(EItemOption InBuffType)
{
	GuildBuffType = InBuffType;

	UpdateWidgets();
}

void UB2UIInGameBuffIcon_GuildBuff::UpdateWidgets()
{
	Super::UpdateWidgets();

	this->SetVisibility((GuildBuffType != EItemOption::EIO_End) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_MainIcon.IsValid())
	{
		switch (GuildBuffType)
		{
		case EItemOption::EIO_Offense_IncAttack: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_Attack); break;
		case EItemOption::EIO_Defense_IncDefense: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_Defense); break;
		case EItemOption::EIO_Health_IncHealth: IMG_MainIcon->SetBrushFromMaterial(MainIconMaterial_HP); break;
		}
	}
}

/*================================================================================
UB2UIInGameBuffIcon_DimensionTower
================================================================================*/

UB2UIInGameBuffIcon_DimensionTower::UB2UIInGameBuffIcon_DimensionTower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyBuffType = EResurrectBuffType::RBT_None;
	MainIconMaterial_Attack = NULL;
	MainIconMaterial_Defense = NULL;
	MainIconMaterial_Health = NULL;
}

void UB2UIInGameBuffIcon_DimensionTower::Init()
{
	Super::Init();
}

void UB2UIInGameBuffIcon_DimensionTower::CacheAssets()
{
	Super::CacheAssets();;
}

void UB2UIInGameBuffIcon_DimensionTower::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIInGameBuffIcon_DimensionTower::BindDocAuto()
{

	Super::BindDocAuto();

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;

	DocBattle->OnCurDimensionBuffAttackCountChanged.AddUObject(this, &UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Attack);
	DocBattle->OnCurDimensionBuffDefenseCountChanged.AddUObject(this, &UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Defense);
	DocBattle->OnCurDimensionBuffHealthCountChanged.AddUObject(this, &UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Health);
}


void UB2UIInGameBuffIcon_DimensionTower::UnbindDoc()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;

	DocBattle->OnCurDimensionBuffAttackCountChanged.RemoveAll(this);
	DocBattle->OnCurDimensionBuffDefenseCountChanged.RemoveAll(this);
	DocBattle->OnCurDimensionBuffHealthCountChanged.RemoveAll(this);

	Super::UnbindDoc();
}

void UB2UIInGameBuffIcon_DimensionTower::SetBuffType(EResurrectBuffType InBuffType)
{
	MyBuffType = InBuffType;
	UpdateWidgets();
}

void UB2UIInGameBuffIcon_DimensionTower::UpdateWidgets()
{
	Super::UpdateWidgets();

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle == nullptr)
		return;

	if ((DocBattle->GetCurDimensionBuffAttackCount() > 0 && MyBuffType == EResurrectBuffType::RBT_Attack)
		|| (DocBattle->GetCurDimensionBuffDefenseCount() > 0 && MyBuffType == EResurrectBuffType::RBT_Defense)
		|| (DocBattle->GetCurDimensionBuffHealthCount() > 0 && MyBuffType == EResurrectBuffType::RBT_Health)
		)
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (IMG_MainIcon.IsValid())
		{
			IMG_MainIcon->SetBrushFromMaterial(
				MyBuffType == EResurrectBuffType::RBT_Attack ? MainIconMaterial_Attack :
				(MyBuffType == EResurrectBuffType::RBT_Defense ? MainIconMaterial_Defense : MainIconMaterial_Health)
			);
		}
	}
	else
	{
		// 실제로 Buff 가 없는 채로 생성이 될 수도 있으니 그런 경우 Collapse
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Attack(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (MyBuffType != EResurrectBuffType::RBT_Attack)
		return;

	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Attack);
	}
}

void UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Defense(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (MyBuffType != EResurrectBuffType::RBT_Defense)
		return;

	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Defense);
	}
}

void UB2UIInGameBuffIcon_DimensionTower::OnChangedDimensionBuffValue_Health(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue)
{
	if (MyBuffType != EResurrectBuffType::RBT_Health)
		return;

	if (CurrentValue != PrevValue)
	{
		if (CurrentValue > PrevValue)
			SetBuffType(EResurrectBuffType::RBT_Health);
	}
}
