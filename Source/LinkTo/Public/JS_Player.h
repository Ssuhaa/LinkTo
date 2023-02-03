// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "JS_Player.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API AJS_Player : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AJS_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UCapsuleComponent* compRoot;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UCameraComponent* compCam; // 카메라 컴프
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UStaticMeshComponent* meshHead; // 머리 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMotionControllerComponent* leftController; // 왼손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* leftLog; // 왼손 로그
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class USkeletalMeshComponent* leftHand; // 왼손 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMotionControllerComponent* rightController; // 오른손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* rightLog; // 오른손 로그
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class USkeletalMeshComponent* rightHand; // 오른손 메쉬
	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
	TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin; // 헤드위치
 	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UInputMappingContext* myMapping; // IMC 담는 변수
 	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	TArray<class UInputAction*> leftInputs; // 왼손 인풋들 배열
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	TArray<class UInputAction*> rightInputs; // 오른손 인풋들 배열
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMoveComponent* moveComp; // 움직임 컴포넌트
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* moveLog;

	UPROPERTY(EditAnywhere,Category = "CharStatus")
	float HP = 10; // 1당 하트 1칸
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float stamina = 100; // 스테미나
	UPROPERTY(EditAnywhere,Category = "WorldStatus")
	float currTime = 0;
	UPROPERTY(EditAnywhere, Category = "WorldStatus")
	float deltaTime = 0;

private:
	// 트리거
	void OnTriggerLeft(const struct FInputActionValue& value);
	// thumbstick
	void OnThumbstickLeft(const struct FInputActionValue& value);
	// grip
	void OnGripLeft(const struct FInputActionValue& value);
	// X
	void On_X_ButtonLeft(const struct FInputActionValue& value);
	// Y
	void On_Y_ButtonLeft(const struct FInputActionValue& value);
	// 메뉴
	void OnMenuLeft(const struct FInputActionValue& value);
	// 왼쪽 로그
	void OnLogLeft(FString value);

	// 오른쪽 
	
	// 트리거
	void OnTriggerRight(const struct FInputActionValue& value);
	// thumbstick
	void OnThumbstickRight(const struct FInputActionValue& value);
	// grip
	void OnGripRight(const struct FInputActionValue& value);
	// A
	void On_A_ButtonRight(const struct FInputActionValue& value);
	// B
	void On_B_ButtonRight(const struct FInputActionValue& value);
	// 로그
	void OnLogRight(FString value);

public:
	void OnDash();
	void OnWalk();
	void OnLogMove(FString value);
	
};
