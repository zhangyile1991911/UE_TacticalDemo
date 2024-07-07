// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbilityAnim_Throw.h"
#include "MyUnit.h"
#include "Grid.h"
#include "BattleFunc.h"
#include "BattleReport.h"

AUnitAbilityAnim_Throw::AUnitAbilityAnim_Throw()
{
}

bool AUnitAbilityAnim_Throw::CanExecute()
{
	return OwnerInstance->HasEnoughAP(SkillData.SpendPoint);
}

TArray<FIntPoint> AUnitAbilityAnim_Throw::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> Result;
	Result.Reserve(4);
	Result.Add(FIntPoint(Int32Point.X + 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X - 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y + 1));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y - 1));
	return MoveTemp(Result);
}

bool AUnitAbilityAnim_Throw::IsValidTarget(const FTileData* TileData, AGrid* MyGrid)
{
	if(TileData == nullptr)return false;
	if(TileData->UnitOnTile == nullptr)return false;
	if(TileData->UnitOnTile->IsDead())return false;
	if(TileData->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return false;
	
	const FIntPoint& StandIndex = OwnerInstance->GetStandGridIndex();
	const FTileData* StandTileData = MyGrid->GetTileDataByIndex(StandIndex);
	const bool bIsDeviation = CheckDeviation(TileData->Height,StandTileData->Height);
	return bIsDeviation;
}

bool AUnitAbilityAnim_Throw::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(Unit->IsFriend(OwnerInstance->GetUnitSide()))return false;
	
	return true;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbilityAnim_Throw::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	const FTileData* TileDataPtr = MyGrid->GetTileDataByIndex(Point);
	do
	{
		if(TileDataPtr == nullptr)break;
		if(TileDataPtr->UnitOnTile == nullptr)break;
		if(TileDataPtr->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))break;
		if(!CheckTileDataHeight(TileDataPtr,MyGrid))break;
		Targets.Add(TileDataPtr->UnitOnTile);
	}
	while (false);

	return MoveTemp(Targets);
}

FBattleReport AUnitAbilityAnim_Throw::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,
	bool NeedCooperator)
{
	return DoCalculation(Targets[0],MyGrid,NeedCooperator);
}

FBattleReport AUnitAbilityAnim_Throw::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	// TObjectPtr<AMyUnit> Cooperator = NeedCooperator ? UBattleFunc::HasWrapAttackUnit(OwnerInstance,Target,MyGrid) : nullptr;
	
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = nullptr;
	Report.CooperatorTarget = nullptr;
	Report.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,Target,MyGrid,SkillData.AllowableDeviation);
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,false,Report.IsBackAtk);

	const int Num = FMath::RandRange(0,100);
	if(Num > Report.HitPercent)
	{//未命中
		Report.Damage = 0;
		Report.IsHit = false;
		return MoveTemp(Report);	
	}

	Report.IsHit = true;
	Report.Damage = 10;
	//扣除血量
	Target->AddHP(Report.Damage);

	//把人往后扔
	FIntPoint StandIndex = OwnerInstance->GetGridIndex();
	
	switch (OwnerInstance->GetUnitDirect())
	{
	case EUnitDirectType::LEFT:
		StandIndex.Y += 1;
		break;
	case EUnitDirectType::RIGHT:
		StandIndex.Y -= 1;
		break;
	case EUnitDirectType::FORWARD:
		StandIndex.X -= 1;
		break;
	case EUnitDirectType::BACKWARD:
		StandIndex.X += 1;
		break;
	case EUnitDirectType::INVALID:
		break;
	}
	
	
	
	const FTileData* TileDataPtr = MyGrid->GetTileDataByIndex(StandIndex);
	
	//1 头顶位置
	Report.FirstIndex = OwnerInstance->GetActorLocation();
	Report.FirstIndex.Z += 250.0f;
	
	do
	{
		if(TileDataPtr == nullptr)
		{//如果是无效格子
			UE_LOG(LogTemp,Log,TEXT("Attack Index = %s Defender Origin Index = %s FinishIndex %s"),
				*OwnerInstance->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString())
			Report.SecondIndex = Target->GetActorLocation();
			break;
		}
		if(TileDataPtr->UnitOnTile != nullptr)
		{//如果已经有别的单位
			UE_LOG(LogTemp,Log,TEXT("Attack Index = %s Defender Origin Index = %s FinishIndex %s"),
				*OwnerInstance->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString())
			Report.SecondIndex = Target->GetActorLocation();
			break;
		}
		if(!CheckTileDataHeight(TileDataPtr,MyGrid))
		{//如果高度差超过了 还是放回原地
			UE_LOG(LogTemp,Log,TEXT("Attack Index = %s Defender Origin Index = %s FinishIndex %s"),
				*OwnerInstance->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString())
			Report.SecondIndex = Target->GetActorLocation();
			break;
		}
		//2 投掷位置
		Report.SecondIndex = TileDataPtr->Transform.GetLocation();
		//更新目标位置
		MyGrid->AddTileDataUnitByIndex(Target->GetGridIndex(),nullptr);
		MyGrid->AddTileDataUnitByIndex(StandIndex,Target);
		Target->SetGridIndex(StandIndex);
		UE_LOG(LogTemp,Log,TEXT("Attack Index = %s Defender Origin Index = %s FinishIndex %s"),
				*OwnerInstance->GetGridIndex().ToString(),
				*Target->GetGridIndex().ToString(),
				*StandIndex.ToString())
	}
	while (false);
	
	
	return MoveTemp(Report);
}


