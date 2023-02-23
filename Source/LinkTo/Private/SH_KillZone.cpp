// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_KillZone.h"
#include <Components/BoxComponent.h>
#include "SH_Ball.h"
#include <Kismet/GameplayStatics.h>
#include "SH_BallFactory.h"
#include "JS_Player.h"

ASH_KillZone::ASH_KillZone()
{
	SetRootComponent(rootComp);
	InteractionMesh->DestroyComponent();
	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KillBox"));
	KillBox->SetupAttachment(RootComponent);
	KillBox->SetCollisionProfileName(TEXT("OverlapAll"));
	KillBox->SetBoxExtent(FVector(1000,1000,200));
}

void ASH_KillZone::BeginPlay()
{
	Super::BeginPlay();
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ASH_KillZone::BoXBeginOverlap);
	BallFactory = Cast<ASH_BallFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_BallFactory::StaticClass()));
	
}
void ASH_KillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void  ASH_KillZone::BoXBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASH_Ball* CurBall = Cast<ASH_Ball>(OtherActor);
	if (CurBall != nullptr)
	{
		CurBall->SetActiveBall(false);
		BallFactory->ballArray.Add(CurBall);
	}

	//플레이어 킬
	AJS_Player* Player = Cast<AJS_Player>(OtherActor);
	if(Player != nullptr)
	{
		Player->ContactKillZone();
	}
}