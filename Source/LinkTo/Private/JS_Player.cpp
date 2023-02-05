// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Player.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MoveComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerStateComponent.h"


AJS_Player::AJS_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	compCam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	compCam->SetupAttachment(RootComponent);

	meshHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HEAD"));
	meshHead->SetupAttachment(compCam);
	meshHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LEFT CONTROLLER"));
	leftController->SetupAttachment(RootComponent);
	leftController->MotionSource = "Left";

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LEFT LOG"));
	leftLog->SetupAttachment(leftController);
	leftLog->SetRelativeScale3D(FVector(0.5));
	leftLog->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextTop);

	moveLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MOVE LOG"));
	moveLog->SetupAttachment(leftController);
	moveLog->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	moveLog->SetRelativeScale3D(FVector(0.3));
	moveLog->SetTextRenderColor(FColor::Green);
	moveLog->SetHorizontalAlignment(EHTA_Center);
	moveLog->SetVerticalAlignment(EVRTA_TextBottom);
	
	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEFT HAND"));
	leftHand->SetupAttachment(leftController);
	leftLog->SetRelativeScale3D(FVector(0.5));
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RIGHT CONTROLLER"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";
	
	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RIGHT LOG"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	moveLog->SetRelativeScale3D(FVector(0.5));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextTop);

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIGHT HAND"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	GetCharacterMovement()->JumpZVelocity = 400.f; 
	JumpMaxCount = 1;

	compMove = CreateDefaultSubobject<UMoveComponent>(TEXT("MOVE COMP"));
	compState = CreateDefaultSubobject<UPlayerStateComponent>(TEXT("STATE COMP"));
	
	
}

// Called when the game starts or when spawned
void AJS_Player::BeginPlay()
{
	Super::BeginPlay();
	
	// 헤드 장비 기준 위치 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	// 1. 플레이어 컨트롤러 가져오기
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
	subsys->AddMappingContext(myMapping, 0);

	
}

// Called every frame
void AJS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow, FString::Printf(TEXT("%f"),GetCharacterMovement()->GravityScale));
}

// Called to bind functionality to input
void AJS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent를 EnhancedInputComponent로 캐스팅
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (enhancedInputComponent != nullptr)
	{
		//왼손 바인딩
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AJS_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AJS_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &AJS_Player::OnThumbstickLeft);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &AJS_Player::OnGripLeft);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Triggered, this, &AJS_Player::On_X_ButtonLeft);
		enhancedInputComponent->BindAction(leftInputs[4], ETriggerEvent::Triggered, this, &AJS_Player::On_Y_ButtonLeft);
		enhancedInputComponent->BindAction(leftInputs[5], ETriggerEvent::Triggered, this, &AJS_Player::OnMenuLeft);

		// 오른손 바인딩 
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &AJS_Player::OnTriggerRight);
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AJS_Player::OnTriggerRight);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &AJS_Player::OnThumbstickRight);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Triggered, this, &AJS_Player::OnGripRight);
		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Triggered, this, &AJS_Player::On_A_ButtonRight);
		enhancedInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &AJS_Player::On_B_ButtonRight);

		compMove->SetupPlayerInputComponent(enhancedInputComponent);

	}
	
}

// 왼손
void AJS_Player::OnTriggerLeft(const FInputActionValue& value)
{
	float val = value.Get<float>();

	// 왼손 로그에 값을 출력
	FString msg = FString::Printf(TEXT("%.2f"), val);
	OnLogLeft(msg);
}
void AJS_Player::OnThumbstickLeft(const FInputActionValue& value)
{
	FVector2D val = value.Get<FVector2D>();
	FString valX = FString::Printf(TEXT("X : %.2f"),val.X);
	FString valY = FString::Printf(TEXT("Y : %.2f"),val.Y);
	OnLogLeft(valX + valY);
}
void AJS_Player::OnGripLeft(const FInputActionValue& value)
{
	OnLogLeft("Grip");
}
void AJS_Player::On_X_ButtonLeft(const FInputActionValue& value)
{
	OnLogLeft("X");
}
void AJS_Player::On_Y_ButtonLeft(const FInputActionValue& value)
{
	OnLogLeft("Y");
}
void AJS_Player::OnMenuLeft(const FInputActionValue& value)
{
	OnLogLeft("Menu");
}
void AJS_Player::OnLogLeft(FString value)
{
	leftLog->SetText(FText::FromString(value));
}

// 오른쪽
void AJS_Player::OnTriggerRight(const FInputActionValue& value)
{
	OnLogRight("Trigger");
}

void AJS_Player::OnThumbstickRight(const FInputActionValue& value)
{
	OnLogRight("Thumbstick");
}

void AJS_Player::OnGripRight(const FInputActionValue& value)
{
	OnLogRight("Grip");
}
void AJS_Player::On_A_ButtonRight(const FInputActionValue& value)
{
	OnLogRight("A");
}
void AJS_Player::On_B_ButtonRight(const FInputActionValue& value)
{
	OnLogRight("B");
}
void AJS_Player::OnLogRight(FString value)
{
	rightLog->SetText(FText::FromString(value));
}


void AJS_Player::OnLogMove(FString value)
{
	moveLog->SetText(FText::FromString(value));
}
