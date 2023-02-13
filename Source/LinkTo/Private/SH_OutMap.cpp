// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_OutMap.h"
#include <Components/BoxComponent.h>
#include "UI_OutGame.h"

ASH_OutMap::ASH_OutMap()
{
	SetRootComponent(rootComp);
	InteractionMesh->DestroyComponent();
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetBoxExtent(FVector(1000,1000,400));
	ConstructorHelpers::FClassFinder <UUI_OutGame> Tempwg(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/BP_OutGame.BP_OutGame_C'"));
	if(Tempwg.Succeeded())
	{
		wgfactory = Tempwg.Class;
	}
}

void ASH_OutMap::BeginPlay()
{
	Super::BeginPlay();
	WGOutGame = CreateWidget<UUI_OutGame>(GetWorld(), wgfactory);
}

void ASH_OutMap::Tick(float DeltaTime)
{
	Super::	Tick(DeltaTime);
}

void ASH_OutMap::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AJS_Player* Player = Cast<AJS_Player>(OtherActor);
	if (Player != nullptr)
	{
		WGOutGame->AddToViewport();
	}
}

