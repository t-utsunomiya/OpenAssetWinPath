#ifdef WITH_EDITOR
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IOpenAssetWinPath.h"
#include "LevelEditor.h"
#include "Textures/SlateIcon.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "OpenAssetWinPathDialog.h"
#define LOCTEXT_NAMESPACE "OpenAssetWinPath"


class FOpenAssetWinPath : public IOpenAssetWinPath
{
private:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void OnWindowMenuExtension(FMenuBuilder& MenuBuilder);
	void OnMenu();
	TSharedPtr<FExtender> Extender;

private:
	void OnMainFrameLoad(TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow);
	TWeakPtr<SWindow> RootWindow;
	TWeakPtr<SOpenAssetWinPathDialog> OpenAssetDialog;

};

IMPLEMENT_MODULE( FOpenAssetWinPath, OpenAssetWinPath )

void FOpenAssetWinPath::StartupModule()
{
	if (IsRunningCommandlet())
	{
		return;
	}
	Extender = MakeShareable(new FExtender);
	Extender->AddMenuExtension(
		"Miscellaneous",
		EExtensionHook::After,
		NULL,
		FMenuExtensionDelegate::CreateRaw(this, &FOpenAssetWinPath::OnWindowMenuExtension)
	);
	FLevelEditorModule& LevelEditorModule =
		FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(Extender);
	IMainFrameModule& MainFrameModule =
		FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	MainFrameModule.OnMainFrameCreationFinished().AddRaw(
		this, &FOpenAssetWinPath::OnMainFrameLoad);
}

void FOpenAssetWinPath::ShutdownModule()
{
	if (Extender.IsValid() && FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule =
			FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(Extender);
	}
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrameModule =
			FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		MainFrameModule.OnMainFrameCreationFinished().RemoveAll(this);
	}
}

void FOpenAssetWinPath::OnMainFrameLoad(TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow)
{
	if ((!bIsNewProjectWindow) && (InRootWindow.IsValid()))
	{
		RootWindow = InRootWindow;
	}
}

void FOpenAssetWinPath::OnWindowMenuExtension(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("OpenAssetWinPathHook", LOCTEXT("OpenAssetWinPath", "OpenAssetWinPath"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("OpenAssetWinPathTitle", "OpenAssetWinPath"),
		LOCTEXT("OpenAssetWinPathToolTip", "open asset from windows path."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FOpenAssetWinPath::OnMenu)));

	MenuBuilder.EndSection();
}

void FOpenAssetWinPath::OnMenu()
{
	if (!OpenAssetDialog.IsValid())
	{
		TSharedPtr<SOpenAssetWinPathDialog> Dialog = SNew(SOpenAssetWinPathDialog)
			.Title(LOCTEXT("OpenAssetWinPathDialog", "OpenAssetWinPathDialog"));
		OpenAssetDialog = TWeakPtr<SOpenAssetWinPathDialog>(Dialog);
		if (RootWindow.IsValid())
		{
			FSlateApplication::Get().AddWindowAsNativeChild(
				Dialog.ToSharedRef(), RootWindow.Pin().ToSharedRef());
		}
		Dialog->SetDialogContent();
	}
	OpenAssetDialog.Pin()->BringToFront();
	OpenAssetDialog.Pin()->SetFocusTopInputForm();
}

#undef LOCTEXT_NAMESPACE
#endif
