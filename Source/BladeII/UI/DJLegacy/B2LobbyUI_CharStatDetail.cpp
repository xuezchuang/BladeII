// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_CharStatDetail.h"
//#include "BladeII.h"

#include "B2LobbyUI_CharStatDetailPopup.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynText_Multiple.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"

FStatTextInfo::FStatTextInfo()
{
	ItemOption = EUnitedCombatOptions::UCO_End;
	StatValue = 0;
	IsEmptyOption = true;
	CreatedStatTextBlock = nullptr;
}

FStatTextInfo::FStatTextInfo(EUnitedCombatOptions InItemOption, float InStatValue, bool InEmptyOption, TWeakObjectPtr<class UB2DynText_Multiple> InCreatedStatTextBlock)
{
	ItemOption = InItemOption;
	StatValue = InStatValue;
	IsEmptyOption = InEmptyOption;
	CreatedStatTextBlock = InCreatedStatTextBlock;
}

FStatTextInfo::~FStatTextInfo()
{
	CreatedStatTextBlock = nullptr;
}

bool FStatTextInfo::IsEmpty()
{
	return (ItemOption == EUnitedCombatOptions::UCO_End);
}

bool FStatTextInfo::IsEmptyValue()
{
	return (GetConvertValue() < FLT_EPSILON);
}

bool FStatTextInfo::GetIsEmptyOption()
{
	return IsEmptyOption;
}

float FStatTextInfo::GetConvertValue()
{
	return GetConvertItemOptionValue(CombatStatEval::GetItemOptionOfUnitedOption(ItemOption), StatValue, false);
}

UB2LobbyUI_CharStatDetail::UB2LobbyUI_CharStatDetail(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StatTextDisplayClass = NULL;
	CurPCClass = EPCClass::EPC_End;
	OldSelectedSlotNum = -1;
}

void UB2LobbyUI_CharStatDetail::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_OffensiveStats)
	GET_SLOT(UVerticalBox, VB_DefensiveStats)
	GET_SLOT(UVerticalBox, VB_HealthStats)
	GET_SLOT(UVerticalBox, VB_AdventureStats)

	GET_SLOT(UTextBlock, TB_StatDetailTitle)
	GET_SLOT(UTextBlock, TB_AttackStatTitle)
	GET_SLOT(UTextBlock, TB_DefenseStatTitle)
	GET_SLOT(UTextBlock, TB_HealthStatTitle)
	GET_SLOT(UTextBlock, TB_AdventureStatTitle)

	GET_SLOT(UB2Button, BTN_Close)

	UpdateStaticTexts();
}

void UB2LobbyUI_CharStatDetail::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
		
	CurPCClass = EPCClass::EPC_End;
	SetPositionInViewport(FVector2D(0.0f, 0.0f)); // 화면 좌측에 붙인다.

	UpdateStats(); // 사실상 이게 열려 있는 동안 처음 열릴 때 말고는 불릴 일 없을 듯. -> 아이템 갈아입는 게 있다 ㄷㄷ
}

void UB2LobbyUI_CharStatDetail::DestroySelf()
{
	Super::DestroySelf();
	DestroyStatTexts();
}

void UB2LobbyUI_CharStatDetail::UpdateStaticTexts()
{
	if (TB_StatDetailTitle.IsValid())
	{
		TB_StatDetailTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharStatDetail_Title"))));
	}

	if (TB_AttackStatTitle.IsValid())
	{
		TB_AttackStatTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))));
	}
	if (TB_DefenseStatTitle.IsValid())
	{
		TB_DefenseStatTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))));
	}	
	if (TB_HealthStatTitle.IsValid())
	{
		TB_HealthStatTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health"))));
	}
	if (TB_AdventureStatTitle.IsValid())
	{
		TB_AdventureStatTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharStatDetailCategory_Adventure"))));
	}
}

void UB2LobbyUI_CharStatDetail::UpdateStats()
{
	EPCClass DisplayPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End; // 영웅관리 창에서 뜨게 될 것.
	
	UpdateStats(DisplayPCClass);
}

void UB2LobbyUI_CharStatDetail::UpdateStats(EPCClass DisplayPCClass, bool bFindAccountInfo)
{
	DestroyStatTexts();

	if (BTN_Close.IsValid())
	{
		if (bFindAccountInfo)
		{
			SetClose(false);
			BTN_Close->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SetClose(true);
			BTN_Close->SetVisibility(ESlateVisibility::Visible);
		}
	}

	// 캐릭터에 적용된 값을 가져오는 게 있고 아이템 옵션 수치를 직접 보는 게 있음
	CurPCClass = DisplayPCClass;

	FB2ModPlayerInfo& OtherPlayer = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();

	// 종류별로 해서 나열하는 순서가 있음.
	int32 RowIndex = 0;
	{
		CreateSingleStatBasicPoint(VB_OffensiveStats.Get(), RowIndex, EItemPrimaryPointType::EIPP_Attack,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))),
			(int32)CombatStatEval::GetPCAttack(DisplayPCClass, bFindAccountInfo ? &OtherPlayer : nullptr));
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncNormalDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncCriticalRate, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncBossDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Skill_IncSkillDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_General, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Offense_IncAdditionalDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		/*CreateSingleStatItemOption(VB_OffensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Misc_LastNormalAttackKnockbackRate, DisplayPCClass, bFindAccountInfo);
		++RowIndex;*/
	}

	{
		CreateSingleStatBasicPoint(VB_DefensiveStats.Get(), RowIndex, EItemPrimaryPointType::EIPP_Defense,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))),
			(int32)CombatStatEval::GetPCDefense(DisplayPCClass, bFindAccountInfo ? &OtherPlayer : nullptr));
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecBossDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecRangedDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_ResistCritical, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_DefensiveStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Defense_DecAdditionalDamage, DisplayPCClass, bFindAccountInfo);
		++RowIndex;
	}

	{
		CreateSingleStatBasicPoint(VB_HealthStats.Get(), RowIndex, EItemPrimaryPointType::EIPP_Health,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health"))),
			(int32)CombatStatEval::GetPCHealth(DisplayPCClass, bFindAccountInfo ? &OtherPlayer : nullptr));
		++RowIndex;

		CreateSingleStatItemOption(VB_HealthStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_HealthStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_HealthStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, DisplayPCClass, bFindAccountInfo);
		++RowIndex;
	}

	{
		// 모험 관련 기본 수치는 없음.
		CreateSingleStatItemOption(VB_AdventureStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Misc_IncMoveSpeed, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_AdventureStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Misc_DecTagCooltime, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_AdventureStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Misc_AdditionalExp, DisplayPCClass, bFindAccountInfo);
		++RowIndex;

		CreateSingleStatItemOption(VB_AdventureStats.Get(), RowIndex, EUnitedCombatOptions::UCO_Misc_AdditionalGold, DisplayPCClass, bFindAccountInfo);
		++RowIndex;
	}
}

void UB2LobbyUI_CharStatDetail::CreateSingleStatBasicPoint(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, EItemPrimaryPointType InPrimaryPointType, const FText& InStatLabelText, int32 InStatValue)
{
	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(StatTextDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		ThisCreated->SetDynText(RowIndexInVB, InStatLabelText, FText::FromString(FString::Printf(TEXT("%d"), InStatValue))); // 걍 정수 표현.
		if (InStatValue == 0)
			ThisCreated->SetChangeEmptyValueColor(1);
		ThisCreated->SetVisibility_BtnSelect(true);
		ThisCreated->FClickSelectBtn.BindUObject(this, &UB2LobbyUI_CharStatDetail::OnClickedStatSlot);

		CreatedStatTextBlockInfos.Add(RowIndexInVB, FStatTextInfo(CombatStatEval::GetUnitedOptionOfItemOption(CombatStatEval::GetPrimPointIncOption(InPrimaryPointType)), InStatValue, false, ThisCreated));
	}
}

void UB2LobbyUI_CharStatDetail::CreateSingleStatItemOption(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, EUnitedCombatOptions InUnitedOptionType, EPCClass InDisplayPCClas, bool bFindAccountInfo)
{
	// 다른 유저 정보 혹은 나의 로컬 캐릭터 정보
	ICharacterDataStore* DisplayCharDataStore = NULL; // 여긴 바로 삼항연산자가 안 먹힘.
	if (bFindAccountInfo)
	{
		DisplayCharDataStore = &BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();
	}
	else
	{
		DisplayCharDataStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	}

	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(StatTextDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		// 여기서 리턴하는 값은 실제 적용되는 값이고 표시하는 값과는 다를 수 있다.		
		EItemOption MapppedItemOption = CombatStatEval::GetItemOptionOfUnitedOption(InUnitedOptionType);
		checkSlow(MapppedItemOption != EItemOption::EIO_End); // 적어도 EItemOption 에는 맵핑이 되는 옵션이어야 한다. SkillOption 에만 맵핑되는 옵션은 여기서 표시하지 않는다.

		float EffectiveValue = CombatStatEval::GetUnitedOptionStatusValue(InDisplayPCClas, InUnitedOptionType, DisplayCharDataStore);
		ThisCreated->SetDynText(RowIndexInVB,GetLOCTextOfItemOption(MapppedItemOption, InDisplayPCClas), GetItemOptionValueDisplayText(MapppedItemOption, EffectiveValue, false));

		float DisplayValue = GetConvertItemOptionValue(MapppedItemOption, EffectiveValue, false);

		CombatStatInfoRawDatas CombatStatInfoData;
		CombatStatEval::GetUnitedOptionStatusRawValues(InDisplayPCClas, InUnitedOptionType, CombatStatInfoData, DisplayCharDataStore);
		const bool IsEmptyOption = CombatStatInfoData.IsEmpty();

		if(IsEmptyOption == true)
			ThisCreated->SetChangeEmptyValueColor(1);

		ThisCreated->SetVisibility_BtnSelect(true);
		ThisCreated->FClickSelectBtn.BindUObject(this, &UB2LobbyUI_CharStatDetail::OnClickedStatSlot);

		CreatedStatTextBlockInfos.Add(RowIndexInVB, FStatTextInfo(InUnitedOptionType, EffectiveValue, IsEmptyOption, ThisCreated));
	}
}

void UB2LobbyUI_CharStatDetail::OnClickedStatSlot(int32 InSlotNum, FVector2D ClickPosition)
{
	if (CachedLobbyGM == nullptr)
		return;

	if (CreatedStatTextBlockInfos.Contains(InSlotNum) == false)
		return;
	
	auto StatTextInfo = CreatedStatTextBlockInfos[InSlotNum];

	if (StatTextInfo.GetIsEmptyOption())
		return;
		
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetailPopup, FDJLegacySubpopupAddPanelInfo()); // 세부의 세부 스탯 pop-up

	if (UB2LobbyUI_CharStatDetailPopup* DetailPopup = Cast<UB2LobbyUI_CharStatDetailPopup>(CachedLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_CharStatDetailPopup)))
	{	
		DetailPopup->UpdateStats(CurPCClass, CreatedStatTextBlockInfos[InSlotNum].ItemOption);		
		DetailPopup->UpdatePosition(ClickPosition);
		DetailPopup->OnClose.BindUObject(this, &UB2LobbyUI_CharStatDetail::OnCloseDetailPopup);
	}

	if (StatTextInfo.CreatedStatTextBlock.IsValid())
		StatTextInfo.CreatedStatTextBlock->SetChangeSelectButtonColor();

	OldSelectedSlotNum = InSlotNum;
}

void UB2LobbyUI_CharStatDetail::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_CharStatDetail::OnCloseDetailPopup()
{
	if (CreatedStatTextBlockInfos.Contains(OldSelectedSlotNum) == false)
		return;

	auto StatTextInfo = CreatedStatTextBlockInfos[OldSelectedSlotNum];
	if (StatTextInfo.CreatedStatTextBlock.IsValid())
		StatTextInfo.CreatedStatTextBlock->SetChangeDefaultSelectButtonColor();

	OldSelectedSlotNum = -1;
}

void UB2LobbyUI_CharStatDetail::DestroyStatTexts()
{
	for (auto CurrElem : CreatedStatTextBlockInfos)
	{		
		if (CurrElem.Value.CreatedStatTextBlock.IsValid())
			CurrElem.Value.CreatedStatTextBlock->DestroySelf();
	}

	CreatedStatTextBlockInfos.Empty();
}

void UB2LobbyUI_CharStatDetail::SetDelegateOnClose(const FDelegateCharStatDetailClose& Delegate)
{
	OnClose = Delegate;
}

void UB2LobbyUI_CharStatDetail::OnCloseButtonClicked()
{
	OnClose.ExecuteIfBound();

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetail);
}

// 테스트 용도로 인게임 전투 화면에서 마찬가지 옵션 세트들을 뿌리는 기능
#if !UE_BUILD_SHIPPING
int32 UB2LobbyUI_CharStatDetail::DebugDrawPlayerStatMode = 0;
EPCClass UB2LobbyUI_CharStatDetail::DebugDrawPlayerStatClass = EPCClass::EPC_End;
void UB2LobbyUI_CharStatDetail::DebugDrawPlayerPrimStatOnScreen(EPCClass InLocalMainCharClass, EPCClass InLocalSubCharClass, EItemPrimaryPointType InPrimPointType, float InStartX, float InStartY)
{
	// 로컬 캐릭터 용도만..
	ICharacterDataStore* LocalCDS = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	float MainCharStatValue = 0.0f;
	float SubCharStatValue = 0.0f;
	FString PrimPointTypeLabel;

	if (InPrimPointType == EItemPrimaryPointType::EIPP_Attack)
	{
		MainCharStatValue = CombatStatEval::GetPCAttack(InLocalMainCharClass, LocalCDS);
		SubCharStatValue = CombatStatEval::GetPCAttack(InLocalSubCharClass, LocalCDS);
		PrimPointTypeLabel = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")).ToString();
	}
	else if (InPrimPointType == EItemPrimaryPointType::EIPP_Defense)
	{
		MainCharStatValue = CombatStatEval::GetPCDefense(InLocalMainCharClass, LocalCDS);
		SubCharStatValue = CombatStatEval::GetPCDefense(InLocalSubCharClass, LocalCDS);
		PrimPointTypeLabel = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")).ToString();
	}
	else if (InPrimPointType == EItemPrimaryPointType::EIPP_Health)
	{
		MainCharStatValue = CombatStatEval::GetPCHealth(InLocalMainCharClass, LocalCDS);
		//SubCharStatValue = CombatStatEval::GetPCHealth(InLocalSubCharClass, LocalCDS); // 체력은 딱히 서브 캐릭터 수치가 필요는 없을 것.
		PrimPointTypeLabel = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health")).ToString();
	}
	
	FString DisplayString;	
	if (InPrimPointType != EItemPrimaryPointType::EIPP_Health)
	{
		DisplayString = FString::Printf(TEXT("%s : %d"), *PrimPointTypeLabel, (int32)MainCharStatValue);
		if (InLocalSubCharClass != EPCClass::EPC_End)
		{ // 실질적으로는 메인과 태그 캐릭터 합친 수치가 필요할 것. 팀대전이면 셋을 합쳐야 되는데 그건 필요해지면 추가로..
			DisplayString += FString::Printf(TEXT(" (Main+Tag United : %d)"), (int32)MainCharStatValue + (int32)SubCharStatValue);
		}
	}
	else
	{
		// 체력의 경우 현재 체력 수치도 표시하는데 적당히 UIDoc 에서 빼옴. 대체로 메인 캐릭터 수치를 보이면 되긴 할껀데 여기로 플레이어 캐릭터를 넘겨주는 게 나으려나 ㅡㅡ
		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		int32 CurrPCHealth = 0.0f;
		if (DocBattle)
		{
			if (IntToPCClass(DocBattle->GetCurPCClass()) == InLocalMainCharClass)
			{
				CurrPCHealth = DocBattle->GetCurrPCHealth();
			}
			else if (IntToPCClass(DocBattle->GetTagPCClass()) == InLocalMainCharClass)
			{
				CurrPCHealth = DocBattle->GetTagPCHealth();
			}
		}
		// 체력은 메인 태그 합쳐서 사용 안함.
		DisplayString = FString::Printf(TEXT("%s : %d/%d"), *PrimPointTypeLabel, (int32)CurrPCHealth, (int32)MainCharStatValue);
	}

	DrawDebugText(InStartX, InStartY, DisplayString, FLinearColor::Yellow, 20); // 폰트컬러랑 크기는 여기서 임의로다가.
}
void UB2LobbyUI_CharStatDetail::DebugDrawPlayerSingleOptionOnScreen(EPCClass InLocalCharClass, EUnitedCombatOptions DisplayOption, float InStartX, float InStartY)
{
	// 로컬 캐릭터 용도만..
	ICharacterDataStore* LocalCDS = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	// 여기서 리턴하는 값은 실제 적용되는 값이고 표시하는 값과는 다를 수 있다.
	float EffectiveValue = CombatStatEval::GetUnitedOptionStatusValue(InLocalCharClass, DisplayOption, LocalCDS);

	EItemOption MapppedItemOption = CombatStatEval::GetItemOptionOfUnitedOption(DisplayOption);
	checkSlow(MapppedItemOption != EItemOption::EIO_End); // 적어도 EItemOption 에는 맵핑이 되는 옵션이어야 한다. SkillOption 에만 맵핑되는 옵션은 여기서 표시하지 않는다.
	
	FString DisplayString = FString::Printf(TEXT("%s : %s"),
		*(GetLOCTextOfItemOption(MapppedItemOption, InLocalCharClass).ToString()),
		*(GetItemOptionValueDisplayText(MapppedItemOption, EffectiveValue, false).ToString()));

	DrawDebugText(InStartX, InStartY, DisplayString, FLinearColor::Green, 18); // 폰트컬러랑 크기는 여기서 임의로다가.
}
void UB2LobbyUI_CharStatDetail::DebugDrawPlayerStatOnScreen(EPCClass InMainCharClassOverride, EPCClass InSubCharClass)
{
	if (DebugDrawPlayerStatMode <= 0) {
		extern bool gIsDevUI;
		if (gIsDevUI)
		{ // 안내 문구 혹 괜찮다 싶으면 복구.
		//	DrawDebugText(10.0f, 10.0f, TEXT("Cmd \"PlayerOptionDisplay #SetNum\" to show player stats."), FLinearColor::Green, 14);
		}
		return;
	}

	// 메인 클래스 인자는 여기서 변수로 들고 있는 걸 오버라이드, 서브 클래스는 그냥 지정. 정비가 좀 덜 되어서 그런 건데 세마리가 출동하는 팀 대전 아니면 딱히 바꿀 필욘 없을 듯도.
	const EPCClass FinalDisplayMainClass = (InMainCharClassOverride != EPCClass::EPC_End) ? InMainCharClassOverride : DebugDrawPlayerStatClass;

	const float CommonDrawX = 40.0f; // 일단 고정. 한번에 세트 하나씩만 그리는 이상..
	const float CommonDrawXPrimStat = 30.0f;
	float DrawY = 50.0f; // 이건 매 줄 단위로 증가
	const float DrawLineHeight = 35.0f; // 폰트 크기 감안해서 적당히..

	// 한번에 하나의 세트만 draw 한다.
	if (DebugDrawPlayerStatMode == 1) // 공격 관련 stat
	{
		// 주요 수치는 인덴트도 넣고 줄간격도 늘려서
		DebugDrawPlayerPrimStatOnScreen(FinalDisplayMainClass, InSubCharClass, EItemPrimaryPointType::EIPP_Attack, CommonDrawXPrimStat, DrawY);
		DrawY += DrawLineHeight * 1.2f; 
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Offense_IncDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Offense_IncNormalDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Offense_IncCriticalRate, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Offense_IncBossDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Skill_IncSkillDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_General, CommonDrawX, DrawY);
		/*DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Misc_LastNormalAttackKnockbackRate, CommonDrawX, DrawY);*/
	}
	else if (DebugDrawPlayerStatMode == 2) // 방어 관련 stat
	{
		DebugDrawPlayerPrimStatOnScreen(FinalDisplayMainClass, InSubCharClass, EItemPrimaryPointType::EIPP_Defense, CommonDrawXPrimStat, DrawY);
		DrawY += DrawLineHeight * 1.2f;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_DecDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_DecBossDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_DecRangedDamage, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Defense_ResistCritical, CommonDrawX, DrawY);
	}
	else if (DebugDrawPlayerStatMode == 3) // 체력 관련 stat
	{
		DebugDrawPlayerPrimStatOnScreen(FinalDisplayMainClass, InSubCharClass, EItemPrimaryPointType::EIPP_Health, CommonDrawXPrimStat, DrawY);
		DrawY += DrawLineHeight * 1.2f;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		// 이건 확실히 의도한 항목인지 모르겠다..
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, CommonDrawX, DrawY);
	}
	else if(DebugDrawPlayerStatMode == 4) // 기타.
	{
 		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Misc_AdditionalExp, CommonDrawX, DrawY);
 		DrawY += DrawLineHeight;
 		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Misc_AdditionalGold, CommonDrawX, DrawY);
 		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Misc_IncMoveSpeed, CommonDrawX, DrawY);
		DrawY += DrawLineHeight;
		DebugDrawPlayerSingleOptionOnScreen(FinalDisplayMainClass, EUnitedCombatOptions::UCO_Misc_DecTagCooltime, CommonDrawX, DrawY);
	}
	else
	{
		DrawDebugText(10.0f, 10.0f, FString::Printf(TEXT("Please specify player option category in valid range. Currently %d"), DebugDrawPlayerStatMode), FLinearColor::Green, 18);
	}
}
void UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatMode(int32 InSetNumber)
{
	DebugDrawPlayerStatMode = InSetNumber;
}
void UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatClass(EPCClass InClass)
{
	DebugDrawPlayerStatClass = InClass;
}
#endif