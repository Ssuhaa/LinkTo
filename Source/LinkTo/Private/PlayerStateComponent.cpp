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


	if (bUseStamina)
	{
		StaminaStatus(bUseStamina, DeltaTime);
	}
	else
	{
		currTime += DeltaTime;
		if (currTime >= 3.0f)
			StaminaStatus(bUseStamina, DeltaTime);
	}
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


void UPlayerStateComponent::StaminaStatus(bool value, float deltaTime)
{

	if (value)
	{
		if (stamina > 0)
			stamina -= deltaTime * 20;
		else
			stamina = 0;
	}
	else
	{
		{
			if (stamina < 100)
				stamina += deltaTime * 10;
			else
				stamina = 100.f;
		}
	}

	FString currStamina = FString::Printf(TEXT("%.2f"), stamina);
	player->OnLogMove(currStamina);
}
void UPlayerStateComponent::ResetCurrTime()
{
	bUseStamina = false;
	currTime = 0;
}
void UPlayerStateComponent::IsInAir()
{
	if (player->GetCharacterMovement()->IsFalling() == true)
		ChangeState(EPlayerState::bFalling);
	else
		ChangeState(EPlayerState::bLanding);

// 	FHitResult hitInfo; // hitresult
// 	FCollisionQueryParams params; // 탐색 방법 설정값 모아놓은 구조체
// 	params.AddIgnoredActor(player); // 내 액터 제외
// 	FVector actorLoc = player->GetActorLocation();
// 
// 	// 캡슐 컴포넌트의 끝을 시작지점으로 (액터의 높이 - 캡슐 컴포넌트의 높이/2)
// 	FVector lineStart = FVector(actorLoc.X,actorLoc.Y, actorLoc.Z - player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
// 
// 	FVector lineEnd = player->GetActorUpVector() * -1000.f;
// 
// 	GetWorld()->LineTraceSingleByChannel(hitInfo, lineStart, lineEnd, ECC_Visibility, params);
// 
// 	// 맞은곳의 거리에 따라 공중상태인지 판별
// 	if (hitInfo.GetActor() != nullptr)
// 	{
// 		if (hitInfo.Location.Z - lineStart.Z < 50.f)
// 			currState = EPlayerState::bLanding;
// 		else
// 			currState = EPlayerState::bFalling;
// 	}

}
