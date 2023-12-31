// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SquadPlayerController.h"
#include "TimerManager.h"
#include "SquadAIController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadAIController : public AAIController
{
	GENERATED_BODY()

public:
	FCommandPointy LastCommand;
	TArray<FCommandPointy> LocalCommandList; //In case I need to store multiple commands


	UPROPERTY(EditAnywhere)
	FCommandPointy PriorityCommand;
	UPROPERTY(EditAnywhere)
	bool bShouldFollow = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indoors")
	bool bIsIndoors = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasPriority = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* AssignedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indoors")
	AActor* Building;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indoors")
	AActor* Room;

	UFUNCTION()
	void ClearRoom();
	UFUNCTION()
	void MoveToCommand(FCommandPointy CommandPoint);

	UFUNCTION(BlueprintCallable)
	void ResetPriorityCommand();



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	
	UFUNCTION()
	void HandleCommand(FCommandPointy CommandPoint);
	UFUNCTION()
	void FollowPlayer();


private:
	UPROPERTY()
	ASquadPlayerController* PlayerController;
	UPROPERTY()
	FTimerHandle TimerHandle;
	

};


