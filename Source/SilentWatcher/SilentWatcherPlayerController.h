#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SilentWatcherPlayerController.generated.h"

class UInputMappingContext;

UCLASS(abstract)
class SILENTWATCHER_API ASilentWatcherPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	ASilentWatcherPlayerController();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	virtual void SetupInputComponent() override;

};
