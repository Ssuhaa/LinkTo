// Fill out your copyright notice in the Description page of Project Settings.


#include "IceMakerBase.h"


AIceMakerBase::AIceMakerBase()
{
	interationType = EObstacleType::Ice;
	SetRootComponent(rootComp);
}

void AIceMakerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AIceMakerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIceMakerBase::InteractionIceMaker(bool isOn)
{
	if (!bIceMaker)
	{
		if (isOn)
		{
			ChangeMaterial(iceMatArray, 1, InteractionMesh);
		}
		else
		{
			ChangeMaterial(iceMatArray, 0, InteractionMesh);
		}
		bLookin = false;
	}
}