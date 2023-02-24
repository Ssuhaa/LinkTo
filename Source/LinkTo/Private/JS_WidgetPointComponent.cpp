// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetPointComponent.h"
#include "JS_Player.h"
#include "Components/WidgetInteractionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values for this component's properties
UJS_WidgetPointComponent::UJS_WidgetPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UInputAction>tempSelect(TEXT("/Script/EnhancedInput.InputAction'/Game/MyInputs/Oculus/Right/JS_IA_ThumStickClick.JS_IA_ThumStickClick'"));
	if (tempSelect.Succeeded())
	{
		select = tempSelect.Object;
	}
	// ...
}


// Called when the game starts
void UJS_WidgetPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = Cast<AJS_Player>(GetOwner());
}


// Called every frame
void UJS_WidgetPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJS_WidgetPointComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(select, ETriggerEvent::Started, this, &UJS_WidgetPointComponent::ClickTrigger); 
	PlayerInputComponent->BindAction(select, ETriggerEvent::Completed, this, &UJS_WidgetPointComponent::ReleaseTrigger); 
	

}

void UJS_WidgetPointComponent::ClickTrigger()
{
	player->compWidgetInter->PressPointerKey(EKeys::LeftMouseButton);
}
void UJS_WidgetPointComponent::ReleaseTrigger()
{
	player->compWidgetInter->ReleasePointerKey(EKeys::LeftMouseButton);
}