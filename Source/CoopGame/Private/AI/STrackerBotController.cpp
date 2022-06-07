#include "STrackerBotController.h"


ASTrackerBotController::ASTrackerBotController()
{
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 2500;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	HearingConfig->bUseLoSHearing = false;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponenet"));
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->SetDominantSense(HearingConfig->GetSenseImplementation());
	AIPerceptionComponent->SetSenseEnabled(HearingConfig->GetSenseImplementation(), true);

	Target = nullptr;

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ASTrackerBotController::HandlePerceptionUpdated);

	TeamId = FGenericTeamId(2);
}

void ASTrackerBotController::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	FActorPerceptionBlueprintInfo Info;
	for (AActor* Actor : UpdatedActors)
	{
		Target = Actor;
		UE_LOG(LogTemp, Log, TEXT("Stimulus Actor: %s"), *Actor->GetName());
		AIPerceptionComponent->GetActorsPerception(Actor, Info);
		for (FAIStimulus Stimulus : Info.LastSensedStimuli)
		{
			UE_LOG(LogTemp, Log, TEXT("Stimulus location: %s"), *Stimulus.StimulusLocation.ToString());
			UE_LOG(LogTemp, Log, TEXT("Stimulus tag: %s"), *Stimulus.Tag.ToString());
		}
	}
}

AActor* ASTrackerBotController::GetTarget() const
{
	return Target;
}
