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

	stamina = 100; // 스태미나 100으로 초기화

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

	CurrStamina(bUseStamina); // 현재 스태미나 사용하는지 판별
	// ...

}

void UPlayerStateComponent::FallingState() // 떨어지고 있는 상태
{
	player->compMove->canParasale = true;
		IsInAir();// 땅/공중 판별
}

void UPlayerStateComponent::LandsingState() // 땅 상태
{
	player->compMove->canParasale = false; // 패러세일 사용 가능 여부
		IsInAir(); // 땅/공중 판별
}

void UPlayerStateComponent::ClimbingState() // 벽타기 상태
{

}

void UPlayerStateComponent::ChangeState(EPlayerState state)
{
	currState = state; // 상태 전환 함수
}


void UPlayerStateComponent::UseStamina() // 스태미나 사용하는 함수
{
	if (stamina > 0)
		stamina -= GetWorld()->DeltaTimeSeconds * 10;
	else
		stamina = 0;
}

void UPlayerStateComponent::ChargeStamina()//스태미나 차는 함수
{

	if (stamina < 100) // 100보다 작을때
	{
		if(stamina > 0) // 남아있으면 
			stamina += GetWorld()->DeltaTimeSeconds * 20;// 바로 채움
		else // 다쓰면
		{
			currTime += GetWorld()->DeltaTimeSeconds;
			if (currTime >= 3.0f)
			{
				stamina += GetWorld()->DeltaTimeSeconds * 10; // 3초후에 채움
			}
		}

	}
	else
		stamina = 100.f;  // 100넘어가면 100으로 고정
	


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
