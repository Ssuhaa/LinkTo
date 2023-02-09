// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateComponent.h"
#include "MoveComponent.h"
#include "JS_Player.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values for this component's properties
UPlayerStateComponent::UPlayerStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	// ...

	stamina = 100; // ���¹̳� 100���� �ʱ�ȭ

}


// Called every frame
void UPlayerStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EPlayerState::bLanding:
	{
		LandsingState();
	}
	break;
	case EPlayerState::bFalling:
		FallingState();
		break;
	case EPlayerState::bCliming:
		break;
	}

	CurrStamina(bUseStamina); // ���� ���¹̳� ����ϴ��� �Ǻ�
	// ...

}

void UPlayerStateComponent::FallingState() // �������� �ִ� ����
{
	player->compMove->canParasale = true;
		IsInAir();// ��/���� �Ǻ�
}

void UPlayerStateComponent::LandsingState() // �� ����
{
	player->compMove->canParasale = false; // �з����� ��� ���� ����
		IsInAir(); // ��/���� �Ǻ�
}

void UPlayerStateComponent::ClimbingState() // ��Ÿ�� ����
{

}

void UPlayerStateComponent::ChangeState(EPlayerState state)
{
	currState = state; // ���� ��ȯ �Լ�
}


void UPlayerStateComponent::UseStamina() // ���¹̳� ����ϴ� �Լ�
{
	if (stamina > 0)
		stamina -= GetWorld()->DeltaTimeSeconds * 10;
	else
		stamina = 0;
}

void UPlayerStateComponent::ChargeStamina()//���¹̳� ���� �Լ�
{

	if (stamina < 100) // 100���� ������
	{
		if(stamina > 0) // ���������� 
			stamina += GetWorld()->DeltaTimeSeconds * 20;// �ٷ� ä��
		else // �پ���
		{
			currTime += GetWorld()->DeltaTimeSeconds;
			if (currTime >= 3.0f)
			{
				stamina += GetWorld()->DeltaTimeSeconds * 10; // 3���Ŀ� ä��
			}
		}

	}
	else
		stamina = 100.f;  // 100�Ѿ�� 100���� ����
	


}

void UPlayerStateComponent::CurrStamina(bool value)
{
	if (value == true)
	{
		UseStamina();
	}
	else if (value == false)
	{
		ChargeStamina();
	}

	player->OnLogMove(FString::Printf(TEXT("%.2f"), stamina));
}
void UPlayerStateComponent::IsInAir()
{
	if (player->GetCharacterMovement()->IsFalling() == true)
		ChangeState(EPlayerState::bFalling);
	else
		ChangeState(EPlayerState::bLanding);
}

void UPlayerStateComponent::SetStaminaState(bool value)
{
	bUseStamina = value;
	currTime = 0;
}
