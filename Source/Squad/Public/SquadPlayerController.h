// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommandPoint.h"
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
	TArray<UCommandPoint> *CommandList;

private:

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;
	UPROPERTY(EditAnywhere)

	FVector CameraLocation;
	FRotator CameraRotation;
	APawn* ControlledPawn;
	

};
