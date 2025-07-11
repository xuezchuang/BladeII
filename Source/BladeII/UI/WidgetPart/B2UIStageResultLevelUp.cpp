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
	
	ThisPhase = EStageLevelUpUIPhase::SLVUUP_Hidden; // ó������ ������ �ʴ´�. StartMe ���ĺ���..
	this->SetVisibility(ESlateVisibility::Hidden); // ���� Hidden ���� ���õǾ� �ְ�����..

	ResetAutoAdvanceCounter();
}

void UB2UIStageResultLevelUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// �����ִ� ���ȿ��� timer ī��Ʈ�� ���� �ʵ���.
	if (ThisPhase != EStageLevelUpUIPhase::SLVUUP_Hidden)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{ // Pause ���¿� ����ϱ� ���� �Ŵ��� Ÿ�̸�
			AACounterLastRealTimeSec = AACounterThisRealTimeSec;
			AACounterThisRealTimeSec = TheWorld->GetRealTimeSeconds();
		}

		UpdateWidgetCustomTimer(); // ����� DeltaTime �� �Ѱ��ִ� �� �ǹ̰� ���� ��. ��ü Ÿ�̸� ���. �׳��� NativeTick �� �Ҹ��� �� ����
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
		
				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
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

					// ������ ������ܿ��� ��ų,���޷�ũ �˻� �� ���
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

	AutoAdvanceCounter = 0.0f; // �̰� Advance ������ ����.
}

void UB2UIStageResultLevelUp::SetLevelUpContent(EPCClass InClass, class UB2UIDocHero* InDocHero)
{
	if (InClass == EPCClass::EPC_End || !InDocHero){
		return;
	}

	// ����μ� �̰� �׻� ���̵���.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// �⺻ ���� ���� ���̰� ��ų ���� ����� ��ũ�� �µ� �����
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

	// �� widget ���� ���� Doc �� ���� ������Ʈ �Ѵ�.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
	//const int32 OldLevel = InDocHero->GetCurrentLevel(); // �������� ������ ���������� Current �� �ƴ� ���� ���� ����
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
	{ // ��� �ؽ��ķ� �׳� ��. ��Ʋ�� �ƴ� ���� �ؽ����̹Ƿ� ��Ƽ���� �Ķ���� ���� �� �ʿ���� �ؽ��ķ� �׳� ���� ������ ��. UI �ʼ��� �װ� ���ϴ� �׷���.
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

	// Ȥ ���� Ŀ���� FormatAsNumber ���..
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

	// ����μ� �̰� �׻� ���̵���.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// �⺻ ���� ���� ����� ��ų ���� ���̰� ��ũ�� ���� �����
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
	// LoadSynchronous �� �ؾ� �ؼ� const ����..
	FSingleSkillInfo* ThisSkillInfo = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(NewSkillID)) : nullptr;
	
	if (IMG_SkillIcon.IsValid() && ThisSkillInfo)
	{ // ��ų ��ư ��Ÿ�Ͽ��� ��ǥ���� Normal ��Ÿ�� material �� �� ���ͼ� ���. 
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
	// ���� ����UI�� ���̰��ϱ� �̰����� ����
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

	// ��ũ ������ ����
	if (IMG_RankIcon.IsValid())
	{
		UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

		if (pMI)
			IMG_RankIcon->SetBrushFromMaterial(pMI);
	}

	// �ɸ��� �ʻ�ȭ ����
	if (IMG_CharPortraitForBrevetRank.IsValid() && InDocHero)
	{
		// �̰� Large �ƴϰ� �׳� �ʻ�ȭ..?
		UMaterialInterface* CastedBrushMtrl = UB2UIDocHero::GetPlayerPortraitMaterial(this, InDocHero->GetHeroClass());
		if (CastedBrushMtrl)
		{
			IMG_CharPortraitForBrevetRank->SetBrushFromMaterial(CastedBrushMtrl);
		}
	}

	// �ɸ��� �̸� ����
	if (TB_ClassNameForBrevetRank.IsValid())
	{
		int32 CurrentCharBrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(InDocHero->GetHeroClass());
		FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
		TB_ClassNameForBrevetRank->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey));
	}

	// ���� ����
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
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
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
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
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
		//������ȭ���� ���� ��� ��ų�� ������ 1�̻��̴�.
		//�ϴ��� ���� �������� ��� ��ų��.
		return SkillIDsToLearn.Last();
	}

	return -1; // ���� ��ų ����.
}

void UB2UIStageResultLevelUp::StartMe(TArray<EPCClass> AllPCClass)
{
	if (ThisPhase == EStageLevelUpUIPhase::SLVUUP_Hidden) // Hidden �� �ƴ϶�� �̹� ���۵� ��.
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
	// �������� �����ϴ� �κ��̹Ƿ� Ŀ���� Ÿ�̸� ī��Ʈ�� ���⼭ ������ �� �ֵ���
	if (GetWorld())
	{// �̰� World pause ���¿��� ����ϰ� �� �� �����Ƿ� GetRealTimeSeconds �� �Ѵ�.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}
	//this->SetVisibility(ESlateVisibility::Visible);

	OnOpen(); // �Ϲ� UIWidget �̶�� UIManager �� ���ؼ� �Ҹ��� �κ�
}

void UB2UIStageResultLevelUp::CloseMe()
{
	// ���⼭ �Ƹ��� Destroy �� ������ ���� ���̴�. �̰� ���� �ִ� �������� ���â ������ ���̵��� �ؼ� �ű� �޴��� ���� ���� �׼��� ���ϵ��� �Ѵ�.

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	for (auto PCClass : SelectedAllPCClass)
	{
		auto* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(PCClass));
		if (HeroDoc)
		{
			//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
			//HeroDoc->InitExp();
			CharacterDataStore.UpdateBeforeExpLevel(PCClass);
			const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
			HeroDoc->SetPowers(ClearAfterLevel);
			HeroDoc->SetPostClearPowers(ClearAfterLevel);
		}
	}

	this->SetVisibility(ESlateVisibility::Hidden);
	
	LevelupFinishDelegate.ExecuteIfBound();

	OnClose(); // �Ϲ� UIWidget �̶�� UIManager �� ���ؼ� �Ҹ��� �κ�
	
	LevelupFinishDelegate.Unbind();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	OnResetStartingLevelInfoClass<>::GetInstance().Signal();
}

void UB2UIStageResultLevelUp::SetupWidgetCustomTimer()
{
	// Pause ���¿��� �� �� �����Ƿ� �⺻ Ÿ�̸� �� ���� ���� ����

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // �̹� �ߵ��� �ɷ� ����.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide �� ������ ����� ������ �Ʒ� ������ ���� HideStop �� ���� �� ���� ������ ���⼭�� �ѹ�.

		// HideStopTime �� HideStartTime ���� ū���� ���� Ȯ����.. ���� ���� �ʴ´�.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // �ٷ� �ݹ� ����
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // �ٷ� �ݹ� ����. �ᱹ HideStopTime �� 0 ���ϸ� �� ������
		}
	}
}

void UB2UIStageResultLevelUp::UpdateWidgetCustomTimer() // Pause ���¸� �����ϸ� DeltaTime �� �ǹ̰� �����Ƿ� ���� ����.
{
	UWorld* TheWorld = GetWorld();
	
	const float TimeSinceStart = (TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f) - CachedWidgetStartTime;
	// Pause ���¿����� ���� �������� Ÿ�̸�
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
			// �ѹ��� HideStart �� �ǰ�. �� �׷��� ������ ��.
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
	{ // ���� ��ġ ���� ���� �ڵ� ��ȯ.
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
