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
	void OnChangeWindowsPath(const FText& NewAssetName);
	void SetFocus(TSharedRef<SWidget> TargetWidget);
	bool GetPackageName(const FString& FilePath, FString& OutPackageName);

private:
	FText WindowsPath;

	TSharedPtr<SWidget> WindowsPathTextBox;
	const TCHAR* WindowTitle;
};

#endif
