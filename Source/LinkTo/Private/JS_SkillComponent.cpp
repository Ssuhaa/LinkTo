// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_SkillComponent.h"
#include "AttackComponent.h"
#include "MoveComponent.h"
#include "JS_Player.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <CollisionQueryParams.h>
#include "PlayerStateComponent.h"
#include "JS_WidgetWeaponSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include "JS_Arrow.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "TimeLockBase.h"
#include "IceMakerBase.h"
#include "MagnetBase.h"
#include "SH_Ice.h"
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include <Math/MathFwd.h>


// Sets default values for this component's properties
UJS_SkillComponent::UJS_SkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder <ASH_Ice> tempice(TEXT("/Script/CoreUObject.Class'/Script/LinkTo.SH_Ice'"));
	if (tempice.Succeeded())
	{
		iceFactory = tempice.Class;
	}

	// ...
}


// Called when the game starts
void UJS_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	APlayerController* skillCon = GetWorld()->GetFirstPlayerController();
	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* skillSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(skillCon->GetLocalPlayer());
	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
	skillSubsys->AddMappingContext(SHMapping, selectContext);
	skillSubsys->AddMappingContext(skillMapping,selectContext);
	// ...
	
	AddArray();

	skillCon->PlayerCameraManager->ViewPitchMin = -80.0f;
	skillCon->PlayerCameraManager->ViewPitchMax = 30.0f;
	
}

void UJS_SkillComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(inputAction[0], ETriggerEvent::Started, this, &UJS_SkillComponent::OnG);
		enhancedInputComponent->BindAction(inputAction[1], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnWS);
		enhancedInputComponent->BindAction(inputAction[2], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnAD);
		enhancedInputComponent->BindAction(inputAction[3], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnLeftMouse);
		enhancedInputComponent->BindAction(inputAction[4], ETriggerEvent::Triggered, this, &UJS_SkillComponent::LookUp);
		enhancedInputComponent->BindAction(inputAction[6], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnF);
	}

	

}


// Called every frame
void UJS_SkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 마그넷 핸들
	player->dir = FTransform(player->GetControlRotation()).TransformVector(player->dir);
	player->AddMovementInput(player->dir.GetSafeNormal());
	player->dir = FVector::ZeroVector;

	if (isPressedG)
	{
		LineTraceInteration();
	}

	if (isGrab)
	{
		player->MagnetHandle->SetTargetLocation(player->MagnetGrabComp->GetComponentLocation());
	}


}
	
// 키보드 
void UJS_SkillComponent::OnWS(const FInputActionValue& value)
{
	float Axis = value.Get<float>();
	player->dir.X += Axis;
}
void UJS_SkillComponent::OnAD(const  FInputActionValue& value)
{
	float Axis = value.Get<float>();
	player->dir.Y += Axis;
}
void UJS_SkillComponent::LookUp(const FInputActionValue& value)
{
	FVector2D MouseAxis = value.Get<FVector2D>();

	player->AddControllerYawInput(MouseAxis.X);
	player->AddControllerPitchInput(-MouseAxis.Y);
}

void UJS_SkillComponent::OnG(const FInputActionValue& value)
{
	player->OnLogRight("OnG");
	isPressedG = true;
	switch (PlayerSkillState)
	{
	case ESkillState::TimeLock:
		LookTimeLock();
		LineColor = FColor::Yellow;
		break;
	case ESkillState::IceMaker:
		LookIceMaker();
		LineColor = FColor::Blue;
		break;
	case ESkillState::Margnet:
		LookMagnet();
		LineColor = FColor::Red;
		break;
	}
}

void UJS_SkillComponent::OnF(const struct FInputActionValue& value)
{
	player->OnLogRight("OnF");
	switch (PlayerSkillState)
	{
	case ESkillState::TimeLock:
		OffTimeLock();
		break;
	case ESkillState::IceMaker:
		OffIceMaker();
		break;
	case ESkillState::Margnet:
		OffMagnet();
		break;
	}
	isPressedG = false;
}

void UJS_SkillComponent::OnLeftMouse(const FInputActionValue& value) // 인식 버튼
{
	player->OnLogRight("OnLeftMouse");
	switch (PlayerSkillState)
	{
	case ESkillState::TimeLock:
		if (hitTLActor != nullptr)
		{
			TimeLock();
		}
		break;
	case ESkillState::IceMaker:
		if (hitIMActor != nullptr)
		{
			IceMaker();
		}
		else if (hitIce != nullptr)
		{
			IceBrake();
		}
		break;
	case ESkillState::Margnet:
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
				player->MagnetHandle->ReleaseComponent();
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
void UJS_SkillComponent::AddArray()
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

void UJS_SkillComponent::LineTraceInteration()
{
	
	FVector Startpos = player->compCam->GetComponentLocation();
	FVector Endpos = Startpos + player->compCam->GetForwardVector() * 5000;
	FCollisionQueryParams par;
	par.AddIgnoredActor(GetOwner());
	DrawDebugLine(GetWorld(), Startpos, Endpos, LineColor, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit)
	{
		switch (PlayerSkillState)
		{
		case ESkillState::TimeLock:
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

		case ESkillState::IceMaker:
			hitIMActor = Cast<AIceMakerBase>(Hitinfo.GetActor());
			hitIce = Cast<ASH_Ice>(Hitinfo.GetActor());
			break;

		case ESkillState::Margnet:
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
void UJS_SkillComponent::LookTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(true);
	}
}

//타임락 표시 끄기
void UJS_SkillComponent::OffTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(false);
	}
}

//타임락 걸기
void UJS_SkillComponent::TimeLock()
{
	if (!FindOnTimeLockActor())
	{
		hitTLActor->OnTimeLock();
		OffTimeLock();
		isPressedG = false;
	}
}

//타임락 걸린 액터가 있는지 찾기
bool UJS_SkillComponent::FindOnTimeLockActor()
{
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		if (timelockActorarr[i]->bTimeLock) return true;
	}
	return false;
}



//아이스메이커 액터 보기
void UJS_SkillComponent::LookIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(true);
	}
}


//아이스메이커 표시 끄기
void UJS_SkillComponent::OffIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(false);
	}
}


//얼음 생성
void UJS_SkillComponent::IceMaker()
{
	iceArray[0]->SetActorLocation(Hitinfo.ImpactPoint);
	iceArray[0]->SetRotation(Hitinfo.ImpactNormal);
	iceArray[0]->SetActiveIce(true);
	iceArray.Swap(0, 1);
}

//얼음 제거
void UJS_SkillComponent::IceBrake()
{
	iceArray[iceArray.Find(hitIce)]->SetActiveIce(false);
}



//마그넷액터 표시하기
void UJS_SkillComponent::LookMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(true);
	}
}

//마그넷 액터 표시 끄기
void UJS_SkillComponent::OffMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(false);
	}
}

//마그넷 선택
void UJS_SkillComponent::Magnet()
{
	if (hitMNActor != nullptr)
	{
		player->MagnetHandle->GrabComponentAtLocation(hitMNActor->InteractionMesh, FName(TEXT("None")), hitMNActor->GetActorLocation());
		player->MagnetGrabComp->SetWorldLocation(hitMNActor->GetActorLocation());
		hitMNActor->OnMagnet();
		GrabMagnetActor = hitMNActor;
		isGrab = true;
	}
}
