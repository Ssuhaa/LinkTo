// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Bomb.h"
#include <Components/SphereComponent.h>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "JS_Player.h"
#include <Kismet/GameplayStatics.h>
#include <PhysicsEngine/RadialForceComponent.h>
#include "JS_SkillComponent.h"

// Sets default values
AJS_Bomb::AJS_Bomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("COMP SPHERE"));
	SetRootComponent(compSphere);
	compSphere->SetSphereRadius(20);

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("COMP MESH"));
	compMesh->SetupAttachment(compSphere);
	compMesh->SetSimulatePhysics(false);

	compRadial = CreateDefaultSubobject<URadialForceComponent>(TEXT("COMP RADIAL"));
	compRadial->SetupAttachment(compSphere);
	compRadial->Radius = 1000.0f;
	compRadial->ForceStrength = 500.0f;


}

// Called when the game starts or when spawned
void AJS_Bomb::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Player::StaticClass()));

// 	FTimerHandle exploWaitTime;
// 	GetWorld()->GetTimerManager().SetTimer(exploWaitTime,this,&AJS_Bomb::Explose,3.0f,false);
}

// Called every frame
void AJS_Bomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Bomb::SetActiveBomb(bool isActive)
{
	if (isActive)
	{
		compMesh->SetVisibility(true);
		compSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//ratioX = 1;
	}
	else
	{
		compMesh->SetVisibility(false);
		compSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ratioX = 0 ;
	}
}

void AJS_Bomb::Explose()
{
		compRadial->FireImpulse();
		SetActiveBomb(false);
		player->compSkill->canExplo = false;
		UE_LOG(LogTemp,Error,TEXT("BOOM"))
}

