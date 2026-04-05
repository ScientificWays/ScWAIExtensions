// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "ScWBTT_GetRelevantPatrolPoint.generated.h"

#define MODULE_API SCWAIEXTENSIONS_API

/**
 *	Task that queries the AI controller for the next patrol point and writes its location to a blackboard key.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] BTT_GetRelevantPatrolPoint", KeyWords = "FindRelevantPatrolPoint, FindRelevantReachablePatrolPoint"))
class UScWBTT_GetRelevantPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UScWBTT_GetRelevantPatrolPoint();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTask
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector LocationReferenceKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector DesiredPathPointOffsetKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	int32 DesiredPathPointOffset;
//~ End Settings
	
//~ Begin Output
protected:

	UPROPERTY(Category = "Output", EditAnywhere)
	FBlackboardKeySelector OutLocationKey;
//~ End Output
};

#undef MODULE_API
