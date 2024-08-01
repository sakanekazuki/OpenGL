#version 450

// 入力変数
layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexcoord;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec3 inPosition;

// 出力変数
out vec4 fragColor;

layout(location=0) out vec4 outinColor;
layout(location=1) out vec2 outinTexcoord;
layout(location=2) out vec3 outinNormal;

// ユニフォーム変数	
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2D texShadow;

layout(location=1) uniform vec3 lightDirection;
layout(location=2) uniform vec3 lightColor;
layout(location=3) uniform vec3 ambientlight;
// 影行列
layout(location=100)uniform mat4 matShadow;

// 平行光源
struct DirerctionalLight
{
  // ライトの向き
  vec3 direction;
  // ライトの色(明るさ)
  vec3 color;
};

DirerctionalLight light = 
{
  { -0.70, -0.59, -0.41 },
  { 1.94,  1.65,  1.24 },
};

// 環境光の色
vec3 ambientLight = { 0.15, 0.10, 0.20 };

// 影をぼかすためのサンプリング座標.
const int sampleCount = 16;
const vec2 poissonDisk[sampleCount] =
{
  {-0.94201624, -0.39906216}, 
  { 0.94558609, -0.76890725},
  { -0.094184101, -0.92938870}, 
  { 0.34495938, 0.29387760},
  { -0.91588581, 0.45771432},
  { -0.81544232, -0.87912464},
  { -0.38277543, 0.27676845},
  { 0.97484398, 0.75648379},
  { 0.44323325, -0.97511554},
  { 0.53742981, -0.47373420},
  { -0.26496911, -0.41893023},
  { 0.79197514, 0.19090188} ,
  { -0.24188840, 0.99706507},
  { -0.81409955, 0.91437590},
  { 0.19984126, 0.78641367},
  { 0.14383161, -0.14100790},
};

// 頂点シェーダープログラム
void main()
{
  vec4 tc = texture(texColor,inTexcoord);
  
  fragColor = inColor * tc;

  // 線形補間によって長さが1ではなくなっているので正規化して長さを1に復元する
  vec3 worldNormal = normalize(inNormal);

  // 面が裏向きの場合、法線の向きを逆にする
  if(gl_FrontFacing == false)
  {
    worldNormal = -worldNormal;
  }

  // 影
  // 法線方向に移動させるピクセル数
  float normalBias = 8.0; 
   // ワールド座標系の影テクスチャの大きさ
  const vec2 shadowAreaSize = vec2(100.0, 100.0);
  vec2 shadowPixelSize =
   // 1ピクセルの大きさ(ワールド座標)
    shadowAreaSize / textureSize(texShadow, 0);
  normalBias *= max(shadowPixelSize.x, shadowPixelSize.y);
  vec4 shadowPos = matShadow * vec4(inPosition + worldNormal * normalBias, 1.0);
  // パースを解除
  shadowPos.xyz *= 1.0 / shadowPos.w;
  // 影をぼかす
//  float shadow = float(texture(texShadow, shadowPos.xy).r < shadowPos.z);
  float shadow = 0.0;
  // ぼかし半径(ワールド座標)
  vec2 shadowRadius = vec2(0.03); 
   // ぼかし半径をテクスチャ座標系に変換
  shadowRadius /= shadowAreaSize;
  for (int i = 0; i < sampleCount; ++i)
  {
    vec2 texcoord = shadowPos.xy + poissonDisk[i] * shadowRadius;
    shadow += float(texture(texShadow, texcoord).r < shadowPos.z);
  }
  shadow *= 1.0 / float(sampleCount);

  // 環境光を設定
  vec3 lightcolor = ambientlight;

  // ランバート反射による明るさを計算
  float cosTheta = max(dot(worldNormal, -lightDirection), 0);
  lightcolor += lightColor * cosTheta * (1.0 - shadow);

  fragColor.rgb *= lightcolor;
}