// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"

void ASquadPlayerController::BeginPlay()
{
}

void ASquadPlayerController::Tick(float DeltatTime)
{
}

void ASquadPlayerController::MoveUpCommand(UInputComponent* PlayerInput)
{
	if (ASquadPlayerController::Camera) {
		//Do line trace from camera to first collision; draw a debug line at location.
	}
}
