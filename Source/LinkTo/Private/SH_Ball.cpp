// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ball.h"
#include <Components/StaticMeshComponent.h>

ASH_Ball::ASH_Ball()
{
	PrimaryActorTick.bCanEverTick = true;
	interationType = EObstacleType::Timelock;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("Ball"));
	BallMesh->SetupAttachment(RootComponent);
	BallMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ConstructorHelpers::FObjectFinder <UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		BallMesh->SetStaticMesh(TempMesh.Object);
	}
}

void ASH_Ball:: BeginPlay()
{
	Super::BeginPlay();
	OriginPos = GetActorLocation();
}

void ASH_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASH_Ball::SetActiveBall(bool isActive)
{

	if (isActive)
	{

	}
	else
	{
	  SetActorLocation(OriginPos);
	}
	BallMesh->SetSimulatePhysics(isActive);
	BallMesh->SetVisibility(isActive);

}