// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_SkillComponent.h"
#include "JS_Player.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputAction.h"
#include <CollisionQueryParams.h>
#include "JS_WidgetWeaponSwitch.h"
#include <Kismet/GameplayStatics.h>
#include "TimeLockBase.h"
#include "IceMakerBase.h"
#include "MagnetBase.h"
#include "SH_Ice.h"
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "JS_Bomb.h"
#include <MotionControllerComponent.h>
#include <Components/SphereComponent.h>
#include "SH_IceGhost.h"
#include <UMG/Public/Components/CanvasPanelSlot.h>
#include <Camera/CameraComponent.h>
#include "JS_WidgetSkillSwitch.h"
#include "PlayerMainWG.h"
#include <Components/SphereComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "NiagaraFunctionLibrary.h"


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
	ConstructorHelpers::FClassFinder<UJS_WidgetSkillSwitch>tempSkillWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/SwitchWeapon/JS_SwitchSkill.JS_SwitchSkill_c'"));
	if (tempSkillWidget.Succeeded())
	{
		skillUIFactory = tempSkillWidget.Class;

	}	
	ConstructorHelpers::FObjectFinder <UNiagaraSystem> tempNS(TEXT("/Script/Niagara.NiagaraSystem'/Game/FX/NS_CrossHair.NS_CrossHair'"));
	if (tempNS.Succeeded())
	{
		NSCrossHair = tempNS.Object;
	}

}


// Called when the game starts
void UJS_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	
	APlayerController* skillCon = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* skillSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(skillCon->GetLocalPlayer());
	skillSubsys->AddMappingContext(skillMapping,selectContext);
	
	//���� ����
	skillSubsys->AddMappingContext(SHMapping, selectContext);
	skillCon->PlayerCameraManager->ViewPitchMin = -80.0f;
	skillCon->PlayerCameraManager->ViewPitchMax = 30.0f;


	//��ų Comp �ʱ� ���� 
	Ghostice = GetWorld()->SpawnActor<ASH_IceGhost>(ASH_IceGhost::StaticClass());
	skillWidget = CreateWidget<UJS_WidgetSkillSwitch>(GetWorld(), skillUIFactory);
	AddArray();
	bSkillMenu = false;
}



void UJS_SkillComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (enhancedInputComponent != nullptr)
	{
		//���� ����
		enhancedInputComponent->BindAction(inputAction[0], ETriggerEvent::Started, this, &UJS_SkillComponent::OnButtonX);
		enhancedInputComponent->BindAction(inputAction[1], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnWS);
		enhancedInputComponent->BindAction(inputAction[2], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnAD);
		enhancedInputComponent->BindAction(inputAction[3], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnButtonA);
		enhancedInputComponent->BindAction(inputAction[4], ETriggerEvent::Triggered, this, &UJS_SkillComponent::LookUp);
		enhancedInputComponent->BindAction(inputAction[6], ETriggerEvent::Triggered, this, &UJS_SkillComponent::OnButtonY);
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
	//�÷��̾� �̵�(���� ����)
	player->dir = FTransform(player->GetControlRotation()).TransformVector(player->dir);
	player->AddMovementInput(player->dir.GetSafeNormal());
	player->dir = FVector::ZeroVector;

	//X/GŰ �������� ����Ʈ���̽� Ȱ��ȭ
	if (isPressedG)
	{
		LineTraceInteration();
	}

	//Magnet ������� 
	if (isGrab)
	{
		ratio += DeltaTime * 2;
		ratio = FMath::Clamp(ratio, 0.0 ,1.0);
		FVector magpos = FMath::Lerp(player->MagNS ->GetComponentLocation(), GrabMagnetActor->GetActorLocation(), ratio);
		player->MagNS->SetNiagaraVariableVec3(TEXT("EndLoc"),magpos);
		if (ratio >= 1)
		{
			ratio = 1;
			player->MagnetHandle->SetTargetLocation(player->MagnetGrabComp->GetComponentLocation());
		}
	}
}
	
// Ű���� ������ ���ε� (���� ����)
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
	player->AddControllerPitchInput(MouseAxis.Y);
}

// ���� �̵� ���ε�
void UJS_SkillComponent::OnGrabRight()
{
	SkilMenuMove(-1);
}
void UJS_SkillComponent::OnGrabLeft()
{
	SkilMenuMove(1);
}
// �����ѱ� ���ε�
void UJS_SkillComponent::OnButtonTrigger()
{
	OnSkillUI();
}
//��ų ��� ���ε�
void UJS_SkillComponent::OnButtonA(const FInputActionValue& value)
{
	if (OnTimeLockActor != nullptr)
	{
		
		OnTimeLockActor->impulseArrowUpdate();

	}
	if (bSkillMenu)
	{

		ChangeSkill();

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
			if (hitIMActor != nullptr && Ghostice->bSpawn)
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
					FVector dir = player->GetActorLocation() - hitMNActor->GetActorLocation();
					if (dir.Length() < MagnetLength)
					{
						Magnet();
					}
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
//��ų Ȱ��ȭ ���ε�
void UJS_SkillComponent::OnButtonX()
{
	player->compAttack->currAttackState = EAttackState::AttackIdle;
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
//��ų ��Ȱ��ȭ ���ε�
void UJS_SkillComponent::OnButtonY()
{
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
void UJS_SkillComponent::ReleaseButtonA()
{
	bIsReady = false;
}
// ��ų���� ����
void UJS_SkillComponent::ChangeSkill()
{
	// ������ X�࿡ ���� ��ų �ٲ�
	float targetSkill = skillWidget->slotPos->GetPosition().X;
	if (targetSkill == 0)
	{
		currSkillState = ESkillState::Margnet;
		OffIceMaker();
		OffTimeLock();
		OffBomb();

	}
	else if (targetSkill == -120.f)
	{
		currSkillState = ESkillState::TimeLock;
		OffIceMaker();
		OffMagnet();
		OffBomb();

	}
	else if (targetSkill == -240.f)
	{
		currSkillState = ESkillState::IceMaker;
		OffTimeLock();
		OffMagnet();
		OffBomb();

	}
	else if (targetSkill == 120.f)
	{
		currSkillState = ESkillState::Bomb;
		OffIceMaker();
		OffTimeLock();
		OffMagnet();
	}
	else
	{
		currSkillState = ESkillState::Defalt;
		OffIceMaker();
		OffTimeLock();
		OffMagnet();

		OffBomb();

	}

	isPressedG = false;
	hitMNActor = nullptr;
	hitTLActor = nullptr;
	hitIce = nullptr;
	// UI ��
	SkillMenuOnOff(false);
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

//����Ʈ���̽�
void UJS_SkillComponent::LineTraceInteration()
{
	FVector Startpos;
	FVector Endpos;
	if (DebagKeyBorad)
	{
		Startpos = player->compCam->GetComponentLocation();
		Endpos = Startpos + player->compCam->GetForwardVector() * 5000;
	}
	else
	{
		Startpos = player->leftController->GetComponentLocation();
		Endpos = Startpos + (player->leftController->GetForwardVector()+ player->leftController->GetUpVector()* -1) * 5000;
	}
	FCollisionQueryParams par;
	par.AddIgnoredActor(GetOwner());
	par.AddIgnoredActor(Ghostice);
	bool bhit = GetWorld()->LineTraceSingleByChannel(Hitinfo, Startpos, Endpos, ECC_Visibility, par);
	if (bhit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NSCrossHair, Hitinfo.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(Hitinfo.ImpactNormal));
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
			Ghostice->SetActorLocation(Hitinfo.ImpactPoint);
			Ghostice->SetRotation(Hitinfo.ImpactNormal);
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
		OnTimeLockActor = hitTLActor;
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
	Ghostice->SetVisibleIce(true);
	if (iceMakerActorarr.IsEmpty()) return;
	for (int32 i = 0; i < iceMakerActorarr.Num(); i++)
	{
		iceMakerActorarr[i]->InteractionIceMaker(true);
	}
}
//���̽�����Ŀ ǥ�� ����
void UJS_SkillComponent::OffIceMaker()
{
	Ghostice->SetVisibleIce(false);
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
	player->MagnetHandle->GrabComponentAtLocation(hitMNActor->InteractionMesh, FName(TEXT("None")), hitMNActor->GetActorLocation());
	player->MagnetGrabComp->SetWorldLocation(hitMNActor->GetActorLocation());
	hitMNActor->OnMagnet(Hitinfo.ImpactPoint);
	GrabMagnetActor = hitMNActor;
	player->MagNS->SetVisibility(true);
	ratio = 0;
	isGrab = true;
}


// UI
void UJS_SkillComponent::OnSkillUI() 
{
	player->compAttack->WeaponMenuOnOff(false);
	
	if (!bSkillMenu) //�޴��� �ȿ��� ������ (!bSwitch)
	{
		// ����Ʈ�� UI ����
		SkillMenuOnOff(true);
		//  ���¿� ���� MovePanel x�� �ʱ� ��ġ�� �����Ѵ�.
		skillWidget->SetUIInitPos((int32)(currSkillState));
	}
	else	// ���� �޴��� ���������� (bSwitch)
	{
		// ����Ʈ���� UI���� (���)
		SkillMenuOnOff(false);
	}

}
void UJS_SkillComponent::SkillMenuOnOff(bool value)
{
	if (value)
	{
		player->ovelayMenuMainWG(skillWidget);
	}
	else
	{
		skillWidget->RemoveFromParent();

	}
	bSkillMenu = value;
}
void UJS_SkillComponent::SkilMenuMove(int32 value)
{
	if (bSkillMenu)
	{
		skillWidget->MoveUI(value);
	}
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
		//DrawDebugLine(GetWorld(), player->rightController->GetComponentLocation(), player->rightController->GetComponentLocation() + throwDirection * 50, FColor::Red, false, 5, 0, 3);
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

