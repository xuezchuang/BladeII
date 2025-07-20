
#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildRound.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildRound : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = BladeII)
	class UParticleSystem* FightFX;

	UPROPERTY(EditAnywhere, Category = BladeII)
	float FightFxOffsetTime;

	UPROPERTY(EditAnywhere, Category = BladeII)
	float FightFxCamDistance;

	UPROPERTY(EditAnywhere, Category = BladeII)
	float FightFxScale;

	UPROPERTY(EditAnywhere, Category = BladeII)
	float ResultOffsetTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TMap<int32, UTexture2D*>	RoundTextures;

	UPROPERTY(BlueprintReadWrite, Category = BladeII)
	bool EndResultAnim;

public:

	UB2UIGuildRound(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void StartRoundUI(float Delay, bool InStartRound);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Begin")
	void OnPlayFightFx_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Begin")
	void OnPlayResult_BP(bool InIsWin);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Begin")
	void StartRoundUI_BP(bool InStartRound);

	virtual void CloseWidgetDelegate() override;

private:
	virtual void CacheAssets() override;

	void ShowFightFX();

	void PlayResult(bool InIsWin);

private:
	TWeakObjectPtr<UImage> IMG_Round;
	float ShowRemainTime;

	TWeakObjectPtr<class UParticleSystemComponent> PSCFightFX;

	bool bStartRound;
	bool bStart;
	bool bWin;
};