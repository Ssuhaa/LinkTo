// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeLockBase.h"
#include "JS_SkillComponent.h"
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include "JS_Player.h"

ATimeLockBase::ATimeLockBase()
{
	bDamage = true;
	interationType = EObstacleType::Timelock;
	ConstructorHelpers::FObjectFinder <UMaterialParameterCollection> TempParm(TEXT("/Script/Engine.MaterialParameterCollection'/Game/Geometry/Material/TimeLock.TimeLock'"));
	if (TempParm.Succeeded())
	{
		timeLockParm = TempParm.Object;
	}
}

void ATimeLockBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimeLockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTimeLock)
	{
		if (isDelay(12))
		{
			releasedTimeLock();
		}
		if (currentTime > 7)
		{
			ChangeMaterial(TimeLockMatArray, 3, InteractionMesh);
			countTime += DeltaTime;
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), timeLockParm, TEXT("Speed"), countTime);
		}
	}
}

// Ÿ�Ӷ� ���͵� �ʷϻ����� ǥ��  ON/OFF
void ATimeLockBase::InteractionTimeLock(bool isOn)
{
	if (!bTimeLock)
	{
		if (isOn)
		{
			ChangeMaterial(TimeLockMatArray, 1, InteractionMesh);
		}
		else
		{
			ChangeMaterial(TimeLockMatArray, 0, InteractionMesh);
		}
		bLookin = false;
	}
}

// �ٶ󺸴� Ÿ�Ӷ� ���� ��������� ǥ��
void ATimeLockBase::LookInTimeLock()
{
	ChangeMaterial(TimeLockMatArray, 2, InteractionMesh);
	bLookin = true;
}

// Ÿ�Ӷ� �ɱ�
void ATimeLockBase::OnTimeLock()
{
	if (bLookin)
	{
		bTimeLock = true;
	}
}

// Ÿ�Ӷ� Ǯ��
void ATimeLockBase::releasedTimeLock()
{
	if (player->compSkill->isPressedG)
	{
		ChangeMaterial(TimeLockMatArray, 1, InteractionMesh);
	}
	else
	{
		ChangeMaterial(TimeLockMatArray, 0, InteractionMesh);
	}
	countTime = 0;
	bTimeLock = false;
	bLookin = false;
}
