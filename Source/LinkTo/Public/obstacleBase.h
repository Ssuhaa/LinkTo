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
	
	// Sets default values for this actor's properties

protected:
	AobstacleBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool isDelay(float DelayTime);
	virtual void ChangeMaterial(TArray<UMaterialInstance*> MatArray, int32 Arrayindex, UStaticMeshComponent* Mesh);
	float currentTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class USceneComponent* rootComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UStaticMeshComponent* InteractionMesh;

	class ASH_Player* player;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EObstacleType interationType = EObstacleType::None;

};
