// Fill out your copyright notice in the Description page of Project Settings.


#include "Cauldron.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ACauldron::ACauldron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn"));
	SpawnPoint->SetupAttachment(Mesh);

	TopCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopCollider->SetupAttachment(Mesh);

	FScriptDelegate overlapFunc;
	overlapFunc.BindUFunction(this, "OnOverlap");
	TopCollider->OnComponentBeginOverlap.Add(overlapFunc);

	FScriptDelegate overlapEndFunc;
	overlapEndFunc.BindUFunction(this, "OnOverlapExit");
	TopCollider->OnComponentEndOverlap.Add(overlapEndFunc);

}

// Called when the game starts or when spawned
void ACauldron::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACauldron::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!tipping) {
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		topOverlappedActors.push_back(OtherActor);
		containedActors.push_back(OtherActor);
		timers.push_back(FTimerHandle());
		GetWorldTimerManager().SetTimer(timers.back(), [&, OtherActor]() {CheckCollision(OtherActor); }, 1, false);
	}
	
}

void ACauldron::OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!tipping) {
		UE_LOG(LogTemp, Warning, TEXT("Exit"));
		topOverlappedActors.remove(OtherActor);
	}
}

void ACauldron::CheckCollision(AActor* targetActor)
{
	
	for (AActor* actor : topOverlappedActors) {
		UE_LOG(LogTemp, Warning, TEXT("Checking: %s, %s"), *targetActor->GetName(), *actor->GetName());
		if (actor == targetActor) {
			UE_LOG(LogTemp, Warning, TEXT("Tipping"));
			TipCauldron();
			return;
		}
	}
}

void ACauldron::TipCauldron()
{
	topOverlappedActors.clear();
	tipping = true;
}

// Called every frame
void ACauldron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tipping) {
		lerpTimer += DeltaTime;
		if (lerpTimer > 1) {
			lerpTimer = 0;
			tipping = false;
			SetActorRotation(FRotator(0, 0, TargetAngle));

			for (AActor* actor : containedActors) {
				AStaticMeshActor* staticMesh = Cast<AStaticMeshActor>(actor);
				if (IsValid(staticMesh)) {
					staticMesh->GetStaticMeshComponent()->AddImpulse((staticMesh->GetActorLocation() - SpawnPoint->GetComponentLocation()) * 50.f);
				}
			}
		}
		else {
			SetActorRotation(FRotator(0, 0, FMath::Lerp(0, TargetAngle, lerpTimer)));
		}
	}
}

