// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Wind.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "SH_Player.h"

ASH_Wind::ASH_Wind()
{
	SetRootComponent(rootComp);
	InteractionMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("//Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	dirComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Dir"));
	dirComp->SetupAttachment(RootComponent);
	dirComp->SetRelativeRotation(FRotator(90,0,0));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(InteractionMesh);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 60));
	BoxCollision->SetBoxExtent(FVector(48, 48, 10));
}

void ASH_Wind::BeginPlay()
{
	Super::BeginPlay();
}

void ASH_Wind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		if (Target->bParasailing)
		{
			Target->WindUp();

		}
	}
}

void ASH_Wind::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Target = Cast<ASH_Player>(OtherActor);
}

void ASH_Wind::NotifyActorEndOverlap(AActor* OtherActor)
{
	Target = nullptr;
}
