#include "BladeII.h"
#include "B2UICharacterSelect.h"
#include "B2UICharacterSelectItem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "BladeIIPlayer.h"
#include "B2PCClassInfo.h"


#include "BladeIIGameImpl.h"

UB2UICharacterSelect::UB2UICharacterSelect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeroItems.Empty();

	MainHeroDoc = nullptr;
	SubHeroDoc = nullptr;

	NewMainHeroDoc = nullptr;
	NewSubHeroDoc = nullptr;

	PrefixHeroItem = TEXT("UIP_HeroItem");

	CurSelectType = EUIHeroSelectType::None;
}

void UB2UICharacterSelect::CacheAssets()
{
	GET_SLOT(UButton, BTN_StartBattle)
	GET_SLOT(UButton, BTN_Confirm)
	GET_SLOT(UButton, BTN_Close)
	GET_SLOT(UTextBlock, TB_HerosTotalAttack)
	GET_SLOT(UTextBlock, TB_HerosTotalDefense)
	GET_SLOT(UTextBlock, TB_NewHerosTotalAttack)
	GET_SLOT(UTextBlock, TB_NewHerosTotalDefense)
	GET_SLOT(UTextBlock, TB_HerosTotalCombat)
	GET_SLOT(UTextBlock, TB_NewHerosTotalCombat)
	GET_SLOT(UImage, IMG_MainCharacter)
	GET_SLOT(UImage, IMG_SubCharacter)
	GET_SLOT(UTextBlock, TBS_MainCharacterText)
	GET_SLOT(UTextBlock, TBS_SubCharacterText)
	GET_SLOT(UTextBlock, TBS_SubTitle)
	GET_SLOT(UButton, BTN_SubCharacter)
	GET_SLOT(UButton, BTN_MainCharacter)
	GET_SLOT(UWidgetSwitcher, WS_SelectMainTarget)
	GET_SLOT(UWidgetSwitcher, WS_SelectSubTarget)
	GET_SLOT(UTextBlock, TB_Guide)

	GET_SLOT(UOverlay, O_TouchHero1)
	GET_SLOT(UOverlay, O_TouchHero2)

	//히어로 어셋을 가져온다.
	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		FString HeroItemAssetStr = PrefixHeroItem + FString::FormatAsNumber(ClassCnt);
		auto HeroItem = GetSlot<UB2UICharacterSelectItem>(FName(*HeroItemAssetStr));
		if (HeroItem)
		{
			HeroItems.AddUnique(HeroItem);
		}
	}

}

void UB2UICharacterSelect::UpdateStaticText()
{
	if (TBS_SubTitle.IsValid())
	{
		TBS_SubTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ChangeHero")));
	}
}

void UB2UICharacterSelect::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_StartBattle, &UB2UICharacterSelect::OnClickBtnStartBattle)
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UICharacterSelect::OnClickBtnConfirm)
	BIND_CLICK_FUNC_TO_BTN(BTN_SubCharacter, &UB2UICharacterSelect::OnClickBtnSubnHero);
	BIND_CLICK_FUNC_TO_BTN(BTN_MainCharacter, &UB2UICharacterSelect::OnClickBtnMainHero);

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICharacterSelect::OnClickBtnClose)
}

void UB2UICharacterSelect::Init()
{
	Super::Init();

	//더미밸류 초기화
	SetHerosTotalCombat(0);
	NewSetHerosTotalCombat(0);

	//밸류표시 텍스트블록의 원래 컬러 저장
	if (TB_NewHerosTotalAttack.IsValid())
		TextColor_OriginValue = TB_NewHerosTotalAttack->GetColorAndOpacity();
	
	for (auto HeroItem : HeroItems)
	{
		HeroItem->Init();
		HeroItem->OnCharacterSelectItemClickedDelgate.BindDynamic(this, &UB2UICharacterSelect::OnClickBtnHero);
	}
	
}

void UB2UICharacterSelect::BindDocAuto()
{
	UnbindDoc();
	
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	SetDoc(DocBattle);
	if (DocBattle)
	{
		//각 아이템에 데이터를 넣어준다.
		for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
		{
			auto HeroItem = HeroItems.IsValidIndex(ClassCnt) ? HeroItems[ClassCnt] : nullptr;
			if (HeroItem)
			{
				HeroItem->BindDoc(UB2UIDocHelper::GetDocHero(ClassCnt));
			}
		}
		
		EPCClass MainPCClass = EPCClass::EPC_Gladiator;
		EPCClass TagPCClass =  EPCClass::EPC_Assassin;

		if (DocBattle->GetCurPCClass() == GetMaxPCClassNum())
		{
			if (DocBattle->GetTagPCClass() != GetMaxPCClassNum())
				TagPCClass = IntToPCClass(DocBattle->GetTagPCClass());
		}
		else
		{
			MainPCClass = IntToPCClass(DocBattle->GetCurPCClass());

			if (DocBattle->GetTagPCClass() == GetMaxPCClassNum())
				TagPCClass = IntToPCClass(((int32)MainPCClass + 1) % 4);
			else
				TagPCClass = IntToPCClass(DocBattle->GetTagPCClass());

		}

		//현재 메인과 섭의 정보를 넣어준다.
		MainHeroDoc = UB2UIDocHelper::GetDocHero((int32)MainPCClass);
		SubHeroDoc = UB2UIDocHelper::GetDocHero((int32)TagPCClass);

		if (MainPCClass != EPCClass::EPC_End)
			TBS_MainCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(MainPCClass));
		else
			TBS_MainCharacterText->SetText(FText::GetEmpty());

		if (TagPCClass != EPCClass::EPC_End)
			TBS_SubCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(TagPCClass));
		else
			TBS_SubCharacterText->SetText(FText::GetEmpty());

		SetMainSelectCharacter(MainPCClass, EUIHeroSelectType::Main);
		SetMainSelectCharacter(TagPCClass, EUIHeroSelectType::Sub);

		//셀렉트 상태 세팅
		for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
		{
			auto HeroItem = HeroItems.IsValidIndex(ClassCnt) ? HeroItems[ClassCnt] : nullptr;
			if (HeroItem)
			{
				if (ClassCnt == (int32)MainPCClass)
				{
					SetHeroSelected(ClassCnt, EUIHeroSelectType::Main);
				}
				else if (ClassCnt == (int32)TagPCClass)
				{
					SetHeroSelected(ClassCnt, EUIHeroSelectType::Sub);
				}
				else
				{
					SetHeroSelected(ClassCnt, EUIHeroSelectType::None);
				}
			}
		}
		int32 nTotalCombatPower = (MainHeroDoc ? MainHeroDoc->GetCombatPower() : 0) + (SubHeroDoc ? SubHeroDoc->GetCombatPower() : 0);
		
		//공/방
		SetHerosTotalCombat(nTotalCombatPower);
	}
}

void UB2UICharacterSelect::UnbindDoc()
{
	SetDoc(nullptr);
}


void UB2UICharacterSelect::SetHerosTotalCombat(int32 InValue)
{
	if(TB_HerosTotalCombat.IsValid())
		TB_HerosTotalCombat->SetText(FText::AsNumber(InValue));
}

void UB2UICharacterSelect::NewSetHerosTotalCombat(int32 InValue)
{
	if (TB_NewHerosTotalCombat.IsValid())
		TB_NewHerosTotalCombat->SetText(FText::AsNumber(InValue));
}

void UB2UICharacterSelect::SetCurrentSelectType(EUIHeroSelectType InCurSelectType)
{
	CurSelectType = InCurSelectType;

	//아무 셀렉트도 아닌상태이면 셀렉트 상태를 클리어 해주어야 한다.
	if (InCurSelectType == EUIHeroSelectType::None)
	{
		for (int32 HeroCnt = 0; HeroCnt < HeroItems.Num(); ++HeroCnt)
		{
			auto HeroItem = HeroItems[HeroCnt];
			HeroItem->SetSelectMode(EUIHeroSelectModeType::None);
		}
		CurSelectType = EUIHeroSelectType::None;
	}
}

void UB2UICharacterSelect::SetHeroSelected(int32 HeroIndex, EUIHeroSelectType SelectType)
{
	auto HeroItem = HeroItems.IsValidIndex(HeroIndex) ? HeroItems[HeroIndex] : nullptr;
	if (HeroItem)
	{
		HeroItem->SetSelectedType(SelectType);
	}
}

void UB2UICharacterSelect::SetMainSelectCharacter(EPCClass type, EUIHeroSelectType SelectType)
{
	int32 PcIndex = static_cast<int32>(type);

	if (!CharterTexture.IsValidIndex(PcIndex))
	{
		if (SelectType == EUIHeroSelectType::Main)
			IMG_MainCharacter->SetColorAndOpacity(DarkColor);
		else
			IMG_SubCharacter->SetColorAndOpacity(DarkColor);
		return;
	}


	if (SelectType == EUIHeroSelectType::Main)
	{
		IMG_MainCharacter->SetBrushFromMaterial(CharterTexture[PcIndex]);
	}

	if (SelectType == EUIHeroSelectType::Sub)
	{
		IMG_SubCharacter->SetBrushFromMaterial(CharterTexture[PcIndex]);
	}
}


void UB2UICharacterSelect::SetMainSelectType(EUIHeroSelectType SelectType, EUIHeroSelectModeType ModType)
{
	/* UWidgetSwitcher 0.애니없음 1. 메인 2. 서브*/

	int32 SwitcherIndex = 0;

	if (ModType == EUIHeroSelectModeType::None)
	{
		O_TouchHero1->SetVisibility(ESlateVisibility::Hidden);
		O_TouchHero2->SetVisibility(ESlateVisibility::Hidden);
		if (SelectType == EUIHeroSelectType::Main)
		{
			WS_SelectSubTarget->SetActiveWidgetIndex(0);
			WS_SelectMainTarget->SetActiveWidgetIndex(1);
			IMG_MainCharacter->SetColorAndOpacity(WhiteColor);
			IMG_SubCharacter->SetColorAndOpacity(DarkColor);
		}
		else if (SelectType == EUIHeroSelectType::Sub)
		{
			WS_SelectMainTarget->SetActiveWidgetIndex(0);
			WS_SelectSubTarget->SetActiveWidgetIndex(1);
			IMG_MainCharacter->SetColorAndOpacity(DarkColor);
			IMG_SubCharacter->SetColorAndOpacity(WhiteColor);;
		}
	}
	else
	{
		WS_SelectMainTarget->SetActiveWidget(0);
		WS_SelectSubTarget->SetActiveWidget(0);
		IMG_MainCharacter->SetColorAndOpacity(WhiteColor);
		IMG_SubCharacter->SetColorAndOpacity(WhiteColor);
		O_TouchHero1->SetVisibility(ESlateVisibility::Visible);
		O_TouchHero2->SetVisibility(ESlateVisibility::Visible);
	}

}

void UB2UICharacterSelect::UpdateNewHerosData()
{
	for (int32 HeroCnt = 0; HeroCnt < HeroItems.Num(); ++HeroCnt)
	{
		auto HeroItem = HeroItems[HeroCnt];
		if (HeroItem->GetSelectedType() == EUIHeroSelectType::Main)
			NewMainHeroDoc = UB2UIDocHelper::GetDocHero(HeroCnt);
		else if (HeroItem->GetSelectedType() == EUIHeroSelectType::Sub)
			NewSubHeroDoc = UB2UIDocHelper::GetDocHero(HeroCnt);
	}

	if (NewMainHeroDoc && NewSubHeroDoc)
	{
		const EPCClass MainPCClass = NewMainHeroDoc->GetHeroClass();
		const EPCClass TagPCClass = NewSubHeroDoc->GetHeroClass();

		//메인 캐릭터 태그캐릭터 표시
		SetMainSelectCharacter(MainPCClass, EUIHeroSelectType::Main);
		SetMainSelectCharacter(TagPCClass, EUIHeroSelectType::Sub);

		TBS_MainCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(MainPCClass));
		TBS_SubCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(TagPCClass));

		//새로운 수치로 변경해주고
		int32 NewCombatPower = NewMainHeroDoc->GetCombatPower() + NewSubHeroDoc->GetCombatPower();

		NewSetHerosTotalCombat(NewCombatPower);;

		//과거의 수치와의 차이점을 표시
		int32 OldAttackPower = 0;
		int32 OldDefensePower = 0;
		int32 OldCombatPower = 0;
		if (MainHeroDoc && SubHeroDoc)
		{
			OldAttackPower = MainHeroDoc->GetAttackPower() + SubHeroDoc->GetAttackPower();
			OldDefensePower = MainHeroDoc->GetDefensePower() + SubHeroDoc->GetDefensePower();

			OldCombatPower = MainHeroDoc->GetCombatPower() + SubHeroDoc->GetCombatPower();
			
			SetTotalAttackIncreaseAmount(NewCombatPower - OldCombatPower);

			//텍스트컬러처리
			if (TB_NewHerosTotalAttack.IsValid())
			{
				if (NewCombatPower - OldCombatPower > 0)
					TB_NewHerosTotalCombat->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Increase);
				else if (NewCombatPower - OldCombatPower < 0)
					TB_NewHerosTotalCombat->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
				else 
					TB_NewHerosTotalCombat->SetColorAndOpacity(TextColor_OriginValue);
			}
			
				
		}
	}
	
	
}

void UB2UICharacterSelect::OnClickBtnStartBattle()
{
	if (NewMainHeroDoc && NewSubHeroDoc) 
	{
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(NewMainHeroDoc->GetDocId()), IntToPCClass(NewSubHeroDoc->GetDocId()));
	}

	if (StartBattleDelegate.ExecuteIfBound())
		StartBattleDelegate.Unbind();

	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UB2UICharacterSelect::SetGuideText(ESlateVisibility GuideText)
{
	TB_Guide->SetVisibility(GuideText);
}

void UB2UICharacterSelect::SetStartBattleCallback(const FStartBattleCallBack& Handler)
{
	StartBattleDelegate = Handler;
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UICharacterSelect::OnClickBtnConfirm()
{
	OnFinalSelectionConfirmCommon();
}

void UB2UICharacterSelect::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UB2UICharacterSelect::OnFinalSelectionConfirmCommon()
{
	//이벤트로 메인캐릭과 섭캐릭을 통지한다. LobbyGameMode같은곳으로. 거기서 DocBattle은 세팅하면 BattleStageInfo의 Main,Sub도 바뀔것이다.
	if (NewMainHeroDoc && NewSubHeroDoc){
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(NewMainHeroDoc->GetDocId()), IntToPCClass(NewSubHeroDoc->GetDocId()));

		// 선택 클래스가 변경됨에 따라 아린의 조언도 체크
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		DevShowNotReadyYetMessage(FString(TEXT("ChangeCharacter")));
	}

	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UB2UICharacterSelect::OnClickBtnHero(class UB2UICharacterSelectItem* ClickedHero)
{
	//클릭한 아이템의 셀렉트 상태는
	auto ClickedSelectType = ClickedHero->GetSelectedType();

	SetMainSelectType(ClickedSelectType, ClickedHero->GetSelecteMode());

	switch (ClickedHero->GetSelecteMode())
	{
		//셀렉트 모드가 아닐때는 셀렉트 상태로 바꾸어 준다.	
		default:
		case EUIHeroSelectModeType::None:
			SetCurrentSelectType(ClickedSelectType);

			//"메인"이나 "태그"가 아닌 "None"상태의 아이템은 클릭해 봐야 아무일 일어나면 안됨
			if (ClickedSelectType != EUIHeroSelectType::None)
			{
				for (auto HeroItem : HeroItems)
				{
					if (HeroItem == ClickedHero)
					{
						if (ClickedSelectType == EUIHeroSelectType::Main)
							HeroItem->SetSelectMode(EUIHeroSelectModeType::SelectOwnerMain);
						else if(ClickedSelectType == EUIHeroSelectType::Sub)
							HeroItem->SetSelectMode(EUIHeroSelectModeType::SelectOwnerSub);
					}
					else
					{
						if (ClickedSelectType == EUIHeroSelectType::Main)
							HeroItem->SetSelectMode(EUIHeroSelectModeType::SelectTargetMain);
						else if (ClickedSelectType == EUIHeroSelectType::Sub)
							HeroItem->SetSelectMode(EUIHeroSelectModeType::SelectTargetSub);
					}
				}
			}

			break;



		//현재 셀렉트중인 상태를 빠져나온다
		case EUIHeroSelectModeType::SelectOwnerMain:
		case EUIHeroSelectModeType::SelectOwnerSub:
			SetCurrentSelectType(EUIHeroSelectType::None);
			break;



		//셀렉트 상태 업데이트 해주고 빠져나온다
		case EUIHeroSelectModeType::SelectTargetMain:
		case EUIHeroSelectModeType::SelectTargetSub:
			EUIHeroSelectType TargetSelectedType = ClickedHero->GetSelectedType();

			//셀렉트 상태를 스왑해준다.
			for (int32 HeroCnt = 0; HeroCnt < HeroItems.Num(); ++HeroCnt)
			{
				auto HeroItem = HeroItems[HeroCnt];
				if (HeroItem->GetSelecteMode() == EUIHeroSelectModeType::SelectOwnerMain || HeroItem->GetSelecteMode() == EUIHeroSelectModeType::SelectOwnerSub)
					HeroItem->SetSelectedType(TargetSelectedType);
			}
			ClickedHero->SetSelectedType(CurSelectType);

			//현재 셀렉트중인 상태를 빠져나온다
			SetCurrentSelectType(EUIHeroSelectType::None);

			//스왑이 완료되고, 새로운 Main,Sub Hero 정보를 갱신해준다. 물론 Confirm을 눌르기 전까지 확정은 아니지만
			UpdateNewHerosData();

			PlayCharacterChangeSoundBP();

			break;
	}
}

void UB2UICharacterSelect::OnClickBtnMainHero()
{
	
	int32 HeroIndex = 0;

	if (NewMainHeroDoc)
	{
		if (NewMainHeroDoc->GetRelevantPCClassInfo())
			HeroIndex = PCClassToInt(NewMainHeroDoc->GetRelevantPCClassInfo()->PCClassEnum);
	}
	else
	{
		if(MainHeroDoc)
			HeroIndex = PCClassToInt(MainHeroDoc->GetRelevantPCClassInfo()->PCClassEnum);
	}
	if (!HeroItems.IsValidIndex(HeroIndex))
		return;

	if (HeroItems[HeroIndex]->GetSelecteMode() != EUIHeroSelectModeType::None)
		HeroItems[HeroIndex]->SetSelectMode(EUIHeroSelectModeType::SelectOwnerMain);

	OnClickBtnHero(HeroItems[HeroIndex]);

}

void UB2UICharacterSelect::OnClickBtnSubnHero()
{

	int32 HeroIndex = 0;

	if (NewSubHeroDoc)
	{
		if(NewSubHeroDoc->GetRelevantPCClassInfo())
			HeroIndex = PCClassToInt(NewSubHeroDoc->GetRelevantPCClassInfo()->PCClassEnum);
	}
	else
	{
		if (SubHeroDoc && SubHeroDoc->GetRelevantPCClassInfo())
			HeroIndex = PCClassToInt(SubHeroDoc->GetRelevantPCClassInfo()->PCClassEnum);
	}

	if (!HeroItems.IsValidIndex(HeroIndex))
		return;

	if (HeroItems[HeroIndex]->GetSelecteMode() != EUIHeroSelectModeType::None)
		HeroItems[HeroIndex]->SetSelectMode(EUIHeroSelectModeType::SelectOwnerMain);

	OnClickBtnHero(HeroItems[HeroIndex]);
}
