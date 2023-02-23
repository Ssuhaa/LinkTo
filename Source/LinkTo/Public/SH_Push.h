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

	void PushActor(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class ASH_PushButton* ConButton;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class ASH_Ball* Target;
	UPROPERTY(EditAnywhere, Category = Value)
	float pushZ = 80.0f;

	float Z = 0.0f;
	float ratioZ = 0.0f;
	FVector OriginPos;


};
