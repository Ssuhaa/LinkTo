// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "JS_Player.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Components/TextRenderComponent.h"
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
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Started, this, &UMoveComponent::JumpPlayer);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UMoveComponent::TriggerButtonB);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UMoveComponent::ReleaseButtonB);

	

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
		if (FMath::Abs(axis.X) >= 0.7 || FMath::Abs(axis.Y) >= 0.7) // X,Y의 절대값에 따라 달리기, 걷기 전환
			OnDash();
		else
			OnWalk();
	}
	else 
	{
		OnWalk(); // 땅 외의 상태에서는 걷기의 이동속도로 움직임
	}

}

void UMoveComponent::OnDash() // 달리기
{
	if (player->compState->stamina > 0) // 스태미너가 남아있으면
	{
		player->GetCharacterMovement()->MaxWalkSpeed = 1000; 
		player->compState->SetStaminaState(true); // 스태미나의 상태를 사용으로
	}
	else
	{
		OnWalk();
	}


}
void UMoveComponent::OnWalk() // 걷기
{
	player->GetCharacterMovement()->MaxWalkSpeed = 300;
	player->compState->SetStaminaState(false); // 스태미나의 상태를 사용 안함으로
}

void UMoveComponent::TriggerButtonB() // B버튼 누르고 있을때
{

	switch ((int32)(playerState))
	{
	case 0:
		Parasale(false); // 땅이면 패러세일 사용 안함
		break;
	case 1:
		Parasale(true); // 공중이면 사용함
		break;
	}
}
void UMoveComponent::ReleaseButtonB() // B버튼 떼면
{
	Parasale(false); // 패러세일 사용 안함
}

void UMoveComponent::JumpPlayer() // 점프
{

	if(playerState != EPlayerState::bFalling) // 공중이 아니면
	player->Jump(); // 점프

}
void UMoveComponent::Parasale(bool value) // 패러세일
{

	if (value) // 패러세일을 사용중이면
	{
		if (player->compState->stamina > 0) // 스태미너가 남아있으면
		{
			player->GetCharacterMovement()->GravityScale = 0.2; // 느리게 떨어짐
			player->compState->SetStaminaState(true); // 스태미너 사용상태로 전환
			bParasale = true; // 현재 패러세일 상태

		}
		else // 스테미너가 없으면
		{
			player->GetCharacterMovement()->GravityScale = 1; // 보통 속도로 떨어짐
			player->compState->SetStaminaState(false); // 스태미너 사용 안함
			bParasale = false; // 패러세일 끄기
		}
	}
	else // 패러세일을 사용하지 않으면
	{
		player->compState->SetStaminaState(false); // 스태미너 사용안함
		bParasale = false;
		player->GetCharacterMovement()->GravityScale = 1;
	}


}
