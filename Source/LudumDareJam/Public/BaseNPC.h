// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "BaseNPC.generated.h"

UCLASS()
class LUDUMDAREJAM_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float ScoreValue = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float TimeBetweenBehaviours = 10.0f;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float LaunchForce = 150.0f;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float MovementSpeed = 100.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PrimaryBehaviour() {};

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle BehaviourTimer;
	TArray<FVector> PathToFollow;
	int PathIndex = 1;

private:


};
