// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_BallFactory.h"
#include "SH_Ball.h"

ASH_BallFactory::ASH_BallFactory()
{
	PrimaryActorTick.bCanEverTick = true;
	
	interationType = EObstacleType::Timelock;

	ConstructorHelpers::FClassFinder <ASH_Ball> tempBall(TEXT("/Script/CoreUObject.Class'/Script/LinkTo.SH_Ball'"));
	if (tempBall.Succeeded())
	{
		ballFactory = tempBall.Class;
	}
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	ConstructorHelpers::FObjectFinder <UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (TempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(TempMesh.Object);
	}

	SpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPos"));
	SpawnPos->SetupAttachment(Mesh);
	SpawnPos->SetRelativeLocation(FVector(0,0,-50));


}

void ASH_BallFactory:: BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < 3; i++)
	{
		ASH_Ball* CurrentBall = GetWorld()->SpawnActor<ASH_Ball>(ballFactory, SpawnPos->GetComponentTransform());
		CurrentBall->SetActiveBall(false);
		ballArray.Add(CurrentBall);
	}

	currentTime = SpawnDelayTime;
}

void ASH_BallFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ballArray.IsValidIndex(2))
	{
		if (ballArray.IsValidIndex(0))
		{
			ballArray[0]->SetActiveBall(true);
			ballArray.RemoveAt(0);

		}
	}

}
