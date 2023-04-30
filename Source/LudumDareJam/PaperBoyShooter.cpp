// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperBoyShooter.h"

// Sets default values
APaperBoyShooter::APaperBoyShooter() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BikeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bike!"));
	RootComponent = BikeStaticMesh;
	AnchoringArm = CreateDefaultSubobject<USceneComponent>(TEXT("Anchoring Point"));

	CartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cart!"));
	ShooterStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shooting Kid!"));

	CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	
#pragma region Bike!

	if (AnchoringArm && BikeStaticMesh) {
		AnchoringArm->SetupAttachment(BikeStaticMesh);
	}

#pragma endregion 

#pragma region Shooter And Camera

	if (CameraArmComponent && FollowCameraComponent) {
		ShooterStaticMesh->SetupAttachment(CartStaticMesh);
		CameraArmComponent->SetupAttachment(ShooterStaticMesh);
		
		ShooterStaticMesh->AddLocalOffset(FVector(0, 0, 50));
		
		FollowCameraComponent->SetupAttachment(CameraArmComponent);
		CameraArmComponent->bEnableCameraLag = true;
		CameraArmComponent->bUsePawnControlRotation = true;

		CameraArmComponent->bInheritPitch = true;
		CameraArmComponent->bInheritYaw = true;

		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;

		CameraArmComponent->SocketOffset = FVector(0, 100,  100);
	}

#pragma endregion

}

// Called when the game starts or when spawned
void APaperBoyShooter::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void APaperBoyShooter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	const FRotator CurrentRotation = GetControlRotation();
	
}

// Called to bind functionality to input
void APaperBoyShooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Shooting
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APaperBoyShooter::Shoot);

	// Moving
	PlayerInputComponent->BindAxis("MoveForward", this, &APaperBoyShooter::MoveForward);
	PlayerInputComponent->BindAxis("SteerRight", this, &APaperBoyShooter::SteerRight);

	// Looking
	PlayerInputComponent->BindAxis("Look Up / Mouse", this, &APaperBoyShooter::AddPitchInput);
	PlayerInputComponent->BindAxis("Turn / Mouse", this, &APaperBoyShooter::AddYawInput);
}

#pragma region Camera Inputs

void APaperBoyShooter::AddPitchInput(const float Axis) {
	FollowCameraComponent->AddLocalRotation(FRotator(25 * Axis * GetWorld()->GetDeltaSeconds(), 0, 0));
}

void APaperBoyShooter::AddYawInput(const float Axis) {
	ShooterStaticMesh->AddLocalRotation(FRotator(0, 25 * Axis * GetWorld()->GetDeltaSeconds(), 0));;
}

#pragma endregion

void APaperBoyShooter::Shoot() {
	
}

void APaperBoyShooter::MoveForward(const float Scale) {
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddActorWorldOffset(Direction * Scale * 5, true);
}

void APaperBoyShooter::SteerRight(const float Scale) {
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(Scale));
	AddControllerYawInput(Scale * 60 * GetWorld()->GetDeltaSeconds());
}

void APaperBoyShooter::IK_Cart() {
	float eps = 0.01f;
	float length_bike_anchor = FVector::Distance(RootComponent->GetComponentLocation(), AnchorPoint->GetComponentLocation());
	float length_anchor_cart = FVector::Distance(AnchorPoint->GetComponentLocation(), CartStaticMesh->GetComponentLocation());

	
	
}