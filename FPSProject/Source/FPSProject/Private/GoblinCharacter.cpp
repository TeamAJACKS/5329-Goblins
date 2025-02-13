// Copyright © 2018, AJACKS

#include "GoblinCharacter.h"
#include "GoblinAIController.h"
#include "Classes/Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"
#include "GoblinAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonCharacter.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"


AGoblinCharacter::AGoblinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object loading
	static ConstructorHelpers::FObjectFinder<UClass> GoblinAnimBP(TEXT("AnimBlueprint'/Game/5329_Goblins/Blueprints/Animation/GoblinAnimBlueprint.GoblinAnimBlueprint_C'"));
	GoblinBodyMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Goblin/Goblin_Body_Mat.Goblin_Body_Mat'")));
	GoblinEyeMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Goblin/Goblin_Eye_Mat.Goblin_Eye_Mat'")));
	GoblinTeethMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Goblin/Goblin_Teeth_Mat.Goblin_Teeth_Mat'")));

	// Goblin Character Defaults
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 95.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Skeletal Mesh
	SK_Goblin = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/5329_Goblins/Assets/Goblin/SKGoblin.SKGoblin'")));
	GetMesh()->SetSkeletalMesh(SK_Goblin);
	GetMesh()->SetWorldLocation(FVector(0, 0, -90));
	GetMesh()->SetWorldScale3D(FVector(1.25, 1.25, 1.25));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel2); // Goblin
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	if (GoblinAnimBP.Object != nullptr)
		GetMesh()->SetAnimInstanceClass(GoblinAnimBP.Object);

	// Sphere Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere Component"));
	SphereComponent->SetupAttachment(GetMesh());
	SphereComponent->SetRelativeLocation(FVector(50.0f, 0, 120.0f));
	SphereComponent->SetSphereRadius(40.0f);

	// Dissolve Timeline
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Dissolve Timeline"));
	DissolveCurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/DissolveLerpTime.DissolveLerpTime'")));

	// Behavior Tree
	GoblinBehavior = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/5329_Goblins/AI/GoblinBT.GoblinBT'")));

	// AI Controller class setup
	AIControllerClass = AGoblinAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AGoblinCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Goblin Setup
	CurrentHealth = MaxHealth;

	// Attachments
	SphereComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	GoblinAnimInstance = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
	GoblinAIController = Cast<AGoblinAIController>(GetController());

	// Dissolve Timeline Setup
	FOnTimelineFloat OnDissolveTimelineCallback;
	OnDissolveTimelineCallback.BindUFunction(this, "OnDissolve");

	if (DissolveCurveFloat != nullptr)
	{
		DissolveTimeline = NewObject<UTimelineComponent>(this, FName("HealthLerpSpeed"));
		DissolveTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		DissolveTimeline->SetPropertySetObject(this);
		DissolveTimeline->SetLooping(false);
		DissolveTimeline->SetPlaybackPosition(0.0f, false);
		DissolveTimeline->SetPlayRate(0.8f);
		DissolveTimeline->AddInterpFloat(DissolveCurveFloat, OnDissolveTimelineCallback);
		DissolveTimeline->SetTimelineLength(1.5f);
		DissolveTimeline->SetTimelineLengthMode(TL_TimelineLength);
		DissolveTimeline->RegisterComponent();

		DissolveCurveFloat->FloatCurve.Reset();
		DissolveCurveFloat->FloatCurve.UpdateOrAddKey(0.0, 0.0f); // Time: 0  Value: 0
		DissolveCurveFloat->FloatCurve.UpdateOrAddKey(1.0f, 1.0f); // Time: LerpSpeed  Value: 1
	}

	// Material Instance Setup
	GoblinBodyMaterialInstance = UMaterialInstanceDynamic::Create(GoblinBodyMaterial, this);
	GoblinEyeMaterialInstance = UMaterialInstanceDynamic::Create(GoblinEyeMaterial, this);
	GoblinTeethMaterialInstance = UMaterialInstanceDynamic::Create(GoblinTeethMaterial, this);

	GetMesh()->SetMaterial(3, GoblinBodyMaterialInstance);
	GetMesh()->SetMaterial(0, GoblinEyeMaterialInstance);
	GetMesh()->SetMaterial(4, GoblinTeethMaterialInstance);
}

void AGoblinCharacter::Dissolve()
{
	DissolveTimeline->PlayFromStart();
}

void AGoblinCharacter::OnDissolve()
{
	// Get the value of the timeline and feed into the curve float for lerping between health values
	const float TimelineValue = DissolveTimeline->GetPlaybackPosition();
	const float CurveFloatValue = DissolveCurveFloat->GetFloatValue(TimelineValue);

	GoblinBodyMaterialInstance->SetScalarParameterValue(FName("DissolveAmount"), FMath::Lerp(1.0f, -0.25f, CurveFloatValue));
	GoblinEyeMaterialInstance->SetScalarParameterValue(FName("DissolveAmount"), FMath::Lerp(1.0f, -0.25f, CurveFloatValue));
	GoblinTeethMaterialInstance->SetScalarParameterValue(FName("DissolveAmount"), FMath::Lerp(1.0f, -0.25f, CurveFloatValue));

}

void AGoblinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If goblin is moving AND has seen the player
	if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		// Walk but don't attack
		GoblinAnimInstance->bCanWalk = true;
		GoblinAnimInstance->bCanAttack = false;
	}
	// If goblin is not moving AND has seen the player
	else if (GetCharacterMovement()->Velocity == FVector::ZeroVector && GoblinAIController->AIStimulus.WasSuccessfullySensed())
	{
		GoblinAnimInstance->bCanWalk = false;
	}
	// If goblin is not moving
	else if (GetCharacterMovement()->Velocity == FVector::ZeroVector)
		GoblinAnimInstance->bCanWalk = false;
	else
		GoblinAnimInstance->bCanAttack = false;

	if (CurrentHealth <= 0)
	{
		GoblinAnimInstance->bHealthBelowZero = true;
		GoblinAnimInstance->bCanAttack = false;
		GoblinAnimInstance->bCanWalk = false;
	}

	if (bIsGoblinHit)
	{
		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			GoblinAnimInstance->bIsStunned = true;
			GoblinAnimInstance->bCanWalk = true;
		}
		else
		{
			GoblinAnimInstance->bIsStunned = false;
			GoblinAnimInstance->bCanWalk = false;
		}
	}
	else
	{
		GoblinAnimInstance->bIsStunned = false;

		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			GoblinAnimInstance->bCanWalk = true;
		}
	}
	
}

void AGoblinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGoblinCharacter::PossessedBy(AController* NewController)
{
	NewController = Cast<AGoblinAIController>(NewController);

	Super::PossessedBy(NewController);
}


