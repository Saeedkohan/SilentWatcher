#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WatcherState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "WatcherEnemyCharacter.generated.h"

class AWatcherAIController;

UCLASS()
class SILENTWATCHER_API AWatcherEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWatcherEnemyCharacter();
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EWatcherState CurrentState = EWatcherState::Patrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	TArray<AActor*> PatrolPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	float WaitTimeAtPatrolPoint = 2.0f;

	UFUNCTION(BlueprintCallable)
	void SetWatcherState(EWatcherState NewState);
	void MoveToNextPatrolPoint();
	void OnMoveCompleted();
	void SetIsBeingFlashed(bool isFlashed);

protected:
	virtual void BeginPlay() override;
	float OriginalWalkSpeed = 0.f;

	UPROPERTY()
	AWatcherAIController* WatcherAIController;
	UPROPERTY()
	ACharacter* PlayerRef;
	int32 CurrentPatrolIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(BlueprintReadOnly,Category="AI")
	bool bIsUnderFlashlight=false;
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
