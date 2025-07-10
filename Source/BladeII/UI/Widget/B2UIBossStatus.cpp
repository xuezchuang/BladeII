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
	SetDoc(DocBattle); //현재 보스 정보가 별도의 Doc으로 없고 Battle에 있음.
	if (DocBattle)
	{
		//최초 일회 세팅
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
	if (BossHPCurrent != PrevBossHPCurrent)	//중복 연출 방지
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
	if (BossHPMax != PrevBossHPMax)	//중복 연출 방지
	{
		auto DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (!DocBattle)return;

		SetHP(DocBattle->GetBossHPCurrent(), BossHPMax);
	}
}

void UB2UIBossStatus::OnChangedBossMobClass(class UB2UIDocBase* Sender, ENPCClass BossMobClass, ENPCClass PrevBossMobClass)
{
	// 여기서 UpdateBossInfo 를 콜하지 않는다. MobClass 와 Variation 둘이 동시에 들어와야 하는데 하나만 값이 의도대로 들어온 상태에서 콜하게 되면 로딩 안된 클래스 데이터가 로딩되면서 끊김을 유발할 수 있다.
	//if (BossMobClass != PrevBossMobClass)
	//{
	//	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	//	if (!DocBattle)return;

	//	UpdateBossInfo(BossMobClass, DocBattle->GetBossMobClassVariation());
	//}
}

void UB2UIBossStatus::OnChangedBossMobClassVariation(class UB2UIDocBase* Sender, ENPCClassVariation BossMobClassVariation, ENPCClassVariation PrevBossMobClassVariation)
{
	// 여기서 UpdateBossInfo 를 콜하지 않는다. MobClass 와 Variation 둘이 동시에 들어와야 하는데 하나만 값이 의도대로 들어온 상태에서 콜하게 되면 로딩 안된 클래스 데이터가 로딩되면서 끊김을 유발할 수 있다.
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

			// Point 다시 차는 거 조건을 여기서 감지하도록. 맨 처음에도 여기로 가긴 하는군 ㅋ
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
		UIP_ProgressBar_Boss->SetShowSecond(bBreakable); // Second PB 가 Break point 표시에 쓰임.
	}
}