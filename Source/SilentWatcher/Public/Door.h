#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Door.generated.h"

UCLASS()
class SILENTWATCHER_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAngle = -90.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 2.f;

	bool bIsOpen;

	FRotator ClosedRotation;
	FRotator OpenRotation;

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Interactor) override;

private:

	void ToggleDoor();

};
