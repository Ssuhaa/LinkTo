// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JS_LinkSound.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKTO_API UJS_LinkSound : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJS_LinkSound();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	class AJS_Player* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* attack1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* attack7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* beltMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* beltMoveNear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* BGM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* iceFail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* iceMaking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* jump1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* jump2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* jump3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* magnetHold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* magnetShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* moveFast;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* moveSlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* seesaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* sheikerClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* shikerOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* slideSelect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* wind1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* wind2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* shrineOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* parasailOpen;



	

		
};
