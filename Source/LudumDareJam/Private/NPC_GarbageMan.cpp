// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_GarbageMan.h"

void ANPC_GarbageMan::BeginPlay()
{
	Super::BeginPlay();

	if (!AIController)
		AIController = GetController<ANPCAIController>();
	if (AIController)
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ANPC_GarbageMan::OnMoveCompleted);

	PathToFollow.Add(GetActorLocation());
	if (TargetPath.Num() > 0)
		for (int i = 0; i < TargetPath.Num(); i++)
			PathToFollow.Add(TargetPath[i]->GetActorLocation());
}

void ANPC_GarbageMan::PrimaryBehaviour()
{
	if (PathToFollow.Num() > 1)
		AIController->MoveToLocation(PathToFollow[PathIndex]);
}

void ANPC_GarbageMan::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		GetWorldTimerManager().SetTimer(PauseTimer, this, &ANPC_GarbageMan::BeginMoving, FMath::RandRange(PauseDuration * 0.8f, PauseDuration * 1.2f));
	}
}

void ANPC_GarbageMan::BeginMoving()
{
	PathIndex++;
	if (PathIndex >= PathToFollow.Num())
		PathIndex = 0;
}
