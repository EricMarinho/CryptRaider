// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("TriggerComponent BeginPlay"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> OverlappingActors;

	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if(Actor->ActorHasTag(TagToCheck) && !(Actor->ActorHasTag("Grabbed")))
		{
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (Component != nullptr)
			{
				Component->SetSimulatePhysics(false);
			}
			Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			Mover->SetShouldMove(true);
			return;
		}
	}
	Mover->SetShouldMove(false);
}

void UTriggerComponent::SetMover(UMover* NewMover) 
{
	Mover = NewMover;
}