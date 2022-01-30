#pragma once

#ifdef WITH_EDITOR

#include "CoreMinimal.h"
#include "Widgets/SWindow.h"

class SOpenAssetWinPathDialog : public SWindow
{
public:
	SOpenAssetWinPathDialog();
	/**
	 * ダイアログの中身を設定する.
	 */
	void SetDialogContent();
	/**
	 * フォーカスを一番上の入力フォームに合わせます。
	 */
	void SetFocusTopInputForm();

private:
	FReply OnOpenButtonClicked();
	void OnChangeWindowsPath(const FText& NewArgs);
	void SetFocus(TSharedRef<SWidget> TargetWidget);
	bool GetPackageName(const FString& FilePath, FString& OutPackageName);
	bool Copy(const TCHAR* DestPath, const TCHAR* SrcPath);

private:
	FText WindowsPath;
	FText AssetName;

	TSharedPtr<SWidget> WindowsPathTextBox;
	const TCHAR* WindowTitle;
};

#endif
