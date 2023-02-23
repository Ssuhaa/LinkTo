// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LinkSound.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include "JS_Player.h"
#include "MoveComponent.h"
#include "AttackComponent.h"

// Sets default values for this component's properties
UJS_LinkSound::UJS_LinkSound()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJS_LinkSound::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(bgmTimer,this,&UJS_LinkSound::PlayBGM,1,false,4.f);
	
}


// Called every frame
void UJS_LinkSound::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

void UJS_LinkSound::PlayBGM()
{
	UGameplayStatics::PlaySound2D(this,BGM,0.8f,1.f,1.0f);
	TimerClear(bgmTimer);
}

void UJS_LinkSound::TimerClear(FTimerHandle handleName)
{
	GetWorld()->GetTimerManager().ClearTimer(handleName);
}

void UJS_LinkSound::PlayWindSound()
{
	if (player->compMove->bParasale)
	{
		SetSound(wind2);
		Play();
	}
	else
	Stop();
}

void UJS_LinkSound::PlaySwordEquipSound()
{

		UGameplayStatics::PlaySound2D(this,equipSword); // 장비 넣기

}
void UJS_LinkSound::PlaySwordUnEquipSound()
{

	UGameplayStatics::PlaySound2D(this, unequipSword); // 장비 넣기

}
void UJS_LinkSound::PlayAttackSwordSound()
{
	int randAttack = FMath::RandRange(1, 2);
	switch (randAttack)
	{
	case 1:
		UGameplayStatics::PlaySound2D(this, attack1, 1.f, 1.f, 0.05f);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(this, attack7, 1.f, 1.f, 0.05f);
		break;
	}
}

void UJS_LinkSound::PlayOpenStoneSound()
{
	UGameplayStatics::PlaySound2D(this, shikerOpen);
}

void UJS_LinkSound::PlayCloseStoneSound()
{
	UGameplayStatics::PlaySound2D(this, sheikerClose);
}

void UJS_LinkSound::PlaySwitchSound()
{
	UGameplayStatics::PlaySound2D(this,switchUI);
}

void UJS_LinkSound::PlayWalkSound()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= walkDelay)
	{
		if (player->compState->currState == EPlayerState::bLanding)
		{

			UGameplayStatics::PlaySound2D(this, moveSlow, 2.0f, 1.f, 0.f);
			currTime = 0;
		}
	}
	

}
void UJS_LinkSound::PlayDashSound()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= dashDelay)
	{
		if (player->compState->currState == EPlayerState::bLanding)
		{
			UGameplayStatics::PlaySound2D(this, moveFast, 2.0f, 1.f, 0.f);
			currTime = 0;
		}
	}
}
void UJS_LinkSound::PlayJumpSound()
{
	int randJump = FMath::RandRange(1,3);
	switch (randJump)
	{
	case 1:
		UGameplayStatics::PlaySound2D(this, jump1,1.f,1.f,0.05f);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(this, jump2,1.f,1.f,0.05f);
		break;
	case 3:
		UGameplayStatics::PlaySound2D(this, jump3,1.f,1.f,0.05f);
		break;
	}

}

void UJS_LinkSound::PlayParaSound()
{
	UGameplayStatics::PlaySound2D(this, parasailOpen);
}

