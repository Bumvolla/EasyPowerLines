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

	void SnapToTerrain(const FTransform& OgTransform, FTransform& SnapedTransform);

	void RemoveExcesPoles(int32 NeededPoleCount);

	void ReuseOrCreatePoles(TArray<FTransform> AllPoleTransforms);

	USplineComponent* Spline;

private:

	TArray<UChildActorComponent*> PoleIndices;

	void DrawDebugLines(FVector StartPoint, FVector EndPoint, bool bHit, FHitResult Hit);

public:	

	UFUNCTION(CallInEditor, Category = "Generation")
	virtual void Generate() override;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	float DistanceBetweenObjects = 1000;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	float RandomTilt = 0;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	TSubclassOf<AUtilityPolePreset> PresetClass;

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	bool bIsClosedLoop = false;

	UPROPERTY(EditInstanceOnly, Category = "Snaping")
	bool bSnapToTerrain = false;

	UPROPERTY(EditInstanceOnly, Category = "Snaping")
	float RayLength = 200;

	UPROPERTY(EditInstanceOnly, Category = "Snaping")
	bool bAlignToNormal = false;

	UPROPERTY(EditInstanceOnly, Category = "Snaping")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditInstanceOnly, Category = "Snaping")
	bool bDrawDebugLines = false;

};
