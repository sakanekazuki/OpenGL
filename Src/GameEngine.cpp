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
	* OpenGL����̃��b�Z�[�W����������
	*
	* @param source    ���b�Z�[�W�̔��M��(OpenGL�AWindows�A�V�F�[�_�[�Ȃ�)
	* @param type      ���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)
	* @param id        ���b�Z�[�W����ʂɎ��ʂ���l
	* @param severity  ���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)
	* @param length    ���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă���
	* @param message   ���b�Z�[�W�{��
	* @param userParam �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
	*/
	// OpenGL����n���ꂽ���b�Z�[�W��W���G���[�o�͂ɕ\������
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
	* ������(���C��)���쐬(�f�o�b�O�p)
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
	* �~��(���C��)���쐬(�f�o�b�O�p)
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
* �G���W���̏�����
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

		// GLFW�̏�����
		if (glfwInit() != GLFW_TRUE)
		{
			return false;
		}

		// �Â��@�\���g���Ȃ��悤�ɂ���
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// ���ݔ񐄏��Ƃ��ꂽ�@�\���g���Ȃ�����
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		// GL�̃o�[�W�����w��
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		// GL�̃o�[�W�����w��
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		// OpenGL�̃f�o�b�O�x���@�\���L���ɂ���
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		// �`��E�B���h�E�̍쐬
		GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLGame", nullptr, nullptr);

		if (!window)
		{
			glfwTerminate();
			return false;
		}

		// �w�肵���E�B���h�E�ɑΉ�����OpenGL�R���e�L�X�g���쐬����
		glfwMakeContextCurrent(window);

		// OpenGL�֐��̃A�h���X���擾����

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			glfwTerminate();
			return false;
		}

		// �R�[���o�b�N�֐���o�^����
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

		// �R���C�_�[�\���p�f�[�^���쐬(�f�o�b�O�p)

		// UI�V�F�[�_�[�𗬗p
		engine->pipelineCollider = engine->pipelineUI;
		const std::vector<uint32_t> imageCollider(4 * 4, 0xff'10'80'f0);
		engine->texCollider = std::make_shared<Texture>(
			"Debug(Collider)", 4, 4, imageCollider.data(), GL_RGBA, GL_UNSIGNED_BYTE);

		//CreateBoxPrimitive(*engine->primitiveBuffer, glm::vec3(-1.0f), glm::vec3(1.0f));
		//CreateCylinderPrimitive(*engine->primitiveBuffer, 1, 0, 1, 8);

		// �X�v���C�g�`��I�u�W�F�N�g��������
		engine->spriteRenderer.Allocate(1000);

		// �J�����̃A�X�y�N�g���ݒ�
		Camera& camera = engine->GetCamera();
		camera.aspectRatio = 
			engine->windowSize.x / engine->windowSize.y;

		// FBO������������
		engine->fboShadow.reset(new FramebufferObject(4096, 4096, FboType::depth));
		if (!engine->fboShadow || !engine->fboShadow->GetId())
			return false;
		

		// ImGui�̏�����
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char glsl_version[] = "#version 450";
		ImGui_ImplOpenGL3_Init(glsl_version);

		// ImGui�̓��o�͂��w��
		ImGuiIO& io = ImGui::GetIO();

		// �f�t�H���g�t�H���g���w��
		// ImGui�W���̃t�H���g�T�C�Y(�s�N�Z��)
		const float defauktFontPixels = 13.0f;
		const float fontPixels = 32.0f;
		ImFont* font = io.Fonts->AddFontFromFileTTF(
			"Res/Font/komorebi-gothic.ttf",
			fontPixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
		if (font)
		{
			io.FontDefault = font;
			io.FontGlobalScale = defauktFontPixels / fontPixels;
			// �t�H���g�摜���쐬
			io.Fonts->Build();
		}
		// ������������
		Audio::Initialize("Res/Audio/WorkUnit_0/OpenGLGame.acf",
			CRI_OPENGLGAME_ACF_DSPSETTING_DSPBUSSETTING_0);
		Audio& audio = Audio::Get();
		audio.Load(0, "Res/Audio/WorkUnit_0/SE.acb", nullptr);
		audio.Load(1, "Res/Audio/WorkUnit_0/BGM.acb", "Res/Audio/WorkUnit_0/BGM.awb");
		
	}
	// �V�[���}�l�[�W���[�̏�����
	if (!SceneManager::Initialize())
	{
		return false;
	}

	// �`��f�[�^��ǉ�����
	
	// �n��
	engine->primitiveBuffer->AddFromObjFile("Res/Ground.obj");	
	// ��
	engine->primitiveBuffer->AddFromObjFile("Res/Tree.obj");
	// ���(Tiger)
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/Tiger_bottom.obj");
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/Tiger_top.obj");	
	// ���(T34)
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/T34_bottom.obj");
	engine->primitiveBuffer->AddFromObjFile("Res/Tank/T34_top.obj");
	// ��1
	engine->primitiveBuffer->AddFromObjFile("Res/House1.obj");
	// �e
	engine->primitiveBuffer->AddFromObjFile("Res/Bullet.obj");
	// �ʍs�~��
	engine->primitiveBuffer->AddFromObjFile("Res/RoadCloased.obj");
	// ��2
	engine->primitiveBuffer->AddFromObjFile("Res/House/Brick_House.obj");
	// ��3
	engine->primitiveBuffer->AddFromObjFile("Res/House/House1.obj");
	// ��4
	engine->primitiveBuffer->AddFromObjFile("Res/House/House2.obj");
	// ��5
	engine->primitiveBuffer->AddFromObjFile("Res/House/House3.obj");
	// ��6
	engine->primitiveBuffer->AddFromObjFile("Res/House/House4.obj");
	// ��7
	engine->primitiveBuffer->AddFromObjFile("Res/House/House5.obj");
	// ��8
	engine->primitiveBuffer->AddFromObjFile("Res/House/House6.obj");
	// ��9
	engine->primitiveBuffer->AddFromObjFile("Res/House/House7.obj");
	// ��10
	return true;
}

/**
* �G���W���̏I��
*/
void GameEngine::Finalize()
{
	if (engine)
	{
		// �����I��
		Audio::Finalize();

		// �V�[���}�l�[�W���[�̏I��
		SceneManager::Get().Finalize();

		// ImGui�̏I��
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// GLFW�̏I��
		glfwTerminate();

		delete engine;
		engine = nullptr;
	}
}

/**
* �Q�[���G���W�����擾����
*/
GameEngine& GameEngine::Get()
{
	return *engine;
}

/**
* �A�N�^�[�擾
* 
* @param layer �A�N�^�[��layer
*/
ActorList& GameEngine::GetActors(Layer layer)
{
	return engine->actors[static_cast<int>(layer)];
}

/**
* UI�擾
*/
std::vector<std::shared_ptr<UI>>& GameEngine::GetUI()
{
	return ui;
}

/**
* �A�N�^�[�z����X�V����
*/
void GameEngine::UpdateActors(float deltaTime)
{
	for (int layer = 0; layer < layerCount; ++layer)
	{
		ActorList& actors = this->actors[layer];
		// �A�N�^�[�̏�Ԃ��X�V����
		for (int i = 0; i < actors.size(); ++i)
		{
			// �A�N�^�[�̎��������炷
			if (actors[i]->lifespan > 0)
			{
				actors[i]->lifespan -= deltaTime;

				// �����̐s�����A�N�^�[���u�����҂��v��Ԃɂ���
				if (actors[i]->lifespan <= 0)
				{
					actors[i]->isDead = true;
					// �����҂��̃A�N�^�[�͍X�V���X�L�b�v
					continue;
				}
			}

			actors[i]->OnUpdate(deltaTime);

			// ���x�ɏd�͉����x��������
			if (!actors[i]->isStatic && actors[i]->useGravity)
			{
				actors[i]->velocity.y += -9.8f * actors[i]->gravityScale * deltaTime;
			}

			// �A�N�^�[�̈ʒu���X�V����
			actors[i]->position += actors[i]->velocity * deltaTime;
		}
	}
}

/**
* UI�X�V
*/
void GameEngine::UpdateUI(float deltaTime)
{
	for (int i = 0; i < ui.size(); ++i)
	{
		ui[i]->OnUpdate(deltaTime);
	}
}

/**
* �A�N�^�[�̏Փ˔�����s��
*/
void GameEngine::UpdatePhysics(float deltaTime)
{
	ActorList& actors = GetActors(Layer::Default);

	// ��X�^�e�B�b�N�ȃA�N�^�[�����X�g�A�b�v
	ActorList partitionedActors = actors;
	const auto itrEndA = std::partition(
		partitionedActors.begin(), partitionedActors.end(),
		[](const ActorList::value_type& e) {return !e->isStatic; });
	const auto itrEndB = partitionedActors.end();

	// �Փˏ��̔z���p�ӂ���
	std::vector<Contact> contacts;
	contacts.reserve(actors.size());

	// �A�N�^�[�̏Փ˔�����s��
	for (auto itrA = partitionedActors.begin();itrA!=itrEndA;++itrA)
	{
		std::shared_ptr<Actor>& a = *itrA;
		for (auto itrB = itrA + 1; itrB != itrEndB; ++itrB)
		{
			std::shared_ptr<Actor>& b = *itrB;
			
			// �����҂��A�N�^�[�͏Փ˂��Ȃ�
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
				// �z��̒��ɁA�쐬�����R���^�N�g�\���̂Ǝ��Ă�����̂����邩���ׂ�
				auto itr = std::find_if(contacts.begin(), contacts.end(),
					[&contact](const Contact& c) { return Equal(contact, c); });

				// ���Ă���R���^�N�g�\���̂�������Ȃ���΁A�쐬�����\���̂�z��ɒǉ�����
				if (itr == contacts.end())
				{
					contacts.push_back(contact);
				}
				else
				{
					// ���Ă���\���̂����������ꍇ�A�Z�������������ق����c��
					if (contact.penLength > itr->penLength)
					{
						*itr = contact;
					}
				}
			}
		}
	}

	// �����蔻�� 
	for (int i = 0; i < contacts.size(); ++i)
	{
		Contact& c = contacts[i];

		// �Փ˕������t�ɂ����Փ˔���������
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
			// �Փˏ����֐����Ăяo��
			c.a->OnCollision(c);
			c.b->OnCollision(contactBtoA);
			// �d�Ȃ����������
			SolveContact(c);
		}
	}
}

/**
* �J�����̍X�V
*/
void GameEngine::UpdateCamera()
{
	mainCamera.Update();
}

/**
* �V�����t���[�����J�n����
*/
void GameEngine::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// �����̍X�V
	Audio::Get().Update();
}

/**
* �A�N�^�[��S�ď���
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
* �Q�[���G���W�����X�V����(�㏈��)
*/
void GameEngine::PostUpdateActors()
{
	// �V�K�ɍ쐬���ꂽ�A�N�^�[���A�N�^�[�z��ɒǉ�����
	for (int i = 0; i < newActors.size(); ++i)
	{
		const int layer = static_cast<int>(newActors[i]->layer);
		if (layer >= 0 && layer < layerCount)
		{
			actors[layer].push_back(newActors[i]);
		}
	}

	// �V�K�A�N�^�[�z�����ɂ���
	newActors.clear();
}

/**
* �����҂��̃A�N�^�[���폜����
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
* �f�t�H���g�A�N�^�[��`�悷��
*/
void GameEngine::RenderDefault()
{
	// ���s�����̌���
	const glm::vec3 lightDirection = glm::normalize(glm::vec4(3, -2, -2, 0));

	// �e�p�v���W�F�N�V�����s����쐬
	const glm::mat4 matShadowProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 10.0f, 300.0f);

	// �e�p�r���[�s����쐬
	const glm::vec3 shadowCameraTarget = mainCamera.target;
	const glm::vec3 shadowCameraPos =
		shadowCameraTarget - glm::vec3(lightDirection) * 100.0f;
	const glm::mat4 matShadowView =
		glm::lookAt(shadowCameraPos, shadowCameraTarget, glm::vec3(0, 1, 0));

	// �e�̕`��
	{
		// �`�����e�`��pFBO�ɕύX
		fboShadow->Bind();

		// �[�x�e�X�g��L���ɂ���
		glEnable(GL_DEPTH_TEST);
		// ���ʃJ�����O��L���ɂ���
		glEnable(GL_CULL_FACE);
		// �A���t�@�u�����h�𖳌��ɂ���
		glDisable(GL_BLEND);

		glClear(GL_DEPTH_BUFFER_BIT);

		primitiveBuffer->BindVertexArray();
		pipeline->Bind();
		
		// �A�N�^�[��`��
		const int layer = static_cast<int>(Layer::Default);
		for (auto& e : actors[layer])
		{
			e->m_Draw(*pipeline, matShadowProj, matShadowView);
		}

		// �f�t�H���g�̃t���[���o�b�t�@�ɖ߂�
		fboShadow->Unbind();
	}

	//
	// �ʏ�̕`��
	//
		
	// �f�t�H���g�t���[���o�b�t�@�̃r���[�|�[�g��ݒ�
	glViewport(0, 0,
		static_cast<GLsizei>(windowSize.x),
		static_cast<GLsizei>(windowSize.y));

	// �[�x�o�b�t�@��L���ɂ���
	glEnable(GL_DEPTH_TEST);
	// 0.0f�`1.0f�ŐF���w��
	/*
	* ��F
	* 0.2f, 1.0f, 1.0f
	*/
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
	// �o�b�N�o�b�t�@����������Ƃ��Ɏg����F���w�肷��
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

	// �v���W�F�N�V�����s����쐬
	const glm::vec2 windowSize = GetWindowSize();
	const float aspectRatio = windowSize.x / windowSize.y;
	mainCamera.target = (mainCamera.rotation_y) * glm::vec4(mainCamera.rotation_y_v, 1);
	mainCamera.target += mainCamera.position;
	const glm::mat4 matProj =
		glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

	// �r���[�s����쐬
	const glm::mat4 matView =
		glm::lookAt(mainCamera.position, mainCamera.target, glm::vec3(0, 0.1, 0));

	// NDC���W�n����e�N�X�`�����W�n�ւ̍��W�ϊ��s��
	const glm::mat4 matShadowTex = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	// �e�s���GPU�������ɃR�s�[����
	const glm::mat4 matShadow = matShadowTex * matShadowProj * matShadowView;
	const GLint locMatShadow = 100;
	pipeline->SetFragUniform(locMatShadow, matShadow);

	// �e�`��p�̐[�x�e�N�X�`�������蓖�Ă�
	fboShadow->BindDepthTexture(1);

	// �R���C�_�[��\������(�f�o�b�O�p)
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

	// �A�N�^�[�̕\��
	pipeline->Bind();
	const int layer = static_cast<int>(Layer::Default);
	ActorList& defaultActors = actors[layer];
	for (int i = 0; i < defaultActors.size(); ++i)
	{
		defaultActors[i]->m_Draw(*pipeline, matProj, matView);
	}
}

/**
* �X�v���C�g��`��
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
* UI�A�N�^�[��`�悷��
*/
void GameEngine::RenderUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
* �`���̌�n��������
*/
void GameEngine::PostRender()
{
	// �e�N�X�`���̊��蓖�ĉ���
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindSampler(0, 0);
	pipeline->Unbind();
	primitiveBuffer->UnbindVectexArray();

}

/**
* �v���~�e�B�u�o�b�t�@�̎擾
*/
PrimitiveBuffer& GameEngine::GetPrimitiveBuffer()
{
	return *engine->primitiveBuffer;
}

/**
* �v���~�e�B�u�擾
* 
* @param n �v���~�e�B�u�̔ԍ�
* 
* @retval n�Ԗڂ̃v���~�e�B�u
*/
const Primitive& GameEngine::GetPrimitive(int n)
{
	return engine->primitiveBuffer->Get(n);
}

/*
* ���O�̈�v����v���~�e�B�u���擾����
* 
* @param filename �v���~�e�B�u��
* 
* @retval filename�Ɩ��O����v����v���~�e�B�u
*/
const Primitive& GameEngine::GetPrimitive(const char* filename) const
{
	return primitiveBuffer->Find(filename);
}

/**
* �e�N�X�`����ǂݍ���
*/
std::shared_ptr<Texture> GameEngine::LoadTexture(const char* filename)
{
	// �����t�@�C�����̃e�N�X�`�����o�^�ς݂����ׂ�
	TextureBuffer::iterator itr = textureBuffer.find(filename);

	// itr��end()���قȂ�ꍇ�A�t�@�C���͓ǂݍ��ݍς݂Ȃ̂ŁA
	// �쐬�ς݂̃e�N�X�`����Ԃ�
	if (itr != textureBuffer.end())
		return itr->second;

	// itr��end���������ꍇ�A�t�@�C���͂܂��ǂݍ��܂�Ă��Ȃ��̂ŁA
	// �t�@�C��������V�����e�N�X�`�����쐬����
	std::shared_ptr<Texture> tex(new Texture(filename));

	// �쐬�����e�N�X�`�����e�N�X�`���o�b�t�@�ɓo�^����
	textureBuffer.insert(std::make_pair(filename, tex));

	// �쐬�����e�N�X�`����Ԃ�
	return tex;
}

/**
* ���̊Ԑ���true��Ԃ��ƃE�B���h�E�����
*/
bool GameEngine::WindowShouldClose()const
{
	return glfwWindowShouldClose(engine->window);
}

/**
* �L�[�������ꂽ��true,������Ă��Ȃ����flase
* 
* @param key ���ׂ�L�[
*/
bool GameEngine::GetKey(int key)const
{
	return glfwGetKey(engine->window, key) == GLFW_PRESS;
}

/**
* �}�E�X�̓���
* 
* @param key ���ׂ�L�[
*/
int GameEngine::GetMouseButton(int key)const
{
	return glfwGetMouseButton(engine->window, key);
}

/**
* �E�B���h�E�T�C�Y��Ԃ�
*/
glm::vec2 GameEngine::GetWindowSize()const
{
	return engine->windowSize;
}

/**
* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@����������
*/
void GameEngine::SwapBuffers()const
{
	glfwSwapBuffers(engine->window);
	glfwPollEvents();
}

/**
* ���Ԃ̎擾
*/
double GameEngine::GetTime()
{
	return glfwGetTime();
}

/**
* �J�[�\���ʒu�̎擾
*/
glm::vec2 GameEngine::GetCursorPosition()
{
	double x, y;
	glfwGetCursorPos(engine->window, &x, &y);

	glm::vec2 cursorPos = glm::vec2(x, y);
	return cursorPos;
}

/**
* �J�[�\���̐ݒ�
*
* @param cursorType �ݒ肷��J�[�\���̏��
*/
void GameEngine::SetCursorType(int cursorType)
{
	glfwSetInputMode(engine->window, GLFW_CURSOR, cursorType);
}

/**
* �J�[�\���̈ʒu�̑��
*
* @param cursorpos �ݒ肷��J�[�\���̈ʒu
*/
void GameEngine::SetCursorPos(glm::vec2 cursorpos)
{
	double x, y;
	x = cursorpos.x;
	y = cursorpos.y;

	glfwSetCursorPos(engine->window, x, y);
}

/**
* �w�i�F�̕ύX
* 
* @param bgcolor �w�i�J���[
*/
void GameEngine::SetglClearColor(glm::vec3 bgcolor)
{
	bgColor = bgcolor;
}

/**
* �R���C�_�[�̕\���ؑ�
* 
* @param isShowCollider true �R���C�_�[��\��,false �R���C�_�[��\��
*/
void GameEngine::SetShowCollider(bool isShowCollider)
{
	showCollider = isShowCollider;
}

/**
* �E�B���h�E�����
*/
void GameEngine::WindowClose()
{
	glfwSetWindowShouldClose(engine->window, true);
}

/**
* �J�����̎擾
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
* �p�C�v���C���̎擾
*/
std::shared_ptr<ProgramPipeline>& GameEngine::GetPipeline()
{
	return pipeline;
}

/**
* �T���v���[�̎擾
*/
std::shared_ptr<Sampler>& GameEngine::GetSampler()
{
	return sampler;
}
