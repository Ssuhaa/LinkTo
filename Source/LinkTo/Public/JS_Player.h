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
	class UCameraComponent* compCam; // ī�޶� ����
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UStaticMeshComponent* meshHead; // �Ӹ� �޽�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMotionControllerComponent* leftController; // �޼� ��Ʈ�ѷ�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* leftLog; // �޼� �α�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class USkeletalMeshComponent* leftHand; // �޼� �޽�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMotionControllerComponent* rightController; // ������ ��Ʈ�ѷ�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* rightLog; // ������ �α�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class USkeletalMeshComponent* rightHand; // ������ �޽�
	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
	TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin; // �����ġ
 	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UInputMappingContext* myMapping; // IMC ��� ����
 	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	TArray<class UInputAction*> leftInputs; // �޼� ��ǲ�� �迭
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	TArray<class UInputAction*> rightInputs; // ������ ��ǲ�� �迭
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMoveComponent* moveComp; // ������ ������Ʈ
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* moveLog;

	UPROPERTY(EditAnywhere,Category = "CharStatus")
	float HP = 10; // 1�� ��Ʈ 1ĭ
	UPROPERTY(EditAnywhere, Category = "CharStatus")
	float stamina = 100; // ���׹̳�
	UPROPERTY(EditAnywhere,Category = "WorldStatus")
	float currTime = 0;
	UPROPERTY(EditAnywhere, Category = "WorldStatus")
	float deltaTime = 0;

private:
	// Ʈ����
	void OnTriggerLeft(const struct FInputActionValue& value);
	// thumbstick
	void OnThumbstickLeft(const struct FInputActionValue& value);
	// grip
	void OnGripLeft(const struct FInputActionValue& value);
	// X
	void On_X_ButtonLeft(const struct FInputActionValue& value);
	// Y
	void On_Y_ButtonLeft(const struct FInputActionValue& value);
	// �޴�
	void OnMenuLeft(const struct FInputActionValue& value);
	// ���� �α�
	void OnLogLeft(FString value);

	// ������ 
	
	// Ʈ����
	void OnTriggerRight(const struct FInputActionValue& value);
	// thumbstick
	void OnThumbstickRight(const struct FInputActionValue& value);
	// grip
	void OnGripRight(const struct FInputActionValue& value);
	// A
	void On_A_ButtonRight(const struct FInputActionValue& value);
	// B
	void On_B_ButtonRight(const struct FInputActionValue& value);
	// �α�
	void OnLogRight(FString value);

public:
	void OnDash();
	void OnWalk();
	void OnLogMove(FString value);
	
};
