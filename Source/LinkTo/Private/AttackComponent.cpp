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
	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem을 가져오기
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	// 3. 가져온 Subsystem에 IMC를 등록.(우선순위 0번)
	attackSubsys->AddMappingContext(attackMapping, 0);
	// 무기 변경 메뉴 초기화
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
		if(!bWeaponMenu) // 만일 메뉴가 안열려 있으면
		switch ((int32)(currAttackState)) // 상태에 따른 행동
		{
			case 0:
			break;
			case 1:
			FireSword(); // 칼 공격
			break;
			case 2:
			break;
		}
		else // 메뉴가 열려있으면 
		{
			ChangeWeapon();
		}
	}
}

void UAttackComponent::ChangeWeapon() // 무기 바꾸는 함수
{
	// 위젯의 X축에 따라 무기 바꿈
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

	// UI 끔
	player->weaponWidget->RemoveFromParent();
}

void UAttackComponent::IdleState() // 기본 상태일때
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::OnButtonMenu() // 메뉴버튼 눌렀을시
{	

	if(player->compSkill->bSkillMenu) // 스킬메뉴가 열려있으면
	{
		player->skillWidget->RemoveFromParent(); // 스킬메뉴를 닫고
		player->compSkill->bSkillMenu = false;
	}
	OnWeaponUI();
}

void UAttackComponent::SwordState() // 무기 상태일때
{
	player->compSword->SetVisibility(true);
	player->compBow->SetVisibility(false);
}

void UAttackComponent::BowState() // 활 상태일때
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(true);
}

void UAttackComponent::FireSword() // 칼 공격
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

void UAttackComponent::OnTriggerArrow() // 화살 조준
{ 
	// 공격 상태가 Bow일때 (Bow를 들고있을때)
	if (currAttackState == EAttackState::AttackBow)
	{
		// 누르는 시간만큼 조준을 한다 (화살에 힘을 더해준다)
		accArrowSpeed += 600*GetWorld()->DeltaTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"), accArrowSpeed);
	}
}

void UAttackComponent::OnReleaseArrow() // 화살 발사
{
	if (currAttackState == EAttackState::AttackBow)
	{
		AJS_Arrow* spawnedArrow = GetWorld()->SpawnActor<AJS_Arrow>(arrowFactory, player->GetActorLocation(), player->GetActorRotation());
		spawnedArrow->FireInDirection(spawnedArrow->GetActorForwardVector());
		accArrowSpeed = 0; // 가속도 0으로 초기화
	}

}

void UAttackComponent::OnWeaponUI() // UI열고 닫는 함수
{

	if (player->compSkill->bSkillMenu)
	{
		player->skillWidget->RemoveFromParent();
		player->compSkill->bSkillMenu = false;
	}
	else
	{
		if (!bWeaponMenu) //메뉴가 안열려 있을때 (!bSwitch)
		{
			// 뷰포트에 UI 띄우기
			player->weaponWidget->AddToViewport();
			//  상태에 따라 MovePanel x의 초기 위치를 세팅한다.
			player->weaponWidget->SetUIInitPos((int32)(currAttackState));
		}
		else 	// 현재 메뉴가 열려있을때 (bSwitch)
		{
			// 뷰포트에서 UI제거 (취소)
			player->weaponWidget->RemoveFromParent();
		}

		bWeaponMenu = !bWeaponMenu;
	}

}


// UI 열려있을때 아이콘 이동시키는 함수
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


