// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_ArrowFactory.generated.h"


UCLASS()
class LINKTO_API AJS_ArrowFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_ArrowFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 	class AJS_Player* player;
// 	class AJS_Arrow* arrow;
// 	FVector camLoc;
// 
// 	UPROPERTY(EditAnywhere, Category = Arrow)
// 	TSubclassOf<AJS_Arrow> arrowFactory;
// 
// 	UPROPERTY(EditAnywhere)
// 	TArray<class AJS_Arrow*> arrows;


/*private:*/
// 	void MakeArrow(FVector pos,FRotator rot);
// 	UFUNCTION()
// 	void AddArrow();

};
