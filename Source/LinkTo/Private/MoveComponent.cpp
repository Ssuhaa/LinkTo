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
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* moveSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(moveCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
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
		if (FMath::Abs(axis.X) >= 0.7 || FMath::Abs(axis.Y) >= 0.7) // X,Y�� ���밪�� ���� �޸���, �ȱ� ��ȯ
			OnDash();
		else
			OnWalk();
	}
	else 
	{
		OnWalk(); // �� ���� ���¿����� �ȱ��� �̵��ӵ��� ������
	}

}

void UMoveComponent::OnDash() // �޸���
{
	if (player->compState->stamina > 0) // ���¹̳ʰ� ����������
	{
		player->GetCharacterMovement()->MaxWalkSpeed = 1000; 
		player->compState->SetStaminaState(true); // ���¹̳��� ���¸� �������
	}
	else
	{
		OnWalk();
	}


}
void UMoveComponent::OnWalk() // �ȱ�
{
	player->GetCharacterMovement()->MaxWalkSpeed = 300;
	player->compState->SetStaminaState(false); // ���¹̳��� ���¸� ��� ��������
}

void UMoveComponent::TriggerButtonB() // B��ư ������ ������
{

	switch ((int32)(playerState))
	{
	case 0:
		Parasale(false); // ���̸� �з����� ��� ����
		break;
	case 1:
		Parasale(true); // �����̸� �����
		break;
	}
}
void UMoveComponent::ReleaseButtonB() // B��ư ����
{
	Parasale(false); // �з����� ��� ����
}

void UMoveComponent::JumpPlayer() // ����
{

	if(playerState != EPlayerState::bFalling) // ������ �ƴϸ�
	player->Jump(); // ����

}
void UMoveComponent::Parasale(bool value) // �з�����
{

	if (value) // �з������� ������̸�
	{
		if (player->compState->stamina > 0) // ���¹̳ʰ� ����������
		{
			player->GetCharacterMovement()->GravityScale = 0.2; // ������ ������
			player->compState->SetStaminaState(true); // ���¹̳� �����·� ��ȯ
			bParasale = true; // ���� �з����� ����

		}
		else // ���׹̳ʰ� ������
		{
			player->GetCharacterMovement()->GravityScale = 1; // ���� �ӵ��� ������
			player->compState->SetStaminaState(false); // ���¹̳� ��� ����
			bParasale = false; // �з����� ����
		}
	}
	else // �з������� ������� ������
	{
		player->compState->SetStaminaState(false); // ���¹̳� ������
		bParasale = false;
		player->GetCharacterMovement()->GravityScale = 1;
	}


}
