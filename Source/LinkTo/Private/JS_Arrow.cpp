// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Arrow.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "JS_Player.h"
#include "Camera/CameraComponent.h"
#include "AttackComponent.h"
#include <Components/SceneComponent.h>

AJS_Arrow::AJS_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(compRoot);

	compCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	compCapsule->SetupAttachment(compRoot);

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	compMesh->SetupAttachment(compCapsule);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	compProjectile->SetUpdatedComponent(compCapsule);
	compProjectile->MaxSpeed = 2000.f;
	compProjectile->InitialSpeed = 1.f;
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AJS_Arrow::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Player::StaticClass()));
// 	if (player != nullptr)
// 	{
// 		player->compAttack->accArrowSpeed = 0;
// 	}
	
}

// Called every frame
void AJS_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"),player->compAttack->accArrowSpeed));
	FVector p = GetActorLocation()+ GetActorForwardVector() * DeltaTime;
	SetActorLocation(p);



}

void AJS_Arrow::FireInDirection(const FVector& ShootDirection)
{
	compProjectile->Velocity = ShootDirection * compProjectile->InitialSpeed * player->compAttack->accArrowSpeed;

	/*player->compAttack->accArrowSpeed = 0;*/
}

// 
// void AJS_Arrow::OnDestroy()
// {
// 	SetActive(false);
// }
// 
// void AJS_Arrow::SetActive(bool value)
// {
// 	if (value)
// 	{
// 		compCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
// 		compMesh->SetVisibility(true);
// 	}
// 	else
// 	{
// 		compCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		compMesh->SetVisibility(false);
// 	}
// }
