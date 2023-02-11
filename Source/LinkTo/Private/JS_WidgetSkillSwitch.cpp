// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetSkillSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/HorizontalBoxSlot.h>
#include "JS_Player.h"
#include <Kismet/GameplayStatics.h>
#include "JS_SkillComponent.h"
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

void UJS_WidgetSkillSwitch::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Player::StaticClass()));
	if (player != nullptr)
	{
		UJS_SkillComponent* playerSkillComp = Cast<UJS_SkillComponent>(player->compSkill);

		widgetSlot = Cast<UPanelSlot>(SwitchPanel->Slot);
		slotPos = Cast<UCanvasPanelSlot>(widgetSlot);
	}

}

void UJS_WidgetSkillSwitch::NativeDestruct()
{
	Super::NativeDestruct();
}

void UJS_WidgetSkillSwitch::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UJS_WidgetSkillSwitch::SetUIInitPos(int32 value)
{
	float locX = -350.f * value + 700.f;
	FVector2D currSkill = FVector2D(locX, -130.f);
	slotPos->SetPosition(currSkill);
}

void UJS_WidgetSkillSwitch::MoveUI(int32 value)
{

	float locX = slotPos->GetPosition().X + value * 350.f;
	float clampX = FMath::Clamp(locX, -700.f, 700.f);
	FVector2D currSkill = FVector2D(clampX, -130.f);
	slotPos->SetPosition(currSkill);
}
