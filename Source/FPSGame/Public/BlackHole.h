// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;
class URadialForceComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// Effect of Black Hole destroying an object
	void BHEffect();

	// Particle effect for the black hole
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestroyEffect;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TMap<AActor*, FVector> AffectedActorMap;

	UFUNCTION()
	void EndBlackHole();

	UPROPERTY(EditAnywhere, Category = "Effects")
	float EffectTimer;

	UPROPERTY(EditAnywhere, Category = "Effects")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = "Effects")
	float ExplosionStrength;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Force field for black hole
	UPROPERTY(EditDefaultsOnly, Category = "Force")
	URadialForceComponent* Force;

};

