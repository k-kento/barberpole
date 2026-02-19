# オモチャバコ

## 機能一覧

| 機能 | 概要 | 画像・動画 |
| --- | --- | --- |
| 車両 | カラフルな車両アニメーション。ホールドで停車し、放置すると渋滞が発生 |  |
| 打楽器 | 8 種類の打楽器を色分けしたボタンで配置。タップで即座に音が鳴る |  |
| 魚群 | 自然界の動きへの興味を刺激 | ボイドアルゴリズムによるリアルな群れの動き。タップで魚が逃げる | https://youtube.com/shorts/N72epCuQm-E |
| サインポール | 色彩感覚・観察力を養う | OpenGL ES で描画。回転速度・色の変更が可能 | https://youtube.com/shorts/edAh27vCm04 |
| 万華鏡 | 創造性・美的感覚を育む | Vulkan による GPU アクセラレーション描画。画像選択・回転操作が可能 | https://youtube.com/shorts/g26pVOUF4vs |

### 基本機能

| 機能 | 説明 |
| --- | --- |
| お知らせ | Firebase Remote Config で動的に表示。既読管理付き |
| 強制アップデート | 古いバージョンの使用をブロックし、アップデートを促す |
| 利用規約同意 | 初回起動時に同意を求め、規約更新時には再同意を促す |

## 技術スタック

| カテゴリ | 技術 | 備考 |
| --- | --- | --- |
| 言語 | Kotlin, C++ | C++ は Vulkan 描画モジュールで使用 |
| UI | Jetpack Compose, Material 3 | 宣言的 UI によるモダンな画面構成 |
| グラフィックス | OpenGL ES 3.0, Vulkan | 車両・魚群・サインポールは OpenGL ES、万華鏡は Vulkan |
| 通信 | Firebase Remote Config | お知らせ・強制アップデート機能で使用 |
| DI | Hilt | - |
| ストレージ | Room, DataStore | Room でお知らせ永続化、DataStore で既読フラグ・同意状態を管理 |
| テスト | JUnit , Mockk, Robolectric, Espresso | - |
| 画像読み込み | Coil | - |
| アーキテクチャ | MVVM + Clean Architecture | Repository パターンによるデータ層の抽象化 |

## 今後の展望

<!-- 例 -->
<!-- - コンテンツの追加（お絵かき、音声ガイドなど） -->
<!-- - 保護者向けの利用レポート機能 -->
<!-- - 対象年齢に応じたレベル調整機能 -->
