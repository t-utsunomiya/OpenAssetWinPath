#ifdef WITH_EDITOR
#include "OpenAssetWinPathDialog.h"
#include "Widgets/SCanvas.h"
#include "Application/SlateApplicationBase.h"
#include "Application/SlateWindowHelper.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformFileManager.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Runtime/SlateCore/Public/Layout/WidgetPath.h"
#include "Runtime/CoreUObject/Public/Misc/PackageName.h"
#include "Editor/EditorEngine.h"
#include "Subsystems/AssetEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "OpenAssetWinPathDialog"

SOpenAssetWinPathDialog::SOpenAssetWinPathDialog()
 : WindowTitle(TEXT("OpenAssetWinPathDialog"))
{
}

void SOpenAssetWinPathDialog::SetDialogContent()
{
	WindowsPathTextBox = SNew(SEditableTextBox)
		.HintText(LOCTEXT("WindowsPathTextBoxHint", "Enter Path[,AssetName]: e.g. D:\\MyProject\\Content\\MyContent\\NewBlueprint.uasset,NewBlueprint"))
		.OnTextChanged_Raw(this, &SOpenAssetWinPathDialog::OnChangeWindowsPath);

	#define ROW(Description, TextBox) \
		+ SVerticalBox::Slot().VAlign(VAlign_Top).Padding(2.f).AutoHeight() [ \
			SNew(SHorizontalBox) \
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Padding(2.f).FillWidth(1.f) \
			[ SNew(STextBlock).Text(LOCTEXT(Description, Description)) ] \
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Padding(2.f).FillWidth(3.f) \
			[ SNew(SCanvas) + SCanvas::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Size(FVector2D(440.f, 18.f))[TextBox.ToSharedRef()] ] \
		]

	SetContent(
		SNew(SVerticalBox)
		ROW("Windows Path[,AssetName]: ", WindowsPathTextBox)
		+ SVerticalBox::Slot().HAlign(HAlign_Right).VAlign(VAlign_Bottom).Padding(2.f)
		[
			SNew(SButton).Text(LOCTEXT("Open", "Open"))
			.OnClicked_Raw(this, &SOpenAssetWinPathDialog::OnOpenButtonClicked)
		]
	#undef ROW
	);
	Resize(FVector2D(595.f, 50.f));
}

void SOpenAssetWinPathDialog::SetFocusTopInputForm()
{
	SetFocus(WindowsPathTextBox.ToSharedRef());
}

void SOpenAssetWinPathDialog::SetFocus(TSharedRef<SWidget> TargetWidget)
{
	TArray< TSharedRef<SWindow> > WindowsToSearch;
	WindowsToSearch.Add(SharedThis(this));
	FWidgetPath WidgetToFocusPath;
	if (FSlateWindowHelper::FindPathToWidget(WindowsToSearch, TargetWidget, WidgetToFocusPath))
	{
		FSlateApplicationBase::Get().SetAllUserFocus(WidgetToFocusPath, EFocusCause::SetDirectly);
	}
}

bool SOpenAssetWinPathDialog::Copy(const TCHAR* DestPath, const TCHAR* SrcPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.CreateDirectoryTree(GetData(FPaths::GetPath(DestPath))))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed create directory '%s'."), DestPath);
		return false;
	}
	if (!PlatformFile.CopyFile(DestPath, SrcPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed copy file. From: '%s' To: '%s'."), SrcPath, DestPath);
		return false;
	}
	return true;
}

FReply SOpenAssetWinPathDialog::OnOpenButtonClicked()
{
	FString const ContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	FString WindowsPathStr = WindowsPath.ToString();
	FPaths::NormalizeFilename(WindowsPathStr);

	UAssetEditorSubsystem* AssetEditor;
	if (GEditor)
	{
		AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		if (!AssetEditor)
		{
			UE_LOG(LogTemp, Error, TEXT("UAssetEditorSubsystem is null."));
			return FReply::Handled();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GEditor is null."));
		return FReply::Handled();
	}

	if (WindowsPathStr.StartsWith(ContentDir))
	{
		FString AssetPath = FString(TEXT("/Game/")) + WindowsPathStr.RightChop(ContentDir.Len());
		AssetEditor->OpenEditorForAsset(AssetPath);
	}
	else
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (!PlatformFile.FileExists(*WindowsPathStr))
		{
			UE_LOG(LogTemp, Error, TEXT("Not found file '%s.'"), *WindowsPathStr);
			return FReply::Handled();
		}
		
		FString DestPath = FPaths::ProjectSavedDir() + TEXT("OpenAssetWinPath/");
		if (!AssetName.EqualTo(FText()))
		{
			DestPath += AssetName.ToString() + TEXT(".uasset");
		}
		else
		{
			DestPath += FPaths::GetCleanFilename(WindowsPathStr);
		}

		const int64 FileSize = PlatformFile.FileSize(*WindowsPathStr);
		const int64 WorningSize = 500 * 1024 * 1024;
		const FString MessageString = FString::Printf(
			TEXT("Contentフォルダ以下にファイルが見つかりませんでした。一時フォルダ以下にコピーして開きますか？\n")
			TEXT("プロジェクト外のファイルを開くと正しく動作しない可能性があります。\n")
			TEXT("コピー元: %s\nコピー先: %s\nサイズ: %d MB"), *WindowsPathStr, *DestPath, (FileSize / (1024 * 1024)) + ((FileSize % (1024 * 1024)) ? 1 : 0));
		const FText TitleText = LOCTEXT("OpenAssetWinPathDialog - FileCopy", "OpenAssetWinPathDialog - FileCopy");
		if (FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(MessageString), &TitleText) == EAppReturnType::Yes)
		{
			Copy(*DestPath, *WindowsPathStr);
			AssetEditor->OpenEditorForAsset(DestPath);
			return FReply::Handled();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Not Found Content Path In Input Asset Windows Path. " \
				"Input Asset Windows Path: '%s', Expect Content Path: '%s'"), *WindowsPathStr, *ContentDir);
		}
	}
	return FReply::Handled();
}

void SOpenAssetWinPathDialog::OnChangeWindowsPath(const FText& NewArgs)
{
	TArray<FString> Args;
	NewArgs.ToString().ParseIntoArray(Args, TEXT(","));
	if (Args.Num() == 2)
	{
		WindowsPath = FText::FromString(Args[0]);
		AssetName = FText::FromString(Args[1]);
	}
	else
	{
		WindowsPath = NewArgs;
		AssetName = FText();
	}
}

#undef LOCTEXT_NAMESPACE
#endif
