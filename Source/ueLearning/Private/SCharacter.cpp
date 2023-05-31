// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame. 
	//You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);


	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<USInteractionComponent>("InteractComp");
	

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC1 = GetWorld()->GetFirstPlayerController();
	if (CrossHairAsset)
	{
		CrossHair = CreateWidget<UUserWidget>(PC1, CrossHairAsset);
	}

	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Visible);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);

}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}


void ASCharacter::PrimaryInteract()
{
	if (InteractComp)
	{
		InteractComp->PrimaryInteract();
		 
	}
}

void ASCharacter::PrimaryAttack() 
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, 
		&ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensureAlways(ProjectileClass))
	{
		//aim
		FVector HitLocation;
		AimToTarget(HitLocation);

		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}

void ASCharacter::AimToTarget(FVector& HitLocation)
{
	if (GetSightRaylocation(HitLocation))
	{
		//TODO::set character world rotation;
	}
}

bool ASCharacter::GetSightRaylocation(FVector& OutHitLocation)
{
	int32 ViewportSizeX, ViewportSizeY;
	PC1->GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector(ViewportSizeX * )

	return false;
}

bool ASCharacter::GetLookVectorHitLocation(FVector LookLocation, FVector& OutHitLocation)
{
	return false;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, 
							&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, 
							&ASCharacter::PrimaryInteract);
}

