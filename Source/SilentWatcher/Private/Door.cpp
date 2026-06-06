#include "Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	DoorFrame->SetupAttachment(Root);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(DoorFrame);
	bIsOpen = false;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = DoorMesh->GetRelativeRotation();
	OpenRotation = ClosedRotation + FRotator(0, OpenAngle, 0);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator TargetRotation = bIsOpen ? OpenRotation : ClosedRotation;
	FRotator NewRotation = FMath::RInterpTo(DoorMesh->GetRelativeRotation(), TargetRotation, DeltaTime, OpenSpeed);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoor::Interact_Implementation(AActor* Interactor)
{
	ToggleDoor();
}

void ADoor::ToggleDoor()
{
	bIsOpen = !bIsOpen;
}
