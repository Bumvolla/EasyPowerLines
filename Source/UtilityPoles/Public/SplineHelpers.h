/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineComponent.h"

#include "Math/Axis.h"

#if WITH_EDITOR

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Editor.h"

#endif


#include "SplineHelpers.generated.h"

#define MIN_SLACK (50.f)

/**
 * 
 */
UCLASS()
class UTILITYPOLES_API USplineHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Spline Construction Script Tools")
	static TArray<FTransform> GetTransformPointsAlongSpline(const USplineComponent* spline, float distBetweenObjects, const bool bFacingUp = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Construction Script Tools")
	static float GetMeshLenght(const UStaticMesh* Mesh, EAxis::Type Axis);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spline Construction Script Tools")
	static int GetMeshesCountInSpline(const USplineComponent* Spline, const UStaticMesh* Mesh, EAxis::Type Axis);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spline Construction Script Tools")
	static void GetSplineMeshStartAndEndByIteration(const int Index, const float Bound, const USplineComponent* Spline, FVector& StartPosition, FVector& StartTangent, FVector& EndPosition, FVector& EndTangent, const float& NewStartPosition = 0);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spline Construction Script Tools")
	static float GetDistanceBetweenSplinePoints(const USplineComponent* Spline, const int32& Point1, const int32& Point2);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spline Construction Script Tools")
	static int GetMeshCountBewteenSplinePoints(const USplineComponent* Spline, const UStaticMesh* Mesh, EAxis::Type Axis, const int32& Point1, const int32& Point2);

};

UCLASS()
class UTILITYPOLES_API UCatenaryHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Newton's Method - Most accurate
	UFUNCTION(BlueprintCallable, Category = "Catenary")
	static TArray<FVector> CreateCatenaryNewton(const FVector& StartPoint, const FVector& EndPoint, const float Slack, const float SlackVariation, int32 Steps);

	// Fixed-point iteration - Balance of speed and accuracy
	UFUNCTION(BlueprintCallable, Category = "Catenary")
	static TArray<FVector> CreateCatenaryFixed(const FVector& StartPoint, const FVector& EndPoint, float Slack, int32 Steps);

private:
	static float FindParameterNewton(float TargetRatio);
	static float FindParameterFixed(float TargetRatio);

	static FORCEINLINE float Sinh(float Value) { return sinhf(Value); }
	static FORCEINLINE double Sinh(double Value) { return sinh(Value); }

	static FORCEINLINE float Cosh(float Value) { return coshf(Value); }
	static FORCEINLINE double Cosh(double Value) { return cosh(Value); }

};