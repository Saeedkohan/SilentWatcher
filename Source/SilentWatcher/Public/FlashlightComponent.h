#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashlightComponent.generated.h"

class USpotLightComponent;
class USpotLightComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SILENTWATCHER_API UFlashlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFlashlightComponent();

	void ToggleFlashlight();

	bool IsEnabled() const;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	USpotLightComponent* Flashlight;

	bool bIsEnabled = false;
};