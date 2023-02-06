// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "obstacleBase.generated.h"

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	None,
	Ice,
	Magnet,
	Boomb,
	Timelock,
};

UCLASS()
class LINKTO_API AobstacleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AobstacleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool isDelay(float DelayTime);
	float currentTime = 0;

	bool bTimeLock = false;
	bool bcount = false;

	float defaultEmissive = 0.5f;
	float CountEmissive = 1.0f;

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;

public:	

	UPROPERTY(EditAnywhere, Category = Material)
	TArray<UMaterialInstance*> MatArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* InteractionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObstacleType interationType = EObstacleType::None;

	virtual void InteractionTimeLock(bool isOn);
	virtual void LookInTimeLock();
	virtual void LookOutTimeLock();
	virtual void OnTimeLock();
	virtual void releasedTimeLock();

	bool FindOnTimeLockActor();

private:
	
	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection*  timeLockParm;

};
