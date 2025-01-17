// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JS_SkillComponent.h"
#include "JS_WidgetBase.h"
#include "JS_WidgetSkillSwitch.generated.h"

UCLASS()
class LINKTO_API UJS_WidgetSkillSwitch : public UJS_WidgetBase
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	class AJS_Player* player;
	class UJS_LinkSound* sounds;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* SwitchPanel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* IDLE;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* BOMB;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* MAGNET;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* TIMELOCK;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ICE;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Selected;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* skillName;
	UPROPERTY(EditAnywhere)
	FVector2D movePos;
	

public:
	UPROPERTY(EditAnywhere)
	class UPanelSlot* widgetSlot;
	UPROPERTY(EditAnywhere)
	class UCanvasPanelSlot * slotPos;

public:
	void SetUIInitPos(int32 value);
	void MoveUI(int32 value);
	void SetSkillText(float value);
};