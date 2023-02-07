// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeLockBase.h"
#include "SH_Seesaw.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Seesaw : public ATimeLockBase
{
	GENERATED_BODY()

	ASH_Seesaw();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnTimeLock()override;
	virtual void releasedTimeLock()override;
	virtual void InteractionTimeLock(bool isOn)override;
	virtual void LookInTimeLock()override;


	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UPhysicsConstraintComponent* PhyComp;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UStaticMeshComponent* AxisComp;



};
