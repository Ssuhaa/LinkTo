// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Push.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Push : public AobstacleBase
{
	GENERATED_BODY()

	ASH_Push();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void OnTimeLock() override;
	virtual void releasedTimeLock() override;


	UPROPERTY(EditAnywhere)
	class ASH_PushButton* ConButton;

	UPROPERTY(VisibleAnywhere)
	class ASH_Ball* Target;
	UPROPERTY(EditAnywhere)
	float pushZ = 50.0f;
	float Z = 0.0f;
	FVector OriginPos;

public:

	float ratioZ = 0.0f;


};
