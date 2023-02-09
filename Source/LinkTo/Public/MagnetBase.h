// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "MagnetBase.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API AMagnetBase : public AobstacleBase
{
	GENERATED_BODY()

protected:
	AMagnetBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<class UMaterialInstance*> MagnetMatarray;

public:
	virtual void InteractionMagnet(bool isOn);
	virtual void LookInMagnet();
	virtual void OnMagnet();
	virtual void releasedMagnet();

	bool bMagnet = false;
	bool bLookin = false;


};
