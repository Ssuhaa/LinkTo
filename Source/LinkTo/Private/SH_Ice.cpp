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
		iceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		iceMesh->PlayAnimation(iceMotion, false);
	}
	else
	{
		iceMesh->SetVisibility(isActive);
		iceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
