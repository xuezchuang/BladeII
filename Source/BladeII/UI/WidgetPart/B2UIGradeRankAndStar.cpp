#include "B2UIGradeRankAndStar.h"
#include "B2UIPVPResult.h"
#include "B2UIDocHelper.h"
#include "MovieScene.h"
#include "BladeIIUtil.h"

UB2UIGradeRankAndStar::FRankAnimInfo::FRankAnimInfo(const FName& _InstanceName, const FName& _WidgetName, const int32 StarCount, TWeakObjectPtr<UWidgetAnimation>& _RankUpAnimation, float InAnimSpeed)
	: WidgetName(_WidgetName)
	, InstanceName(_InstanceName)
	, StarCountAfterAnimation(StarCount)
	, AnimSpeed(InAnimSpeed)
	, RankUpAnimation(_RankUpAnimation)
{
}

UB2UIGradeRankAndStar::FRankAnimInfo::~FRankAnimInfo()
{
	RankUpAnimation.Reset();
}

UB2UIGradeRankAndStar::UB2UIGradeRankAndStar(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIGradeRankAndStar::Init()
{
	Super::Init();

	SetMaxElementNum(MaxNum);

	IsRankUpAnimation = false; // 局聪皋捞记阑 荤侩窍绰瘤, 窜鉴 喊 钎矫父 窍绰瘤
	IsGradeUp		  = false; // 殿鞭 诀 局聪皋捞记阑 荤侩窍绰瘤
	IsAfterAdding     = false; // 力磊府俊辑 喊 局聪皋捞记苞 殿鞭诀 局聪皋捞记 + 喊 局聪皋捞记牢瘤 备盒
	
	UpGradeAnimationSpeed	= 1.0f;
	DownGradeAnimationSpeed = 1.0f;
}

void UB2UIGradeRankAndStar::SetGradeRank(int32 Rank, int32 StarNum)
{
	if (IMG_GradeRank.IsValid())
	{
		if (RankMATs.IsValidIndex(Rank - 1))
		{
			IMG_GradeRank->SetBrushFromMaterial(RankMATs[Rank - 1]);
		}
	}

	if (TB_Rank.IsValid())
	{
		if (Rank > 3)
		{
			TB_Rank->SetText(FText::AsNumber(Rank));
			TB_Rank->SetVisibility(ESlateVisibility::Visible);
		}
		else
			TB_Rank->SetVisibility(ESlateVisibility::Hidden);
	}
	SetStarNum(Rank, StarNum);
}

void UB2UIGradeRankAndStar::SetStarNum(int32 Rank, int32 StarNum)
{
	if (Rank == 1)
	{
		if (CON_Elements.IsValid())
			CON_Elements->SetVisibility(StarNum > 3 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		if (OV_OverGrade.IsValid())
		{
			OV_OverGrade->SetVisibility(StarNum > 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}

		if (StarNum > 3 && TB_NumStarOverGrade.IsValid())
			TB_NumStarOverGrade->SetText(FText::Format(FText::FromString(TEXT("x {0}")), FText::AsNumber(StarNum)));
		else
			SetCurElementNum(StarNum);
	}
	else
	{
		if (CON_Elements.IsValid())
			CON_Elements->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (OV_OverGrade.IsValid())
			OV_OverGrade->SetVisibility(ESlateVisibility::Collapsed);

		SetCurElementNum(StarNum);
	}
}

void UB2UIGradeRankAndStar::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_GradeRank);
	GET_SLOT(UTextBlock, TB_Rank);

	GET_SLOT(UOverlay, OV_OverGrade);
	GET_SLOT(UTextBlock, TB_NumStarOverGrade);
}

void  UB2UIGradeRankAndStar::LoadAnimation(const FName& AnimName, int32 AfterStarCount, float InPlaySpeed)
{
	auto Anim = GetAnimation(this, AnimName);
	if (Anim.IsValid())
	{
		StarAnims.Add(AnimName, FRankAnimInfo(Anim->GetFName(), AnimName, AfterStarCount, Anim, InPlaySpeed));
	}
}

void UB2UIGradeRankAndStar::SetUseAnimation()
{
	LoadAnimation(FName("Star_On_01"), 1, UpGradeAnimationSpeed);
	LoadAnimation(FName("Star_On_02"), 2, UpGradeAnimationSpeed);
	LoadAnimation(FName("Star_On_03"), 3, UpGradeAnimationSpeed);
	LoadAnimation(FName("off_01"), 0, DownGradeAnimationSpeed);
	LoadAnimation(FName("off_02"), 1, DownGradeAnimationSpeed);
	LoadAnimation(FName("off_03"), 2, DownGradeAnimationSpeed);
	LoadAnimation(FName("Number_fx_Anim"), -1, 1.0f);
	LoadAnimation(FName("Sheild_fx_Anim"), -1, 1.0f);
	LoadAnimation(FName("ANIM_Grade_Lose"), -1, 1.0f);

	if (StarAnims.Num() == 0)
	{
		IsRankUpAnimation = false;
		return;
	}

	IsRankUpAnimation = true;
}

void UB2UIGradeRankAndStar::StartStarAnimation(int32 BeforeStarNum, int32 StarNum, bool GradeUp)
{
	if (IsRankUpAnimation == false)
	{
		StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::STATE_END);
		return;
	}

	NextAnimation = EStateAfterGradeUp::STATE_END;
	auto Grade = UB2UIPVPResult::GetGrade(GetB2GameModeType(this));
	auto BeforeGrade = UB2UIPVPResult::GetGrade(GetB2GameModeType(this), false);
	IsGradeUp = BeforeGrade > Grade;

	if (Grade == BeforeGrade && Grade == 1 && StarNum > 3)
	{
		StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::GRADE_ANIM_START);
		return;
	}

	if (GradeUp) // 殿鞭 诀 老锭
	{
		int32 StarGap = BeforeStarNum > StarNum ?
			StarNum + 3 - BeforeStarNum : StarNum - BeforeStarNum;

		if (StarGap <= 0)
		{
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::STATE_END);
			return;
		}

		if (BeforeStarNum == 0)
		{
			if (StarGap == 1)
			{
				PlayStarAnimation("Star_On_01");
				NextAnimation = EStateAfterGradeUp::STATE_END;
			}
			else if (StarGap == 2)
			{
				PlayStarAnimation("Star_On_01");
				NextAnimation = EStateAfterGradeUp::ADD_STAR;
			}
		}
		else if (BeforeStarNum == 1) // 扁粮俊 1俺老锭
		{
			// play 1
			if (StarGap == 1)
			{
				PlayStarAnimation("Star_On_02");
				NextAnimation = EStateAfterGradeUp::STATE_END;
			}
			if (StarGap == 2)
			{
				PlayStarAnimation("Star_On_02");
				NextAnimation = EStateAfterGradeUp::ADD_STAR;
			}
		}
		else if (BeforeStarNum == 2) // 扁粮俊 2俺老锭
		{
			if (StarGap == 1)
			{
				PlayStarAnimation("Star_On_03");
				NextAnimation = EStateAfterGradeUp::STATE_END;
			}
			if (StarGap == 2)
			{
				//殿鞭诀
				PlayStarAnimation("Star_On_03");
				NextAnimation = EStateAfterGradeUp::GRADE_ANIM_START;
			}
		}
		else if (BeforeStarNum == 3) // 扁粮俊 3俺老锭 // 荐沥
		{
			//殿鞭诀 龋免
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::GRADE_ANIM_START);
		}
		else if (BeforeStarNum > 3)
		{
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::STATE_END);
		}
	}
	else // 殿鞭促款
	{
		if (BeforeStarNum > 3)
		{
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::STATE_END);
		}

		else if (BeforeStarNum == 3)
		{
			PlayStarAnimation("off_03");
			NextAnimation = EStateAfterGradeUp::STATE_END;
		}
		else if (BeforeStarNum == 2)
		{
			PlayStarAnimation("off_02");
			NextAnimation = EStateAfterGradeUp::STATE_END;
		}
		else if (BeforeStarNum == 1)
		{
			const int32 MAX_GRADE = 10;

			PlayStarAnimation("off_01");
			NextAnimation = UB2UIPVPResult::GetGrade(GetB2GameModeType(this), false) == MAX_GRADE ? EStateAfterGradeUp::STATE_END : EStateAfterGradeUp::GRADE_ANIM_START;
		}
	}
}

void UB2UIGradeRankAndStar::StartRankUpAnimation(int32 BeforeRank, int32 Rank, int32 StarNum)
{
	SetGradeRank(Rank, IsGradeUp ? 0 : StarNum);
	PlayStarAnimation(BeforeRank > Rank ? "Number_fx_Anim" : "ANIM_Grade_Lose");
}

// 0 = 场 1 = 1锅局聪 2 = 2锅局聪 3 = 3锅局聪 4 = 4锅局聪 5 = 殿鞭诀
void UB2UIGradeRankAndStar::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (auto* Anim = StarAnims.Find(Animation->GetMovieScene()->GetFName()))
	{
		// 促澜 局聪皋捞记捞 抗沥登绢乐绰 版快
		if (EStateAfterGradeUp::ADD_STAR == NextAnimation)
		{
			AddStarAnimation(Anim);
		}
		else // 抗沥 救等版快
		{
			NoAddStarAnimation(Anim);
		}
	}
}

void UB2UIGradeRankAndStar::AddStarAnimation(FRankAnimInfo* InAnim)
{
	if (InAnim->StarCountAfterAnimation == 1)
	{
		PlayStarAnimation("Star_On_02");
	}
	else if (InAnim->StarCountAfterAnimation == 2)
	{
		PlayStarAnimation("Star_On_03");
	}
	SetStarNum(UB2UIPVPResult::GetGrade(GetB2GameModeType(this)), InAnim->StarCountAfterAnimation);
	NextAnimation = EStateAfterGradeUp::STATE_END;
}

void UB2UIGradeRankAndStar::NoAddStarAnimation(FRankAnimInfo* InAnim)
{
	// 蒋靛 局聪皋捞记捞绊, 殿鞭诀牢版快
	if (InAnim->StarCountAfterAnimation == -1 && IsGradeUp) // 规菩局聪皋捞记捞搁 // 弊府绊 诀局聪皋捞记
	{
		IsGradeUp = false;
		IsAfterAdding = true; // 规菩啊 栋乐绰 惑怕俊辑 殿鞭诀 局聪皋捞记阑 柳青
		StartStarAnimation(0, UB2UIPVPResult::GetStarCount(GetB2GameModeType(this)), true);
		return;
	}
	else
	{
		if (IsAfterAdding) // 规菩啊 栋乐绰 惑怕俊辑 殿鞭诀 局聪皋捞记阑 柳青吝牢版快
		{
			IsAfterAdding = false;
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(EStateAfterGradeUp::GRADE_UP_END);
			SetStarNum(UB2UIPVPResult::GetGrade(GetB2GameModeType(this)), UB2UIPVPResult::GetStarCount(GetB2GameModeType(this)));
			NextAnimation = EStateAfterGradeUp::STATE_END;
			return;
		}
		else
		{
			StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Signal(InAnim->StarCountAfterAnimation == -1 ? EStateAfterGradeUp::GRADE_UP_END : NextAnimation);
			SetStarNum(UB2UIPVPResult::GetGrade(GetB2GameModeType(this)), IsGradeUp ? 0 : UB2UIPVPResult::GetStarCount(GetB2GameModeType(this)));
		}
	}
	NextAnimation = EStateAfterGradeUp::STATE_END;
}

void UB2UIGradeRankAndStar::PlayStarAnimation(const FName& AnimationName)
{
	if (StarAnims.Contains(AnimationName) && StarAnims[AnimationName].RankUpAnimation.IsValid())
		PlayAnimation(StarAnims[AnimationName].RankUpAnimation.Get(), 0.0f, 1, EUMGSequencePlayMode::Forward, StarAnims[AnimationName].AnimSpeed);
}
