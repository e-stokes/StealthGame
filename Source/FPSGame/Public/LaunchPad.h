// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Creating Mesh component for launch pad
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// Collision mesh for launch pad
	UPROPERTY(VisibleAnywhere, Category = "CollisionComponent")
	UBoxComponent* OverlapComp;

	// Effect for launch pad when player is launched
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* LaunchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float LaunchVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float LaunchHeight;

	UFUNCTION()
	void LaunchOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

