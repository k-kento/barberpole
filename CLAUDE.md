# Barberpole - 知育アプリ

幼児向け知育コンテンツを提供する Android アプリ。Vulkan を使用した高性能グラフィックス描画を特徴とする。

## プロジェクト構造

```
barberpole/
├── app/                    # Android アプリモジュール (Kotlin/Compose)
├── myvulkan/               # Vulkan レンダリングモジュール
│   ├── src/main/java/      # Kotlin ラッパー (JNI ブリッジ)
│   ├── src/main/cpp/       # C++ ネイティブコード
│   │   ├── vulkan/         # Vulkan コア & 機能
│   │   │   ├── core/       # VulkanContext, Pipeline, RenderPass 等
│   │   │   └── features/   # Drawing, Kaleidoscope 等の機能
│   │   ├── platform/       # Android JNI バインディング
│   │   └── external/       # 外部ライブラリ (Vulkan-Hpp, glm, stb)
│   └── shaders/            # GLSL シェーダー (.vert, .frag, .comp)
├── doc/design/             # 設計ドキュメント (Mermaid)
└── web/                    # Web 関連
```

## 技術スタック

- **言語**: Kotlin, C++17, GLSL
- **UI**: Jetpack Compose
- **グラフィックス**: Vulkan (vulkan.hpp C++ バインディング)
- **DI**: Hilt
- **テスト**: JUnit, MockK


## 開発ガイドライン

### ブラシ追加手順

1. `myvulkan/shaders/drawing/` にシェーダー作成 (.vert, .frag)
2. `pipeline/` に `*_pipeline.hpp` 作成
3. `brush/` に `*_brush.hpp` 作成
4. `PipelineManager` と `BrushManager` に登録
5. `DrawingRenderer.kt` の `BrushType` enum に追加
6. `DrawingPage.kt` の UI に追加

### Vulkan コード規約

- RAII: `vk::Unique*` を使用してリソース管理
- フレームバッファリング: `MAX_FRAMES_IN_FLIGHT` (通常 2)
- コマンドプール: 描画用 (`mGraphicsCommandPool`) と一時用 (`mTransientCommandPool`) を分離

### 命名規則

- C++ メンバ変数: `m` プレフィックス (例: `mDevice`)
- Kotlin: キャメルケース
- シェーダー入力: `in` プレフィックス (例: `inPos`, `inColor`)
- シェーダー出力: `out` / `v` プレフィックス (例: `outColor`, `vColor`)

## 注意事項

- 常にアプリのビルドはしないでください。
- 常にシェーダーのコンパイルはしないでください。
- シェーダーの `input` は GLSL 予約語。変数名には使用不可
- PointList 使用時は `gl_PointSize` を設定すること
- JNI のネイティブハンドルは `0L` チェックを必ず行う

## ドキュメント生成

### クラス図

設計図（クラス図・アーキテクチャ図）には
原則としてメソッドを記載しない。

設計図は以下のみを表現すること:
- クラス名
- 責務
- 依存関係
- 所有関係

メソッドの詳細や実装は
必ずソースコードを参照する方針とする。

設計図と実装の二重管理を避け、
設計図は「構造理解」にのみ使う。
ヘルパークラスは、図に出力しなくていい。
