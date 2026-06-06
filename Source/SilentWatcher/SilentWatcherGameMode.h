// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SilentWatcherGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class ASilentWatcherGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASilentWatcherGameMode();
};



