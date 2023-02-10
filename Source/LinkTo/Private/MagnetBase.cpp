// Fill out your copyright notice in the Description page of Project Settings.

#include "MagnetBase.h"
#include "JS_SkillComponent.h"
#include "JS_Player.h"

AMagnetBase::AMagnetBase()
{
	interationType = EObstacleType::Magnet;
	SetRootComponent(rootComp);
	InteractionMesh->SetupAttachment(RootComponent);
}

void AMagnetBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMagnetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagnetBase::InteractionMagnet(bool isOn)
{
	if (!bMagnet)
	{
		if (isOn)
		{
			ChangeMaterial(MagnetMatarray, 1, InteractionMesh);
		}
		else
		{
			ChangeMaterial(MagnetMatarray, 0, InteractionMesh);
		}

	}
}

void AMagnetBase::LookInMagnet()
{
	if (!bMagnet)
	{
		ChangeMaterial(MagnetMatarray, 2, InteractionMesh);
		InteractionMesh->SetSimulatePhysics(true);

	}

}

void AMagnetBase::OnMagnet()
{
	ChangeMaterial(MagnetMatarray, 3, InteractionMesh);
	bMagnet = true;
}

void AMagnetBase::releasedMagnet()
{
	if (player->compSkill->isPressedG)
	{
		ChangeMaterial(MagnetMatarray, 1, InteractionMesh);
	}
	else
	{
		ChangeMaterial(MagnetMatarray, 0, InteractionMesh);
	}

	InteractionMesh->SetSimulatePhysics(false);
	bMagnet = false;
}
