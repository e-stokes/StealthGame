// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolController.h"

#include "AIGuard.h"
#include "AIPatrolController.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"


AAIPatrolController::AAIPatrolController()
{
    // Initialize blackboard and behavior tree
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // Initialize blackboard keys
    LocationToGo = "LocationToGo";
    PlayerKey = "Target";

    CurrentPatrolPoint = 0;
}

void AAIPatrolController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Get reference to character
    AAIGuard* AICharacter = Cast<AAIGuard>(InPawn);

    if(AICharacter)
    {
        if (AICharacter->BehaviorTree->BlackboardAsset)
        {
            BlackboardComponent->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
        }

        // Populate patrol point
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);

        BehaviorTreeComponent->StartTree(*AICharacter->BehaviorTree);
    }
}

void AAIPatrolController::SetHeardTarget(APawn* HeardTarget)
{
    // Set a new target to follow
   // if (BlackboardComponent)
    {
        //BlackboardComponent->SetValueAsObject(TargetKey, HeardTarget);
    }
}




