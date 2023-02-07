// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "TimeLockBase.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ATimeLockBase : public AobstacleBase
{
	GENERATED_BODY()

protected:
	ATimeLockBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float countTime = 0;
	bool bLookin = false;

	UPROPERTY(EditAnywhere, Category = "TimeLock|Material")
	TArray<UMaterialInstance*> TimeLockMatArray;
	UPROPERTY(VisibleAnywhere, Category = "TimeLock|Material")
	class UMaterialParameterCollection* timeLockParm;

public:
	virtual void InteractionTimeLock(bool isOn);
	virtual void LookInTimeLock();
	virtual void OnTimeLock();
	virtual void releasedTimeLock();

	UPROPERTY(VisibleAnywhere, Category = "TimeLock|Value")
	bool bTimeLock = false;
};
