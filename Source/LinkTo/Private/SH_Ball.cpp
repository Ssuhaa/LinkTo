// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ball.h"
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>

ASH_Ball::ASH_Ball()
{
	SetRootComponent(InteractionMesh);
	rootComp->DestroyComponent();
	ConstructorHelpers::FObjectFinder <UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Geometry/Mesh/SM_Ball.SM_Ball'"));
	if (TempMesh.Succeeded())
	{
		InteractionMesh->SetStaticMesh(TempMesh.Object);
	}
	InteractionMesh->SetMassOverrideInKg(FName(TEXT("NAME_None")),900.0f, true);
	InteractionMesh->SetCollisionProfileName(TEXT("BlockAll"));

	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Ball.MI_Ball'"));
	if (TempMat.Succeeded())
	{
		TimeLockMatArray.Add(TempMat.Object);
		InteractionMesh->SetMaterial(0, TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_BallTimeLockOn.MI_BallTimeLockOn'"));
	if (TempMat1.Succeeded())
	{
		TimeLockMatArray.Add(TempMat1.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_BallTimeLockSelect.MI_BallTimeLockSelect'"));
	if (TempMat2.Succeeded())
	{
		TimeLockMatArray.Add(TempMat2.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_BallTimeLockCount.MI_BallTimeLockCount'"));
	if (TempMat3.Succeeded())
	{
		TimeLockMatArray.Add(TempMat3.Object);
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
	InteractionMesh->SetSimulatePhysics(isActive);
	InteractionMesh->SetVisibility(isActive);

}

void ASH_Ball::OnTimeLock()
{
	Super::OnTimeLock();
	if (bTimeLock)
	{
		InteractionMesh->SetSimulatePhysics(false);
	}
}

void ASH_Ball::releasedTimeLock()
{
	Super::releasedTimeLock();
	InteractionMesh->SetSimulatePhysics(true);
}

void ASH_Ball::Goal(bool Physics)
{
	InteractionMesh->SetSimulatePhysics(Physics);
}

void  ASH_Ball ::InteractionTimeLock(bool isON)
{
	Super::InteractionTimeLock(isON);
}

