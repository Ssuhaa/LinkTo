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
	leftLog->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextTop);
	
	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEFT HAND"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RIGHT CONTROLLER"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";
	
	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RIGHT LOG"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextTop);

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIGHT HAND"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

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

}

// Called to bind functionality to input
void AJS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent를 EnhancedInputComponent로 캐스팅
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AJS_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AJS_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &AJS_Player::OnThumbstickLeft);
	}
}
void AJS_Player::OnTriggerLeft(const struct FInputActionValue& value)
{
	float val = value.Get<float>();

	// 왼손 로그에 값을 출력
	FString msg = FString::Printf(TEXT("%3.f"), val);
	leftLog->SetText(FText::FromString(msg));
}

void AJS_Player::OnThumbstickLeft(const struct FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();
	FVector dir = FVector(axis.X, axis.Y, 0);
	dir.GetSafeNormal();
	AddMovementInput(dir*walkSpeed);
	
}