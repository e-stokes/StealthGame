// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_SelectPatrolPoint.h"

#include "AIPatrolController.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BT_SelectPatrolPoint.h"
/*
EBTNodeResult::Type UBT_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIPatrolController* AICon = Cast<AAIPatrolController>(OwnerComp.GetAIOwner());;

    if (AICon)
    {
        // Get Blackboard Component
        UBlackboardComponent* BlackboardComponent = AICon->GetBlackboardComponent();

        AAIPatrolPoint* CurrentPoint = Cast<AAIPatrolPoint>(BlackboardComponent->GetValueAsObject("LocationToGo"));

        TArray<AActor*> AvailablePatrolPoints = AICon->GetPatrolPoints();

        AAIPatrolPoint* NextPatrolPoint = nullptr;

        if (AICon->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
        {
            NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[++AICon->CurrentPatrolPoint]);
        }
        else // if no other points
        {
            NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[0]);
            AICon->CurrentPatrolPoint = 0;
        }

        BlackboardComponent->SetValueAsObject("LocationToGo", NextPatrolPoint);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
*/
