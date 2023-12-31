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
#include "AssignMemberInterface.h"
#include "GameFramework/Character.h"

void ASquadPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ASquadPlayerController::GetPawn())
	{
		ControlledPawn = GetPawn();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASquadAIController::StaticClass(), SquadMembers); // Get all Squad Member controllers.
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASquadAIController::StaticClass(), DisposableList); // Get all Squad Member controllers.
		
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
{  //Starting location of a CommandPoint to be given to an AI. This may change depending on its type.
	CommandPoint.Location = HitResult.Location;
	return CommandPoint;

}

FCommandPointy ASquadPlayerController::AssignType(FCommandPointy CommandPoint, FHitResult HitResult)
{ //CommandPoints can have a variety of types. 
  //Move: self explanatory. This is the default fallback.
  //Cover: crouch at location; this is set in SquadAIController::HandleCommand
  //Investigate: CommandPoint.Location gets set to a specific component.
  //FirePoint: One AI gets set this as a priority to move to or recalled from. See SquadPlayerController::GetAvailableMembers() for assignment.
  //Return: used in SquadPlayerController::FormUpCommand. Otherwise used as a backup if there was no actor found from the HitResult.
  
	AActor* Actor = HitResult.GetActor();
	if (Actor)
	{
		UActorComponent* Component = Actor->FindComponentByClass<UCommandComponent>();
		if (Component)
		{
			FString TagType = Component->ComponentTags[0].ToString(); //Always look for the first tag on an actor to determine the type of CommandPoint.
			if (TagType.Len() > 0)
			{
				CommandPoint.Type = FName(TagType);
				DrawDebugSphere(GetWorld(), HitResult.Location, 20, 8, FColor::Green, false, 2, 0, 1.f);

				if (CommandPoint.Type == FName("Investigate")) //Grab a static mesh called EndLocation on the actor. That will be the new location to move to.
				{
					UStaticMeshComponent* EndLocation = Cast<UStaticMeshComponent>(Actor->GetDefaultSubobjectByName(TEXT("EndLocation"))); 
					if (EndLocation)
					{
						FVector RightLocation = EndLocation->GetComponentLocation();
						CommandPoint.Location = RightLocation;
						return CommandPoint;
						
					}
				}
				if (CommandPoint.Type == FName("FirePoint"))
				{
					if(Actor->Implements<UAssignMemberInterface>())
					{	//We're checking to see if this actor has an assigned member. If not, assign one through SquadPlayerController::GetAvailableMember. If it already does, recall them.
						IAssignMemberInterface::Execute_CheckAssignedMember(Actor, CommandPoint); 
						CommandPoint.Location.X = 0.00f; //This is to restrict other AI other than the one specified in GetAvailableMember from moving to this location.
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
	CommandPoint.Type = FName("Return");
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

void ASquadPlayerController::DeployInvestigate(FCommandPointy CommandPoint)  
{ //I think this was a prototype for my Investigate CommandPoint type. Safe to delete?

	UE_LOG(LogTemp, Warning, TEXT("Test 0: DeployInvestigate"));
	if (DisposableList.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test 1: DisposableList is > 0"));
		for (int32 Index = DisposableList.Num() - 1; Index >= 0; --Index)
		{
			UE_LOG(LogTemp, Warning, TEXT("Test 2: Inside loop"));
			AActor* CurrentActor = DisposableList[Index];
			ASquadAIController* Commando = Cast<ASquadAIController>(CurrentActor);
			if (Commando)
			{
				UE_LOG(LogTemp, Warning, TEXT("Test 3: Cast successful"))
				Commando->MoveToCommand(CommandPoint);
				DisposableList.RemoveAt(Index);
				break;
			}
			
		}
		UE_LOG(LogTemp, Warning, TEXT("Test 4: Outside loop"));
		
		

		// Perform any required actions with CurrentActor

		// Remove the element at the end of the iteration
		
	}
}

void ASquadPlayerController::AssignPriorityCommand(FCommandPointy CommandPoint) // Could probably be part of refactoring GetAvailableMember.
{

}

ASquadAIController* ASquadPlayerController::GetAvailableMember(FCommandPointy CommandPoint) 
{  // Get the first AIController that doesn't have a priority command

	FVector CommandLocation = CommandPoint.Location;
	float BestDistance = 100000000; //Arbitrary number.
	ASquadAIController* ClosestMember = nullptr;
	for (AActor* Actor : SquadMembers)
	{
		ASquadAIController* SquadMember = Cast<ASquadAIController>(Actor);
		if (SquadMember)
		{
			if (SquadMember->PriorityCommand.Location.X == 0.00)
			{
				FVector MemberLocation = SquadMember->GetCharacter()->GetActorLocation();
				if (FVector::Distance(MemberLocation, CommandLocation) <= BestDistance)
				{
					BestDistance = FVector::Distance(MemberLocation, CommandLocation);
					ClosestMember = SquadMember;
				}	
			}
		}
	}
	if (ClosestMember) //AIController gets all the info he needs to move to the spot. Assignment of AssignedLocation pointer set in the CheckAssignedMember event.
	{
		ClosestMember->PriorityCommand.Location = CommandPoint.Location;
		ClosestMember->PriorityCommand.Type = CommandPoint.Type;
		ClosestMember->MoveToCommand(CommandPoint);
		return ClosestMember;
	}
	UE_LOG(LogTemp, Warning, TEXT("No member found! returning nullptr"));
	return nullptr;
}

void ASquadPlayerController::Tick(float DeltatTime)
{

}

void ASquadPlayerController::MoveUpCommand()
{
	//Use a line trace to find a location for AI to move to.

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
			//CreateCommandPointy checks to see if the hitresult actor has a Command Component and its type.
			FCommandPointy CommandPoint = CreateCommandPointy(HitResult);
			CommandList.Add(CommandPoint);
			
		}
	}
}

void ASquadPlayerController::FormUpCommand() //Generic recall function to return all AI regardless if they have assgned locations.
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
