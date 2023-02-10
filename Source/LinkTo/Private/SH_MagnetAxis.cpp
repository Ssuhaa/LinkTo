// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_MagnetAxis.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>

ASH_MagnetAxis::ASH_MagnetAxis()
{
	SetRootComponent(rootComp);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);
	InteractionMesh->SetRelativeScale3D(FVector(6.0f, 0.2f, 0.2f));

	PhyComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhyComp->SetupAttachment(RootComponent);
	PhyComp->ComponentName1.ComponentName = TEXT("interationMesh");
	PhyComp->ComponentName2.ComponentName = TEXT("interationMesh");
	PhyComp->SetDisableCollision(true);
	PhyComp->ConstraintInstance.SetLinearLimits(LCM_Limited, LCM_Locked, LCM_Locked, 300.0f);
	PhyComp->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	PhyComp->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	PhyComp->SetAngularTwistLimit(ACM_Locked, 0.0f);
	PhyComp->SetAngularDriveMode(EAngularDriveMode::SLERP);
}

void ASH_MagnetAxis::BeginPlay()
{

}

void ASH_MagnetAxis::Tick(float DeltaTime)
{

}
