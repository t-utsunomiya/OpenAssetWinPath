#if WITH_EDITOR

#include "OpenAssetWinPathCommands.h"

#define LOCTEXT_NAMESPACE "OpenAssetWinPathCommands"

void FOpenAssetWinPathCommands::RegisterCommands()
{
	UI_COMMAND(OpenAssetWinPathCommand
		, "Call OpenAssetWinPath Command", "Description: call OpenAssetWinPath Command"
		// なるべく他のキー設定と被らないように EModifierkey を複数設定しています。
		// 手動で起動するのではなく、ツールを介して OpenAssetWinPathDialog を開くことを想定しているので
		// EModifierkey の複数設定は使い勝手の低下につながらないと思います。
		// EKeys::F12 を設定すると、 EModifierKey と一緒に押したときは正しく動作するものの
		// F12 単体で押したときに例外が発生したので使用を控えています。
		, EUserInterfaceActionType::None, FInputChord(EKeys::O
			, EModifierKey::Control|EModifierKey::Shift|EModifierKey::Alt));
}

#undef LOCTEXT_NAMESPACE

#endif
