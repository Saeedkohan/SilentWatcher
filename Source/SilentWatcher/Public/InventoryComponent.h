#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SILENTWATCHER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> Items;

	void AddItem(FName ItemID, int32 Amount = 1);

	bool HasItem(FName ItemID);

	bool ConsumeItem(FName ItemID, int32 Amount = 1);
};