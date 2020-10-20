// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "AIGuard.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class UBlackboardComponent;
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};





UCLASS()
class FPSGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();


	// Blackboard component
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);


	FRotator OriginalRotation;

	FTimerHandle TimerHandle_ResetOrientation;

	UFUNCTION()
	void ResetOrientation();


	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetKey = "SensedPawn";


	


};
