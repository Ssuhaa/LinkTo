// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKTO_API UPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
public:
	class AJS_Player* player;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float HP = 10; // 1당 하트 1칸
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float stamina = 100; // 스테미나
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	bool bUseStamina = false;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float currTime = 0;


	void StaminaStatus(bool value, float deltaTime);
	void ResetCurrTime();
};
