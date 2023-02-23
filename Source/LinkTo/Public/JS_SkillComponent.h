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
protected:
	// Sets default values for this component's properties
	UJS_SkillComponent();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	class AJS_Player* player;
	class UJS_LinkSound* sounds;
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

public:
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TArray<class AJS_Bomb*> bombArray;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	TSubclassOf<class AJS_Bomb> bombFactory;

	bool canExplo = false;


private:

	void OnButtonA(const struct FInputActionValue& value);
	void ReleaseButtonA();
	void OnButtonX();
	void OnButtonY();
	void ChangeSkill();
	void OnButtonTrigger();
	void OnButtonTriggerRight();
	void OnSkillUI();
	void OnGrabRight();
	void OnGrabLeft();

	//폭탄
	void OffBomb();
	void OnBomb();
	void ReadyToThrowBomb();
	void ReleaseBomb(USkeletalMeshComponent* selectHand, FVector torque);
	FVector prevLocation;
	FVector prevForward;
	FVector throwDirection;
	bool bIsReady = false;
	float torquePower = 6000;
	float throwPower = 3000;
	
	class AJS_Bomb* grabbedBomb;

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
	class ASH_IceGhost* Ghostice;
	int32 iceNum = 0;

	//마그넷
	void Magnet();
	void LookMagnet();
	void OffMagnet();
	UPROPERTY(EditAnywhere)
	float MagnetLength = 1500;
	class AMagnetBase* GrabMagnetActor;
	float ratio = 0;


	//키바인딩
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
	class UNiagaraSystem* NSCrossHair;

	bool isClickedLMouse = true;
	bool isGrab = false;
	bool bSkillMenu = false;

public:

	//스테이트
	UPROPERTY(EditAnywhere, Category = Player)
	bool bParasailing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	ESkillState currSkillState = ESkillState::Defalt;

	bool isPressedG = false;
	UPROPERTY(VisibleAnywhere)
	class ATimeLockBase* OnTimeLockActor;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	class UJS_WidgetSkillSwitch* skillWidget;
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Widget")
	TSubclassOf<class UJS_WidgetSkillSwitch> skillUIFactory;
	void SkillMenuOnOff(bool value);
	void SkilMenuMove(int32 value);

	UPROPERTY(EditAnywhere, Category = "VR_Settings|SkillComponent")
	bool bGrabBomb = false;

	//라인트레이스 변수
	class ATimeLockBase* hitTLActor;
	class AIceMakerBase* hitIMActor;
	class AMagnetBase* hitMNActor;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere)
	bool DebagKeyBorad = false;
};
