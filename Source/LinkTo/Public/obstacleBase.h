// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "obstacleBase.generated.h"

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	None,
	Ice,
	Magnet,
	Boomb,
	Timelock,
};

UCLASS()
class LINKTO_API AobstacleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AobstacleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObstacleType interationType = EObstacleType::None;

	virtual bool isDelay(float DelayTime);
	float currentTime = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
