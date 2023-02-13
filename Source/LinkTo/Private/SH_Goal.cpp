// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Goal.h"
#include <Components/BoxComponent.h>
#include "SH_Ball.h"
#include <Kismet/GameplayStatics.h>

ASH_Goal::ASH_Goal()
{
	SetRootComponent(rootComp);
	InteractionMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Geometry/Mesh/SM_Goal.SM_Goal'"));
	if (tempMesh.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMesh.Object);
	}
	InteractionMesh->SetRelativeScale3D(FVector(1.5f));

	BoxComp = CreateDefaultSubobject<UBoxComponent> (TEXT("BoxCollision"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(50));
	BoxComp->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxComp->SetRelativeLocation(FVector(0,0,-132));
}

void ASH_Goal::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASH_Goal::CheckGoal);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ASH_Goal::OutGoal);

	//ball = Cast<ASH_Ball>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Ball::StaticClass()));
}

void ASH_Goal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	if (currball != nullptr)
// 	{
// 		if (currball->InteractionMesh->IsSimulatingPhysics())
// 		{
// 			bisGoal = false;
// 		}
// 	}
}

void ASH_Goal::CheckGoal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	currball = Cast<ASH_Ball>(OtherActor);
	if (currball != nullptr)
	{
		//currball->Goal(false);
		bisGoal = true;
	}
}


void ASH_Goal::OutGoal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	currball = Cast<ASH_Ball>(OtherActor);
	if (currball != nullptr)
	{
		//currball->Goal(true);
		bisGoal =false;
	}
}
