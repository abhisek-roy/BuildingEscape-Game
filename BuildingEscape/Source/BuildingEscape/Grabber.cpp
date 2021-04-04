// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Warning, TEXT("Grabber reporting to duty!"));	
	CheckPhysicsHandle();
	SetupInputHandle();
}

void UGrabber::CheckPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s attached to %s has not initialized yet."), 
			*PhysicsHandle->GetName(), *GetOwner()->GetName()
		);
	}
}

// Set up Input and callbacks
void UGrabber::SetupInputHandle()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle)
	{
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Find the Physics Object in front and grab it
void UGrabber::Grab()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grab called."));
	// Get the object layer Looking at on button press and attach
	
	GetFirstPhysicsBodyinReach();
	AActor* ActorHit = FirstHit.GetActor();
	UPrimitiveComponent* ComponentToGrab = FirstHit.GetComponent();
	if(ActorHit)
	{
		// UE_LOG(LogTemp, Warning, TEXT("We are hit: %s!"), *ActorHit->GetName());
		if(!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocation( 
			ComponentToGrab,
			NAME_None,
			GetPlayerReach()
		);
	}else{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsObject to be grabbed!"));	
	}
}

void UGrabber::Release()
{
	// Detach the object on button release
	if(!PhysicsHandle){return;}
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Warning, TEXT("Released: %s."), *FirstHit.GetActor()->GetName());
	}	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If PhysicsHandle is attached, move the attached body with Player
	if(!PhysicsHandle){return;}
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

void UGrabber::GetFirstPhysicsBodyinReach() //const
{
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	//FHitResult ActorHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT FirstHit, 
		GetPlayerPosWorld(), 
		GetPlayerReach(), 
		FCollisionObjectQueryParams(ECC_PhysicsBody), 
		TraceParam
	);

	//return ActorHit;
}

	FVector UGrabber::GetPlayerPosWorld() const
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

		return PlayerViewPointLocation;
	}

	FVector UGrabber::GetPlayerReach() const
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

		return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	}