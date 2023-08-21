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
	SpawnProjectile(BaseProjectileClass);
}

void ASCharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&ASCharacter::DashAttack_TimeElapsed, 0.2f);
}

void ASCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		//aim
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FVector BeginLocation = CameraComp->GetComponentLocation();
		FVector EndLocation = BeginLocation + 5000 * GetControlRotation().Vector();
		FRotator SpawRotator;
		FHitResult Hits;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		if (GetWorld()->SweepSingleByObjectType(Hits, BeginLocation, EndLocation, FQuat::Identity, ObjParams, Shape, Params))
		{
			EndLocation = Hits.ImpactPoint;
		}
		SpawRotator = FRotationMatrix::MakeFromX(EndLocation - HandLocation).Rotator();

		
		FTransform SpawnTM = FTransform(SpawRotator, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
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
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this,
							&ASCharacter::DashAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this,
							&ASCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, 
							&ASCharacter::PrimaryInteract);
}

