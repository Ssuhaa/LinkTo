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
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include "SH_Ice.h"
#include <Kismet/GameplayStatics.h>

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

	canClimb = false;
	bClimb = false;
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;



	ClimbingLineTrace();

	if (playerState == EPlayerState::bLanding)
	{
		lastLoc = player->GetActorLocation();
	}
	

}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::RotateCamera);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UMoveComponent::StartButtonA);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Started, this, &UMoveComponent::StartButtonB);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UMoveComponent::TriggerButtonB);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UMoveComponent::ReleaseButtonB);
	
	PlayerInputComponent->BindAction(spaceBar, ETriggerEvent::Started, this, &UMoveComponent::StartButtonB);
	PlayerInputComponent->BindAction(spaceBar, ETriggerEvent::Completed, this, &UMoveComponent::ReleaseButtonB);
	PlayerInputComponent->BindAction(spaceBar, ETriggerEvent::Triggered, this, &UMoveComponent::TriggerButtonB);
}
void UMoveComponent::RotateCamera(const FInputActionValue& value)
{
	
	player->bUseControllerRotationYaw = true;
/*	player->bUseControllerRotationPitch = true;*/
	FVector2D axis = value.Get<FVector2D>();
	
	player->AddControllerYawInput(axis.X);
	float currPitch = player->compCam->GetComponentRotation().Pitch;
	currPitch += axis.Y * (-1.0) * GetWorld()->DeltaTimeSeconds;
	
	player->compCam->SetRelativeRotation(FRotator(currPitch,0,0));
	
	
/*	player->AddControllerPitchInput(axis.Y * -1.0f);*/

}

void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D MovementVector = value.Get<FVector2D>();
	
	if(canClimb)
	{
	
				FVector p0 = player->GetActorLocation();
				FVector v = player->GetActorUpVector() * MovementVector.Y + player->GetActorRightVector() * MovementVector.X;
				v.Normalize();
				float t = 100.f * GetWorld()->DeltaTimeSeconds;
				FVector p = p0 + v * t;
				player->SetActorLocation(p);
	}
	else
	{

		switch (playerState)
		{
		case EPlayerState::bLanding:
		{


			const FRotator Rotation = player->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			player->AddMovementInput(ForwardDirection, MovementVector.Y);
			player->AddMovementInput(RightDirection, MovementVector.X);

			if (FMath::Abs(MovementVector.X) >= 0.7 || FMath::Abs(MovementVector.Y) >= 0.7) // X,Y�� ���밪�� ���� �޸���, �ȱ� ��ȯ
				OnDash();
			else
				OnWalk();
		}
		break;
		case EPlayerState::bFalling:

			const FRotator Rotation = player->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			player->AddMovementInput(ForwardDirection, MovementVector.Y);
			player->AddMovementInput(RightDirection, MovementVector.X);

			player->GetCharacterMovement()->MaxWalkSpeed = 800.f;
			break;

		}

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
void UMoveComponent::StartButtonA() 
{
	if (canClimb)
	{
		ReleaseClimb();
	}
	
}
void UMoveComponent::TriggerButtonB() // B��ư ������ ������
{
	if (canParasale == true)
	{
		Parasale(true);
	}
	else
	Parasale(false);
}
void UMoveComponent::ReleaseButtonB() // B��ư ����
{
	Parasale(false);
}

void UMoveComponent::StartButtonB() // ����
{
	if(!canClimb)
	{
		
		switch (playerState)
		{
		case EPlayerState::bLanding:
			player->Jump(); // ����
			break;
		case EPlayerState::bFalling:
			canParasale = true; // �з������� ��� ������ ���·� 
			player->LaunchCharacter(FVector(0,0,100.f),false,false);
			break;
		}
	}
	if (canClimb)
	{
		Climb();
	}

	
}
void UMoveComponent::Parasale(bool value) // �з�����
{

	if (value) // �з������� ������̰�
	{
		if (player->compState->stamina > 0) // ���¹̳ʰ� ����������
		{

			player->GetCharacterMovement()->GravityScale = 0.2; // ������ ������
			player->GetCharacterMovement()->MaxWalkSpeed = 300;
			player->GetCharacterMovement()->AirControl = 0.8;
			player->compState->SetStaminaState(true); // ���¹̳� �����·� ��ȯ
			bParasale = true; // ���� �з����� ����

		}
		else // ���׹̳ʰ� ������
		{
			player->GetCharacterMovement()->GravityScale = 1; // ���� �ӵ��� ������
			player->compState->SetStaminaState(false); // ���¹̳� ��� ����
			player->GetCharacterMovement()->AirControl = 0.2;
			bParasale = false; // �з����� ����
 		}
	}
	else  // �з������� ������� ������
	{
		player->compState->SetStaminaState(false); // ���¹̳� ������
		bParasale = false;
		player->GetCharacterMovement()->AirControl = 0.2;
		player->GetCharacterMovement()->GravityScale = 1;
	}


}
void UMoveComponent::ClimbingLineTrace()
{
	FVector startLoc = player->GetActorLocation();
	FVector endLoc = player->GetActorLocation() + player->GetActorForwardVector() * 200;
	TArray<AActor*> ignore;
	ignore.Add(player);
	ice = Cast<ASH_Ice>(UGameplayStatics::GetActorOfClass(GetWorld(),ASH_Ice::StaticClass()));

	 UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		startLoc,
		endLoc,
		ETraceTypeQuery::TraceTypeQuery2,
		false,
		ignore,
		EDrawDebugTrace::ForOneFrame,
		hitInfo,
		true);

	if (hitInfo.GetActor() == ice)
	{
		canClimb = true;
	}
	else
	{
		canClimb = false;
	}


		
}

 
 void UMoveComponent::Climb()
{
 	if (canClimb)
 	{
 		playerState = EPlayerState::bCliming;
 		player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		player->GetCharacterMovement()->bOrientRotationToMovement = false;
		player->bUseControllerRotationYaw=false;
 		ice = Cast<ASH_Ice>(UGameplayStatics::GetActorOfClass(GetWorld(),ASH_Ice::StaticClass()));
 		/*ice->iceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
//  		player->GetCharacterMovement()->StopMovementImmediately();
//  
			 		FVector climbLoc =  FVector(ice->GetActorLocation().X - 100.0f, ice->GetActorLocation().Y, hitInfo.Location.Z) + ice->GetActorForwardVector();
			 		FRotator climbRot = UKismetMathLibrary::MakeRotFromX(hitInfo.Normal);
					FRotator newActorRot = player->GetActorRotation();
			 		FLatentActionInfo Info;
			 		Info.CallbackTarget = this;
					UKismetSystemLibrary::MoveComponentTo(
						player->GetCapsuleComponent(),
						climbLoc,
						newActorRot,
						true,
						true,
						0.2f,
						false,
						EMoveComponentAction::Type::Move,
						Info);
	}
	else
	{
	

		player->GetCharacterMovement()->bOrientRotationToMovement = false;
		player->bUseControllerRotationYaw = true;
		player->bUseControllerRotationPitch = true;
		if (player->GetCharacterMovement()->IsFalling() == false)
		{
			player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			playerState = EPlayerState::bLanding;
		}
		else
		{
			player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			playerState = EPlayerState::bFalling;

		}


 	}
 }

 void UMoveComponent::ReleaseClimb()
{
		player->GetCharacterMovement()->bOrientRotationToMovement = false;
		player->bUseControllerRotationPitch = false;
		player->bUseControllerRotationYaw= false;
		if (player->GetCharacterMovement()->IsFalling() == true)
		{
			playerState = EPlayerState::bFalling;
			player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		else
		{
			playerState = EPlayerState::bLanding;
			player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		

		
 
			FVector HangingLoc = player->GetActorLocation();
			FRotator HangingRot = FRotator(player->GetActorRotation().Roll, player->GetActorRotation().Yaw, player->GetActorRotation().Pitch);
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(
				player->GetCapsuleComponent(),
				HangingLoc,
				HangingRot,
				true,
				true,
				0.2f,
				false,
				EMoveComponentAction::Type::Move,
				Info);

 }
