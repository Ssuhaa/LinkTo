// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_HorizonMagnet.h"

ASH_HorizonMagnet::ASH_HorizonMagnet()
{
	SetRootComponent(InteractionMesh);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetRelativeScale3D(FVector(2.0f,2.0f,0.8f));
	SetRootComponent(InteractionMesh);

}

void ASH_HorizonMagnet::BeginPlay()
{
	Super::BeginPlay();
}

void ASH_HorizonMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
