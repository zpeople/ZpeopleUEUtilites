// Fill out your copyright notice in the Description page of Project Settings.

#include "ZpeopleScreenShootTool.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"
#include "Engine.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "IImageWrapperModule.h"

static  UScreenShootTool::FZpeopleShootRequestDelegate  ShootRequestDelegate;
void UScreenShootTool::SetShootRequestDelegate(FZpeopleShootRequestDelegate OnShootRequestDelegate)
{
	ShootRequestDelegate = OnShootRequestDelegate;
}

void UScreenShootTool::OnShootRequestDelegate(FString path, bool IsSuccess)
{
	ShootRequestDelegate.ExecuteIfBound(path,IsSuccess);
}

FString UScreenShootTool::TakeScreenShoot(FString picName,bool bUnique)
{
	FString inStr;
	FString Filename;
	FScreenshotRequest::RequestScreenshot(picName + ".png", false, bUnique);
	FString ResultStr=FScreenshotRequest::GetFilename();
	ResultStr = FPaths::GetBaseFilename(ResultStr, true);
	return ResultStr+".png";
}


FString fileName;
	EImageFormatType format;
	int32 quality;
FDelegateHandle delegateHandle;


void UScreenShootTool::TakeScreenShotAdvance(const FString &_fineName, const FString &_path,EImageFormatType _format, int32 _quality, bool bInsertTimeStampToFileName, bool bInShowUI, bool bAddFilenameSuffix)
{
	FString filenameTemp = _fineName;
	if (filenameTemp.IsEmpty())
	{
		filenameTemp = FString(TEXT("Screenshot"));
	}

	format = _format;
	quality = _quality;

	//名字后面加上现在的时间
	if (bInsertTimeStampToFileName)
	{

		filenameTemp += FDateTime::Now().ToString(TEXT("%Y_%m_%d_%H_%M_%S"));
	}
	//名字后面加上下划线
	if (bAddFilenameSuffix)
	{
		filenameTemp += "_";
	}
	if (_path.IsEmpty())
	{

		fileName = FPaths::ProjectSavedDir() + "ScreenShots/Windows/" + filenameTemp;
	}
	else {
		fileName = _path + filenameTemp;
	}

	if (!UGameViewportClient::OnScreenshotCaptured().IsBound())
	{
		delegateHandle = UGameViewportClient::OnScreenshotCaptured().AddStatic(&OnScreenshotCapturedInternal);
	
		FScreenshotRequest::RequestScreenshot(fileName, bInShowUI, bAddFilenameSuffix);
		
		//加这个是高分辨率截图但是会有锯齿
		//GEngine->GameViewport->Exec(nullptr, TEXT("HighResShot 2560x1280"), *GLog);
	}

}
void UScreenShootTool::OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper;
	if (format == EImageFormatType::PNG)
	{
		TArray<uint8> PNGData;
		TArray<FColor> BitmapCopy(Bitmap);
		FImageUtils::CompressImageArray(Width, Height, BitmapCopy, PNGData);

		fileName += ".png";
		FFileHelper::SaveArrayToFile(PNGData, *fileName);
	}
	else if (format == EImageFormatType::JPEG)
	{
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& JPEGData = ImageWrapper->GetCompressed(quality);
		fileName += ".jpg";
		FFileHelper::SaveArrayToFile(JPEGData, *fileName);
	}
	else if (format == EImageFormatType::EXR)
	{
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& Data = ImageWrapper->GetCompressed(quality);
		fileName += ".exr";
		FFileHelper::SaveArrayToFile(Data, *fileName);
	}
	else
	{
		//默认使用Jpg格式
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& JPEGData = ImageWrapper->GetCompressed(quality);
		fileName += ".jpg";
		FFileHelper::SaveArrayToFile(JPEGData, *fileName);
	}
	UGameViewportClient::OnScreenshotCaptured().Remove(delegateHandle);
	ImageWrapper.Reset();
	OnShootRequestDelegate(*fileName,true);
}