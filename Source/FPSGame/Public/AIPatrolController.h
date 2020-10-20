// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "AIPatrolController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AAIPatrolController : public AAIController
{
	GENERATED_BODY()

	// Behavior tree component
	UBehaviorTreeComponent* BehaviorTreeComponent;

	// Blackboard component
	UBlackboardComponent* BlackboardComponent;

	// Blackboard keys
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGo;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerKey;

	/*
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetKey = "SensedPawn";
	*/
	

	TArray<AActor*> PatrolPoints;

	virtual void OnPossess(APawn* InPawn) override;

public:

	AAIPatrolController();
	
	void SetHeardTarget(APawn* HeardTarget);

	int32 CurrentPatrolPoint;

	// inline getter functions
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	
};
