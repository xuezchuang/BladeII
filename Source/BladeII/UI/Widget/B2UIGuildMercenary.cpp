// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildMercenary.h"
#include "B2GuildDataStore.h"
#include "B2GuildNPCInfo.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2UIMsgPopupTooltipSkillOption.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIStoreProductCost.h"

void UB2UIGuildMercenarySlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_NPCSelect)

	GET_SLOT(UPanelWidget, O_Select)
	GET_SLOT(UImage, IMG_SelectNPCImage)

	GET_SLOT(UPanelWidget, O_UnSelect)
	GET_SLOT(UImage, IMG_UnSelectNPCImage)

	GET_SLOT(UPanelWidget, O_Lock)
	GET_SLOT(UImage, IMG_LockNPCImage)
}

void UB2UIGuildMercenarySlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_NPCSelect, &UB2UIGuildMercenarySlot::OnClickNPCButton);
}

void UB2UIGuildMercenarySlot::Init()
{
	Super::Init();

	SlotDelegate.Unbind();
}

void UB2UIGuildMercenarySlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIGuildMercenarySlot::SetSlot(b2network::B2mdGuildMercenaryPtr InNPCInfo)
{

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();

	NPCID = InNPCInfo->id;
	const FGuildNPCInfo* ResourceInfo = NPCInfo->GetGuildNPCInfoOfID(NPCID);

	if (ResourceInfo)
	{
		if (IMG_SelectNPCImage.IsValid())
			IMG_SelectNPCImage->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->NPCSlotIcon.LoadSynchronous()));
		
		if(IMG_UnSelectNPCImage.IsValid())
			IMG_UnSelectNPCImage->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->NPCUnSlotIcon.LoadSynchronous()));

		if (IMG_LockNPCImage.IsValid())
			IMG_LockNPCImage->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->NPCSlotLock.LoadSynchronous()));
	}

}

void UB2UIGuildMercenarySlot::SetSlotState(ENPCIconSlotState InIconSlot)
{

	O_Select->SetVisibility(ESlateVisibility::Hidden);
	O_UnSelect->SetVisibility(ESlateVisibility::Hidden);
	O_Lock->SetVisibility(ESlateVisibility::Hidden);

	if (InIconSlot == ENPCIconSlotState::Select)
	{
		O_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (InIconSlot == ENPCIconSlotState::UnSelect)
	{
		O_UnSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (InIconSlot == ENPCIconSlotState::Lock)
	{
		O_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	SlotState = InIconSlot;
}

void UB2UIGuildMercenarySlot::OnClickNPCButton()
{
	if (SlotState == ENPCIconSlotState::Lock)
	{
		LockPlaySoundEvent();
		return;
	}

	SelectPlaySoundEvent();
	SlotDelegate.ExecuteIfBound(NPCID);

	SetSlotState(ENPCIconSlotState::Select);
}


void UB2UIGuildMercenarySkillSlot::CacheAssets()
{
	GET_SLOT(UPanelWidget, O_VisiblePanel);
	GET_SLOT(UB2Button, BTN_SKill);
	GET_SLOT(UImage, IMG_Skill);

	GET_SLOT(UPanelWidget,	O_LockPanel);
	GET_SLOT(UImage,		IMG_LockImage);
	GET_SLOT(UTextBlock,	TB_LockLv);

}

void UB2UIGuildMercenarySkillSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_SKill, &UB2UIGuildMercenarySkillSlot::OnClickSkill);
}

void UB2UIGuildMercenarySkillSlot::Init()
{
	Super::Init();
}


void UB2UIGuildMercenarySkillSlot::SetIDAndSkillType(int32 InID, ENPCSKillType InSkillType, int32 InLevel, ENPCSpecialSKill InSpecialType)
{
	ID = InID;
	SkillType = InSkillType;
	Level = InLevel;
	SpecialType = InSpecialType;

	if (ID == 0)
		return;

	if (SkillType == ENPCSKillType::Special)
		SetSpecialType();
	else
		SetSkillType();
}

void UB2UIGuildMercenarySkillSlot::SetSpecialType()
{
	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();
	b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteries = GuildMasterData.GetMercenaryMasteries(ID, Level);

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();
	const FGuildNPCInfo* ResourceInfo = NPCInfo->GetGuildNPCInfoOfID(ID);

	if (O_VisiblePanel.IsValid())
		O_VisiblePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (O_LockPanel.IsValid())
		O_LockPanel->SetVisibility(ESlateVisibility::Hidden);

	if (!IMG_Skill.IsValid())
		return;

	if (IMG_Skill.IsValid())
	{
		switch (SpecialType)
		{
		case ENPCSpecialSKill::Option1:
			IMG_Skill->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill1.LoadSynchronous()));
			break;
		case ENPCSpecialSKill::Option2:
			IMG_Skill->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill2.LoadSynchronous()));
			break;
		case ENPCSpecialSKill::Option3:
			IMG_Skill->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill3.LoadSynchronous()));
			break;
		}
			
	}
}

void UB2UIGuildMercenarySkillSlot::SetSkillType()
{
	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();
	auto MercenarySkill = GuildMasterData.GetMercenarySkill(ID);

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();
	const FGuildMercenarySkill* ResourceInfo = NPCInfo->GetMercenarySkillOfID(ID);

	if (MercenarySkill && Level < MercenarySkill->require_mercenary_level)
	{
		if (O_LockPanel.IsValid() && O_VisiblePanel.IsValid())
		{
			O_LockPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			O_VisiblePanel->SetVisibility(ESlateVisibility::Hidden);
		}

		if(TB_LockLv.IsValid())
			TB_LockLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_MercenaryLevel")), FText::AsNumber(MercenarySkill->require_mercenary_level)));

		if(IMG_LockImage.IsValid() && ResourceInfo && ResourceInfo->SkillImageDisabled.IsValid())
			IMG_LockImage->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SkillImageDisabled.LoadSynchronous()));
	}
	else
	{
		if (O_LockPanel.IsValid() && O_VisiblePanel.IsValid())
		{
			O_LockPanel->SetVisibility(ESlateVisibility::Hidden);
			O_VisiblePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		if(IMG_Skill.IsValid() && ResourceInfo && ResourceInfo->SkillImageNormal.IsValid())
			IMG_Skill->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SkillImageNormal.LoadSynchronous()));
	}
}

void UB2UIGuildMercenarySkillSlot::OnClickSkill()
{
	UB2UIMsgPopupTooltipSkillOption* SkillOption = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupTooltipSkillOption>(UIFName::MsgPopupTooltipSkillOption);
	if (SkillOption)
	{

		if (SkillType == ENPCSKillType::Skill)
		{

			SkillOption->SetMercenarySkillID(ID);

		}
		else if (SkillType == ENPCSKillType::Special)
		{
			SkillOption->SetMercenarySpecialSKillID(ID, Level, SpecialType);
		}
	}
}

void UB2UIGuildMercenary::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_NPCName);
	GET_SLOT(UB2RichTextBlock, TB_NPCDesc);
	GET_SLOT(UTextBlock, TB_NPCLv);
	GET_SLOT(UTextBlock, TB_NPCLvNext);

	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_SpecialSkill1);
	if (UIP_SpecialSkill1.IsValid())
		UIP_SpecialSkill1->Init();
	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_SpecialSkill2);
	if (UIP_SpecialSkill2.IsValid())
		UIP_SpecialSkill2->Init();
	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_SpecialSkill3);
	if (UIP_SpecialSkill3.IsValid())
		UIP_SpecialSkill3->Init();
	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_Skill1);
	if (UIP_Skill1.IsValid())
		UIP_Skill1->Init();
	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_Skill2);
	if (UIP_Skill2.IsValid())
		UIP_Skill2->Init();
	GET_SLOT(UB2UIGuildMercenarySkillSlot, UIP_Skill3);
	if (UIP_Skill3.IsValid())
		UIP_Skill3->Init();

	GuildMercenarySlotArr.SetNum(9);

	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[0], UIP_GuildMercenarySlot1);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[1], UIP_GuildMercenarySlot2);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[2], UIP_GuildMercenarySlot3);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[3], UIP_GuildMercenarySlot4);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[4], UIP_GuildMercenarySlot5);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[5], UIP_GuildMercenarySlot6);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[6], UIP_GuildMercenarySlot7);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[7], UIP_GuildMercenarySlot8);
	GET_SLOT_BYNAME(UB2UIGuildMercenarySlot, GuildMercenarySlotArr[8], UIP_GuildMercenarySlot9);


	GET_SLOT(UButton,			BTN_Donatio)
	

	GET_SLOT(UScrollBox, SB_Mercenary)


	GET_SLOT(UB2Button, BTN_GoldDonatio)
	GET_SLOT(UB2RichTextBlock, TB_GoldDonatioExp)
	GET_SLOT(UB2UIStoreProductCost, UIP_GoldPrice)

	if(UIP_GoldPrice.IsValid())
		UIP_GoldPrice->Init();


	GET_SLOT(UB2Button, BTN_GemDonatio)
	GET_SLOT(UB2RichTextBlock, TB_GemDonatioExp)
	GET_SLOT(UB2UIStoreProductCost, UIP_GemPrice)

	if (UIP_GemPrice.IsValid())
		UIP_GemPrice->Init();

	GET_SLOT(UProgressBar , PB_Exp)
	GET_SLOT(UB2RichTextBlock , TB_Exp)


	GET_SLOT(UTextBlock, TB_DonatioCount)
	GET_SLOT(UTextBlock, TB_DonatioMaxCount)

	GET_SLOT(UImage, IMG_LevelMax)

	GET_SLOT(UImage, IMG_FX_bar_01)
	//static
	GET_SLOT(UB2RichTextBlock, TB_DonatioDesc)
	GET_SLOT(UTextBlock, TB_Special)
	GET_SLOT(UTextBlock, TB_Skill)

	GET_SLOT(UTextBlock, TB_GoldDonatio)
	GET_SLOT(UTextBlock, TB_GemDonatio)

	GET_SLOT(UTextBlock, TB_Donatio1)
	GET_SLOT(UTextBlock, TB_Donatio2)
}

void UB2UIGuildMercenary::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GoldDonatio , &UB2UIGuildMercenary::OnClickGoldDonatio)
	BIND_CLICK_FUNC_TO_BTN(BTN_GemDonatio, &UB2UIGuildMercenary::OnClickGemDonatio)
}

void UB2UIGuildMercenary::OnOpenComplete()
{
	Super::OnOpenComplete();

}

void UB2UIGuildMercenary::OnCloseComplete()
{
	Super::OnCloseComplete();

}

void UB2UIGuildMercenary::Init()
{
	Super::Init();
	UpgradeTimeRemain = 0;
	SetMercenary();
	UnSubscribeEvent();
	SubscribeEvent();

}

void UB2UIGuildMercenary::DestroySelf(UB2UIManager* InUIManager)
{

	UnSubscribeEvent();
	Super::DestroySelf(InUIManager);
}

void UB2UIGuildMercenary::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_MercenaryBtn"));
}

void UB2UIGuildMercenary::UpdateStaticText()
{
	if (TB_DonatioDesc.IsValid())
		TB_DonatioDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_DailyDonationCount")));

	if (TB_Special.IsValid())
		TB_Special->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_Talent")));

	if (TB_Skill.IsValid())
		TB_Skill->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_Skill")));

	if (TB_GoldDonatio.IsValid())
		TB_GoldDonatio->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_GoldDonation")));

	if (TB_GemDonatio.IsValid())
		TB_GemDonatio->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_GemDonation")));


	if (TB_Donatio1.IsValid())
		TB_Donatio1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_Donation")));

	if (TB_Donatio2.IsValid())
		TB_Donatio2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_Donation")));
}

void UB2UIGuildMercenary::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpgradeTick(InDeltaTime);
}

void UB2UIGuildMercenary::SubscribeEvent()
{
		DeliveryuildDonationTiket = DeliveryuildDonationClass<FB2ResponseGuildDonationPtr>::GetInstance().Subscribe(
		[this](FB2ResponseGuildDonationPtr InGuildDonatio)
	{
			DeliveryuildDonation(InGuildDonatio);
			
		}
	);

		DeliveryuildDonatioErrorTiket = DeliveryuildDonatioErrorClass<>::GetInstance().Subscribe(
			[this]()
		{
			DonatioButtonEnbled();
		}
		);
}

void UB2UIGuildMercenary::UnSubscribeEvent()
{
	DeliveryuildDonationClass<FB2ResponseGuildDonationPtr>::GetInstance().UnsubscribeAll();
	DeliveryuildDonatioErrorClass<>::GetInstance().UnsubscribeAll();
}

void UB2UIGuildMercenary::SetMercenary()
{
	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();


	auto NPCData = GuildData.GetGuildAllMercenaryData();				//현재 가지고있는 용병데이타
	//auto NPCMasterDataArr = GuildData.GetGuildMercenaryMasterData();			//용병 정보 마스터 데이타(전체)

	auto NPCMasterDataArr = GuildData.GetGuildMercenaryMasterData().GetMercenariesArr();


	int32 i = 0;
	for (auto ElNPCData : NPCMasterDataArr)
	{
		//auto NPCMasterData = NPCMasterDataArr.GetMercenaries(ElNPCData.Value->id);				//용병 정보 마스터 데이타(개인)

		if (GuildMercenarySlotArr[i].IsValid())
		{
			GuildMercenarySlotArr[i]->Init();
			GuildMercenarySlotArr[i]->SetSlot(ElNPCData.Value);
			GuildMercenarySlotArr[i]->SetNPCSelectSlotDelegate([this](int32 InNPCID) { SetMercenaryInfo(InNPCID); });
			
		}

		++i;
	}

	GuildMercenarySlotArr[0]->OnClickNPCButton();
	SelectSlot = GuildMercenarySlotArr[0].Get();
	
}

void UB2UIGuildMercenary::SetDonatioCount()
{
	int32 RemainCount = BladeIIGameImpl::GetClientDataStore().GetGuildMercenaryDonatioMaxCount() - BladeIIGameImpl::GetGuildDataStore().GetDonatioCount();

	if (TB_DonatioMaxCount.IsValid())
	{
		FString DonatioMaxCount = TEXT("/{0}");
		TB_DonatioMaxCount->SetText(FText::Format(FText::FromString(DonatioMaxCount),
			FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetGuildMercenaryDonatioMaxCount())));
	}

	if (TB_DonatioCount.IsValid())
	{
		TB_DonatioCount->SetText(FText::AsNumber(RemainCount));
		TB_DonatioCount->SetColorAndOpacity(RemainCount > 0 ? FLinearColor::White : FLinearColor::Red);
	}

	

}

void UB2UIGuildMercenary::SetMercenaryInfo(int32 InNPCID)
{
	/*UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();

	const FGuildNPCInfo* ResourceInfo = NPCInfo->GetGuildNPCInfoOfID(InNPCInfo->id);*/

	NPCID = InNPCID;
	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();

	auto NPCData = GuildData.GetGuildMercenaryData(NPCID);			//자기 용병레벨 경험치 등등 가져오기

	auto NPCMasterDataArr = GuildData.GetGuildMercenaryMasterData();		//용병 마스터 데이터 전체 가져오기
	auto NPCMercenariesData = GuildMasterData.GetMercenaries(NPCID);		//용병이 무슨스킬가지고있는지 사용여부등 가져오기
	b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteries = GuildMasterData.GetMercenaryMasteries(NPCID, NPCData->level);	//레벨업위한경험치, 전문화기술 뭐있나 가져오기

	b2network::B2mdGuildMercenaryDonationPtr MercenaryDonatioGold = GuildMasterData.GetMercenaryDonations(b2network::B2GuildMercenaryDonationType::GOLD);		//기부골드정보
	b2network::B2mdGuildMercenaryDonationPtr MercenaryDonatioGem = GuildMasterData.GetMercenaryDonations(b2network::B2GuildMercenaryDonationType::GEM);		//기부보석정보

	auto NPCMercenariesDataArr = NPCMasterDataArr.GetMercenariesArr();		//용병전체Array가져오기

	int32 i = 0;
	for (auto El : NPCMercenariesDataArr)
	{
		if (El.Value->is_usable == 0 && GuildMercenarySlotArr[i].IsValid())
			GuildMercenarySlotArr[i]->SetSlotState(ENPCIconSlotState::Lock);
		else
			GuildMercenarySlotArr[i]->SetSlotState(ENPCIconSlotState::UnSelect);

		if (GuildMercenarySlotArr[i]->GuildNPCID() == InNPCID)
		{
			GuildMercenarySlotArr[i]->SetSlotState(ENPCIconSlotState::Select);
			SelectSlot = GuildMercenarySlotArr[i].Get();
		}

		++i;
	}

	//이름
	if (TB_NPCName.IsValid())
		TB_NPCName->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::NPCName, NPCID)));

	//이름 설명
	if (TB_NPCDesc.IsValid())
		TB_NPCDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::NPCNameDesc, NPCID)));

	//레벨
	if (TB_NPCLv.IsValid())
		TB_NPCLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(NPCData->level)));
	if (TB_NPCLvNext.IsValid())
		TB_NPCLvNext->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(NPCData->level)));

	//경험치
	if (PB_Exp.IsValid())
	{
		float Exp = float(NPCData->exp) / float(MercenaryMasteries->max_exp);
		PB_Exp->SetPercent(Exp);
	}
	

	if (TB_Exp.IsValid())
	{
		FString ExpString = TEXT("({0}/{1})");
		TB_Exp->SetText(FText::Format(FText::FromString(ExpString), FText::AsNumber(NPCData->exp), FText::AsNumber(MercenaryMasteries->max_exp)));
	}

	//전문능력
	if (UIP_SpecialSkill1.IsValid())
	{
		UIP_SpecialSkill1->SetVisibility(MercenaryMasteries->option_id_1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_SpecialSkill1->SetIDAndSkillType(NPCID, ENPCSKillType::Special, NPCData->level, ENPCSpecialSKill::Option1);
	}

	if (UIP_SpecialSkill2.IsValid())
	{
		UIP_SpecialSkill2->SetVisibility(MercenaryMasteries->option_id_2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_SpecialSkill2->SetIDAndSkillType(NPCID, ENPCSKillType::Special, NPCData->level, ENPCSpecialSKill::Option2);
	}

	if (UIP_SpecialSkill3.IsValid())
	{
		UIP_SpecialSkill3->SetVisibility(MercenaryMasteries->option_id_3 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_SpecialSkill3->SetIDAndSkillType(NPCID, ENPCSKillType::Special, NPCData->level, ENPCSpecialSKill::Option3);
	}

	//스킬

	if (UIP_Skill1.IsValid())
	{
		UIP_Skill1->SetVisibility(NPCMercenariesData->skill_id_1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_Skill1->SetIDAndSkillType(NPCMercenariesData->skill_id_1, ENPCSKillType::Skill, NPCData->level);
	}
	if (UIP_Skill2.IsValid())
	{
		UIP_Skill2->SetVisibility(NPCMercenariesData->skill_id_2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_Skill2->SetIDAndSkillType(NPCMercenariesData->skill_id_2, ENPCSKillType::Skill, NPCData->level);
	}
	if (UIP_Skill3.IsValid())
	{
		UIP_Skill3->SetVisibility(NPCMercenariesData->skill_id_3 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		UIP_Skill3->SetIDAndSkillType(NPCMercenariesData->skill_id_3, ENPCSKillType::Skill, NPCData->level);
	}

	

	auto DocUser = UB2UIDocHelper::GetDocUser();

	check(DocUser)

	//기부쪽
	if (UIP_GoldPrice.IsValid())
	{	
		UIP_GoldPrice->SetCost(EStoreItemCost::Gold, MercenaryDonatioGold->donation_amount, 0);
	}

	if (TB_GoldDonatioExp.IsValid())
		TB_GoldDonatioExp->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_GetEXP")), FText::AsNumber(MercenaryDonatioGold->exp_increment)));

	if(UIP_GemPrice.IsValid())
	{	
		UIP_GemPrice->SetCost(EStoreItemCost::Gem, MercenaryDonatioGem->donation_amount, 0);
		
	}

	if (TB_GemDonatioExp.IsValid())
		TB_GemDonatioExp->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_GetEXP")), FText::AsNumber(MercenaryDonatioGem->exp_increment)));

	SetDonatioCount();

	int32 MaxLevel = NPCMasterDataArr.GetMercenaryMaxLevel(NPCData->id);

	DonatioButtonEnbled(MaxLevel == NPCData->level ? false : true);
	PB_Exp->SetVisibility(MaxLevel == NPCData->level ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	TB_Exp->SetVisibility(MaxLevel == NPCData->level ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	IMG_LevelMax->SetVisibility(MaxLevel == NPCData->level ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	
}

void UB2UIGuildMercenary::DonatioButtonEnbled(bool IsEnable)
{
	if(BTN_GoldDonatio.IsValid())
		BTN_GoldDonatio->SetIsEnabled(IsEnable);

	if(BTN_GemDonatio.IsValid())
		BTN_GemDonatio->SetIsEnabled(IsEnable);
}

void UB2UIGuildMercenary::DeliveryuildDonation(FB2ResponseGuildDonationPtr InGuildDonatio)
{
	auto GuildMemberInfo = BladeIIGameImpl::GetGuildDataStore().GetCurrentGuildMemberInfo();
	GuildMemberInfo->today_donation_count = InGuildDonatio->guild_member->today_donation_count;
	GuildMemberInfo->today_support_count = InGuildDonatio->guild_member->today_support_count;
	GuildMemberInfo->total_donation_count = InGuildDonatio->guild_member->total_donation_count;
	GuildMemberInfo->total_support_count = InGuildDonatio->guild_member->total_support_count;

	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();

	auto NPCData = GuildData.GetGuildMercenaryData(InGuildDonatio->mercenary->id);			//자기 용병레벨 경험치 등등 가져오기]
	if (NPCData)
	{
		BeforeExp = NPCData->exp;
		AfterExp = InGuildDonatio->mercenary->exp;
		BeforeLevel = NPCData->level;
		AfterLevel = InGuildDonatio->mercenary->level;

		NPCData->exp = InGuildDonatio->mercenary->exp;
		NPCData->level = InGuildDonatio->mercenary->level;

	
	}

	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, InGuildDonatio->light_account->cash);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, InGuildDonatio->light_account->money);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::BladePoint, InGuildDonatio->light_account->blade_point);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::DarkGem, InGuildDonatio->light_account->dark_crystal);

	UpgradeTimeRemain = UpgradeTimeMax;
	this->InceraseExpEvent();
	SelectSlot->InceraseExpEvent();
	LevelUpEffect = false;
	if (UpgradeTimeRemain <= 0)
		SetMercenaryInfo(NPCID);
}

void UB2UIGuildMercenary::OnClickGoldDonatio()
{
	BTN_GoldDonatio->SetIsEnabled(false);
	BTN_GemDonatio->SetIsEnabled(false);
	data_trader::Retailer::GetInstance().RequestGuildDonation(NPCID, b2network::B2GuildMercenaryDonationType::GOLD);
}

void UB2UIGuildMercenary::OnClickGemDonatio()
{
	BTN_GoldDonatio->SetIsEnabled(false);
	BTN_GemDonatio->SetIsEnabled(false);
	data_trader::Retailer::GetInstance().RequestGuildDonation(NPCID, b2network::B2GuildMercenaryDonationType::GEM);
}

void UB2UIGuildMercenary::UpgradeTick(float InDeltaTime)
{
	if (UpgradeTimeRemain > 0.f)
	{

		FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
		FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();
		b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteriesBeforeExp = GuildMasterData.GetMercenaryMasteries(NPCID, BeforeLevel);
		b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteriesAfterExp = GuildMasterData.GetMercenaryMasteries(NPCID, AfterLevel);


		UpgradeTimeRemain = UpgradeTimeRemain - InDeltaTime;

		int32 IncreaseExp = BeforeLevel == AfterLevel ? AfterExp - BeforeExp : (MercenaryMasteriesBeforeExp->max_exp - BeforeExp) + AfterExp;			//올라갈 양의 EXP
		int32 IncreaseTickExp = 0; //이번틱의 EXP
		int32 MaxExp = 0;
		float TickPercent = 0;

		if (BeforeExp + (IncreaseExp * ((UpgradeTimeMax - UpgradeTimeRemain) * 2))
					> MercenaryMasteriesBeforeExp->max_exp)
		{
			IncreaseTickExp = (IncreaseExp - (MercenaryMasteriesBeforeExp->max_exp - BeforeExp)) * ((UpgradeTimeMax - UpgradeTimeRemain) * 2);
			MaxExp = MercenaryMasteriesAfterExp->max_exp;
			TickPercent = float(IncreaseTickExp) / float(MaxExp);

			if (!LevelUpEffect)
			{
				SetMercenaryInfo(NPCID);

				LevelUpEvent();
				SelectSlot->LevelUpEvent();
				DonatioButtonEnbled(false);
				LevelUpEffect = true;
				if (TB_NPCLv.IsValid())
					TB_NPCLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(BeforeLevel)));
				if (TB_NPCLvNext.IsValid())
					TB_NPCLvNext->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(AfterLevel)));
			}
		}
		else
		{
			IncreaseTickExp = (IncreaseExp * ((UpgradeTimeMax - UpgradeTimeRemain) * 2)) + BeforeExp;
			MaxExp = MercenaryMasteriesBeforeExp->max_exp;
			TickPercent = float(IncreaseTickExp) / float(MaxExp);
		}




		if (UpgradeTimeRemain <= 0)
		{
			MaxExp = MercenaryMasteriesAfterExp->max_exp;
			IncreaseTickExp = AfterExp;
			TickPercent = float(IncreaseTickExp) / float(MercenaryMasteriesAfterExp->max_exp);
			UpgradeTimeRemain = 0.f;
			DonatioButtonEnbled(true);
			SetMercenaryInfo(NPCID);

			if (TB_NPCLv.IsValid())
				TB_NPCLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(BeforeLevel)));
			if (TB_NPCLvNext.IsValid())
				TB_NPCLvNext->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(AfterLevel)));
		}
		
		if (IMG_FX_bar_01.IsValid())
		{
			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(IMG_FX_bar_01->Slot);
			FMargin FxSlot = PanelSlot->GetOffsets();
			FxSlot.Right = FXbarXSize * TickPercent;
			PanelSlot->SetOffsets(FxSlot);
		}


		PB_Exp->SetPercent(TickPercent);
		FString ExpString = TEXT("({0}/{1})");
		TB_Exp->SetText(FText::Format(FText::FromString(ExpString), FText::AsNumber(IncreaseTickExp), FText::AsNumber(MaxExp)));

	}
}
