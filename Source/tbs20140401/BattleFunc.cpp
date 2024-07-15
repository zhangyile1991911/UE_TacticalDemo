#include "BattleFunc.h"

#include "MyUnit.h"
#include "Grid.h"
#include "UnitAbilityAnim.h"

// bool UBattleFunc::IsWrapAttack(AMyUnit* Attacker,AMyUnit* Defender,AMyUnit* Cooperator)
// {//todo
// 	
// 	//包夹其实不在乎 参与包夹的角色朝向
// 	FVector AttackerForward = Attacker->GetActorForwardVector();
// 	FVector DefenderForward = Defender->GetActorForwardVector();
// 	//夹击协助者 必须站在 防御者的 上下左右四个相邻格子上
// 	FIntPoint DefenderIndex = Defender->GetGridIndex();
// 	FIntPoint Up = DefenderIndex.X + 1;
// 	FIntPoint Down = DefenderIndex.X - 1;
// 	FIntPoint Left = DefenderIndex.Y + 1;
// 	FIntPoint Right = DefenderIndex.Y - 1;
//
// 	if(Up == Cooperator->GetGridIndex() || Down == Cooperator->GetGridIndex()
// 		|| Left == Cooperator->GetGridIndex() || Right == Cooperator->GetGridIndex())
// 	{
// 		
// 	}
// 	
// 	return false;
// }

AMyUnit* UBattleFunc::HasWrapAttackUnit(AMyUnit* Attacker, AMyUnit* Defender,AGrid* Grid)
{
	FIntPoint Center = Defender->GetGridIndex();
	//まず　周囲の取る
	TArray<FIntPoint> Around;
	Around.Reserve(4);
	Around.Add(FIntPoint(Center.X+1,Center.Y));
	Around.Add(FIntPoint(Center.X-1,Center.Y));
	Around.Add(FIntPoint(Center.X,Center.Y+1));
	Around.Add(FIntPoint(Center.X,Center.Y-1));

	Around.RemoveAll([Attacker,Grid](const FIntPoint& one)->bool
	{
		if(!Grid->IsValidGridIndex(one))return true;
		TObjectPtr<AMyUnit> Unit = Grid->GetUnitOnTile(one);
		if(Unit == nullptr)return true;
		if(Attacker->GetUniqueID() == Unit->GetUniqueID())return true;
		// if(Unit->GetRuntimeProperty().UnitSide != Attacker->GetRuntimeProperty().UnitSide)
		// 	return true;
		if(!Unit->IsFriend(Attacker->GetUnitSide()))return true;
		return false;
	});

	FVector AttackerLocation = Attacker->NeedToMove() ? Attacker->GetShadowUnitLocation() : Attacker->GetActorLocation();
	FVector DirA = AttackerLocation - Defender->GetActorLocation();
	DirA = DirA.GetSafeNormal();

	for(auto one : Around)
	{
		// if(one.X ==Attacker->GetGridIndex().X || one.Y == Attacker->GetGridIndex().Y)
		// 	return Grid->GetUnitOnTile(one);
		auto Cooperator = Grid->GetUnitOnTile(one);
		FVector DirB = Cooperator->GetActorLocation() - Defender->GetActorLocation();
		DirB = DirB.GetSafeNormal();

		// 计算点积
		float DotProduct = FVector::DotProduct(DirA, DirB);
		// 计算夹角的弧度
		float AngleRadians = FMath::Acos(DotProduct);
		// 将弧度转换为度
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
		UE_LOG(LogTemp,Log,TEXT("DirA %s DirB %s"),*DirA.ToString(),*DirB.ToString())
		UE_LOG(LogTemp,Log,TEXT("HasWrapAttackUnit AngleDegrees = %f"),AngleDegrees)
		if(AngleDegrees > 90)
		{//计算高度差
			const auto CooperatorAbility = Cooperator->GetCooperationAbilityAnim();
			const FIntPoint DefenderIndex = Defender->GetGridIndex();
			const FTileData* DefenderTileData = Grid->GetTileDataByIndex(DefenderIndex);
			const bool bIsHeight = CooperatorAbility->CheckTileDataHeight(DefenderTileData,Grid);
			if(bIsHeight)
				return Cooperator;
		}
	}
	
	return nullptr;
}

bool UBattleFunc::IsCritical(AMyUnit* Attacker,AMyUnit* Defender)
{
	int Lucky = 8 + Attacker->GetRuntimeProperty().Lucky - Defender->GetRuntimeProperty().Lucky;
	Lucky = FMathf::Clamp(Lucky,0,100);
	Lucky *= 100;
	const int Num = FMath::RandRange(0,10000);
	return Num <= Lucky;
}

// AMyUnit* UBattleFunc::GetWrapAttackUnit(AMyUnit* Attacker, AMyUnit* Defender,AGrid* Grid)
// {
// 	return nullptr;
// }

bool UBattleFunc::IsBackAttack(AMyUnit* Attacker,AMyUnit* Defender,AGrid* MyGrid,int Deviation)
{
	if(Attacker == Defender)return false;
	// FRotator AttackRotation;
	// if(Attacker->NeedToMove())
	// {
	// 	AttackRotation = Attacker->GetShadowUnitRotation();
	// }
	// else
	// {
	// 	AttackRotation = Attacker->GetUnitForward();
	// }
	// FRotator DefenderRotation = Defender->GetUnitForward();
	// //只能说明是相同方向
	// float DeltaYaw = FMathf::Abs(AttackRotation.Yaw - DefenderRotation.Yaw);
	// bool bIsSameDir = DeltaYaw <= 0.01f;
	// if(!bIsSameDir)return false;
	FIntPoint BackAtkPoint = Defender->GetGridIndex();
	EUnitDirectType Direct = Defender->GetUnitDirect();
	switch (Direct)
	{
	case EUnitDirectType::LEFT:
		BackAtkPoint.Y += 1;
		break;
	case EUnitDirectType::RIGHT:
		BackAtkPoint.Y -= 1;
		break;
	case EUnitDirectType::FORWARD:
		BackAtkPoint.X -= 1;
		break;
	case EUnitDirectType::BACKWARD:
		BackAtkPoint.X += 1;
		break;
	}
	const FIntPoint& AttackPoint = Attacker->GetStandGridIndex();
	const FTileData* BackAtkTileData = MyGrid->GetTileDataByIndex(BackAtkPoint);
	const FTileData* DefendTileData = MyGrid->GetTileDataByIndex(Defender->GetGridIndex());
	if(BackAtkTileData == nullptr || DefendTileData == nullptr)return false;
	const int Delta = FMathf::Abs(BackAtkTileData->Height - DefendTileData->Height);
	if(Delta > Deviation)return false;
	
	return AttackPoint == BackAtkPoint;
}

/*
 *
* 命中率=90 ± 高低格h*2 + 夹击系数10（参与夹击的单位）+ 背击系数20 +2*（角色命中-角色闪避）
* 90+（6-8）*2=86%
* 敌人 8
* 主角 6
 */
int UBattleFunc::CalculateHitRate(AMyUnit* Attacker,AMyUnit* Defender,AGrid* MyGrid,bool HasWrap,bool IsBackAtk)
{
	FIntPoint AttackerGrid = Attacker->GetGridIndex();
	FIntPoint DefenderGrid = Defender->GetGridIndex();

	
	int HighDisparity = MyGrid->CalcHighDisparity(AttackerGrid,DefenderGrid);

	int Wrap = HasWrap ? 10 : 0;

	int BackAtk = IsBackAtk ? 20 : 0;

	
	int HitResult = 90 + HighDisparity + Wrap + BackAtk
	+ (Attacker->GetRuntimeProperty().HitProb - Defender->GetRuntimeProperty().HitProb) * 2;
	
	return HitResult;
}

int UBattleFunc::CalculateDamage(AMyUnit* Attacker, AMyUnit* Defender, AGrid* MyGrid,int BaseDamage,bool bIsCritical,bool bIsBackAtk)
{
	const FIntPoint AttackerIndex = Attacker->GetGridIndex();
	const FIntPoint DefenderIndex = Defender->GetGridIndex();

	const int AttackerHeight = MyGrid->GetTileDataByIndex(AttackerIndex)->Height;
	const int DefenderHeight = MyGrid->GetTileDataByIndex(DefenderIndex)->Height;
	BaseDamage = bIsCritical ? BaseDamage + 5 : BaseDamage;
	BaseDamage = BaseDamage + (bIsBackAtk ? 5 : 0);
	const int Damage = BaseDamage + 2 * (AttackerHeight - DefenderHeight);
	return Damage;
}


