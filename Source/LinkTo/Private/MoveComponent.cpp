// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


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
#include <Engine/EngineTypes.h>

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());

	APlayerController* moveCon = GetWorld()->GetFirstPlayerController();
	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* moveSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(moveCon->GetLocalPlayer());
	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
	moveSubsys->AddMappingContext(moveMapping, 0);

	player = Cast<AJS_Player>(GetOwner());

}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::RotateCamera);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UMoveComponent::TriggerButtonB);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UMoveComponent::ReleaseButtonB);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UMoveComponent::JumpPlayer);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UMoveComponent::JumpPlayer);

}
void UMoveComponent::RotateCamera(const FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();
	player->AddControllerPitchInput(axis.Y * -1.0f);
	player->AddControllerYawInput(axis.X);

}
void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();
	FVector dir = FVector(axis.Y, axis.X, 0);
	dir.Normalize();

	player->AddMovementInput(dir, 1, false);
	if ((int32)(playerState) == 0)
	{
		if (FMath::Abs(axis.X) >= 0.7 || FMath::Abs(axis.Y) >= 0.7)
			OnDash();
		else
			OnWalk();
	}
	else if ((int32)(playerState) == 1)
	{
		player->compState->SetStaminaState(false);
		player->GetCharacterMovement()->MaxWalkSpeed = 600;
	}

}

void UMoveComponent::OnDash()
{
	if (player->compState->stamina > 0)
	{
		player->GetCharacterMovement()->MaxWalkSpeed = 1000;
		player->compState->SetStaminaState(true);
	}
	else
	{
		OnWalk();
	}


}
void UMoveComponent::OnWalk()
{
	player->compState->SetStaminaState(false);
	player->GetCharacterMovement()->MaxWalkSpeed = 300;
}

void UMoveComponent::TriggerButtonB()
{

	switch ((int32)(playerState))
	{
	case 0:
		Parasale(false);
		break;
	case 1:
		Parasale(true);
		break;
	}
}
void UMoveComponent::ReleaseButtonB()
{
	Parasale(false);
}
void UMoveComponent::JumpPlayer(const FInputActionValue& value)
{
	if (value.Get<float>() == 1)
	{
		player->Jump();
	}
	else if (value.Get<float>() == 0)
	{
		player->StopJumping();
	}
}
void UMoveComponent::Parasale(bool value)
{
	player->StopJumping();
	if (value)
	{
		if (player->compState->stamina > 0)
		{
			player->GetCharacterMovement()->GravityScale = 0.2;
			player->compState->SetStaminaState(true);
		}
		else
		{
			player->GetCharacterMovement()->GravityScale = 1;
		}
	}
	else if (value == false)
	{
		player->compState->SetStaminaState(false);
		player->GetCharacterMovement()->GravityScale = 1;
	}


}
