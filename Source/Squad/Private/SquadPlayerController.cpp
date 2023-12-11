// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"

void ASquadPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ASquadPlayerController::GetPawn()) {
		ControlledPawn = GetPawn();
	}
}

void ASquadPlayerController::Tick(float DeltatTime)
{
}

void ASquadPlayerController::MoveUpCommand()
{
	//Line trace to a location. If it hits something, add the hit result vector to a list.
	//to-do: AI need to check for items in the array then go to them. When they're there, remove the location from the array.
	//future: struct to store vector and type? (move, cover, shoot, breach)
	if (ControlledPawn) {
		GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector End = CameraLocation + CameraRotation.Vector() * MaxRange;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ControlledPawn);

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility, CollisionParams);
		if (bHit)
		{
			Commands.Add(HitResult.Location);
			DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Red, true, 60, 0, 1.f);
		}
		FString ArrayAsString;
		for (FVector& Element : Commands)
		{
			ArrayAsString.Append(FString::Printf(TEXT("(X=%.2f, Y=%.2f, Z=%.2f), "), Element.X, Element.Y, Element.Z));
		}
		UE_LOG(LogTemp, Warning, TEXT("Vector Array Contents: %s"), *ArrayAsString);
	}
}

void ASquadPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveUpCommand", IE_Pressed, this, &ASquadPlayerController::MoveUpCommand);

}