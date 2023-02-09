// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ArrowFactory.h"
#include "JS_Arrow.h"
#include "JS_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AJS_ArrowFactory::AJS_ArrowFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	ConstructorHelpers::FClassFinder<AJS_Arrow>tempArrow(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Weapons/BP_Arrow.BP_Arrow_c'"));
// 	if (tempArrow.Succeeded())
// 	{
// 		arrowFactory = tempArrow.Class;
// 	}
}

// Called when the game starts or when spawned
void AJS_ArrowFactory::BeginPlay()
{
	Super::BeginPlay();
// 
// 	// player Cast
// 	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Player::StaticClass()));
// 	arrow = Cast<AJS_Arrow>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Arrow::StaticClass()));
// 
// 	camLoc = player->compCam->GetComponentLocation();
// 	// 오브젝트 생성
// 	if (player != nullptr)
// 	{
// 		for (int32 i = 0; i < 3; i++)
// 		{
// 			// 오브젝트 스폰
// 			arrow = GetWorld()->SpawnActor<AJS_Arrow>(arrowFactory, camLoc, FRotator(0));
// 			// 비활성화
// 			arrow->SetActive(false);
// 			// 배열에 추가
// 			arrows.Add(arrow);
// 		}
// 	}
// 	
}

// // Called every frame
 void AJS_ArrowFactory::Tick(float DeltaTime)
 {
 	Super::Tick(DeltaTime);
// 
// 	
// 
// }
// 
// void AJS_ArrowFactory::MakeArrow(FVector pos,FRotator rot)
// {
// // 	arrows[0]->SetActorLocation(pos);
// // 	arrows[0]->SetActorRotation(rot);
// // 	arrows[0]->SetActive(true);
// // 	arrows.RemoveAt(0);
// }
// // 
// // void AJS_ArrowFactory::AddArrow()
// // {
// // 	arrows.Add(arrow);
}

