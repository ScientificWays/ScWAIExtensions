// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "ScWBTD_Tickable.generated.h"

#define MODULE_API SCWAIEXTENSIONS_API

/**
 *	Abstract decorator base that re-evaluates its condition at a configurable tick rate.
 */
UCLASS(MinimalAPI, Abstract, meta = (DisplayName = "[ScW] BTD_Tickable"))
class UScWBTD_Tickable : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UScWBTD_Tickable();

//~ Begin Decorator
protected:
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float TickRate;
//~ End Settings
};

#undef MODULE_API
