// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"


//LoadImageAsync
#include "DynamicLoader.h"

#include "ZpeopleLoadSource.generated.h"

/**
 * 
 */
UCLASS()
class ZPEOPLEUEUTILITES_API UZpeopleLoadSource : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	

	static FString GetFullPath(FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
	static	bool IsVaildPath(const FString FilePath); 
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
	static	FString GetFileName(const FString FilePath);
	//从本地加载贴图
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
	static	UTexture2D * LoadTexture2DFromFile(const FString & FilePath, bool & IsValid, int32 & Width, int32 & Height);
    //从本地加载贴图 转换为asset (editor)
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static	UTexture2D * LoadTexAndSaveAssetFromFile(const FString & FilePath, bool & IsValid, int32 & Width, int32 & Height,  FString TextureName);//with editor
    //从本地加载贴图 保存为bin文件 第二次加载bin文件 
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static	UTexture2D * LoadAndSaveBinTexFromFile(const FString & FilePath, const bool & isSave ,bool & IsValid, int32 & Width, int32 & Height);
	//load本地bin文件 贴图
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static	UTexture2D * LoadTexture2DFromBin(const FString & FilePath,  bool& IsValid, int32& Width, int32& Height);
	//异步加载本地贴图
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static UImageLoader * LoadTexture2DFromFile_Async(UObject* WorldContextObject,const FString & FilePath, const FString & ID);
	
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static 	bool Texture2d2PNG(UTextureRenderTarget2D * TextureRenderTarget, const FString & FilePath);
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static UTexture2D * BytesToTexture2d(const TArray<uint8> bytes);
	
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static USoundWave * LoadOggDataFromFile(const FString & FilePath);
	
	UFUNCTION(BlueprintCallable, Category = "Zpeople|Utilityes")
		static USoundWave * LoadWaveDataFromFile(const FString & FilePath);

	static bool ReadOggWaveData(USoundWave * sw, TArray<uint8>* rawFile);
};


