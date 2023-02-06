// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ball.h"
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>

ASH_Ball::ASH_Ball()
{
	interationType = EObstacleType::Timelock;
	SetRootComponent(InteractionMesh);
	rootComp->DestroyComponent();
	ConstructorHelpers::FObjectFinder <UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		InteractionMesh->SetStaticMesh(TempMesh.Object);
	}
	InteractionMesh->SetMassOverrideInKg(FName(TEXT("NAME_None")),500.0f, true);
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
	InteractionMesh->SetSimulatePhysics(isActive);
	InteractionMesh->SetVisibility(isActive);

}

void ASH_Ball::OnTimeLock()
{
	Super::OnTimeLock();
	InteractionMesh->SetSimulatePhysics(false);
}

void ASH_Ball::releasedTimeLock()
{
	Super::releasedTimeLock();
	InteractionMesh->SetSimulatePhysics(true);
}

void  ASH_Ball ::InteractionTimeLock(bool isON)
{
	Super::InteractionTimeLock(isON);
}

