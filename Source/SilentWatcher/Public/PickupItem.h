#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "PickupItem.generated.h"

UCLASS()
class SILENTWATCHER_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	APickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	virtual void Interact_Implementation(AActor* Interactor) override;

};
