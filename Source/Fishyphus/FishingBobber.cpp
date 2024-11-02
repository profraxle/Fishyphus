// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingBobber.h"

// Sets default values
AFishingBobber::AFishingBobber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphereCollider->SetupAttachment(RootComponent);
	staticMeshAnchorPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Static Mesh Anchor Point"));
	staticMeshAnchorPoint->SetupAttachment(sphereCollider);
	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	staticMeshComponent->SetupAttachment(staticMeshAnchorPoint);
	splashSound = CreateDefaultSubobject<USoundBase>(TEXT("Splash Sound"));
	splashParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	splashParticlesComponent->SetupAttachment(sphereCollider);
}

// Called when the game starts or when spawned
void AFishingBobber::BeginPlay()
{
	Super::BeginPlay();
	// Remove this later
	//beginFishing();
	splashParticlesComponent->SetActive(false);
}

// Called every frame
void AFishingBobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(FQuat());
	if (fishing) {
		if (timeTilCatch > -fishingCatchTimeLeniency) {
			timeTilCatch -= DeltaTime;
			if (timeTilCatch <= 0 && !canCatch) {
				canCatch = true;
				// Play splash noise
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), splashSound, GetActorLocation());
				// Add splash particles
				UE_LOG(LogTemp, Warning, TEXT("particles on"));
				splashParticlesComponent->SetActive(true);
				splashParticlesComponent->Activate(true);
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
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), splashSound, GetActorLocation());
	generateRandomFishingTime();
	fishing = true;
	canCatch = false;
	UE_LOG(LogTemp, Warning, TEXT("particles off"));
	splashParticlesComponent->SetActive(false);
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
	}
	player->spawnedBobber = nullptr;
	Destroy();
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