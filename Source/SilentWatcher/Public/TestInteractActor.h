#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "TestInteractActor.generated.h"

UCLASS()
class SILENTWATCHER_API ATestInteractActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
};
