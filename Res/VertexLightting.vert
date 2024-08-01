#version 450

// 入力変数
layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexcoord;
layout(location=3) in vec3 vNormal;

// 出力変数
layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outTexcoord;
out gl_PerVertex {
  vec4 gl_Position;
};

// ユニフォーム変数
layout(location=0) uniform mat4 matTRS;
layout(location=1) uniform mat4 matModel;

// 平行光源
struct DirectionalLight
{
  vec3 direction; // ライトの向き
  vec3 color;     // ライトの色(明るさ)
};

DirectionalLight light =
{
  { 0.08, -0.82, -0.57},
  { 2.00, 1.88, 1.82},
};

// 環境光の色(明るさ)
vec3 ambientLight = { 0.10, 0.15, 0.20};

// 頂点シェーダープログラム
void main() {
  // 回転行列を取り出す
  mat3 matNormal = transpose(inverse(mat3(matModel)));

  // ワールド座標系の法線を計算
  vec3 worldNormal = normalize(matNormal * vNormal);

  // 環境光を設定
  vec3 lightColor = ambientLight;

  // ランバート反射による明るさを計算
  float cosTheta = max(dot(worldNormal, -light.direction),0);
  lightColor += light.color * cosTheta;
  
  // 物体の色とライトの色を乗算
  outColor.rgb = vColor.rgb * lightColor;
  
  // 不透明度は物体の値をそのまま使う
  outColor.a = vColor.a;

  outTexcoord = vTexcoord;
  gl_Position = matTRS * vec4(vPosition,1.0);
}