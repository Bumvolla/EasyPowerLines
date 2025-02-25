// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatenaryBase.h"
#include "PoleBasedCatenary.generated.h"

/**
 * 
 */
UCLASS(NotPlaceable)
class UTILITYPOLES_API APoleBasedCatenary : public ACatenaryBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, Category = "Generation")
	float RandomTilt = 0;

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

protected:

	void SnapToTerrain(const FTransform& OgTransform, FTransform& SnapedTransform, ECoordSystem CoordSystem = ECoordSystem::COORD_Local);

	void AlignToNormal(const FVector& ImpactNormal, FQuat& NormalAlignedQuat);

	void RandomizeTilt(const FTransform& OgTransform, FTransform& RandomRotatedTransform);

	void DrawDebugLines(FVector StartPoint, FVector EndPoint, bool bHit, FHitResult Hit);
	
};
