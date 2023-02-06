// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Player.h"
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
#include "obstacleBase.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PawnMovementComponent.h>




ASH_Player::ASH_Player()
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


	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEFT HAND"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RIGHT CONTROLLER"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";


	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIGHT HAND"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	

}

// Called when the game starts or when spawned
void ASH_Player::BeginPlay()
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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AobstacleBase::StaticClass(), obstaclearray);
	for (int32 i = 0; i < obstaclearray.Num(); i++)
	{
		AobstacleBase* currA = Cast<AobstacleBase>(obstaclearray[i]);
		if (currA->interationType == EObstacleType::Timelock)
		{
			timelockActorarr.Add(currA);

		}
	}
	playerCon->PlayerCameraManager->ViewPitchMin = -80.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 30.0f;
}

// Called every frame
void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dir = FTransform(GetControlRotation()).TransformVector(dir);

	AddMovementInput(dir.GetSafeNormal());
	dir = FVector::ZeroVector;
}

// Called to bind functionality to input
void ASH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent를 EnhancedInputComponent로 캐스팅
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		

		// 오른손 바인딩 
		enhancedInputComponent->BindAction(keyInputs[0], ETriggerEvent::Triggered, this, &ASH_Player::OnG);
		enhancedInputComponent->BindAction(keyInputs[0], ETriggerEvent::Completed, this, &ASH_Player::OffG);
		enhancedInputComponent->BindAction(keyInputs[1], ETriggerEvent::Triggered, this, &ASH_Player::OnWS);
		enhancedInputComponent->BindAction(keyInputs[2], ETriggerEvent::Triggered, this, &ASH_Player::OnAD);
		enhancedInputComponent->BindAction(keyInputs[3], ETriggerEvent::Triggered, this, &ASH_Player::OnLeftMouse);
		enhancedInputComponent->BindAction(keyInputs[4], ETriggerEvent::Triggered, this, &ASH_Player::LookUp);
		enhancedInputComponent->BindAction(keyInputs[5], ETriggerEvent::Triggered, this, &ASH_Player::PlayerJump);
	}

}



// 오른쪽
void ASH_Player::OnG(const FInputActionValue& value)
{
	if(timelockActorarr.IsEmpty()) return;
	for (int32 i=0; i<timelockActorarr.Num() ; i++)
	{
		timelockActorarr[i]->InteractionTimeLock(true);
	}

	FVector Startpos = compCam->GetComponentLocation(); //시작 위치
	FVector Endpos = Startpos + compCam->GetForwardVector() * 5000; // 끝나는 위치
	FHitResult Hitinfo; //결과를 담을 변수
	//나머지 옵션설정 
	FCollisionQueryParams par; // Ignore를 담을 변수 
	par.AddIgnoredActor(this); // 자기자신은 HIT에서 제외 (더 추가 가능)
	DrawDebugLine(GetWorld(),  Startpos, Endpos, FColor::Red, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par); // 히트했는지 안했는지 담아두겠다
	if (bhit == true)
	{
		hitObs = Cast<AobstacleBase>(Hitinfo.GetActor());
		if (hitObs != nullptr)
		{
			if (hitObs->interationType == EObstacleType::Timelock)
			{
				
				hitObs->LookInTimeLock();
			}
		}
	}
}

void  ASH_Player::OffG(const struct FInputActionValue& value)
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(false);

	}
}

void ASH_Player::OnWS(const struct FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.X += Axis;

}

void ASH_Player::OnAD(const struct FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.Y+= Axis;

}

void ASH_Player::OnLeftMouse(const struct FInputActionValue& value)
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		if (timelockActorarr[i]->InteractionMesh->GetMaterial(0) == timelockActorarr[i]->MatArray[2])
		{
			timelockActorarr[i]->OnTimeLock();
			return;
		}

	}
}

void ASH_Player::LookUp(const struct FInputActionValue& value)
{
	FVector2D MouseAxis = value.Get<FVector2D>();

	AddControllerYawInput(MouseAxis.X);
	AddControllerPitchInput(-MouseAxis.Y);
}

void ASH_Player::PlayerJump()
{
	Jump();
	
}

void ASH_Player::WindUp()
{
	FVector Vel = GetMovementComponent()->Velocity;
	GetMovementComponent()->Velocity = FVector(Vel.X, Vel.Y,Vel.Z+100);
}
