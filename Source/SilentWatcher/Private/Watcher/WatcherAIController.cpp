#include "Watcher/WatcherAIController.h"
#include "WatcherEnemyCharacter.h"

AWatcherAIController::AWatcherAIController()
{
	bAttachToPawn = true;
}

void AWatcherAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AWatcherAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	AWatcherEnemyCharacter* Enemy = Cast<AWatcherEnemyCharacter>(GetPawn());
	if (Enemy)
	{
		Enemy->OnMoveCompleted();
	}
}
