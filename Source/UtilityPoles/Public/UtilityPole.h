/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "PoleBasedCatenary.h"
#include "UtilityPole.generated.h"

/**
 * 
 */
UCLASS(Placeable)
class UTILITYPOLES_API AUtilityPole : public APoleBasedCatenary
{
	GENERATED_BODY()
	
public:
	AUtilityPole();

	UFUNCTION(CallInEditor, Category = "Generation")
	virtual void Generate() override;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	TSubclassOf<AUtilityPolePreset> PresetClass;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	AUtilityPole* ActorToConectTo;

protected:

	UChildActorComponent* PoleComponent;

	void GeneratePole();
	void GenerateWires();

};
