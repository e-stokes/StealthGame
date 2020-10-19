// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"

#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// main mesh for launch pad
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	// collision mesh for launch pad - this will launch the player into the air when player overlaps
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	OverlapComp->SetupAttachment(RootComponent);

	// calls function on overlap
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::LaunchOnOverlap);

	
	LaunchStrength = 1500.f;
	LaunchAngle = 45.f;
	
	

	
	

}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPad::LaunchOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Rotation with current pitch and convert to a direction vector * intensity
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	
	ACharacter* MyPawn = Cast<ACharacter>(OtherActor);
	
	if (MyPawn)
	{
		// Launch Character
		MyPawn->LaunchCharacter(LaunchVelocity, true, true);

		// Spawns effect at launch pad location
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Launch component simulating physics
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		// Spawns effect at launch pad location
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, GetActorLocation());
	}
}


