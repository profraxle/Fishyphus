// Fill out your copyright notice in the Description page of Project Settings.


#include "Cauldron.h"
#include "Engine/StaticMeshActor.h"
#include "Item.h"
#include "MyPlayer.h"

// Sets default values
ACauldron::ACauldron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(GetRootComponent());

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn"));
	SpawnPoint->SetupAttachment(Mesh);

	TippingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Tipping"));
	TippingSound->SetupAttachment(Mesh);

	MagicSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Magic"));
	MagicSound->SetupAttachment(Mesh);

	TopColl = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopColl"));
	TopColl->SetupAttachment(Mesh);

	TopCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopCollider->SetupAttachment(Mesh);

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particles->SetupAttachment(Mesh);

	FScriptDelegate overlapFunc;
	overlapFunc.BindUFunction(this, "OnOverlap");
	//TopCollider->OnComponentBeginOverlap.Add(overlapFunc);
	TopColl->OnComponentBeginOverlap.Add(overlapFunc);

	FScriptDelegate overlapEndFunc;
	overlapEndFunc.BindUFunction(this, "OnOverlapExit");
	//TopCollider->OnComponentEndOverlap.Add(overlapEndFunc);
	TopColl->OnComponentEndOverlap.Add(overlapEndFunc);

}

// ---- Call this to get location to move items to ----
FVector ACauldron::GetSpawnLocation()
{
	return SpawnPoint->GetComponentLocation();
}

// Called when the game starts or when spawned
void ACauldron::BeginPlay()
{
	Super::BeginPlay();
	Particles->SetActive(false);
	TippingSound->Stop();
	MagicSound->Stop();
	startRotator = GetActorRotation();
}

void ACauldron::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(Cast<AMyPlayer>(OtherActor))) return;
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
	if (IsValid(Cast<AMyPlayer>(OtherActor))) return;
	if (!tipping && !tipped) {
		UE_LOG(LogTemp, Warning, TEXT("Exit"));

		topOverlappedActors.remove(OtherActor);
	}
	else {
		auto it = std::find(containedActors.begin(), containedActors.end(), OtherActor);
		if(it != containedActors.end()) containedActors.erase(it);

		timers.push_back(FTimerHandle());
		GetWorldTimerManager().SetTimer(timers.back(), [&, OtherActor]() {UE_LOG(LogTemp, Warning, TEXT("Destroying")); OtherActor->Destroy(); }, 1, false);
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
	TippingSound->Play();
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
			SetActorRotation(startRotator + FRotator(0, 0, TargetAngle));
		}
		else {
			Mesh->SetRelativeRotation(startRotator + FRotator(0, 0, FMath::Lerp(0, TargetAngle, lerpTimer)));
		}
	}
	else if (tipped && !containedActors.empty()) {
		timeElapsed += DeltaTime;
		if (timeElapsed > 0.2) {
			timeElapsed = 0.f;
			AStaticMeshActor* staticMesh = Cast<AStaticMeshActor>(containedActors.back());
			//containedActors.pop_back();
			if (IsValid(staticMesh)) {
				staticMesh->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
				staticMesh->GetStaticMeshComponent()->AddImpulse((SpawnPoint->GetComponentLocation() - staticMesh->GetActorLocation()) * Impulse);

				if (FVector(staticMesh->GetActorLocation() - GetActorLocation()).Length() >= 1000) {
					staticMesh->Destroy();
					containedActors.pop_back();
				}
				
			}
			else {
				containedActors.pop_back();
			}
		}
	}
	else if (tipped && containedActors.empty()) {
		timeElapsed += DeltaTime;
		if (timeElapsed > 1.5f) {
			if (!Particles->IsActive()) {
				Particles->SetActive(true);
				Particles->Activate(true);
				MagicSound->Play();
			}
			lerpTimer += DeltaTime;
			if (lerpTimer > 2) {
				timeElapsed = 0.f;
				lerpTimer = 0;
				tipped = false;
				timers.clear();
				Mesh->SetRelativeRotation(startRotator);
				Particles->SetActive(false);
			}
			else {
				Mesh->SetRelativeRotation(startRotator + FRotator(0, 0, FMath::Lerp(TargetAngle, 0, lerpTimer/2.f)));
			}
		}
	}
}

