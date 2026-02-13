// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "ScWBTT_TryActivateAbility.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] BTT_TryActivateAbility", KeyWords = "TryUseAbility, ActivateAbility"))
class UScWBTT_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UScWBTT_TryActivateAbility();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "AbilityClassKey != nullptr"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector AbilityClassKey; // If set, try activate ability by class
//~ End Settings
};
