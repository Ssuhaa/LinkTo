// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_IceGhost.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Animation/AnimationAsset.h>
#include <Materials/MaterialInstance.h>
#include <Kismet/GameplayStatics.h>
#include "SH_WaterWall.h"

ASH_IceGhost::ASH_IceGhost()
{
	SetRootComponent(rootComp);
	InteractionMesh->DestroyComponent();
	iceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	iceMesh->SetupAttachment(RootComponent);
	iceMesh->SetCollisionProfileName(TEXT("NoCollision"));
	iceMesh->SetRelativeScale3D(FVector(1,1,1.2));
	ConstructorHelpers::FObjectFinder <USkeletalMesh> tempMash(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/Mesh/CryonisIce.CryonisIce'"));
	if (tempMash.Succeeded())
	{
		iceMesh->SetSkeletalMesh(tempMash.Object);
	}

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	boxComp->SetupAttachment(iceMesh);
	boxComp->SetCollisionProfileName(TEXT("NoCollision"));
	boxComp->SetBoxExtent(FVector(65, 65, 105));
	boxComp->SetRelativeLocation(FVector(0, 0, 100));

	ConstructorHelpers::FObjectFinder <UAnimationAsset> tempanim(TEXT("/Script/Engine.AnimSequence'/Game/Geometry/Mesh/Ghostce_Anim.Ghostce_Anim'"));
	if (tempanim.Succeeded())
	{
		iceMotion = tempanim.Object;
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Spawn.MI_Spawn'"));
	if (TempMat.Succeeded())
	{
		iceMatArray.Add(TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_Nospawn.MI_Nospawn'"));
	if (TempMat1.Succeeded())
	{
		iceMatArray.Add(TempMat1.Object);
	}
}

void ASH_IceGhost::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASH_WaterWall::StaticClass(), IgnoreArray);
	
	SetVisibleIce(false);
}

void ASH_IceGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bActive)
	{
		bool bhit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), boxComp->GetComponentLocation(), boxComp->GetComponentLocation(), boxComp->GetScaledBoxExtent(), boxComp->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery15, false, IgnoreArray, EDrawDebugTrace::None, hitinfo, true);
		if (bhit)
		{
			if (!iceMatArray.IsEmpty())
			{
				
				iceMesh->SetMaterial(0, iceMatArray[1]);
				bSpawn = false;
			}
		}
		else
		{
			if (!iceMatArray.IsEmpty())
			{
				
					iceMesh->SetMaterial(0, iceMatArray[0]);
					bSpawn = true;
			}
		}
	
	}
}

void ASH_IceGhost::SetRotation(FVector HitNormal)
{
	SetActorRotation(UKismetMathLibrary::MakeRotFromZ(HitNormal));
}

void ASH_IceGhost::SetVisibleIce(bool visible)
{
	bActive = visible;
	iceMesh->SetVisibility(visible);
	if (visible)
	{
		iceMesh->PlayAnimation(iceMotion, true);
	}
	else
	{
		iceMesh->Stop();
	}
	
}

