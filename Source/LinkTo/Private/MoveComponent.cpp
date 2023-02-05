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
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* moveSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(moveCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
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
// 	FCollisionQueryParams params; // Ž�� ��� ������ ��Ƴ��� ����ü
// 	params.AddIgnoredActor(player); // �� ���� ����
// 
// 	// ĸ�� ������Ʈ�� ���� ������������ (������ ���� - ĸ�� ������Ʈ�� ����/2)
// 	FVector lineStart = FVector(0,0,player->GetActorLocation().Z - player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
// 	
// 	FVector lineEnd = FVector::UpVector * 1000.0f;
// 
// 	GetWorld()->LineTraceSingleByChannel(hitInfo,lineStart,lineEnd,ECC_Visibility,params);
// 	
// 	// �������� �Ÿ��� ���� ���߻������� �Ǻ�
// 	if(hitInfo.Location.Z - lineStart.Z < 50.f)
// 		bInAir = false;
// 	else
// 		bInAir = true;
// 	
// 	// �����̸� 1, �����̸� 0�� ����Ʈ�� �����
// 	GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Red,FString::Printf(TEXT("%d"),bInAir));
/*}*/