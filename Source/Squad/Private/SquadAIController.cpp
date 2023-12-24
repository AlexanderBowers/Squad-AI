// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadAIController.h"
#include "SquadPlayerController.h"
#include "Engine/EngineTypes.h"

#include "CommandPoint.h"
#include "SquadPlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"

void ASquadAIController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFirstPlayerController())
	{
		PlayerController = GetWorld()->GetFirstPlayerController<ASquadPlayerController>();

		//Giving them their own fake command to have something to compare to the first command.
		FCommandPointy BaseCommand;
		BaseCommand.Location = PlayerController->GetPawn()->GetActorLocation();
		BaseCommand.Type = FName("Move");
		LastCommand = BaseCommand;

	}
}

//NEXT: Force AI to jump when they enter a building!
//This will require:
//	What is a building?
//		How will AI know they are in a building? Overlap on Trigger Volume!
//			
//
//		BONUS: Assign Rooms to Squad members! -Assign when Player overlaps building!
//			How will AI know they are in a room ? -Room IDs & Overlap!
//				Start with 3 rooms for 3 squad members
//					Each should go into their own designated room
//						Leave Hallway alone for now
				
			
			
void ASquadAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerController)
	{
		if (bShouldFollow)
		{
			FollowPlayer();

		}
		if (PlayerController->CommandList.Num() > 0)
		{
			MoveToCommand(PlayerController->CommandList.Last()); //Get the most recent command and prepare to move to it.
			if (FVector::Distance(GetCharacter()->GetActorLocation(), PlayerController->GetPawn()->GetActorLocation()) >= 2000.0f)
			{
				bShouldFollow = true;
				FollowPlayer();

			}
		}
		
	}
	if (bIsIndoors)
	{
		
	}
}

void ASquadAIController::MoveToCommand(FCommandPointy CommandPoint) //If they receive a new command, move to it.
{
	if (CommandPoint.Location != LastCommand.Location)
	{
		bShouldFollow = false;
		if (GetCharacter()->bIsCrouched)
		{
			GetCharacter()->UnCrouch();

		}

		MoveToLocation(CommandPoint.Location, 25);
		HandleCommand(CommandPoint);
		LastCommand = PlayerController->CommandList.Last();

	}
}

void ASquadAIController::HandleCommand(FCommandPointy CommandPoint) //Check if they need to crouch, suppress, etc.
{
	//Called when AIController reached their destination
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "HandleCommand", CommandPoint);
	float DistanceThreshold = 150.0f;
	float DistanceToCommand = FVector::Distance(GetPawn()->GetActorLocation(), CommandPoint.Location);
	if (DistanceToCommand <= DistanceThreshold)
	{
		StopMovement();
		if (CommandPoint.Type == FName("Cover")) // trying to convert this to switch statement
		{ 
			GetCharacter()->Crouch();
		
		}
		if (CommandPoint.Type ==  FName("Return"))
		{
			bShouldFollow = true;
		}
		Delegate.Unbind();
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 2.0f, false, 0.0f);

	}
}

void ASquadAIController::FollowPlayer()
{
	if (GetCharacter()->bIsCrouched)
	{
		GetCharacter()->UnCrouch();

	}
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "FollowPlayer");
	MoveToLocation(PlayerController->GetPawn()->GetActorLocation(), 200);
	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, bShouldFollow, 0.0f);

}


	//TODO: Allow multiple Squad AI to respond to command. DONE!
	//Allow multiple types of commands to be implemented (cover, move, suppress)
//NEXT STEPS:
//Get Following player working. Use a boolean to determine whether the AI should stick to the player
