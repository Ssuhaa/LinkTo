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
//	weaponWidget = CreateWidget<UJS_WidgetWeaponSwitch>(GetWorld(), weaponUIFactory);

	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	attackSubsys->AddMappingContext(attackMapping, 0);

	// ���� ���� �޴� �ʱ�ȭ
	bWeaponMenu = false;

}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

	/*GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow, FString::Printf(TEXT("%d"),(int32)(currAttackState)));*/

	switch (currAttackState)
	{
	case EAttackState::AttackIdle:
		IdleState();
		break;
	case EAttackState::AttackSword:
		SwordState();
		break;
	case EAttackState::AttackBow:
		BowState();
		break;
	}

}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[5], ETriggerEvent::Started, this, &UAttackComponent::OnButtonMenu);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA); 
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabRight);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabLeft);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerArrow);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseArrow);
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
			FireSword(); // Į ����
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
	else if (targetWeapon == -350.f)
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
	if (value)
	{
		player->ovelayMenuMainWG(weaponWidget);
		weaponWidget->SetUIInitPos((int32)(currAttackState));
	}
	else
	{
		weaponWidget->RemoveFromParent();

	}
	bWeaponMenu = value;
}
// UI ���������� ������ �̵���Ű�� �Լ�
void UAttackComponent::WeaponMenuMove(int32 value)
{
	if (bWeaponMenu)
	{
		weaponWidget->MoveUI(value);
	}
}
