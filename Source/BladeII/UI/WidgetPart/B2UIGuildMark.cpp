#include "B2UIGuildMark.h"
#include "B2GuildMarkInfo.h"
#include "BladeIIUtil.h"

void UB2UIGuildMark::Init()
{
	Super::Init();
}

void UB2UIGuildMark::CacheAssets()
{
	GET_SLOT(UB2Image,IMG_Pattern);
	GET_SLOT(UB2Image, IMG_BG);
	GET_SLOT(UB2Image, IMG_Border);

}

void UB2UIGuildMark::UpdateStaticText()
{

}

void UB2UIGuildMark::BindDelegates()
{

}

void UB2UIGuildMark::SetGuildMark(int32 InPatternIndex, int32 InPetternColor, int32 InBGImage, int32 InBGColor)
{
	UB2GuildMarkInfo* GuildMarkInfo = StaticFindGuildMarkInfo();

	check(GuildMarkInfo);

	UMaterialInstance* GuildMark = nullptr;
	UMaterialInstance* GuildBG = nullptr;
	UMaterialInstance* GuildBGBorder = nullptr;

	if (GuildMarkInfo->GetPatternImageNum() > InPatternIndex)
		GuildMark = GuildMarkInfo->GetPatternImage(InPatternIndex);

	if (GuildMarkInfo->GetBGImageNum() > InBGImage)
		GuildBG = GuildMarkInfo->GetBGImage(InBGImage);

	if (GuildMarkInfo->GetPatternColorNum() > InPetternColor)
		PatternColor = GuildMarkInfo->GetPatternColor(InPetternColor);

	if (GuildMarkInfo->GetBGColorNum() > InBGColor)
		BGColor = GuildMarkInfo->GetBGColor(InBGColor);

	if (GuildMarkInfo->GetBGBorderNum() > InBGImage)
		GuildBGBorder = GuildMarkInfo->GetBGBorder(InBGImage);

	if (IMG_Pattern.IsValid())
	{
		IMG_Pattern->SetBrushFromMaterial(GuildMark);
		auto DM = IMG_Pattern->GetDynamicMaterial();
		DM->SetVectorParameterValue(MtrlParamName_UIAdditionalBlendColor, PatternColor);
	}

	if (IMG_BG.IsValid())
	{
		IMG_BG->SetBrushFromMaterial(GuildBG);
		auto DM = IMG_BG->GetDynamicMaterial();
		DM->SetVectorParameterValue(MtrlParamName_UIAdditionalBlendColor, BGColor);
	}

	if (IMG_Border.IsValid())
		IMG_Border->SetBrushFromMaterial(GuildBGBorder);

}
