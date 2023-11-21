#define OLC_PGE_APPLICATION
#include "Source.h"
#include <unordered_map>

float aov = vecmath::PI / 1.7f;

float sdBox(vecmath::Vec3 p, vecmath::Vec3 b)
{
	using vecmath::Vec3;
	Vec3 q = Vec3(fabsf(p.x), fabsf(p.y), fabsf(p.z)) - b;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}


void Player::UpdateInputs(int xI, int zI, bool j)
{
	xInp = xI;
	zInp = zI;
	jumpInp = j;
}

void Player::SetPosition(vecmath::Vec3 pos)
{
	transform.pos = pos;
	collider.setBegin(transform.pos + colliderOffset);
}

void Player::Move(float delta)
{
	collider.setBegin(transform.pos + colliderOffset);

	const float ma = moveSpeed * 3;

	moveVector.x += (1.0f - moveVector.x / ma) * moveSpeed * xInp * delta;
	moveVector.z += (1.0f - moveVector.z / ma) * moveSpeed * zInp * delta;

	moveVector.x -= 0.55f * moveVector.x * delta;
	moveVector.z -= 0.55f * moveVector.z * delta;

	moveVector.y += gravity * delta;

	//moveVector = vecmath::rotateY(moveVector, -scene..y);

	if (transform.pos.y >= 0)
	{
		landed = true;
		transform.pos.y = 0;
		moveVector.y = -moveVector.y * 0.5f;

		if (fabsf(moveVector.y) < 0.1f)
			moveVector.y = 0;
	}

	if (jumpInp)
	{
		if (landed)
		{
			moveVector.y = jumpForce;
			landed = false;
		}
	}

	

	nextCollider = collider;
	nextCollider.moveBegin(moveVector * delta);

	cpuRender::AABB hit;
	if (scene->CheckCollision(nextCollider, hit))
	{
		float yo = transform.pos.y + collider.size.y - hit.getPosition().y;
		if (yo < 0.4f && yo >= 0)
		{
			transform.pos.y = hit.getPosition().y - collider.size.y;
			if (moveVector.y > 0)
				landed = true;
		}
		else
		{
			moveVector.x = 0;
		}
	}

	

	landed = false;
	
	transform.pos += moveVector * delta;
	
	mesh->transform.pos = transform.pos;
	vecmath::Quaternion a;
	vecmath::Vec3 axis = CrossProduct(moveVector, vecmath::Vec3(0, 1, 0));
	a.FromEuler(mesh->transform.rot);

	a.Rotate(axis, vecmath::Length(moveVector)*delta*0.5f);

	mesh->transform.rot = a.toEuler();

	mesh->rotOrder = 5;
}

void BasicScene::Load()
{

	sprites["ball"] = std::make_unique<olc::Sprite>("ballTexture.png");
	sprites["box"] = std::make_unique<olc::Sprite>("box.png");

	meshes["ball"] = cpuRender::Mesh("sphere.obj", true);
	meshes["box"] = cpuRender::Mesh("cubeMeshUV.obj", true);
	
	shaders["simple"] = std::make_unique<cpuRender::ShaderLibrary::FullColorShader>(olc::CYAN, 1.0f);
	shaders["collider"] = std::make_unique<cpuRender::ShaderLibrary::DepthShader>();

	player.scene = this;
	player.transform.pos = vecmath::Vec3(0, -10, 5);
	player.collider = cpuRender::AABB(vecmath::Vec3(0, 0, 0), vecmath::Vec3(1, 1, 1));
	player.colliderOffset = vecmath::Vec3(0, 0, 0);
	player.cameraOffset = vecmath::Vec3(0, -2.0f, -12.0f);
	
	

	objects.push_back(cpuRender::DrawableMesh());
	cpuRender::DrawableMesh& ball = *(objects.end() - 1);
	ball.mesh.reset(&meshes["ball"]);
	ball.shader.reset(shaders["simple"].get());
	ball.texture.reset(sprites["ball"].get());

	ball.transform.pos = vecmath::Vec3(0, 0, 20);
	

	objects.push_back(cpuRender::DrawableMesh());
	cpuRender::DrawableMesh& box = *(objects.end() - 1);
	box.mesh.reset(&meshes["box"]);
	box.shader.reset(shaders["simple"].get());
	box.texture.reset(sprites["box"].get());

	box.transform.pos = vecmath::Vec3(0, 3.5f, 10);
	box.transform.scale = vecmath::Vec3(10.5f, 0.3f, 10.5f);


	colliderBox = cpuRender::DrawableMesh();
	colliderBox.mesh.reset(&meshes["box"]);
	colliderBox.shader.reset(shaders["simple"].get());

	colliderBox.pivot = vecmath::Vec3(1, 1, 1);
	

	colliders.push_back(cpuRender::AABB(vecmath::Vec3(-50, 20, -50), vecmath::Vec3(100, 1, 100), true));

	//must be after all objects were pushed in vector since it reallocates its data
	player.mesh = &objects[0];
}

void BasicScene::Update(float delta)
{
	for (int i = 0; i < objects.size(); i++)
	{
		renderer->DrawMesh(objects[i], 0);
	}

	if (true)
	{

		/*colliderBox.pos = player.collider.getPosition();
		colliderBox.scale = player.collider.getSize() * 0.5f;

		//drawMesh(colliderBox, mainBuffer);

		for (int i = 0; i < colliders.size(); i++)
		{
			colliderBox.pos = colliders[i]->getPosition();
			colliderBox.scale = colliders[i]->getSize() * 0.5f;

			drawMesh(colliderBox, mainBuffer);
		}*/
	}

	renderer->ShowBuffers();
}

class Adventure: public olc::PixelGameEngine
{
	vecmath::Vec2 prevmouse;
	bool editMode;
	float camSpd = 15;
	BasicScene gameField;
	cpuRender::Renderer renderer;

public:
	Adventure(): gameField(this), renderer(1)
	{
		sAppName = "TentAdventure";
		renderer.SetPixelEngine(this);
		gameField.SetRenderer(&renderer);
		renderer.SetCamera(&gameField.camera);
		renderer.SetLightsList(gameField.GetLights());
		
	}
	bool OnUserCreate() override
	{
		gameField.Load();
		gameField.camera = cpuRender::Camera(ScreenWidth(), ScreenHeight());
		gameField.camera.setPerspective(3.1415f / 1.2f);
		return true;
	}


	bool OnUserUpdate(float delta) override
	{
		

		Clear(olc::DARK_BLUE);


		float mx = GetMouseX();
		float my = GetMouseY();
		
		gameField.Update(delta);
		
		if (GetKey(olc::M).bPressed)
		{
			editMode = !editMode;
		}
			

		if (!editMode)
		{
			int xInp = 0, zInp = 0;
			bool jumpInp = false;

			if (GetKey(olc::W).bHeld)
			{
				zInp = 1;
			}

			if (GetKey(olc::S).bHeld)
			{
				zInp = -1;
			}

			if (GetKey(olc::A).bHeld)
			{
				xInp = -1;
			}

			if (GetKey(olc::D).bHeld)
			{
				xInp = 1;
			}

			if (GetKey(olc::SPACE).bPressed)
			{
				jumpInp = true;
			}
			
			gameField.player.UpdateInputs(xInp, zInp, jumpInp);
			gameField.player.Move(delta);
		}
		else
		{
			if (gameField.playable)
			{
				if (GetKey(olc::W).bHeld)
				{
					gameField.camera.transform.pos.z += camSpd * cosf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.x += camSpd * sinf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.y += camSpd * sinf(gameField.camera.transform.rot.x) * delta;
				}


				if (GetKey(olc::W).bHeld)
				{
					gameField.camera.transform.pos.z += camSpd * cosf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.x += camSpd * sinf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.y += camSpd * sinf(gameField.camera.transform.rot.x) * delta;
				}

				if (GetKey(olc::S).bHeld)
				{
					gameField.camera.transform.pos.z -= camSpd * cosf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.x -= camSpd * sinf(gameField.camera.transform.rot.y) * cosf(gameField.camera.transform.rot.x) * delta;
					gameField.camera.transform.pos.y -= camSpd * sinf(gameField.camera.transform.rot.x) * delta;
				}

				if (GetKey(olc::A).bHeld)
				{
					gameField.camera.transform.pos.z += camSpd * cosf(gameField.camera.transform.rot.y - vecmath::PI / 2) * delta;
					gameField.camera.transform.pos.x += camSpd * sinf(gameField.camera.transform.rot.y - vecmath::PI / 2) * delta;
				}

				if (GetKey(olc::D).bHeld)
				{
					gameField.camera.transform.pos.z -= camSpd * cosf(gameField.camera.transform.rot.y - vecmath::PI / 2) * delta;
					gameField.camera.transform.pos.x -= camSpd * sinf(gameField.camera.transform.rot.y - vecmath::PI / 2) * delta;
				}

			}

			if (GetKey(olc::F).bHeld)
			{
				gameField.camera.transform.rot.y += (mx - prevmouse.x) * delta / ScreenWidth() * 200;
				gameField.camera.transform.rot.x += (my - prevmouse.y) * delta / ScreenHeight() * 200;
				if (gameField.camera.transform.rot.x > vecmath::PI / 2.5f)
					gameField.camera.transform.rot.x = vecmath::PI / 2.5f;
				if (gameField.camera.transform.rot.x < -vecmath::PI / 2.5f)
					gameField.camera.transform.rot.x = -vecmath::PI / 2.5f;
			}

			/*if (GetKey(olc::Q).bHeld)
			{

				gameField.camera += delta / 100;
			}

			if (GetKey(olc::E).bHeld)
			{

				zCof -= delta / 100;
			}*/
		}
		
		DrawString(20, 20, "test");
		prevmouse.x = mx;
		prevmouse.y = my;

		return true;
	}
};

int main()
{
	Adventure game;

	if (game.Construct(640, 480, 1,1 ,false ,false))
		game.Start();
	return 0;
}