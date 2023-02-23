// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_MovingWalk.h"
#include <Components/BoxComponent.h>
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>
#include "SH_MWButton.h"
#include "SH_Ball.h"
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <UMG/Public/Components/WidgetComponent.h>


ASH_MovingWalk::ASH_MovingWalk()
{

	SetRootComponent(rootComp);
	WGcomp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMesh.Object);
	}
	InteractionMesh->SetRelativeScale3D(FVector(3.0f, 4.0f, 0.5f));
	InteractionMesh->SetupAttachment(rootComp);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(InteractionMesh);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 60));
	BoxCollision->SetBoxExtent(FVector(48, 48, 10));

	dirComp = CreateDefaultSubobject<UArrowComponent>(TEXT("DirComp"));
	dirComp->SetupAttachment(InteractionMesh);
	dirComp->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder <UMaterialParameterCollection> tempParm(TEXT("/Script/Engine.MaterialParameterCollection'/Game/Geometry/Material/Panning.Panning'"));
	if (tempParm.Succeeded())
	{
		panParm = tempParm.Object;
	}

	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_MovingWalk.MI_MovingWalk'"));
	if (TempMat.Succeeded())
	{
		TimeLockMatArray.Add(TempMat.Object);
		InteractionMesh->SetMaterial(0, TempMat.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_MovingWalk_TimeLcokOn.MI_MovingWalk_TimeLcokOn'"));
	if (TempMat1.Succeeded())
	{
		TimeLockMatArray.Add(TempMat1.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_MovingWalk_TimeLcokSelect.MI_MovingWalk_TimeLcokSelect'"));
	if (TempMat2.Succeeded())
	{
		TimeLockMatArray.Add(TempMat2.Object);
	}
	ConstructorHelpers::FObjectFinder <UMaterialInstance> TempMat3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Geometry/Material/MI_MovingWalk_TimeLcokCount.MI_MovingWalk_TimeLcokCount'"));
	if (TempMat3.Succeeded())
	{
		TimeLockMatArray.Add(TempMat3.Object);
	}

}

void ASH_MovingWalk::BeginPlay()
{
	Super::BeginPlay();

	ConButton = Cast<ASH_MWButton>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_MWButton::StaticClass()));

}

void ASH_MovingWalk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bTimeLock)
	{
		if (ConButton->binButton)
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), panParm, TEXT("Pan"), MovingWalkSpeed);
			FVector Dir = dirComp->GetForwardVector() * DeltaTime * force;
			if (Target != nullptr)
			{
				Target->AddActorWorldOffset(Dir);

			}
		}
		else
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), panParm, TEXT("Pan"), -MovingWalkSpeed);
			FVector Dir = -dirComp->GetForwardVector() * DeltaTime * force;
			if (Target != nullptr)
			{
				Target->AddActorWorldOffset(Dir);

			}
		}
	}
}

void ASH_MovingWalk::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Target = OtherActor;
}

void ASH_MovingWalk::NotifyActorEndOverlap(AActor* OtherActor)
{
	Target = nullptr;
}

void ASH_MovingWalk::OnTimeLock()
{
	Super::OnTimeLock();
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), panParm, TEXT("Pan"), 0.0);
}

void ASH_MovingWalk::releasedTimeLock()
{
	Super::releasedTimeLock();
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), panParm, TEXT("Pan"), MovingWalkSpeed);
}
