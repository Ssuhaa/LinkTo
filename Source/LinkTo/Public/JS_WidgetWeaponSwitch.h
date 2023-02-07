// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackComponent.h"
#include "JS_WidgetBase.h"
#include "JS_WidgetWeaponSwitch.generated.h"

/**
 * 
 */

UCLASS()
class LINKTO_API UJS_WidgetWeaponSwitch : public UJS_WidgetBase
{
	GENERATED_BODY()
	
public:
// 	virtual void NativeOnInitialized() override;
// 	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	class AJS_Player* player;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* SwitchPanel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* IDLE;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* SWORD;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* BOW;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Selected;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	FVector2D movePos;

public:
	class UPanelSlot* widgetSlot;
	class UCanvasPanelSlot * slotPos;

public:
	void SetUIInitPos(EAttackState currAttackState);
	void MoveUI(int32 thumbstickAxis);
};
