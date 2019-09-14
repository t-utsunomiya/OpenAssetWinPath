# OpenAssetWinPath
uasset を Windows のエクスプローラなどから開くためのツール  
[![](https://img.youtube.com/vi/ridmdgbuG50/0.jpg)](https://www.youtube.com/watch?v=ridmdgbuG50)  
例えば TortoiseSVN のコミットログから "Open Local" で uasset を開く事もできます。  
[![](https://img.youtube.com/vi/hNvsksUdbss/0.jpg)](https://www.youtube.com/watch?v=hNvsksUdbss)

## 使い方
1. このリポジトリを UE4 プロジェクトフォルダの /Plugins フォルダにコピーするかチェックアウトしてください。
2. リポジトリ内の OpenAssetWinPath/WindowsTool を都合のいい場所に移動やコピーしてください。  
このフォルダには uasset を Windows で開くための exe ファイルが入っています。 UE4 プロジェクト内ではなく、PC 内の一か所に置いた方が管理しやすいと思います。
3. UE4 プロジェクトのプラグイン設定でこのプラグインを有効にしてください。
4. OpenAssetWinPath/WindowsTool/OpenUE4Asset.exe を uasset を実行するプログラムに指定してください。  
適当な uasset をダブルクリックして "この .uasset ファイルを開く方法を選択してください。" - "その他のアプリ" - "この PC で別のプログラムを選ぶ" から OpenUE4Asset.exe を選択してください。
ダブルクリックした uasset が Content フォルダに含まれている UE4 プロジェクトのエディタが開いていれば uasset が開きます。

## 注意
- UE4 Editor で開いているプロジェクトの Content フォルダ以下の uasset 以外は開きません。uasset が開かないときはアウトプットログを見てエラーが無いか確認してください。
- OpenAssetWinPath プラグインはショートカットキー「Alt + Shift + Ctrl + O」を同時に押してダイアログを開いた後にパスを入力して uasset を開きます。ショートカットキーの設定を変更するなどしてダイアログが開かない状態だと、 uasset を開けません。
