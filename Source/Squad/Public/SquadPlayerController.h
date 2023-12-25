// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "CommandPoint.h"
#include "SquadPlayerController.generated.h"

/**
 * 
 */
//CHANGE ALL CommandPoint REFERENCES TO FCommandPointy UNTIL RENAME IS SUCCESSFUL

USTRUCT(BlueprintType)
struct FCommandPointy
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type;
};

UCLASS()
class SQUAD_API ASquadPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	//APlayerController();
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	FCommandPointy CreateCommandPointy(FHitResult HitResult);
	UFUNCTION()
	FCommandPointy AssignLocation(FCommandPointy CommandPoint, FHitResult HitResult);
	UFUNCTION()
	FCommandPointy AssignType(FCommandPointy CommandPoint, FHitResult HitResult);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetRooms(AActor* Building);
	UFUNCTION()
	void CheckRoomValues(UClass* ActorClass, AActor* Room);

	
	virtual void Tick(float DeltatTime) override;

	UFUNCTION()
	void MoveUpCommand();
	UFUNCTION()
	void FormUpCommand();


	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere)
	TArray<FCommandPointy> CommandList;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SquadMembers;
	TArray<AActor*> RoomsInBuilding;
	

private:

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;
	UPROPERTY(EditAnywhere)

	FVector CameraLocation;
	FRotator CameraRotation;
	APawn* ControlledPawn;
	
};



