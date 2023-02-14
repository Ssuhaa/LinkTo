// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Sword.h"
#include <Components/CapsuleComponent.h>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AJS_Sword::AJS_Sword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compAxis = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SWORD"));
	compMesh->SetupAttachment(compAxis);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compMesh->SetRelativeLocation(FVector(0,0,-32.f));
	
	compCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	compCapsule->SetupAttachment(compMesh);
	compCapsule->SetCollisionObjectType(ECC_GameTraceChannel1);
	compCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	compCapsule->SetRelativeLocation(FVector(0,0,-9.f));
	compCapsule->SetCapsuleHalfHeight(30.f);
	compCapsule->SetCapsuleRadius(5.f);
}

// Called when the game starts or when spawned
void AJS_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

