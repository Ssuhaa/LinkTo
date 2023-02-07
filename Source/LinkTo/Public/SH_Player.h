// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "SH_Player.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
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

public:

	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UCameraComponent* compCam;
	UPROPERTY(EditDefaultsOnly, Category = Component)
	class UInputMappingContext* myMapping; 
	UPROPERTY(EditDefaultsOnly, Category = Component)
	TArray<class UInputAction*> keyInputs; 


private:
	void OnG(const struct FInputActionValue& value);
	void OffG(const struct FInputActionValue& value);
	void OnWS(const struct FInputActionValue& value);
	void OnAD(const struct FInputActionValue& value);
	void OnLeftMouse(const struct FInputActionValue& value);
	void LookUp(const struct FInputActionValue& value);
	void PlayerJump();

	FVector dir;
	float speed = 500.f;

	void AddArray();
	TArray<class AActor*> obstaclearray;
	class ATimeLockBase* hitTLActor;
	class AIceMakerBase* hitIMActor;
	UPROPERTY(EditAnywhere)
	class ASH_Ice* hitIce;
	
	void TimeLock();
	void LookTimeLock();
	void OffTimeLock();
	bool FindOnTimeLockActor();

	void IceMaker();
	void IceBrake();
	void LookIceMaker();
	void OffIceMaker();
	UPROPERTY(EditAnywhere, Category = IceMaker)
	TSubclassOf<class ASH_Ice> iceFactory;
	UPROPERTY(VisibleAnywhere, Category = IceMaker)
	TArray<class ASH_Ice*> iceArray;
	FVector waterHitPoint;


public:
	UPROPERTY(EditAnywhere, Category = Interation)
	TArray<class ATimeLockBase*> timelockActorarr;
	UPROPERTY(EditAnywhere, Category = Interation)
	TArray<class AIceMakerBase*> iceMakerActorarr;
	
	UPROPERTY(EditAnywhere, Category = Player)
	bool bParasailing = false;
	
	UPROPERTY(EditAnywhere, Category = Player)
	EPlayerState PlayerInterState = EPlayerState::Defalt; 

	void WindUp(float WindValue);

};
