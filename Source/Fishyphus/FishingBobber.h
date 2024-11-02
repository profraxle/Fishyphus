// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyPlayer.h"
#include "FishingBobber.generated.h"



UCLASS()
class FISHYPHUS_API AFishingBobber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingBobber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void generateRandomFishingTime();
	void handleBobbing(float DeltaTime);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void beginFishing();
	UFUNCTION(BlueprintCallable)
	void attemptCatch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* staticMeshAnchorPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* staticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* splashSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* splashParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* splashParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minFishingCatchTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxFishingCatchTime = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fishingCatchTimeLeniency = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bobAltitude = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float canCatchBobMultiplier = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bobSpeed = 4.f;

	bool fishing = false;
	bool canCatch = false;
	float timeTilCatch = 0.0f;
	float bobbingTimer = 0.0f;
	AMyPlayer* player = nullptr;
};
