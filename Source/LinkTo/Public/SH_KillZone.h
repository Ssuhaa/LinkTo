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

	UPROPERTY(EditAnyWhere)
	class UBoxComponent* KillBox;

	UFUNCTION()
	void BoXBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	class ASH_BallFactory* BallFactory;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
