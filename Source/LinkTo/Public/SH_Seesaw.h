// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Seesaw.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Seesaw : public AobstacleBase
{
	GENERATED_BODY()

	ASH_Seesaw();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhyComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* AxisComp;



};
