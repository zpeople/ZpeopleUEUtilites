#include "ZpeopleHttpDownloadManager.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"
#include "Http.h"

DEFINE_LOG_CATEGORY_STATIC(LogHttpDownload, Warning, All);

UHttpDownloadManager::UHttpDownloadManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UHttpDownloadManager* UHttpDownloadManager::DownloadFile(FString url, EDownLoadBaseDir basePath, FString relateve_path)
{
	UHttpDownloadManager* Downloader = NewObject<UHttpDownloadManager>();
	Downloader->url = url;
	FString FileName = FPaths::GetCleanFilename(relateve_path);
	
	FString bPath;
	switch (basePath) {
	case EDownLoadBaseDir::GameDir:
		bPath = FPaths::ProjectDir();
		break;
	case EDownLoadBaseDir::EngineDir:
		bPath = FPaths::EngineDir();
		break;
	case EDownLoadBaseDir::SaveDir:
		bPath = FPaths::ProjectSavedDir();
		break;
	case EDownLoadBaseDir::UserDir:
		bPath = FPaths::ProjectUserDir();
		break;
	}
	bPath = FPaths::ConvertRelativePathToFull(bPath);
	bPath = FPaths::Combine(bPath, relateve_path);
	FString desPath = FPaths::GetPath(bPath);
	FString desName = FPaths::GetCleanFilename(bPath);
	UE_LOG(LogHttpDownload, Warning, TEXT("desPath = %s  desName = %s   =>%s"), *desPath,*desName,*bPath);
	if (IFileManager::Get().DirectoryExists(*desPath) == false)
	{
		if (IFileManager::Get().MakeDirectory(*desPath, true) ==false) {
			Downloader->OnFail.Broadcast("Make dir failed", 0);
		}
	}
	Downloader->url = url;
	Downloader->DestinationPath = bPath;
	Downloader->StartMission();
	return Downloader;
}
void UHttpDownloadManager::StartMission()
{
	TSharedRef<class IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHttpDownloadManager::HandleHttpCompelete);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHttpDownloadManager::HandleHttpProgress);
	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->ProcessRequest();
}
void UHttpDownloadManager::HandleHttpProgress(FHttpRequestPtr req, int32 uploaded, int32 downloaded) {
	UE_LOG(LogHttpDownload, Warning, TEXT("Download Progress %d   %d"),uploaded,downloaded);
	//UE_LOG(LogHttpDownload, Warning, TEXT("Chunk Size =  %d "), req->GetResponse()->GetContent().Num());
	float raio = (downloaded*1.0) / req->GetResponse()->GetContentLength();
	OnProgress.Broadcast(DestinationPath, raio);
}
void UHttpDownloadManager::HandleHttpCompelete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (!bSucceeded) {
		UE_LOG(LogHttpDownload, Warning, TEXT("获取下载目标文件信息失败，请检查网络"));
		OnFail.Broadcast("获取下载目标文件信息失败，请检查网络", 0);
		return;
	}
	if (HttpResponse.IsValid() && EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()) && bSucceeded) {

		TArray<uint8> data = HttpResponse->GetContent();
		FFileHelper::SaveArrayToFile(data, *DestinationPath);
		OnSuccess.Broadcast(DestinationPath, 1);
	}
	else {
		OnFail.Broadcast("获取下载目标文件信息失败，请检查网络", 0);
	}
	return;
}