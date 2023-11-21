#pragma once
#include "Zohich3DEngine.h"
#include <unordered_map>



/*struct Interactive
{
	bool activated = false;
	bool inside = false;
	bool justEntered = false;
	AABB boxCollider;

	Interactive()
	{

	}
	Interactive(AABB c)
	{
		boxCollider = c;
	}
	virtual void onEnter()
	{

	}

	virtual void action(float delta)
	{

	}
	void checkInter(AABB& other, float delta)
	{
		justEntered = false;
		if (boxCollider.aabbCollision(other.begin, other.end))
		{
			if (!activated)
			{
				justEntered = true;
				onEnter();
			}
			activated = true;
			inside = true;
		}
		else
			inside = false;

		if (inside)
		{
			action(delta);
		}
	}
};*/


struct Player
{

public:
	cpuRender::utils::Transform transform;
	vecmath::Vec3 colliderOffset;
	vecmath::Vec3 cameraOffset;
	cpuRender::DrawableMesh* mesh;

	cpuRender::AABB collider;
	cpuRender::AABB nextCollider;
	vecmath::Vec3 moveVector = vecmath::Vec3(0, 0, 0);

	float moveSpeed = 14;
	float jumpForce = -20;
	int xInp = 0;
	int zInp = 0;
	bool jumpInp;

	bool landed = false;
	float gravity = 30;

	cpuRender::Scene* scene;

	void UpdateInputs(int xI, int zI, bool j);
	void SetPosition(vecmath::Vec3 pos);
	void Move(float delta);


};

class BasicScene: public cpuRender::Scene
{
	BasicScene() = delete;
public:
	std::vector<cpuRender::Collider> colliders;
	std::vector<cpuRender::DrawableMesh> objects;

	olc::PixelGameEngine* game;

	cpuRender::Camera camera;
	Player player;

	std::unordered_map<std::string, std::unique_ptr<olc::Sprite>> sprites;
	std::unordered_map<std::string, cpuRender::Mesh> meshes;
	std::unordered_map<std::string, std::unique_ptr<cpuRender::Shader>> shaders;

	cpuRender::DrawableMesh skyBox;
	cpuRender::DrawableMesh colliderBox;

	BasicScene(olc::PixelGameEngine* p): game(p)
	{

	}

	void Load() override;
	void Update(float delta) override;

	/*
	void loadFromFile(std::string fileName)
	{
		colliders.clear();
		slopes.clear();

		std::ifstream fin;
		fin.open(fileName);

		AABB newCollider;

		int counterB;
		if (fin.is_open())
		{

			std::string type;

			while (!fin.eof())
			{
				type = " ";
				fin >> type;

				if (type == "C")
				{
					fin >> newCollider.begin.x;
					fin >> newCollider.begin.y;
					fin >> newCollider.begin.z;
					fin >> newCollider.size.x;
					fin >> newCollider.size.y;
					fin >> newCollider.size.z;

					newCollider.end.x = newCollider.end.x + newCollider.size.x;
					newCollider.end.y = newCollider.end.y + newCollider.size.y;
					newCollider.end.z = newCollider.end.z + newCollider.size.z;
					colliders.push_back(new AABB(newCollider.getPosition(), newCollider.getSize(), true));
				}
				if (type == "S")
				{
					vecmath::Vec3 pS;
					vecmath::Vec3 sZ;
					int dR;
					fin >> pS.x;
					fin >> pS.y;
					fin >> pS.z;

					fin >> sZ.x;
					fin >> sZ.y;
					fin >> sZ.z;

					fin >> dR;

					slopes.push_back(Slope(pS, sZ, dR));
				}
			}

		}
		else
		{
			std::cout << "error with load scene from file";
		}

	}
	void saveToFile(std::string fileName)
	{
		std::ofstream fout;
		fout.open(fileName);
		if (fout.is_open())
		{
			for (int i = 0; i < colliders.size(); i++)
			{
				fout << "C " + std::to_string(colliders[i]->getPosition().x) + " " + std::to_string(colliders[i]->getPosition().y) + " " + std::to_string(colliders[i]->getPosition().z) +
					" " + std::to_string(colliders[i]->getSize().x) + " " + std::to_string(colliders[i]->getSize().y) + " " + std::to_string(colliders[i]->getSize().z) + " \n";
			}
			for (int i = 0; i < slopes.size(); i++)
			{
				fout << "S " + std::to_string(slopes[i].begin.x) + " " + std::to_string(slopes[i].begin.y) + " " + std::to_string(slopes[i].begin.z) +
					" " + std::to_string(slopes[i].beginSize.x) + " " + std::to_string(slopes[i].beginSize.y) + " " + std::to_string(slopes[i].beginSize.z) + " " + std::to_string(slopes[i].direct) + " \n";
			}
			for (int i = 0; i < palms.size(); i++)
			{
				fout << "Palm " + std::to_string(palms[i].x) + " " + std::to_string(palms[i].y) + " " + std::to_string(palms[i].z) + " \n";
			}

			for (int i = 0; i < piles.size(); i++)
			{
				fout << "Pile " + std::to_string(piles[i].x) + " " + std::to_string(piles[i].y) + " " + std::to_string(piles[i].z) + " \n";
			}
		}
		else
		{
			std::cout << "error with saving scene file";
		}
	}
	*/
};