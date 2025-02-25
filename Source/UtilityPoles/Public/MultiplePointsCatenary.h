// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatenaryBase.h"
#include "MultiplePointsCatenary.generated.h"

/**
 * 
 */
UCLASS()
class UTILITYPOLES_API AMultiplePointsCatenary : public ACatenaryBase
{
	GENERATED_BODY()

	AMultiplePointsCatenary();

public:
	UFUNCTION(CallInEditor, Category = "Generation")
	void Generate() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateWire();

	UPROPERTY(EditInstanceOnly, meta = (MakeEditWidget = true), Category = "Generation")
	TArray<FVector> CatenaryConectionPoints;

private:

	USceneComponent* SceneComponent;
	
};
