; UE4 の Asset を開くためにファイル名などの情報を UE4Editor に渡すための ahk スクリプト.

#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

ArgCount = %0%
If(ArgCount != 1)
{
	MsgBox Usage: %a_scriptname% AssetPath
	ExitApp
}
AssetPath = %1%

AssetOpenWindowTitle := "OpenAssetWinPathDialog"
UE4WindowClass := "UnrealWindow"

OpenAsset(ByRef WindowsPath)
{
	BlockInput, on
	clipboard = %WindowsPath%
	Send, ^{a down}{a up}^v
	SendInput, {Tab}{Enter}
	BlockInput, off
}

OpenDialog()
{
	SendInput ^!+{o down}{o up}
}

SetTitleMatchMode 3 ; タイトル検索方法は完全一致を指定
; UE4Editor が立ち上がっているかどうか
IfWinExist ahk_class %UE4WindowClass%
{
	WinActivate ahk_class %UE4WindowClass%
	OpenDialog()
	WinWaitActive, %AssetOpenWindowTitle% ahk_class %UE4WindowClass%, , 0
	If ErrorLevel <> 0
	{
		Msgbox, %AssetOpenWindowTitle% が開きませんでした。`n UE4 の OpenAssetWinPath プラグインを有効にして、ショートカットキー(Ctrl+Alt+Shift+o)で%AssetOpenWindowTitle% が開く設定になっているか確認してください。
	}
	Else
	{
		WinActivate %AssetOpenWindowTitle% ahk_class %UE4WindowClass%
		OpenAsset(AssetPath)
	}
}
Else
{
	Msgbox, 実行中の UE4Editor が見つかりませんでした。UE4Editor が起動していなければ立ち上げてから実行してください。
}

Return
