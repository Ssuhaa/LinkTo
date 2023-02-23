// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChainWG.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API UChainWG : public UUserWidget
{
	GENERATED_BODY()

protected:


UPROPERTY(meta = (BindWidgetAnim), Transient)
class UWidgetAnimation* ChainPlay;

public:
 void AnimPlay();

};
