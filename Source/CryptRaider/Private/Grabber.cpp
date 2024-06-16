// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on %s"), *GetOwner()->GetName());
		return;
	}

	if(PhysicsHandle->GrabbedComponent != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("No PhysicsHandleComponent found on %s"), *GetOwner()->GetName());
		return;
	}

	if(PhysicsHandle->GrabbedComponent == nullptr) return;

	PhysicsHandle->GrabbedComponent->GetOwner()->Tags.Remove("Grabbed");

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("TryGrab"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("No PhysicsHandleComponent found on %s"), *GetOwner()->GetName());
		return;
	}

	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + (GetForwardVector() * Reach);
	
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, CollisionShape);

	if(!HasHit) return;

	UE_LOG(LogTemp, Display, TEXT("HasHit"));

	HitResult.GetActor()->Tags.Add("Grabbed");

	AActor* ActorHit = HitResult.GetActor();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	ComponentToGrab->SetSimulatePhysics(true);
	ComponentToGrab->WakeAllRigidBodies();

	ActorHit->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const {
		return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}
