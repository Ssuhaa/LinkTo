// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetSkillSwitch.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/HorizontalBoxSlot.h>
#include <Kismet/GameplayStatics.h>
#include "JS_SkillComponent.h"
#include "Components/PanelSlot.h"
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include "JS_Player.h"


void UJS_WidgetSkillSwitch::NativeConstruct()
{
	Super::NativeConstruct();
	widgetSlot = Cast<UPanelSlot>(SwitchPanel->Slot);
	slotPos = Cast<UCanvasPanelSlot>(widgetSlot);


	player = Cast<AJS_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Player::StaticClass()));
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
	float locX = -120.f * value + 240.f;
	FVector2D currSkill = FVector2D(locX, 0);
	slotPos->SetPosition(currSkill);
	SetSkillText(currSkill.X);
}

void UJS_WidgetSkillSwitch::MoveUI(int32 value)
{

	float locX = slotPos->GetPosition().X + value * 120.f;
	float clampX = FMath::Clamp(locX, -240.f, 240.f);
	FVector2D currSkill = FVector2D(clampX, 0);
	slotPos->SetPosition(currSkill);
	SetSkillText(currSkill.X);
}

void UJS_WidgetSkillSwitch::SetSkillText(float value)
{
	if (value == 0)
	{	
		FString skill = FString::Printf(TEXT("Magnet Catch"));
		FText skillTex = FText::FromString(skill);
		skillName->SetText(skillTex);
	}
	else if (value == -120.f)
	{
		FString skill = FString::Printf(TEXT("Time Lock"));
		FText skillTex = FText::FromString(skill);
		skillName->SetText(skillTex);

	}
	else if (value == -240.f)
	{
		FString skill = FString::Printf(TEXT("Ice Maker"));
		FText skillTex = FText::FromString(skill);
		skillName->SetText(skillTex);

	}
	else if (value == 120.f)
	{
		FString skill = FString::Printf(TEXT("Remote Bomb"));
		FText skillTex = FText::FromString(skill);
		skillName->SetText(skillTex);
	}
	else
	{
		FString skill = FString::Printf(TEXT("Default"));
		FText skillTex = FText::FromString(skill);
		skillName->SetText(skillTex);
	}
}
