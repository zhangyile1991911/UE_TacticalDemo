// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "BlueprintEditor.h"
#include "GridModifier.h"
#include "MyGridVisual.h"
#include "My_Utilities.h"
#include "TileData.h"
#include "Chaos/ClusterUnionManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"

#include "Math/Plane.h"
#include "Math/Vector.h"
#include "Math/UnrealMath.h"


// Sets default values
AGrid::AGrid():GridTileSize(200.0f,200.0f,100.0f),GridTileCount(10.0f,10.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// InstanceStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("static_mesh_instance"));

	ChildActor_GridVisual = CreateDefaultSubobject<UChildActorComponent>(TEXT("child_actor"));
	ChildActor_GridVisual->SetupAttachment(this->RootComponent);
	ChildActor_GridVisual->SetChildActorClass(AMyGridVisual::StaticClass());
	
	UE_LOG(LogTemp,Log,TEXT("AGrid::AGrid() default construct %s at %s thread: %d"),*GetName(),*FDateTime::Now().ToString(), FPlatformTLS::GetCurrentThreadId())
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(ChildActor_GridVisual->GetChildActor() == nullptr)
	{
		ChildActor_GridVisual->CreateChildActor();
		GridVisual = Cast<AMyGridVisual>(ChildActor_GridVisual->GetChildActor());	
	}
	
	if(GridVisual)SpawnGrid();
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	GridCenterLocation = GetActorLocation();
	// SpawnGrid();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// total += DeltaTime;
	// if(total >= 2.0f)
	// {
	// 	GridCenterLocation = GetActorLocation();
	// 	SpawnGrid();
	// 	total = 0;
	// }
		
}

void AGrid::SpawnGridShape(EGridShape Shape)
{
	GridShape = Shape;
	SpawnGrid();
}

void AGrid::SpawnGridTileCount(FVector2D tileCount)
{
	GridTileCount = FIntPoint(tileCount.X,tileCount.Y);
	SpawnGrid();
}

void AGrid::SpawnGridLocation(FVector location)
{
	GridCenterLocation = location;
	SetActorLocation(location);
	SpawnGrid();
}

void AGrid::SpawnGridSize(FVector size)
{
	GridTileSize = size;
	SpawnGrid();
}

void AGrid::SpawnGrid()
{
	GridVisual = Cast<AMyGridVisual>(ChildActor_GridVisual->GetChildActor());	
	DestroyGrid();
	if (GridShape == EGridShape::None)
	{
		return;
	}
	
	GridVisual->InitializedGridVisual(this);
	CurGridTileSize = GridTileSize;
	switch (GridShape)
	{
	case EGridShape::Triangle:
		CurGridTileSize.X *= 2.0f;
		CurGridTileSize.Y *= 1.0f;
		CurGridTileSize.Z *= 1.0f;
		break;
	case EGridShape::Hexagon:
		CurGridTileSize.X *= 1.5f;
		CurGridTileSize.Y *= 1.0f;
		CurGridTileSize.Z *= 1.0f;
		break;
	case EGridShape::Square:
		break;
	}

	GridTileCount.X = FMathf::Round(GridTileCount.X);
	GridTileCount.Y = FMathf::Round(GridTileCount.Y);
	
	FVector center = SnapVectorToVector(GridCenterLocation,CurGridTileSize);
	GridCenterLocation = center;
	if(GridShape == EGridShape::Triangle)
	{
		FVector v1((GridTileCount.X - 1)/2.0f,(GridTileCount.X - 1)/4.0f,0.0f);
		v1 *= GridTileSize;
		GridBottomLeftCornerLocation = center - SnapVectorToVector(v1,CurGridTileSize);
	}
	else if(GridShape == EGridShape::Hexagon)
	{
		FVector v1(GridTileCount.X/3.0f,GridTileCount.Y/2.0f,0);
		v1 *= GridTileSize;
		GridBottomLeftCornerLocation = center - SnapVectorToVector(v1,CurGridTileSize);
	}
	else
	{
		FIntPoint tmp;
		tmp.X = GridTileCount.X -  (IsIntEven(GridTileCount.X) ? 0 : 1);
		tmp.Y = GridTileCount.Y -  (IsIntEven(GridTileCount.Y) ? 0 : 1);

		tmp /= 2;
		FVector v1(tmp.X,tmp.Y,0);
		v1 *= GridTileSize;
		GridBottomLeftCornerLocation = center - v1;
	}
	
	
	int lastXIndex = GridTileCount.X; 
	int lastYIndex = GridTileCount.Y;
	FGridShapeData* curGridShape = GetShapeData(GridShape);
	FTransform TileTransform;
	FIntPoint index;
	for(int x = 0;x < lastXIndex;x++)
	{
		int startIndex = 0;
		int endIndex = 0;
		int addIndex = 1;
		if(GridShape != EGridShape::Hexagon)
		{
			startIndex = 0;
			endIndex = lastYIndex;
			addIndex = 1;
		}
		else
		{
			startIndex = IsIntEven(x) ? 0 : 1;
			endIndex = lastYIndex * 2;
			addIndex = 2;
		}

		for(int y = startIndex ;y < endIndex;y += addIndex)
		{
			index.X = x;
			index.Y = y;

			FVector loc = GetTileLocationFromGridIndex(index);
			FQuat rot = GetTileRotationFromGridIndex(index);
			FVector scal = GridTileSize / curGridShape->MeshSize;
			TileTransform.SetLocation(loc);
			TileTransform.SetRotation(rot);
			TileTransform.SetScale3D(scal);
			FTileData data;
			data.Index = index;
			data.TileType = ETileType::Normal;
			data.Transform = TileTransform;
			if(UseEnvironment)
			{
				data.TileType = TraceForGround(TileTransform);
				data.Transform = TileTransform;
			}
			UE_LOG(LogTemp,Log,TEXT("grid type = %d location = %s"),data.TileType,*data.Transform.GetLocation().ToString());
			AddGridTile(data);
		}
	}

	
}


FVector AGrid::GetTileLocationFromGridIndex(FIntPoint GridIndex)
{
	FVector2d v1(GridIndex);
	FVector2d v2(0,0);
	switch (GridShape)
	{
		case EGridShape::Triangle:
			v2.X = 1.0f;
			v2.Y = 0.5f;
			break;
		case EGridShape::Square:
			v2.X = 1;
			v2.Y = 1;
			break;
		case EGridShape::Hexagon:
			v2.X = 0.75f;
			v2.Y = 0.5f;
			break;
		default:
			break;
	}
	v1 *= v2;
	return GridBottomLeftCornerLocation +  GridTileSize * FVector(v1.X,v1.Y,0);
}

FQuat AGrid::GetTileRotationFromGridIndex(FIntPoint GridIndex)
{
	if(GridShape == EGridShape::Triangle)
	{
		float angle = 0;
		if(IsFloatEven(GridIndex.X))
		{
			angle += 180;
		}
		if(IsFloatEven(GridIndex.Y))
		{
			angle += 180;
		}
		return FQuat(FRotator(0,angle,0));
		
	}
	
	return FQuat(FRotator(0,0,0));
}

ETileType AGrid::TraceForGround(FTransform& RTransform)
{
	float Radius = GridShape == EGridShape::Triangle ?  GridTileSize.X /5.0f : GridTileSize.X /4.0f;
	
	FVector start = RTransform.GetLocation() + FVector(0,0,100000.0f);
	FVector end = RTransform.GetLocation() - FVector(0,0,100000.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);//忽略自身
	
	TArray<FHitResult> HitResults;
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		start,
		end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius),
		QueryParams);
	
	if(HitResults.IsEmpty())
	{
		return ETileType::None;
	}

	bool IsHeightFound = false;
	ETileType retType = ETileType::Normal;
	float retZ = 0;
	for(auto& one : HitResults)
	{
		AGridModifier* target = Cast<AGridModifier>(one.GetActor());
		if(target != nullptr)
		{
			retType = target->GetTileType();
			if(target->GetUseForTileHeight())
			{
					IsHeightFound = true;
			}
			retZ = SnapFloatToFloat(one.Location.Z - Radius,GridTileSize.Z);
		}
		else
		{
			if(!IsHeightFound)
			{
				retZ = SnapFloatToFloat(one.Location.Z - Radius,GridTileSize.Z);
			}
		}
	}

	FVector location = RTransform.GetLocation();
	location.Z = retZ;
	RTransform.SetLocation(location);
	
	for(FHitResult& one : HitResults)
	{
		DrawDebugSphere(
			GetWorld(),
			one.Location,     // 球心位置
			Radius,     // 半径
			8,         // 分段数（球面的平滑度）
			FColor::Green, // 颜色
			false,      // 是否保留
			5.0f,       // 存在时间
			0,          // 深度优先级
			1.0f        // 线宽
		);
	}
	return retType;
}


FGridShapeData* AGrid::GetGridShape()
{
	return GetShapeData(GridShape);
}

void AGrid::AddGridTile(FTileData data)
{
	GridTiles.Add(data.Index,data);
	GridVisual->AddTileVisual(data);
}


void AGrid::DestroyGrid()
{
	GridTiles.Empty();
	GridVisual->DestroyGridVisual();
}

bool ConvertMouseLocationToWorldSpace(APlayerController* Controller, FVector& OutWorldLocation, FVector& OutWorldDirection)
{
	if (Controller && Controller->GetLocalPlayer())
	{
		float MouseX, MouseY;
		if (Controller->GetMousePosition(MouseX, MouseY))
		{
			return Controller->DeprojectScreenPositionToWorld(MouseX, MouseY, OutWorldLocation, OutWorldDirection);
		}
	}
	return false;
}

bool AreVectorsParallel(const FVector& VectorA, const FVector& VectorB)
{
	if (VectorA.IsZero() || VectorB.IsZero())
		return false; // 零向量与任何向量都不平行

	float DotProduct = FVector::DotProduct(VectorA, VectorB);
	float MagnitudeProduct = VectorA.Size() * VectorB.Size();

	// 由于浮点数运算的精度问题，我们不能直接比较DotProduct和MagnitudeProduct
	// 我们检查它们的差的绝对值是否小于一个很小的数（epsilon），例如1e-6
	return FMath::Abs(DotProduct - MagnitudeProduct) < 1e-6 || FMath::Abs(DotProduct + MagnitudeProduct) < 1e-6;
}

FVector AGrid::GetCursorLocationOnGrid(TObjectPtr<APlayerController> playerIndex,bool traceForGround,bool traceForEmptySpace)
{
	FHitResult HitResult;
	
	bool result = playerIndex->GetHitResultUnderCursorByChannel(TraceTypeQuery1,true,HitResult);
	if(result)
	{
		return HitResult.Location;
	}

	if(traceForGround)
	{
		result = playerIndex->GetHitResultUnderCursorByChannel(TraceTypeQuery2,false,HitResult);
		if(result)
		{
			return HitResult.Location;
		}
	}

	if(traceForEmptySpace)
	{
		FVector worldLocation,WorldDirection;
		ConvertMouseLocationToWorldSpace(playerIndex,worldLocation,WorldDirection);
		
		FVector endLine = WorldDirection*100000000.0f + worldLocation;
		FVector LineDir = endLine - worldLocation;
		FVector PlaneNormal(0,0,1);
		//计算投影 垂直是0
		// double Denominator = LineDir | PlaneNormal;
		
		if (AreVectorsParallel(LineDir,PlaneNormal))  // 检测分母是否非常接近于零
		{//不会相交
			return FVector(-999,-999,-999);
		}
		
		return FMath::LinePlaneIntersection<double>(worldLocation,endLine,GridCenterLocation,PlaneNormal);	
	}
	
	return FVector(-999,-999,-999);
		
}

FIntPoint AGrid::GetTileIndexUnderCursor(TObjectPtr<APlayerController> playerIndex, bool traceForGround,bool traceForEmptySpace)
{
	FVector location = GetCursorLocationOnGrid(playerIndex,traceForGround,traceForEmptySpace);
	return GetTileIndexFromWorldLocation(location);
}

FIntPoint AGrid::GetTileIndexFromWorldLocation(FVector location)
{
	FVector LocationOnGrid = location - GridBottomLeftCornerLocation;

	FVector InvalidLocation(-999,-999,-999);
	if(LocationOnGrid.Equals(InvalidLocation))
		return FIntPoint(InvalidLocation.X,InvalidLocation.Y);
	
	FIntPoint snapLocationOnGrid;
	FVector tmp;
	switch (GridShape)
	{
	case EGridShape::None:
		return FIntPoint(-999,-999);
	case EGridShape::Square:
		tmp = SnapVectorToVector(LocationOnGrid,CurGridTileSize);
		snapLocationOnGrid.X = tmp.X;
		snapLocationOnGrid.Y = tmp.Y;
		snapLocationOnGrid.X /= FMathf::Floor(GridTileSize.X);
		snapLocationOnGrid.Y /= FMathf::Floor(GridTileSize.Y);
		return snapLocationOnGrid;
	case EGridShape::Triangle:
		tmp = SnapVectorToVector(LocationOnGrid,GridTileSize*FVector(1.0f,2.0f,1.0f));
		snapLocationOnGrid.X = tmp.X;
		snapLocationOnGrid.Y = tmp.Y;
		snapLocationOnGrid.X /= FMathf::Floor(GridTileSize.X);
		snapLocationOnGrid.Y /= FMathf::Floor(GridTileSize.Y);
		snapLocationOnGrid.Y *= 2.0f;
		return snapLocationOnGrid;
	case EGridShape::Hexagon:
		LocationOnGrid *= FVector(1.0f,2.0f,1.0f);
		tmp = SnapVectorToVector(LocationOnGrid,GridTileSize*FVector(0.75f,0.25f,1.0f));
		snapLocationOnGrid.X = tmp.X;
		snapLocationOnGrid.Y = tmp.Y;
		FIntPoint TempIndex;
		TempIndex.X = snapLocationOnGrid.X / (GridTileSize.X*0.75f);
		TempIndex.Y = snapLocationOnGrid.Y / GridTileSize.Y;
		if(IsFloatEven(TempIndex.X))
		{
			TempIndex.X = static_cast<int>(TempIndex.X);
			TempIndex.Y = static_cast<int>(FMathf::Round(TempIndex.Y/2.0f)*2.0f);
			return TempIndex;
		}
		
		TempIndex.X = static_cast<int>(FMathf::Floor(TempIndex.X));	
		TempIndex.Y = static_cast<int>(FMathf::Floor(TempIndex.Y / 2.0f))*2.0f+1.0f;
		return TempIndex;
	}
	return FIntPoint(-999,-999);
}

void AGrid::SetUseEnvironment(bool bis)
{
	UseEnvironment = bis;
	SpawnGrid();
}

void AGrid::SetOffsetFromGround(float offset)
{
	GridVisual->SetOffsetFromGround(offset);
	SpawnGrid();
}

void AGrid::AddStateToTile(FIntPoint index, ETileState stat)
{
	auto result = GridTiles.Find(index);
	if( result != nullptr)
	{
		if(result->States.AddUnique(stat) >= 0)
		{
			// UE_LOG(LogTemp,Log,TEXT("result->States.AddUnique(stat) >= 0"))
			GridVisual->UpdateTileVisual(*result,EGriUpdateMode::UpdateState);
		}
	}
}

void AGrid::RemoveStateFromTile(FIntPoint index, ETileState stat)
{
	auto result = GridTiles.Find(index);
	if( result != nullptr)
	{
		if(result->States.Remove(stat) > 0)
		{
			GridVisual->UpdateTileVisual(*result,EGriUpdateMode::UpdateState);
		}
	}
}

void AGrid::AddNewOneTIle(FIntPoint index)
{
	auto result = GridTiles.Find(index);
	if(result != nullptr)return;

	if (GridShape == EGridShape::None)
	{
		return;
	}
	
	GridVisual = Cast<AMyGridVisual>(ChildActor_GridVisual->GetChildActor());	
	
	CurGridTileSize = GridTileSize;
	switch (GridShape)
	{
	case EGridShape::Triangle:
		CurGridTileSize.X *= 2.0f;
		CurGridTileSize.Y *= 1.0f;
		CurGridTileSize.Z *= 1.0f;
		break;
	case EGridShape::Hexagon:
		CurGridTileSize.X *= 1.5f;
		CurGridTileSize.Y *= 1.0f;
		CurGridTileSize.Z *= 1.0f;
		break;
	case EGridShape::Square:
		break;
	}

	GridTileCount.X = FMathf::Round(GridTileCount.X);
	GridTileCount.Y = FMathf::Round(GridTileCount.Y);
	
	
	FGridShapeData* curGridShape = GetShapeData(GridShape);
	FTransform TileTransform;
	
	FVector loc = GetTileLocationFromGridIndex(index);
	FQuat rot = GetTileRotationFromGridIndex(index);
	FVector scal = GridTileSize / curGridShape->MeshSize;
	TileTransform.SetLocation(loc);
	TileTransform.SetRotation(rot);
	TileTransform.SetScale3D(scal);
	FTileData data;
	data.Index = index;
	data.TileType = ETileType::Normal;
	data.Transform = TileTransform;
	if(UseEnvironment)
	{
		data.TileType = TraceForGround(TileTransform);
		data.Transform = TileTransform;
	}
	UE_LOG(LogTemp,Log,TEXT("grid type = %d location = %s"),data.TileType,*data.Transform.GetLocation().ToString());
	AddGridTile(data);
}

void AGrid::RemoveOneTIle(FIntPoint index)
{
	auto result = GridTiles.Find(index);
	if(result == nullptr)return;
	GridTiles.Remove(index);
	GridVisual->RemoveTIle(index);
}

void AGrid::SetTileTypeByIndex(FIntPoint index, ETileType tileType)
{
	auto result = GridTiles.Find(index);
	if(result != nullptr)return;

	if(tileType == ETileType::None)
	{
		RemoveOneTIle(index);
	}
	else
	{
		result->TileType = tileType;
		GridVisual->UpdateTileVisual(*result,EGriUpdateMode::UpdateTileType);	
	}
}


bool AGrid::IsValidGridIndex(const FIntPoint& index)
{
	return GridTiles.Contains(index);
}

bool AGrid::IsTileWalkable(const FIntPoint& index)
{
	if(!IsValidGridIndex(index))return false;

	auto tile = GridTiles.Find(index);
	return IsTileTypeWalkable(tile->TileType);
}


void AGrid::IncreaseDecreaseTileHeight(const FIntPoint& index,bool increase)
{
	FTileData* data = GridTiles.Find(index);
	if(data == nullptr)
	{
		return;
	}
	auto curLocation = data->Transform.GetLocation();
	curLocation.Z += GridTileSize.Z * (increase? 1.0f : -1.0f);
	
	data->Transform.SetLocation(curLocation);
	GridVisual->UpdateTileVisual(*data,EGriUpdateMode::UpdateTransform);
}

const FTileData* AGrid::GetTileDataByIndex(const FIntPoint& index)
{
	return GridTiles.Find(index);
}

