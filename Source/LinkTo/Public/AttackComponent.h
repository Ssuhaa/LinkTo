// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.h"
#include "AttackComponent.generated.h"


UENUM(BlueprintType)
enum class EAttackState : uint8
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
		
	class AobstacleBase* obstacleBase;
	class AJS_Player* player;
	class AJS_Arrow* arrow;
	class ATimeLockBase* timeLockActor;
	class UJS_LinkSound* sounds;
	UPROPERTY(EditAnywhere, Category = Arrow)
	TSubclassOf<class AJS_Arrow> arrowFactory;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	class UInputMappingContext* attackMapping;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	TArray <class UInputAction*> leftInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	TArray <class UInputAction*> rightInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|AttackComponent")
	EPlayerState playerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "VR_Settings|AttackComponent")
	EAttackState currAttackState = EAttackState::AttackIdle;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	bool bWeaponMenu = false;
	UPROPERTY(EditAnywhere, Category = Arrow)
	float accArrowSpeed = 0; // 화살의 가속도
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Sword)
	FVector currHandPos = FVector(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sword)
	FVector oldPos = FVector(0);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Sword)
	bool bHit = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Sword)
	float attackPower = 500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Sword)
	FVector hitNormal;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	TSubclassOf<class UJS_WidgetWeaponSwitch> weaponUIFactory;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	class UJS_WidgetWeaponSwitch* weaponWidget;

	void WeaponMenuOnOff(bool value);
	void WeaponMenuMove(int32 value);

private:
	void OnButtonA();
	void ChangeWeapon();
	void IdleState();
	void SwordState();
	void BowState();
	void SwordLineTrace();
	void SetLineTrace();


	void OnButtonMenu();
	void OnTriggerArrow();
	void OnReleaseArrow();
	void OnWeaponUI();
	void OnGrabRight();
	void OnGrabLeft();

};
