// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"

void ASquadPlayerController::BeginPlay()
{
	if (ASquadPlayerController::GetPawn()) {
		ControlledPawn = GetPawn();
		
		
		

	}
}

void ASquadPlayerController::Tick(float DeltatTime)
{
	GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector End = CameraLocation + CameraRotation.Vector() * MaxRange;
	DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Red, false, 0, 10);
}

void ASquadPlayerController::MoveUpCommand(UInputComponent* playerinput)
{
	if (ControlledPawn) {
		UE_LOG(LogTemp, Display, TEXT("Works!"));
	}
}

void ASquadPlayerController::SetupInputComponent(UInputComponent* PlayerInputComponent) override
{
	Super::SetupInputComponent();
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASquadPlayerController::MoveUpCommand);

}