#include "Public/FlashlightComponent.h"

#include "Components/SpotLightComponent.h"

UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFlashlightComponent::ToggleFlashlight()
{
	if (!Flashlight)
	{
		return;
	}

	bIsEnabled = !bIsEnabled;

	Flashlight->SetVisibility(bIsEnabled);

	UE_LOG(LogTemp,Warning,TEXT("Flashlight: %s"),bIsEnabled ? TEXT("ON") : TEXT("OFF"));
}

bool UFlashlightComponent::IsEnabled() const
{
	return bIsEnabled;
}


void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
	Flashlight = GetOwner()->FindComponentByClass<USpotLightComponent>();
	if (Flashlight)
	{
		Flashlight->SetVisibility(false);
	}
}
