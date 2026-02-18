// Scientific Ways

#include "AI/ScWAIController.h"

#include "Tags/ScWCoreTags.h"

#include "Team/ScWTeamSettings.h"

#include "AI/ScWPFC_Base.h"
#include "AI/ScWAIPC_Base.h"
#include "AI/ScWAIFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWAIController)

//~ Begin Initialize
AScWAIController::AScWAIController(const FObjectInitializer& InObjectInitializer)
	//: Super(InObjectInitializer.SetDefaultSubobjectClass<UScWPFC_Base>(TEXT("PathFollowingComponent")))
	: Super(InObjectInitializer.DoNotCreateDefaultSubobject(TEXT("PathFollowingComponent")))
{
	//BasePFC = Cast<UScWPFC_Base>(GetPathFollowingComponent());
	BasePFC = CreateDefaultSubobject<UScWPFC_Base>(TEXT("BasePFC"));
	BasePFC->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
	SetPathFollowingComponent(BasePFC);

	BaseAIPC = CreateDefaultSubobject<UScWAIPC_Base>("BaseAIPC");
	SetPerceptionComponent(*BaseAIPC);

	TeamTag = FScWCoreTags::Team_None;
}

void AScWAIController::PreInitializeComponents() // AActor
{
	Super::PreInitializeComponents();

	SetTeamTag(TeamTag);
}

void AScWAIController::BeginPlay() // AActor
{
	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}
	Super::BeginPlay();
}

void AScWAIController::EndPlay(const EEndPlayReason::Type EndPlayReason) // AActor
{
	Super::EndPlay(EndPlayReason);


}
//~ End Initialize

//~ Begin Behavior Tree
//~ End Behavior Tree

//~ Begin Blackboard
bool AScWAIController::InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) // AAIController
{
	if (Super::InitializeBlackboard(InBlackboardComponent, InBlackboardAsset))
	{
		return true;
	}
	return false;
}
//~ End Blackboard

//~ Begin Team
FGenericTeamId AScWAIController::GetGenericTeamId() const // IGenericTeamAgentInterface
{
	if (bWantsPlayerState)
	{
		const IScWTeamAgentInterface* PlayerStateTeamInterface = Cast<IScWTeamAgentInterface>(PlayerState);
		ensureReturn(PlayerStateTeamInterface, FGenericTeamId::NoTeam);
		return PlayerStateTeamInterface->GetGenericTeamId();
	}
	return GetDefault<UScWTeamSettings>()->GetGenericTeamIdFromTag(TeamTag);
}

void AScWAIController::SetGenericTeamId(const FGenericTeamId& InTeamID) // IGenericTeamAgentInterface
{
	if (bWantsPlayerState)
	{
		IScWTeamAgentInterface* PlayerStateTeamInterface = Cast<IScWTeamAgentInterface>(PlayerState);
		ensureReturn(PlayerStateTeamInterface);
		PlayerStateTeamInterface->SetGenericTeamId(InTeamID);
	}
	Super::SetGenericTeamId(InTeamID);
	UpdateTeamAttitude(BaseAIPC);
}

void AScWAIController::SetTeamTag(const FGameplayTag& InTeamTag) // IScWTeamAgentInterface
{
	TeamTag = InTeamTag;
	SetGenericTeamId(GetDefault<UScWTeamSettings>()->GetGenericTeamIdFromTag(TeamTag));
}

FOnScWTeamIndexChangedDelegate* AScWAIController::GetOnTeamIndexChangedDelegate() // IScWTeamAgentInterface
{
	return &OnTeamChangedDelegate;
}

void AScWAIController::UpdateTeamAttitude(UAIPerceptionComponent* InPerception)
{
	if (InPerception)
	{
		InPerception->RequestStimuliListenerUpdate();
	}
}
//~ End Team

//~ Begin Navigation
AScWAIPatrolPoint* AScWAIController::BP_GetRelevantPatrolPoint_Implementation(const FVector& InReferenceLocation, int32 InDesiredPathPointOffset) const
{
	int32 NearestPointIndex = UScWAIFunctionLibrary::GetNearestPatrolPointIndex(InReferenceLocation, CurrentPatrolPoints, true);
	if (CurrentPatrolPoints.IsValidIndex(NearestPointIndex))
	{
		return CurrentPatrolPoints[(NearestPointIndex + InDesiredPathPointOffset) % CurrentPatrolPoints.Num()];
	}
	return nullptr;
}
//~ End Navigation

//~ Begin AbilitySystem
UAbilitySystemComponent* AScWAIController::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}
//~ End AbilitySystem

//~ Begin Rotation
void AScWAIController::ClearAllFocuses()
{
	for (FFocusKnowledge::FFocusItem& SamplePriority : FocusInformation.Priorities)
	{
		SamplePriority.Actor = nullptr;
		SamplePriority.Position = FAISystem::InvalidLocation;
	}
}

void AScWAIController::ForceControlRotationOnPawn()
{
	if (APawn* OwnerPawn = GetPawn())
	{
		bPrevForceControlRotationOnPawnCallUseYawValue = OwnerPawn->bUseControllerRotationYaw;
		OwnerPawn->bUseControllerRotationYaw = true;

		if (UCharacterMovementComponent* CharacterMovement = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>())
		{
			bPrevForceControlRotationOnPawnCallOrientValue = CharacterMovement->bOrientRotationToMovement;
			CharacterMovement->bOrientRotationToMovement = false;
		}
	}
}

void AScWAIController::DisableForceControlRotationOnPawn()
{
	if (APawn* OwnerPawn = GetPawn())
	{
		OwnerPawn->bUseControllerRotationYaw = bPrevForceControlRotationOnPawnCallUseYawValue;

		if (UCharacterMovementComponent* CharacterMovement = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>())
		{
			CharacterMovement->bOrientRotationToMovement = bPrevForceControlRotationOnPawnCallOrientValue;
		}
	}
}

FVector AScWAIController::GetFocalPointOnActor(const AActor* InActor) const // AAIController
{
	return InActor ? InActor->GetTargetLocation(const_cast<AScWAIController*>(this)) : FAISystem::InvalidLocation;
}

void AScWAIController::UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) // AAIController
{
	if (APawn* ControlledPawn = GetPawn())
	{
		FRotator NewControlRotation = GetControlRotation();

		if (ControlRotationUpdateBlockSet.IsEmpty())
		{
			const FVector FocalPoint = GetFocalPoint();

			if (FAISystem::IsValidLocation(FocalPoint))
			{
				FQuat CurrentQuat = GetControlRotation().Quaternion();
				FQuat TargetQuat = (FocalPoint - ControlledPawn->GetPawnViewLocation()).ToOrientationQuat();

				if (!CurrentQuat.Equals(TargetQuat, 1e-3f))
				{
					NewControlRotation = FQuat::Slerp(CurrentQuat, TargetQuat, FMath::Min(FocusInterpSpeed * InDeltaSeconds, 1.0f)).Rotator();
					SetControlRotation(NewControlRotation);
				}
			}
			else if (bSetControlRotationFromPawnOrientation)
			{
				NewControlRotation = ControlledPawn->GetActorRotation();
				SetControlRotation(NewControlRotation);
			}
		}
		if (bInUpdatePawn)
		{
			const FRotator CurrentPawnRotation = ControlledPawn->GetActorRotation();

			if (!CurrentPawnRotation.Equals(NewControlRotation, 1e-3f))
			{
				ControlledPawn->FaceRotation(NewControlRotation, InDeltaSeconds);
			}
		}
	}
}
//~ End Rotation
