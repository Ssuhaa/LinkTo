// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Arrow.h"

AJS_Arrow::AJS_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJS_Arrow::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AJS_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}