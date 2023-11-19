
#include "DynamicLoader.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Core/Public/Async/Async.h"

#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "IImageWrapperModule.h"

#include "ZpeopleLoadSource.h"

#include "Engine/Texture2D.h"





void UImageLoader::LoadImageAsync(UObject* worldContextObject, const FString& ImagePath,const FString &inputID)
{
	

	AsyncTask(ENamedThreads::AnyThread, [=]()
	{

		LoadID = inputID;
		reTexture = LoadImageFromDisk(ImagePath);

		if (worldContextObject->IsValidLowLevel())
		{
		LoadCompleted.Broadcast(reTexture,LoadID);
		}
	});

}


UImageLoader::~UImageLoader()
{
	if (reTexture->IsValidLowLevel())
	{
		reTexture->RemoveFromRoot();
		reTexture->ConditionalBeginDestroy();
	}
	reTexture = nullptr;
}

UTexture2D* UImageLoader::LoadImageFromDisk( const FString& ImagePath)
{

	bool Succss;	int32 Weight;
	int32 Heighting;
	
	//return UZpeopleLoadSource::LoadTexture2DFromFile(ImagePath,Succss, Weight,Heighting);
	
	//return UZpeopleLoadSource::LoadTexAndSaveThumbnailFromFile(ImagePath,false, Succss, Weight, Heighting);
	return UZpeopleLoadSource::LoadTexAndSaveBinFromFile(ImagePath,true, Succss, Weight, Heighting);
}
//如果蓝图没有引用 生成时Loader->AddToRoot();
void UImageLoader::RemoveSelf() {
	if (this->IsValidLowLevel())
	{
	this->RemoveFromRoot();
	}

}



AExeActor::AExeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

bool AExeActor::Getstat()
{

	bool a = FPlatformProcess::IsProcRunning(CheckProc);

	return a;

	//return true;
}


// Called when the game starts or when spawned
void AExeActor::BeginPlay()
{

	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("EXE Begin"));

}

void AExeActor::Tick(float DeltaTime)
{
	//Super::Tick();

	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("I AM IN There"));

	bool a = FPlatformProcess::IsProcRunning(CheckProc);

	if (!a && !bisShutDown)
	{

		UE_LOG(LogTemp, Warning, TEXT("EXE End"));

		bisShutDown = true;
		ProcShutdown.Broadcast(true);
		PrimaryActorTick.bCanEverTick = false;

		Destroy();

	}
	//return true;
}
