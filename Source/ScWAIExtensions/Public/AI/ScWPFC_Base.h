// Scientific Ways

#pragma once

#include "ScWAIExtensions.h"

#include "Navigation/CrowdFollowingComponent.h"

#include "ScWPFC_Base.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AI PFC Base"))
class UScWPFC_Base : public UCrowdFollowingComponent
{
	GENERATED_BODY()

public:

	UScWPFC_Base();
	
//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // UActorComponent
//~ End Initialize
};
