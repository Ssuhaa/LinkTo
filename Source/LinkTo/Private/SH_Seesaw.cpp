// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Seesaw.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>
#include <Components/StaticMeshComponent.h>

ASH_Seesaw::ASH_Seesaw()
{
	interationType = EObstacleType::Timelock;

	SetRootComponent(rootComp);

	AxisComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxisMesh"));
	AxisComp->SetupAttachment(RootComponent);
	AxisComp->SetRelativeLocation(FVector(0,0,-38));
	AxisComp->SetRelativeRotation(FRotator(0,0,90));
	AxisComp->SetRelativeScale3D(FVector(0.5,0.5,2.0));

	ConstructorHelpers::FObjectFinder <UStaticMesh> tempAxis(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if(tempAxis.Succeeded())
	{
		AxisComp->SetStaticMesh(tempAxis.Object);
	}

	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);
	InteractionMesh->SetRelativeScale3D(FVector(1.5, 4.0, 0.25));
	InteractionMesh->SetRelativeRotation(FRotator(0,90,0));
	InteractionMesh->SetSimulatePhysics(true);



	PhyComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhyComp->SetupAttachment(RootComponent);
	PhyComp->SetRelativeLocation(FVector(0,0,-45));
	PhyComp->ComponentName1.ComponentName = TEXT("interationMesh");
	PhyComp->ComponentName2.ComponentName = TEXT("AxisMesh");
	PhyComp->SetLinearXLimit(LCM_Locked, 0.0);
	PhyComp->SetLinearYLimit(LCM_Locked, 0.0);
	PhyComp->SetLinearZLimit(LCM_Locked, 0.0);
	PhyComp->SetAngularSwing1Limit(ACM_Locked, 45.0f);
	PhyComp->SetAngularSwing2Limit(ACM_Limited, 50.0f);
	PhyComp->SetAngularTwistLimit(ACM_Locked, 45.0F);
	PhyComp->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhyComp->SetAngularVelocityDriveTwistAndSwing(false, true);
	PhyComp->SetOrientationDriveTwistAndSwing(false, true);
	PhyComp->SetAngularDriveParams(20, 2, 0);

	

	
}


void ASH_Seesaw::BeginPlay()
{

}

void ASH_Seesaw::Tick(float DeltaTime)
{

}
