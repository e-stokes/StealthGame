// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"

#include "AIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize blackboard component
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AAIGuard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Patrol check
	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		// Check if we are within 50 units of point
		if (DistanceToGoal <= 50)
		{
			MoveToNextPatrolPoint();
		}
	}
}

void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
	
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Yellow, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false); 
	}

	SetGuardState(EAIState::Alerted);

	// Stop patrol movement
	AController* Control = GetController();
	if (Control)
	{
		Control->StopMovement();
	}
}

void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) return;

	/*
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TargetKey, NoiseInstigator);
	}
	*/

	/*
	AAIPatrolController* Con = Cast<AAIPatrolController>(GetController());
	if (Con && NoiseInstigator != this)
	{
		Con->SetHeardTarget(NoiseInstigator);
	}
	*/
	
	DrawDebugSphere(GetWorld(),Location, 32.f, 12, FColor::Red, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAtRotation =  FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAtRotation.Pitch = 0.f;
	NewLookAtRotation.Roll = 0.f;

	SetActorRotation(NewLookAtRotation);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIGuard::ResetOrientation, 3.f);

	SetGuardState(EAIState::Suspicious);

	// Stop Patrol movement
	AController* Control = GetController();
	if (Control)
	{
		Control->StopMovement();
	}
	
}

void AAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) return;
	
	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	// Resume patrol
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) return;

	GuardState = NewState;
	OnRep_GuardState();
}

void AAIGuard::MoveToNextPatrolPoint()
{
	// Assign next patrol point
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void AAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIGuard, GuardState);
}


