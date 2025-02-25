/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "CatenaryBase.h"
#include "PointToPointCatenary.generated.h"

/**
 * 
 */
UCLASS(Placeable)
class UTILITYPOLES_API APointToPointCatenary : public ACatenaryBase
{
	GENERATED_BODY()

	APointToPointCatenary();

public:
	UFUNCTION(CallInEditor, Category = "Generation")
	void Generate() override;

	void GenerateWire();

	UPROPERTY(EditInstanceOnly, meta = (MakeEditWidget = true), Category = "Generation")
	FVector SplineStartPoint;
	UPROPERTY(EditInstanceOnly, meta = (MakeEditWidget = true), Category = "Generation")
	FVector SplineEndPoint;
	
private:

	USceneComponent* SceneComponent;
};
