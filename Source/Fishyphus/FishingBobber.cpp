// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingBobber.h"

// Sets default values
AFishingBobber::AFishingBobber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMeshAnchorPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Static Mesh Anchor Point"));
	staticMeshAnchorPoint->SetupAttachment(RootComponent);
	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	staticMeshComponent->SetupAttachment(staticMeshAnchorPoint);
	splashSound = CreateDefaultSubobject<USoundBase>(TEXT("Splash Sound"));
	//splashParticlesComponent = UGameplayStatics::SpawnEmitterAttached(splashParticles, staticMeshAnchorPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
}

// Called when the game starts or when spawned
void AFishingBobber::BeginPlay()
{
	Super::BeginPlay();
	// Remove this later
	beginFishing();
}

// Called every frame
void AFishingBobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (fishing) {
		if (timeTilCatch > -fishingCatchTimeLeniency) {
			timeTilCatch -= DeltaTime;
			if (timeTilCatch <= 0 && !canCatch) {
				canCatch = true;
				// Play splash noise
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), splashSound, GetActorLocation());
				// Add splash particles
				//splashParticlesComponent->Activate();
			}
		}
		else {
			beginFishing();
		}
	}
	handleBobbing(DeltaTime);
}

// Call this once the bobber hits the water, also called when a fishing attempt times out
void AFishingBobber::beginFishing() {
	generateRandomFishingTime();
	fishing = true;
	canCatch = false;
	//splashParticlesComponent->Deactivate();
}

void AFishingBobber::generateRandomFishingTime() {
	float increment = (maxFishingCatchTime - minFishingCatchTime) / 1000.f;
	int numberOfIncrements = rand() % 1000;
	timeTilCatch = minFishingCatchTime + numberOfIncrements * increment;
}

// Hook this up to an input action in player controller
void AFishingBobber::attemptCatch() {
	if (canCatch) {
		if (player) {
			player->OnFishCaught.Broadcast();
		}
		fishing = false;
		canCatch = false;
		player->spawnedBobber = nullptr;
		Destroy();
	}
}

void AFishingBobber::handleBobbing(float DeltaTime) {
	if (fishing) {
		bobbingTimer += DeltaTime * (canCatch ? canCatchBobMultiplier : 1);
		staticMeshComponent->SetRelativeLocation(FVector(0, 0, -bobAltitude + cosf(bobSpeed * bobbingTimer) * bobAltitude));
	}
	else {
		staticMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
	}
}