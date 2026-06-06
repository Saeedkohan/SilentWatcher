#include "PickupItem.h"

#include "SilentWatcherCharacter.h"
#include "InventoryComponent.h"


APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(Root);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupItem::Interact_Implementation(AActor* Interactor)
{
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (ASilentWatcherCharacter* Character = Cast<ASilentWatcherCharacter>(Player))
	{
		if (Character->Inventory)
		{
			Character->Inventory->AddItem(ItemID, 1);
		}
	}

	Destroy();
}
