#include "B2UIStageResultLevelUp.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocHero.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"
#include "B2UIPlayerSkillInfo.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2CodeTable.h"
#include "B2PlatformAPI.h"

UB2UIStageResultLevelUp::UB2UIStageResultLevelUp(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsMainCharLevelUp = false;
	bIsSubCharLevelUp = false;
	ThisPhase = EStageLevelUpUIPhase::SLVUUP_Hidden;
	AACounterLastRealTimeSec = 0.0f;
	AACounterThisRealTimeSec = 0.0f;
	AutoAdvanceInterval = 5.0f;
	CachedWidgetStartTime = 0.0f;
	AutoAdvanceCounter = 0.0f;
}

void UB2UIStageResultLevelUp::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_WholeArea);

	GET_SLOT(UPanelWidget, CP_LevelUpTitleSet);

	GET_SLOT(UTextBlock, TB_CharLevel_Prev);
	GET_SLOT(UTextBlock, TB_CharLevel);
	GET_SLOT(UImage, IMG_CharPortrait);
	GET_SLOT(UTextBlock, TB_LevelUp);

	GET_SLOT(UPanelWidget, CP_BasicStatInfoSet);

	GET_SLOT(UTextBlock, TB_ClassName);
	GET_SLOT(UTextBlock, TB_SkillPtAcquireInfo);

	GET_SLOT(UTextBlock, TB_StatTitle_Health);
	GET_SLOT(UTextBlock, TB_StatTitle_Attack);
	GET_SLOT(UTextBlock, TB_StatTitle_Defense);

	GET_SLOT(UTextBlock, TB_StatBefore_Health);
	GET_SLOT(UTextBlock, TB_StatBefore_Attack);
	GET_SLOT(UTextBlock, TB_StatBefore_Defense);

	GET_SLOT(UTextBlock, TB_StatAfter_Health);
	GET_SLOT(UTextBlock, TB_StatAfter_Attack);
	GET_SLOT(UTextBlock, TB_StatAfter_Defense);

	GET_SLOT(UPanelWidget, CP_AllNewSkillSet);
	if (CP_AllNewSkillSet.IsValid())
	{
		CP_AllNewSkillSet->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UImage, IMG_SkillIcon);
	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UTextBlock, TB_SkillAcquireMessage);

	GET_SLOT(UPanelWidget, CP_AllNewBrevetRankSet);
	if (CP_AllNewBrevetRankSet.IsValid())
	{
		CP_AllNewBrevetRankSet->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UImage, IMG_RankIcon);
	GET_SLOT(UImage, IMG_CharPortraitForBrevetRank);
	GET_SLOT(UTextBlock, TB_ClassNameForBrevetRank);
	GET_SLOT(UTextBlock, TB_RankUpNotice);

	GET_SLOT(UPanelWidget, P_Bottom);
	if (P_Bottom.IsValid())
	{
		P_Bottom->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UTextBlock, TB_SkillType);
	GET_SLOT(UImage, IMG_Skill_Badge);
	GET_SLOT(UTextBlock, STB_Confirm);
}

void UB2UIStageResultLevelUp::UpdateStaticText()
{
	if (TB_LevelUp.IsValid())
	{
		TB_LevelUp->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("StageClear_LevelUp"))));
	}
	if (TB_StatTitle_Health.IsValid())
	{
		TB_StatTitle_Health->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health"))));
	}
	if (TB_StatTitle_Attack.IsValid())
	{
		TB_StatTitle_Attack->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))));
	}
	if (TB_StatTitle_Defense.IsValid())
	{
		TB_StatTitle_Defense->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))));
	}
	if (STB_Confirm.IsValid())
	{
		STB_Confirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
	}
}

void UB2UIStageResultLevelUp::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_WholeArea, &UB2UIStageResultLevelUp::OnClickBtnWholeArea)
}

void UB2UIStageResultLevelUp::CloseWidgetDelegate()
{
	OnClickBtnWholeArea();
}

void UB2UIStageResultLevelUp::Init()
{
	Super::Init();
	
	ThisPhase = EStageLevelUpUIPhase::SLVUUP_Hidden; // 처음에는 보이지 않는다. StartMe 이후부터..
	this->SetVisibility(ESlateVisibility::Hidden); // 물론 Hidden 으로 세팅되어 있겠지만..

	ResetAutoAdvanceCounter();
}

void UB2UIStageResultLevelUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 숨어있는 동안에는 timer 카운트를 하지 않도록.
	if (ThisPhase != EStageLevelUpUIPhase::SLVUUP_Hidden)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{ // Pause 상태에 대비하기 위한 매뉴얼 타이머
			AACounterLastRealTimeSec = AACounterThisRealTimeSec;
			AACounterThisRealTimeSec = TheWorld->GetRealTimeSeconds();
		}

		UpdateWidgetCustomTimer(); // 여기로 DeltaTime 을 넘겨주는 건 의미가 없을 듯. 자체 타이머 운용. 그나마 NativeTick 이 불리는 게 다행
	}
}

void UB2UIStageResultLevelUp::OnOpenComplete()
{
	Super::OnOpenComplete();
}

void UB2UIStageResultLevelUp::OnCloseComplete()
{
	Super::OnCloseComplete();
}

void UB2UIStageResultLevelUp::AdvancePhase()
{
	if (ThisPhase == EStageLevelUpUIPhase::SLVUUP_Hidden)
	{
		OnStart();
	}
	uint8 PhaseNum = (uint8)ThisPhase;

	if (SelectedAllPCClass.IsValidIndex(PhaseNum))
	{
		EPCClass CurrentClass = SelectedAllPCClass[PhaseNum];

		ThisPhase = static_cast<EStageLevelUpUIPhase>(PhaseNum+1);

		if (CurrentClass != EPCClass::EPC_End)
		{
			UB2UIDocHero* DocHeroMain = UB2UIDocHelper::GetDocHero((int32)CurrentClass);
			if (DocHeroMain)
			{
				const int32 MainAcquiredSkillID = GetNewAcquiredSkillID(CurrentClass, DocHeroMain);
		
				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
				//const int32 MainLevel = DocHeroMain ? FMath::Max(DocHeroMain->GetExtraUpgradeLevel(), DocHeroMain->GetBasicUpgradeLevel()) : 0;
				//const int32 MainLevelOld = DocHeroMain ? DocHeroMain->GetCurrentLevel() : 0;
				FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
				const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(CurrentClass);
				const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(CurrentClass);
				int32 MainBrevetRank = BladeIIGameImpl::GetClientDataStore().GetBrevetRankByOpenLevel(ClearBeforeLevel, ClearAfterLevel);

				if (SetIfPlayerCharacterGotLevelUp(CurrentClass))
				{
					this->SetVisibility(ESlateVisibility::Visible);

					SetLevelUpContent(CurrentClass, DocHeroMain);
					OnLevelUpSceneStart(CurrentClass);

					// 레벨업 페이즈단에서 스킬,진급랭크 검사 후 출력
					if (MainAcquiredSkillID > 0)
					{
						SetNewSkillContent(CurrentClass, DocHeroMain, MainAcquiredSkillID);
						OnNewSkillSceneStart(CurrentClass);
					}

					if (MainBrevetRank > 1)
					{
						SetNewBrevetRankContnt(CurrentClass, DocHeroMain, MainBrevetRank);
						OnNewBrevetRankSceneStart();
					}
				}
				else
				{
					AdvancePhase();
				}
			}
		}
	}
	else
	{
		CloseMe();
		return;
	}

	AutoAdvanceCounter = 0.0f; // 이건 Advance 때마다 리셋.
}

void UB2UIStageResultLevelUp::SetLevelUpContent(EPCClass InClass, class UB2UIDocHero* InDocHero)
{
	if (InClass == EPCClass::EPC_End || !InDocHero){
		return;
	}

	// 현재로선 이건 항상 보이도록.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 기본 스탯 셋은 보이고 스킬 셋은 숨기고 랭크업 셋도 숨기고
	if (CP_BasicStatInfoSet.IsValid())
	{
		CP_BasicStatInfoSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (CP_AllNewSkillSet.IsValid())
	{
		CP_AllNewSkillSet->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CP_AllNewBrevetRankSet.IsValid())
	{
		CP_AllNewBrevetRankSet->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 각 widget 들을 들어온 Doc 에 따라 업데이트 한다.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
	//const int32 OldLevel = InDocHero->GetCurrentLevel(); // 스테이지 종료한 시점에서는 Current 가 아닌 이전 레벨 값임
	//const int32 NewLevel = FMath::Max(InDocHero->GetExtraUpgradeLevel(), InDocHero->GetBasicUpgradeLevel());
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(InClass);
	const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(InClass);

	ensure(ClearAfterLevel > ClearBeforeLevel);

	if (TB_CharLevel_Prev.IsValid())
	{
		TB_CharLevel_Prev->SetText(FText::AsNumber(ClearBeforeLevel));
	}
	if (TB_CharLevel.IsValid())
	{
		TB_CharLevel->SetText(FText::AsNumber(ClearAfterLevel));
	}

	// tracking
	if (ClearBeforeLevel < TRACKING_LEVEL && ClearAfterLevel >= TRACKING_LEVEL)
	//if (ClearAfterLevel == TRACKING_LEVEL)
	{
		B2P_LevelTarcking(TRACKING_LEVEL, B2ActionLogType<EPCClass>::GetName(InClass));
	}
	if (IMG_CharPortrait.IsValid())
	{ // 요건 텍스쳐로 그냥 들어감. 아틀라스 아닌 별도 텍스쳐이므로 머티리얼 파라미터 같은 거 필요없이 텍스쳐로 그냥 세팅 가능한 거. UI 쪽서도 그게 편하다 그러고.
		UTexture2D* LargePortraitTexture = UB2UIDocHero::GetPlayerLargePortraitTexture(InDocHero->GetHeroClass());
		if (LargePortraitTexture)
		{
			IMG_CharPortrait->SetBrushFromTexture(LargePortraitTexture);
		}
	}
	if (TB_ClassName.IsValid())
	{
		TB_ClassName->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(InDocHero->GetHeroClass()));
	}
	if (TB_SkillPtAcquireInfo.IsValid())
	{
		TB_SkillPtAcquireInfo->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LevelUpAcquireSkillPointMessage"))), FText::FromString(FString::FromInt(InDocHero->GetLastAcquiredSkillPoint())))
			);
	}

	// 혹 숫자 커지면 FormatAsNumber 사용..
	if (TB_StatBefore_Health.IsValid())
	{
		TB_StatBefore_Health->SetText(FText::FromString(FString::FromInt(InDocHero->GetHealthPoint())));
	}
	if (TB_StatBefore_Attack.IsValid())
	{
		TB_StatBefore_Attack->SetText(FText::FromString(FString::FromInt(InDocHero->GetAttackPower())));
	}
	if (TB_StatBefore_Defense.IsValid())
	{
		TB_StatBefore_Defense->SetText(FText::FromString(FString::FromInt(InDocHero->GetDefensePower())));
	}

	if (TB_StatAfter_Health.IsValid())
	{
		TB_StatAfter_Health->SetText(FText::FromString(FString::FromInt(InDocHero->GetPostClearHealthPoint())));
	}
	if (TB_StatAfter_Attack.IsValid())
	{
		TB_StatAfter_Attack->SetText(FText::FromString(FString::FromInt(InDocHero->GetPostClearAttackPower())));
	}
	if (TB_StatAfter_Defense.IsValid())
	{
		TB_StatAfter_Defense->SetText(FText::FromString(FString::FromInt(InDocHero->GetPostClearDefensePower())));
	}
}

void UB2UIStageResultLevelUp::SetNewSkillContent(EPCClass InClass, class UB2UIDocHero* InDocHero, int32 NewSkillID)
{
	if (InClass == EPCClass::EPC_End || !InDocHero || NewSkillID <= 0){
		return;
	}

	// 현재로선 이건 항상 보이도록.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 기본 스탯 셋은 숨기고 스킬 셋은 보이고 랭크업 셋은 숨기고
	/*if (CP_BasicStatInfoSet.IsValid())
	{
		CP_BasicStatInfoSet->SetVisibility(ESlateVisibility::Collapsed);
	}*/

	if (CP_AllNewSkillSet.IsValid())
	{
		CP_AllNewSkillSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	/*if (CP_AllNewBrevetRankSet.IsValid())
	{
		CP_AllNewBrevetRankSet->SetVisibility(ESlateVisibility::Collapsed);
	}*/

	if (P_Bottom.IsValid())
	{
		P_Bottom->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
	UB2SkillInfo* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;
	// LoadSynchronous 를 해야 해서 const 제거..
	FSingleSkillInfo* ThisSkillInfo = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(NewSkillID)) : nullptr;
	
	if (IMG_SkillIcon.IsValid() && ThisSkillInfo)
	{ // 스킬 버튼 스타일에서 대표격인 Normal 스타일 material 만 쏙 빼와서 사용. 
		IMG_SkillIcon->SetBrushFromMaterial(
			Cast<UMaterialInterface>(ThisSkillInfo->ButtonIcon_Normal.LoadSynchronous())
			);		
	}
	/*if (TB_ClassNameForSkill.IsValid())
	{
		TB_ClassNameForSkill->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(InDocHero->GetHeroClass()));
	}*/
	if (TB_SkillName.IsValid())
	{
		TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, NewSkillID)));	
	}
	if (TB_SkillAcquireMessage.IsValid())
	{
		TB_SkillAcquireMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageLevelUpMessage")));
	}

	SetSkillType(ThisSkillInfo->SkillType);
}

void UB2UIStageResultLevelUp::SetSkillType(ESkillType InSkillType)
{
	if (TB_SkillType.IsValid())
	{
		switch (InSkillType)
		{
		case ESkillType::ESkillType_Attack:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Attack")));
			break;
		case ESkillType::ESkillType_Defense:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Defense")));
			break;
		case ESkillType::ESkillType_Survive:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Survive")));
			break;
		case ESkillType::ESkillType_Buff:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Buff")));
			break;
		case ESkillType::ESkillType_Counter:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Counter")));
			break;
		case ESkillType::ESkillType_Tag:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Tag")));
			break;
		}

		TB_SkillType->SetColorAndOpacity(FSkillBadgeImage::GetSkillBadgeFontColor(SkillBadgeImages, InSkillType));
	}

	if (IMG_Skill_Badge.IsValid())
	{
		auto* SkillBadgeImage = FSkillBadgeImage::GetSkillBadgeImage(SkillBadgeImages, InSkillType);
		if (SkillBadgeImage)
			IMG_Skill_Badge->SetBrushFromMaterial(SkillBadgeImage);
	}
}

void UB2UIStageResultLevelUp::SetNewBrevetRankContnt(EPCClass InClass, class UB2UIDocHero* InDocHero, int32 BrevetRank)
{
	// 진급 관련UI만 보이게하구 이것저것 세팅
	/*if (CP_BasicStatInfoSet.IsValid())
	{
		CP_BasicStatInfoSet->SetVisibility(ESlateVisibility::Collapsed);
	}*/
	/*if (CP_AllNewSkillSet.IsValid())
	{
		CP_AllNewSkillSet->SetVisibility(ESlateVisibility::Collapsed);
	}*/
	if (CP_AllNewBrevetRankSet.IsValid())
	{
		CP_AllNewBrevetRankSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (P_Bottom.IsValid())
	{
		P_Bottom->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 랭크 아이콘 셋팅
	if (IMG_RankIcon.IsValid())
	{
		UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

		if (pMI)
			IMG_RankIcon->SetBrushFromMaterial(pMI);
	}

	// 케릭터 초상화 셋팅
	if (IMG_CharPortraitForBrevetRank.IsValid() && InDocHero)
	{
		// 이건 Large 아니고 그냥 초상화..?
		UMaterialInterface* CastedBrushMtrl = UB2UIDocHero::GetPlayerPortraitMaterial(this, InDocHero->GetHeroClass());
		if (CastedBrushMtrl)
		{
			IMG_CharPortraitForBrevetRank->SetBrushFromMaterial(CastedBrushMtrl);
		}
	}

	// 케릭터 이름 셋팅
	if (TB_ClassNameForBrevetRank.IsValid())
	{
		int32 CurrentCharBrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(InDocHero->GetHeroClass());
		FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
		TB_ClassNameForBrevetRank->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey));
	}

	// 설명 셋팅
	if (TB_RankUpNotice.IsValid())
	{
		TB_RankUpNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageRankUpNotice")));
	}
}

bool UB2UIStageResultLevelUp::SetIfPlayerCharacterGotLevelUp(EPCClass CurrentClass)
{
	UB2UIDocHero* DocHeroMain = UB2UIDocHelper::GetDocHero((int32)CurrentClass);

	if (DocHeroMain)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//int32 LevelDelta = FMath::Max(DocHeroMain->GetExtraUpgradeLevel(), DocHeroMain->GetBasicUpgradeLevel()) - DocHeroMain->GetCurrentLevel();
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(CurrentClass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(CurrentClass);

		int32 LevelDelta = ClearAfterLevel - ClearBeforeLevel;
		return (LevelDelta > 0);
	}
	else {
		return false;
	}
}

int32 UB2UIStageResultLevelUp::GetNewAcquiredSkillID(EPCClass InClass, class UB2UIDocHero* InDocHero)
{
	TArray<int32> SkillIDsToLearn;

	if (InClass != EPCClass::EPC_End && InDocHero)
	{	
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(InClass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(InClass);

		for (int32 CheckLevel = ClearBeforeLevel + 1; CheckLevel <= ClearAfterLevel; ++CheckLevel)
		{
			BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillToLearnAtLevel(SkillIDsToLearn, InClass, CheckLevel);			
		}		
	}

	if (SkillIDsToLearn.Num() > 0)
	{
		//레벨변화량에 따라 얻는 스킬의 개수는 1이상이다.
		//일단은 제일 마지막에 얻는 스킬로.
		return SkillIDsToLearn.Last();
	}

	return -1; // 얻은 스킬 없음.
}

void UB2UIStageResultLevelUp::StartMe(TArray<EPCClass> AllPCClass)
{
	if (ThisPhase == EStageLevelUpUIPhase::SLVUUP_Hidden) // Hidden 이 아니라면 이미 시작된 것.
	{
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
		SelectedAllPCClass.Empty();
		SelectedAllPCClass = AllPCClass;

		ResetAutoAdvanceCounter();
		AdvancePhase();
	}
}

void UB2UIStageResultLevelUp::SetAutoAdvanceInterval(float InValue)
{
	AutoAdvanceInterval = InValue;
}

void UB2UIStageResultLevelUp::OnStart()
{
	// 보여지기 시작하는 부분이므로 커스텀 타이머 카운트도 여기서 시작할 수 있도록
	if (GetWorld())
	{// 이게 World pause 상태에서 사용하게 될 수 있으므로 GetRealTimeSeconds 를 한다.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}
	//this->SetVisibility(ESlateVisibility::Visible);

	OnOpen(); // 일반 UIWidget 이라면 UIManager 에 의해서 불리는 부분
}

void UB2UIStageResultLevelUp::CloseMe()
{
	// 여기서 아마도 Destroy 를 하지는 않을 것이다. 이게 속해 있는 스테이지 결과창 바탕이 보이도록 해서 거기 메뉴를 통해 다음 액션을 취하도록 한다.

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	for (auto PCClass : SelectedAllPCClass)
	{
		auto* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(PCClass));
		if (HeroDoc)
		{
			//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
			//HeroDoc->InitExp();
			CharacterDataStore.UpdateBeforeExpLevel(PCClass);
			const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
			HeroDoc->SetPowers(ClearAfterLevel);
			HeroDoc->SetPostClearPowers(ClearAfterLevel);
		}
	}

	this->SetVisibility(ESlateVisibility::Hidden);
	
	LevelupFinishDelegate.ExecuteIfBound();

	OnClose(); // 일반 UIWidget 이라면 UIManager 에 의해서 불리는 부분
	
	LevelupFinishDelegate.Unbind();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	OnResetStartingLevelInfoClass<>::GetInstance().Signal();
}

void UB2UIStageResultLevelUp::SetupWidgetCustomTimer()
{
	// Pause 상태에서 쓸 수 있으므로 기본 타이머 못 쓰고 따로 세팅

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 이미 발동한 걸로 간주.
		}
	}
}

void UB2UIStageResultLevelUp::SetupWidgetCustomHideTimer()
{
	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide == NULL)
		{
			continue;
		}

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 할 때에도 백업해 놓지만 아래 로직에 따라 HideStop 이 먼저 될 수도 있으니 여기서도 한번.

		// HideStopTime 이 HideStartTime 보다 큰지에 대한 확인은.. 굳이 하지 않는다.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 바로 콜백 실행
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 바로 콜백 실행. 결국 HideStopTime 이 0 이하면 안 숨겨짐
		}
	}
}

void UB2UIStageResultLevelUp::UpdateWidgetCustomTimer() // Pause 상태를 가정하면 DeltaTime 이 의미가 없으므로 받지 않음.
{
	UWorld* TheWorld = GetWorld();
	
	const float TimeSinceStart = (TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f) - CachedWidgetStartTime;
	// Pause 상태에서도 직접 돌려쓰는 타이머
	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedCallbackInfo& ThisInfo = CustomTimedCallBackInfo[CTI];
		if (ThisInfo.bIsInvoked == true)
		{
			continue;
		}

		if (ThisInfo.InvokeTime <= TimeSinceStart)
		{
			OriginalCustomTimeCallback(CTI);
			ThisInfo.bIsInvoked = true;
		}
	}

	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];
		
		if (ThisHideInfo.bIsHidden == false)
		{
			// 한번만 HideStart 가 되게. 안 그러면 깜박일 것.
			if (ThisHideInfo.bHiddenDoneOnce == false && ThisHideInfo.HideStartTime <= TimeSinceStart)
			{
				CustomTimeHideStartCallback(CTI);
			}
		}
		else
		{
			if (ThisHideInfo.HideStopTime <= TimeSinceStart)
			{
				CustomTimeHideStopCallback(CTI);
			}
		}
	}

	AutoAdvanceCounter += (AACounterThisRealTimeSec - AACounterLastRealTimeSec);
	if (AutoAdvanceCounter >= AutoAdvanceInterval)
	{ // 유저 터치 없을 때의 자동 전환.
		AdvancePhase();
	}
}

void UB2UIStageResultLevelUp::ResetAutoAdvanceCounter()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		AACounterLastRealTimeSec = TheWorld->GetRealTimeSeconds();
		AACounterThisRealTimeSec = TheWorld->GetRealTimeSeconds();
	}
	else
	{
		AACounterLastRealTimeSec = 0.0f;
		AACounterThisRealTimeSec = 0.0f;
	}
	AutoAdvanceCounter = 0.0f;
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIStageResultLevelUp::OnClickBtnWholeArea()
{
	AdvancePhase();
}
