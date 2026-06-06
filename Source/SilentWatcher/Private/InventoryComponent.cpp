#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddItem(FName ItemID, int32 Amount)
{
	if (Items.Contains(ItemID))
	{
		Items[ItemID] += Amount;
	}
	else
	{
		Items.Add(ItemID, Amount);
	}

	UE_LOG(LogTemp, Warning, TEXT("Added Item: %s x%d"), *ItemID.ToString(), Amount);
}

bool UInventoryComponent::HasItem(FName ItemID)
{
	return Items.Contains(ItemID);
}

bool UInventoryComponent::ConsumeItem(FName ItemID, int32 Amount)
{
	if (!Items.Contains(ItemID))
		return false;

	if (Items[ItemID] < Amount)
		return false;

	Items[ItemID] -= Amount;

	if (Items[ItemID] <= 0)
	{
		Items.Remove(ItemID);
	}

	return true;
}
