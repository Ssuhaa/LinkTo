// Fill out your copyright notice in the Description page of Project Settings.


#include "obstacleBase.h"

// Sets default values
AobstacleBase::AobstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AobstacleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AobstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AobstacleBase::isDelay(float DelayTime)
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > DelayTime)
	{
		currentTime = 0;
		return true;
	}
	else
	{
		return false;
	}
}