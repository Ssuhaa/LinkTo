// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Push.h"
#include "SH_PushButton.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Ball.h"

ASH_Push::ASH_Push()
{
	SetRootComponent(rootComp);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);
}

void ASH_Push::BeginPlay()
{
	Super::BeginPlay();

	ConButton = Cast<ASH_PushButton>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_PushButton::StaticClass()));
	OriginPos = InteractionMesh->GetRelativeLocation();
}

void ASH_Push::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PushActor(DeltaTime);
}

void ASH_Push::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Target = Cast<ASH_Ball>(OtherActor);

}

void ASH_Push::NotifyActorEndOverlap(AActor* OtherActor)
{
	Target = nullptr;
}

void ASH_Push::PushActor(float DeltaTime)
{
	if (!ConButton->bTimeLock)
	{
		if (ConButton->binButton)
		{
			ratioZ += (DeltaTime * 10);
			if (ratioZ > 1)
			{
				ratioZ = 1;
			}
			Z = FMath::Lerp(OriginPos.Z, pushZ, ratioZ);
			InteractionMesh->SetRelativeLocation(FVector(OriginPos.X, OriginPos.Y, Z));
		}
		else if (!ConButton->binButton)
		{
			ratioZ -= (DeltaTime * 10);
			if (ratioZ < 0)
			{
				ratioZ = 0;
			}
			Z = FMath::Lerp(OriginPos.Z, pushZ, ratioZ);
			InteractionMesh->SetRelativeLocation(FVector(OriginPos.X, OriginPos.Y, Z));
		}
	}
}

