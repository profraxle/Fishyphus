// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include <vector>
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

	void CheckCollision(AActor* actor);

	void TipCauldron();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cauldron,
		meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cauldron, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TopCollider = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cauldron, meta = (AllowPrivateAccess = "true"))
	float TargetAngle = 60.f;

	bool tipping = false;

	std::vector<FTimerHandle> timers;

	std::list<AActor*> topOverlappedActors;
	std::vector<AActor*> containedActors;

	float lerpTimer = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
