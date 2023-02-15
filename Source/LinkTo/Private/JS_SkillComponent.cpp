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
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>




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

	bSkillMenu = false;
	
}

void UJS_SkillComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(inputAction[0], ETriggerEvent::Started, this, &UJS_SkillComponent::OnG);
		enhancedInputComponent->BindAction(inputAction[1], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnWS);
		enhancedInputComponent->BindAction(inputAction[2], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnAD);
		enhancedInputComponent->BindAction(inputAction[3], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnButtonA);
		enhancedInputComponent->BindAction(inputAction[4], ETriggerEvent::Triggered, this, &UJS_SkillComponent::LookUp);
		enhancedInputComponent->BindAction(inputAction[6], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnF);
		enhancedInputComponent->BindAction(inputAction[7], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonTrigger);
		enhancedInputComponent->BindAction(inputAction[8], ETriggerEvent::Started, this, &UJS_SkillComponent::OnGrabRight);
		enhancedInputComponent->BindAction(inputAction[9], ETriggerEvent::Started, this, &UJS_SkillComponent::OnGrabLeft);

		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonA);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UJS_SkillComponent::OnGrabRight);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UJS_SkillComponent::OnGrabLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonTrigger);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonX);
		enhancedInputComponent->BindAction(leftInputs[4], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonY);
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
		ratio += DeltaTime * 2;
		ratio = FMath::Clamp(ratio, 0.0 ,1.0);
		FVector magpos = FMath::Lerp(player->GetActorLocation(), GrabMagnetActor->GetActorLocation(), ratio);
		player->MagNS->SetNiagaraVariableVec3(TEXT("EndLoc"),magpos);
		if (ratio >= 1)
		{
			ratio = 1;
			player->MagnetHandle->SetTargetLocation(player->MagnetGrabComp->GetComponentLocation());
		}
	}
	
	
	UE_LOG(LogTemp,Warning,TEXT("%d"),bSkillMenu)
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
	switch (currSkillState)
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
	switch (currSkillState)
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
	//DrawDebugLine(GetWorld(), Startpos, Endpos, LineColor, false, 1, 0, 1);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit)
	{
		switch (currSkillState)
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
	
	if (iceArray[iceNum]->isIceVisible())
	{
		iceNum = ++iceNum % iceArray.Num();
	}
	iceArray[iceNum]->SetActorLocation(Hitinfo.ImpactPoint);
	iceArray[iceNum]->SetRotation(Hitinfo.ImpactNormal);
	iceArray[iceNum]->SetActiveIce(true);
	
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
		player->MagNS->SetVisibility(true);
		ratio = 0;
		isGrab = true;
	}
}

void UJS_SkillComponent::OnButtonTrigger() // 메뉴버튼 눌렀을시
{

	if (player->compAttack->bWeaponMenu) // 무기메뉴가 열려있으면
	{
		player->weaponWidget->RemoveFromParent(); // 무기메뉴를 닫고
		player->compAttack->bWeaponMenu = false;
	}
	OnSkillUI();
}

void UJS_SkillComponent::OnSkillUI() // UI열고 닫는 함수
{
	if (player->compAttack->bWeaponMenu)
	{
		player->weaponWidget->RemoveFromParent();
		player->compAttack->bWeaponMenu = false;
	}
	else
	{
		if (!bSkillMenu) //메뉴가 안열려 있을때 (!bSwitch)
		{
			// 뷰포트에 UI 띄우기
			player->skillWidget->AddToViewport();
			//  상태에 따라 MovePanel x의 초기 위치를 세팅한다.
			player->skillWidget->SetUIInitPos((int32)(currSkillState));
		}
		else	// 현재 메뉴가 열려있을때 (bSwitch)
		{
			// 뷰포트에서 UI제거 (취소)
			player->skillWidget->RemoveFromParent();
		}

		bSkillMenu = !bSkillMenu;
	}
		
}

void UJS_SkillComponent::OnButtonA(const FInputActionValue& value)
{
	if (bSkillMenu)
	{
		if (player->compAttack->bWeaponMenu == false)
		{

			ChangeSkill();

		}
	}
	else
	{
		switch (currSkillState)
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
					player->MagNS->SetVisibility(false);
					isGrab = false;
					GrabMagnetActor = nullptr;
					hitMNActor = nullptr;
				}
				isClickedLMouse = true;
			}
			break;
		}
	}

}

void UJS_SkillComponent::OnButtonX()
{
	player->compAttack->currAttackState = EAttackState::AttackIdle;
	player->OnLogRight("OnButtonX");
	isPressedG = true;
	switch (currSkillState)
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

void UJS_SkillComponent::OnButtonY()
{
	player->OnLogRight("OnButtonY");
	switch (currSkillState)
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

void UJS_SkillComponent::ChangeSkill() // 스킬상태 바꾸는 함수
{
	// 위젯의 X축에 따라 스킬 바꿈
	float targetSkill = player->skillWidget->slotPos->GetPosition().X;
	if (targetSkill == 0)
	{
		currSkillState = ESkillState::Margnet;
		OffIceMaker();
		OffTimeLock();
		isPressedG = false;
	}
	else if (targetSkill == -350.f)
	{
		currSkillState = ESkillState::TimeLock;
		OffIceMaker();
		OffMagnet();
		isPressedG = false;
	}
	else if (targetSkill == -700.f)
	{
		currSkillState = ESkillState::IceMaker;
		OffTimeLock();
		OffMagnet();
		isPressedG = false;
	}
	else if (targetSkill == 350.f)
	{
		currSkillState = ESkillState::Boomb;
		OffIceMaker();
		OffTimeLock();
		OffMagnet();
		isPressedG = false;
	}
	else 
	{
		currSkillState = ESkillState::Defalt;
		OffIceMaker();
		OffTimeLock();
		OffMagnet();
		isPressedG = false;
	}

	hitMNActor = nullptr;
	hitTLActor = nullptr;
	hitIce = nullptr;
	// UI 끔
	player->skillWidget->RemoveFromParent();
	bSkillMenu = false;
}
// UI 열려있을때 아이콘 이동시키는 함수
void UJS_SkillComponent::OnGrabRight()
{
	if (bSkillMenu)
		player->skillWidget->MoveUI(-1);
}

void UJS_SkillComponent::OnGrabLeft()
{
	if (bSkillMenu)
		player->skillWidget->MoveUI(1);
}
