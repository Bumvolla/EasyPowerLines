// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatenaryBase.h"
#include "UtilityPole.generated.h"

/**
 * 
 */
UCLASS()
class UTILITYPOLES_API AUtilityPole : public ACatenaryBase
{
	GENERATED_BODY()
	
public:
	AUtilityPole();

	UFUNCTION(CallInEditor, Category = "Generation")
	virtual void Generate() override;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	TSubclassOf<AUtilityPolePreset> PresetClass;

	UPROPERTY(EditInstanceOnly, Category = "Generation", meta = (AllowedClasses = "UtilityPolePreset"))
	UObject* ActorToConectTo;

protected:

	UChildActorComponent* PoleComponent;

	void GeneratePole();
	void GenerateWires();

};
