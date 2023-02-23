// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetWeaponSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/HorizontalBoxSlot.h>
#include "JS_SkillComponent.h"
#include "JS_Player.h"
#include <Kismet/GameplayStatics.h>
#include "AttackComponent.h"
#include "Components/PanelSlot.h"
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include "JS_LinkSound.h"



void UJS_WidgetWeaponSwitch::NativeConstruct()
{
	Super::NativeConstruct();

	widgetSlot = Cast<UPanelSlot>(SwitchPanel->Slot);
	slotPos = Cast<UCanvasPanelSlot>(widgetSlot);

	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Player::StaticClass()));
	sounds = player->compSound;
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
	float locX = -120.f * value + 120.f;
	FVector2D currWeapon = FVector2D(locX, 0);
	slotPos->SetPosition(currWeapon);
	SetWeaponText(currWeapon.X);
}

void UJS_WidgetWeaponSwitch::MoveUI(int32 thumbstickAxis)
{
	
	float locX = slotPos->GetPosition().X + thumbstickAxis * 120.f;
	float clampX = FMath::Clamp(locX,-120.f,120.f);
	FVector2D currWeapon = FVector2D(clampX, 0);
	slotPos->SetPosition(currWeapon);
	sounds->PlaySwitchSound();
	SetWeaponText(currWeapon.X);
}


void UJS_WidgetWeaponSwitch::SetWeaponText(float value)
{
	if (value == 0)
	{
		FString weapon = FString::Printf(TEXT("Sword"));
		FText weaponTex = FText::FromString(weapon);
		WeaponName->SetText(weaponTex);
	}
	else if (value == -120.f)
	{
		FString weapon = FString::Printf(TEXT("Bow"));
		FText weaponTex = FText::FromString(weapon);
		WeaponName->SetText(weaponTex);
	}
	else if (value == 120.f)
	{
		FString weapon = FString::Printf(TEXT("Idle"));
		FText weaponTex = FText::FromString(weapon);
		WeaponName->SetText(weaponTex);
	}
}

