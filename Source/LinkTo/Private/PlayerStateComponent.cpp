// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateComponent.h"
#include "JS_Player.h"

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

