// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "SH_Player.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Player : public ACharacterBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ASH_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UCameraComponent* compCam; // 카메라 컴프
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UStaticMeshComponent* meshHead; // 머리 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* leftController; // 왼손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* leftHand; // 왼손 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* rightController; // 오른손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* rightHand; // 오른손 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin; // 헤드위치
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UInputMappingContext* myMapping; // IMC 담는 변수
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		TArray<class UInputAction*> keyInputs; // 오른손 인풋들 배열
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMoveComponent* moveComp;

private:


	// 오른쪽 

	// 트리거
	void OnG(const struct FInputActionValue& value);
	void OffG(const struct FInputActionValue& value);
	void OnWS(const struct FInputActionValue& value);
	void OnAD(const struct FInputActionValue& value);
	void OnLeftMouse(const struct FInputActionValue& value);
	void LookUp(const struct FInputActionValue& value);
	void PlayerJump();

	TArray<class AActor*> obstaclearray;

	class AobstacleBase* hitObs;

	FVector dir;
	float speed = 500.f;
public:
	UPROPERTY(EditAnywhere)
	TArray<class AobstacleBase*> timelockActorarr;
	
	UPROPERTY(EditAnywhere)
	bool bParasailing = false;
	
	void WindUp();

};
