// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	bLanding,
	bFalling,
	bCliming,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	EPlayerState currState = EPlayerState::bLanding;

	class AJS_Player* player;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float HP = 10;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CharStatus")
	float stamina = 100;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	bool bUseStamina = false;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float currTime = 0;
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	bool bInAir = false;


	void LandsingState();
	void FallingState();
	void ClimbingState();
	void ChangeState(EPlayerState currState);
	void UseStamina();
	void ChargeStamina();
	void CurrStamina(bool value);
	void IsInAir();
	void SetStaminaState(bool value);
	
};
