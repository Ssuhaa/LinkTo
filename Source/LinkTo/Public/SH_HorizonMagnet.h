// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagnetBase.h"
#include "SH_HorizonMagnet.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_HorizonMagnet : public AMagnetBase
{

	GENERATED_BODY()
	ASH_HorizonMagnet();

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* AxisComp;
	UPROPERTY(EditDefaultsOnly)
	class UPhysicsConstraintComponent* PhyComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
