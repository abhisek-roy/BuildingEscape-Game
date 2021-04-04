// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// User defined functions
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float GetTotalWeight() const;

private:
	float CurrentYaw, InitialYaw, DoorOpened = 0.f;
	FRotator ActorRotation;
	
	UPROPERTY(EditAnywhere)
	float ThresholdWeight = 10.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.4f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 3;
	
	UPROPERTY(EditAnywhere)
	float MaxOpening = 90.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioHandle = nullptr;

	bool OpenDoorSound = false, CloseDoorSound = true;
	void CheckAudioHandle();
	void FindPressurePlate();
};
