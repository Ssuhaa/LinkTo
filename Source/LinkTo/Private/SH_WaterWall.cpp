// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_WaterWall.h"

ASH_WaterWall::ASH_WaterWall()
{
	SetRootComponent(rootComp);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);
	InteractionMesh->SetRelativeScale3D(FVector(0.2, 7, 4));
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_WaterWall.MI_WaterWall'"));
	if (TempMat.Succeeded())
	{
		iceMatArray.Add(TempMat.Object);
		InteractionMesh->SetMaterial(0, TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_WaterWallIceMaker.MI_WaterWallIceMaker'"));
	if (TempMat1.Succeeded())
	{
		iceMatArray.Add(TempMat1.Object);
	}
}
