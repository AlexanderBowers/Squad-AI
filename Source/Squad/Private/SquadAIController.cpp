// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadAIController.h"
#include "SquadPlayerController.h"

void ASquadAIController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFirstPlayerController())
	{
		PlayerController = Cast<ASquadPlayerController>(GetWorld()->GetFirstPlayerController());
	}
}


void ASquadAIController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("What?"))
}

void ASquadAIController::MoveToCommand(FVector CommandLocation)
{
	MoveToLocation(CommandLocation);
}
//ASquadPlayerController* NewPlayerController = PlayerController;
	//if (NewPlayerController)
	//{
		//if (NewPlayerController->Commands.Length > 0)
		//{

		//}
	//}

	/*if (PlayerController)
		{
			UE_LOG(LogTemp, Display, TEXT("Exists"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Doesn't exist"));
		}*/