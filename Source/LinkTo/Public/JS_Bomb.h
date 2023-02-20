// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JS_WeaponBase.h"
#include "JS_Bomb.generated.h"

/**
 * 
 */
UCLASS()
class LINKTO_API AJS_Bomb : public AJS_WeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJS_Bomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	class AJS_Player* player;
	UPROPERTY(EditAnywhere)
	class USphereComponent* compSphere;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere)
	class URadialForceComponent* compRadial;

	
	void SetActiveBomb(bool isActive);
	void Explose();
};
