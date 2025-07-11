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
//[@AKI, 170331] 이 아래 것들은 B2SomeInfo것을 가지고 온 것임.

// 추가되는 것들 마찬가지로..

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

	// 한번 미리 불러주면 AssetPtr 이 valid 해 질 것. 물론 이 와중에 메모리는 왕창 먹겠지.
	//GetStageClearWingFxTemplate();
	
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	GetPCIntroFlipbookMtrl(IntToPCClass(PCI));
	//}
}
#endif

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2GoodsShortcutInfo::CleanupOnPreSave()
{
	//LoadedStageClearWingFxTemplate = NULL;
	//LoadedPCIntroFlipbookMtrls.Empty();
}

void UB2GoodsShortcutInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	//CleanupOnPreSave();
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////