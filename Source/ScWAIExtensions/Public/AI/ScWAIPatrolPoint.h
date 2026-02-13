// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "ScWAIPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, Abstract, meta = (DisplayName = "[ScW] AI Patrol Point"))
class AScWAIPatrolPoint : public AActor
{
	GENERATED_BODY()

public:

	AScWAIPatrolPoint();
	
//~ Begin Location
public:

	UFUNCTION(Category = "Location", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetMoveToLocation"))
	FVector BP_GetMoveToLocation() const;
//~ End Location
};
