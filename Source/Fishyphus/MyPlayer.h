// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "MyPlayer.generated.h"

class AFishingBobber;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFishCaught);

UCLASS()
class FISHYPHUS_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

	//--- Movement Functions ---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpFunc(const FInputActionValue& Value);
	void Fish(const FInputActionValue& Value);

	// Launch Fishing Bobber
	void launchBobber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* moveIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* lookIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* fishIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* jumpIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFishingBobber* spawnedBobber = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnFishCaught OnFishCaught;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* bobberSpawn;

};
