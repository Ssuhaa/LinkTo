// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ice.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/BoxComponent.h>


ASH_Ice::ASH_Ice()
{
	SetRootComponent(rootComp);
	InteractionMesh->DestroyComponent();
	
	iceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	iceMesh->SetupAttachment(RootComponent);
	iceMesh->SetCollisionProfileName(TEXT("NoCollision"));
	iceMesh->SetRelativeScale3D(FVector(1,1,1.2));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComp->SetupAttachment(iceMesh);
	BoxComp->SetCollisionProfileName(TEXT("BlockAll"));
	BoxComp->SetBoxExtent(FVector(60, 60, 100));
	BoxComp->SetRelativeLocation(FVector(0, 0, 100));

	ConstructorHelpers::FObjectFinder <USkeletalMesh> tempMash(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/Mesh/CryonisIce.CryonisIce'"));
	if (tempMash.Succeeded())
	{
		iceMesh->SetSkeletalMesh(tempMash.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_ice.MI_ice'"));
	if(TempMat.Succeeded())
	{
		iceMesh->SetMaterial(0, TempMat.Object);
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


}

void ASH_Ice::SetActiveIce(bool isActive)
{
	if (isActive)
	{
		iceMesh->SetVisibility(isActive);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		iceMesh->PlayAnimation(iceMotion, false);
	}
	else
	{
		iceMesh->SetVisibility(isActive);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		iceMesh->Stop();
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
