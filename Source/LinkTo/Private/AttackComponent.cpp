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

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());

	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
	attackSubsys->AddMappingContext(attackMapping, 0);
	// ...
	
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

	switch (currAttackState)
	{
	case EAttackState::AttackIdle:
		IdleState();
		break;
	case EAttackState::AttackSwitch:
		WeaponUIState();
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
	PlayerInputComponent->BindAction(leftInputs[5], ETriggerEvent::Started, this, &UAttackComponent::WeaponUIState);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerRight);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseRight);
}

void UAttackComponent::OnButtonA()
{
	switch ((int32)(currAttackState))
	{
		case 0:
		break;
		case 1:
		ChangeWeapon();
		break;
		case 2:
		// 검 공격
		break;
		case 3:
		// 활 공격
		break;
	}
}

void UAttackComponent::ChangeWeapon()
{
	
}

void UAttackComponent::IdleState()
{
	
}

void UAttackComponent::WeaponUIState()
{
	currAttackState = EAttackState::AttackSwitch;
}

void UAttackComponent::SwordState()
{
	
}

void UAttackComponent::BowState()
{
	
}

void UAttackComponent::FireArrow()
{

}

void UAttackComponent::FireSword()
{

}

void UAttackComponent::OnTriggerRight()
{

}

void UAttackComponent::OnReleaseRight()
{

}

