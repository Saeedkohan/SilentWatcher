#include "PickupItem.h"
#include "SilentWatcherCharacter.h"
#include "InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;


	Quantity = 1;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Root);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickupItem::Interact_Implementation(AActor* Interactor)
{
	if (ASilentWatcherCharacter* Character = Cast<ASilentWatcherCharacter>(Interactor))
	{
		if (Character->Inventory)
		{
			Character->Inventory->AddItem(ItemID, Quantity);
			UE_LOG(LogTemp, Warning, TEXT("Picked up %d of %s"), Quantity, *ItemID.ToString());

			Destroy();
		}
	}
}
