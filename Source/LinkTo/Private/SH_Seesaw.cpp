// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Seesaw.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>

ASH_Seesaw::ASH_Seesaw()
{
	SetRootComponent(rootComp);

	WGcomp->SetupAttachment(RootComponent);
	AxisComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxisMesh"));
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempAxis(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if(tempAxis.Succeeded())
	{
		AxisComp->SetStaticMesh(tempAxis.Object);
	}
	AxisComp->SetupAttachment(RootComponent);
	AxisComp->SetRelativeLocation(FVector(0,0,-38));
	AxisComp->SetRelativeRotation(FRotator(0,0,90));
	AxisComp->SetRelativeScale3D(FVector(0.5,0.5,2.8));

	InteractionMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetRelativeScale3D(FVector(3.0, 6.0, 0.3));
	InteractionMesh->SetRelativeRotation(FRotator(0,90,0));
	InteractionMesh->SetSimulatePhysics(true);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetRelativeLocation(FVector(-100,0,-45));


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

	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Seesew.MI_Seesew'"));
	if (TempMat.Succeeded())
	{
		TimeLockMatArray.Add(TempMat.Object);
		InteractionMesh->SetMaterial(0, TempMat.Object);
		AxisComp->SetMaterial(0, TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_SeeSewTimeLockOn.MI_SeeSewTimeLockOn'"));
	if (TempMat1.Succeeded())
	{
		TimeLockMatArray.Add(TempMat1.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_SeesewTimeLockSelect.MI_SeesewTimeLockSelect'"));
	if (TempMat2.Succeeded())
	{
		TimeLockMatArray.Add(TempMat2.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_SeesewlTimeLockCount.MI_SeesewlTimeLockCount'"));
	if (TempMat3.Succeeded())
	{
		TimeLockMatArray.Add(TempMat3.Object);
	}
}


void ASH_Seesaw::BeginPlay()
{
	Super::BeginPlay();
}

void ASH_Seesaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASH_Seesaw::InteractionTimeLock(bool isOn)
{
	Super::InteractionTimeLock(isOn);
	if (!bTimeLock)
	{
		if (isOn)
		{
			ChangeMaterial(TimeLockMatArray, 1, AxisComp);
		}
		else
		{
			ChangeMaterial(TimeLockMatArray, 0, AxisComp);
		}
	}
}

void ASH_Seesaw::LookInTimeLock()
{
	Super::LookInTimeLock();

	ChangeMaterial(TimeLockMatArray, 2, AxisComp);
}

void ASH_Seesaw::OnTimeLock()
{
	Super::OnTimeLock();
	if (bLookin)
	{
		ChangeMaterial(TimeLockMatArray, 2, AxisComp);
		InteractionMesh->SetSimulatePhysics(false);
	}
}

void ASH_Seesaw::releasedTimeLock()
{
	Super::releasedTimeLock();
	ChangeMaterial(TimeLockMatArray, 0, AxisComp);
	InteractionMesh->SetSimulatePhysics(true);
}

