// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_OutMap.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_OutMap : public AobstacleBase
{
	GENERATED_BODY()
protected:
	ASH_OutMap();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = component)
	class UBoxComponent* boxComp;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUI_OutGame> wgfactory;
	UPROPERTY(EditAnywhere)
	class UUI_OutGame* WGOutGame;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
