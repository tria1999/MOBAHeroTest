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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class AMobaTestHero;

UCLASS()
class MOBATEST_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float PitchChangeRate =90.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float YawRate = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float PanRate = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float ZoomRate = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float ScreenEdgePanZonePercent = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxPitch = 335.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinPitch = 285.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxZoom = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinZoom = 300.0f;


	void PanForward(float magnitude);
	void PanRight(float magnitude);
	void ZoomIn(float magnitude);
	void ChangePitch(float magnitude);
	void Yaw(float magnitude);
	void EngageRotation();
	void DisengageRotation();
	void Orbit(float magnitude);
	bool rotationMode = false;

	FVector2D mouseScreenLocationGrab;	
	UFloatingPawnMovement* MovementComponent;
	USphereComponent* CustomCollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Main Character")
	AMobaTestHero* MainCharacter;

	void CommandMove();
};
