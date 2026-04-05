// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "AI/Decorators/ScWBTD_Tickable.h"

#include "ScWBTD_CanShoot.generated.h"

#define MODULE_API SCWAIEXTENSIONS_API

/**
 *	Decorator that passes when the pawn's equipped weapon is ready to fire.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] BTD_CanShoot"))
class UScWBTD_CanShoot : public UScWBTD_Tickable
{
	GENERATED_BODY()

public:

	UScWBTD_CanShoot();

//~ Begin Decorator
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator
};

#undef MODULE_API
