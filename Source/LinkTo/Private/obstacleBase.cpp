// Fill out your copyright notice in the Description page of Project Settings.


#include "obstacleBase.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "CharacterBase.h"
#include "JS_Player.h"


// Sets default values
AobstacleBase::AobstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("interationMesh"));
	InteractionMesh->SetCollisionProfileName(TEXT("BlockAll"));

}

// Called when the game starts or when spawned
void AobstacleBase::BeginPlay()
{
	Super::BeginPlay();
	player = Cast <AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	
}

// Called every frame
void AobstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AobstacleBase::isDelay(float DelayTime)
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > DelayTime)
	{
		currentTime = 0;
		return true;
	}
	else
	{
		return false;
	}
}


void AobstacleBase::ChangeMaterial(TArray<UMaterialInstance*> MatArray, int32 Arrayindex, UStaticMeshComponent* Mesh)
{
	if (MatArray.IsValidIndex(Arrayindex))
	{

		Mesh->SetMaterial(0, MatArray[Arrayindex]);
	
	}
}

