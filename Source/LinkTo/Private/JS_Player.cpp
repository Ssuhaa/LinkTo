// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Player.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MoveComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerStateComponent.h"
#include "AttackComponent.h"
#include "SH_Ice.h"
#include <Kismet/KismetMathLibrary.h>
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "JS_SkillComponent.h"
#include "Components/CapsuleComponent.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <UMG/Public/Components/Overlay.h>
#include "PlayerMainWG.h"
#include "JS_LinkSound.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "JS_WidgetPointComponent.h"


AJS_Player::AJS_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	compCam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	compCam->SetupAttachment(RootComponent);

	meshHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HEAD"));
	meshHead->SetupAttachment(compCam);
	meshHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LEFT CONTROLLER"));
	leftController->SetupAttachment(RootComponent);
	leftController->MotionSource = "Left";
	
	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEFT HAND"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RIGHT CONTROLLER"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIGHT HAND"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	MagnetGrabComp = CreateDefaultSubobject<USceneComponent>(TEXT("MagnetGrabPos"));
	MagnetGrabComp->SetupAttachment(RootComponent);
	MagnetGrabComp->SetRelativeLocation(FVector(400, 0, 120));

	MagnetHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("MagnetHandle"));
	MagnetHandle->LinearDamping = 150.0f;
	MagnetHandle->LinearStiffness = 500.f;
	MagnetHandle->AngularDamping = 150.0f;
	MagnetHandle->AngularStiffness = 500.f;
	MagnetHandle->InterpolationSpeed = 2.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	GetCharacterMovement()->MaxFlySpeed = 700.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1000.f;
	GetCharacterMovement()->JumpZVelocity = 400.f; 
	GetCharacterMovement()->AirControl = 0.2;
	JumpMaxCount = 1;

	compMove = CreateDefaultSubobject<UMoveComponent>(TEXT("MOVE COMP"));
	compState = CreateDefaultSubobject<UPlayerStateComponent>(TEXT("STATE COMP"));
	compAttack = CreateDefaultSubobject<UAttackComponent>(TEXT("ATTACK COMP"));
	compSkill = CreateDefaultSubobject<UJS_SkillComponent>(TEXT("SKILL COMP"));
	compSound = CreateDefaultSubobject<UJS_LinkSound>(TEXT("SOUND COMP"));
	
	compBow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOW"));
	compBow->SetupAttachment(RootComponent);
	compBow->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	compSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SWORD"));
	compSword->SetupAttachment(rightHand);
	compSword->SetCollisionProfileName(TEXT("NoCollision"));
	compSword->SetRelativeLocation(FVector(30,8,0));
	compSword->SetRelativeRotation(FRotator(0,90,90));


	MagNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("magNScomp"));
	MagNS->SetupAttachment(leftController);
	MagNS->SetVisibility(false);


	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	ConstructorHelpers::FClassFinder <UUserWidget> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/MainUI.MainUI_C'"));
	if (tempWG.Succeeded())
	{
		widgetComp->SetWidgetClass(tempWG.Class);
	}
	widgetComp->SetGeometryMode(EWidgetGeometryMode::Cylinder);
	widgetComp->SetupAttachment(compCam);
	widgetComp->SetDrawSize(FVector2D(1920,1080));
	widgetComp->SetRelativeRotation(FRotator(0,180,0));
	widgetComp->SetRelativeLocation(FVector(628.0f, 0.0f, -51.0f));

	leftWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Left_Widget"));
	ConstructorHelpers::FClassFinder <UUserWidget> templeftWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/MainUI.MainUI_C'"));
	if (templeftWidget.Succeeded())
	{
		leftWidgetComp->SetWidgetClass(templeftWidget.Class);
	}
	leftWidgetComp->SetupAttachment(leftHand);
	leftWidgetComp->SetDrawSize(FVector2D(60, 60));
	leftWidgetComp->SetRelativeRotation(FRotator(0, 180, 0));

	rightWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Right_Widget"));
	ConstructorHelpers::FClassFinder <UUserWidget> temprightWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/MainUI.MainUI_C'"));
	if (temprightWidget.Succeeded())
	{
		rightWidgetComp->SetWidgetClass(temprightWidget.Class);
	}
	rightWidgetComp->SetupAttachment(rightHand);
	rightWidgetComp->SetDrawSize(FVector2D(60, 60));
	rightWidgetComp->SetRelativeRotation(FRotator(0, 0, 0));
	
	compWidgetInter = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WIDGET INTERACTION"));
	compWidgetInter->SetupAttachment(rightController);
	
	compWidgetPoint = CreateDefaultSubobject<UJS_WidgetPointComponent>(TEXT("WIDGET POINT"));

	

}

// Called when the game starts or when spawned
void AJS_Player::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());
	subsys->AddMappingContext(myMapping, 0);

	MainWG = Cast<UPlayerMainWG>(widgetComp->GetWidget());
/*	sword = GetWorld()->SpawnActor<AJS_Sword>(swordFactory, rightHand->GetComponentTransform());*/
	
}

// Called every frame
void AJS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AJS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// PlayerInputComponent를 EnhancedInputComponent로 캐스팅
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		compMove->SetupPlayerInputComponent(enhancedInputComponent);
		compAttack->SetupPlayerInputComponent(enhancedInputComponent);
		compSkill->SetupPlayerInputComponent(PlayerInputComponent);
		compWidgetPoint->SetupPlayerInputComponent(enhancedInputComponent);
	}
	
}
void AJS_Player::ContactKillZone()
{
	compState->HP -= 1;
	SetActorLocation(FVector(1300,-1300,300));
}

void AJS_Player::ovelayMenuMainWG(UUserWidget* widget)
{
	MainWG->Overlay_Skill->AddChildToOverlay(widget);
}

