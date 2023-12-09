// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

void ASquadPlayerController::BeginPlay()
{
	if (ASquadPlayerController::GetPawn()) {
		ControlledPawn = GetPawn();
		GetPlayerViewPoint(CameraLocation, CameraRotation);
	}
}

void ASquadPlayerController::Tick(float DeltatTime)
{
	DrawDebugPoint(GetWorld(), CameraLocation, 50, FColor::Red, false, 30, 1);
}

void ASquadPlayerController::MoveUpCommand(UInputComponent* PlayerInput)
{
	if (ControlledPawn) {

		
		//UActorComponent &Camera = GetComponentByClass<ControlledPawn>(UCameraComponent);
		//Do line trace from camera to first collision; draw a debug line at location.
		
		
	}
}
