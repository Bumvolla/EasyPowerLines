// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SplineHelpers.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "UtilityPolePreset.h"
#include "CatenaryBase.generated.h"

UCLASS()
class UTILITYPOLES_API ACatenaryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACatenaryBase();

protected:

	TArray<USplineComponent*> AllWires;
	TArray<USplineMeshComponent*> AllSplineMeshes;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Generate();

	UPROPERTY(EditInstanceOnly, Category = "Catenary")
	float Slack = 100;

	UPROPERTY(EditInstanceOnly, Category = "Catenary")
	float SlackVariation = 2;

	UPROPERTY(EditInstanceOnly, Category = "Catenary")
	uint16 SplineResolution = 10;

	UPROPERTY(EditInstanceOnly, Category = "Mesh")
	UStaticMesh* WireMesh;

	UPROPERTY(EditInstanceOnly, Category = "Mesh")
	TEnumAsByte<EAxis::Type> WireMeshAxis = EAxis::X;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	bool autoGenerate = false;

	float MeshLenght;

	void SetMeshLenght();

	void RemoveExcesSplines(int32 NeededSplines);

	void RemoveSplines();

	void RemoveSplineMeshes();

	void ReuseOrCreateSplines();

	TArray<FVector> CalculateSingleCatenary(TArray<FVector> ConectionPoints, bool bIsClosedLoop = false);

	TArray<TArray<FVector>> CalculateCatenariesParalel(const TArray<AUtilityPolePreset*>& ConectionPoints, bool bIsClosedLoop = false);

	void ConstructSplineMeshesAlongSplines(USplineComponent* Spline);

};