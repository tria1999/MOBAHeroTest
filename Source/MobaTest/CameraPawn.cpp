// The MIT License (MIT)

// Copyright (c) 2015 Mark Vatsel
// Copyright (c) 2021 Triantafyllos Tsiakiris

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "CameraPawn.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EngineUtils.h"
#include "MobaTestHero.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CustomCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere_Component1"));
	CustomCollisionComponent->InitSphereRadius(100.0f);
	CustomCollisionComponent->CanCharacterStepUpOn = ECB_No;
	CustomCollisionComponent->SetCanEverAffectNavigation(false);
	CustomCollisionComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	RootComponent = CustomCollisionComponent;	

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement_Component1"));
	MovementComponent->UpdatedComponent = CustomCollisionComponent;

	
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AMobaTestHero> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr->GetName().Equals(TEXT("BP_MobaTestMainCharacter"), ESearchCase::CaseSensitive))
		{
			MainCharacter = Cast<AMobaTestHero>(*ActorItr);
			return;
		}
	}

 
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			FVector2D mouseLocation;
			if (playerController->GetMousePosition(mouseLocation.X, mouseLocation.Y))
			{
				int32 screenWidth = 0;
				int32 screenHeight = 0;
				playerController->GetViewportSize(screenWidth, screenHeight);
								
				if (rotationMode) //Main Camera rotation calls
				{
					Orbit((mouseScreenLocationGrab.X - mouseLocation.X) / screenWidth);
					ChangePitch((mouseScreenLocationGrab.Y - mouseLocation.Y)*-1 / screenWidth);
				}
				else //  Mouse on screen edge pan
				{		
					const float xPanZone = screenWidth * ScreenEdgePanZonePercent / 100;
					const float yPanZone = screenHeight * ScreenEdgePanZonePercent / 100;
					
					if (mouseLocation.X < xPanZone)							PanRight((1 - mouseLocation.X / xPanZone) * -1);
					else if (mouseLocation.X > screenWidth - xPanZone)		PanRight(1 - (screenWidth - mouseLocation.X) / xPanZone);
					if (mouseLocation.Y < yPanZone)							PanForward(1 - mouseLocation.Y / yPanZone);
					else if (mouseLocation.Y > screenHeight - yPanZone)		PanForward((1 - (screenHeight - mouseLocation.Y) / yPanZone)*-1);
					
				}
				
							
			}
		}
	}	

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputComponent);
	InputComponent->BindAxis("Pan Forward", this, &ACameraPawn::PanForward);
	InputComponent->BindAxis("Pan Right", this, &ACameraPawn::PanRight);
	InputComponent->BindAxis("Mouse Wheel", this, &ACameraPawn::ZoomIn);

	InputComponent->BindAction("Rotate Camera", IE_Pressed, this, &ACameraPawn::EngageRotation);
	InputComponent->BindAction("Rotate Camera", IE_Released, this, &ACameraPawn::DisengageRotation);
}


void ACameraPawn::ChangePitch(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
		if (magnitude < 0 && GetControlRotation().Pitch < MaxPitch || magnitude > 0 && GetControlRotation().Pitch > MinPitch)
			AddControllerPitchInput(magnitude * PitchChangeRate * GetWorld()->GetDeltaSeconds());		
}

void ACameraPawn::Yaw(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
	{
		AddControllerYawInput(magnitude * YawRate *GetWorld()->GetDeltaSeconds());
	}		
			
}

void ACameraPawn::Orbit(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			AddControllerYawInput(magnitude * YawRate *GetWorld()->GetDeltaSeconds());
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), magnitude * -(YawRate/4)* GetWorld()->GetDeltaSeconds());
		}
	}

}

void ACameraPawn::PanForward(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && Controller)
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetScaledAxis(EAxis::X), magnitude * 4000 * GetWorld()->GetDeltaSeconds());				
}

void ACameraPawn::PanRight(const float magnitude)
{
	if (magnitude != 0  && !rotationMode && Controller)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y),magnitude * PanRate* GetWorld()->GetDeltaSeconds());
}

void ACameraPawn::ZoomIn(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && Controller)
		if (magnitude > 0 && GetActorLocation().Z > MinZoom || magnitude < 0 && GetActorLocation().Z < MaxZoom)
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), magnitude * ZoomRate* GetWorld()->GetDeltaSeconds());	
}




void ACameraPawn::EngageRotation()
{	
	if (Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{				
			FVector2D mousePosition;
			if (playerController->GetMousePosition(mousePosition.X, mousePosition.Y))
			{
				mouseScreenLocationGrab = mousePosition;
				rotationMode = true;
			}
		}
	}
}

void ACameraPawn::DisengageRotation()
{
	rotationMode = false;
}


void ACameraPawn::CommandMove()
{
	UE_LOG(LogTemp,Warning,TEXT("Main Character is in location"));
}