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

	CurrStamina(bUseStamina);
	// ...

}

void UPlayerStateComponent::FallingState()
{
	player->compMove->canParasale = true;
	UE_LOG(LogTemp, Warning, TEXT("currState = Falling"))
		IsInAir();
}

void UPlayerStateComponent::LandsingState()
{
	player->compMove->canParasale = false;

	UE_LOG(LogTemp, Warning, TEXT("currState = Landing"))
		IsInAir();
}

void UPlayerStateComponent::ClimbingState()
{

}

void UPlayerStateComponent::ChangeState(EPlayerState state)
{
	currState = state;
}


void UPlayerStateComponent::UseStamina()
{
	if (stamina > 0)
		stamina -= GetWorld()->DeltaTimeSeconds * 20;
	else
		stamina = 0;
}

void UPlayerStateComponent::ChargeStamina()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= 3.0f)
	{
		if (stamina < 100)
			stamina += GetWorld()->DeltaTimeSeconds * 10;
		else
			stamina = 100.f;
	}

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
