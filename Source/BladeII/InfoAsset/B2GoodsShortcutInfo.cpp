// Fill out your copyright notice in the Description page of Project Settings.


#include "B2GoodsShortcutInfo.h"



UB2GoodsShortcutInfo::UB2GoodsShortcutInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UB2GoodsShortcutInfo::PostLoad()
{
	for (int i = 0; i < ShortcutInfoArray.Num(); i++)
	{
		m_GoodsShortcutInfoMap.Add(ShortcutInfoArray[i].ShortcutInfoID, ShortcutInfoArray[i]);
	}
	Super::PostLoad();
}


/////////////////////////////////////////////////////////////////////////////////////////////
//[@AKI, 170331] 捞 酒贰 巴甸篮 B2SomeInfo巴阑 啊瘤绊 柯 巴烙.

// 眠啊登绰 巴甸 付蛮啊瘤肺..

void UB2GoodsShortcutInfo::UnloadAll()
{
	//UnloadStageClearWingFxTemplate();
}

#if WITH_EDITOR
void UB2GoodsShortcutInfo::EditorLoadAll()
{
	if (GIsEditor == false) {
		return;
	}

	// 茄锅 固府 阂矾林搁 AssetPtr 捞 valid 秦 龙 巴. 拱沸 捞 客吝俊 皋葛府绰 空芒 冈摆瘤.
	//GetStageClearWingFxTemplate();
	
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	GetPCIntroFlipbookMtrl(IntToPCClass(PCI));
	//}
}
#endif

// TAsset lazy-loading 阑 荤侩窍绰 Blueprint InfoAsset 狼 坷悼累 巩力甫 沥侥栏肺 秦搬窍扁 傈鳖瘤狼 烙矫 东龙.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2GoodsShortcutInfo::CleanupOnPreSave()
{
	//LoadedStageClearWingFxTemplate = NULL;
	//LoadedPCIntroFlipbookMtrls.Empty();
}

void UB2GoodsShortcutInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	//CleanupOnPreSave();
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////