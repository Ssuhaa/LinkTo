// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeLockBase.h"
#include "SH_MovingWalk.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_MovingWalk : public ATimeLockBase
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


	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UArrowComponent* dirComp;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class ASH_MWButton* ConButton;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class AActor* Target;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UMaterialParameterCollection* panParm;


	UPROPERTY(EditAnywhere, Category = Material)
	float force = 500;

	UPROPERTY(EditAnywhere, Category = MovingWalk)
	float MovingWalkSpeed = 0.1;
};
