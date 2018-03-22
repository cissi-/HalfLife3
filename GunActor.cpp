// Fill out your copyright notice in the Description page of Project Settings.

#include "GunActor.h"
#include "HalfLife3Projectile.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/MeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Public/DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimInstance.h"


// Sets default values
AGunActor::AGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Gun starts not holding any objects
	GunActiveStatus = false;

	// Default offset from the character location for projectiles to spawn
	GrabOffset = 300.f;

	// Default force when pushing objects with gun
	GunForce = 1000000.f;

	//Default range when picking up items
	GunRange = 5000.f;

	// Create a gun mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	// GunMesh->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	GunMesh->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

}

// Called when the game starts or when spawned
void AGunActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GunActiveStatus)
	{
		PickedUpComponent->SetAllPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false);
		FVector ActorLocation = MuzzleLocation->GetComponentLocation() + GunUserCamera->GetForwardVector()*GrabOffset;
		PickedUpActor->SetActorLocation(ActorLocation);
	}

}

void AGunActor::OnPlayerEnterPickupBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherbodyIndex, bool BFromSweep, const FHitResult & SweepResult)
{
	//Attach Gun to Player or put in player inventory
}

/** Called when fireing the gun with left trigger, draws an object to the player */
void AGunActor::LeftFire(UCameraComponent* Camera)
{
	GunUserCamera = Camera;

	// Raycast to find object in trigger sight
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector();
	FVector EndTrace = StartTrace + (Direction * GunRange);
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true);

	//If gun is holding an item, drop the item
	if (GunActiveStatus)
	{
		PickedUpComponent->DetachFromParent(true);
		PickedUpComponent->SetEnableGravity(true);
		GunActiveStatus = false;
	}
	//If raycasting is successful
	else if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		//If gun is not holding an item, pick up the item found in the trigger sight, unless it is not movable
		if (HitResult->GetActor() != NULL && !GunActiveStatus && HitResult->GetActor()->IsRootComponentMovable())
		{
			PickedUpComponent = HitResult->GetComponent();
			PickedUpActor = HitResult->GetActor();
			PickedUpComponent->AttachToComponent(GunMesh, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			FVector ActorLocation = MuzzleLocation->GetComponentLocation() + Direction*GrabOffset;
			PickedUpActor->SetActorLocation(ActorLocation);
			PickedUpComponent->SetEnableGravity(false);
			GunActiveStatus = true;
		}
	}
}

/** Called when fireing the gun with right trigger, pushes items away */
void AGunActor::RightFire(UCameraComponent* Camera)
{
	GunUserCamera = Camera;

	// Raycast to find object in trigger sight
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector();
	FVector EndTrace = StartTrace + (Direction * 5000.f);
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);


	//If gun is holding an item, push that item
	if (GunActiveStatus)
	{
		PickedUpComponent->DetachFromParent(true);
		PickedUpComponent->SetEnableGravity(true);
		PickedUpComponent->AddImpulseAtLocation(Direction * GunForce, PickedUpComponent->GetComponentLocation());
		GunActiveStatus = false;
	}
	//If gun is not holding an item, push the item found in the trigger sight
	else if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		if (HitResult->GetActor() != NULL && !GunActiveStatus)
		{
			HitResult->GetComponent()->AddImpulseAtLocation(Direction * GunForce, HitResult->GetActor()->GetActorLocation());
			GunActiveStatus = false;
		}
	}

	if (ParticleComponent != NULL)
	{
		ParticleComponent->AttachTo(MuzzleLocation);
		ParticleComponent->Activate();
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

