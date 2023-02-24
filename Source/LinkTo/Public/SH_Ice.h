// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Ice.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Ice : public AobstacleBase
{
	GENERATED_BODY()
	
public:
	ASH_Ice();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class USkeletalMeshComponent* iceMesh; 
	class UAnimationAsset* iceMotion;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* BoxComp;

	bool bIsActiveIce = false;
	float ratioX = 0.0f;
	float X = 0.0f;
	float pushX = 50.0f;
	FVector SpawnPos;

public:
	void SetActiveIce(bool isActive);
	bool isIceVisible();
	void SetRotation(FVector HitNormal);

};
