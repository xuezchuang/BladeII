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
//[@AKI, 170331] �� �Ʒ� �͵��� B2SomeInfo���� ������ �� ����.

// �߰��Ǵ� �͵� ����������..

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

	// �ѹ� �̸� �ҷ��ָ� AssetPtr �� valid �� �� ��. ���� �� ���߿� �޸𸮴� ��â �԰���.
	//GetStageClearWingFxTemplate();
	
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	GetPCIntroFlipbookMtrl(IntToPCClass(PCI));
	//}
}
#endif

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2GoodsShortcutInfo::CleanupOnPreSave()
{
	//LoadedStageClearWingFxTemplate = NULL;
	//LoadedPCIntroFlipbookMtrls.Empty();
}

void UB2GoodsShortcutInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	//CleanupOnPreSave();
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////