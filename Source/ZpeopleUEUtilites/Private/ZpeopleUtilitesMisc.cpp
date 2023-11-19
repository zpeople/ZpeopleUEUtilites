// Fill out your copyright notice in the Description page of Project Settings.

#include "ZpeopleUtilitesMisc.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"
#include "Kismet/KismetSystemLibrary.h"
DEFINE_LOG_CATEGORY_STATIC(LogZpeopleUtilitesMisc, Warning, All);
void UZpeopleUtilitesMisc::RestartGameClient()
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "You Can`t use  \"RestartGameClient\" node with editor mode !");
	//return;
#endif
	FString RootPath = FPaths::RootDir();
	RootPath=RootPath.Replace(*FString("/"), *FString("\\"));
	FString GameExeFullPath=RootPath+ UKismetSystemLibrary::GetGameName() + ".exe";
	UE_LOG(LogZpeopleUtilitesMisc, Warning, TEXT("%s"), *GameExeFullPath);
	FString Command=FString::Printf(TEXT("ping 127.0.0.1 -n 2 > nul\r\n@echo off\r\nstart %s"),*GameExeFullPath);
	FFileHelper::SaveStringToFile(Command, *(FPaths::ProjectSavedDir() / "RestartClient.bat"));
	FPlatformProcess::ExecProcess(*(FPaths::ProjectSavedDir() / "RestartClient.bat"), nullptr, nullptr, nullptr, nullptr);
	GEngine->Exec(GWorld, *FString("Exit"));
}