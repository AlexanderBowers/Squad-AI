// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "CommandComponent.h"
#include "SquadAIController.h"
#include <Kismet/GameplayStatics.h>

void ASquadPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ASquadPlayerController::GetPawn())
	{
		ControlledPawn = GetPawn();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASquadAIController::StaticClass(), SquadMembers); // Get all Squad Member controllers.
	}

}


FCommandPointy ASquadPlayerController::CreateCommandPointy(FHitResult HitResult)
{
	//if we get a collision, create a FCommandPointy. 
	//If the collided actor has a Command Component, get its type and add to the CommandList.

	FCommandPointy CommandPoint;
	CommandPoint = AssignLocation(CommandPoint, HitResult);
	CommandPoint = AssignType(CommandPoint, HitResult);
	return CommandPoint;
}

FCommandPointy ASquadPlayerController::AssignLocation(FCommandPointy CommandPoint, FHitResult HitResult)
{
	CommandPoint.Location = HitResult.Location;
	return CommandPoint;
}

FCommandPointy ASquadPlayerController::AssignType(FCommandPointy CommandPoint, FHitResult HitResult)
{
	AActor* Actor = HitResult.GetActor();
	if (Actor)
	{
		UActorComponent* Component = Actor->FindComponentByClass<UCommandComponent>();
		if (Component)
		{
			FString TagType = Component->ComponentTags[0].ToString();
			if (TagType.Len() > 0)
			{
				CommandPoint.Type = FName(TagType);
				DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Green, false, 2, 0, 1.f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Component but not tag!"))
				CommandPoint.Type = FName("Move");
			}
		}
		else //If there is no component, default the type to move.
		{
			CommandPoint.Type = FName("Move");
			DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Red, false, 2, 0, 1.f);
		}
		return CommandPoint;
	}
	//If there is no actor hit, return to the player
	UE_LOG(LogTemp, Warning, TEXT("No actor found. Returning to player."))
	CommandPoint.Location = this->GetPawn()->GetActorLocation();
	CommandPoint.Type = FName("Move");
	return CommandPoint;
}

TArray<AActor*> ASquadPlayerController::GetRooms(AActor* Building)
{
	RoomsInBuilding;  //Get all components that match BP_Room in BP_Building
									  //After, Assign a room to each squad member inside of AssignRoom
									  // 
									  //BP_Room needs a boolean of IsCleared 
									  //if AIController overlaps it, set to true.
									  // Inside AssignRoom, If a room isn't cleared, and there's no assigned AI, assign to the first squad member that doesn't have a room assigned
									  //If there are leftover rooms, set a timer to rerun AssignRoom
		


	return RoomsInBuilding;
}

void ASquadPlayerController::Tick(float DeltatTime)
{

}

void ASquadPlayerController::MoveUpCommand()
{
	//Line trace to a location. On collision, create a FCommandPointy then add it to the list.

	if (ControlledPawn)
	{
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
			FCommandPointy CommandPoint = CreateCommandPointy(HitResult);
			CommandList.Add(CommandPoint);

			
			
		}
	}
}

void ASquadPlayerController::FormUpCommand()
{
	if (ControlledPawn)
	{
		FCommandPointy CommandPoint;
		CommandPoint.Location = ControlledPawn->GetActorLocation();
		CommandPoint.Type = FName("Return");
		CommandList.Add(CommandPoint);
		DrawDebugSphere(GetWorld(), ControlledPawn->GetActorLocation(), 20, 20, FColor::Purple, false, 2, 0, 1.f);
	}
}

void ASquadPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveUpCommand", IE_Pressed, this, &ASquadPlayerController::MoveUpCommand);
	InputComponent->BindAction("FormUpCommand", IE_Pressed, this, &ASquadPlayerController::FormUpCommand);

}