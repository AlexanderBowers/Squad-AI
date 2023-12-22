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


void ASquadAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerController)
	{
		if (PlayerController->CommandList.Num() > 0)
		{
			MoveToCommand(PlayerController->CommandList.Last()); //Get the most recent command and prepare to move to it.
		}
	}
}

void ASquadAIController::MoveToCommand(FCommandPointy CommandPoint) //If they receive a new command, move to it.
{
	if (CommandPoint.Location != LastCommand.Location)
	{
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
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "HandleCommand", CommandPoint);
	float DistanceThreshold = 150.0f;
	float DistanceToCommand = FVector::Distance(GetPawn()->GetActorLocation(), CommandPoint.Location);
	if (DistanceToCommand <= DistanceThreshold)
	{
		StopMovement();
		if (CommandPoint.Type == FName("Cover"))
		{
			GetCharacter()->Crouch();
		
		}
		Delegate.Unbind();
	}
	else
	{
		
		GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 2.0f, false, 0.0f);
	}
}


	//TODO: Allow multiple Squad AI to respond to command. DONE!
	//Allow multiple types of commands to be implemented (cover, move, suppress)