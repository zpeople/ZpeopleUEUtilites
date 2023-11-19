#pragma once
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ZpeopleHttpDownloadManager.generated.h"
/*
�����ļ�������
*/
class UHttpDownloader;

using namespace UF;
using namespace UP;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDownloadFileDelegate, FString, DestinationPath, float, DownloadPercent);

UENUM()
enum class EDownLoadBaseDir : uint8
{
	GameDir,
	EngineDir,
	SaveDir,
	UserDir
};

UCLASS()
class UHttpDownloadManager : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Zpeople", meta = (BlueprintInternalUseOnly = "true"))
	static UHttpDownloadManager* DownloadFile(FString url,EDownLoadBaseDir basePath, FString relateve_path);

	UPROPERTY(BlueprintAssignable)
	FDownloadFileDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FDownloadFileDelegate OnFail;

	UPROPERTY(BlueprintAssignable)
	FDownloadFileDelegate OnProgress;

public:

private:
	void StartMission();
	FString url;//�������ص��ļ���URL��ַ
	FString DestinationPath;//����·��
	//**************Parent************
	int32 FileTotalSize = 0;//�ļ��ܴ�С
	int32 FileDownloadedSize = 0;

	//HTTP������ɻ��ж�
	void HandleHttpCompelete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void HandleHttpProgress(FHttpRequestPtr req, int32 uploaded, int32 downloaded);

	
};