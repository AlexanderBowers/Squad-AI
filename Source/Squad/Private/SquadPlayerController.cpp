// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"

void ASquadPlayerController::BeginPlay()
{
	if (ASquadPlayerController::GetPawn()) {
		ControlledPawn = GetPawn();
	}
}

void ASquadPlayerController::Tick(float DeltatTime)
{
}

void ASquadPlayerController::MoveUpCommand()
{
	if (ControlledPawn) {
		UE_LOG(LogTemp, Display, TEXT("Works!"));
		GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector End = CameraLocation + CameraRotation.Vector() * MaxRange;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ControlledPawn);

		FHitResult HitResult;

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility, CollisionParams);
			if (bHit)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Red, true, 60, 0, 1.f);
			}
	}
}

void ASquadPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveUpCommand", IE_Pressed, this, &ASquadPlayerController::MoveUpCommand);

}