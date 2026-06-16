#include "WatcherEnemyCharacter.h"
#include "TimerManager.h"
#include "Watcher/WatcherAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

#include "Slate/SGameLayerManager.h"

AWatcherEnemyCharacter::AWatcherEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 1700.f;
	SightConfig->PeripheralVisionAngleDegrees = 70.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AWatcherEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	WatcherAIController = Cast<AWatcherAIController>(GetController());
	PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("Watcher Enemy BeginPlay - AI and Player linked"));

	if (CurrentState == EWatcherState::Patrol)
	{
		MoveToNextPatrolPoint();
	}
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AWatcherEnemyCharacter::OnTargetDetected);
	OriginalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AWatcherEnemyCharacter::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || bIsUnderFlashlight) return;
	if (!Actor)
	{
		return;
	}
	ACharacter* Player = Cast<ACharacter>(Actor);
	if (Player && Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
		SetWatcherState(EWatcherState::Chase);
		if (WatcherAIController)
		{
			WatcherAIController->MoveToActor(Player);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lost Player"))
		SetWatcherState(EWatcherState::Patrol);
		MoveToNextPatrolPoint();
	}
}

void AWatcherEnemyCharacter::MoveToNextPatrolPoint()
{
	if (bIsUnderFlashlight || !WatcherAIController) return;

	if (!WatcherAIController || PatrolPoints.Num() == 0)
	{
		return;
	}
	AActor* TargetPoint = PatrolPoints[CurrentPatrolIndex];

	if (TargetPoint)
	{
		WatcherAIController->MoveToActor(TargetPoint, 50.f);
	}
}

void AWatcherEnemyCharacter::OnMoveCompleted()
{
	if (bIsUnderFlashlight) return;
	if (CurrentState != EWatcherState::Patrol) return;

	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();

	FTimerHandle PatrolTimerHandle;
	GetWorldTimerManager().SetTimer(PatrolTimerHandle, this, &AWatcherEnemyCharacter::MoveToNextPatrolPoint,
	                                WaitTimeAtPatrolPoint);
}
void AWatcherEnemyCharacter::SetIsBeingFlashed(bool bIsFlashed)
{
	bIsUnderFlashlight = bIsFlashed;

	if (bIsUnderFlashlight)
	{
		if (WatcherAIController)
			WatcherAIController->StopMovement();

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
		}
	}
	else
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
		}

		if (CurrentState == EWatcherState::Chase)
		{
			if (WatcherAIController && PlayerRef)
				WatcherAIController->MoveToActor(PlayerRef);
		}
		else
		{
			MoveToNextPatrolPoint();
		}
	}
}


void AWatcherEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWatcherEnemyCharacter::SetWatcherState(EWatcherState NewState)
{
	if (CurrentState == NewState)
		return;

	CurrentState = NewState;

	UE_LOG(LogTemp, Warning, TEXT("Watcher State changed to: %d"), (uint8)CurrentState);
}

//
// void AWatcherEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
// }
