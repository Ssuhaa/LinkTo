// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_BallFactory.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_BallFactory : public AobstacleBase
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ASH_BallFactory();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Componont")
	class UStaticMeshComponent* Mesh; 

	UPROPERTY(EditAnywhere,  Category = "Componont")
	class USceneComponent* SpawnPos;

	UPROPERTY(EditAnywhere, Category = "Ball")
	TArray<class ASH_Ball*> ballArray;

	UPROPERTY(EditAnywhere, Category = "Ball")
	TSubclassOf<class ASH_Ball> ballFactory;


	float currentTime = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Timer")
	float SpawnDelayTime = 3;
};
