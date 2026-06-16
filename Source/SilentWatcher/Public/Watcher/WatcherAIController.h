#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "WatcherAIController.generated.h"

UCLASS()
class SILENTWATCHER_API AWatcherAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWatcherAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
