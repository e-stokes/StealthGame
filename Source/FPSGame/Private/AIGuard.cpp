// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"

#include "AIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) return;

	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TargetKey, NoiseInstigator);

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
	}

	/*
	AAIPatrolController* Con = Cast<AAIPatrolController>(GetController());
	if (Con && NoiseInstigator != this)
	{
		Con->SetHeardTarget(NoiseInstigator);
	}
	
	
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
	*/
}

void AAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) return;
	
	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);
}

void AAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) return;

	GuardState = NewState;

	OnStateChanged(GuardState);
}


