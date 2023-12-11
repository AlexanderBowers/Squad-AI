// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadAIController.h"
#include "SquadPlayerController.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"

void ASquadAIController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFirstPlayerController())
	{
		PlayerController = GetWorld()->GetFirstPlayerController<ASquadPlayerController>();
	}
}


void ASquadAIController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	
	if (PlayerController)
	{
		//If there are any vectors in the command list, go to it and remove it from the list.
		if (PlayerController->Commands.Num() > 0)
		{
			FVector &Location = PlayerController->Commands[0];
			MoveToCommand(Location);
			if (&Location)
			{
				
				PlayerController->Commands.RemoveAt(0);
				
			}
		}
	}
}

void ASquadAIController::MoveToCommand(FVector CommandLocation)
{
	MoveToLocation(CommandLocation);
}


	//TODO: Allow multiple Squad AI to respond to command. 
	//Allow multiple types of commands to be implemented (cover, move, suppress)
	//Assign points in environment for cover and suppress; otherwise move.




//Perhaps multiple AI can't respond to the command because it's getting removed too quickly? Should I use a timer?
// 
// 
	////Timers need a handle, delegate, and float. Delegate is the function and in this case is a lambda function.
	//FTimerHandle DelayTimerHandle;
	//float DelayInSeconds = 5.0f;
	//// Define the function to be executed after the delay
	//FTimerDelegate DelayDelegate;
	//DelayDelegate.BindLambda([]() {

	//	UE_LOG(LogTemp, Warning, TEXT("Delayed action executed after 5 seconds"));

	//	});

	//// Start the timer
	//GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, DelayDelegate, DelayInSeconds, false);
