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
	
	ThisPhase = EStageLevelUpUIPhase::SLVUUP_Hidden; // 贸澜俊绰 焊捞瘤 臼绰促. StartMe 捞饶何磐..
	this->SetVisibility(ESlateVisibility::Hidden); // 拱沸 Hidden 栏肺 技泼登绢 乐摆瘤父..

	ResetAutoAdvanceCounter();
}

void UB2UIStageResultLevelUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 见绢乐绰 悼救俊绰 timer 墨款飘甫 窍瘤 臼档废.
	if (ThisPhase != EStageLevelUpUIPhase::SLVUUP_Hidden)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{ // Pause 惑怕俊 措厚窍扁 困茄 概春倔 鸥捞赣
			AACounterLastRealTimeSec = AACounterThisRealTimeSec;
			AACounterThisRealTimeSec = TheWorld->GetRealTimeSeconds();
		}

		UpdateWidgetCustomTimer(); // 咯扁肺 DeltaTime 阑 逞败林绰 扒 狼固啊 绝阑 淀. 磊眉 鸥捞赣 款侩. 弊唱付 NativeTick 捞 阂府绰 霸 促青
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
		
				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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

					// 饭骇诀 其捞令窜俊辑 胶懦,柳鞭珐农 八荤 饶 免仿
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

	AutoAdvanceCounter = 0.0f; // 捞扒 Advance 锭付促 府悸.
}

void UB2UIStageResultLevelUp::SetLevelUpContent(EPCClass InClass, class UB2UIDocHero* InDocHero)
{
	if (InClass == EPCClass::EPC_End || !InDocHero){
		return;
	}

	// 泅犁肺急 捞扒 亲惑 焊捞档废.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 扁夯 胶湃 悸篮 焊捞绊 胶懦 悸篮 见扁绊 珐农诀 悸档 见扁绊
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

	// 阿 widget 甸阑 甸绢柯 Doc 俊 蝶扼 诀单捞飘 茄促.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	//const int32 OldLevel = InDocHero->GetCurrentLevel(); // 胶抛捞瘤 辆丰茄 矫痢俊辑绰 Current 啊 酒囱 捞傈 饭骇 蔼烙
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
	{ // 夸扒 咆胶媚肺 弊成 甸绢皑. 酒撇扼胶 酒囱 喊档 咆胶媚捞骨肺 赣萍府倔 颇扼固磐 鞍篮 芭 鞘夸绝捞 咆胶媚肺 弊成 技泼 啊瓷茄 芭. UI 率辑档 弊霸 祈窍促 弊矾绊.
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

	// 趣 箭磊 目瘤搁 FormatAsNumber 荤侩..
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

	// 泅犁肺急 捞扒 亲惑 焊捞档废.
	if (CP_LevelUpTitleSet.IsValid())
	{
		CP_LevelUpTitleSet->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 扁夯 胶湃 悸篮 见扁绊 胶懦 悸篮 焊捞绊 珐农诀 悸篮 见扁绊
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
	// LoadSynchronous 甫 秦具 秦辑 const 力芭..
	FSingleSkillInfo* ThisSkillInfo = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(NewSkillID)) : nullptr;
	
	if (IMG_SkillIcon.IsValid() && ThisSkillInfo)
	{ // 胶懦 滚瓢 胶鸥老俊辑 措钎拜牢 Normal 胶鸥老 material 父 斤 哗客辑 荤侩. 
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
	// 柳鞭 包访UI父 焊捞霸窍备 捞巴历巴 技泼
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

	// 珐农 酒捞能 悸泼
	if (IMG_RankIcon.IsValid())
	{
		UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

		if (pMI)
			IMG_RankIcon->SetBrushFromMaterial(pMI);
	}

	// 纳腐磐 檬惑拳 悸泼
	if (IMG_CharPortraitForBrevetRank.IsValid() && InDocHero)
	{
		// 捞扒 Large 酒聪绊 弊成 檬惑拳..?
		UMaterialInterface* CastedBrushMtrl = UB2UIDocHero::GetPlayerPortraitMaterial(this, InDocHero->GetHeroClass());
		if (CastedBrushMtrl)
		{
			IMG_CharPortraitForBrevetRank->SetBrushFromMaterial(CastedBrushMtrl);
		}
	}

	// 纳腐磐 捞抚 悸泼
	if (TB_ClassNameForBrevetRank.IsValid())
	{
		int32 CurrentCharBrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(InDocHero->GetHeroClass());
		FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
		TB_ClassNameForBrevetRank->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey));
	}

	// 汲疙 悸泼
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
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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
		//饭骇函拳樊俊 蝶扼 掘绰 胶懦狼 俺荐绰 1捞惑捞促.
		//老窜篮 力老 付瘤阜俊 掘绰 胶懦肺.
		return SkillIDsToLearn.Last();
	}

	return -1; // 掘篮 胶懦 绝澜.
}

void UB2UIStageResultLevelUp::StartMe(TArray<EPCClass> AllPCClass)
{
	if (ThisPhase == EStageLevelUpUIPhase::SLVUUP_Hidden) // Hidden 捞 酒聪扼搁 捞固 矫累等 巴.
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
	// 焊咯瘤扁 矫累窍绰 何盒捞骨肺 目胶乓 鸥捞赣 墨款飘档 咯扁辑 矫累且 荐 乐档废
	if (GetWorld())
	{// 捞霸 World pause 惑怕俊辑 荤侩窍霸 瞪 荐 乐栏骨肺 GetRealTimeSeconds 甫 茄促.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}
	//this->SetVisibility(ESlateVisibility::Visible);

	OnOpen(); // 老馆 UIWidget 捞扼搁 UIManager 俊 狼秦辑 阂府绰 何盒
}

void UB2UIStageResultLevelUp::CloseMe()
{
	// 咯扁辑 酒付档 Destroy 甫 窍瘤绰 臼阑 巴捞促. 捞霸 加秦 乐绰 胶抛捞瘤 搬苞芒 官帕捞 焊捞档废 秦辑 芭扁 皋春甫 烹秦 促澜 咀记阑 秒窍档废 茄促.

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	for (auto PCClass : SelectedAllPCClass)
	{
		auto* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(PCClass));
		if (HeroDoc)
		{
			//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
			//HeroDoc->InitExp();
			CharacterDataStore.UpdateBeforeExpLevel(PCClass);
			const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
			HeroDoc->SetPowers(ClearAfterLevel);
			HeroDoc->SetPostClearPowers(ClearAfterLevel);
		}
	}

	this->SetVisibility(ESlateVisibility::Hidden);
	
	LevelupFinishDelegate.ExecuteIfBound();

	OnClose(); // 老馆 UIWidget 捞扼搁 UIManager 俊 狼秦辑 阂府绰 何盒
	
	LevelupFinishDelegate.Unbind();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	OnResetStartingLevelInfoClass<>::GetInstance().Signal();
}

void UB2UIStageResultLevelUp::SetupWidgetCustomTimer()
{
	// Pause 惑怕俊辑 镜 荐 乐栏骨肺 扁夯 鸥捞赣 给 静绊 蝶肺 技泼

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 捞固 惯悼茄 吧肺 埃林.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 且 锭俊档 归诀秦 初瘤父 酒贰 肺流俊 蝶扼 HideStop 捞 刚历 瞪 荐档 乐栏聪 咯扁辑档 茄锅.

		// HideStopTime 捞 HideStartTime 焊促 奴瘤俊 措茄 犬牢篮.. 被捞 窍瘤 臼绰促.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 官肺 妮归 角青
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 官肺 妮归 角青. 搬惫 HideStopTime 捞 0 捞窍搁 救 见败咙
		}
	}
}

void UB2UIStageResultLevelUp::UpdateWidgetCustomTimer() // Pause 惑怕甫 啊沥窍搁 DeltaTime 捞 狼固啊 绝栏骨肺 罐瘤 臼澜.
{
	UWorld* TheWorld = GetWorld();
	
	const float TimeSinceStart = (TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f) - CachedWidgetStartTime;
	// Pause 惑怕俊辑档 流立 倒妨静绰 鸥捞赣
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
			// 茄锅父 HideStart 啊 登霸. 救 弊矾搁 濒冠老 巴.
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
	{ // 蜡历 磐摹 绝阑 锭狼 磊悼 傈券.
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
