#pragma once


#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZpeopleScreenShootTool.generated.h"


/**
 * 这是一个用来实现蓝图里截图功能的扩展
 *因为目前（4.10）为止，虽然能实现截图，用ExecConsoleCommand节点。 但是它不能获取截图的返回值
 *而且不能手动设置截图的名字
 *所以写了此功能
 */
using namespace UF;
UENUM(BlueprintType)
enum class EImageFormatType : uint8
{
	/** Portable Network Graphics. */
	PNG,

	/** Joint Photographic Experts Group. */
	JPEG,

	/** OpenEXR (HDR) image file format. */
	EXR,
};
UCLASS()
class ZPEOPLEUEUTILITES_API UScreenShootTool : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FZpeopleShootRequestDelegate, FString, Path, bool, StatusCode);

	UFUNCTION(BlueprintCallable, Category = "Zpeople|ZpeopleUEUtilites")
		static void SetShootRequestDelegate(FZpeopleShootRequestDelegate OnShootRequestDelegate);

	static void OnShootRequestDelegate(FString path, bool IsSuccess);

	/**
	 *截图，并设置名字
	 *@picName 图片名字。可以不设置
	 *@bUnique 是否是唯一的名字  例如 picName 设置为 a的话，会截一张名为a.png的图片。
	 *然后，再次执行截图，  如果bUnique是true，则新截出来的图片会叫作a0001.png ，
	 *如果bUnique为fasle，则新图片会覆盖之前的
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|ZpeopleUEUtilites")
		static FString TakeScreenShoot(FString picName, bool bUnique = true);


	/**
	 *截图，并设置名字
	 *@fineName 图片名字。可以不设置
	 *@path 路径,可以不设置
	 *@ _format 图片后缀
	 *@ _quality 质量
	 *@ bInsertTimeStampToFileName 是否时间戳
	  *@bInShowUI 禁用屏幕消息时，屏幕截图请求没有UI
	  *@bAddFilenameSuffix 名字后面加上下划线
	 *@bUnique 是否是唯一的名字  例如 picName 设置为 a的话，会截一张名为a.png的图片。
	 *然后，再次执行截图，  如果bUnique是true，则新截出来的图片会叫作a0001.png ，
	 *如果bUnique为fasle，则新图片会覆盖之前的
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"), Category = "Zpeople|ZpeopleUEUtilites")
		static void TakeScreenShotAdvance(const FString & _fineName, const FString & _path, EImageFormatType _format, int32 _quality, bool bInsertTimeStampToFileName, bool bInShowUI, bool bAddFilenameSuffix);



	static void OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap);
};
