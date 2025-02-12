// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "FishingBobber.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Create a CameraComponent	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.f, 0.f, 50.f)); // Position the camera
	camera->bUsePawnControlRotation = true;

	bobberSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("BobberSpawnPoint"));
	bobberSpawn->SetupAttachment(RootComponent);

}
// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Mapping context for movement
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMappingContext, 0);
		}
	}


}

//move around
void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D moveVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(),moveVector.Y);
	AddMovementInput(GetActorRightVector(), moveVector.X);
}

//look around
void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D lookVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(-lookVector.Y);
}

//jump? idk if we wanna jump but why not
void AMyPlayer::JumpFunc(const FInputActionValue& Value)
{
	const bool jumpPressed = Value.Get<bool>();

	ACharacter::Jump();
}

//fish function?
void AMyPlayer::Fish(const FInputActionValue& Value)
{
	//like idk someone can put something here fishing related
	if (spawnedBobber == nullptr) {
		//UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/MyFishingBobber.MyFishingBobber")));

		//UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);


		//if (!SpawnActor)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		//	return;
		//}

		//UClass* SpawnClass = SpawnActor->StaticClass();
		//if (SpawnClass == NULL)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		//	return;
	//	}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnedBobber = (AFishingBobber*)World->SpawnActor<AFishingBobber>(actorToSpawn, bobberSpawn->GetComponentLocation(), GetActorRotation(), SpawnParams);
		spawnedBobber->player = this;
		launchBobber();
	}
	else {
		spawnedBobber->attemptCatch();
	}
}


// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponenet->BindAction(moveIA, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponenet->BindAction(lookIA, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		EnhancedInputComponenet->BindAction(jumpIA, ETriggerEvent::Started, this, &AMyPlayer::JumpFunc);
		EnhancedInputComponenet->BindAction(fishIA, ETriggerEvent::Started, this, &AMyPlayer::Fish);
	}


}

void AMyPlayer::launchBobber() {
	if (spawnedBobber) {
		if (camera) {
			spawnedBobber->sphereCollider->AddImpulse(camera->GetForwardVector() * 1000, NAME_None, true);
		}
	}
}
