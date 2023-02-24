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

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void ContactKillZone();

public:

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UStaticMeshComponent* compSword;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Weapon")
	class UStaticMeshComponent* compBow;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Weapon")
	class AJS_Sword* sword;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Weapon")
	TSubclassOf<AJS_Sword> swordFactory;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "VR_Settings|Components")
	class UWidgetInteractionComponent* compWidgetInter;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "VR_Settings|Components")
	class UJS_WidgetPointComponent* compWidgetPoint;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UJS_LinkSound* compSound;


	UPROPERTY(EditDefaultsOnly, Category = Component)
	TArray<class UInputAction*> keyInputs;
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
	class UMoveComponent* compMove; // ������ ������Ʈ
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "VR_Settings|Components")
	class UJS_SkillComponent* compSkill;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* moveLog;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "VR_Settings|Components")
	class UPlayerStateComponent* compState;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "VR_Settings|Components")
	class UAttackComponent* compAttack;



	UPROPERTY(EditDefaultsOnly, Category = Component)
	class USceneComponent* MagnetGrabComp;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UPhysicsHandleComponent* MagnetHandle;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UNiagaraComponent* MagNS;


	FVector dir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UWidgetComponent* widgetComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UWidgetComponent* leftWidgetComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UWidgetComponent* rightWidgetComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UPlayerMainWG* MainWG;
	void ovelayMenuMainWG(UUserWidget* widget);

};
