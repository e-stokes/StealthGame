// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"

#include <utility>



#include "Axis.h"
#include "NavigationSystemTypes.h"
#include "PropertyEditorModule.h"
#include "TextFilterExpressionEvaluator.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"


// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Main Mesh -> Root Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	RootComponent = MeshComponent;

	// Force Field parameters
	Force = CreateDefaultSubobject<URadialForceComponent>(TEXT("Force"));
	Force->Radius = 2000.f; // Radius that is affected by the Black Hole
	Force->ForceStrength = -1500000.f; // Must be a negative number or it will rebel objects instead
	Force->SetAutoActivate(false);

	//Objects that will be destroyed by Black Hole
	Force->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	Force->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	Force->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Destructible));
	Force->SetupAttachment(RootComponent);

	EffectTimer = 20.f;

	ExplosionRadius = 1.f;
	ExplosionStrength = 100.f;

}



// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	// Starts the Black Hole
	Force->Activate();

	// Array and parameters for OverlapMultiByObjectType function
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Force->Radius);

	GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(), FQuat::Identity,
		QueryParams, CollisionShape);

	// Iterates over the objects that overlap with the Black Hole Radius
	for (FOverlapResult Result: OverlapResults)
	{
		AActor* Overlap = Result.GetActor();

		if (Overlap)
		{
			TTuple<AActor*, FVector>MapTuple = TTuple<AActor*, FVector>(Overlap, Overlap->GetActorScale3D());
			AffectedActorMap.Add(MapTuple);
		}
	}

	// Timer to end Black Hole effect
	FTimerHandle EffectTimer_TimerHandle;
	GetWorldTimerManager().SetTimer(EffectTimer_TimerHandle, this, &ABlackHole::EndBlackHole, EffectTimer, false);


	
	
}

void ABlackHole::EndBlackHole()
{
	Force->Deactivate();

	if (AffectedActorMap.Num() > 0)
	{
		for (const TPair<AActor*, FVector>&Pair:AffectedActorMap)
		{
			Pair.Key->SetActorScale3D(Pair.Value);
		}
	}
	AffectedActorMap.Empty();

	//Explode Items
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(ExplosionRadius);

	GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(), FQuat::Identity,
		QueryParams, CollisionShape);

	for (FOverlapResult Result : OverlapResults)
	{
		UPrimitiveComponent* Overlap = Result.GetComponent();

		if (Overlap && Overlap->IsSimulatingPhysics())
		{
			Overlap->AddRadialImpulse(GetActorLocation(), ExplosionRadius, ExplosionStrength, RIF_Constant);
		}
	}

	Destroy();
	
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AffectedActorMap.Num() > 0)
	{
		for (const TPair<AActor*, FVector>&Pair : AffectedActorMap)
		{
			float DistanceFromBlackHole = FMath::Clamp(((Pair.Key->GetActorLocation() - this->GetActorLocation()).Size())
				/1000.f, 0.1f,1.f);

			FVector Scale = FMath::VInterpTo(Pair.Key->GetActorScale3D(),
				FVector(DistanceFromBlackHole), DeltaTime, 20.f);

			Pair.Key->SetActorScale3D(Scale);
		}
	}

}


void ABlackHole::BHEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DestroyEffect, GetActorLocation());
}





