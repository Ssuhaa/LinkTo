// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_HorizonMagnet.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>

ASH_HorizonMagnet::ASH_HorizonMagnet()
{
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetRelativeScale3D(FVector(2.0f,2.0f,0.8f));
	InteractionMesh->SetRelativeLocation(FVector(220,0,0));
	InteractionMesh->SetSimulatePhysics(true);

	AxisComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxisMesh"));
	AxisComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));;
	if (tempMesh.Succeeded())
	{
		AxisComp->SetStaticMesh(tempMesh.Object);
	}
	AxisComp->SetRelativeScale3D(FVector(6.5f, 0.09, 0.09));

	PhyComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhyComp->SetupAttachment(RootComponent);
	PhyComp->ComponentName1.ComponentName = TEXT("interationMesh");
	PhyComp->ComponentName2.ComponentName = TEXT("AxisMesh");
	PhyComp->SetDisableCollision(true);
	PhyComp->SetLinearXLimit(LCM_Limited, 300.0f);
	PhyComp->SetLinearYLimit(LCM_Locked, 0.0f);
	PhyComp->SetLinearZLimit(LCM_Locked, 0.0f);
	PhyComp->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	PhyComp->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	PhyComp->SetAngularTwistLimit(ACM_Locked, 0.0f);
	PhyComp->SetAngularDriveMode(EAngularDriveMode::SLERP);
}

void ASH_HorizonMagnet::BeginPlay()
{
	Super::BeginPlay();
}

void ASH_HorizonMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
