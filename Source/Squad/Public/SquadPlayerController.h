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
	FVector CameraLocation;
	FRotator CameraRotation;
	APawn* ControlledPawn;

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltatTime) override;

	virtual void MoveUpCommand(class UInputComponent* PlayerInput);
};
