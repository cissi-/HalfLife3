// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HalfLife3HUD.generated.h"

UCLASS()
class AHalfLife3HUD : public AHUD
{
	GENERATED_BODY()

public:
	AHalfLife3HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

