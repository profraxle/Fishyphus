// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include <vector>
#include <stack>
#include <list>
#include "Engine/TimerHandle.h"
#include "TimerManager.h"
#include "Cauldron.generated.h"

UCLASS()
class FISHYPHUS_API ACauldron : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACauldron();

	// ---- Call this to get location to move items to ----
	UFUNCTION(BlueprintCallable)
	FVector GetSpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* Particles;

	void CheckCollision(AActor* actor);

	void TipCauldron();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TopColl;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* TippingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* MagicSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cauldron, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TopCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Cauldron, meta = (AllowPrivateAccess = "true"))
	float TargetAngle = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Cauldron, meta = (AllowPrivateAccess = "true"))
	float Impulse = 30.f;

	FRotator startRotator;

	bool tipping = false;
	bool tipped = false;

	std::vector<FTimerHandle> timers;

	std::list<AActor*> topOverlappedActors;
	std::vector<AActor*> containedActors;

	float lerpTimer = 0.f;
	float timeElapsed = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
