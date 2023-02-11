// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Door.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Door : public AobstacleBase
{
	GENERATED_BODY()
	
	ASH_Door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ASH_Goal* goal;

	FVector OriginPos;

	void OpenDoor(float DeltaTime);

	float ratioZ = 0;
	UPROPERTY(EditAnywhere)
	float pushZ = 500.0f;
	float Z = 0;
};
