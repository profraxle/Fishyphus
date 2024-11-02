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

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particles->SetupAttachment(Mesh);

	FScriptDelegate overlapFunc;
	overlapFunc.BindUFunction(this, "OnOverlap");
	TopCollider->OnComponentBeginOverlap.Add(overlapFunc);

	FScriptDelegate overlapEndFunc;
	overlapEndFunc.BindUFunction(this, "OnOverlapExit");
	TopCollider->OnComponentEndOverlap.Add(overlapEndFunc);

}

FVector ACauldron::GetSpawnLocation()
{
	return SpawnPoint->GetComponentLocation();
}

// Called when the game starts or when spawned
void ACauldron::BeginPlay()
{
	Super::BeginPlay();
	Particles->SetActive(false);
}

void ACauldron::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!tipping && !tipped) {
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		topOverlappedActors.push_back(OtherActor);
		containedActors.push_back(OtherActor);
		timers.push_back(FTimerHandle());
		GetWorldTimerManager().SetTimer(timers.back(), [&, OtherActor]() {CheckCollision(OtherActor); }, 1, false);
	}
	
}

void ACauldron::OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!tipping && !tipped) {
		UE_LOG(LogTemp, Warning, TEXT("Exit"));
		topOverlappedActors.remove(OtherActor);
	}
	else {
		auto it = std::find(containedActors.begin(), containedActors.end(), OtherActor);
		if(it != containedActors.end()) containedActors.erase(it);
	}
}

void ACauldron::CheckCollision(AActor* targetActor)
{
	
	for (AActor* actor : topOverlappedActors) {
		//UE_LOG(LogTemp, Warning, TEXT("Checking: %s, %s"), *targetActor->GetName(), *actor->GetName());
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
			tipped = true;
			timers.clear();
			SetActorRotation(FRotator(0, 0, TargetAngle));
		}
		else {
			SetActorRotation(FRotator(0, 0, FMath::Lerp(0, TargetAngle, lerpTimer)));
		}
	}
	else if (tipped && !containedActors.empty()) {
		timeElapsed += DeltaTime;
		if (timeElapsed > 0.2) {
			timeElapsed = 0.f;
			AStaticMeshActor* staticMesh = Cast<AStaticMeshActor>(containedActors.back());
			containedActors.pop_back();
			if (IsValid(staticMesh)) {
				staticMesh->GetStaticMeshComponent()->AddImpulse((SpawnPoint->GetComponentLocation() - staticMesh->GetActorLocation()) * Impulse);
				timers.push_back(FTimerHandle());
				GetWorldTimerManager().SetTimer(timers.back(), [&, staticMesh]() {UE_LOG(LogTemp, Warning, TEXT("Destroying")); staticMesh->Destroy(); }, 1, false);
			}
		}
	}
	else if (tipped && containedActors.empty()) {
		timeElapsed += DeltaTime;
		if (timeElapsed > 1.5f) {
			if (!Particles->IsActive()) {
				Particles->SetActive(true);
				Particles->Activate(true);
			}
			lerpTimer += DeltaTime;
			if (lerpTimer > 2) {
				timeElapsed = 0.f;
				lerpTimer = 0;
				tipped = false;
				timers.clear();
				SetActorRotation(FRotator(0, 0, 0));
				Particles->SetActive(false);
			}
			else {
				SetActorRotation(FRotator(0, 0, FMath::Lerp(TargetAngle, 0, lerpTimer/2.f)));
			}
		}
	}
}

