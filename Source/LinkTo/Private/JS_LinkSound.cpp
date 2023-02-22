// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LinkSound.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include "JS_Player.h"

// Sets default values for this component's properties
UJS_LinkSound::UJS_LinkSound()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJS_LinkSound::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AJS_Player>(GetOwner());
	UGameplayStatics::PlaySound2D(this, shrineOpen);
	// ...
	
}


// Called every frame
void UJS_LinkSound::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

