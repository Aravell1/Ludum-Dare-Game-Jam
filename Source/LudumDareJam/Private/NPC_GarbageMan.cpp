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
	if (TargetLocation)
		PathToFollow.Add(TargetLocation->GetActorLocation());
}

void ANPC_GarbageMan::PrimaryBehaviour()
{
	AIController->MoveToLocation(PathToFollow[PathIndex]);
}

void ANPC_GarbageMan::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		PathIndex++;
	}
}
