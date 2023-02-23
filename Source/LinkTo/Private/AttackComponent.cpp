// Fill out your copyright notice in the Description page of Project Settings.


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
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerStateComponent.h"
#include "JS_WidgetWeaponSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include "JS_Arrow.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Components/PrimitiveComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "TimeLockBase.h"
#include <MotionControllerComponent.h>
#include "JS_LinkSound.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

 	ConstructorHelpers::FClassFinder<AJS_Arrow>tempArrow(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Weapons/BP_Arrow.BP_Arrow_C'"));
 	if (tempArrow.Succeeded())
 	{
 		arrowFactory = tempArrow.Class;
 	}
	// ���� ����ġ UI ã�ƿ���

	ConstructorHelpers::FClassFinder<UJS_WidgetWeaponSwitch> tempWeaponWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/SwitchWeapon/JS_SwitchWeapon.JS_SwitchWeapon_C'"));
	if (tempWeaponWidget.Succeeded())
	{
		weaponUIFactory = tempWeaponWidget.Class;
	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	weaponWidget = CreateWidget<UJS_WidgetWeaponSwitch>(GetWorld(), weaponUIFactory);

	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	attackSubsys->AddMappingContext(attackMapping, 0);

	// ���� ���� �޴� �ʱ�ȭ
	bWeaponMenu = false;

	sounds = player->compSound;

}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

	GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow, FString::Printf(TEXT("%d"),(int32)(currAttackState)));

	switch (currAttackState)
	{
	case EAttackState::AttackIdle:
		IdleState();
		break;
	case EAttackState::AttackSword:
	{
		SwordState();
		
		if (player->compMove->bMove == false)
		{
			currHandPos = player->rightHand->GetComponentLocation();
		// �ֵθ��� ����
		FVector attackDir = oldPos - currHandPos;
		// �ֵθ��� �ӷ�(����)
		float swordSpeed = UKismetMathLibrary::Square(attackDir.Size());
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%f"),swordSpeed), false, FVector2D(3.f));


		
			if (swordSpeed >= 15.f && !bHit)
			{
				SwordLineTrace();
				sounds->PlayAttackSwordSound();
			}
			
		
		
		
		// ���� ƽ���� ����� ���� ��ġ
		oldPos = currHandPos;
	}
		}
		
		break;
	case EAttackState::AttackBow:
		BowState();
		break;
	}

}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &UAttackComponent::OnButtonMenu);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA); 
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Triggered, this, &UAttackComponent::FireSword); 
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabRight);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabLeft);
// 	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerArrow);
// 	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseArrow);
}

void UAttackComponent::OnButtonA()
{
	if (!bWeaponMenu) // ���� �޴��� �ȿ��� ������
	{
		switch (currAttackState) // ���¿� ���� �ൿ
		{
		case EAttackState::AttackIdle:
			break;
		case EAttackState::AttackSword:
			break;
		case EAttackState::AttackBow:
			break;
		}
	}
	else // �޴��� ���������� 
	{
		ChangeWeapon();
	}
}
// ���� �ٲٴ� �Լ�
void UAttackComponent::ChangeWeapon() 
{
	// ������ X�࿡ ���� ���� �ٲ�
	float targetWeapon = weaponWidget->slotPos->GetPosition().X;
	if (targetWeapon == 0)
	{
		currAttackState = EAttackState::AttackSword;
	}
	else if (targetWeapon == -120.f)
	{
		currAttackState = EAttackState::AttackBow;
	}
	else
		currAttackState = EAttackState::AttackIdle;

	// UI ��
	WeaponMenuOnOff(false);
}
// �⺻ �����϶�
void UAttackComponent::IdleState() 
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(false);
}
// �޴���ư ��������
void UAttackComponent::OnButtonMenu() 
{	
	player->compSkill->SkillMenuOnOff(false);
	OnWeaponUI();
}

//������ �׷���ư ��������
void UAttackComponent::OnGrabRight()
{
	WeaponMenuMove(-1);
}
// ���� �׷� ��ư ������ ��
void UAttackComponent::OnGrabLeft()
{

	WeaponMenuMove(1);
}
// ���� �����϶�
void UAttackComponent::SwordState() 
{
	player->compSword->SetVisibility(true);
	player->compBow->SetVisibility(false);
}
// Ȱ �����϶�
void UAttackComponent::BowState() 
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(true);
}




void UAttackComponent::SwordLineTrace()
{
	if (!bHit)
	{
		bHit = true;
		FTimerHandle AttackHandle;

		GetWorld()->GetTimerManager().ClearTimer(AttackHandle);
		GetWorld()->GetTimerManager().SetTimer(AttackHandle, this, &UAttackComponent::SetLineTrace, 0.2f, false);
		// ����Ʈ���̽� �߻�
		FHitResult hitInfo;
		FVector startPos = player->compSword->GetComponentLocation() + FVector(0, 0, 15);
		FVector endPos = (startPos + (player->compSword->GetForwardVector() * -1) * 100) + FVector(0, 0, 15);
		FCollisionShape shape = FCollisionShape::MakeSphere(25.f);
	
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner());

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, false, 0.1f, 0, 1.f);
		
/*		bool isTrace = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);*/
	
		// ���� ���� ���Ͱ� Ÿ�Ӷ��� �ɷ�������
		if (GetWorld()->SweepSingleByChannel(hitInfo,startPos,endPos,FQuat::Identity,ECC_Visibility,shape,params))
		{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Ball")))
			{
				timeLockActor = Cast<ATimeLockBase>(hitInfo.GetActor());
				if (timeLockActor->bTimeLock)
				{
					GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("hit")), false, FVector2D(3.f));
					timeLockActor->impulseArrowUpdate();
				}
			}
		}
	}
}

void UAttackComponent::SetLineTrace()
{
	bHit = false;
}

// Į ����
void UAttackComponent::FireSword() 
{


}
// ȭ�� ����
void UAttackComponent::OnTriggerArrow() 
{ 
	// ���� ���°� Bow�϶� (Bow�� ���������)
	if (currAttackState == EAttackState::AttackBow)
	{
		// ������ �ð���ŭ ������ �Ѵ� (ȭ�쿡 ���� �����ش�)
		accArrowSpeed += 1000*GetWorld()->DeltaTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"), accArrowSpeed);
	}
}
// ȭ�� �߻�
void UAttackComponent::OnReleaseArrow()
{
	if (currAttackState == EAttackState::AttackBow)
	{
		AJS_Arrow* spawnedArrow = GetWorld()->SpawnActor<AJS_Arrow>(arrowFactory, player->GetActorLocation(), player->GetActorRotation());
		spawnedArrow->FireInDirection(spawnedArrow->GetActorForwardVector());
		accArrowSpeed = 0; // ���ӵ� 0���� �ʱ�ȭ
	}

}



// UI���� �ݴ� �Լ�
void UAttackComponent::OnWeaponUI()
{

	player->compSkill->SkillMenuOnOff(false);

	if (!bWeaponMenu) //�޴��� �ȿ��� ������ (!bSwitch)
	{
		WeaponMenuOnOff(true);
	}
	else 	// ���� �޴��� ���������� (bSwitch)
	{
		// ����Ʈ���� UI���� (���)
		WeaponMenuOnOff(false);
	}
}
//���� UI ���� �ݴ� �Լ�
void UAttackComponent::WeaponMenuOnOff(bool value)
{
	bWeaponMenu = value;
	if (value)
	{
		player->ovelayMenuMainWG(weaponWidget);
		sounds->PlayOpenStoneSound();
		weaponWidget->SetUIInitPos((int32)(currAttackState));
	}
	else
	{
		weaponWidget->RemoveFromParent();
		sounds->PlayCloseStoneSound();
	}
	
}
// UI ���������� ������ �̵���Ű�� �Լ�
void UAttackComponent::WeaponMenuMove(int32 value)
{
	if (bWeaponMenu)
	{
		weaponWidget->MoveUI(value);
	}
}
