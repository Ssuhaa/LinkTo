// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Arrow.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "JS_Player.h"

AJS_Arrow::AJS_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	SetRootComponent(compCapsule);
	compCapsule->SetRelativeScale3D(FVector(0.5f));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	compMesh->SetupAttachment(compCapsule);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));


}

// Called when the game starts or when spawned
void AJS_Arrow::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Player::StaticClass()));
}

// Called every frame
void AJS_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}