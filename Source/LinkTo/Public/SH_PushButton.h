// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeLockBase.h"
#include "SH_PushButton.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_PushButton : public ATimeLockBase
{
	GENERATED_BODY()

	ASH_PushButton();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void OnTimeLock() override;
	virtual void releasedTimeLock() override;

	void PressedButton(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UBoxComponent* BoxCollision;

	FVector OriginPos;
	float currZ = 0;
	UPROPERTY(EditAnywhere, Category = Value)
	float pressZ = -30.0f;

public:

	float Z;
	float ratioZ = 0;
	bool binButton = false;
	bool bTimeLockInButton = false;
	
};
