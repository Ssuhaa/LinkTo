// Fill out your copyright notice in the Description page of Project Settings.


#include "obstacleBase.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "CharacterBase.h"
#include <Materials/MaterialParameterCollection.h>

// Sets default values
AobstacleBase::AobstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("interationMesh"));

	InteractionMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Ball.MI_Ball'"));
	if (TempMat.Succeeded())
	{
		InteractionMesh->SetMaterial(0, TempMat.Object);
		MatArray.Add(TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_BallTimeLockOn.MI_BallTimeLockOn'"));
	if (TempMat1.Succeeded())
	{
		MatArray.Add(TempMat1.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_BallTimeLockSelect.MI_BallTimeLockSelect'"));
	if (TempMat2.Succeeded())
	{
		MatArray.Add(TempMat2.Object);
	}

	ConstructorHelpers::FObjectFinder <UMaterialParameterCollection> TempParm(TEXT("/Script/Engine.MaterialParameterCollection'/Game/Geometry/Material/TimeLock.TimeLock'"));
	if (TempParm.Succeeded())
	{
		timeLockParm = TempParm.Object;
	}

}

// Called when the game starts or when spawned
void AobstacleBase::BeginPlay()
{
	Super::BeginPlay();
	player = Cast <ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	
}

// Called every frame
void AobstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTimeLock)
	{
		if (isDelay(10))
		{
			releasedTimeLock();
		}
		if (currentTime > 7)
		{
			if (!bcount)
			{
				UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), timeLockParm, TEXT("OnTimeLock"), CountEmissive);
				bcount = true;
			}
			else
			{
				UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), timeLockParm, TEXT("OnTimeLock"), defaultEmissive);
				bcount = false;
			}
		}

	}
}



bool AobstacleBase::isDelay(float DelayTime)
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > DelayTime)
	{
		currentTime = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void AobstacleBase:: InteractionTimeLock(bool isOn)
{

	if (isOn)
	{

		InteractionMesh->SetMaterial(0, MatArray[1]);

	}
	else
	{

		InteractionMesh->SetMaterial(0, MatArray[0]);

	}
	if (bTimeLock)
	{
		InteractionMesh->SetMaterial(0, MatArray[2]);
	}
	
}

void AobstacleBase::LookInTimeLock()
{
	InteractionMesh->SetMaterial(0, MatArray[2]);
}

void AobstacleBase::LookOutTimeLock()
{
	InteractionMesh->SetMaterial(0, MatArray[1]);
}

void AobstacleBase::OnTimeLock()
{
	if (!FindOnTimeLockActor())
	{
		InteractionMesh->SetMaterial(0, MatArray[2]);
		bTimeLock = true;
	}

}

void AobstacleBase::releasedTimeLock()
{
	InteractionMesh->SetMaterial(0, MatArray[0]);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), timeLockParm, TEXT("OnTimeLock"), defaultEmissive);
	bTimeLock = false;
}

bool AobstacleBase::FindOnTimeLockActor()
{
	for (int32 i = 0; i < player->timelockActorarr.Num(); i++)
	{
		if(player->timelockActorarr[i]->bTimeLock) return true;
	}
	return false;
}

