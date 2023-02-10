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
#include <Kismet/KismetMathLibrary.h>
#include "MagnetBase.h"
#include <PhysicsEngine/PhysicsHandleComponent.h>




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
	MagnetGrabComp = CreateDefaultSubobject<USceneComponent>(TEXT("MagnetGrabPos"));
	MagnetGrabComp->SetupAttachment(compCam);
	MagnetGrabComp->SetRelativeLocation(FVector(500, 0, 900));

	MagnetHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("MagnetHandle"));


}

// Called when the game starts or when spawned
void ASH_Player::BeginPlay()
{
	Super::BeginPlay();

	// 1. 플레이어 컨트롤러 가져오기
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());
	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
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

	if (isPressedG)
	{
		LineTraceInteration();
	}

	if (isGrab)
	{
		MagnetHandle->SetTargetLocation(MagnetGrabComp->GetComponentLocation());
	}



}

// Called to bind functionality to input
void ASH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// PlayerInputComponent를 EnhancedInputComponent로 캐스팅
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(keyInputs[0], ETriggerEvent::Started, this, &ASH_Player::OnG);
		enhancedInputComponent->BindAction(keyInputs[1], ETriggerEvent::Triggered, this, &ASH_Player::OnWS);
		enhancedInputComponent->BindAction(keyInputs[2], ETriggerEvent::Triggered, this, &ASH_Player::OnAD);
		enhancedInputComponent->BindAction(keyInputs[3], ETriggerEvent::Triggered, this, &ASH_Player::OnLeftMouse);
		enhancedInputComponent->BindAction(keyInputs[4], ETriggerEvent::Triggered, this, &ASH_Player::LookUp);
		enhancedInputComponent->BindAction(keyInputs[5], ETriggerEvent::Triggered, this, &ASH_Player::PlayerJump);
		enhancedInputComponent->BindAction(keyInputs[6], ETriggerEvent::Triggered, this, &ASH_Player::OnF);
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

// 플레이어 바람으로 띄우기
void ASH_Player::WindUp(float WindValue)
{
 	FVector Vel = GetMovementComponent()->Velocity;
 	GetMovementComponent()->Velocity = FVector(Vel.X, Vel.Y, Vel.Z + WindValue);
}

void ASH_Player::OnG(const FInputActionValue& value)
{
	isPressedG = true;
	switch (PlayerInterState)
	{
	case EPlayerState1::TimeLock:
		LookTimeLock();
		LineColor = FColor::Yellow;
		break;
	case EPlayerState1::IceMaker:
		LookIceMaker();
		LineColor = FColor::Blue;
		break;
	case EPlayerState1::Margnet:
		LookMagnet();
		LineColor = FColor::Red;
		break;
	}
}

void ASH_Player::OnF(const struct FInputActionValue& value)
{
	switch (PlayerInterState)
	{
	case EPlayerState1::TimeLock:
		OffTimeLock();
		break;
	case EPlayerState1::IceMaker:
		OffIceMaker();
		break;
	case EPlayerState1::Margnet:
		OffMagnet();
		break;
	}
	isPressedG = false;
}

void ASH_Player::OnLeftMouse(const FInputActionValue& value)
{

	switch (PlayerInterState)
	{
	case EPlayerState1::TimeLock:
		if (hitTLActor != nullptr)
		{
			TimeLock();
		}
		break;
	case EPlayerState1::IceMaker:
		if (hitIMActor != nullptr)
		{
			IceMaker();
		}
		else if (hitIce != nullptr)
		{
			IceBrake();
		}
		break;
	case EPlayerState1::Margnet:
		if (isClickedLMouse)
		{
			if (hitMNActor != nullptr)
			{
				Magnet();
			}
			isClickedLMouse = false;
		}
		else
		{
			if (GrabMagnetActor != nullptr)
			{
				GrabMagnetActor->releasedMagnet();
				MagnetHandle->ReleaseComponent();
				isGrab = false;
				GrabMagnetActor = nullptr;
				hitMNActor = nullptr;
			}
			isClickedLMouse = true;
		}
		break;
	}
}


//interaction 배열에 Actor들 추가하기
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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMagnetBase::StaticClass(), obstaclearray);
	for (int32 i = 0; i < obstaclearray.Num(); i++)
	{
		AMagnetBase* currA = Cast<AMagnetBase>(obstaclearray[i]);
		magnetActorarr.Add(currA);
	}

	for (int32 i = 0; i < 2; i++)
	{
		ASH_Ice* currice = GetWorld()->SpawnActor<ASH_Ice>(iceFactory);
		currice->SetActiveIce(false);
		iceArray.Add(currice);
	}
}

void ASH_Player::LineTraceInteration()
{
	if(FindOnTimeLockActor()) return;
	if(FindOnMagnetActor()) return;

	FVector Startpos = compCam->GetComponentLocation();
	FVector Endpos = Startpos + compCam->GetForwardVector() * 5000;
	FCollisionQueryParams par;
	par.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), Startpos, Endpos, LineColor, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit)
	{
		switch (PlayerInterState)
		{
		case EPlayerState1::TimeLock:
			if (hitTLActor != nullptr && hitTLActor != Hitinfo.GetActor())
			{
				hitTLActor->InteractionTimeLock(true);
			}
			hitTLActor = Cast<ATimeLockBase>(Hitinfo.GetActor());
			if (hitTLActor != nullptr)
			{
				hitTLActor->LookInTimeLock();
			}
			break;

		case EPlayerState1::IceMaker:
			hitIMActor = Cast<AIceMakerBase>(Hitinfo.GetActor());
			hitIce = Cast<ASH_Ice>(Hitinfo.GetActor());
			break;

		case EPlayerState1::Margnet:
			if (hitMNActor != nullptr && hitMNActor != Hitinfo.GetActor())
			{
				hitMNActor->InteractionMagnet(true);
			}
			hitMNActor = Cast<AMagnetBase>(Hitinfo.GetActor());
			if (hitMNActor != nullptr)
			{
				hitMNActor->LookInMagnet();

			}
			break;
		}
	}

}


//타임락 액터 보기
void ASH_Player::LookTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(true);
	}
}

//타임락 표시 끄기
void ASH_Player::OffTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(false);
	}
}

//타임락 걸기
void ASH_Player::TimeLock()
{
	hitTLActor->OnTimeLock();
	OffTimeLock();
	isPressedG = false;
}

//타임락 걸린 액터가 있는지 찾기
bool ASH_Player::FindOnTimeLockActor()
{
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		if (timelockActorarr[i]->bTimeLock) return true;
	}
	return false;
}



//아이스메이커 액터 보기
void ASH_Player::LookIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(true);
	}
}


//아이스메이커 표시 끄기
void ASH_Player::OffIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(false);
	}
}


//얼음 생성
void ASH_Player::IceMaker()
{
	iceArray[0]->SetActorLocation(Hitinfo.ImpactPoint);
	iceArray[0]->SetRotation(Hitinfo.ImpactNormal);
	iceArray[0]->SetActiveIce(true);
	iceArray.Swap(0, 1);
}

//얼음 제거
void ASH_Player::IceBrake()
{
	iceArray[iceArray.Find(hitIce)]->SetActiveIce(false);
}



//마그넷액터 표시하기
void ASH_Player::LookMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(true);
	}
}

//마그넷 액터 표시 끄기
void ASH_Player::OffMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(false);
	}
}

bool ASH_Player::FindOnMagnetActor()
{
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		if (magnetActorarr[i]->bMagnet) return true;
	}
	return false;
}

//마그넷 선택
void ASH_Player:: Magnet()
{
	if (hitMNActor != nullptr)
	{
		MagnetHandle->GrabComponentAtLocation(hitMNActor->InteractionMesh, FName(TEXT("None")), hitMNActor->GetActorLocation());
		MagnetGrabComp->SetWorldLocation(hitMNActor->GetActorLocation());
		hitMNActor->OnMagnet();
		GrabMagnetActor = hitMNActor;
		isGrab = true;
	}
}


