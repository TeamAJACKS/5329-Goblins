// Copyright � 2018, AJACKS

#include "SlidingDoor.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_4.18/Engine/Plugins/MovieScene/ActorSequence/Source/ActorSequence/Private/ActorSequencePrivatePCH.h"

ASlidingDoor::ASlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = Root;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Door = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));
	if (Door != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Door);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(0.5, 4.25, 3.5));
	}

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Component"));
	TriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	TriggerBoxComponent->SetWorldLocation(FVector(-150, 0, 0));
	TriggerBoxComponent->SetBoxExtent(FVector(50, 40, 50));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoor::Slide);

	// Timeline Component
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	// Curve Float default
	CurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/DoorSlideTime.DoorSlideTime'")));

	// Other settings
	this->bCanBeDamaged = false;
}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	// Timeline Setup
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "Slide");

	if (CurveFloat != nullptr)
	{
		Timeline = NewObject<UTimelineComponent>(this, FName("DoorAnimation"));
		Timeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		Timeline->SetPropertySetObject(this);
		Timeline->SetLooping(false);
		Timeline->SetPlaybackPosition(0.0f, false);
		Timeline->SetPlayRate(Speed);
		Timeline->AddInterpFloat(CurveFloat, OnTimelineCallback);
		Timeline->SetTimelineLength(Speed);
		Timeline->RegisterComponent();
	}

	// Door Setup
	CurrentLocation = StaticMeshComponent->GetComponentLocation();

	// Player reference
	PlayerCharacter = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Calculate the amount of goblins to kill from the GoblinSpawners array
	if (!GoblinSpawners.Num() == 0)
	{
		for (int32 i = 0; i < GoblinSpawners.Num(); i++)
		{
			GoblinsToKill += GoblinSpawners[i]->Goblins;
		}
	}
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Timeline != nullptr)
		Timeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
}

void ASlidingDoor::Slide(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0) && GoblinsToKill == PlayerCharacter->GoblinsKilledForDoorUnlock)
	{
		Timeline->Play();

		PlayerCharacter->GoblinsKilledForDoorUnlock = 0;

		if (DoorSlidingSound)
			UGameplayStatics::PlaySoundAtLocation(this, DoorSlidingSound, GetActorLocation());

		if (IsValid(TriggerBoxComponent) && CurveFloat != nullptr)
			TriggerBoxComponent->DestroyComponent(); // Destroy the trigger box once the door has opened.
	}

	const float TimelineValue = Timeline->GetPlaybackPosition();
	const float CurveFloatValue = CurveFloat->GetFloatValue(TimelineValue); // Get the value of the timeline and feed into the curve float for lerping between start and end location

	if (Sideways)
		EndLocation = FVector(CurrentLocation.X - XSlide, CurrentLocation.Y - YSlide, CurrentLocation.Z); // Minus the X or Y axis to set the end location
	else
		EndLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - ZSlide); // Minus the Z axis to set the end location

	StaticMeshComponent->SetWorldLocation(FMath::Lerp(CurrentLocation, EndLocation, CurveFloatValue)); // Move the door smoothly from current location to the end location
}
