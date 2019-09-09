#ifdef WITH_EDITOR
#include "OpenAssetWinPathDialog.h"
#include "Widgets/SCanvas.h"
#include "Application/SlateApplicationBase.h"
#include "Application/SlateWindowHelper.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Runtime/SlateCore/Public/Layout/WidgetPath.h"
#include "Runtime/CoreUObject/Public/Misc/PackageName.h"
#include "Toolkits/AssetEditorManager.h"

#define LOCTEXT_NAMESPACE "OpenAssetWinPathDialog"

SOpenAssetWinPathDialog::SOpenAssetWinPathDialog()
 : WindowTitle(TEXT("OpenAssetWinPathDialog"))
{
}

void SOpenAssetWinPathDialog::SetDialogContent()
{
	WindowsPathTextBox = SNew(SEditableTextBox)
		.HintText(LOCTEXT("WindowsPathTextBoxHint", "Enter Path: e.g. D:\\MyProject\\Content\\MyContent\\NewBlueprint.uasset"))
		.OnTextChanged_Raw(this, &SOpenAssetWinPathDialog::OnChangeWindowsPath);

	#define ROW(Description, TextBox) \
		+ SVerticalBox::Slot().VAlign(VAlign_Top).Padding(2.f).AutoHeight() [ \
			SNew(SHorizontalBox) \
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Padding(2.f).FillWidth(1.f) \
			[ SNew(STextBlock).Text(LOCTEXT(Description, Description)) ] \
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Padding(2.f).FillWidth(6.f) \
			[ SNew(SCanvas) + SCanvas::Slot().VAlign(VAlign_Center).HAlign(HAlign_Left).Size(FVector2D(500.f, 18.f))[TextBox.ToSharedRef()] ] \
		]

	SetContent(
		SNew(SVerticalBox)
		ROW("Windows Path: ", WindowsPathTextBox)
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

FReply SOpenAssetWinPathDialog::OnOpenButtonClicked()
{
	FString const ContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	FString WindowsPathStr = WindowsPath.ToString();
	FPaths::NormalizeFilename(WindowsPathStr);
	if (WindowsPathStr.StartsWith(ContentDir))
	{
		FString AssetPath = FString(TEXT("/Game/")) + WindowsPathStr.RightChop(ContentDir.Len());
		FAssetEditorManager::Get().OpenEditorForAsset(AssetPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found Content Path In Input Asset Windows Path. " \
			"Input Asset Windows Path: '%s', Expect Content Path: '%s'"), *WindowsPathStr, *ContentDir);
	}
	return FReply::Handled();
}

void SOpenAssetWinPathDialog::OnChangeWindowsPath(const FText& NewWindowsPath)
{
	WindowsPath = NewWindowsPath;
}

#undef LOCTEXT_NAMESPACE
#endif
