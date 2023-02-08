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

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	
	

}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	
	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
	attackSubsys->AddMappingContext(attackMapping, 0);
	// ...
	bWeaponMenu = false;
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
		SwordState();
		break;
	case EAttackState::AttackBow:
		BowState();
		break;
	}
	// ...
}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[5], ETriggerEvent::Started, this, &UAttackComponent::OnButtonMenu);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA); 
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UAttackComponent::OnThumbstickLeft);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UAttackComponent::OnThumbstickLeft);
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabRight);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabLeft);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerRight);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseRight);
}

void UAttackComponent::OnButtonA()
{
	if(!bWeaponMenu) // ���� �޴��� �ȿ��� ������
	switch ((int32)(currAttackState))
	{
		case 0:
		break;
		case 1:
		FireSword(); // Į ����
		break;
		case 2:
		break;
	}
	else // �޴��� ���������� 
	{
		ChangeWeapon();
	}
}

void UAttackComponent::ChangeWeapon()
{
	float targetWeapon = player->weaponWidget->slotPos->GetPosition().X;
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

	player->weaponWidget->RemoveFromParent();
}

void UAttackComponent::IdleState()
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::OnButtonMenu()
{	
	OnWeaponUI();
}

void UAttackComponent::SwordState()
{
	player->compSword->SetVisibility(true);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::BowState()
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(true);
}

void UAttackComponent::FireArrow()
{
	
}

void UAttackComponent::FireSword()
{
	FHitResult hitInfo;
	FVector startLoc = player->GetActorLocation();
	FVector endLoc = player->GetActorForwardVector() * 100;
	GetWorld()->LineTraceSingleByChannel(hitInfo,startLoc, endLoc, ECC_Visibility);
	DrawDebugLine(GetWorld(),startLoc,endLoc,FColor::Blue, 1.0f, 1.0f, 1.0f);

// 	if (hitInfo.GetActor() == obstacleBase)
// 	{
// 		if(obstacleBase->)
// 			GEngine->AddOnScreenDebugMessage(1, 1.0, FColor::Red, FString::Printf(TEXT("ATTACK SUCCEDED")));
// 	}
// 	else
// 	{
// 
// 	}
}

void UAttackComponent::OnTriggerRight()
{

}

void UAttackComponent::OnReleaseRight()
{

}

void UAttackComponent::OnWeaponUI()
{

	// ���� �޴��� ���������� (bSwitch)

	if (!bWeaponMenu)
	{
		// ����Ʈ�� UI ����
		player->weaponWidget->AddToViewport();
		//  ���¿� ���� MovePanel x�� �ʱ� ��ġ�� �����Ѵ�.
		player->weaponWidget->SetUIInitPos((int32)(currAttackState));
	}
	if (bWeaponMenu)
	{
		// ����Ʈ���� UI���� (���)
		player->weaponWidget->RemoveFromParent();
	}
	// �޴��� �ȿ��� ������ (!bSwitch)
	bWeaponMenu = !bWeaponMenu;
}

void UAttackComponent::OnThumbstickLeft(const FInputActionValue& value)
{

}

void UAttackComponent::OnGrabRight()
{
	if(bWeaponMenu)
	player->weaponWidget->MoveUI(-1);
}

void UAttackComponent::OnGrabLeft()
{
	if(bWeaponMenu)
	player->weaponWidget->MoveUI(1);
}


