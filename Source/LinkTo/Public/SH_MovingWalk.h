// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_MovingWalk.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_MovingWalk : public AobstacleBase
{
	GENERATED_BODY()

	ASH_MovingWalk();
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

	UPROPERTY(EditAnywhere)
	class UArrowComponent* dirComp;

	UPROPERTY(EditAnywhere)
	class ASH_MWButton* ConButton;

	UPROPERTY(VisibleAnywhere)
	class AActor* Target;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* panParm;


	UPROPERTY(EditAnywhere)
	float force = 500;

	UPROPERTY(EditAnywhere)
	float MovingWalkSpeed = 0.1;
};
