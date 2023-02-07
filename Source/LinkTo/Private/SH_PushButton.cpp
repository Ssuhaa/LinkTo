// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_PushButton.h"
#include <Components/BoxComponent.h>
#include "SH_Player.h"
#include <Math/UnrealMathUtility.h>
#include "SH_Push.h"
#include <Kismet/GameplayStatics.h>

ASH_PushButton::ASH_PushButton()
{
	SetRootComponent(rootComp);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
	InteractionMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(InteractionMesh);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 60));
	BoxCollision->SetBoxExtent(FVector(48, 48, 10));

	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Button.MI_Button'"));
	if (TempMat.Succeeded())
	{
		TimeLockMatArray.Add(TempMat.Object);
		InteractionMesh->SetMaterial(0,TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_ButtonTimeLockOn.MI_ButtonTimeLockOn'"));
	if (TempMat1.Succeeded())
	{
		TimeLockMatArray.Add(TempMat1.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_ButtonTimeLockSelect.MI_ButtonTimeLockSelect'"));
	if (TempMat2.Succeeded())
	{
		TimeLockMatArray.Add(TempMat2.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_ButtonTimeLockCount.MI_ButtonTimeLockCount'"));
	if (TempMat3.Succeeded())
	{
		TimeLockMatArray.Add(TempMat3.Object);
	}


}

void ASH_PushButton::BeginPlay()
{
	Super::BeginPlay();
	OriginPos = InteractionMesh->GetRelativeLocation();

	
}

void ASH_PushButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PressedButton(DeltaTime);
}

void ASH_PushButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ASH_Player* Player = Cast<ASH_Player>(OtherActor);
	if (Player != nullptr)
	{
		if (!bTimeLock)
		{
			binButton = true;
			Z = currZ;
		}
		else
		{
			bTimeLockInButton = true;
		}
	}
}

void ASH_PushButton::NotifyActorEndOverlap(AActor* OtherActor)
{
	ASH_Player* Player = Cast<ASH_Player>(OtherActor);
	if (Player != nullptr)
	{
		if (!bTimeLock)
		{
			binButton = false;
			Z = currZ;
		}
		else
		{
			bTimeLockInButton = false;
		}
	}
}

void ASH_PushButton::OnTimeLock()
{
	Super::OnTimeLock();
}

void ASH_PushButton::releasedTimeLock()
{
	Super::releasedTimeLock();
	bTimeLock = false;
	binButton = bTimeLockInButton;
	Z = currZ;
}

void ASH_PushButton::PressedButton(float DeltaTime)
{
	if (!bTimeLock)
	{
		if (binButton)
		{
			ratioZ += (DeltaTime * 2);
			if (ratioZ > 1)
			{
				ratioZ = 1;
			}
			Z = FMath::Lerp(OriginPos.Z, pressZ, ratioZ);
			InteractionMesh->SetRelativeLocation(FVector(OriginPos.X, OriginPos.Y, Z));
		}
		else if (!binButton)
		{
			ratioZ -= (DeltaTime * 2);
			if (ratioZ < 0)
			{
				ratioZ = 0;
			}
			Z = FMath::Lerp(OriginPos.Z, pressZ, ratioZ);
			InteractionMesh->SetRelativeLocation(FVector(OriginPos.X, OriginPos.Y, Z));
		}
		currZ = Z;
	}


}
