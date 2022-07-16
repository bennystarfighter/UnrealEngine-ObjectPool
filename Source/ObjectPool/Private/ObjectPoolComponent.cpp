// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"


// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	// ...
}

void UObjectPoolComponent::Push(AActor* Actor, bool& Success_Out)
{
	if (IsValid(Actor))
	{
		if (Actor->GetClass()->ImplementsInterface(UObjectPool::StaticClass()))
		{
			InactivePool.Add(Actor);

			// Remove from active pool
			if (int32 i = ActivePool.Find(Actor); i != INDEX_NONE)
			{
				ActivePool.RemoveAt(i, 1, false);
			}
			else
			{
				i = NULL;
			}

			// Reset + disable actor
			Actor->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			if (Actor->GetActorEnableCollision())
			{
				Actor->SetActorEnableCollision(false);
			}
			IObjectPool::Execute_OnPushed(Actor);
			Success_Out = true;
			return;
		}
	}

	Success_Out = false;
	return;
}

void UObjectPoolComponent::Pull(AActor*& Actor_Out, bool& Success_Out)
{
	Success_Out = false;

	// Find first available item in pool
	for (int i = 0; i < InactivePool.Num(); ++i)
	{
		if (IsValid(InactivePool[i]))
		{
			Actor_Out = ActivePool[ActivePool.Add(InactivePool[i])];
			InactivePool.Remove(Actor_Out);

			IObjectPool::Execute_OnPulled(Actor_Out, this);

			Success_Out = true;
			return;
		}
	}

	// Spawn new one and thereby expand the pool if that option is activated
	if (!Success_Out && bCanExpandIfNeeded)
	{
		Actor_Out = SpawnNewActor();
		if (IsValid(Actor_Out))
		{
			Success_Out = true;
			return;
		}
	}
}

AActor* UObjectPoolComponent::SpawnNewActor()
{
	if (UWorld* World = this->GetWorld(); World != nullptr)
	{
		FActorSpawnParameters* SpawnParameters = new FActorSpawnParameters;
		SpawnParameters->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AActor* SpawnedActor = World->SpawnActor<AActor>(ObjectClass,FVector::ZeroVector, FRotator::ZeroRotator, *SpawnParameters); IsValid(SpawnedActor))
		{
			bool Result;
			Push(SpawnedActor, Result);
			return SpawnedActor;
		}
	}
	return nullptr;
}

// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InitialSpawnAmount; ++i)
	{
		if (SpawnNewActor() == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ObjectPoolComponent spawned NULL AActor.") );
		}
	}
	// ...
}



// Called every frame
void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
