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
	
	
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	IsInAir();
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::RotateCamera);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UMoveComponent::Jump);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UMoveComponent::Jump);
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
	if(FMath::Abs(axis.X) >= 0.7 || FMath::Abs(axis.Y) >= 0.7)
		OnDash();
	else
		OnWalk();
}

void UMoveComponent::OnDash()
{
	if (player->stamina > 0)
	{
		player->GetCharacterMovement()->MaxWalkSpeed = 1000;
		player->bUseStamina = true;
	}
	else
	{
		OnWalk();
	}


}
void UMoveComponent::OnWalk()
{
	player->ResetCurrTime();
	player->GetCharacterMovement()->MaxWalkSpeed = 300;
}

void UMoveComponent::Jump()
{
	
	player->Jump();

	if(player->GetCharacterMovement()->IsFalling() == true)
		player->GetCharacterMovement()->GravityScale = 0.5;
}

// void UMoveComponent::IsInAir()
// {
// 	
// 	FHitResult hitInfo; // hitresult
// 	FCollisionQueryParams params; // 탐색 방법 설정값 모아놓은 구조체
// 	params.AddIgnoredActor(player); // 내 액터 제외
// 
// 	// 캡슐 컴포넌트의 끝을 시작지점으로 (액터의 높이 - 캡슐 컴포넌트의 높이/2)
// 	FVector lineStart = FVector(0,0,player->GetActorLocation().Z - player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
// 	
// 	FVector lineEnd = FVector::UpVector * 1000.0f;
// 
// 	GetWorld()->LineTraceSingleByChannel(hitInfo,lineStart,lineEnd,ECC_Visibility,params);
// 	
// 	// 맞은곳의 거리에 따라 공중상태인지 판별
// 	if(hitInfo.Location.Z - lineStart.Z < 50.f)
// 		bInAir = false;
// 	else
// 		bInAir = true;
// 	
// 	// 공중이면 1, 지상이면 0을 뷰포트에 띄워줌
// 	GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Red,FString::Printf(TEXT("%d"),bInAir));
/*}*/