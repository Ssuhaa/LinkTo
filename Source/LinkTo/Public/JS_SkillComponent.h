// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JS_SkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	Defalt,
	Bomb,
	Margnet,
	TimeLock,
	IceMaker
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKTO_API UJS_SkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJS_SkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
		

public:
	
	class AJS_Player* player;
	class AJS_Bomb* grabbedBomb;
	//컴포넌트
	
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	int32 selectContext = 0;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	class UInputMappingContext* skillMapping;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TArray <class UInputAction*> leftInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TArray <class UInputAction*> rightInputs;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	class UInputMappingContext* SHMapping;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TArray <class UInputAction*> inputAction;
	bool bGrabBomb = false;
	bool bSkillMenu = false;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TArray<class AJS_Bomb*> bombArray;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TSubclassOf<class AJS_Bomb> bombFactory;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	FVector prevLocation;
	FVector prevForward;
	FVector throwDirection;
	bool bIsReady = false;
	float torquePower = 3000;
	float throwPower = 1500;
private:
	void OnButtonA(const struct FInputActionValue& value);
	void ReleaseButtonA();
	void OnButtonX();
	void OnButtonY();
	void ChangeSkill();
	void OnButtonTrigger();
	void OnSkillUI();
	void OnGrabRight();
	void OnGrabLeft();
	
	void OffBomb();
	void OnBomb();
	void ReadyToThrowBomb();
	void ReleaseBomb(USkeletalMeshComponent* selectHand, FVector torque);

///////////////////////////////////////////////////////////////////////////////
public:
	//스테이트
	UPROPERTY(EditAnywhere, Category = Player)
	bool bParasailing = false;
	UPROPERTY(EditAnywhere, Category = Player)
	ESkillState currSkillState = ESkillState::Defalt;

	bool isPressedG = false;
	bool isClickedLMouse = true;
	bool isGrab = false;

	//라인트레이스 변수	FVector waterHitPoint;
	class ATimeLockBase* hitTLActor;
	class AIceMakerBase* hitIMActor;
	class AMagnetBase* hitMNActor;

private:
	//타임락
	void TimeLock();
	void LookTimeLock();
	void OffTimeLock();
	bool FindOnTimeLockActor();

	//아이스메이커
	void IceMaker();
	void LookIceMaker();
	void OffIceMaker();
	void IceBrake();
	TSubclassOf<class ASH_Ice> iceFactory;
	TArray<class ASH_Ice*> iceArray;
	int32 iceNum = 0;

	//마그넷
	void Magnet();
	void LookMagnet();
	void OffMagnet();
	class AMagnetBase* GrabMagnetActor;
	float ratio = 0;



	//키바인딩
	void OnG(const struct FInputActionValue& value);
	void OnF(const struct FInputActionValue& value);
	void OnWS(const struct FInputActionValue& value);
	void OnAD(const struct FInputActionValue& value);
	void LookUp(const struct FInputActionValue& value);

	float speed = 500.f;

	FColor LineColor = FColor::Red;

	//어레이
	void AddArray();
	TArray<class AActor*> obstaclearray;
	TArray<class ATimeLockBase*> timelockActorarr;
	TArray<class AIceMakerBase*> iceMakerActorarr;
	TArray<class AMagnetBase*> magnetActorarr;

	//라인트레이스
	FHitResult Hitinfo;
	class ASH_Ice* hitIce;
	void LineTraceInteration();

};
