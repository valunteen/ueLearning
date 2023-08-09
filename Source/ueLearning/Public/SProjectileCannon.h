// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseMagicProjectile.h"
#include "SProjectileCannon.generated.h"

/**
 * 
 */
UCLASS()
class UELEARNING_API ASProjectileCannon : public ASBaseMagicProjectile
{
	GENERATED_BODY()

private:
	ASProjectileCannon();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
