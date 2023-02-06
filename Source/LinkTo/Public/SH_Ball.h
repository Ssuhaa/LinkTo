// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Ball.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Ball : public AobstacleBase
{
	GENERATED_BODY()

private:

	ASH_Ball();
	FVector OriginPos;
	// Sets default values for this actor's properties

	FVector CurrentPos;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void InteractionTimeLock(bool isOn) override;

public:


	void SetActiveBall(bool isActive);
	virtual void OnTimeLock() override;
	virtual void releasedTimeLock() override;

};
