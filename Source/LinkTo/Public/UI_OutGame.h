// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_OutGame.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API UUI_OutGame : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* T_Successed;
	
};
