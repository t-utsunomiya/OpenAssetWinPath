#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

class FOpenAssetWinPathCommands : public TCommands<FOpenAssetWinPathCommands>
{
public:
	FOpenAssetWinPathCommands()
		: TCommands<FOpenAssetWinPathCommands>(TEXT("OpenAssetWinPath")
		, NSLOCTEXT("Contexts", "OpenAssetWinPathCommands", "OpenAssetWinPath Command")
		, NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<class FUICommandInfo> OpenAssetWinPathCommand;
};

#endif
