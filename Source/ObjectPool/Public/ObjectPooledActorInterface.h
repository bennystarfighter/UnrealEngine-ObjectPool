#pragma once

#include "CoreMinimal.h"
#include "ObjectPooledActorInterface.generated.h"

UINTERFACE()
class UObjectPooledActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OBJECTPOOL_API IObjectPooledActor 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ObjectPool")
	void OnPushedToPool();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ObjectPool")
	void OnPulledFromPool();
};
