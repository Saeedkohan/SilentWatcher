#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SilentWatcherCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class USpotLightComponent;
class UFlashlightComponent;
class UInventoryComponent;

struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
class AWatcherEnemyCharacter; 
UCLASS(abstract)
class ASilentWatcherCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* FlashlightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* ReloadAction;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* Flashlight;
	UPROPERTY(VisibleAnywhere)
	UFlashlightComponent* FlashlightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlashLight")
	float MaxBatteryLife = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlashLight")
	float CurrentBatteryLife;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlashLight")
	float BatteryDrainRate = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlashLight")
	bool bIsFlashlightOn = false;
	UPROPERTY(EditAnywhere, Category="FlashLight")
	float FlickerThreshold = 0.2f;
	float DefaultFlashlightIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;
	UUserWidget* PlayerHUD;

public:
	ASilentWatcherCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:
	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	void ToggleFlashlight();
	void StartSprint();
	void StopSprint();
	void Interact();
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void ReloadFlashlight();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* Inventory;
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintPure, Category="Flashlight")
	float GetBatteryPercent() const;

private:
	UPROPERTY(EditAnywhere, Category="Movement")
	float WalkSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float SprintSpeed = 900.f;
	AWatcherEnemyCharacter* LastFlashedEnemy = nullptr;

};
