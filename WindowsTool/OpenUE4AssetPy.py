# coding: UTF-8
# uasset のファイルパスをパスとアセット名を分けて同じフォルダにある OpenUE4Asset.exe に受け渡します。
# UE4 の OpenEditorForAsset() 関数は、拡張子を除いたファイル名がアセット名でないと開けません。
# Perforce 履歴からファイルを開こうとするとファイル名に#が付き、ファイル名がアセット名になりません。
# このような場合にもパスからアセット名を取得して OpenUE4Asset.exe に渡します。
# 
# ファイル名から以下のパターンでアセット名を判断します。
#  "Path/AssetName.uasset" - UE4 が生成する通常のuassetファイル
#  "Path/AssetName#1.uasset" - Perforce 履歴から開こうとした時
import sys
import os
import subprocess

def OpenAsset(path):
  name = os.path.basename(path).split('.', 1)[0]
  if '#' in name:
    name = name.split('#', 1)[0]
  
  subprocess.call(('OpenUE4Asset.exe', path + ',' + name))
#  print(path + ',' + name)

if __name__ == '__main__':
  args = sys.argv
  if len(args) == 2:
    OpenAsset(args[1])
  else:
    print('Invalid Argments. uasge: python OpenUE4AssetPy.py "AssetPath"')

