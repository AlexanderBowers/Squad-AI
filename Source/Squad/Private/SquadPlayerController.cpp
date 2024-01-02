// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components.h"
#include "Engine/Engine.h"
#include "CommandComponent.h"
#include "SquadAIController.h"
#include <Kismet/GameplayStatics.h>
#include "UObject/Class.h"
#include "GameFramework/Character.h"

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
				if (CommandPoint.Type == FName("Investigate"))
				{
					USceneComponent* EndLocation = Actor->FindComponentByClass<USceneComponent>();
					if (EndLocation)
					{
						FVector RightLocation = EndLocation->GetComponentLocation();
						RightLocation.X += 200.f;
						CommandPoint.Location = RightLocation;
						DrawDebugSphere(GetWorld(), RightLocation, 20, 8, FColor::Red, false, 2, 0, 1.f);
						return CommandPoint;
						
					}
				}

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

TArray<AActor *> ASquadPlayerController::GetRooms(AActor* Building)
{
	//Search through all Rooms in the building
	//Check if a Room is cleared.
	//If it's not cleared, check if it's assigned.
	//If it isn't assigned, assign the first SquadMember who doesn't have a room assigned.
	Building->GetAllChildActors(RoomsInBuilding);
	for (AActor* Room : RoomsInBuilding)
	{
		FVector RoomLocation = Room->GetActorLocation();
		UChildActorComponent* BPRoom = Cast<UChildActorComponent>(Room);
		UClass* ActorClass = Room->GetClass();
		CheckRoomValues(ActorClass, Room);

	}
	return RoomsInBuilding;
}

void ASquadPlayerController::CheckRoomValues(UClass* ActorClass, AActor* Room)
{
	FProperty* IsCleared = ActorClass->FindPropertyByName(TEXT("bIsCleared"));
	if (IsCleared) // Checking to see if there is a property by name of bIsCleared
	{
		bool* ClearedValue = IsCleared->ContainerPtrToValuePtr<bool>(Room);

		if (!*ClearedValue) //Check to see if the value of bIsCleared is false.
		{
			FProperty* AssignedSquadMember = ActorClass->FindPropertyByName(TEXT("AssignedSquadMember"));
			if (AssignedSquadMember) //Check to see if there is a property by the name of AssignedSquadMember
			{
				ASquadAIController* AssignedValue = AssignedSquadMember->ContainerPtrToValuePtr<ASquadAIController>(Room);
				if (AssignedValue) //Check to see if AssignedSquadMember's value is a pointer of type ASquadAIController
				{
					if (AssignedValue->GetCharacter() == nullptr) //Check to see if the value of SquadMember's GetCharacter is a nullptr
					{
						AssignRoom(Room, AssignedValue);
						
					}
					else
					{
						AssignedValue = nullptr;
						AssignRoom(Room, AssignedValue);

					}
				}
			}
		}
	}
}

void ASquadPlayerController::AssignRoom(AActor* Room, ASquadAIController* AssignedValue)
{
	for (AActor* Member : SquadMembers)
	{
		ASquadAIController* Commando = Cast<ASquadAIController>(Member);
		if (Commando) //Check to see if the squad member is valid
		{
			if (Commando->Room == nullptr) //Check to see if the squad member has an assigned room alread
			{
				AssignedValue = Commando;
				Commando->Room = Room;
				Commando->ClearRoom();
				return;

			}
		}

	}
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