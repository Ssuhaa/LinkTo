// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_MWButton.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_MWButton : public AobstacleBase
{
	GENERATED_BODY()

	ASH_MWButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void OnTimeLock() override;
	virtual void releasedTimeLock() override;


	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	FVector OriginPos;
	float pressZ = -30.0f;
	float ratioZ = 0;
	float Z;

public:

	bool binButton = false;

};
