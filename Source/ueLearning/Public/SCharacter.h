// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontag;
class UUserWidget;

UCLASS()
class UELEARNING_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	//基础组件
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	//攻击互动类组件
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	//UI组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrossHairAsset")
	TSubclassOf<class UUserWidget> CrossHairAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrossHairAsset")
	UUserWidget* CrossHair;

	// Called when the game starts or when spawned
	APlayerController* PC1;
	virtual void BeginPlay() override;

	//custom Function::move
	void MoveForward(float Value);
	void MoveRight(float Value);

	//custom Function::Interact
	void PrimaryInteract();

	//custom Function::Attack
	FTimerHandle TimerHandle_PrimaryAttack;
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	//custom Function::aim

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
