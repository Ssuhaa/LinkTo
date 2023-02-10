// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Door.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Door : public AobstacleBase
{
	GENERATED_BODY()
	
	ASH_Door();

protected:
	class ASH_Goal* Goal;
};
