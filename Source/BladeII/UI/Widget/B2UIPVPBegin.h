#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIPVPBegin.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPBegin : public UB2UIWidget
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

public:

	UB2UIPVPBegin(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFightFX_PlayTime(float ToSet);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Begin")
	void OnPlayFightFx_BP();

private:
	virtual void CacheAssets() override;

	void ShowFightFX();

private:
	TWeakObjectPtr<UImage> IMG_Ready;
	float ShowFightFXRemainTime;

	TWeakObjectPtr<class UParticleSystemComponent> PSCFightFX;
};