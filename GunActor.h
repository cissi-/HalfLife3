// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GunActor.generated.h"

UCLASS()
class HALFLIFE3_API AGunActor : public AActor
{
	GENERATED_BODY()

	/** Indicates weather gun is active (holding an object) or not */
	UPROPERTY(VisibleAnywhere, Category = GunStats)
		bool GunActiveStatus;

	/** Gun mesh component */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* GunMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* MuzzleLocation;

	UFUNCTION()
		void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool BFromSweep, const FHitResult& SweepResult);

	
public:	
	// Sets default values for this actor's properties
	AGunActor();

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystemComponent* ParticleComponent;

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AHalfLife3Projectile> ProjectileClass;

	/** grabbed objects offset from gunmuzzle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float GrabOffset;

	/** Force to push items with right trigger*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float GunForce;

	/** Range from where you can pick up items with left trigger*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float GunRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UPrimitiveComponent* PickedUpComponent;

	UPROPERTY(VisibleAnywhere)
		class AActor* PickedUpActor;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* GunUserCamera;

	UFUNCTION()
		void LeftFire(UCameraComponent* Camera);

	UFUNCTION()
		void RightFire(UCameraComponent* Camera);

	
	
};
