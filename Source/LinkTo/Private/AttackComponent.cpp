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
#include "Components/PrimitiveComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "TimeLockBase.h"
#include <MotionControllerComponent.h>

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

 	ConstructorHelpers::FClassFinder<AJS_Arrow>tempArrow(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Weapons/BP_Arrow.BP_Arrow_C'"));
 	if (tempArrow.Succeeded())
 	{
 		arrowFactory = tempArrow.Class;
 	}
	// 무기 스위치 UI 찾아오기

	ConstructorHelpers::FClassFinder<UJS_WidgetWeaponSwitch> tempWeaponWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/SwitchWeapon/JS_SwitchWeapon.JS_SwitchWeapon_C'"));
	if (tempWeaponWidget.Succeeded())
	{
		weaponUIFactory = tempWeaponWidget.Class;
	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	weaponWidget = CreateWidget<UJS_WidgetWeaponSwitch>(GetWorld(), weaponUIFactory);

	APlayerController* attackCon = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* attackSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(attackCon->GetLocalPlayer());
	attackSubsys->AddMappingContext(attackMapping, 0);

	// 무기 변경 메뉴 초기화
	bWeaponMenu = false;

	// 타임락 액터 가져오기
	

}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerState = player->compState->currState;

	GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow, FString::Printf(TEXT("%d"),(int32)(currAttackState)));

	switch (currAttackState)
	{
	case EAttackState::AttackIdle:
		IdleState();
		break;
	case EAttackState::AttackSword:
	{
		SwordState();
		player = Cast<AJS_Player>(GetOwner());
		currHandPos = player->rightHand->GetComponentLocation();
	
		
		// 휘두르는 방향
		FVector attackDir = oldPos - currHandPos;
		// 휘두르는 속력(길이)
		float swordSpeed = UKismetMathLibrary::Square(attackDir.Size());

		

		DrawDebugLine(GetWorld(),oldPos,currHandPos,FColor::Green,false,1,0,1);
		
		// 라인트레이스 발사
		FHitResult hitInfo;
		FVector startPos = player->compSword->GetComponentLocation()+FVector(0,0,30);
		FVector endPos = (startPos + (player->compSword->GetForwardVector()*-1) * 5000) + FVector(0,-10,20);
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner());
		timeLockActor = Cast<ATimeLockBase>(UGameplayStatics::GetActorOfClass(GetWorld(),ATimeLockBase::StaticClass()));
		DrawDebugLine(GetWorld(),startPos,endPos,FColor::Red,false,0.1f,0,1.f);
		bool isTrace = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%d"),isTrace), false, FVector2D(10.0f));
		// 만약 맞은 액터가 타임락이 걸려있으면
		if (hitInfo.GetActor() == timeLockActor)
		{

			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("hit")), false, FVector2D(10.f));

			timeLockActor->impulseArrowUpdate();

			/*bCanHit = false;*/
		}

		// 오른손의 속력이 일정 속력 이상이면
		//if (swordSpeed >= 3.f)
		//{
			// 칼의 콜리젼을 공격으로 변경
			// player->compSword->SetCollisionProfileName(TEXT("AttackSwordState"));


		
		//}
		// 칼의 속력이 일정속력 이하면
		//else
		//{
			// player->compSword->SetCollisionProfileName(TEXT("NoCollision"));
			
			//bCanHit = true;
		//}

		// FString swordState = player->compSword->GetCollisionProfileName().ToString();
		// GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Yellow,swordState,true,FVector2D(10.f));
		
		// 다음 틱에서 사용할 이전 위치
		oldPos = currHandPos;
	}
		break;
	case EAttackState::AttackBow:
		BowState();
		break;
	}

}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[5], ETriggerEvent::Started, this, &UAttackComponent::OnButtonMenu);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &UAttackComponent::OnButtonA); 
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Triggered, this, &UAttackComponent::FireSword); 
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabRight);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &UAttackComponent::OnGrabLeft);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UAttackComponent::OnTriggerArrow);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UAttackComponent::OnReleaseArrow);
}

void UAttackComponent::OnButtonA()
{
	if (!bWeaponMenu) // 만일 메뉴가 안열려 있으면
	{
		switch (currAttackState) // 상태에 따른 행동
		{
		case EAttackState::AttackIdle:
			break;
		case EAttackState::AttackSword:
			break;
		case EAttackState::AttackBow:
			break;
		}
	}
	else // 메뉴가 열려있으면 
	{
		ChangeWeapon();
	}
}
// 무기 바꾸는 함수
void UAttackComponent::ChangeWeapon() 
{
	// 위젯의 X축에 따라 무기 바꿈
	float targetWeapon = weaponWidget->slotPos->GetPosition().X;
	if (targetWeapon == 0)
	{
		currAttackState = EAttackState::AttackSword;
	}
	else if (targetWeapon == -120.f)
	{
		currAttackState = EAttackState::AttackBow;
	}
	else
		currAttackState = EAttackState::AttackIdle;

	// UI 끔
	WeaponMenuOnOff(false);
}
// 기본 상태일때
void UAttackComponent::IdleState() 
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(false);
}
// 메뉴버튼 눌렀을시
void UAttackComponent::OnButtonMenu() 
{	
	player->compSkill->SkillMenuOnOff(false);
	OnWeaponUI();
}

//오른쪽 그랩버튼 눌렀을때
void UAttackComponent::OnGrabRight()
{
	WeaponMenuMove(-1);
}
// 왼쪽 그랩 버튼 눌렀을 때
void UAttackComponent::OnGrabLeft()
{

	WeaponMenuMove(1);
}
// 무기 상태일때
void UAttackComponent::SwordState() 
{
	player->compSword->SetVisibility(true);
	player->compBow->SetVisibility(false);
}
// 활 상태일때
void UAttackComponent::BowState() 
{
	player->compSword->SetVisibility(false);
	player->compBow->SetVisibility(true);
}
// 칼 공격
void UAttackComponent::FireSword() 
{


}
// 화살 조준
void UAttackComponent::OnTriggerArrow() 
{ 
	// 공격 상태가 Bow일때 (Bow를 들고있을때)
	if (currAttackState == EAttackState::AttackBow)
	{
		// 누르는 시간만큼 조준을 한다 (화살에 힘을 더해준다)
		accArrowSpeed += 1000*GetWorld()->DeltaTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"), accArrowSpeed);
	}
}
// 화살 발사
void UAttackComponent::OnReleaseArrow()
{
	if (currAttackState == EAttackState::AttackBow)
	{
		AJS_Arrow* spawnedArrow = GetWorld()->SpawnActor<AJS_Arrow>(arrowFactory, player->GetActorLocation(), player->GetActorRotation());
		spawnedArrow->FireInDirection(spawnedArrow->GetActorForwardVector());
		accArrowSpeed = 0; // 가속도 0으로 초기화
	}

}



// UI열고 닫는 함수
void UAttackComponent::OnWeaponUI()
{

	player->compSkill->SkillMenuOnOff(false);

	if (!bWeaponMenu) //메뉴가 안열려 있을때 (!bSwitch)
	{
		WeaponMenuOnOff(true);
	}
	else 	// 현재 메뉴가 열려있을때 (bSwitch)
	{
		// 뷰포트에서 UI제거 (취소)
		WeaponMenuOnOff(false);
	}
}
//웨폰 UI 열고 닫는 함수
void UAttackComponent::WeaponMenuOnOff(bool value)
{
	bWeaponMenu = value;
	if (value)
	{
		player->ovelayMenuMainWG(weaponWidget);
		weaponWidget->SetUIInitPos((int32)(currAttackState));
	}
	else
	{
		weaponWidget->RemoveFromParent();

	}
	
}
// UI 열려있을때 아이콘 이동시키는 함수
void UAttackComponent::WeaponMenuMove(int32 value)
{
	if (bWeaponMenu)
	{
		weaponWidget->MoveUI(value);
	}
}
