// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Ice.h"

ASH_Ice::ASH_Ice()
{
	SetRootComponent(rootComp);
	InteractionMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMash(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMash.Succeeded())
	{
		InteractionMesh->SetStaticMesh(tempMash.Object);
	}
}

void ASH_Ice::SetActiveIce(bool isActive)
{
	 if (isActive)
	 {
		 InteractionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	 }
	 else
	 {
		 InteractionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 }

	 InteractionMesh->SetVisibility(isActive);
}

bool ASH_Ice::isIceVisible()
{
	return InteractionMesh->IsVisible();
}
