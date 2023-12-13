// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "CommandComponent.h"
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
			//if we get a collision, create a FCommandPointy. 
			//If the collided actor has a Command Component, get its type and add to the CommandList.
			FCommandPointy CommandPoint;
			CommandPoint.Location = HitResult.Location;
			AActor* Actor = HitResult.GetActor();
			if (Actor)
			{
				UActorComponent *Component = Actor->FindComponentByClass<UCommandComponent>();

				//to-do: check component reference and look at its tag.
			}

			CommandPoint.Type = TEXT("Move");

			CommandList.Add(CommandPoint);
			DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Red, false, 60, 0, 1.f);
		}
	}
}

void ASquadPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveUpCommand", IE_Pressed, this, &ASquadPlayerController::MoveUpCommand);

}