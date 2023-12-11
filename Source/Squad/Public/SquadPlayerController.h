// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SquadPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class SQUAD_API ASquadPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	//APlayerController();
	
protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltatTime) override;

	UFUNCTION()
	void MoveUpCommand();

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere)
	TArray<FVector> Commands;

private:

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;
	UPROPERTY(EditAnywhere)
	//UInputComponent InputComponent;

	FVector CameraLocation;
	FRotator CameraRotation;
	APawn* ControlledPawn;
	

};
