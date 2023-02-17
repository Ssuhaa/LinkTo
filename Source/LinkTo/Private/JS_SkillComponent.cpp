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
#include "JS_Bomb.h"
#include <MotionControllerComponent.h>
#include <Components/SphereComponent.h>




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
	ConstructorHelpers::FClassFinder <AJS_Bomb> tempBomb(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Actors/Weapon/JS_BP_Bomb.JS_BP_Bomb_c'"));
	if (tempBomb.Succeeded())
	{
		bombFactory = tempBomb.Class;
	}
}


// Called when the game starts
void UJS_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	
	APlayerController* skillCon = GetWorld()->GetFirstPlayerController();
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* skillSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(skillCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
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
		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Completed, this, &UJS_SkillComponent::ReleaseButtonA);
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
	
	// ���׳� �ڵ�
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
	
// Ű���� 
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
	case ESkillState::Bomb:
		OnBomb();
		break;
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
	case ESkillState::Bomb:
		OnBomb();
		break;
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



//interaction �迭�� Actor�� �߰��ϱ�
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

	for (int32 i = 0; i < 2; i++)
	{
		AJS_Bomb* currBomb = GetWorld()->SpawnActor<AJS_Bomb>(bombFactory);
		currBomb->SetActiveBomb(false);
		bombArray.Add(currBomb);
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


//Ÿ�Ӷ� ���� ����
void UJS_SkillComponent::LookTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(true);
	}
}

//Ÿ�Ӷ� ǥ�� ����
void UJS_SkillComponent::OffTimeLock()
{
	if (timelockActorarr.IsEmpty()) return;
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		timelockActorarr[i]->InteractionTimeLock(false);
	}
}

//Ÿ�Ӷ� �ɱ�
void UJS_SkillComponent::TimeLock()
{
	if (!FindOnTimeLockActor())
	{
		hitTLActor->OnTimeLock();
		OffTimeLock();
		isPressedG = false;
	}
}

//Ÿ�Ӷ� �ɸ� ���Ͱ� �ִ��� ã��
bool UJS_SkillComponent::FindOnTimeLockActor()
{
	for (int32 i = 0; i < timelockActorarr.Num(); i++)
	{
		if (timelockActorarr[i]->bTimeLock) return true;
	}
	return false;
}



//���̽�����Ŀ ���� ����
void UJS_SkillComponent::LookIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(true);
	}
}


//���̽�����Ŀ ǥ�� ����
void UJS_SkillComponent::OffIceMaker()
{
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(false);
	}
}


//���� ����
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

//���� ����
void UJS_SkillComponent::IceBrake()
{
	iceArray[iceArray.Find(hitIce)]->SetActiveIce(false);
}



//���׳ݾ��� ǥ���ϱ�
void UJS_SkillComponent::LookMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(true);
	}
}

//���׳� ���� ǥ�� ����
void UJS_SkillComponent::OffMagnet()
{
	if (magnetActorarr.IsEmpty()) return;
	for (int32 i = 0; i < magnetActorarr.Num(); i++)
	{
		magnetActorarr[i]->InteractionMagnet(false);
	}

}

//���׳� ����
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

void UJS_SkillComponent::OnButtonTrigger() // �޴���ư ��������
{

	if (player->compAttack->bWeaponMenu) // ����޴��� ����������
	{
		player->weaponWidget->RemoveFromParent(); // ����޴��� �ݰ�
		player->compAttack->bWeaponMenu = false;
	}
	OnSkillUI();
}

void UJS_SkillComponent::OnSkillUI() // UI���� �ݴ� �Լ�
{
	if (player->compAttack->bWeaponMenu)
	{
		player->weaponWidget->RemoveFromParent();
		player->compAttack->bWeaponMenu = false;
	}
	else
	{
		if (!bSkillMenu) //�޴��� �ȿ��� ������ (!bSwitch)
		{
			// ����Ʈ�� UI ����
			player->skillWidget->AddToViewport();
			//  ���¿� ���� MovePanel x�� �ʱ� ��ġ�� �����Ѵ�.
			player->skillWidget->SetUIInitPos((int32)(currSkillState));
		}
		else	// ���� �޴��� ���������� (bSwitch)
		{
			// ����Ʈ���� UI���� (���)
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
		case ESkillState::Bomb:
			bIsReady = true;
			break;
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

void UJS_SkillComponent::ReleaseButtonA()
{
	bIsReady = false;
}

void UJS_SkillComponent::OnButtonX()
{
	player->compAttack->currAttackState = EAttackState::AttackIdle;
	player->OnLogRight("OnButtonX");
	isPressedG = true;
	switch (currSkillState)
	{
	case ESkillState::Bomb:
		OnBomb();
		break;
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
	case ESkillState::Bomb:
		OffBomb();
		break;
			
	}
	isPressedG = false;
}

void UJS_SkillComponent::ChangeSkill() // ��ų���� �ٲٴ� �Լ�
{
	// ������ X�࿡ ���� ��ų �ٲ�
	float targetSkill = player->skillWidget->slotPos->GetPosition().X;
	if (targetSkill == 0)
	{
		currSkillState = ESkillState::Margnet;
		OffIceMaker();
		OffTimeLock();
		OffBomb();
		isPressedG = false;
	}
	else if (targetSkill == -350.f)
	{
		currSkillState = ESkillState::TimeLock;
		OffIceMaker();
		OffMagnet();
		OffBomb();
		isPressedG = false;
	}
	else if (targetSkill == -700.f)
	{
		currSkillState = ESkillState::IceMaker;
		OffTimeLock();
		OffMagnet();
		OffBomb();
		isPressedG = false;
	}
	else if (targetSkill == 350.f)
	{
		currSkillState = ESkillState::Bomb;
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
		OffBomb();
		isPressedG = false;
	}

	hitMNActor = nullptr;
	hitTLActor = nullptr;
	hitIce = nullptr;
	// UI ��
	player->skillWidget->RemoveFromParent();
	bSkillMenu = false;
}
// UI ���������� ������ �̵���Ű�� �Լ�
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

void UJS_SkillComponent::OffBomb()
{
	if (bombArray.IsEmpty()) return;
	for (int32 i = 0; i < bombArray.Num(); i++)
	{
		bombArray[i]->SetActiveBomb(false);
	}
}

void UJS_SkillComponent::OnBomb()
{
	if (player->compAttack->currAttackState != EAttackState::AttackIdle) // �տ� ���⸦ ���������
	{
		player->compAttack->currAttackState = EAttackState::AttackIdle; // �������

		// ��ź�� �տ� ����
		bombArray[0]->SetActiveBomb(true);
		bombArray[0]->AttachToComponent(player->rightHand,FAttachmentTransformRules::KeepWorldTransform, FName("RightHandSocket"));
		bGrabBomb = true;
		grabbedBomb = Cast<AJS_Bomb>(bombArray[0]);
	}
}

void UJS_SkillComponent::ReadyToThrowBomb()
{
	if (!bIsReady)
	{
		//DrawGrabRange();
		prevLocation = player->rightController->GetComponentLocation();
		prevForward = player->rightController->GetForwardVector();
		UE_LOG(LogTemp, Log, TEXT("X Press!!!!!"));
	}
	else
	{
		// ���� ����
		throwDirection = player->rightController->GetComponentLocation() - prevLocation;

		// ȸ����
		FVector rotAxis = FVector::CrossProduct(prevForward, player->rightController->GetForwardVector());
		float angle = FMath::Acos(FVector::DotProduct(prevForward, player->rightController->GetForwardVector()));
		angle = FMath::RadiansToDegrees(angle);

		ReleaseBomb(player->rightHand, rotAxis * angle);

		UE_LOG(LogTemp, Log, TEXT("X Release!!!!!"));
		DrawDebugLine(GetWorld(), player->rightController->GetComponentLocation(), player->rightController->GetComponentLocation() + throwDirection * 50, FColor::Red, false, 5, 0, 3);
	}
	bIsReady = !bIsReady;
}

void UJS_SkillComponent::ReleaseBomb(USkeletalMeshComponent* selectHand, FVector torque)
{
	// ��� �ִ� ��ü�� �����.
	grabbedBomb->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// ��ü�� ���� ������ on/off ���θ� �ǵ����ش�.
	USphereComponent* compSphere = Cast<USphereComponent>(grabbedBomb->compSphere);
	if (compSphere != nullptr)
	{
		compSphere->SetSimulatePhysics(true);
	}
	throwDirection.Normalize();
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), throwDirection.X, throwDirection.Y, throwDirection.Z));
	UE_LOG(LogTemp, Log, TEXT("%.3f, %.3f, %.3f"), throwDirection.X, throwDirection.Y, throwDirection.Z);
	// ���� ������ ����� ���Ѵ�.
	compSphere->AddImpulse(throwDirection * throwPower);
	compSphere->AddTorqueInDegrees(torque * torquePower, NAME_None, true);
	bGrabBomb = false;
}
