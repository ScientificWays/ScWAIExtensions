// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "ModularAIController.h"
#include "Team/ScWTeamAgentInterface.h"

#include "ScWAIController.generated.h"

#define MODULE_API SCWAIEXTENSIONS_API

/**
 *
 */
UCLASS(MinimalAPI, Abstract, meta = (DisplayName = "[ScW] AI Controller"))
class AScWAIController : public AModularAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
//~ Begin Initialize
public:	
	AScWAIController(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
protected:
	MODULE_API virtual void PreInitializeComponents() override; // AActor
	MODULE_API virtual void BeginPlay() override; // AActor
	MODULE_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // AActor
//~ End Initialize

//~ Begin Blackboard
protected:
	virtual bool InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) override; // AAIController
//~ End Blackboard

//~ Begin Team
public:

	UFUNCTION(Category = "Team", BlueprintCallable)
	MODULE_API FGenericTeamId GetTeamId() const { return GetGenericTeamId(); }

	MODULE_API virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
//~ End Team

//~ Begin Perception
public:

	UFUNCTION(Category = "Perception", BlueprintCallable, meta = (KeyWords = "GetPerceptionComponent, GetAIPC, GetPC"))
	MODULE_API class UScWAIPC_Base* GetBaseAIPC() const { return BaseAIPC; }

protected:

	UPROPERTY(Category = "Perception", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWAIPC_Base> BaseAIPC;
//~ End Perception

//~ Begin Navigation
public:

	UFUNCTION(Category = "Navigation | Patrol", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetRelevantPatrolPoint"))
	MODULE_API class AScWAIPatrolPoint* BP_GetRelevantPatrolPoint(const FVector& InReferenceLocation, int32 InDesiredPathPointOffset) const;

protected:

	UPROPERTY(Category = "Navigation", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWPFC_Base> BasePFC;

	UPROPERTY(Category = "Navigation | Patrol", EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class AScWAIPatrolPoint>> CurrentPatrolPoints;
//~ End Navigation
	
//~ Begin AbilitySystem
public:
	MODULE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface
//~ End AbilitySystem

//~ Begin Rotation
public:

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	MODULE_API void ClearAllFocuses();

	UPROPERTY(Category = "Rotation", EditAnywhere, BlueprintReadWrite)
	float FocusInterpSpeed;

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	MODULE_API void ForceControlRotationOnPawn();

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	MODULE_API void DisableForceControlRotationOnPawn();

	MODULE_API virtual FVector GetFocalPointOnActor(const AActor* Actor) const override; // AAIController

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	MODULE_API void AddControlRotationUpdateBlockSource(UObject* InObject) { ControlRotationUpdateBlockSet.Add(InObject); }

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	MODULE_API void RemoveControlRotationUpdateBlockSource(UObject* InObject) { ControlRotationUpdateBlockSet.Remove(InObject); }

protected:
	virtual void UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) override; // AAIController

	UPROPERTY(Category = "Rotation", BlueprintReadOnly)
	TSet<UObject*> ControlRotationUpdateBlockSet;

	UPROPERTY()
	bool bPrevForceControlRotationOnPawnCallUseYawValue;

	UPROPERTY()
	bool bPrevForceControlRotationOnPawnCallOrientValue;
//~ End Rotation
};

#undef MODULE_API
