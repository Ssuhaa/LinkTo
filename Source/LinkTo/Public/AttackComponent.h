// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.h"
#include "AttackComponent.generated.h"


UENUM()
enum class EAttackState : int8
{
	AttackIdle,
	AttackSword,
	AttackBow
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKTO_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);
		
	class AJS_Player* player;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	class UInputMappingContext* attackMapping;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	TArray <class UInputAction*> leftInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	TArray <class UInputAction*> rightInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	EPlayerState playerState;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	EAttackState currAttackState = EAttackState::AttackIdle;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	bool bSwitch = false;

	/*UPROPERTY(EditAnywhere, Category = "VR_Settings|UI")*/
	

private:
	void OnButtonA();
	void ChangeWeapon();
	void IdleState();
	void SwordState();
	void BowState();

	void OnButtonMenu();
	void FireArrow();
	void FireSword();
	void OnTriggerRight();
	void OnReleaseRight();
	void OnWeaponUI();
};
