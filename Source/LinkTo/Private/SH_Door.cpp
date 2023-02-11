// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Door.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Goal.h"

ASH_Door::ASH_Door()
{
	SetRootComponent(rootComp);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);
	InteractionMesh->SetRelativeScale3D(FVector(4.0f, 0.2f, 2.5f));
}

void ASH_Door::BeginPlay()
{
	Super::BeginPlay();
	OriginPos = GetActorLocation();
	goal = Cast<ASH_Goal>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Goal::StaticClass()));
	
}

void ASH_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpenDoor(DeltaTime);
}

void ASH_Door::OpenDoor(float DeltaTime)
{

	if (goal->bisGoal)
	{
		ratioZ += (DeltaTime * 10);
		if (ratioZ > 1)
		{
			ratioZ = 1;
		}
		Z = FMath::Lerp(OriginPos.Z, pushZ, ratioZ);
		SetActorLocation(FVector(OriginPos.X, OriginPos.Y, Z));
	}
	else
	{
		ratioZ -= (DeltaTime * 10);
		if (ratioZ < 0)
		{
			ratioZ = 0;
		}
		Z = FMath::Lerp(OriginPos.Z, pushZ, ratioZ);
		SetActorLocation(FVector(OriginPos.X, OriginPos.Y, Z));
	}

}
