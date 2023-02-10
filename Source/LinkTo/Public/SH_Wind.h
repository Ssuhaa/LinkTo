// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Wind.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Wind : public AobstacleBase
{
	GENERATED_BODY()

	ASH_Wind();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UArrowComponent* dirComp;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, Category = Component)
	class ASH_Player* Target;

	UPROPERTY(EditAnywhere, Category = Wind)
	float WindValue = 30.0f;

	bool binWind = false;
};
