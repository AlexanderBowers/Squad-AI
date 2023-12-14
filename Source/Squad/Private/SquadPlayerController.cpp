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


FCommandPointy ASquadPlayerController::CreateCommandPointy(FHitResult HitResult)
{
	//if we get a collision, create a FCommandPointy. 
	//If the collided actor has a Command Component, get its type and add to the CommandList.

	FCommandPointy CommandPoint;
	CommandPoint.Location = HitResult.Location;

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
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Component but not tag!"))
				CommandPoint.Type = TEXT("Move");
			}
		}
		else //If there is no component, default the type to move.
		{
			CommandPoint.Type = TEXT("Move");
		}
		FString CommandPointString = CommandPoint.Location.ToString();
		FString TypeString = CommandPoint.Type.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Location: %s ; Type: %s "), *CommandPointString, *TypeString);
		return FCommandPointy();
	}
	//If there is no actor hit, return to the player
	CommandPoint.Location = this->GetPawn()->GetActorLocation();
	CommandPoint.Type = TEXT("Move");
	return FCommandPointy();
}

void ASquadPlayerController::Tick(float DeltatTime)
{

}

void ASquadPlayerController::MoveUpCommand()
{
	//Line trace to a location. On collision, create a FCommandPointy then add it to the list.

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

			FCommandPointy CommandPoint = CreateCommandPointy(HitResult);
			//UE_LOG(LogTemp, Warning, TEXT("Command Point: %s"), CommandPoint.Location.ToString());
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