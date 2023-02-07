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
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PawnMovementComponent.h>
#include "TimeLockBase.h"
#include "IceMakerBase.h"
#include "SH_Ice.h"




ASH_Player::ASH_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compCam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	compCam->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	ConstructorHelpers::FClassFinder <ASH_Ice> tempice(TEXT("/Script/CoreUObject.Class'/Script/LinkTo.SH_Ice'"));
	if (tempice.Succeeded())
	{
		iceFactory = tempice.Class;
	}
}

// Called when the game starts or when spawned
void ASH_Player::BeginPlay()
{
	Super::BeginPlay();

	// 1. �÷��̾� ��Ʈ�ѷ� ��������
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
	subsys->AddMappingContext(myMapping, 0);

	AddArray();

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
	// PlayerInputComponent�� EnhancedInputComponent�� ĳ����
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(keyInputs[0], ETriggerEvent::Triggered, this, &ASH_Player::OnG);
		enhancedInputComponent->BindAction(keyInputs[0], ETriggerEvent::Completed, this, &ASH_Player::OffG);
		enhancedInputComponent->BindAction(keyInputs[1], ETriggerEvent::Triggered, this, &ASH_Player::OnWS);
		enhancedInputComponent->BindAction(keyInputs[2], ETriggerEvent::Triggered, this, &ASH_Player::OnAD);
		enhancedInputComponent->BindAction(keyInputs[3], ETriggerEvent::Triggered, this, &ASH_Player::OnLeftMouse);
		enhancedInputComponent->BindAction(keyInputs[4], ETriggerEvent::Triggered, this, &ASH_Player::LookUp);
		enhancedInputComponent->BindAction(keyInputs[5], ETriggerEvent::Triggered, this, &ASH_Player::PlayerJump);
	}
}

void ASH_Player::OnWS(const FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.X += Axis;
}

void ASH_Player::OnAD(const  FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.Y += Axis;
}

void ASH_Player::LookUp(const FInputActionValue& value)
{
	FVector2D MouseAxis = value.Get<FVector2D>();

	AddControllerYawInput(MouseAxis.X);
	AddControllerPitchInput(-MouseAxis.Y);
}

void ASH_Player::PlayerJump()
{
	Jump();
}

// �÷��̾� �ٶ����� ����
void ASH_Player::WindUp(float WindValue)
{
	FVector Vel = GetMovementComponent()->Velocity;
	GetMovementComponent()->Velocity = FVector(Vel.X, Vel.Y, Vel.Z + WindValue);
}

void ASH_Player::OnG(const FInputActionValue& value)
{
	switch (PlayerInterState)
	{
	case EPlayerState::Defalt:
		break;
	case EPlayerState::TimeLock:
		LookTimeLock();
		break;
	case EPlayerState::IceMaker:
		LookIceMaker();
		break;
	case EPlayerState::Margnet:
		break;
	case EPlayerState::Boomb:
		break;
	}
}

void  ASH_Player::OffG(const FInputActionValue& value)
{
	switch (PlayerInterState)
	{
	case EPlayerState::Defalt:
		break;
	case EPlayerState::TimeLock:
		OffTimeLock();
		break;
	case EPlayerState::IceMaker:
		OffIceMaker();
		break;
	case EPlayerState::Margnet:
		break;
	case EPlayerState::Boomb:
		break;
	}
}

void ASH_Player::OnLeftMouse(const FInputActionValue& value)
{
	switch (PlayerInterState)
	{
	case EPlayerState::Defalt:
		break;
	case EPlayerState::TimeLock:
		TimeLock();
		break;
	case EPlayerState::IceMaker:
		if (hitIMActor != nullptr)
		{
		IceMaker();
		}
		else if(hitIce != nullptr)
		{
		IceBrake();
		}

		break;
	case EPlayerState::Margnet:
		break;
	case EPlayerState::Boomb:
		break;
	}
}

//interaction �迭�� Actor�� �߰��ϱ�
void ASH_Player::AddArray()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeLockBase::StaticClass(), obstaclearray);
	for (int32 i = 0; i < obstaclearray.Num(); i++)
	{
		ATimeLockBase* currA = Cast<ATimeLockBase>(obstaclearray[i]);
		timelockActorarr.Add(currA);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIceMakerBase::StaticClass(), obstaclearray);
	for (int32 i = 0; i < obstaclearray.Num(); i++)
	{
		AIceMakerBase* currA = Cast<AIceMakerBase>(obstaclearray[i]);
		iceMakerActorarr.Add(currA);
	}

	for (int32 i = 0; i < 2; i++)
	{
		ASH_Ice* currice = GetWorld()->SpawnActor<ASH_Ice>(iceFactory);
		currice->SetActiveIce(false);
		iceArray.Add(currice);
	}
}


//Ÿ�Ӷ� ���� ����
void ASH_Player::LookTimeLock()
{
	//Ÿ�Ӷ� ���� ǥ���ϱ�
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(true);
	}
	// ����Ʈ���̽� ���� Ÿ�Ӷ� ���Ͱ� �ɸ��� ��������� �ٲٱ�
	FVector Startpos = compCam->GetComponentLocation();
	FVector Endpos = Startpos + compCam->GetForwardVector() * 5000;
	FHitResult Hitinfo;
	FCollisionQueryParams par;
	par.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), Startpos, Endpos, FColor::Red, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit == true)
	{
		hitTLActor = Cast<ATimeLockBase>(Hitinfo.GetActor());
		if (hitTLActor != nullptr)
		{
			hitTLActor->LookInTimeLock();
		}
	}
}

//Ÿ�Ӷ� ǥ�� ����
void ASH_Player::OffTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(false);
	}
}

//Ÿ�Ӷ� �ɱ�
void ASH_Player::TimeLock()
{
	if (!FindOnTimeLockActor())
	{
		for (int32 i = 0; i < timelockActorarr.Num(); i++)
		{
			timelockActorarr[i]->OnTimeLock();
		}
	}
}

//Ÿ�Ӷ� �ɸ� ���Ͱ� �ִ��� ã��
bool ASH_Player::FindOnTimeLockActor()
{
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		if (timelockActorarr[i]->bTimeLock) return true;
	}
	return false;
}



//���̽�����Ŀ ���� ����
void ASH_Player::LookIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(true);
	}

	// ����Ʈ���̽��� �ε��� �� ����
	FVector Startpos = compCam->GetComponentLocation();
	FVector Endpos = Startpos + compCam->GetForwardVector() * 5000;
	FHitResult Hitinfo;
	FCollisionQueryParams par;
	par.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), Startpos, Endpos, FColor::Blue, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit == true)
	{
		hitIMActor = Cast<AIceMakerBase>(Hitinfo.GetActor());
		if (hitIMActor != nullptr)
		{
			waterHitPoint = Hitinfo.ImpactPoint;
		}
		hitIce = Cast<ASH_Ice>(Hitinfo.GetActor());
	}
}


//���̽�����Ŀ ǥ�� ����
void ASH_Player::OffIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(false);
	}
}


//���� ����
void ASH_Player::IceMaker()
{
	if (!iceArray[0]->isIceVisible())
	{
		iceArray[0]->SetActiveIce(true);
		iceArray[0]->SetActorLocation(waterHitPoint);
		iceArray.Swap(0, 1);
	}
	else
	{
		iceArray[1]->SetActiveIce(true);
		iceArray[1]->SetActorLocation(waterHitPoint);
		iceArray.Swap(1, 0);
	}
}

//���� ����
void ASH_Player::IceBrake()
{
	iceArray[iceArray.Find(hitIce)]->SetActiveIce(false);
}

