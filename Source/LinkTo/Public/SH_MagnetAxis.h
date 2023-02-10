// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_MagnetAxis.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_MagnetAxis : public AobstacleBase
{
	GENERATED_BODY()
	
	ASH_MagnetAxis();
	UPROPERTY(EditDefaultsOnly)
	class UPhysicsConstraintComponent* PhyComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
