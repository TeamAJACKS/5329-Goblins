// Copyright © 2018, AJACKS

#include "HealthPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"
#include "FirstPersonCharacter.h"


AHealthPickup::AHealthPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	// StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	FirstAidKit = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Environment/HealthPack.HealthPack'")));
	if (FirstAidKit != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(FirstAidKit);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
		StaticMeshComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	}

	// Sphere Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SphereComponent->SetupAttachment(StaticMeshComponent);
	SphereComponent->SetCollisionProfileName(FName("Trigger"));
	SphereComponent->SetSphereRadius(5.0f, true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::Pickup);
}

void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	// Attachments
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Float(DeltaTime);
	Rotate(DeltaTime);
}

void AHealthPickup::Pickup(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AFirstPersonCharacter::StaticClass()))
	{
		auto Player = Cast<AFirstPersonCharacter>(OtherActor);

		if (Player->CurrentHealth < 100)
		{
			const int AmountOfHealth = FMath::RandRange(MinAmount, MaxAmount);
			Player->AddHealth(AmountOfHealth);
			Player->HUD->PlayAnimation(Player->HUD->HealthPickupAnimation, 0);
			GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString("+ ") + FString::FromInt(AmountOfHealth) + FString(" Health!"));
			Destroy();
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString("Health Full!"));
	}
}

void AHealthPickup::Float(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	float DeltaHeight = (FMath::Sin(RunningTimeLoc + FloatingSpeed + DeltaTime) - FMath::Sin(RunningTimeLoc + FloatingSpeed));
	NewLocation.Z += DeltaHeight * FloatHeight;

	RunningTimeLoc += DeltaTime * FloatingSpeed;
	SetActorLocation(NewLocation);
}

void AHealthPickup::Rotate(float DeltaTime)
{
	// Rotate
	FRotator NewRotation = GetActorRotation();
	float DeltaYaw = DeltaTime * SpinSpeed;
	NewRotation.Yaw += DeltaYaw;

	SetActorRotation(NewRotation);
}
