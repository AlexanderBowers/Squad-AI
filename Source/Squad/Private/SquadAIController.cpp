// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadAIController.h"
#include "SquadPlayerController.h"
#include "Engine/EngineTypes.h"
#include "AssignMemberInterface.h"
#include "CommandPoint.h"
#include "SquadPlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"

void ASquadAIController::BeginPlay()
{
	Super::BeginPlay();
	if (this->GetPawn()->Implements<UAssignMemberInterface>())
	{
		IAssignMemberInterface::Execute_SetBehaviorTree(this->GetPawn(), this);
	}
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
			if (PlayerController->CommandList.Last().Location != LastCommand.Location)
			{
				MoveToCommand(PlayerController->CommandList.Last()); //Get the most recent command and prepare to move to it.

			}
		}
		
		if (FVector::Distance(GetCharacter()->GetActorLocation(), PlayerController->GetPawn()->GetActorLocation()) >= 1000.0f)
		{
			bShouldFollow = true;
			FollowPlayer();

		}
	}
}

void ASquadAIController::MoveToCommand(FCommandPointy CommandPoint) //If they receive a new command, move to it.
{
	if (CommandPoint.Type == FName("Return"))
	{
		ResetPriorityCommand();
	}
	if (bHasPriority == true)
	{
		return;
	}
	if (CommandPoint.Location.X == 0.00f)
	{
		return;
	}
		bShouldFollow = false;
		if (GetCharacter()->bIsCrouched)
		{
			GetCharacter()->UnCrouch();

		}
		MoveToLocation(CommandPoint.Location, 25);
		HandleCommand(CommandPoint);
		LastCommand = CommandPoint;

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
			ResetPriorityCommand();
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
	ResetPriorityCommand();

	if (GetCharacter()->bIsCrouched)
	{
		GetCharacter()->UnCrouch();

	}
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "FollowPlayer");
	MoveToLocation(PlayerController->GetPawn()->GetActorLocation(), 200);
	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, bShouldFollow, 0.0f);
	Delegate.Unbind();
}

void ASquadAIController::ClearRoom()
{
	if (Room != nullptr)
	{
		bShouldFollow = false;
		FCommandPointy RoomPoint;
		FVector Test = Room->GetActorLocation();
		RoomPoint.Location = Room->GetActorLocation();
		RoomPoint.Type = FName("Cover");
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ClearRoom");
		MoveToCommand(RoomPoint);
		float DistanceThreshold = 150.0f;
		float DistanceToCommand = FVector::Distance(GetPawn()->GetActorLocation(), RoomPoint.Location);
		if (DistanceToCommand <= DistanceThreshold)
		{
			bShouldFollow = true;
			Delegate.Unbind();

		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 5.0f, false, 5.0f);

		}
	}

}

void ASquadAIController::ResetPriorityCommand()
{
	FVector ResetLocation = { 0.00f, 0.00f, 0.00f };
	PriorityCommand.Location = ResetLocation;
	if (AssignedPosition)
	{
		if (AssignedPosition->Implements<UAssignMemberInterface>())
		{
			IAssignMemberInterface::Execute_ResetAssignedMember(AssignedPosition);
		}
	}
	AssignedPosition = nullptr;
	bShouldFollow = true;
	bHasPriority = false;
	return;
}



	//TODO: Allow multiple Squad AI to respond to command. DONE!
	//Allow multiple types of commands to be implemented (cover, move, suppress)
//NEXT STEPS:
//Get Following player working. Use a boolean to determine whether the AI should stick to the player. DONE!
