// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_IceGhost.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_IceGhost : public AobstacleBase
{
	GENERATED_BODY()

	ASH_IceGhost();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class USkeletalMeshComponent* iceMesh;
	class UAnimationAsset* iceMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	TArray<UMaterialInstance*> iceMatArray;

	TArray<AActor*> IgnoreArray;
	FHitResult hitinfo;
	bool bActive = false;

public:
	void SetRotation(FVector HitNormal);
	void SetVisibleIce(bool visible);

	bool bSpawn = false;
};
