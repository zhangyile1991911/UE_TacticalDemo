// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDataHelper.h"

#include "GridInfoSave.h"
#include "JsonObjectConverter.h"

bool UGridDataHelper::SaveGridToJson(const FGridInfoSave& GridData, const FString& FilePath)
{
	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(GridData, OutputString);
	const bool bIsSuccess = FFileHelper::SaveStringToFile(OutputString, *FilePath);
	UE_LOG(LogTemp,Log,TEXT("%s File Save %d"),*FilePath,bIsSuccess)
	return bIsSuccess;
}

bool UGridDataHelper::LoadGridFromJson(FGridInfoSave& GridData, const FString& FilePath)
{
	FString JsonString;
	bool bIsExist = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	if(!bIsExist)
	{
		UE_LOG(LogTemp,Log,TEXT("%s don't exist"),*FilePath)
		return bIsExist;
	}
	FFileHelper::LoadFileToString(JsonString, *FilePath);
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &GridData, 0, 0);
	return true;
}


