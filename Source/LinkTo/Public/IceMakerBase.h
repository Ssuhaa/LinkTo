// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "IceMakerBase.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API AIceMakerBase : public AobstacleBase
{
	GENERATED_BODY()
protected:
	AIceMakerBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "IceMaker|Material")
	TArray<UMaterialInstance*> iceMatArray;

	bool bLookin = false;
	bool bIceMaker = false;
	FVector ImpactPoint;

public:
	void Spawnice();
	void SaveimpactPoint(FVector impactPoint);
	void InteractionIceMaker(bool isOn);
};
