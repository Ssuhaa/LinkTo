// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetWeaponSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/HorizontalBoxSlot.h>
#include "JS_Player.h"
#include <Kismet/GameplayStatics.h>
#include "AttackComponent.h"
#include "Components/PanelSlot.h"
#include <UMG/Public/Components/HorizontalBox.h>

// void UJS_WidgetBase::NativeOnInitialized()
// {
// 	Super::NativeOnInitialized();
// }
// 
// void UJS_WidgetBase::NativePreConstruct()
// {
// 	Super::NativePreConstruct();
// }

void UJS_WidgetWeaponSwitch::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Player::StaticClass()));
	if (player != nullptr)
	{
		UAttackComponent* playerAttackComp = Cast<UAttackComponent>(player->compAttack);
	
		widgetSlot = Cast<UPanelSlot>(SwitchPanel->Slot);
		slotPos = Cast<UCanvasPanelSlot>(widgetSlot);
	}
	
}

void UJS_WidgetWeaponSwitch::NativeDestruct()
{
	Super::NativeDestruct();
}

void UJS_WidgetWeaponSwitch::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UJS_WidgetWeaponSwitch::SetUIInitPos(int32 value)
{
	float locX = -350.f * value + 350.f;
	FVector2D currWeapon = FVector2D(locX, -130.f);
	slotPos->SetPosition(currWeapon);
}

void UJS_WidgetWeaponSwitch::MoveUI(int32 thumbstickAxis)
{
	
	float locX = slotPos->GetPosition().X + thumbstickAxis * 350.f;
	float clampX = FMath::Clamp(locX,-350.f,350.f);
	FVector2D currWeapon = FVector2D(clampX, -130.f);
	slotPos->SetPosition(currWeapon);
}

