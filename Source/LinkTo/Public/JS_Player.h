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
	float walkSpeed = 600.f;

private:
	// ���� Ʈ���� �Լ�
	void OnTriggerLeft(const struct FInputActionValue& value);

	// ���� thumbstick �Լ�
	void OnThumbstickLeft(const struct FInputActionValue& value);

};
