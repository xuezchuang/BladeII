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

	//����� ����� �����´�.
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

	//���̹�� �ʱ�ȭ
	SetHerosTotalCombat(0);
	NewSetHerosTotalCombat(0);

	//���ǥ�� �ؽ�Ʈ����� ���� �÷� ����
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
		//�� �����ۿ� �����͸� �־��ش�.
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

		//���� ���ΰ� ���� ������ �־��ش�.
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

		//����Ʈ ���� ����
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
		
		//��/��
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

	//�ƹ� ����Ʈ�� �ƴѻ����̸� ����Ʈ ���¸� Ŭ���� ���־�� �Ѵ�.
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
	/* UWidgetSwitcher 0.�ִϾ��� 1. ���� 2. ����*/

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

		//���� ĳ���� �±�ĳ���� ǥ��
		SetMainSelectCharacter(MainPCClass, EUIHeroSelectType::Main);
		SetMainSelectCharacter(TagPCClass, EUIHeroSelectType::Sub);

		TBS_MainCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(MainPCClass));
		TBS_SubCharacterText->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(TagPCClass));

		//���ο� ��ġ�� �������ְ�
		int32 NewCombatPower = NewMainHeroDoc->GetCombatPower() + NewSubHeroDoc->GetCombatPower();

		NewSetHerosTotalCombat(NewCombatPower);;

		//������ ��ġ���� �������� ǥ��
		int32 OldAttackPower = 0;
		int32 OldDefensePower = 0;
		int32 OldCombatPower = 0;
		if (MainHeroDoc && SubHeroDoc)
		{
			OldAttackPower = MainHeroDoc->GetAttackPower() + SubHeroDoc->GetAttackPower();
			OldDefensePower = MainHeroDoc->GetDefensePower() + SubHeroDoc->GetDefensePower();

			OldCombatPower = MainHeroDoc->GetCombatPower() + SubHeroDoc->GetCombatPower();
			
			SetTotalAttackIncreaseAmount(NewCombatPower - OldCombatPower);

			//�ؽ�Ʈ�÷�ó��
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
	//�̺�Ʈ�� ����ĳ���� ��ĳ���� �����Ѵ�. LobbyGameMode����������. �ű⼭ DocBattle�� �����ϸ� BattleStageInfo�� Main,Sub�� �ٲ���̴�.
	if (NewMainHeroDoc && NewSubHeroDoc){
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(NewMainHeroDoc->GetDocId()), IntToPCClass(NewSubHeroDoc->GetDocId()));

		// ���� Ŭ������ ����ʿ� ���� �Ƹ��� ���� üũ
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		DevShowNotReadyYetMessage(FString(TEXT("ChangeCharacter")));
	}

	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UB2UICharacterSelect::OnClickBtnHero(class UB2UICharacterSelectItem* ClickedHero)
{
	//Ŭ���� �������� ����Ʈ ���´�
	auto ClickedSelectType = ClickedHero->GetSelectedType();

	SetMainSelectType(ClickedSelectType, ClickedHero->GetSelecteMode());

	switch (ClickedHero->GetSelecteMode())
	{
		//����Ʈ ��尡 �ƴҶ��� ����Ʈ ���·� �ٲپ� �ش�.	
		default:
		case EUIHeroSelectModeType::None:
			SetCurrentSelectType(ClickedSelectType);

			//"����"�̳� "�±�"�� �ƴ� "None"������ �������� Ŭ���� ���� �ƹ��� �Ͼ�� �ȵ�
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



		//���� ����Ʈ���� ���¸� �������´�
		case EUIHeroSelectModeType::SelectOwnerMain:
		case EUIHeroSelectModeType::SelectOwnerSub:
			SetCurrentSelectType(EUIHeroSelectType::None);
			break;



		//����Ʈ ���� ������Ʈ ���ְ� �������´�
		case EUIHeroSelectModeType::SelectTargetMain:
		case EUIHeroSelectModeType::SelectTargetSub:
			EUIHeroSelectType TargetSelectedType = ClickedHero->GetSelectedType();

			//����Ʈ ���¸� �������ش�.
			for (int32 HeroCnt = 0; HeroCnt < HeroItems.Num(); ++HeroCnt)
			{
				auto HeroItem = HeroItems[HeroCnt];
				if (HeroItem->GetSelecteMode() == EUIHeroSelectModeType::SelectOwnerMain || HeroItem->GetSelecteMode() == EUIHeroSelectModeType::SelectOwnerSub)
					HeroItem->SetSelectedType(TargetSelectedType);
			}
			ClickedHero->SetSelectedType(CurSelectType);

			//���� ����Ʈ���� ���¸� �������´�
			SetCurrentSelectType(EUIHeroSelectType::None);

			//������ �Ϸ�ǰ�, ���ο� Main,Sub Hero ������ �������ش�. ���� Confirm�� ������ ������ Ȯ���� �ƴ�����
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
