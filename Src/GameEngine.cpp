/**
* @file GameEngine.cpp
*/

#include "GameEngine.h"
#include "SceneManager.h"
#include "Audio/Audio.h"
#include "Audio/OpenGLGame_acf.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace 
{
	GameEngine* engine = nullptr;

	/**
	* OpenGLからのメッセージを処理する
	*
	* @param source    メッセージの発信者(OpenGL、Windows、シェーダーなど)
	* @param type      メッセージの種類(エラー、警告など)
	* @param id        メッセージを一位に識別する値
	* @param severity  メッセージの重要度(高、中、低、最低)
	* @param length    メッセージの文字数. 負数ならメッセージは0終端されている
	* @param message   メッセージ本体
	* @param userParam コールバック設定時に指定したポインタ
	*/
	// OpenGLから渡されたメッセージを標準エラー出力に表示する
	void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (length < 0)
		{
			std::cerr << message << "\n";
		}
		else
		{
			const std::string s(message, message + length);
			std::cerr << s << "\n";
		}
	}

	/**
	* 直方体(ライン)を作成(デバッグ用)
	*/
	void CreateBoxPrimitive(PrimitiveBuffer& buffer,
		const glm::vec3& min, const glm::vec3& max)
	{
		const int count = 4;
		std::vector<glm::vec3> positions(count * 2);
		std::vector<glm::vec3> normals(count * 2);
		std::vector<glm::vec2> texcoords(count * 2, glm::vec2(0));
		std::vector<glm::vec4> colors(count * 2, glm::vec4(0.8f, 0.4f, 0.1f, 1.0f));

		positions[0] = glm::vec3(min.x, min.y, min.z);
		positions[1] = glm::vec3(max.x, min.y, min.z);
		positions[2] = glm::vec3(max.x, min.y, max.z);
		positions[3] = glm::vec3(min.x, min.y, max.z);

		for (int i = 0; i < count; ++i)
		{
			positions[i + 4] = glm::vec3(positions[i].x, max.y, positions[i].z);
		}
		for (int i = 0; i < count * 2; ++i)
		{
			normals[i] = glm::normalize(positions[i]);
		}

		std::vector<GLushort> indices(count * 6);
		for (int i = 0; i < count; ++i)
		{
			indices[(i + count * 0) * 2] = i;
			indices[(i + count * 0) * 2 + 1] = (i + 1) % count;

			indices[(i + count * 1) * 2] = i + count;
			indices[(i + count * 1) * 2 + 1] = (i + 1) % count + count;

			indices[(i + count * 2) * 2] = i;
			indices[(i + count * 2) * 2 + 1] = i + count;
		}

		buffer.Add(positions.size(), positions.data(),
			colors.data(), texcoords.data(), normals.data(),
			indices.size(), indices.data(), "Collider(Box)", GL_LINES);
	}

	/**
	* 円柱(ライン)を作成(デバッグ用)
	*/
	void CreateCylinderPrimitive(PrimitiveBuffer& buffer,
		float top, float bottom, float radius, int count)
	{
		count = std::max(3, count);
		std::vector<glm::vec3> positions(count * 2);
		std::vector<glm::vec3> normals(count * 2);
		std::vector<glm::vec2> texcoords(count * 2, glm::vec2(0));
		std::vector<glm::vec4> colors(count * 2, glm::vec4(0.8f, 0.4f, 0.1f, 1.0f));

		for (int i = 0; i < count; ++i)
		{
			const float r = glm::radians(360.0f / static_cast<float>(count) * i);
			float x = std::cos(r);
			float z = -std::sin(r);
			normals[i] = glm::vec3(x, 0, z);
			normals[i + count] = glm::vec3(x, 0, z);

			x *= radius;
			z *= radius;
			positions[i] = glm::vec3(x, top, z);
			positions[i + count] = glm::vec3(x, bottom, z);
		}

		std::vector<GLushort> indices(count * 2 * 3);
		for (int i = 0; i < count; ++i)
		{
			indices[(i + count * 0) * 2] = i;
			indices[(i + count * 0) * 2 + 1] = (i + 1) % count;

			indices[(i + count * 1) * 2] = i + count;
			indices[(i + count * 1) * 2 + 1] = (i + count) % count + count;

			indices[(i + count * 2) * 2] = i;
			indices[(i + count * 2) * 2 + 1] = i + count;
		}
		buffer.Add(positions.size(), positions.data(),
			colors.data(), texcoords.data(), normals.data(),
			indices.size(), indices.data(),
			"Collider(Cylinder)", GL_LINES);
	}

}

/**
* エンジンの初期化
*/
bool GameEngine::Initialize()
{
	if (!engine)
	{
		engine = new GameEngine;
		if (!engine)
		{
			return false;
		}

		// GLFWの初期化
		if (glfwInit() != GLFW_TRUE)
		{
			return false;
		}

		// 古い機能を使えないようにする
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// 現在非推奨とされた機能が使えなくする
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		// GLのバージョン指定
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		// GLのバージョン指定
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		// OpenGLのデバッグ支援機能が有効にする
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		// 描画ウィンドウの作成
		GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLGame", nullptr, nullptr);

		if (!window)
		{
			glfwTerminate();
			return false;
		}

		// 指定したウィンドウに対応するOpenGLコンテキストを作成する
		glfwMakeContextCurrent(window);

		// OpenGL関数のアドレスを取得する

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			glfwTerminate();
			return false;
		}

		// コールバック関数を登録する
		glDebugMessageCallback(DebugCallback, nullptr);

		engine->window = window;
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		engine->windowSize = glm::vec2(w, h);

		engine->pipeline.reset(new ProgramPipeline(
			"Res/FragmentLighting.vert",
			"Res/FragmentLighting.frag"));
		engine->pipelineUI.reset(
			new ProgramPipeline("Res/Simple.vert", "Res/Simple.frag"));
		
		engine->sampler.reset(new Sampler(GL_REPEAT));
		engine->samplerUI.reset(new Sampler(GL_CLAMP_TO_EDGE));

		engine->pipelienShadow.reset(new ProgramPipeline("Res/Simple.vert", "Res/Simple.frag"));
		engine->samplerShadow.reset(new Sampler(GL_CLAMP_TO_EDGE));

		for (int layer = 0; layer < layerCount; ++layer)
		{
			engine->actors[layer].reserve(1000);
		}
		
		engine->newActors.reserve(1000);
		engine->primitiveBuffer.reset(new PrimitiveBuffer(1'000'000, 4'000'000));
		engine->textureBuffer.reserve(1000);

		// コライダー表示用データを作成(デバッグ用)

		// UIシェーダーを流用
		engine->pipelineCollider = engine->pipelineUI;
		const std::vector<uint32_t> imageCollider(4 * 4, 0xff'10'80'f0);
		engine->texCollider = std::make_shared<Texture>(
			"Debug(Collider)", 4, 4, imageCollider.data(), GL_RGBA, GL_UNSIGNED_BYTE);

		//CreateBoxPrimitive(*engine->primitiveBuffer, glm::vec3(-1.0f), glm::vec3(1.0f));
		//CreateCylinderPrimitive(*engine->primitiveBuffer, 1, 0, 1, 8);

		// スプライト描画オブジェクトを初期化
		engine->spriteRenderer.Allocate(1000);

		// カメラのアスペクト比を設定
		Camera& camera = engine->GetCamera();
		camera.aspectRatio = 
			engine->windowSize.x / engine->windowSize.y;

		// FBOを初期化する
		engine->fboShadow.reset(new FramebufferObject(4096, 4096, FboType::depth));
		if (!engine->fboShadow || !engine->fboShadow->GetId())
			return false;
		

		// ImGuiの初期化
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char glsl_version[] = "#version 450";
		ImGui_ImplOpenGL3_Init(glsl_version);

		// ImGuiの入出力を指定
		ImGuiIO& io = ImGui::GetIO();

		// デフォルトフォントを指定
		// ImGui標準のフォントサイズ(ピクセル)
		const float defauktFontPixels = 13.0f;
		const float fontPixels = 32.0f;
		ImFont* font = io.Fonts->AddFontFromFileTTF(
			"Res/Font/komorebi-gothic.ttf",
			fontPixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
		if (font)
		{
			io.FontDefault = font;
			io.FontGlobalScale = defauktFontPixels / fontPixels;
			// フォント画像を作成
			io.Fonts->Build();
		}
		// 音声を初期化
		Audio::Initialize("Res/Audio/WorkUnit_0/OpenGLGame.acf",
			CRI_OPENGLGAME_ACF_DSPSETTING_DSPBUSSETTING_0);
		Audio& audio = Audio::Get();
		audio.Load(0, "Res/Audio/WorkUnit_0/SE.acb", nullptr);
		audio.Load(1, "Res/Audio/WorkUnit_0/BGM.acb", "Res/Audio/WorkUnit_0/BGM.awb");
		
	}
	// シーンマネージャーの初期化
	if (!SceneManager::Initialize())
	{
		return false;
	}

	// 描画データを追加する
	
	// 地面
	engine->primitiveBuffer->AddFromObjFile("Res/Ground.obj");	
	// 木
	engine->primitiveBuffer->AddFromObjFile("Res/Tree.obj");
	// 戦車(Tiger)
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/Tiger_bottom.obj");
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/Tiger_top.obj");	
	// 戦車(T34)
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/T34_bottom.obj");
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/T34_top.obj");
	// 家1
	engine->primitiveBuffer->AddFromObjFile("Res/House1.obj");
	// 弾
	engine->primitiveBuffer->AddFromObjFile("Res/Bullet.obj");
	// 通行止め
	engine->primitiveBuffer->AddFromObjFile("Res/RoadCloased.obj");
	// 家2
	engine->primitiveBuffer->AddFromObjFile("Res/House/Brick_House.obj");
	// 家3
	engine->primitiveBuffer->AddFromObjFile("Res/House/House1.obj");
	// 家4
	engine->primitiveBuffer->AddFromObjFile("Res/House/House2.obj");
	// 家5
	engine->primitiveBuffer->AddFromObjFile("Res/House/House3.obj");
	// 家6
	engine->primitiveBuffer->AddFromObjFile("Res/House/House4.obj");
	// 家7
	engine->primitiveBuffer->AddFromObjFile("Res/House/House5.obj");
	// 家8
	engine->primitiveBuffer->AddFromObjFile("Res/House/House6.obj");
	// 家9
	engine->primitiveBuffer->AddFromObjFile("Res/House/House7.obj");
	// 家10
	return true;
}

/**
* エンジンの終了
*/
void GameEngine::Finalize()
{
	if (engine)
	{
		// 音声終了
		Audio::Finalize();

		// シーンマネージャーの終了
		SceneManager::Get().Finalize();

		// ImGuiの終了
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// GLFWの終了
		glfwTerminate();

		delete engine;
		engine = nullptr;
	}
}

/**
* ゲームエンジンを取得する
*/
GameEngine& GameEngine::Get()
{
	return *engine;
}

/**
* アクター取得
* 
* @param layer アクターのlayer
*/
ActorList& GameEngine::GetActors(Layer layer)
{
	return engine->actors[static_cast<int>(layer)];
}

/**
* UI取得
*/
std::vector<std::shared_ptr<UI>>& GameEngine::GetUI()
{
	return ui;
}

/**
* アクター配列を更新する
*/
void GameEngine::UpdateActors(float deltaTime)
{
	for (int layer = 0; layer < layerCount; ++layer)
	{
		ActorList& actors = this->actors[layer];
		// アクターの状態を更新する
		for (int i = 0; i < actors.size(); ++i)
		{
			// アクターの寿命を減らす
			if (actors[i]->lifespan > 0)
			{
				actors[i]->lifespan -= deltaTime;

				// 寿命の尽きたアクターを「消去待ち」状態にする
				if (actors[i]->lifespan <= 0)
				{
					actors[i]->isDead = true;
					// 消去待ちのアクターは更新をスキップ
					continue;
				}
			}

			actors[i]->OnUpdate(deltaTime);

			// 速度に重力加速度を加える
			if (!actors[i]->isStatic && actors[i]->useGravity)
			{
				actors[i]->velocity.y += -9.8f * actors[i]->gravityScale * deltaTime;
			}

			// アクターの位置を更新する
			actors[i]->position += actors[i]->velocity * deltaTime;
		}
	}
}

/**
* UI更新
*/
void GameEngine::UpdateUI(float deltaTime)
{
	for (int i = 0; i < ui.size(); ++i)
	{
		ui[i]->OnUpdate(deltaTime);
	}
}

/**
* アクターの衝突判定を行う
*/
void GameEngine::UpdatePhysics(float deltaTime)
{
	ActorList& actors = GetActors(Layer::Default);

	// 非スタティックなアクターをリストアップ
	ActorList partitionedActors = actors;
	const auto itrEndA = std::partition(
		partitionedActors.begin(), partitionedActors.end(),
		[](const ActorList::value_type& e) {return !e->isStatic; });
	const auto itrEndB = partitionedActors.end();

	// 衝突情報の配列を用意する
	std::vector<Contact> contacts;
	contacts.reserve(actors.size());

	// アクターの衝突判定を行う
	for (auto itrA = partitionedActors.begin();itrA!=itrEndA;++itrA)
	{
		std::shared_ptr<Actor>& a = *itrA;
		for (auto itrB = itrA + 1; itrB != itrEndB; ++itrB)
		{
			std::shared_ptr<Actor>& b = *itrB;
			
			// 消去待ちアクターは衝突しない
			if (a->isDead)
			{
				break;
			}
			else if (b->isDead)
			{
				continue;
			}

			Contact contact;
			if (DetectCollision(*a, *b, contact))
			{
				// 配列の中に、作成したコンタクト構造体と似ているものがあるか調べる
				auto itr = std::find_if(contacts.begin(), contacts.end(),
					[&contact](const Contact& c) { return Equal(contact, c); });

				// 似ているコンタクト構造体が見つからなければ、作成した構造体を配列に追加する
				if (itr == contacts.end())
				{
					contacts.push_back(contact);
				}
				else
				{
					// 似ている構造体が見つかった場合、浸透距離が長いほうを残す
					if (contact.penLength > itr->penLength)
					{
						*itr = contact;
					}
				}
			}
		}
	}

	// 当たり判定 
	for (int i = 0; i < contacts.size(); ++i)
	{
		Contact& c = contacts[i];

		// 衝突方向を逆にした衝突判定情報を作る
		Contact contactBtoA;
		contactBtoA.a = c.a;
		contactBtoA.b = c.b;
		contactBtoA.velocityA = c.velocityB;
		contactBtoA.velocityB = c.velocityA;
		contactBtoA.penetration = -c.penetration;
		contactBtoA.normal = -c.normal;
		contactBtoA.position = c.position;
		contactBtoA.penLength = c.penLength;

		Box boxA = static_cast<Box&>(*contactBtoA.a->collider);
		Box boxB = static_cast<Box&>(*contactBtoA.b->collider);

		if (boxA.isTrigger || boxB.isTrigger)
		{
			c.a->OnTrigger(c);
			c.b->OnTrigger(contactBtoA);
		}
		else
		{
			// 衝突処理関数を呼び出す
			c.a->OnCollision(c);
			c.b->OnCollision(contactBtoA);
			// 重なりを解決する
			SolveContact(c);
		}
	}
}

/**
* カメラの更新
*/
void GameEngine::UpdateCamera()
{
	mainCamera.Update();
}

/**
* 新しいフレームを開始する
*/
void GameEngine::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 音声の更新
	Audio::Get().Update();
}

/**
* アクターを全て消去
*/
void GameEngine::ActorsClear()
{
	for (int layer = 0; layer < layerCount; ++layer)
	{
		for (int i = 0; i < actors[layer].size(); ++i)
		{
			actors[layer][i]->isDead = true;
		}
	}
}

/**
* ゲームエンジンを更新する(後処理)
*/
void GameEngine::PostUpdateActors()
{
	// 新規に作成されたアクターをアクター配列に追加する
	for (int i = 0; i < newActors.size(); ++i)
	{
		const int layer = static_cast<int>(newActors[i]->layer);
		if (layer >= 0 && layer < layerCount)
		{
			actors[layer].push_back(newActors[i]);
		}
	}

	// 新規アクター配列を空にする
	newActors.clear();
}

/**
* 消去待ちのアクターを削除する
*/
void GameEngine::RemoveDeadActors()
{
	for (int layer = 0; layer < layerCount; ++layer)
	{
		ActorList& a = actors[layer];
		a.erase(std::remove_if(a.begin(), a.end(),
			[](std::shared_ptr<Actor>& actor) {return actor->isDead; }),
			a.end());
	}
}

/**
* デフォルトアクターを描画する
*/
void GameEngine::RenderDefault()
{
	// 平行光源の向き
	const glm::vec3 lightDirection = glm::normalize(glm::vec4(3, -2, -2, 0));

	// 影用プロジェクション行列を作成
	const glm::mat4 matShadowProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 10.0f, 300.0f);

	// 影用ビュー行列を作成
	const glm::vec3 shadowCameraTarget = mainCamera.target;
	const glm::vec3 shadowCameraPos =
		shadowCameraTarget - glm::vec3(lightDirection) * 100.0f;
	const glm::mat4 matShadowView =
		glm::lookAt(shadowCameraPos, shadowCameraTarget, glm::vec3(0, 1, 0));

	// 影の描画
	{
		// 描画先を影描画用FBOに変更
		fboShadow->Bind();

		// 深度テストを有効にする
		glEnable(GL_DEPTH_TEST);
		// 裏面カリングを有効にする
		glEnable(GL_CULL_FACE);
		// アルファブレンドを無効にする
		glDisable(GL_BLEND);

		glClear(GL_DEPTH_BUFFER_BIT);

		primitiveBuffer->BindVertexArray();
		pipeline->Bind();
		
		// アクターを描画
		const int layer = static_cast<int>(Layer::Default);
		for (auto& e : actors[layer])
		{
			e->m_Draw(*pipeline, matShadowProj, matShadowView);
		}

		// デフォルトのフレームバッファに戻す
		fboShadow->Unbind();
	}

	//
	// 通常の描画
	//
		
	// デフォルトフレームバッファのビューポートを設定
	glViewport(0, 0,
		static_cast<GLsizei>(windowSize.x),
		static_cast<GLsizei>(windowSize.y));

	// 深度バッファを有効にする
	glEnable(GL_DEPTH_TEST);
	// 0.0f～1.0fで色を指定
	/*
	* 空色
	* 0.2f, 1.0f, 1.0f
	*/
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
	// バックバッファを消去するときに使われる色を指定する
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	primitiveBuffer->BindVertexArray();
	pipeline->Bind();
	glBindSampler(0, sampler->GetSampler());

	glm::mat4 matT = glm::mat4(1);
	matT[3] = glm::vec4(-0.3, -0.5, 0.0, 1.0);
	glm::mat4 matS = glm::mat4(1);
	matS[0][0] = 0.5;
	matS[1][1] = 1.5;
	glm::mat4 matR = glm::mat4(1);

	// プロジェクション行列を作成
	const glm::vec2 windowSize = GetWindowSize();
	const float aspectRatio = windowSize.x / windowSize.y;
	mainCamera.target = (mainCamera.rotation_y) * glm::vec4(mainCamera.rotation_y_v, 1);
	mainCamera.target += mainCamera.position;
	const glm::mat4 matProj =
		glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

	// ビュー行列を作成
	const glm::mat4 matView =
		glm::lookAt(mainCamera.position, mainCamera.target, glm::vec3(0, 0.1, 0));

	// NDC座標系からテクスチャ座標系への座標変換行列
	const glm::mat4 matShadowTex = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	// 影行列をGPUメモリにコピーする
	const glm::mat4 matShadow = matShadowTex * matShadowProj * matShadowView;
	const GLint locMatShadow = 100;
	pipeline->SetFragUniform(locMatShadow, matShadow);

	// 影描画用の深度テクスチャを割り当てる
	fboShadow->BindDepthTexture(1);

	// コライダーを表示する(デバッグ用)
	if (showCollider)
	{
		const Primitive& primBox = GetPrimitive("Collider(Box)");
		const Primitive& primCylinder = GetPrimitive("Collider(Cylinder)");
		pipelineCollider->Bind();
		texCollider->Bind(0);

		for (auto& e : actors[static_cast<int>(Layer::Default)])
		{
			if (!e->collider)
			{
				continue;
			}
			switch (e->collider->GetShapeType())
			{
			case ShapeTye::box:
			{
				const Box& box = static_cast<Box&>(*e->collider);
				const glm::vec3 offset = (box.min + box.max) * 0.5f;
				const glm::vec3 scale = (box.max - box.min) * 0.5f;
				const glm::mat4 matTRS =
					glm::translate(glm::mat4(1), e->position + offset) *
					glm::scale(glm::mat4(1), scale);
				pipelineCollider->SetVertUniform(0, matProj* matView* matTRS);
				primBox.Draw();
				break;
			}

			case ShapeTye::cylinder:
			{
				const Cylinder& cylinder = static_cast<Cylinder&>(*e->collider);
				const glm::vec3 offset = cylinder.bottom;
				const glm::vec3 scale = glm::vec3(cylinder.radius, 
					cylinder.height, cylinder.radius);
				const glm::mat4 matTRS =
					glm::translate(glm::mat4(1), e->position + offset) *
					glm::scale(glm::mat4(1), scale);
				pipelineCollider->SetVertUniform(0, matProj * matView * matTRS);
				primCylinder.Draw();
				break;
			}
			}
		}
		texCollider->Unbind(0);
		pipelineCollider->Unbind();
	}

	// アクターの表示
	pipeline->Bind();
	const int layer = static_cast<int>(Layer::Default);
	ActorList& defaultActors = actors[layer];
	for (int i = 0; i < defaultActors.size(); ++i)
	{
		defaultActors[i]->m_Draw(*pipeline, matProj, matView);
	}
}

/**
* スプライトを描画
*/
void GameEngine::RenderSprite()
{
	//fbo->Bind();
	const glm::mat4& matProj = mainCamera.GetProjectMatrix();
	const glm::mat4 matView = mainCamera.GetViewMatrix();
	spriteRenderer.Update(GetActors(Layer::Sprite), matView);
	spriteRenderer.Draw(pipelineUI, matProj * matView);
}

/**
* UIアクターを描画する
*/
void GameEngine::RenderUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
* 描画後の後始末をする
*/
void GameEngine::PostRender()
{
	// テクスチャの割り当て解除
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindSampler(0, 0);
	pipeline->Unbind();
	primitiveBuffer->UnbindVectexArray();

}

/**
* プリミティブバッファの取得
*/
PrimitiveBuffer& GameEngine::GetPrimitiveBuffer()
{
	return *engine->primitiveBuffer;
}

/**
* プリミティブ取得
* 
* @param n プリミティブの番号
* 
* @retval n番目のプリミティブ
*/
const Primitive& GameEngine::GetPrimitive(int n)
{
	return engine->primitiveBuffer->Get(n);
}

/*
* 名前の一致するプリミティブを取得する
* 
* @param filename プリミティブ名
* 
* @retval filenameと名前が一致するプリミティブ
*/
const Primitive& GameEngine::GetPrimitive(const char* filename) const
{
	return primitiveBuffer->Find(filename);
}

/**
* テクスチャを読み込む
*/
std::shared_ptr<Texture> GameEngine::LoadTexture(const char* filename)
{
	// 同じファイル名のテクスチャが登録済みか調べる
	TextureBuffer::iterator itr = textureBuffer.find(filename);

	// itrとend()が異なる場合、ファイルは読み込み済みなので、
	// 作成済みのテクスチャを返す
	if (itr != textureBuffer.end())
		return itr->second;

	// itrとendが等しい場合、ファイルはまだ読み込まれていないので、
	// ファイル名から新しいテクスチャを作成する
	std::shared_ptr<Texture> tex(new Texture(filename));

	// 作成したテクスチャをテクスチャバッファに登録する
	textureBuffer.insert(std::make_pair(filename, tex));

	// 作成したテクスチャを返す
	return tex;
}

/**
* この間数がtrueを返すとウィンドウを閉じる
*/
bool GameEngine::WindowShouldClose()const
{
	return glfwWindowShouldClose(engine->window);
}

/**
* キーが押されたらtrue,押されていなければflase
* 
* @param key 調べるキー
*/
bool GameEngine::GetKey(int key)const
{
	return glfwGetKey(engine->window, key) == GLFW_PRESS;
}

/**
* マウスの入力
* 
* @param key 調べるキー
*/
int GameEngine::GetMouseButton(int key)const
{
	return glfwGetMouseButton(engine->window, key);
}

/**
* ウィンドウサイズを返す
*/
glm::vec2 GameEngine::GetWindowSize()const
{
	return engine->windowSize;
}

/**
* フロントバッファとバックバッファを交換する
*/
void GameEngine::SwapBuffers()const
{
	glfwSwapBuffers(engine->window);
	glfwPollEvents();
}

/**
* 時間の取得
*/
double GameEngine::GetTime()
{
	return glfwGetTime();
}

/**
* カーソル位置の取得
*/
glm::vec2 GameEngine::GetCursorPosition()
{
	double x, y;
	glfwGetCursorPos(engine->window, &x, &y);

	glm::vec2 cursorPos = glm::vec2(x, y);
	return cursorPos;
}

/**
* カーソルの設定
*
* @param cursorType 設定するカーソルの状態
*/
void GameEngine::SetCursorType(int cursorType)
{
	glfwSetInputMode(engine->window, GLFW_CURSOR, cursorType);
}

/**
* カーソルの位置の代入
*
* @param cursorpos 設定するカーソルの位置
*/
void GameEngine::SetCursorPos(glm::vec2 cursorpos)
{
	double x, y;
	x = cursorpos.x;
	y = cursorpos.y;

	glfwSetCursorPos(engine->window, x, y);
}

/**
* 背景色の変更
* 
* @param bgcolor 背景カラー
*/
void GameEngine::SetglClearColor(glm::vec3 bgcolor)
{
	bgColor = bgcolor;
}

/**
* コライダーの表示切替
* 
* @param isShowCollider true コライダーを表示,false コライダー非表示
*/
void GameEngine::SetShowCollider(bool isShowCollider)
{
	showCollider = isShowCollider;
}

/**
* ウィンドウを閉じる
*/
void GameEngine::WindowClose()
{
	glfwSetWindowShouldClose(engine->window, true);
}

/**
* カメラの取得
*/
Camera& GameEngine::GetCamera()
{
	return mainCamera;
}

const Camera& GameEngine::GetCamera() const
{
	return mainCamera;
}

/**
* パイプラインの取得
*/
std::shared_ptr<ProgramPipeline>& GameEngine::GetPipeline()
{
	return pipeline;
}

/**
* サンプラーの取得
*/
std::shared_ptr<Sampler>& GameEngine::GetSampler()
{
	return sampler;
}
