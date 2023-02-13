// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKTO_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);
	

	class AJS_Player* player;
	class ASH_Ice* ice;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	float walkSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	class UInputMappingContext* moveMapping;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	TArray<class UInputAction*> keyInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	TArray <class UInputAction*> leftInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	TArray <class UInputAction*> rightInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	class UInputAction* spaceBar;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	EPlayerState playerState;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	bool canJump = true;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	bool bParasale = true;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|MoveComponent")
	bool canParasale = true;
	UPROPERTY(EditAnywhere)
	bool canClimb = false;
	UPROPERTY(EditAnywhere)
	bool bClimb = false;
	UPROPERTY(EditAnywhere)
	FVector lastLoc;

	UPROPERTY()
	FHitResult hitInfo;
private:
	void RotateCamera(const struct FInputActionValue& value);
	void Move(const struct FInputActionValue& value);
	void OnDash();
	void OnWalk();
	void TriggerButtonB();
	void ReleaseButtonB();
	void StartButtonA();
	void StartButtonB();
	void Parasale(bool value);
	void ClimbingLineTrace();
	void Climb();
	void ReleaseClimb();
	void CameraReset();
};
