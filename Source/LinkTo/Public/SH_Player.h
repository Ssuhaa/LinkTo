// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "SH_Player.generated.h"


UENUM(BlueprintType)
enum class EPlayerState1 : uint8
{
	Defalt,
	TimeLock,
	IceMaker,
	Margnet,
	Boomb,

};
/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Player : public ACharacterBase
{
	GENERATED_BODY()

	// Sets default values for this character's properties
	ASH_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:

//키바인딩
	void OnG(const struct FInputActionValue& value);
	void OnF(const struct FInputActionValue& value);
	void OnWS(const struct FInputActionValue& value);
	void OnAD(const struct FInputActionValue& value);
	void OnLeftMouse(const struct FInputActionValue& value);
	void LookUp(const struct FInputActionValue& value);
	void PlayerJump();
	FVector dir;
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
	
	float currentTime = 0;

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

	//마그넷
	void Magnet();
	void LookMagnet();
	void OffMagnet();
	bool FindOnMagnetActor();
	class AMagnetBase* GrabMagnetActor;


public:

//컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UCameraComponent* compCam;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UInputMappingContext* myMapping;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	TArray<class UInputAction*> keyInputs;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class USceneComponent* MagnetGrabComp;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UPhysicsHandleComponent* MagnetHandle;

//스테이트
	UPROPERTY(EditAnywhere, Category = Player)
	bool bParasailing = false;
	UPROPERTY(EditAnywhere, Category = Player)
	EPlayerState1 PlayerInterState = EPlayerState1::Defalt; 

	void WindUp(float WindValue);
	bool isPressedG = false;
	bool isClickedLMouse = true;
	bool isGrab = false;

//라인트레이스 변수	FVector waterHitPoint;
	class ATimeLockBase* hitTLActor;
	class AIceMakerBase* hitIMActor;
	class AMagnetBase* hitMNActor;


	FVector currentwindlos;

};
