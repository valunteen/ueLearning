// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileCannon.h"

ASProjectileCannon::ASProjectileCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASProjectileCannon::BeginPlay()
{
	Super::BeginPlay();
}

void ASProjectileCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}