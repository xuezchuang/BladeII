#include "B2UIBossStatus.h"
#include "B2UIProgressBarDivided.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2NPCClassInfoBox.h"
#include "B2InfluenceSkillInfo.h"


void UB2UIBossStatus::CacheAssets()
{
	GET_SLOT(UB2UIProgressBarDividedDual, UIP_ProgressBar_Boss)
	GET_SLOT(UTextBlock, TB_BossName)
	GET_SLOT(UTextBlock, TB_BossInfluence)
}

void UB2UIBossStatus::UpdateStaticText()
{
}

void UB2UIBossStatus::BindDelegates()
{
}

void UB2UIBossStatus::Init()
{
	Super::Init();
	
	if (UIP_ProgressBar_Boss.IsValid())
		UIP_ProgressBar_Boss->Init();
}

void UB2UIBossStatus::BindDocAuto()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	SetDoc(DocBattle); //���� ���� ������ ������ Doc���� ���� Battle�� ����.
	if (DocBattle)
	{
		//���� ��ȸ ����
		SetHP(DocBattle->GetBossHPCurrent(), DocBattle->GetBossHPMax());
		UpdateBossInfo(DocBattle->GetBossMobClass(), DocBattle->GetBossMobClassVariation());

		DocBattle->OnBossHPCurrentChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossHPCurrent);
		DocBattle->OnBossHPMaxChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossHPMax);
		DocBattle->OnBossMobClassChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossMobClass);
		DocBattle->OnBossMobClassVariationChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossMobClassVariation);

		DocBattle->OnBossQTEBreakPointCurrentChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossQTEBreakPointCurrent);
		DocBattle->OnBossQTEBreakPointMaxChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossQTEBreakPointMax);
		DocBattle->OnbBossQTEBreakableChanged.AddUObject(this, &UB2UIBossStatus::OnChangedBossIsQTEBreakable);
	}
}

void UB2UIBossStatus::UnbindDoc()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnBossHPCurrentChanged.RemoveAll(this);
		DocBattle->OnBossHPMaxChanged.RemoveAll(this);
		DocBattle->OnBossMobClassChanged.RemoveAll(this);
		DocBattle->OnBossMobClassVariationChanged.RemoveAll(this);

		DocBattle->OnBossQTEBreakPointCurrentChanged.RemoveAll(this);
		DocBattle->OnBossQTEBreakPointMaxChanged.RemoveAll(this);
		DocBattle->OnbBossQTEBreakableChanged.RemoveAll(this);
	}
	SetDoc(nullptr);
}

void UB2UIBossStatus::SetName(const FText& InText)
{
	if (TB_BossName.IsValid())
		TB_BossName->SetText(InText);
}

void UB2UIBossStatus::SetHP(float CurHP, float MaxHP)
{
	if (UIP_ProgressBar_Boss.IsValid())
		UIP_ProgressBar_Boss->SetPercentByValue(CurHP , MaxHP);
}

void UB2UIBossStatus::SetInfluence(const FText &InText)
{
	if (TB_BossInfluence.IsValid())
	{
		TB_BossInfluence->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_BossInfluence->SetText(InText);
	}
}

void UB2UIBossStatus::UpdateBossInfo(ENPCClass InClass, ENPCClassVariation InVariation)
{
	UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
	if (MobClassInfoBox)
	{
		UB2NPCSingleClassInfo* SingleMobInfo = MobClassInfoBox->GetNPCSingleClassInfo(InClass, InVariation);
		if (SingleMobInfo)
		{
			SetName(SingleMobInfo->GetLocalizedName());
			HideInfluence();

			if (SingleMobInfo->InfluenceType != EInfluenceType::EIT_End)
			{
				if (UB2InfluenceSkillInfo* InfluenceInfo = StaticFindInfluenceSkillInfo())
				{
					if (const FSingleInfluenceSkillInfo *SingleSkillInfo = InfluenceInfo->GetSingleInfoOfID(SingleMobInfo->InfluenceType))
					{
						const int32 SkillId = SingleSkillInfo->SkillID;
						SetInfluence(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, SkillId)));
					}
				}
			}
		}
	}
}

void UB2UIBossStatus::HideInfluence()
{
	if (TB_BossInfluence.IsValid())
		TB_BossInfluence->SetVisibility(ESlateVisibility::Hidden);
}
//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIBossStatus::OnChangedBossHPCurrent(class UB2UIDocBase* Sender, float BossHPCurrent, float PrevBossHPCurrent)
{
	if (BossHPCurrent != PrevBossHPCurrent)	//�ߺ� ���� ����
	{
		auto DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (!DocBattle)return;

		SetHP(BossHPCurrent, DocBattle->GetBossHPMax());

		if (PrevBossHPCurrent > 0.f)
		SetDamaged(PrevBossHPCurrent - BossHPCurrent);
	}
}

void UB2UIBossStatus::OnChangedBossHPMax(class UB2UIDocBase* Sender, float BossHPMax, float PrevBossHPMax)
{
	if (BossHPMax != PrevBossHPMax)	//�ߺ� ���� ����
	{
		auto DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (!DocBattle)return;

		SetHP(DocBattle->GetBossHPCurrent(), BossHPMax);
	}
}

void UB2UIBossStatus::OnChangedBossMobClass(class UB2UIDocBase* Sender, ENPCClass BossMobClass, ENPCClass PrevBossMobClass)
{
	// ���⼭ UpdateBossInfo �� ������ �ʴ´�. MobClass �� Variation ���� ���ÿ� ���;� �ϴµ� �ϳ��� ���� �ǵ���� ���� ���¿��� ���ϰ� �Ǹ� �ε� �ȵ� Ŭ���� �����Ͱ� �ε��Ǹ鼭 ������ ������ �� �ִ�.
	//if (BossMobClass != PrevBossMobClass)
	//{
	//	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	//	if (!DocBattle)return;

	//	UpdateBossInfo(BossMobClass, DocBattle->GetBossMobClassVariation());
	//}
}

void UB2UIBossStatus::OnChangedBossMobClassVariation(class UB2UIDocBase* Sender, ENPCClassVariation BossMobClassVariation, ENPCClassVariation PrevBossMobClassVariation)
{
	// ���⼭ UpdateBossInfo �� ������ �ʴ´�. MobClass �� Variation ���� ���ÿ� ���;� �ϴµ� �ϳ��� ���� �ǵ���� ���� ���¿��� ���ϰ� �Ǹ� �ε� �ȵ� Ŭ���� �����Ͱ� �ε��Ǹ鼭 ������ ������ �� �ִ�.
	//if (BossMobClassVariation != PrevBossMobClassVariation)
	//{
	//	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	//	if (!DocBattle)return;

	//	UpdateBossInfo(DocBattle->GetBossMobClass(), BossMobClassVariation);
	//}
}

void UB2UIBossStatus::OnChangedBossQTEBreakPointCurrent(class UB2UIDocBase* Sender, float PointCurrent, float PrevPointCurrent)
{
	if (PointCurrent != PrevPointCurrent)
	{
		UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (DocBattle && UIP_ProgressBar_Boss.IsValid())
		{
			UIP_ProgressBar_Boss->SetProgressOfSecond(PointCurrent, DocBattle->GetBossQTEBreakPointMax());

			// Point �ٽ� ���� �� ������ ���⼭ �����ϵ���. �� ó������ ����� ���� �ϴ±� ��
			if (PointCurrent > PrevPointCurrent && PointCurrent == DocBattle->GetBossQTEBreakPointMax())
			{
				PlayQTEBreakPointRefreshAnim();
			}
		}
	}
}

void UB2UIBossStatus::OnChangedBossQTEBreakPointMax(class UB2UIDocBase* Sender, float PointMax, float PrevPointMax)
{
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (DocBattle && UIP_ProgressBar_Boss.IsValid())
	{
		UIP_ProgressBar_Boss->SetProgressOfSecond(DocBattle->GetBossQTEBreakPointCurrent(), PointMax);
	}
}

void UB2UIBossStatus::OnChangedBossIsQTEBreakable(class UB2UIDocBase* Sender, bool bBreakable, bool bPrevBreakable)
{
	if (UIP_ProgressBar_Boss.IsValid())
	{
		UIP_ProgressBar_Boss->SetShowSecond(bBreakable); // Second PB �� Break point ǥ�ÿ� ����.
	}
}