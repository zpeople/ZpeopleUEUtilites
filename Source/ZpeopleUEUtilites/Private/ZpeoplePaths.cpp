#include "ZpeoplePaths.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"

FString UZpeoplePaths::GetEngineDir()
{
	return FPaths::EngineDir();
	
}
FString UZpeoplePaths::GetGameSaveDir()
{
	return FPaths::ProjectSavedDir();
}
FString UZpeoplePaths::GetGameContentDir()
{
	return FPaths::ProjectContentDir();
}
FString UZpeoplePaths::GetGamePaksDir()
{
	return FPaths::ProjectContentDir() / "Paks/";
}
FString UZpeoplePaths::GetBaseScreenShotPath()
{
	return  FPaths::ConvertRelativePathToFull(FPaths::ScreenShotDir());
}