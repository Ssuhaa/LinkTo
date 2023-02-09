// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ice.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/SkeletalMeshComponent.h>

ASH_Ice::ASH_Ice()
{
	SetRootComponent(rootComp);
	InteractionMesh->DestroyComponent();
	
	iceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	iceMesh->SetupAttachment(RootComponent);
	iceMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ConstructorHelpers::FObjectFinder <USkeletalMesh> tempMash(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/Mesh/CryonisIce.CryonisIce'"));
	if (tempMash.Succeeded())
	{
		iceMesh->SetSkeletalMesh(tempMash.Object);
	}
	ConstructorHelpers::FObjectFinder <UAnimationAsset> tempanim(TEXT("/Script/Engine.AnimSequence'/Game/Geometry/Mesh/CryonisIce_Anim.CryonisIce_Anim'"));
	if(tempanim.Succeeded())
	{
		iceMotion = tempanim.Object;
	}
}

void ASH_Ice::BeginPlay()
{
	Super::BeginPlay();

}

void ASH_Ice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetIceSpawnPos(DeltaTime);

}

void ASH_Ice::SetActiveIce(bool isActive)
{
	
	//bIsActiveIce = isActive;
	
	//SpawnPos = GetActorLocation();
	if (isActive)
	{
		iceMesh->SetVisibility(isActive);
		iceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		iceMesh->PlayAnimation(iceMotion, false);
	
		//ratioX = 1;
	}
	else
	{
		iceMesh->SetVisibility(isActive);
		iceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ratioX = 0 ;
	}


}

bool ASH_Ice::isIceVisible()
{
	return iceMesh->IsVisible();
}

void ASH_Ice::SetRotation(FVector HitNormal)
{
	SetActorRotation(UKismetMathLibrary::MakeRotFromZ(HitNormal));
}

void ASH_Ice::SetIceSpawnPos(float DeltaTime)
{
	if (!bIsActiveIce)
	{
		ratioX += (DeltaTime * 10);
		if (ratioX > 1)
		{
			ratioX = 1;
			InteractionMesh->SetVisibility(bIsActiveIce);
		}
		X = FMath::Lerp(SpawnPos.X- pushX,SpawnPos.X, ratioX);
		SetActorLocation(FVector(X, SpawnPos.Y, SpawnPos.Z));
		
	}
	else
	{
		ratioX -= (DeltaTime * 10);
		if (ratioX < 0)
		{
			ratioX = 0;
		}
		else if (ratioX > 0)
		{
			InteractionMesh->SetVisibility(bIsActiveIce);
		}
		X = FMath::Lerp(SpawnPos.X- pushX, SpawnPos.X, ratioX);
		SetActorLocation(FVector(X, SpawnPos.Y, SpawnPos.Z));
	}

}