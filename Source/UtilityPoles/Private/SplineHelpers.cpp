// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineHelpers.h"

TArray<FTransform> USplineHelpers::GetTransformPointsAlongSpline(const USplineComponent* spline, const float distBetweenObjects, const bool bFacingUp)
{
	float splineLength = spline->GetSplineLength();
	int32 instancesToSpawn = FMath::CeilToInt(splineLength / distBetweenObjects);

	TArray<FTransform> SpawnPointsTransform;

	for (int32 i = 0; i <= instancesToSpawn; i++)
	{
		float distanceAlongSpline = (splineLength / instancesToSpawn) * i;

		FTransform splinePointTransform = spline->GetTransformAtDistanceAlongSpline(distanceAlongSpline, ESplineCoordinateSpace::Local);
		FTransform facingUpTransform = FTransform
			(FRotator(0.f, splinePointTransform.Rotator().Yaw, splinePointTransform.Rotator().Roll),
			splinePointTransform.GetLocation(),
			FVector(1.f, 1.f, 1.f));

		SpawnPointsTransform.Add(bFacingUp ? facingUpTransform : splinePointTransform);
	}

	return SpawnPointsTransform;
}

float USplineHelpers::GetMeshLenght(const UStaticMesh* Mesh, EAxis::Type Axis)
{

	if (!Mesh) return 0.0;

	FVector MinBoundingBox = Mesh->GetBoundingBox().Min;
	FVector MaxBoundingBox = Mesh->GetBoundingBox().Max;

	switch (Axis)
	{
	case EAxis::X:
		return FMath::Abs(MinBoundingBox.X) + FMath::Abs(MaxBoundingBox.X);
		break;
	case EAxis::Y:
		return FMath::Abs(MinBoundingBox.Y) + FMath::Abs(MaxBoundingBox.Y);
		break;
	case EAxis::Z:
		return FMath::Abs(MinBoundingBox.Z) + FMath::Abs(MaxBoundingBox.Z);
		break;
	default:
		return FMath::Abs(MinBoundingBox.X) + FMath::Abs(MaxBoundingBox.X);
		break;
	}

}

int USplineHelpers::GetMeshesCountInSpline(const USplineComponent* Spline, const UStaticMesh* Mesh, EAxis::Type Axis)
{
	return FMath::TruncToInt(Spline->GetSplineLength() / GetMeshLenght(Mesh, Axis));
}

void USplineHelpers::GetSplineMeshStartAndEndByIteration(const int Index, const float Bound, const USplineComponent* Spline, FVector& StartPosition, FVector& StartTangent, FVector& EndPosition, FVector& EndTangent, const float& NewStartPosition)
{
	float StartDistance = Index * Bound + NewStartPosition;
	float EndDistance = (Index + 1) * Bound + NewStartPosition;

	StartPosition = Spline->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	StartTangent = Spline->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local).GetClampedToSize(0, Bound);
	EndPosition = Spline->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	EndTangent = Spline->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local).GetClampedToSize(0, Bound);
}

float USplineHelpers::GetDistanceBetweenSplinePoints(const USplineComponent* Spline, const int32& Point1, const int32& Point2)
{
	return FMath::Abs(Spline->GetDistanceAlongSplineAtSplinePoint(Point1) - Spline->GetDistanceAlongSplineAtSplinePoint(Point2));
}

int USplineHelpers::GetMeshCountBewteenSplinePoints(const USplineComponent* Spline, const UStaticMesh* Mesh, EAxis::Type Axis, const int32& Point1, const int32& Point2)
{
	return FMath::TruncToInt(USplineHelpers::GetDistanceBetweenSplinePoints(Spline, Point1, Point2) / USplineHelpers::GetMeshLenght(Mesh, Axis));
}

TArray<FVector> UCatenaryHelpers::CreateCatenaryNewton(const FVector& StartPoint, const FVector& EndPoint, const float Slack, const float SlackVariation, int32 Steps)
{

	TArray<FVector> Points;

	if (FVector::PointsAreNear(StartPoint, EndPoint, 20.))
	{
		return Points;
	}

	float TotalDistance = FVector::Dist(StartPoint, EndPoint);
	float HorizontalDistance = FVector::Dist(
		FVector(EndPoint.X, EndPoint.Y, StartPoint.Z),
		StartPoint
	);
	float ran = FMath::FRandRange(SlackVariation * -1, SlackVariation);
	float WireLength = TotalDistance + FMath::Max(50.f, Slack+ran);
	float HeightDiff = EndPoint.Z - StartPoint.Z;

	float TargetRatio = FMath::Sqrt(
		FMath::Pow(WireLength, 2.f) - FMath::Pow(HeightDiff, 2.f)
	) / HorizontalDistance;

	float Z = FindParameterNewton(TargetRatio);

	float A = HorizontalDistance / (2.0f * Z);
	float P = (HorizontalDistance - A * FMath::Loge(
		(WireLength + HeightDiff) / (WireLength - HeightDiff)
	)) / 2.0f;
	float Q = (EndPoint.Z + StartPoint.Z -
		WireLength * FMath::Cosh(Z) / FMath::Sinh(Z)) / 2.0f;

	Points.SetNum(Steps);

	for (int32 i = 0; i < Steps; ++i)
	{
		float T = static_cast<float>(i) / (Steps - 1);

		FVector Pos;
		Pos.X = FMath::Lerp(StartPoint.X, EndPoint.X, T);
		Pos.Y = FMath::Lerp(StartPoint.Y, EndPoint.Y, T);
		Pos.Z = A * FMath::Cosh((T * HorizontalDistance - P) / A) + Q;

		Points[i] = Pos;
	}

	return Points;
}

TArray<FVector> UCatenaryHelpers::CreateCatenaryFixed(const FVector& StartPoint, const FVector& EndPoint, float Slack, int32 Steps)
{

	// Same as Newton version but using FindParameterApproximate
	float TotalDistance = FVector::Dist(StartPoint, EndPoint);
	float HorizontalDistance = FVector::Dist(
		FVector(EndPoint.X, EndPoint.Y, StartPoint.Z),
		StartPoint
	);
	float WireLength = TotalDistance + FMath::Max(0.0001f, Slack);
	float HeightDiff = EndPoint.Z - StartPoint.Z;

	float TargetRatio = FMath::Sqrt(
		FMath::Pow(WireLength, 2.f) - FMath::Pow(HeightDiff, 2.f)
	) / HorizontalDistance;

	float Z = FindParameterFixed(TargetRatio);

	float A = HorizontalDistance / (2.0f * Z);
	float P = (HorizontalDistance - A * FMath::Loge(
		(WireLength + HeightDiff) / (WireLength - HeightDiff)
	)) / 2.0f;
	float Q = (EndPoint.Z + StartPoint.Z -
		WireLength * FMath::Cosh(Z) / FMath::Sinh(Z)) / 2.0f;

	TArray<FVector> Points;
	Points.SetNum(Steps);

	for (int32 i = 0; i < Steps; ++i)
	{
		float T = static_cast<float>(i) / (Steps - 1);

		FVector Pos;
		Pos.X = FMath::Lerp(StartPoint.X, EndPoint.X, T);
		Pos.Y = FMath::Lerp(StartPoint.Y, EndPoint.Y, T);
		Pos.Z = A * FMath::Cosh((T * HorizontalDistance - P) / A) + Q;

		Points[i] = Pos;
	}

	return Points;
}

float UCatenaryHelpers::FindParameterNewton(float TargetRatio)
{
	float Z = 1.0f;  // Initial guess
	constexpr int32 MAX_ITERATIONS = 8;
	constexpr float TOLERANCE = 0.01f;

	for (int32 i = 0; i < MAX_ITERATIONS; ++i)
	{
		float Ratio = FMath::Sinh(Z) / Z;
		float Derivative = (FMath::Cosh(Z) * Z - FMath::Sinh(Z)) / (Z * Z);

		float Delta = (Ratio - TargetRatio) / Derivative;
		Z -= Delta;

		if (FMath::Abs(Delta) < TOLERANCE)
			break;
	}

	return Z;
}

float UCatenaryHelpers::FindParameterFixed(float TargetRatio)
{
	float Z = 1.0f;
	constexpr int32 MAX_STEPS = 8;
	float StepSize = 10.0f;

	for (int32 i = 0; i < MAX_STEPS; ++i)
	{
		for (int32 j = 0; j < 5; ++j)
		{
			float TestZ = Z + StepSize;
			float Ratio = FMath::Sinh(TestZ) / TestZ;

			if (isinf(Ratio))
				break;

			if (FMath::Abs(Ratio - TargetRatio) < 0.05f)
				return TestZ;

			if (Ratio > TargetRatio)
				break;

			Z = TestZ;
		}
		StepSize *= 0.2f;
	}
	return Z;
}


#if WITH_EDITOR

TArray<AActor*> UEditorHelpers::GetObjectsInRadius(const FVector& Center, float Radius)
{
	TArray<AActor*> HitActors;

	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (World)
		{
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams CollisionParams;
			CollisionParams.bTraceComplex = true;
			CollisionParams.bReturnPhysicalMaterial = false;

			// Perform the overlap check
			bool bHit = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECC_WorldStatic, // You can change the collision channel as needed
				FCollisionShape::MakeSphere(Radius),
				CollisionParams
			);

			if (bHit)
			{
				for (const FOverlapResult& OverlapResult : OverlapResults)
				{
					AActor* HitActor = OverlapResult.GetActor();
					if (HitActor)
					{
						HitActors.Add(HitActor);
					}
				}
			}

		}
	}

	return HitActors;

}

#endif