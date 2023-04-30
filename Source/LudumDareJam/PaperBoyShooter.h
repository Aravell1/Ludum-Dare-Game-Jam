// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperBoyShooter.generated.h"

UCLASS()
class LUDUMDAREJAM_API APaperBoyShooter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaperBoyShooter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
				
#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", DisplayName = "Camera Arm")
	USpringArmComponent* CameraArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraComponent;

	#pragma region Bike Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bike", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BikeStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bike", meta = (AllowPrivateAccess = "true"))
	USceneComponent* AnchorPoint = nullptr;

	#pragma endregion

	#pragma region Shooter Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooters Cart", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CartStaticMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooters Cart", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShooterStaticMesh = nullptr;

	#pragma endregion

#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	AActor* PaperTemplate = nullptr;
	
#pragma region Camera Inputs

	void AddPitchInput(const float Axis);

	void AddYawInput(const float Axis);

#pragma endregion

	void Shoot();

	void MoveForward(const float Scale);
	void SteerRight(const float Scale);

	void IK_Cart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
