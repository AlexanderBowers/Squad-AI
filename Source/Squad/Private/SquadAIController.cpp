// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadAIController.h"
#include "SquadPlayerController.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "CommandPoint.h"
#include "SquadPlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"

void ASquadAIController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFirstPlayerController())
	{

		//Create their own list of commands. Compare it to the other list on tick.
		//If the list is different, go to the last element of the array,
		PlayerController = GetWorld()->GetFirstPlayerController<ASquadPlayerController>();

		FCommandPointy BaseCommand;
		BaseCommand.Location = PlayerController->GetPawn()->GetActorLocation();
		BaseCommand.Type = FName("Move");
		LastCommand = BaseCommand;
	}
}


void ASquadAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ACharacter* Character = this->GetCharacter(); // this is stupid and doesn't want to work.

	if (PlayerController)
	{
		//If the location of the last command is the same as the previous, do nothing.
		// 
		//If there are any vectors in the command list, go to it and remove it from the list.
		if (PlayerController->CommandList.Num() > 0)
		{
			MoveToCommand(PlayerController->CommandList.Last());
		}
	}
}

void ASquadAIController::MoveToCommand(FCommandPointy CommandPoint)
{

	if (CommandPoint.Location != LastCommand.Location)
	{
		MoveToLocation(CommandPoint.Location, 20);
		LastCommand = PlayerController->CommandList.Last();

		if(GetPathFollowingComponent()->HasReached(CommandPoint.Location))
		if (LastCommand.Type == FName("Cover"))
		{
			GetCharacter()->Jump(); //Using Jump just to see if they're doing it before they reach their intended destination. They currently are.
		}
	}
}

void ASquadAIController::HandleCommand(FCommandPointy CommandPoint)
{
}


	//TODO: Allow multiple Squad AI to respond to command. DONE!
	//Allow multiple types of commands to be implemented (cover, move, suppress)
	//Assign points in environment for cover and suppress; otherwise move.




//Perhaps multiple AI can't respond to the command because it's getting removed too quickly? Should I use a timer?
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
