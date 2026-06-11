#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "PickupItem.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SILENTWATCHER_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	APickupItem();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties", meta = (ClampMin = "1"))
	int32 Quantity;

	virtual void Interact_Implementation(AActor* Interactor) override;
};
