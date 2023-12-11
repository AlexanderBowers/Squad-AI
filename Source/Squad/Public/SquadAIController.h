// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SquadAIController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveToCommand(FVector CommandLocation);

private:
	APlayerController* PlayerController;
};


