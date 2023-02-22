// Fill out your copyright notice in the Description page of Project Settings.

#include "MagnetBase.h"
#include "JS_SkillComponent.h"
#include "JS_Player.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>

AMagnetBase::AMagnetBase()
{
	interationType = EObstacleType::Magnet;
	SetRootComponent(InteractionMesh);
	rootComp->DestroyComponent();
	InteractionMesh->SetSimulatePhysics(false);

	NSOnMagnet = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	NSOnMagnet->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UNiagaraSystem> tempNS(TEXT("/Script/Niagara.NiagaraSystem'/Game/FX/NS_OnMagnet.NS_OnMagnet'"));
	if (tempNS.Succeeded())
	{
		NSOnMagnet->SetAsset(tempNS.Object);
	}
	NSOnMagnet->bAutoActivate = false;
	

}

void AMagnetBase::BeginPlay()
{
	Super::BeginPlay();
	NSOnMagnet->SetVisibility(false);
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
			InteractionMesh->SetSimulatePhysics(false);
		}

	}
}

void AMagnetBase::LookInMagnet()
{
	if (!bMagnet)
	{
		ChangeMaterial(MagnetMatarray, 2, InteractionMesh);
	}
}

void AMagnetBase::OnMagnet(FVector hitpos)
{
	ChangeMaterial(MagnetMatarray, 3, InteractionMesh);
	InteractionMesh->SetSimulatePhysics(true);
	NSOnMagnet->SetWorldLocation(hitpos);
	NSOnMagnet->SetVisibility(true);
	NSOnMagnet->Activate(true);
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
		InteractionMesh->SetSimulatePhysics(false);
	}
	bMagnet = false;
}
