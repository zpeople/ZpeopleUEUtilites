

// Fill out your copyright notice in the Description page of Project Settings.

#include "ZpeopleLoadSource.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"


#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"
#include "AssetRegistryModule.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

UTexture2D* LoadedT2D;
FString UZpeopleLoadSource::GetFullPath(FString FilePath)
{
	//Check Relative Or absolute path
	FString FullFilePath;
	if (FilePath.StartsWith(".", ESearchCase::CaseSensitive))
	{

		FullFilePath = *FPaths::Combine(FPaths::ProjectContentDir(), FilePath);

		FullFilePath = *FPaths::ConvertRelativePathToFull(FullFilePath);

	}
	else
	{

		FullFilePath = FilePath;
	}

	return FullFilePath;
}

bool UZpeopleLoadSource::IsVaildPath(const FString FilePath)
{

	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *FilePath);
		return false;
	}

	// Load the compressed byte data from the file
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *FilePath);
		return false;
	}

	return true;
}

FString UZpeopleLoadSource::GetFileName(const FString FilePath)
{
	
	return FPaths::GetCleanFilename(FilePath);;
}

//Discern Texture Type

static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::GetModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));



	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}
	return nullptr;
}



UTexture2D* UZpeopleLoadSource::LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height){

	FString FullFilePath = GetFullPath(FilePath);

	if (!IsVaildPath(FullFilePath))
	{
		return NULL;
	}


	IsValid = false;

     LoadedT2D = NULL;


	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FullFilePath);

	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath, 0)) return NULL;

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
			TArray<uint8> UncompressedBGRA;
		//const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (!LoadedT2D) return NULL;

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			//FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
		

			LoadedT2D->UpdateResource();
			UncompressedBGRA.Empty();
			//delete[] TextureData;
			
		}
	}

	IsValid = true;
	return LoadedT2D;

}

UTexture2D * UZpeopleLoadSource::LoadTexAndSaveAssetFromFile(const FString & FilePath, bool & IsValid, int32 & TextureWidth, int32 & TextureHeight, FString TextureName)
{
	FString FullFilePath = GetFullPath(FilePath);

	if (!IsVaildPath(FullFilePath))
	{
		return NULL;
	}


	IsValid = false;

	 LoadedT2D = NULL;


	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FullFilePath);

	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath, 0)) return NULL;

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		//const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{

			TextureWidth = ImageWrapper->GetWidth();
			TextureHeight = ImageWrapper->GetHeight();
			/*if (TextureWidth % 2 != 0)
			{
				TextureWidth += 1;
			}
			if (TextureHeight % 2 != 0)
			{
				TextureHeight += 1;
			}*/
			LoadedT2D = UTexture2D::CreateTransient(TextureWidth, TextureHeight, PF_B8G8R8A8);

			if (!LoadedT2D) return NULL;

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			//FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();


	

			LoadedT2D->UpdateResource();

	
			UTexture2D* TextureToBeExpot = LoadedT2D;
		
			uint32* PixelPointer = (uint32*)TextureData;
			if (PixelPointer == nullptr)
			{
				return LoadedT2D;
			}

			uint8* colors = new uint8[TextureWidth * TextureHeight * 4];
			/*TArray<uint8>colors;
			colors.AddUninitialized(TextureWidth * TextureHeight * 4);*/

			for (int32 x = 0; x < TextureWidth * TextureHeight; x++)
			{
				uint32 EncodedPixel = *PixelPointer;
				//B
				colors[4 * x] = (EncodedPixel & 0x000000FF);

				//G
				colors[4 * x + 1] = (EncodedPixel & 0x0000FF00) >> 8;

				//R
				colors[4 * x + 2] = (EncodedPixel & 0x00FF0000) >> 16;

				//A
				colors[4 * x + 3] = (EncodedPixel & 0xFF000000) >> 24;

				PixelPointer++;
			}
	

			//----------------------------------------------------------//
		     
			// Create Package
			FString pathPackage = FString("/Game/MyTextures/") + TextureName;
			//FString absolutePathPackage = FPaths::ProjectContentDir() + "/MyTextures/";

			//FPackageName::RegisterMountPoint(*pathPackage, *absolutePathPackage);

			UPackage * Package = CreatePackage(nullptr, *pathPackage);

			// Create the Texture
			UObject* TexObj = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone);
			UTexture2D* TextureSave = Cast<UTexture2D>(TexObj);
			TextureSave->AddToRoot();
			TextureSave->PlatformData = new FTexturePlatformData();
			TextureSave->PlatformData->SizeX = TextureWidth;
			TextureSave->PlatformData->SizeY = TextureHeight;
			TextureSave->PlatformData->PixelFormat = PF_B8G8R8A8;



			// Passing the pixels information to the texture
			FTexture2DMipMap* Mip = new FTexture2DMipMap();
			TextureSave->PlatformData->Mips.Add(Mip);
			Mip->SizeX = TextureWidth;
			Mip->SizeY = TextureHeight;
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			uint8* newTextureData = (uint8 *)Mip->BulkData.Realloc(TextureHeight * TextureWidth * sizeof(uint8) * 4);
			FMemory::Memcpy(newTextureData, colors, sizeof(uint8) * TextureHeight * TextureWidth * 4);
			Mip->BulkData.Unlock();
#if WITH_EDITORONLY_DATA

			TextureSave->Source.Init(TextureWidth, TextureHeight, 1,1, ETextureSourceFormat:: TSF_BGRA8, colors);
#endif
			// Updating Texture & mark it as unsaved
		
			TextureSave->UpdateResource();
			Package->MarkPackageDirty();
			FAssetRegistryModule::AssetCreated(TextureSave);
			FString PackageFileName = FPackageName::LongPackageNameToFilename(pathPackage, FPackageName::GetAssetPackageExtension());
			bool bSaved = UPackage::SavePackage(Package, TextureSave, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
			delete[] newTextureData;
			delete[] colors;
	
	
		}
	}

	IsValid = true;
	return LoadedT2D;
}

UTexture2D * UZpeopleLoadSource::LoadTexAndSaveBinFromFile(const FString & FilePath, const bool & isSave,bool & IsValid, int32 & Width, int32 & Height)
{
	FString FullFilePath = GetFullPath(FilePath);

	if (!IsVaildPath(FullFilePath))
	{
		return NULL;
	}
	
	FString binPath = FullFilePath + ".bin";
	IsValid = false;

	


	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FullFilePath);

	TArray<uint8> RawFileData;
	if (IsVaildPath(binPath))//是否存在bin
	{
		TArray<uint8> TheBinaryArray;

		if (!FFileHelper::LoadFileToArray(TheBinaryArray, *binPath))
		{
			FString str = FString::Printf(TEXT("--- LoadFileToArray Fail"));
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, str);
			return nullptr;
		}
		if (TheBinaryArray.Num() <= 0)return nullptr;
		// Decompress File 
		//FArchiveLoadCompressedProxy Decompressor =
		//	FArchiveLoadCompressedProxy(TheBinaryArray,FName(),  ECompressionFlags::COMPRESS_ZLIB);

		////Decompression Error?
		//if (Decompressor.GetError())
		//{
		//	FString str = FString::Printf(TEXT("--- FArchiveLoadCompressedProxy>> ERROR : File Was Not Compressed"));
		//	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, str);
		//	return nullptr;
		//}
	
		////Decompress
		//FBufferArchive DecompressedBinaryArray;
		//Decompressor << DecompressedBinaryArray;

	
		FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true);
		FromBinary.Seek(0);
		FromBinary << RawFileData;
		//Clean up 
		FromBinary.FlushCache();
		FromBinary.Close();

		// Empty & Close Buffer 
		TheBinaryArray.Empty();
		/*Decompressor.FlushCache();
		DecompressedBinaryArray.Empty();
		DecompressedBinaryArray.Close();*/
	}
	else
	{
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath, 0)) return NULL;

	}

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))//图片压缩数据
	{
			TArray<uint8> UncompressedBGRA;
		//const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))//图片解压后的数据
		{

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();
		
		
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
			if (true)
			{
#pragma region  Memcpy
				if (!LoadedT2D) return NULL;
				void*  TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
				LoadedT2D->UpdateResource();
				delete[] TextureData;
#pragma endregion
			}
			else {
			
				TArray<uint8>  UncompressedData;
				TArray<FColor> colorArr;
				UncompressedData = UncompressedBGRA;
				for (int i = 0; i < Height * 4; i += 4)
				{
					for (int j = 0; j < Width * 4; j += 4)
					{
						uint8& Red = UncompressedData[i * Width + j];
						uint8& Green = UncompressedData[i * Width + j + 1];
						uint8& Blue = UncompressedData[i * Width + j + 2];
						uint8& Alpha = UncompressedData[i * Width + j + 3];
						FColor color = FColor(Red, Green, Blue, Alpha);
						colorArr.Emplace(color);
					}
				}
				uint8* MipData = static_cast<uint8*>(LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
				for (int32 y = 0; y < Height; y++)
				{
					uint8* DestPtr = &MipData[(Height - 1 - y) * Width * sizeof(FColor)];
					FColor* SrcPtr = const_cast<FColor*>(&colorArr[(Height - 1 - y) * Width]);
					for (int32 x = 0; x < Width; x++)
					{//RGBA的赋值顺序，如果想改成RGBA模式（PNG）,在赋值的时候们需要把B，R的顺序调整就可以了。
						*DestPtr++ = SrcPtr->R;
						*DestPtr++ = SrcPtr->G;
						*DestPtr++ = SrcPtr->B;
						*DestPtr++ = SrcPtr->A;

						SrcPtr++;
					}
				}
				LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
				LoadedT2D->UpdateResource();
				UncompressedData.Empty();
				colorArr.Empty();
				delete[] MipData;
			}


		
			

#pragma region SaveBin
			if (isSave &&!IsVaildPath(binPath))
			{
		
			FBufferArchive ToBinary;
			ToBinary << RawFileData;
			

			//TArray<uint8> CompressedData;
			//FArchiveSaveCompressedProxy Compressor =
			//	FArchiveSaveCompressedProxy(CompressedData,FName(TEXT("test")), ECompressionFlags::COMPRESS_GZIP);

			////Send entire binary array/archive to compressor
			//Compressor << ToBinary;

			////send archive serialized data to binary array
			//Compressor.Flush();
			
			//No Data
			if (ToBinary.Num() > 0) {

				// Binary to Hard Disk
				if (FFileHelper::SaveArrayToFile(ToBinary, *binPath))
				{
					UE_LOG(LogTemp, Warning, TEXT("Save Success: %s"), *binPath);
				}
			}

			// Free Binary Array
			//Compressor.FlushCache();
			//CompressedData.Empty();

			ToBinary.FlushCache();
			ToBinary.Empty();
			ToBinary.Close();
			}
#pragma endregion


		}
	}

	IsValid = true;
	return LoadedT2D;
}

UTexture2D * UZpeopleLoadSource::LoadTexture2DFromBin(const FString & FilePath, bool& IsValid, int32& Width, int32& Height)
{
	TArray<uint8> TheBinaryArray;
	IsValid = false;
    LoadedT2D = NULL;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FilePath))
	{
		FString str = FString::Printf(TEXT("--- LoadFileToArray Fail"));
		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, str);
		return nullptr;
	}
	if (TheBinaryArray.Num() <= 0)return nullptr;
	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true);
	TArray<uint8> RawFileData;
	FromBinary << RawFileData;
	//Clean up 
	FromBinary.FlushCache();
	FromBinary.Close();

	// Empty & Close Buffer 
	TheBinaryArray.Empty();

	IImageWrapperModule& ImageWrapperModule = FModuleManager::GetModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
			TArray<uint8> UncompressedBGRA;
		//const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (!LoadedT2D) return NULL;

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			//FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
			LoadedT2D->UpdateResource();

		
		
		}
	}

	IsValid = true;
	return LoadedT2D;

}

UImageLoader* UZpeopleLoadSource::LoadTexture2DFromFile_Async(UObject* WorldContextObject,const FString& FilePath, const FString& ID)
{
	if (!WorldContextObject->IsValidLowLevel())
		return nullptr;  
	UImageLoader* Loader = NewObject<UImageLoader>();
	Loader->LoadImageAsync(WorldContextObject,FilePath, ID);
	//Loader->AddToRoot();
	return Loader;
}


UTexture2D * UZpeopleLoadSource::LoadTexAndSaveThumbnailFromFile(const FString & FilePath, const bool & isRid, bool & IsValid, int32 & Width, int32 & Height)
{
	FString FullFilePath = GetFullPath(FilePath);

	if (!IsVaildPath(FullFilePath))
	{
		return NULL;
	}


	IsValid = false;

    LoadedT2D = NULL;
	TSharedPtr<IImageWrapper> ImageWrapper;
	FString thumpPath = FilePath + "_thump.png";
	if (IsVaildPath(thumpPath)) {
		 ImageWrapper = GetImageWrapperByExtention(thumpPath);
	}
	else {
	      ImageWrapper = GetImageWrapperByExtention(FullFilePath);
	}

	TArray<uint8> RawFileData;
	if (IsVaildPath(thumpPath)) {
		if (!FFileHelper::LoadFileToArray(RawFileData, *thumpPath, 0)) return NULL;
	}
	else {
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath, 0)) return NULL;
	}

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
			TArray<uint8> UncompressedRBGA;
		//const TArray<uint8>* UncompressedRBGA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRBGA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);

			if (!LoadedT2D) return NULL;

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();
			if (IsVaildPath(thumpPath))
			{
				void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRBGA.GetData(), UncompressedRBGA.Num());
				//FMemory::Memcpy(TextureData, UncompressedRBGA->GetData(), UncompressedRBGA->Num());
				LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
				LoadedT2D->UpdateResource();

		
			
			}
			else
			{
				TArray<uint8>  UncompressedData;
				TArray<FColor> colorArr;
				UncompressedData = UncompressedRBGA;
				for (int i = 0; i < Height * 4; i += 4)
				{
					for (int j = 0; j < Width * 4; j += 4)
					{
						uint8& Red = UncompressedData[i * Width + j];
						uint8& Green = UncompressedData[i * Width + j + 1];
						uint8& Blue = UncompressedData[i * Width + j + 2];
						uint8& Alpha = UncompressedData[i * Width + j + 3];
						FColor color = FColor(Red, Green, Blue, Alpha);
						colorArr.Emplace(color);
					}
				}

				TArray<FColor> NewBitmap;
				int32 NewWidget = 0;
				int32 NewHeight = 0;

				if (ImageShrink(colorArr, Width, Height, 0.125f, NewBitmap, NewWidget, NewHeight, isRid)) {
					uint8* MipData = static_cast<uint8*>(LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
					for (int32 y = 0; y < Height; y++)
					{
						uint8* DestPtr = &MipData[(Height - 1 - y) * Width * sizeof(FColor)];
						FColor* SrcPtr = const_cast<FColor*>(&colorArr[(Height - 1 - y) * Width]);
						for (int32 x = 0; x < Width; x++)
						{
							*DestPtr++ = SrcPtr->R;
							*DestPtr++ = SrcPtr->G;
							*DestPtr++ = SrcPtr->B;
							*DestPtr++ = SrcPtr->A;

							SrcPtr++;
						}
					}
					LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
					LoadedT2D->UpdateResource();

					TArray<uint8> PNGData;
					FImageUtils::CompressImageArray(NewWidget, NewHeight, NewBitmap, PNGData);


					FFileHelper::SaveArrayToFile(PNGData, *thumpPath);


					delete[] MipData;
				
				}

			}
		
		}
	}

	IsValid = true;
	return LoadedT2D;
}

bool UZpeopleLoadSource::ImageShrink(const TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const float Ratio, TArray<FColor>& NewBitmap, int32 & NewWidth, int32 & NewHeight,bool isRid)
{
	if (Bitmap.Num() <= 0 || Width < 1 || Height < 1 || Ratio < 0.01f || Ratio > 0.99f)
		return false;
	if (NewBitmap.Num() > 0)
		NewBitmap.Empty();

	NewWidth = (int32)((float)Width * Ratio);
	NewHeight = (int32)((float)Height * Ratio);

	NewBitmap.SetNum(NewWidth * NewHeight);

	int32 nStep = Width / NewWidth;
	int32 nVStep = Height / NewHeight;
	int32 nRowStep = 0;
	for (int32 i = 0; i < NewHeight; i++)
	{
		for (int32 j = 0; j < NewWidth; j++)
		{
			if (isRid)
			{
				//是否需要切角
				if (IsNeedRid(j, i, NewWidth, NewHeight))
					NewBitmap[i * NewWidth + j] = FColor(0, 0, 0, 0);
				else
					NewBitmap[i * NewWidth + j] = Bitmap[nRowStep + j * nStep];
			}
			else {
				NewBitmap[i * NewWidth + j] = Bitmap[nRowStep + j * nStep];
			}
			
		}
		nRowStep += Width * nVStep;
	}
	return true;


}

bool UZpeopleLoadSource::IsNeedRid(const int32 w, const int32 h, const int32 Width, const int32 Height)
{
	if (h < 6 && w < 6)
	{
		if ((5 - w) * (5 - w) + (5 - h) * (5 - h) > 25)
			return true;
		return false;
	}
	else if (h < 6 && w + 6 > Width)
	{
		if ((w - Width + 5) * (w - Width + 5) + (5 - h) * (5 - h) > 25)
			return true;
		return false;
	}
	else if (h + 6 > Height && w < 6)
	{
		if ((5 - w) * (5 - w) + (h - Height + 5) * (h - Height + 5) > 25)
			return true;
		return false;
	}
	else if (h + 6 > Height && w + 6 > Width)
	{
		if ((w - Width + 5) * (w - Width + 5) + (h - Height + 5) * (h - Height + 5) > 25)
			return true;
		return false;
	}
	return false;

	
}

bool UZpeopleLoadSource::Texture2d2PNG(UTextureRenderTarget2D* TextureRenderTarget, const FString& FilePath)
{
	FTextureRenderTargetResource* rtResource = TextureRenderTarget->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);

	TArray<FColor> outBMP;

	for (FColor& color : outBMP)
	{
		color.A = 255;
	}
	outBMP.AddUninitialized(TextureRenderTarget->GetSurfaceWidth() * TextureRenderTarget->GetSurfaceHeight());
	rtResource->ReadPixels(outBMP, readPixelFlags);

	FIntPoint destSize(TextureRenderTarget->GetSurfaceWidth(), TextureRenderTarget->GetSurfaceHeight());
	TArray<uint8> CompressedBitmap;
	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap);
	bool imageSavedOk = FFileHelper::SaveArrayToFile(CompressedBitmap, *FilePath);

	return imageSavedOk;



}

UTexture2D* UZpeopleLoadSource::BytesToTexture2d(const TArray<uint8> bytes)
{

    LoadedT2D = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat Format = ImageWrapperModule.DetectImageFormat(bytes.GetData(), bytes.Num());
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);


	if (!ImageWrapper.IsValid())
	{
		return nullptr;
	}

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(bytes.GetData(), bytes.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		//const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (!LoadedT2D) return NULL;

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			//FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			LoadedT2D->UpdateResource();
		}
	}

	return LoadedT2D;

}


class USoundWave* UZpeopleLoadSource::LoadOggDataFromFile(const FString& FilePath)
{

	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return NULL;

	//* If true the song was successfully loaded
	bool loaded = false;

	FString FullPath = GetFullPath(FilePath);


	//* loaded song file (binary, encoded)
	TArray < uint8 > rawFile;

	loaded = FFileHelper::LoadFileToArray(rawFile, FullPath.GetCharArray().GetData());

	if (loaded)
	{
		FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(TEXT("OGG"));
		//sw->RawData = sw->CompressedFormatData.GetFormat(TEXT("OGG"));

		bulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(bulkData->Realloc(rawFile.Num()), rawFile.GetData(), rawFile.Num());
		bulkData->Unlock();

		sw->RawData = *bulkData;

		loaded = ReadOggWaveData(sw, &rawFile) == 0 ? true : false;
	}

	if (!loaded)
		return NULL;

	return sw;

}

bool UZpeopleLoadSource::ReadOggWaveData(class USoundWave* sw, TArray<uint8>* rawFile)
{
	//FSoundQualityInfo info;
	//FVorbisAudioInfo vorbisObj;

	//if (!vorbisObj.ReadCompressedInfo(rawFile->GetData(), rawFile->Num(), &info))
	//{
	//	//Debug("Can't load header");
	//	return true;
	//}

	//if (!sw) return true;
	//sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	//sw->NumChannels = info.NumChannels;
	//sw->Duration = info.Duration;
	//sw->RawPCMDataSize = info.SampleDataSize;
	//sw->SetSampleRate(info.SampleRate);
	//sw->RawData;

	return false;
}

class USoundWave* UZpeopleLoadSource::LoadWaveDataFromFile(const FString& FilePath)
{
	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return nullptr;

	FString FullPath = GetFullPath(FilePath);

	TArray < uint8 > rawFile;

	FFileHelper::LoadFileToArray(rawFile, FullPath.GetCharArray().GetData());
	FWaveModInfo WaveInfo;

	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
	{
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv)
		{
			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			sw->Duration = 0.0f;
		}

		sw->SetSampleRate(*WaveInfo.pSamplesPerSec);
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	}
	else {
		return nullptr;
	}

	return sw;
}