// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
//#include "CommandPoint.h"

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
	//Line trace to a location. On collision, create a FCommandPointy then add it to the list.
	//TODO: Check if the hit object has a CommandComponent. If it does, create a FCommandPointy with its location and type.

	if (ControlledPawn) {
		GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector End = CameraLocation + CameraRotation.Vector() * MaxRange;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ControlledPawn);

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility, CollisionParams);
		if (bHit)
		{
			//just testing move for now. next step is checking if there's a component. if there is, which type it is.
			FCommandPointy Command;
			Command.Location = HitResult.Location;
			Command.Type = TEXT("Move");

			CommandList.Add(Command);
			DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Red, true, 60, 0, 1.f);
		}
		FString ArrayAsString;
		for (FCommandPointy& Element : CommandList)
		{
			ArrayAsString.Append(FString::Printf(TEXT("(X=%.2f, Y=%.2f, Z=%.2f), "), Element.Location.X, Element.Location.Y, Element.Location.Z));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Vector Array Contents: %s"), *ArrayAsString);
	}
}

void ASquadPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveUpCommand", IE_Pressed, this, &ASquadPlayerController::MoveUpCommand);

}