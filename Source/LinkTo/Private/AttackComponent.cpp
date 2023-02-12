// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
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
#include "PlayerStateComponent.h"
#include "JS_WidgetWeaponSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include "JS_Arrow.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

 	ConstructorHelpers::FClassFinder<AJS_Arrow>tempArrow(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Weapons/BP_Arrow.BP_Arrow_c'"));
 	if (tempArrow.Succeeded())
 	{
 		arrowFactory = tempArrow.Class;
 	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());

	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� ��������
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	// 3. ������ Subsystem�� IMC�� ���.(�켱���� 0��)
	attackSubsys->AddMappingContext(attackMapping, 0);
	// ���� ���� �޴� �ʱ�ȭ
	bWeaponMenu = false;
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

	/*GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow, FString::Printf(TEXT("%d"),(int32)(currAttackState)));*/

	switch (currAttackState)
	{
	case EAttackState::AttackIdle:
		IdleState();
		break;
	case EAttackState::AttackSword:
		SwordState();
		break;
	case EAttackState::AttackBow:
		BowState();
		break;
	}
	// ...
}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[5], ETriggerEvent::Started, this, &UAttackComponent::OnButtonMenu);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA); 
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabRight);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabLeft);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerArrow);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseArrow);
}

void UAttackComponent::OnButtonA()
{

	if (player->compSkill->bSkillMenu == false)
	{
		if(!bWeaponMenu) // ���� �޴��� �ȿ��� ������
		switch ((int32)(currAttackState)) // ���¿� ���� �ൿ
		{
			case 0:
			break;
			case 1:
			FireSword(); // Į ����
			break;
			case 2:
			break;
		}
		else // �޴��� ���������� 
		{
			ChangeWeapon();
		}
	}
}

void UAttackComponent::ChangeWeapon() // ���� �ٲٴ� �Լ�
{
	// ������ X�࿡ ���� ���� �ٲ�
	float targetWeapon = player->weaponWidget->slotPos->GetPosition().X;
	if (targetWeapon == 0)
	{
		currAttackState = EAttackState::AttackSword;
	}
	else if (targetWeapon == -350.f)
	{
		currAttackState = EAttackState::AttackBow;
	}
	else
		currAttackState = EAttackState::AttackIdle;

	// UI ��
	player->weaponWidget->RemoveFromParent();
}

void UAttackComponent::IdleState() // �⺻ �����϶�
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::OnButtonMenu() // �޴���ư ��������
{	

	if(player->compSkill->bSkillMenu) // ��ų�޴��� ����������
	{
		player->skillWidget->RemoveFromParent(); // ��ų�޴��� �ݰ�
		player->compSkill->bSkillMenu = false;
	}
	OnWeaponUI();
}

void UAttackComponent::SwordState() // ���� �����϶�
{
	player->compSword->SetVisibility(true);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::BowState() // Ȱ �����϶�
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(true);
}

void UAttackComponent::FireSword() // Į ����
{
	FHitResult hitInfo;
	FVector startLoc = player->GetActorLocation();
	FVector endLoc = player->GetActorForwardVector() * 100;
	GetWorld()->LineTraceSingleByChannel(hitInfo,startLoc, endLoc, ECC_Visibility);
	DrawDebugLine(GetWorld(),startLoc,endLoc,FColor::Blue, 1.0f, 1.0f, 1.0f);

// 	if (hitInfo.GetActor() == obstacleBase)
// 	{
// 		if(obstacleBase->)
// 			GEngine->AddOnScreenDebugMessage(1, 1.0, FColor::Red, FString::Printf(TEXT("ATTACK SUCCEDED")));
// 	}
// 	else
// 	{
// 
// 	}
}

void UAttackComponent::OnTriggerArrow() // ȭ�� ����
{ 
	// ���� ���°� Bow�϶� (Bow�� ���������)
	if (currAttackState == EAttackState::AttackBow)
	{
		// ������ �ð���ŭ ������ �Ѵ� (ȭ�쿡 ���� �����ش�)
		accArrowSpeed += 600*GetWorld()->DeltaTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"), accArrowSpeed);
	}
}

void UAttackComponent::OnReleaseArrow() // ȭ�� �߻�
{
	if (currAttackState == EAttackState::AttackBow)
	{
		AJS_Arrow* spawnedArrow = GetWorld()->SpawnActor<AJS_Arrow>(arrowFactory, player->GetActorLocation(), player->GetActorRotation());
		spawnedArrow->FireInDirection(spawnedArrow->GetActorForwardVector());
		accArrowSpeed = 0; // ���ӵ� 0���� �ʱ�ȭ
	}

}

void UAttackComponent::OnWeaponUI() // UI���� �ݴ� �Լ�
{

	if (player->compSkill->bSkillMenu)
	{
		player->skillWidget->RemoveFromParent();
		player->compSkill->bSkillMenu = false;
	}
	else
	{
		if (!bWeaponMenu) //�޴��� �ȿ��� ������ (!bSwitch)
		{
			// ����Ʈ�� UI ����
			player->weaponWidget->AddToViewport();
			//  ���¿� ���� MovePanel x�� �ʱ� ��ġ�� �����Ѵ�.
			player->weaponWidget->SetUIInitPos((int32)(currAttackState));
		}
		else 	// ���� �޴��� ���������� (bSwitch)
		{
			// ����Ʈ���� UI���� (���)
			player->weaponWidget->RemoveFromParent();
		}

		bWeaponMenu = !bWeaponMenu;
	}

}


// UI ���������� ������ �̵���Ű�� �Լ�
void UAttackComponent::OnGrabRight()
{
	if(bWeaponMenu)
	player->weaponWidget->MoveUI(-1);
}

void UAttackComponent::OnGrabLeft()
{
	if(bWeaponMenu)
	player->weaponWidget->MoveUI(1);
}


