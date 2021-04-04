// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

/* 	FString Log = TEXT("Hello!");
	FString* PtrLog = &Log; // Pointer stores the address of Log

	Log.Len();
	*PtrLog.Len(); //Won't work due to lower precedence of *
	(*PtrLog).Len();
	PtrLog->Len; // Works; overloaded operator -> */

	FString ObjectName = GetOwner()->GetName();
	FString ObjecLocation = GetOwner()->GetActorLocation().ToCompactString();

	UE_LOG(LogTemp, Warning, TEXT("This component is attached to %s, placed at %s."), *ObjectName, *ObjecLocation);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

