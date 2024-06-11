// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ObjectPoolSettings.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FPooledActorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InitialSpawnCount = 0;

	// Can expand actor pool if needed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Can Expand If Needed")
	bool bCanExpand = true;
};

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Object Pool"))
class OBJECTPOOL_API UObjectPoolSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UObjectPoolSettings()
	{
	};

public:
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); };
	virtual FName GetContainerName() const override { return TEXT("Project"); };
	virtual FName GetSectionName() const override { return TEXT("ObjectPool"); };

	// Destroy all pool actors when pool is destroyed
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEndPlay = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AActor>, FPooledActorSettings> InitialActorsToPool;
};
