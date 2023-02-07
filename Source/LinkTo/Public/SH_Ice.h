// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "obstacleBase.h"
#include "SH_Ice.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API ASH_Ice : public AobstacleBase
{
	GENERATED_BODY()
	
protected:
	ASH_Ice();

public:

	void SetActiveIce(bool isActive);
	bool isIceVisible();

};
