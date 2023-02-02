// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Ball.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Ball : public AobstacleBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASH_Ball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BallMesh;

	void SetActiveBall(bool isActive);

	FVector OriginPos;

};
