#include "SilentWatcherCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "FlashlightComponent.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SpotLightComponent.h"
#include "Components/LightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASilentWatcherCharacter::ASilentWatcherCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f),
	                                                           FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;


	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCameraComponent);
	Flashlight->SetVisibility(false);
	Flashlight->Intensity = 20000.f;
	Flashlight->AttenuationRadius = 3000.f;
	FlashlightComponent = CreateDefaultSubobject<UFlashlightComponent>(TEXT("FlashlightComponent"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");

	CurrentBatteryLife = MaxBatteryLife;
	bIsFlashlightOn = false;
}

void ASilentWatcherCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Flashlight)
	{
		DefaultFlashlightIntensity=Flashlight->Intensity;
	}

	if (Inventory)
	{
		Inventory->AddItem("Battery", 2);
		UE_LOG(LogTemp, Warning, TEXT("Test Batteries Added"));
	}

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}

}

void ASilentWatcherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFlashlightOn && Flashlight)
	{
		CurrentBatteryLife -= BatteryDrainRate * DeltaTime;

		float BatteryPercent = CurrentBatteryLife / MaxBatteryLife;

		if (BatteryPercent <= FlickerThreshold)
		{
			float FlickerChance = FMath::FRand(); 
            
			if (FlickerChance > 0.8f) 
			{
				float RandomIntensity = FMath::FRandRange(0.2f, 1.0f);
				Flashlight->SetIntensity(DefaultFlashlightIntensity * RandomIntensity);
			}
			else
			{
				Flashlight->SetIntensity(DefaultFlashlightIntensity);
			}
		}
		else
		{
			Flashlight->SetIntensity(DefaultFlashlightIntensity);
		}

		if (CurrentBatteryLife <= 0)
		{
			ToggleFlashlight(); 
			CurrentBatteryLife = 0;
		}
	}
}

void ASilentWatcherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &ASilentWatcherCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ASilentWatcherCharacter::DoJumpEnd);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &ASilentWatcherCharacter::MoveInput);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &ASilentWatcherCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &ASilentWatcherCharacter::LookInput);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &ASilentWatcherCharacter::StartSprint);

		EnhancedInputComponent->BindAction(
			SprintAction,
			ETriggerEvent::Completed,
			this,
			&ASilentWatcherCharacter::StopSprint
		);

		EnhancedInputComponent->BindAction(
			FlashlightAction,
			ETriggerEvent::Started,
			this,
			&ASilentWatcherCharacter::ToggleFlashlight
		);

		EnhancedInputComponent->BindAction(
			InteractAction,
			ETriggerEvent::Started,
			this,
			&ASilentWatcherCharacter::Interact
		);


		EnhancedInputComponent->BindAction(
			ReloadAction,
			ETriggerEvent::Started,
			this,
			&ASilentWatcherCharacter::ReloadFlashlight
		);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

float ASilentWatcherCharacter::GetBatteryPercent() const
{
	if (MaxBatteryLife <= 0)
	{
		return 0.f;
	}
	return CurrentBatteryLife / MaxBatteryLife;
}


void ASilentWatcherCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void ASilentWatcherCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ASilentWatcherCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASilentWatcherCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ASilentWatcherCharacter::DoJumpStart()
{
	Jump();
}

void ASilentWatcherCharacter::DoJumpEnd()
{
	StopJumping();
}

void ASilentWatcherCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASilentWatcherCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASilentWatcherCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Pressed"));

	FHitResult Hit;

	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	const FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 300.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);

	// DrawDebugLine(GetWorld(),Start,End,bHit ? FColor::Green : FColor::Red,false,2.f,0,2.f);

	if (!bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing Hit"));
		return;
	}

	AActor* HitActor = Hit.GetActor();

	if (!HitActor)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

	IInteractable* Interactable = Cast<IInteractable>(HitActor);

	if (!Interactable)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not Interactable"), *HitActor->GetName());

		return;
	}

	if (HitActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(HitActor, this);
	}
}

void ASilentWatcherCharacter::ReloadFlashlight()
{
	static const FName BatteryItemID(TEXT("Battery"));

	if (!Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is null!"));
		return;
	}

	if (CurrentBatteryLife >= MaxBatteryLife)
	{
		UE_LOG(LogTemp, Warning, TEXT("Battery is already full!"));
		return;
	}

	if (Inventory->ConsumeItem(BatteryItemID, 1))
	{
		CurrentBatteryLife = MaxBatteryLife;
		UE_LOG(LogTemp, Warning, TEXT("Flashlight Reloaded!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Battery in Inventory!"));
	}
}


void ASilentWatcherCharacter::ToggleFlashlight()
{
	if (CurrentBatteryLife <= 0 && !bIsFlashlightOn)
		return;

	if (FlashlightComponent)
	{
		FlashlightComponent->ToggleFlashlight();
		bIsFlashlightOn = FlashlightComponent->IsEnabled();
	}
}
