// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/ChildActorComponent.h"

#include "CatenaryBase.h"
#include "UtilityPolePreset.h"

#include "SplineUtilityPole.generated.h"

UCLASS()
class UTILITYPOLES_API ASplineUtilityPole : public ACatenaryBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineUtilityPole();

protected:

	void GenerateWires();

	void GeneratePoles();

	void RemoveExcesPoles(int32 NeededPoleCount);

	void ReuseOrCreatePoles(TArray<FTransform> AllPoleTransforms);

	USplineComponent* Spline;

private:

	TArray<UChildActorComponent*> PoleIndices;

public:	

	UFUNCTION(CallInEditor, Category = "Generation")
	virtual void Generate() override;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	float DistanceBetweenObjects = 1000;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	TSubclassOf<AUtilityPolePreset> PresetClass;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	bool bIsClosedLoop = false;

};
