// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_KillZone.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_KillZone : public AobstacleBase
{
	GENERATED_BODY()
	

	ASH_KillZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BoXBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* KillBox;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class ASH_BallFactory* BallFactory;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
