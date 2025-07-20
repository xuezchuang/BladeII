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
	SetDoc(DocBattle); //泅犁 焊胶 沥焊啊 喊档狼 Doc栏肺 绝绊 Battle俊 乐澜.
	if (DocBattle)
	{
		//弥檬 老雀 技泼
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
	if (BossHPCurrent != PrevBossHPCurrent)	//吝汗 楷免 规瘤
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
	if (BossHPMax != PrevBossHPMax)	//吝汗 楷免 规瘤
	{
		auto DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (!DocBattle)return;

		SetHP(DocBattle->GetBossHPCurrent(), BossHPMax);
	}
}

void UB2UIBossStatus::OnChangedBossMobClass(class UB2UIDocBase* Sender, ENPCClass BossMobClass, ENPCClass PrevBossMobClass)
{
	// 咯扁辑 UpdateBossInfo 甫 妮窍瘤 臼绰促. MobClass 客 Variation 笛捞 悼矫俊 甸绢客具 窍绰单 窍唱父 蔼捞 狼档措肺 甸绢柯 惑怕俊辑 妮窍霸 登搁 肺爹 救等 努贰胶 单捞磐啊 肺爹登搁辑 谗辫阑 蜡惯且 荐 乐促.
	//if (BossMobClass != PrevBossMobClass)
	//{
	//	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	//	if (!DocBattle)return;

	//	UpdateBossInfo(BossMobClass, DocBattle->GetBossMobClassVariation());
	//}
}

void UB2UIBossStatus::OnChangedBossMobClassVariation(class UB2UIDocBase* Sender, ENPCClassVariation BossMobClassVariation, ENPCClassVariation PrevBossMobClassVariation)
{
	// 咯扁辑 UpdateBossInfo 甫 妮窍瘤 臼绰促. MobClass 客 Variation 笛捞 悼矫俊 甸绢客具 窍绰单 窍唱父 蔼捞 狼档措肺 甸绢柯 惑怕俊辑 妮窍霸 登搁 肺爹 救等 努贰胶 单捞磐啊 肺爹登搁辑 谗辫阑 蜡惯且 荐 乐促.
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

			// Point 促矫 瞒绰 芭 炼扒阑 咯扁辑 皑瘤窍档废. 盖 贸澜俊档 咯扁肺 啊变 窍绰焙 せ
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
		UIP_ProgressBar_Boss->SetShowSecond(bBreakable); // Second PB 啊 Break point 钎矫俊 静烙.
	}
}