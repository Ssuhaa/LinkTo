// Fill out your copyright notice in the Description page of Project Settings.

#include "MagnetBase.h"
#include "JS_SkillComponent.h"
#include "JS_Player.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include <Sound/SoundCue.h>
#include <Components/AudioComponent.h>

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
	ConstructorHelpers::FObjectFinder<USoundBase> tempsound(TEXT("/Script/Engine.SoundWave'/Game/Sound/SE_MagneCatch_Stop.SE_MagneCatch_Stop'"));
	if (tempsound.Succeeded())
	{
		MreleasedSound = tempsound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempsound1(TEXT("/Script/Engine.SoundCue'/Game/Sound/SC_MagnetMove.SC_MagnetMove'"));
	if (tempsound1.Succeeded())
	{
		MmovingSound = tempsound1.Object;
	}

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
	MovingSound = UGameplayStatics::SpawnSound2D(GetWorld(), MmovingSound);
	ChangeMaterial(MagnetMatarray, 3, InteractionMesh);
	InteractionMesh->SetSimulatePhysics(true);
	NSOnMagnet->SetWorldLocation(hitpos);
	NSOnMagnet->SetVisibility(true);
	NSOnMagnet->Activate(true);
	bMagnet = true;
}

void AMagnetBase::releasedMagnet()
{
	MovingSound->Stop();
	InteractionMesh->SetSimulatePhysics(false);
	if (player->compSkill->isPressedG)
	{
		ChangeMaterial(MagnetMatarray, 1, InteractionMesh);
	}
	else
	{
		ChangeMaterial(MagnetMatarray, 0, InteractionMesh);
	}
	bMagnet = false;
}
