// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorRotation = GetOwner()->GetActorRotation();
	InitialYaw = ActorRotation.Yaw;
	CurrentYaw = InitialYaw;

	FindPressurePlate();
	CheckAudioHandle();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// GetTotalWeight
	if(GetTotalWeight() > ThresholdWeight)
	{
		OpenDoor(DeltaTime);
		DoorOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		float DoorShouldOpen = DoorOpened + DoorCloseDelay;
		if (GetWorld()->GetTimeSeconds() > DoorShouldOpen)
		{
			CloseDoor(DeltaTime);
		}
	}	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = ActorRotation.Yaw;
	ActorRotation.Yaw = FMath::FInterpTo(CurrentYaw, InitialYaw + MaxOpening, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
	// UE_LOG(LogTemp, Warning, TEXT("Yaw Absolute Angle: %f."), ActorRotation.Yaw);

	if(!AudioHandle) return;
	if(!OpenDoorSound)
	{
		AudioHandle->Play();
		UE_LOG(LogTemp, Warning, TEXT("Playing Door opening Sound."));
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	ActorRotation.Yaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
	
	if(!AudioHandle) return;
	if(!CloseDoorSound)
	{
		AudioHandle->Play();
		UE_LOG(LogTemp, Warning, TEXT("Playing Door closing Sound."));
		CloseDoorSound = true;
		OpenDoorSound = false;
	}
}

float UOpenDoor::GetTotalWeight() const
{
	float TotalWeight = 0.f;
	TArray<AActor*> ActorsOnPlate;
	PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);


	for(AActor* Actor : ActorsOnPlate)
	{
		TotalWeight += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalWeight;
}

void UOpenDoor::CheckAudioHandle()
{
	AudioHandle = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioHandle) return;
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has uninitialized PressurePlate field!"), *GetOwner()->GetName());
	}
}

