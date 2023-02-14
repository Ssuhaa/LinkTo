// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_Sword.generated.h"

UCLASS()
class LINKTO_API AJS_Sword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* compAxis;
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* compCapsule;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
};
