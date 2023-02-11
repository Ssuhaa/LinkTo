// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Goal.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Goal : public AobstacleBase
{
	GENERATED_BODY()

	ASH_Goal();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UBoxComponent* BoxComp;

	class ASH_Ball* ball;

	UFUNCTION()
	void CheckGoal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OutGoal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	UPROPERTY(VisibleAnywhere)
	bool bisGoal = false;
};
