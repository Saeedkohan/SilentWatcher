#pragma once
#include "CoreMinimal.h"
#include "WatcherState.generated.h"

UENUM(BlueprintType)
enum class EWatcherState : uint8
{
	Patrol      UMETA(DisplayName = "Patrol"),
	Investigate UMETA(DisplayName = "Investigate"),
	Chase       UMETA(DisplayName = "Chase")
};
