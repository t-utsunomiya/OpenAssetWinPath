#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IOpenAssetWinPath.h"
#include "LevelEditor.h"
#include "MultiBox/MultiBoxBuilder.h"
#include "Textures/SlateIcon.h"
#define LOCTEXT_NAMESPACE "OpenAssetWinPath"


class FOpenAssetWinPath : public IOpenAssetWinPath
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void OnWindowMenuExtension(FMenuBuilder& MenuBuilder);
	void OnMenu();
	TSharedPtr<FExtender> Extender;
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
}


void FOpenAssetWinPath::ShutdownModule()
{
	if (Extender.IsValid() && FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule =
			FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(Extender);
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

}

