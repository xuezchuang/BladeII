#include "BladeII.h"
#include "B2UISkillOptionList.h"
#include "B2UISkillOptionText.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UISkillOptionList::SetSkillOptionType(ESkillOptionTitleType InOptionType, int32 SkillLevel)
{
	OptionType = InOptionType;

	for (auto& TypeSkin : SkillOptionListTitleSkin)
	{
		if (TypeSkin.Type == OptionType)
		{
			if (IMG_Title.IsValid())
				IMG_Title->SetBrushFromMaterial(TypeSkin.TitleImage);

			if (TB_Title.IsValid())
			{
				if (InOptionType == ESkillOptionTitleType::SpecialOption)
					TB_Title->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TypeSkin.TextCodeName), FText::AsNumber(SkillLevel)));
				else
					TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TypeSkin.TextCodeName));
				TB_Title->SetColorAndOpacity(TypeSkin.FontColor);
			}

			return;
		}
	}
}

void UB2UISkillOptionList::CacheAssets()
{
	GET_SLOT(UVerticalBox,	VB_SkillOptionList);
	GET_SLOT(UImage,		IMG_Title);
	GET_SLOT(UTextBlock,	TB_Title);
}

TArray<FSkillOptionData> UB2UISkillOptionList::GetSkillOptions(int32 SkillId, int32 SkillLevel, bool OnlyNew)
{
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto* SkillMasterData = ClientDataStore.GetSkillMasterData(SkillId);

	TArray<FSkillOptionData> SkillOptionDataArray;

	if (SkillMasterData)
	{
		SkillMasterData->LevelOptionData.MultiFind(SkillLevel, SkillOptionDataArray);

		if (OnlyNew && SkillLevel > 0)
		{
			TArray<FSkillOptionData> OldSkillOptionDataArray;
			SkillMasterData->LevelOptionData.MultiFind(SkillLevel - 1, OldSkillOptionDataArray);

			for (auto& OldSkillOption : OldSkillOptionDataArray)
			{
				SkillOptionDataArray.Remove(OldSkillOption);

				for (auto& NewSkillOption : SkillOptionDataArray)
				{
					if (NewSkillOption.OptionId == OldSkillOption.OptionId && NewSkillOption.OptionId != (int32)ESkillOption::ESO_IncMotion)
					{//OptionId는 같고 Value가 다른 경우, 모션증가 제외
						NewSkillOption.OptionValue -= OldSkillOption.OptionValue;
					}
				}
			}
		}

		//모션증가에 대한 예외처리
		for (int32 i = 0; !OnlyNew && i < SkillOptionDataArray.Num(); ++i)
		{
			auto& SkillOption = SkillOptionDataArray[i];
			if (SkillOption.OptionId == (int32)ESkillOption::ESO_IncMotion && SkillOption.OptionValue > 1)
			{
				SkillOptionDataArray.Insert(FSkillOptionData(SkillOption.OptionId, SkillOption.OptionValue - 1), i + 1);
			}
		}
	}

	return SkillOptionDataArray;
}

void UB2UISkillOptionList::DisplaySkillOptions(int32 SkillId, TArray<FSkillOptionData>& SkillOptionDataArray)
{
	//ToDo : Sorting
	SkillOptionDataArray.Sort([SkillId](const FSkillOptionData& A, const FSkillOptionData& B) {
		auto& LocalClientDataStore = BladeIIGameImpl::GetClientDataStore();

		return LocalClientDataStore.GetSkillMinLevelAcquiredOption(SkillId, A.OptionId) < LocalClientDataStore.GetSkillMinLevelAcquiredOption(SkillId, B.OptionId);
	});

	//
	if (VB_SkillOptionList.IsValid())
	{
		VB_SkillOptionList->ClearChildren();

		for (auto& SkillOption : SkillOptionDataArray)
		{
			auto* CreatedWidget = CreateWidget<UB2UISkillOptionText>(GetOwningPlayer(), SkillOptionTextClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_SkillOptionList->AddChild(CreatedWidget));
				CreatedWidget->Init();

				if (SkillOption.OptionId != (int32)ESkillOption::ESO_IncMotion)
					CreatedWidget->SetSkillOption(FText::Format(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, SkillId, SkillOption.OptionId)), FText::AsNumber(SkillOption.OptionValue)));
				else
					CreatedWidget->SetSkillOption(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, SkillId, SkillOption.OptionId, SkillOption.OptionValue)));
			}
		}
	}
}


void UB2UISkillOptionList::DisplayUnitySkillOptions(int32 SkillId,const TArray<FUnitySkillOption>& UnityDataArray)
{

	if (VB_SkillOptionList.IsValid())
	{
		VB_SkillOptionList->ClearChildren();

		for (auto& SkillOption : UnityDataArray)
		{
			auto* CreatedWidget = CreateWidget<UB2UISkillOptionText>(GetOwningPlayer(), SkillOptionTextClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_SkillOptionList->AddChild(CreatedWidget));
				CreatedWidget->Init();

				if (SkillOption.OptionId != (int32)ESkillOption::ESO_IncMotion)
					CreatedWidget->SetSkillOption(FText::Format(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, SkillId, SkillOption.OptionId)), FText::AsNumber(SkillOption.OptionValue)));
				else
					CreatedWidget->SetSkillOption(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, SkillId, SkillOption.OptionId, SkillOption.OptionValue)));
			}
		}
	}
}

void UB2UISkillOptionList::InitializeMercenaryOptions(int32 NPCID, int32 InLv, ENPCSpecialSKill InSpecialSkillTpye)
{
	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();

	b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteries = GuildMasterData.GetMercenaryMasteries(NPCID, InLv);	//레벨업위한경험치, 전문화기술 뭐있나 가져오기

	check(MercenaryMasteries)

	if (VB_SkillOptionList.IsValid())
	{
		VB_SkillOptionList->ClearChildren();

		auto* CreatedWidget = CreateWidget<UB2UISkillOptionText>(GetOwningPlayer(), SkillOptionTextClass);
		if (CreatedWidget != NULL)
		{
			auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_SkillOptionList->AddChild(CreatedWidget));
			CreatedWidget->Init();
			
			if(InSpecialSkillTpye == ENPCSpecialSKill::Option1)
				CreatedWidget->SetSkillOption(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillOption1, NPCID)), FText::AsNumber(MercenaryMasteries->option_value_1)));
			else if(InSpecialSkillTpye == ENPCSpecialSKill::Option2)
				CreatedWidget->SetSkillOption(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillOption2, NPCID)), FText::AsNumber(MercenaryMasteries->option_value_2)));
			else if (InSpecialSkillTpye == ENPCSpecialSKill::Option3)
				CreatedWidget->SetSkillOption(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillOption3, NPCID)), FText::AsNumber(MercenaryMasteries->option_value_3)));

		}

	}
}

void UB2UISkillOptionList::InitializeList(int32 SkillId, int32 SkillLevel, bool bOnlyNew)
{
	TArray<FSkillOptionData> SkillOptions = GetSkillOptions(SkillId, SkillLevel, bOnlyNew);
	DisplaySkillOptions(SkillId, SkillOptions);
}

void UB2UISkillOptionList::InitializeUnitySkillList(int32 SkillId, EPCClass MainClass)
{
	const TArray<FUnitySkillOption> OptionList = GLOBALUNITYSKILLMANAGER.GetUnitySkillOptionInfoList(MainClass);

	DisplayUnitySkillOptions(SkillId, OptionList);
}

void UB2UISkillOptionList::SetSpecialOptionList(int32 SkillId, int32 SkillLevel)
{
	auto* PCClassInfoBox = StaticFindPCClassInfoBox();
	UB2SkillInfo* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;

	if (AllSkillInfo)
	{
		TArray<FSkillOptionData> DisplayOptions;
		TArray<FSkillOptionData> SkillOptions = GetSkillOptions(SkillId, SkillLevel, true);
		TArray<ESkillOption> SpecialDisplayOptions = AllSkillInfo->GetSpecialDisplaySkillOption(SkillId, SkillLevel);
		for (auto& OptionData : SkillOptions)
		{
			for (auto &SkillOption : SpecialDisplayOptions)
			{
				if (OptionData.OptionId == (int32)SkillOption)
				{
					DisplayOptions.Add(OptionData);
				}
			}
		}

		DisplaySkillOptions(SkillId, DisplayOptions);
	}
}
