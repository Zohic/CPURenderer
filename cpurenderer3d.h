#include "olcPixelGameEngine.h"
#include "Vectors.h"
#include<algorithm>
#include <fstream>
#include <strstream>


#undef max
#undef min

float deg = vecmath::PI / 180.0f;

namespace cpuRender {

	namespace utils {
		struct Collider
		{

			int type = 0;
			Collider()
			{

			}
			virtual bool RayCollision(vecmath::Vec3 begin, vecmath::Vec3 dir, vecmath::Vec3& outPoint)
			{
				return false;
			}
			virtual bool aabbCollision(vecmath::Vec3 begin, vecmath::Vec3 end) const
			{
				return false;
			}
			virtual void Draw(olc::PixelGameEngine gfx)
			{
				gfx.DrawCircle(20, 20, 10, olc::RED);
			}
			virtual void Move(vecmath::Vec3 moveV)
			{

			}
			virtual vecmath::Vec3 getPosition()
			{
				return vecmath::Vec3(0);
			}
			virtual vecmath::Vec3 getSize()
			{
				return vecmath::Vec3(0);
			}

		};
		struct AABB : public Collider
		{
			vecmath::Vec3 begin;
			vecmath::Vec3 end;
			vecmath::Vec3 size;
			AABB()
			{
				type = 1;
			}
			AABB(vecmath::Vec3 b, vecmath::Vec3 e)
			{
				begin = b;
				end = e;
				size = end - begin;
				type = 1;
			}
			AABB(vecmath::Vec3 b, vecmath::Vec3 s, bool yeah)
			{
				begin = b;
				size = s;
				end = begin + size;
				type = 1;
			}
			void setBegin(vecmath::Vec3 b)
			{
				begin = b;
				end = b + size;
			}

			void moveBegin(vecmath::Vec3 m)
			{
				setBegin(begin + m);
			}

			void setSize(vecmath::Vec3 s)
			{
				size = s;
				end = begin + s;
			}


			bool aabbCollision(vecmath::Vec3 iBegin, vecmath::Vec3 iEnd) const override
			{
				if (end.x > iBegin.x && begin.x < iEnd.x)
					if (end.y > iBegin.y && begin.y < iEnd.y)
						if (end.z > iBegin.z && begin.z < iEnd.z)
							return true;
				return false;
			}

			/*
			bool RayCollision(vecmath::Vec3 beg, vecmath::Vec3 dir, vecmath::Vec3& outPoint) override
			{

				vecmath::Vec2 rectB = vecmath::Vec2(begin.x, begin.z);
				vecmath::Vec2 rectS = vecmath::Vec2(size.x, size.z);

				vecmath::Vec2 lineO = vecmath::Vec2(beg.x, beg.z);
				float len = sqrt(dir.x * dir.x + dir.z * dir.z);
				vecmath::Vec2 lineD = vecmath::Vec2(dir.x / len, dir.z / len);

				bool onTop = RectLineIntersection(rectB, rectS, lineO, lineD);

				rectB = vecmath::Vec2(begin.x, begin.y);
				rectS = vecmath::Vec2(size.x, size.y);

				lineO = vecmath::Vec2(beg.x, beg.y);
				len = sqrt(dir.x * dir.x + dir.y * dir.y);
				lineD = vecmath::Vec2(dir.x / len, dir.y / len);

				bool fromSide = RectLineIntersection(rectB, rectS, lineO, lineD);

				rectB = vecmath::Vec2(begin.y, begin.z);
				rectS = vecmath::Vec2(size.y, size.z);

				lineO = vecmath::Vec2(beg.y, beg.z);
				len = sqrt(dir.y * dir.y + dir.z * dir.z);
				lineD = vecmath::Vec2(dir.y / len, dir.z / len);

				bool inFront = RectLineIntersection(rectB, rectS, lineO, lineD);


				return onTop && fromSide && inFront;

			}
			void Draw(olc::PixelGameEngine gfx) override
			{

				vecmath::Vec3 side = vecmath::Vec3(size.x,0,0);
				vecmath::Vec3 up = vecmath::Vec3(0, size.y, 0);
				vecmath::Vec3 front = vecmath::Vec3(0, 0, size.z);

				drawVector(begin,end, olc::GREEN, gfx);
				drawVector(begin, begin + side, olc::GREEN, gfx);
				drawVector(begin, begin + front, olc::GREEN, gfx);
				drawVector(begin, begin + up, olc::GREEN, gfx);

				drawVector(begin + up, begin + up + side, olc::GREEN, gfx);
				drawVector(begin + up + side, begin + side, olc::GREEN, gfx);

				drawVector(begin + up, begin + up + front, olc::GREEN, gfx);
				drawVector(begin + up + side, begin + up + side + front, olc::GREEN, gfx);
				drawVector(begin + side, begin + side + front, olc::GREEN, gfx);

				drawVector(begin + up + front + side, begin + up + front, olc::GREEN, gfx);
				drawVector(begin + up + front - up, begin + up + front, olc::GREEN, gfx);
				drawVector(begin + up + front - up, begin + up + front - up + side, olc::GREEN, gfx);
				drawVector(begin + up + front - up + side, begin + up + front - up + side + up, olc::GREEN, gfx);

				int cenX = floorf(gfx.ScreenWidth() / 2);
				int cenY = floorf(gfx.ScreenHeight() / 2);
			}
			*/
			void Move(vecmath::Vec3 moveV) override
			{
				moveBegin(moveV);
			}

			vecmath::Vec3 getPosition() override
			{
				return begin;
			}
			vecmath::Vec3 getSize() override
			{
				return size;
			}

		};
		class Transform
		{
			using Vec3 = vecmath::Vec3;
		private:

		public:
			Vec3 pos, rot, scale;
			Transform() : pos(), rot(), scale(1.0f)
			{

			}
			Transform(Vec3 p) : pos(p), rot(), scale(1.0f)
			{

			}
			Transform(Vec3 p, Vec3 r) : pos(p), rot(r), scale(1.0f)
			{

			}
			Transform(Vec3 p, Vec3 r, Vec3 s) : pos(p), rot(r), scale(s)
			{

			}
		};
		struct LightSoucrce
		{
			vecmath::Vec3 position;
			olc::Pixel color;
			float power = 1;
			int type;//0-directional, 1-point
			LightSoucrce()
			{}

			LightSoucrce(int itype, vecmath::Vec3 dirpos)
			{
				type = itype;
				position = dirpos;
				color = olc::WHITE;
			}

		};
		
		struct Shader;
		struct DrawableMesh;

		struct ShaderPacket
		{
		public:
			vecmath::Vec3 normal;
			vecmath::Vec3 pixelPosition;//0
			vecmath::Vec3 trianglePoints[3];//1
			vecmath::Vec2 uv;//2
			olc::Sprite* spr;//3
			Shader* shader;
			std::vector<LightSoucrce>* lights;
			vecmath::Vec3 cameraPos;
			int accuracy = 1;
			ShaderPacket()
			{
				spr = nullptr;
			};

		};
		struct VertexShader
		{
			virtual vecmath::Vec3 proceed(vecmath::Vec3 inV, vecmath::Vec3 PosOff)
			{
				return inV + vecmath::Vec3((float)(rand() % 2) / 10);
			}
		};
		struct Shader
		{
		public:
			bool request[7];//0-pixel, 1-triangle, 2-uv, 3-clr, 4-spr, 5-normalMap
			Shader() {};
			virtual olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack)
			{
				return olc::CYAN;
			};

		};
		struct Triangle //not only geometry, but also all information needed for rasterizing
		{
			vecmath::Vec3 p[3];//points , coords
			vecmath::Vec3 op[3];//original points , coords
			vecmath::Vec2 tp[3];//texture points, UV coords
			std::shared_ptr<DrawableMesh> parentMesh;
			vecmath::Vec3 n;//normal
			Triangle()
			{
				p[0] = vecmath::Vec3(); p[1] = vecmath::Vec3(); p[2] = vecmath::Vec3();
			}
			Triangle(vecmath::Vec3 p1, vecmath::Vec3 p2, vecmath::Vec3 p3)
			{
				p[0] = p1; p[1] = p2; p[2] = p3;
			}
			Triangle(vecmath::Vec3 p1, vecmath::Vec3 p2, vecmath::Vec3 p3, vecmath::Vec2 tp1, vecmath::Vec2 tp2, vecmath::Vec2 tp3)
			{
				p[0] = p1; p[1] = p2; p[2] = p3;
				op[0] = p1; op[1] = p2; op[2] = p3;
				tp[0] = tp1; tp[1] = tp2; tp[2] = tp3;
			}

		};
		struct Mesh
		{
			std::vector<vecmath::Vec3> v;//vertexes of mesh
			std::vector<int*> trisInd;

			std::vector<vecmath::Vec2> texs;
			std::vector<int*> texsInd;
			bool hasUVs = false;
			std::string fileNames[2];
			Mesh() {}
			Mesh(std::string sFilename, bool uv = false)
			{
				if (!LoadFromObjectFile(sFilename, uv))
				{
					std::cout << "MESH LOAD ERROR";
				}
			}
			void SetFiles(std::string meshFilename, std::string textureFilename)
			{
				fileNames[0] = meshFilename;
				fileNames[1] = textureFilename;
			}
			bool LoadFromObjectFile(std::string sFilename, bool uv = false)//load mesh from .obj file
			{

				std::cout << "begin loading " << sFilename << std::endl;

				std::ifstream f(sFilename);
				if (!f.is_open())
					return false;
				hasUVs = uv;
				// Local cache of verts

				while (!f.eof())
				{
					char line[128];
					f.getline(line, 128);

					std::strstream s;
					s << line;

					char junk;

					if (line[0] == 'v')
					{
						if (line[1] == 't')
						{
							vecmath::Vec2 v;
							s >> junk >> junk >> v.x >> v.y;
							// A little hack for the spyro texture
							//v.u = 1.0f - v.u;
							v.y = 1.0f - v.y;
							texs.push_back(v);
						}
						else
						{
							vecmath::Vec3 nv;
							s >> junk >> nv.x >> nv.y >> nv.z;
							v.push_back(nv);
						}
					}

					if (!uv)
					{
						if (line[0] == 'f')
						{
							int f[3];
							s >> junk >> f[0] >> f[1] >> f[2];

							trisInd.push_back(new int[3]{ f[0] - 1, f[1] - 1, f[2] - 1 });

							//tris.push_back(Triangle(v[f[0] - 1], v[f[1] - 1], v[f[2] - 1]));
						}
					}
					else
					{
						if (line[0] == 'f')
						{
							s >> junk;

							std::string tokens[6];
							int nTokenCount = -1;


							while (!s.eof())
							{
								char c = s.get();
								if (c == ' ' || c == '/')
									nTokenCount++;
								else
									tokens[nTokenCount].append(1, c);
							}

							tokens[nTokenCount].pop_back();

							trisInd.push_back(new int[3]{ stoi(tokens[0]) - 1, stoi(tokens[2]) - 1, stoi(tokens[4]) - 1 });
							texsInd.push_back(new int[3]{ stoi(tokens[1]) - 1, stoi(tokens[3]) - 1, stoi(tokens[5]) - 1 });



							//tris.push_back(Triangle(v[stoi(tokens[0]) - 1], v[stoi(tokens[2]) - 1], v[stoi(tokens[4]) - 1],
							//	texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1]));

						}

					}
				}

				std::cout << "succesfully loaded " << sFilename << std::endl;

				return true;
			}

		};
		struct DrawableMesh
		{
			Transform transform;
			vecmath::Vec3 pivot;
			int rotOrder;

			std::shared_ptr<olc::Sprite> texture;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexShader> vertexShader;
			std::shared_ptr<Mesh> mesh;

			int shaderAccuracy = 1;

			bool drawBackFaces = false;
			bool drawOnTop = false;

			DrawableMesh()
			{

			}
			~DrawableMesh()
			{

			}
		};

		olc::Pixel mixColor(olc::Pixel c1, olc::Pixel c2, float coof)
		{
			float m = 1 - coof;
			return olc::Pixel(std::min(255, (int)(c1.r * m + c2.r * coof)),
				std::min(255, (int)(c1.g * m + c2.g * coof)),
				std::min(255, (int)(c1.b * m + c2.b * coof)));
		}
		
		class Camera
		{
			float xCof = 1;// 1 / tanf((3.1415f / 2) / 2);
			float yCof = 1;// 1 / tanf((3.1415f / 2) / 2);
			float zCof = 1;// 1;
			float* zBuffer = nullptr;
			Camera& operator=(const Camera& other) = delete;
		public:
			vecmath::Vec2 convert(vecmath::Vec3 pi)//converts 3d point to 2d screen , projection
			{
				if (pi.z > 0)
				{
					pi.z = std::max(nearClipDist, pi.z);

					return vecmath::Vec2(pi.x * xCof / (pi.z * zCof), pi.y * yCof / (pi.z * zCof));
				}
				else return vecmath::Vec2(100, 100);
			}
			vecmath::Vec3 antiConvert(vecmath::Vec2 pi, float iz)//converts 3d point to 2d screen , projection
			{
				if (iz > 0)
				{
					return vecmath::Vec3(pi.x / xCof * (iz * zCof), pi.y / yCof * (iz * zCof), iz);
				}
				else return vecmath::Vec3(0);
			}

			Camera()
			{
				
			}

			Camera& operator=(Camera&& other)
			{
				zBuffer = other.zBuffer;

				xCof = other.xCof;
				yCof = other.yCof;
				zCof = other.zCof;

				nearClipDist = other.nearClipDist;
				farClipDist = other.farClipDist;

				other.zBuffer = nullptr;

				
				return *this;
			}

			Camera(int frameW, int frameH)
			{
				zBuffer = new float[frameW * frameH];
				for (int i = 0; i < frameW * frameH; i++)
				{
					zBuffer[i] = INFINITY;
				}
				valid = true;
			}
			~Camera()
			{
				delete[] zBuffer;
			}
			void setPerspective(float AngOfView)
			{
				xCof = 1.0f / tanf((AngOfView) / 2);
				yCof = 1.0f / tanf((AngOfView) / 2);
				zCof = 0.001f;
			}
			float* GetZBuffer()
			{
				return zBuffer;
			}

			float nearClipDist = 1.1f;
			float farClipDist = 40000;
			Transform transform;
			bool valid = false;

		};

}
	
	/*void drawVector(vecmath::Vec3 p1, vecmath::Vec3 p2, olc::Pixel clr, olc::PixelGameEngine p)
	{
		vecmath::Vec2 pd1 = convert(rotate3d(p1 - camPos, camAng, 2));
		vecmath::Vec2 pd2 = convert(rotate3d(p2 - camPos, camAng, 2));

		int cenX = floorf(p.ScreenWidth() / 2);
		int cenY = floorf(p.ScreenHeight() / 2);
		p.DrawLine(pd1.x + cenX, pd1.y + cenY, pd2.x + cenX, pd2.y + cenY, clr);
	}*/
	
	
	using namespace utils;
	class Renderer;
	class Scene;
	class Scene
	{
		
	protected:
		std::vector<LightSoucrce> lights;
		std::vector<Collider> colliders;
		Renderer* renderer;
	public:
		
		Scene()
		{

		}

		bool CheckCollision(const AABB& coll, Collider& out) const
		{
			for (int i = 0; i < colliders.size(); i++)
			{
				if (colliders[i].aabbCollision(coll.begin, coll.end))
				{
					out = colliders[i];
					return true;
				}
			}
			out = AABB();
			return false;
		}

		void SetRenderer(Renderer* r)
		{
			renderer = r;
		}

		std::vector<LightSoucrce>* GetLights()
		{
			return &lights;
		}

		virtual void Load() {

		}
		virtual void Update(float delta) {

		}

		bool playable = true;
	};
	class Renderer
	{

	private:
		Camera* activeCamera = nullptr;
		std::vector<LightSoucrce>* lights;
		std::vector<std::vector<Triangle>> layers;
		olc::PixelGameEngine* pixel;

		Renderer() {};
		void DrawMesh(const DrawableMesh& dm, std::vector<Triangle>& drawBuffer)
		{

			for (int f = 0; f < dm.mesh->trisInd.size(); f++)
			{
				Triangle out;

				std::vector<vecmath::Vec3> vers = dm.mesh->v;

				if (dm.vertexShader.get() != nullptr)
					for (int ver = 0; ver < dm.mesh->v.size(); ver++)
					{
						vers[ver] = dm.vertexShader->proceed(vers[ver], dm.transform.pos);
					}

				vecmath::Vec3 p1 = vers[dm.mesh->trisInd[f][0]];
				vecmath::Vec3 p2 = vers[dm.mesh->trisInd[f][1]];
				vecmath::Vec3 p3 = vers[dm.mesh->trisInd[f][2]];

				p1 = p1 + dm.pivot;
				p2 = p2 + dm.pivot;
				p3 = p3 + dm.pivot;

				p1.MultiplyPerComponent(dm.transform.scale);
				p2.MultiplyPerComponent(dm.transform.scale);
				p3.MultiplyPerComponent(dm.transform.scale);

				p1 = rotate3d(p1, dm.transform.rot, dm.rotOrder);
				p2 = rotate3d(p2, dm.transform.rot, dm.rotOrder);
				p3 = rotate3d(p3, dm.transform.rot, dm.rotOrder);


				p1 = p1 + dm.transform.pos;
				p2 = p2 + dm.transform.pos;
				p3 = p3 + dm.transform.pos;

				vecmath::Vec3 normal, l1, l2;

				l1 = p1 - p3;
				l2 = p2 - p3;

				normal = CrossProduct(l1, l2);
				normal = vecmath::NormaliseVec(normal);

				vecmath::Vec3 outNorm = normal;

				if (dm.shader->request[0] || dm.shader->request[1])
				{
					out.op[0] = p1;
					out.op[1] = p2;
					out.op[2] = p3;
				}

				p1 = p1 - activeCamera->transform.pos;
				p2 = p2 - activeCamera->transform.pos;
				p3 = p3 - activeCamera->transform.pos;

				p1 = rotate3d(p1, activeCamera->transform.rot, 2);
				p2 = rotate3d(p2, activeCamera->transform.rot, 2);
				p3 = rotate3d(p3, activeCamera->transform.rot, 2);


				l1 = p1 - p3;
				l2 = p2 - p3;


				normal.x = l1.y * l2.z - l1.z * l2.y;

				normal.y = l1.z * l2.x - l1.x * l2.z;

				normal.z = l1.x * l2.y - l1.y * l2.x;

				normal = vecmath::NormaliseVec(normal);

				float dc = DotProduct(normal, p1);


				if (dc <= 0.0f || dm.drawBackFaces)
					if (p1.z > activeCamera->nearClipDist || p2.z > activeCamera->nearClipDist || p3.z > activeCamera->nearClipDist)
					{

						std::vector<Triangle> outTris;

						const auto Clip1Out = [&](vecmath::Vec3 outP, vecmath::Vec3 inP1, vecmath::Vec3 inP2, int outTN, int in1TN, int in2TN)
						{
							float to1 = (-outP.z + activeCamera->nearClipDist) / (inP1.z - outP.z);
							float to2 = (-outP.z + activeCamera->nearClipDist) / (inP2.z - outP.z);

							vecmath::Vec3 newPnt1 = outP + (inP1 - outP) * to1;
							vecmath::Vec3 newPnt2 = outP + (inP2 - outP) * to2;

							vecmath::Vec2 newTPnt1 = dm.mesh->texs[dm.mesh->texsInd[f][outTN]] + (dm.mesh->texs[dm.mesh->texsInd[f][in1TN]] - dm.mesh->texs[dm.mesh->texsInd[f][outTN]]) * to1;
							vecmath::Vec2 newTPnt2 = dm.mesh->texs[dm.mesh->texsInd[f][outTN]] + (dm.mesh->texs[dm.mesh->texsInd[f][in2TN]] - dm.mesh->texs[dm.mesh->texsInd[f][outTN]]) * to2;

							outTris.push_back(
								Triangle(
									newPnt1, inP1, inP2,
									newTPnt1, dm.mesh->texs[dm.mesh->texsInd[f][in1TN]], dm.mesh->texs[dm.mesh->texsInd[f][in2TN]]));

							outTris[outTris.size() - 1].op[0] = out.op[outTN] + (out.op[in1TN] - out.op[outTN]) * to1;
							outTris[outTris.size() - 1].op[1] = out.op[in1TN];
							outTris[outTris.size() - 1].op[2] = out.op[in2TN];

							outTris.push_back(
								Triangle(
									newPnt2, newPnt1, inP2,
									newTPnt2, newTPnt1, dm.mesh->texs[dm.mesh->texsInd[f][in2TN]]));

							outTris[outTris.size() - 1].op[0] = out.op[outTN] + (out.op[in2TN] - out.op[outTN]) * to2;
							outTris[outTris.size() - 1].op[1] = out.op[outTN] + (out.op[in1TN] - out.op[outTN]) * to1;
							outTris[outTris.size() - 1].op[2] = out.op[in2TN];




						};
						const auto Clip2Out = [&](vecmath::Vec3 outP1, vecmath::Vec3 outP2, vecmath::Vec3 inP, int out1TN, int out2TN, int inTN)
						{
							float to1 = (-outP1.z + activeCamera->nearClipDist) / (inP.z - outP1.z);
							float to2 = (-outP2.z + activeCamera->nearClipDist) / (inP.z - outP2.z);

							vecmath::Vec3 newPnt1 = outP1 + (inP - outP1) * to1;
							vecmath::Vec3 newPnt2 = outP2 + (inP - outP2) * to2;

							vecmath::Vec2 newTPnt1 = dm.mesh->texs[dm.mesh->texsInd[f][out1TN]] + (dm.mesh->texs[dm.mesh->texsInd[f][inTN]] - dm.mesh->texs[dm.mesh->texsInd[f][out1TN]]) * to1;
							vecmath::Vec2 newTPnt2 = dm.mesh->texs[dm.mesh->texsInd[f][out2TN]] + (dm.mesh->texs[dm.mesh->texsInd[f][inTN]] - dm.mesh->texs[dm.mesh->texsInd[f][out2TN]]) * to2;

							outTris.push_back(
								Triangle(
									newPnt1, inP, newPnt2,
									newTPnt1, dm.mesh->texs[dm.mesh->texsInd[f][inTN]], newTPnt2));

							outTris[outTris.size() - 1].op[0] = out.op[out1TN] + (out.op[inTN] - out.op[out1TN]) * to1;
							outTris[outTris.size() - 1].op[1] = out.op[inTN];
							outTris[outTris.size() - 1].op[2] = out.op[out2TN] + (out.op[inTN] - out.op[out2TN]) * to2;



						};


						if (p1.z > activeCamera->nearClipDist && p2.z > activeCamera->nearClipDist && p3.z > activeCamera->nearClipDist)
						{
							outTris.push_back(
								Triangle(
									p1, p2, p3,
									dm.mesh->texs[dm.mesh->texsInd[f][0]], dm.mesh->texs[dm.mesh->texsInd[f][1]], dm.mesh->texs[dm.mesh->texsInd[f][2]]));

							outTris[outTris.size() - 1].op[0] = out.op[0];
							outTris[outTris.size() - 1].op[1] = out.op[1];
							outTris[outTris.size() - 1].op[2] = out.op[2];
						}
						else
						{
							if (p1.z < activeCamera->nearClipDist)
							{

								if (p2.z < activeCamera->nearClipDist)
								{
									Clip2Out(p1, p2, p3, 0, 1, 2);
								}
								else if (p3.z < activeCamera->nearClipDist)
								{
									Clip2Out(p1, p3, p2, 0, 2, 1);
								}
								else
								{
									Clip1Out(p1, p2, p3, 0, 1, 2);
								}


							}
							else if (p2.z < activeCamera->nearClipDist)
							{

								if (p3.z < activeCamera->nearClipDist)
								{
									Clip2Out(p2, p3, p1, 1, 2, 0);
								}
								else
								{
									Clip1Out(p2, p1, p3, 1, 0, 2);
								}
							}
							else if (p3.z < activeCamera->nearClipDist)
							{
								Clip1Out(p3, p1, p2, 2, 0, 1);
							}
						}

						for (int i = 0; i < outTris.size(); i++)
						{
							outTris[i].parentMesh = std::make_shared<DrawableMesh>(dm);
							outTris[i].n = outNorm;
							drawBuffer.push_back(outTris[i]);
						}
					}

			}
		}
		void DrawTextureTriangle
		(
			int x1, int y1, float z1,
			int x2, int y2, float z2,
			int x3, int y3, float z3,
			float u1, float v1,
			float u2, float v2,
			float u3, float v3,
			ShaderPacket& pack, olc::PixelGameEngine& p
		)
		{
			bool need0 = pack.shader->request[0];
			bool need1 = pack.shader->request[1];

			if (y2 < y1)
			{
				std::swap(y1, y2);
				std::swap(x1, x2);
				std::swap(z1, z2);
				std::swap(u1, u2);
				std::swap(v1, v2);
				if (need0)
					std::swap(pack.trianglePoints[0], pack.trianglePoints[1]);

			}
			if (y3 < y1)
			{
				std::swap(y1, y3);
				std::swap(x1, x3);
				std::swap(z1, z3);
				std::swap(u1, u3);
				std::swap(v1, v3);
				if (need0)
					std::swap(pack.trianglePoints[0], pack.trianglePoints[2]);
			}
			if (y3 < y2)
			{
				std::swap(y2, y3);
				std::swap(x2, x3);
				std::swap(z2, z3);
				std::swap(u2, u3);
				std::swap(v2, v3);
				if (need0)
					std::swap(pack.trianglePoints[1], pack.trianglePoints[2]);
			}

			int dx1 = x2 - x1,
				dy1 = y2 - y1,
				dx2 = x3 - x1,
				dy2 = y3 - y1;

			float dz1 = z2 - z1,
				du1 = u2 - u1,
				dv1 = v2 - v1,
				dz2 = z3 - z1,
				du2 = u3 - u1,
				dv2 = v3 - v1,
				ka = 0,
				kb = 0,
				kz1 = 0, kz2 = 0,
				ku1 = 0,
				ku2 = 0,
				kv1 = 0,
				kv2 = 0,
				texU, texV;


			if (dy1)
			{
				ka = dx1 / (float)abs(dy1);
				kz1 = dz1 / (float)abs(dy1);
				ku1 = du1 / (float)abs(dy1);
				kv1 = dv1 / (float)abs(dy1);
			}
			if (dy2)
			{
				kb = dx2 / (float)abs(dy2);
				kz2 = dz2 / (float)abs(dy2);
				ku2 = du2 / (float)abs(dy2);
				kv2 = dv2 / (float)abs(dy2);
			}

			int toDrawY1 = y1;
			int toDrawY2 = y2;

			olc::Pixel prevPix = olc::MAGENTA;
			int pixCounter = -1;
			float* zBuffer = activeCamera->GetZBuffer();
			float a = zBuffer[150000];

			if (dy1)
			{


				if (toDrawY1 < 0)toDrawY1 = 0;
				if (toDrawY2 > p.ScreenHeight())toDrawY2 = p.ScreenHeight() - 1;

				for (int drawY = toDrawY1; drawY <= toDrawY2; drawY++)
				{

					int SX = x1 + (float)(drawY - y1) * ka;
					int EX = x1 + (float)(drawY - y1) * kb;

					float SPX, SPY, SPZ;
					float EPX, EPY, EPZ;

					if (need0)
					{
						SPX = pack.trianglePoints[0].x + (float)(drawY - y1) * ((pack.trianglePoints[1].x - pack.trianglePoints[0].x) / (float)dy1);
						EPX = pack.trianglePoints[0].x + (float)(drawY - y1) * ((pack.trianglePoints[2].x - pack.trianglePoints[0].x) / (float)dy2);

						SPY = pack.trianglePoints[0].y + (float)(drawY - y1) * ((pack.trianglePoints[1].y - pack.trianglePoints[0].y) / (float)dy1);
						EPY = pack.trianglePoints[0].y + (float)(drawY - y1) * ((pack.trianglePoints[2].y - pack.trianglePoints[0].y) / (float)dy2);

						SPZ = pack.trianglePoints[0].z + (float)(drawY - y1) * ((pack.trianglePoints[1].z - pack.trianglePoints[0].z) / (float)dy1);
						EPZ = pack.trianglePoints[0].z + (float)(drawY - y1) * ((pack.trianglePoints[2].z - pack.trianglePoints[0].z) / (float)dy2);
					}


					float SU = u1 + (float)(drawY - y1) * ku1;
					float SV = v1 + (float)(drawY - y1) * kv1;

					float EU = u1 + (float)(drawY - y1) * ku2;
					float EV = v1 + (float)(drawY - y1) * kv2;

					float SZ = z1 + (float)(drawY - y1) * kz1;
					float EZ = z1 + (float)(drawY - y1) * kz2;


					if (SX > EX)
					{
						std::swap(SX, EX);
						std::swap(SZ, EZ);
						std::swap(SU, EU);
						std::swap(SV, EV);
						if (need0)
						{
							std::swap(SPX, EPX);
							std::swap(SPY, EPY);
							std::swap(SPZ, EPZ);
						}

					}
					texU = SU; texV = SV;


					float tSX = SX;
					float tEX = EX;

					float t = 0;

					if (SX < 0)
					{
						SX = 0;
						t = abs(tSX) * (1.0f / ((float)(tEX - tSX)));
					}
					if (EX > p.ScreenWidth())
					{
						EX = p.ScreenWidth() - 1;
					}



					for (int drawX = SX; drawX < EX; drawX++)
					{
						float w = (SZ + (EZ - SZ) * (t));

						texU = SU + (EU - SU) * t;
						texV = SV + (EV - SV) * t;

						float invW = 1.0f / w;

						texU *= invW;
						texV *= invW;



						if (drawX + drawY * p.ScreenWidth() >= 0 && drawX + drawY * p.ScreenWidth() < p.ScreenWidth() * p.ScreenHeight() - 1)
							if (zBuffer[drawX + drawY * p.ScreenWidth()] > invW && invW > activeCamera->nearClipDist && invW < activeCamera->farClipDist)
							{
								zBuffer[drawX + drawY * p.ScreenWidth()] = invW;

								olc::Pixel finalPix;

								if (pack.spr != nullptr)
									finalPix = pack.spr->Sample(texU, texV);

								pack.uv.x = texU;
								pack.uv.y = texV;

								if (need0)
								{
									pack.pixelPosition.x = (SPX + (EPX - SPX) * (t));
									pack.pixelPosition.y = (SPY + (EPY - SPY) * (t));
									pack.pixelPosition.z = (SPZ + (EPZ - SPZ) * (t));

									pack.pixelPosition.x = pack.pixelPosition.x * invW;
									pack.pixelPosition.y = pack.pixelPosition.y * invW;
									pack.pixelPosition.z = pack.pixelPosition.z * invW;
								}


								pixCounter += 1;
								if (pixCounter >= pack.accuracy || pixCounter == 0)
								{

									finalPix = pack.shader->proceed(finalPix, pack);
									prevPix = finalPix;
									pixCounter = 0;
								}
								else
								{
									finalPix = prevPix;
								}
								p.Draw(drawX, drawY, finalPix);

							}

						t += 1.0f / ((float)(tEX - tSX));
					}


				}
			}
			dx1 = x3 - x2; dy1 = y3 - y2;

			du1 = u3 - u2; dv1 = v3 - v2;

			dz1 = z3 - z2;

			if (dy1)
			{
				ka = dx1 / (float)abs(dy1);
				kz1 = dz1 / (float)abs(dy1);
				ku1 = du1 / (float)abs(dy1);
				kv1 = dv1 / (float)abs(dy1);

			}
			if (dy2)
			{
				kb = dx2 / (float)abs(dy2);
			}


			toDrawY1 = y2;
			toDrawY2 = y3;

			if (toDrawY1 < 0)toDrawY1 = 0;
			if (toDrawY2 > p.ScreenHeight())toDrawY2 = p.ScreenHeight() - 1;
			float prevT = 0;
			for (int drawY = toDrawY1; drawY <= toDrawY2; drawY++)
			{
				int SX = x2 + (float)(drawY - y2) * ka;
				int EX = x1 + (float)(drawY - y1) * kb;

				float SPX, SPY, SPZ;
				float EPX, EPY, EPZ;

				if (need0)
				{
					SPX = pack.trianglePoints[1].x + (float)(drawY - y2) * ((pack.trianglePoints[2].x - pack.trianglePoints[1].x) / (float)dy1);
					EPX = pack.trianglePoints[0].x + (float)(drawY - y1) * ((pack.trianglePoints[2].x - pack.trianglePoints[0].x) / (float)dy2);

					SPY = pack.trianglePoints[1].y + (float)(drawY - y2) * ((pack.trianglePoints[2].y - pack.trianglePoints[1].y) / (float)dy1);
					EPY = pack.trianglePoints[0].y + (float)(drawY - y1) * ((pack.trianglePoints[2].y - pack.trianglePoints[0].y) / (float)dy2);

					SPZ = pack.trianglePoints[1].z + (float)(drawY - y2) * ((pack.trianglePoints[2].z - pack.trianglePoints[1].z) / (float)dy1);
					EPZ = pack.trianglePoints[0].z + (float)(drawY - y1) * ((pack.trianglePoints[2].z - pack.trianglePoints[0].z) / (float)dy2);
				}


				float SU = u2 + (float)(drawY - y2) * ku1;
				float EU = u1 + (float)(drawY - y1) * ku2;

				float SV = v2 + (float)(drawY - y2) * kv1;
				float EV = v1 + (float)(drawY - y1) * kv2;

				float SZ = z2 + (float)(drawY - y2) * kz1;
				float EZ = z1 + (float)(drawY - y1) * kz2;

				float SDZ = z2;
				float EDZ = z2 + (z3 - z2) * ((float)(drawY - toDrawY1) / (toDrawY2 - toDrawY1));

				if (SX > EX)
				{
					std::swap(SX, EX);
					std::swap(SZ, EZ);
					std::swap(SU, EU);
					std::swap(SV, EV);
					if (need0)
					{
						std::swap(SPX, EPX);
						std::swap(SPY, EPY);
						std::swap(SPZ, EPZ);
					}
				}
				texU = SU; texV = SV;
				float t = 0;

				float tSX = SX;
				float tEX = EX;

				if (SX < 0)
				{
					SX = 0;
					t = abs(tSX) * (1.0f / ((float)(tEX - tSX)));
				}
				if (EX > p.ScreenWidth())
				{
					EX = p.ScreenWidth() - 1;
					//t = abs(tEX - EX) * (1.0f / ((float)(tEX - tSX)));
				}

				prevT = t;

				for (int drawX = SX; drawX < EX; drawX++)
				{
					float w = (SZ + (EZ - SZ) * (t));

					texU = SU + (EU - SU) * t;
					texV = SV + (EV - SV) * t;

					float invW = 1.0f / w;

					texU *= invW;
					texV *= invW;


					float b = activeCamera->nearClipDist;

					if (drawX + drawY * p.ScreenWidth() >= 0 && drawX + drawY * p.ScreenWidth() < p.ScreenWidth() * p.ScreenHeight() - 1)
					{
						if (zBuffer[drawX + drawY * p.ScreenWidth()] > invW && invW > activeCamera->nearClipDist && invW < activeCamera->farClipDist)
						{
							zBuffer[drawX + drawY * p.ScreenWidth()] = invW;
							olc::Pixel finalPix;
							if (pack.spr != nullptr)
								finalPix = pack.spr->Sample(texU, texV);

							pack.uv.x = texU;
							pack.uv.y = texV;

							if (need0)
							{
								pack.pixelPosition.x = (SPX + (EPX - SPX) * (t));
								pack.pixelPosition.y = (SPY + (EPY - SPY) * (t));
								pack.pixelPosition.z = (SPZ + (EPZ - SPZ) * (t));

								pack.pixelPosition.x = pack.pixelPosition.x * invW;
								pack.pixelPosition.y = pack.pixelPosition.y * invW;
								pack.pixelPosition.z = pack.pixelPosition.z * invW;

							}
							pixCounter += 1;
							if (pixCounter >= pack.accuracy || pixCounter == 0)
							{
								finalPix = pack.shader->proceed(finalPix, pack);
								prevPix = finalPix;
								pixCounter = 0;
							}
							else
							{
								finalPix = prevPix;
							}
							p.Draw(drawX, drawY, finalPix);

						}
					}

					t += 1.0f / ((float)(tEX - tSX));
				}



			}
		

		}

		void DrawBuffer(const std::vector<Triangle>& drawBuffer, olc::PixelGameEngine& p)
		{
			for (int f = 0; f < drawBuffer.size(); f++) {
				Triangle td = drawBuffer[f];

				bool needP = td.parentMesh->shader->request[0] || td.parentMesh->shader->request[1];

				for (int g = 0; g < 3; g++)
				{
					vecmath::Vec2 converted = activeCamera->convert(td.p[g]);

					td.p[g].x = converted.x + floorf((float)p.ScreenWidth() / 2);
					td.p[g].y = converted.y + floorf((float)p.ScreenHeight() / 2);

					td.tp[g].x = td.tp[g].x / td.p[g].z;
					td.tp[g].y = td.tp[g].y / td.p[g].z;

					if (needP)
					{

						td.op[g].x = td.op[g].x / td.p[g].z;
						td.op[g].y = td.op[g].y / td.p[g].z;
						td.op[g].z = td.op[g].z / td.p[g].z;
					}

					td.p[g].z = 1.0f / td.p[g].z;
				}

				ShaderPacket pack = ShaderPacket();
				pack.normal = td.n;
				pack.accuracy = td.parentMesh->shaderAccuracy;

				pack.spr = td.parentMesh->texture.get();
				pack.shader = td.parentMesh->shader.get();
				pack.cameraPos = activeCamera->transform.pos;
				pack.lights = lights;

				if (needP)
				{
					pack.trianglePoints[0] = td.op[0];
					pack.trianglePoints[1] = td.op[1];
					pack.trianglePoints[2] = td.op[2];
				}

				DrawTextureTriangle(
					td.p[0].x, td.p[0].y, td.p[0].z, // 1point
					td.p[1].x, td.p[1].y, td.p[1].z, // 2point
					td.p[2].x, td.p[2].y, td.p[2].z, // 3point
					td.tp[0].x, td.tp[0].y,// 1 UV
					td.tp[1].x, td.tp[1].y,// 2 UV
					td.tp[2].x, td.tp[2].y, // 3 UV
					pack, p);

			}
		}
	
	
	public:

		void DrawMesh(const DrawableMesh& dm, int layer = 0)
		{
			DrawMesh(dm, layers[layer]);
		}


		void ShowBuffers()
		{
			for (unsigned char i = 0; i < layers.size(); i++)
			{
				DrawBuffer(layers[i], *pixel);
				layers[i].clear();
			}
			//float infv = INFINITY;
			//int v = *((int*)(&infv));

			for (int i = 0; i < pixel->ScreenWidth() * pixel->ScreenHeight(); i++)
			{
				activeCamera->GetZBuffer()[i] = INFINITY;
			}
				
			//memset(activeCamera->GetZBuffer(), v, pixel->ScreenWidth() * pixel->ScreenHeight());
		}

		void SetPixelEngine(olc::PixelGameEngine* p)
		{
			pixel = p;
		}
		void SetLightsList(std::vector<LightSoucrce>* ligs)
		{
			lights = ligs;
		}
		Renderer(unsigned char n)
		{
			SetLayersNumber(n);
		}
		void SetCamera(Camera* camera)
		{
			activeCamera = camera;
		}
		void SetLayersNumber(unsigned char n)
		{
			layers.resize(n);
		}
	};

	namespace ShaderLibrary {
		typedef utils::Shader Shader;
		typedef utils::VertexShader VertexShader;
		typedef utils::ShaderPacket ShaderPacket;
		
		struct FullColorShader :public Shader
		{
		public:

			olc::Pixel clr;
			float alpha;
			FullColorShader()
			{
				clr = olc::WHITE;
				alpha = 1;

			};
			FullColorShader(olc::Pixel c, float a)
			{
				clr = c;
				alpha = a;
			}
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{
				return mixColor(clr, in, alpha);
			};

		};
		struct FillTexture : public Shader
		{

			vecmath::Vec2 offset;
			vecmath::Vec2 scale;
			FillTexture()
			{
				request[2] = true;
				request[4] = true;
				scale = vecmath::Vec2(1, 1);
				offset = vecmath::Vec2(1, 1);
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{
				vecmath::Vec2 offsetOO = vecmath::Vec2(1.0f / offset.x, 1.0f / offset.y);
				vecmath::Vec2 newP = vecmath::Vec2(floorf(pack.uv.x / offsetOO.x), floorf(pack.uv.y / offsetOO.y));
				vecmath::Vec2 ksp = vecmath::Vec2(pack.uv.x - offsetOO.x * newP.x, pack.uv.y - offsetOO.y * newP.y);
				return pack.spr->Sample(ksp.x * offset.x, ksp.y * offset.y);
			};
		};
		struct InverseShader :public Shader
		{
		public:
			InverseShader()
			{

			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{
				return olc::Pixel(255 - in.r, 255 - in.g, 255 - in.b);
			};

		};
		struct GradientShader :public Shader
		{
		public:
			olc::Pixel begin;
			olc::Pixel end;
			int type;
			GradientShader(int t)
			{
				begin = olc::BLACK;
				end = olc::WHITE;
				type = t;
				request[2] = true;

			};
			GradientShader(int t, olc::Pixel b, olc::Pixel e)
			{
				begin = b;
				end = e;
				type = t;
				request[2] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{

				float k;
				float u = pack.uv.x;
				float v = pack.uv.y;
				if (type == 0)
					k = u;
				else if (type == 1)
					k = v;
				else if (type == 2)
					k = (u + v) / 2;

				return olc::Pixel(
					begin.r + (end.r - begin.r) * k,
					begin.g + (end.g - begin.g) * k,
					begin.b + (end.b - begin.b) * k);
			};

		};
		struct SurfaceLightShader :public Shader
		{
		public:
			float minLight = 0.0f;
			bool center;
			SurfaceLightShader()
			{
				center = false;
				request[1] = true;
			};
			SurfaceLightShader(bool cen)
			{
				center = cen;
				request[1] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{
				//std::cout << pack.originalPoints[0].x;
				float dl = minLight;
				if (pack.lights->size() > 0)
				{
					vecmath::Vec3 cen, toLight;
					for (int lig = 0; lig < pack.lights->size(); lig++)
					{

						if ((*pack.lights)[lig].type == 0)
							toLight = vecmath::NormaliseVec((*pack.lights)[lig].position);
						else
						{
							vecmath::Vec3 p1, p2, p3;
							p1 = pack.trianglePoints[0];
							if (center)
							{
								p2 = pack.trianglePoints[1];
								p3 = pack.trianglePoints[2];
								cen = vecmath::Vec3((p1.x + p2.x + p3.x) / 3.0f, (p1.y + p2.y + p3.y) / 3.0f, (p1.z + p2.z + p3.z) / 3.0f);
							}
							else
							{
								cen = p1;
							}


							toLight = (*pack.lights)[lig].position - cen;
							toLight = vecmath::NormaliseVec(toLight);

						}

						float newD = DotProduct(pack.normal, toLight);

						if (newD > dl)
							dl = newD;

						if (dl < minLight)dl = minLight;
					}
				}
				else
				{
					dl = minLight;
				}

				return olc::Pixel(std::min(255.0f, in.r * dl), std::min(255.0f, in.g * dl), std::min(255.0f, in.b * dl));
			};

		};
		struct DepthShader :public Shader
		{
			int coord;
			float depth;
		public:
			DepthShader()
			{
				coord = 2;
				depth = 40.0f;
				request[0] = true;
			};
			DepthShader(int tp, float dep)
			{
				coord = tp;
				depth = dep;
				request[0] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{

				if (coord == 2)
				{
					float k = depth / pack.pixelPosition.z;
					int  clr = (int)(255 * k);
					clr = clr % 255;
					return olc::Pixel(clr, clr, clr);
				}
				if (coord == 0)
				{
					float k = depth / pack.pixelPosition.x;
					int  clr = (int)(255 * k);
					clr = clr % 255;
					return olc::Pixel(clr, clr, clr);
				}
				if (coord == 1)
				{
					float k = depth / pack.pixelPosition.y;
					int  clr = (int)(255 * k);
					clr = clr % 255;
					return olc::Pixel(clr, clr, clr);
				}
				return olc::RED;
			};

		};
		struct PerPixelLightShader :public Shader
		{
		public:
			float minLigth = 0.0f;
			PerPixelLightShader()
			{
				request[0] = true;
				request[2] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{
				float dl = minLigth;
				if ((*pack.lights).size() > 0)
				{
					vecmath::Vec3 toLight;
					for (int lig = 0; lig < (*pack.lights).size(); lig++)
					{

						if ((*pack.lights)[lig].type == 0)
							toLight = vecmath::NormaliseVec((*pack.lights)[lig].position);
						else
						{
							toLight = (*pack.lights)[lig].position - pack.pixelPosition;
							toLight = vecmath::NormaliseVec(toLight);
						}


						float newD = DotProduct(pack.normal, toLight);
						newD *= (*pack.lights)[lig].power;

						if (newD > dl)
							dl = newD;

						if (dl < minLigth)
							dl = minLigth;

					}
				}
				else
				{
					dl = minLigth;
				}

				return olc::Pixel(std::min(255.0f, in.r * dl), std::min(255.0f, in.g * dl), std::min(255.0f, in.b * dl));
			};

		};
		struct PlasticLightShader :public Shader
		{
		public:

			float minLight = 0.0f;
			int shiness = 10;

			PlasticLightShader()
			{
				request[0] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{

				float last = 0.0f;

				olc::Pixel out = in;
				if ((*pack.lights).size() > 0)
				{
					for (int lig = 0; lig < (*pack.lights).size(); lig++)
					{

						//normal in pack
						vecmath::Vec3 light;
						if ((*pack.lights)[lig].type == 0)
						{
							light = vecmath::NormaliseVec((*pack.lights)[lig].position);
						}
						else
						{
							light = vecmath::NormaliseVec((*pack.lights)[lig].position - pack.pixelPosition);
						}

						vecmath::Vec3 view = vecmath::NormaliseVec(pack.cameraPos - pack.pixelPosition);
						vecmath::Vec3 ref = vecmath::NormaliseVec(vecmath::Reflect(light, pack.normal) * (-1.0f));

						out = in;


						float lightCoof = std::max(DotProduct(pack.normal, light), 0.0f);

						if (lightCoof < minLight)
							lightCoof = minLight;

						out.r *= lightCoof;
						out.g *= lightCoof;
						out.b *= lightCoof;



						float shine = std::max(DotProduct(ref, view), 0.0f);
						if (shine > 0.45f)
						{
							float shineCoof = shine;

							for (int sh = 0; sh < shiness; sh++)
								shineCoof *= shine;

							if (shineCoof > last)
							{
								last = shineCoof;
							}
						}


					}
				}
				else
				{
					return olc::Pixel(in.r * minLight, in.g * minLight, in.b * minLight);
				}
				return mixColor(out, olc::WHITE, last);
				/*olc::Pixel(std::min(255, (int)floorf(out.r * (1 - last) + lightClr.r * last)),
							  std::min(255, (int)floorf(out.g * (1 - last) + lightClr.g * last)),
							  std::min(255, (int)floorf(out.b * (1 - last) + lightClr.b * last)));*/
			}

		};
		struct SmoothPlasticLightShader :public Shader
		{
		public:

			float minLight = 0.0f;
			int shiness = 10;

			SmoothPlasticLightShader()
			{
				request[0] = true;
			};
			olc::Pixel proceed(olc::Pixel in, ShaderPacket& pack) override
			{

				float last = 0.0f;

				olc::Pixel out = in;
				if ((*pack.lights).size() > 0)
				{
					for (int lig = 0; lig < (*pack.lights).size(); lig++)
					{

						//normal in pack
						vecmath::Vec3 normal = pack.normal;

						vecmath::Vec3 light;
						if ((*pack.lights)[lig].type == 0)
						{
							light = vecmath::NormaliseVec((*pack.lights)[lig].position);
						}
						else
						{
							light = vecmath::NormaliseVec((*pack.lights)[lig].position - pack.pixelPosition);
						}
						vecmath::Vec3 view = vecmath::NormaliseVec(pack.cameraPos - pack.pixelPosition);
						vecmath::Vec3 ref = vecmath::NormaliseVec(vecmath::Reflect(light, normal * (-1.0f)));

						out = in;


						float lightCoof = std::max(DotProduct(normal, light), 0.0f);

						if (lightCoof < minLight)
							lightCoof = minLight;

						out.r *= lightCoof;
						out.g *= lightCoof;
						out.b *= lightCoof;



						float shine = std::max(DotProduct(ref, view), 0.0f);
						if (shine > 0.95f)
						{
							float shineCoof = shine;

							for (int sh = 0; sh < shiness; sh++)
								shineCoof *= shine;

							if (shineCoof > last)
							{
								last = shineCoof;
							}
						}


					}
				}
				else
				{
					return olc::Pixel(in.r * minLight, in.g * minLight, in.b * minLight);
				}
				return mixColor(out, olc::WHITE, last);
				/*olc::Pixel(std::min(255, (int)floorf(out.r * (1 - last) + lightClr.r * last)),
							  std::min(255, (int)floorf(out.g * (1 - last) + lightClr.g * last)),
							  std::min(255, (int)floorf(out.b * (1 - last) + lightClr.b * last)));*/
			}

		};
		struct WaveVShader : public VertexShader
		{
		public:
			float amplitude = 1.6f;
			float freqTime = 3.09f;
			float freqXWave = 0.2f;
			float freqZWave = 0.4f;
			float timer = 0;
			float delta = 0;
			vecmath::Vec3 proceed(vecmath::Vec3 inV, vecmath::Vec3 PosOff) override
			{
				vecmath::Vec3 out = inV;
				timer += delta;
				if (timer > 1000)
					timer -= 1000;
				out.y += sinf(timer * freqTime + (out.x) * freqXWave + (out.z) * freqZWave) * amplitude;


				return out;
			}

		};
		struct WaveVSShader : public VertexShader
		{
		public:
			float amplitude = 1.6f;
			float freqTime = 3.09f;
			float freqXWave = 0.2f;
			float freqZWave = 0.4f;
			float timer = 0;
			float delta = 0;
			vecmath::Vec3 proceed(vecmath::Vec3 inV, vecmath::Vec3 PosOff) override
			{
				vecmath::Vec3 out = inV;
				timer += delta;
				if (timer > 1000)
					timer -= 1000;
				float xO = inV.x - PosOff.x;
				float zO = inV.z - PosOff.z;
				out.y += sinf(timer * freqTime + (xO * xO) * freqXWave + (zO * zO) * freqZWave) * amplitude;


				return out;
			}

		};
}

}





/*
struct Sphere: public Collider
{
	float radius;
	vecmath::Vec3 position;
	Sphere()
	{
		radius = 1;
		type = 0;
	}
	Sphere(vecmath::Vec3 pos, float r)
	{
		position = pos;
		radius = r;
	}
	bool RayCollision(vecmath::Vec3 begin, vecmath::Vec3 dir, vecmath::Vec3& outPoint) override
	{
		vecmath::Vec3 toCen = position - begin;

		float closestT = DotProduct(toCen, dir);

		vecmath::Vec3 closest = begin + dir * closestT;

		outPoint = closest;

		if (vecmath::Distance(closest, position) <= radius)
			return true;
		else return false;
	}
	void Draw(olc::PixelGameEngine gfx) override
	{
		vecmath::Vec3 viewPos = rotate3d(position - camPos, camAng, 2);
		vecmath::Vec2 screenPos = convert(viewPos);

		drawVector(position, position + vecmath::Vec3(radius, 0, 0), olc::GREEN, gfx);
		drawVector(position, position + vecmath::Vec3(-radius, 0, 0), olc::GREEN, gfx);
		drawVector(position, position + vecmath::Vec3(0, radius, 0), olc::GREEN, gfx);
		drawVector(position, position + vecmath::Vec3(0, -radius, 0), olc::GREEN, gfx);
		drawVector(position, position + vecmath::Vec3(0, 0, radius), olc::GREEN, gfx);
		drawVector(position, position + vecmath::Vec3(0, 0, -radius), olc::GREEN, gfx);

		int cenX = floorf(gfx.ScreenWidth() / 2);
		int cenY = floorf(gfx.ScreenHeight() / 2);

		gfx.DrawCircle(screenPos.x+cenX, screenPos.y+cenY,radius/(viewPos.z*zCof),olc::GREEN);
	}
	void Move(vecmath::Vec3 moveV) override
	{
		position += moveV;
	}
	vecmath::Vec3 getPosition() override
	{
		return position;
	}
	vecmath::Vec3 getSize() override
	{
		return vecmath::Vec3(radius);
	}
};
bool RaySphereCollision(vecmath::Vec3 begin, vecmath::Vec3 dir, Sphere s)
{
	vecmath::Vec3 toCen = s.position - begin;

	float closestT = DotProduct(toCen, dir);

	vecmath::Vec3 closest = begin + dir * closestT;

	if (vecmath::Distance(closest, s.position) <= s.radius)
		return true;
	else return false;

}
bool RaySphereCollision(vecmath::Vec3 begin, vecmath::Vec3 dir, Sphere s, vecmath::Vec3& outPoint)
{
	vecmath::Vec3 toCen = s.position - begin;

	float closestT = vecmath::DotProduct(toCen, dir);

	vecmath::Vec3 closest = begin + dir * closestT;

	outPoint = closest;

	if (vecmath::Distance(closest, s.position) <= s.radius)
		return true;
	else return false;

}
bool RectLineIntersection(vecmath::Vec2 rB, vecmath::Vec2 rS, vecmath::Vec2 lO, vecmath::Vec2 lD)
{
	vecmath::Vec2 enter = vecmath::Vec2(0, 0);
	float t = (rB.y - lO.y)/lD.y;
	enter.y = rB.y;
	enter.x = lO.x + lD.x * t;
	if (enter.x >= rB.x && enter.x <= rB.x + rS.x && t > 0)
	{
		return true;
	}
	else
	{
		t = (rB.y+rS.y - lO.y) / lD.y;
		enter.y = rB.y + rS.y;
		enter.x = lO.x + lD.x * t;
		if (enter.x >= rB.x && enter.x <= rB.x + rS.x && t> 0)
		{
			return true;
		}
		else
		{

			t = (rB.x - lO.x) / lD.x;
			enter.x = rB.x;
			enter.y = lO.y + lD.y * t;
			if (enter.y >= rB.y && enter.y <= rB.y + rS.y && t > 0)
			{
				return true;
			}
			else
			{
				t = (rB.x + rS.x - lO.x) / lD.x;
				enter.x = rB.x + rS.x;
				enter.y = lO.y + lD.y * t;
				if (enter.y >= rB.y && enter.y <= rB.y + rS.y && t > 0)
				{
					return true;
				}
				else
					return false;
			}

		}

	}

}
*/



//END OF RASTERIZER-----------------------------------------BEGIN OF ANIMATION--------------------------------------------------------
/*
struct Bone
{
	Mesh mesh;
	Pnt3 pos;
	Pnt3 ang;
	Pnt3 scale;
	Pnt3 globPos;
	Pnt3 globAng;
	std::vector<Pnt3> outPoint;
	Bone* parent = nullptr;
	Bone* child = nullptr;
	int id, parId, outPointId;

	Bone()
	{

	}

	Bone(Mesh m)
	{
		mesh = m;
		parent = nullptr;
		child = nullptr;
		parId = -1;
	}
};

//               child    parent
void AttachBone(Bone& b1, Bone& b2, int outPoint)
{
	b1.parent = &b2;
	b1.outPointId = outPoint;
	b2.child = &b1;
	b1.parId = b2.id;
}



struct Body
{
	std::vector<Bone> bones;
	Body()
	{

	}

};

void AddBone(Body& body, Bone& bone)
{
	body.bones.push_back(bone);
	body.bones[body.bones.size() - 1].id = body.bones.size() - 1;
}


void DrawBody(Body& b, std::vector<Triangle> &buf)
{
	bool allGotit = false;
	std::vector<bool> gotit;
	gotit.resize(b.bones.size());
	
	while (!allGotit)
	{
		allGotit = true;
		for (int i = 0; i < b.bones.size(); i++)
		{
			if (gotit[i] == false)
			{
				if (b.bones[i].parent != nullptr)
				{
					if (gotit[b.bones[i].parId] == true)
					{
						Pnt3 totalPos = sum3d(b.bones[i].parent->outPoint[b.bones[i].outPointId], b.bones[i].pos);
						Pnt3 rotatePnt = rotate3d(totalPos, b.bones[i].parent->globAng, b.bones[i].parent->mesh.rotOrder);

						b.bones[i].globPos = rotatePnt;
						b.bones[i].globPos = sum3d(b.bones[i].globPos, b.bones[i].parent->globPos);

						b.bones[i].globAng = sum3d(b.bones[i].ang, b.bones[i].parent->globAng);

						b.bones[i].mesh.pos = b.bones[i].globPos;
						b.bones[i].mesh.ang = b.bones[i].globAng;
						drawMesh(b.bones[i].mesh, buf);
						gotit[i] = true;
					}
					else
					{
						allGotit = false;
					}
						
				}
				else
				{

					b.bones[i].globPos = b.bones[i].pos;
					b.bones[i].globAng = b.bones[i].ang;

					b.bones[i].mesh.pos = b.bones[i].globPos;
					b.bones[i].mesh.ang = b.bones[i].globAng;
					drawMesh(b.bones[i].mesh, buf);
					gotit[i] = true;
				}
			}
			


		}

	}
}

void saveBody(Body b, std::string wha)
{
	std::ofstream fout;
	fout.open(wha);
	if (fout.is_open())
	{
		for (int i = 0; i < b.bones.size(); i++)
		{
			fout << "N " + b.bones[i].mesh.fileNames[0] + " " + b.bones[i].mesh.fileNames[1] + " " + //mesh and texture file names
					std::to_string(b.bones[i].outPoint.size()) + " ";//amount of attach points
			for (int k = 0; k < b.bones[i].outPoint.size(); k++)
			{
				fout << std::to_string(b.bones[i].outPoint[k].x) + " " + std::to_string(b.bones[i].outPoint[k].y) + " " + std::to_string(b.bones[i].outPoint[k].z) + " "; //attach points positions
			}
			fout <<	std::to_string(b.bones[i].mesh.rot.x) + " " + std::to_string(b.bones[i].mesh.rot.y) + " " + std::to_string(b.bones[i].mesh.rot.z) + " " + //meshs center points
					std::to_string(b.bones[i].mesh.rotOrder) + " " +
					std::to_string(b.bones[i].outPointId) + " " + std::to_string(b.bones[i].parId) + "\n";// parents and childs ids in body bones vector
		}//         attach point number where i attach

	}
	else
	{
		std::cout << "error with saving file";
	}
}

void loadBody(Body &in, std::string wha)
{
	std::ifstream fin;
	fin.open(wha);

	int counterB;
	if (fin.is_open())
	{

		std::string type;

		counterB = -1;

		std::string meshName;
		std::string texName;
		int outCnt;
		std::vector<Pnt3> outPts;
		float rotx, roty, rotz;
		int MrotOrder;
		int outId;

		int boneParId;

		in.bones.clear();

		while (!fin.eof())
		{
			type = " ";
			fin >> type;

			if (type == "N")
			{
				outPts.clear();
				counterB = counterB + 1;

				Bone newBone;
				fin >> meshName;
				fin >> texName;

				//std::cout << "got mesh: " + meshName << std::endl;
				//std::cout << "got tex: " + texName << std::endl;

				fin >> outCnt;
				for (int i = 0; i < outCnt; i++)
				{
					Pnt3 newOutPt;
					fin >> newOutPt.x;
					fin >> newOutPt.y;
					fin >> newOutPt.z;
					outPts.push_back(newOutPt);
				}
				

				//std::cout << "got outP: " + std::to_string(outx) + ";"  + std::to_string(outy) + ";" + std::to_string(outz)<< std::endl;

				fin >> rotx;
				fin >> roty;
				fin >> rotz;

				fin >> MrotOrder;
				//std::cout << "got rotP: " + std::to_string(rotx) + ";" + std::to_string(roty) + ";" + std::to_string(rotz) << std::endl;

				
				fin >> outId;
				

				fin >> boneParId;

				//std::cout << "got par: " + std::to_string(boneParId) << std::endl;

				olc::Sprite* spr;
				spr = new olc::Sprite(texName);

				Mesh mesh;
				mesh.LoadFromObjectFile(meshName, true);
				mesh.spr = spr;
				mesh.rot = Pnt3(rotx,roty,rotz);
				mesh.rotOrder = MrotOrder;
				mesh.SetFiles(meshName, texName);
				newBone = Bone(mesh);
				
				AddBone(in, newBone);

				for(int i=0;i<outCnt;i++)
					in.bones[counterB].outPoint.push_back(outPts[i]);

				in.bones[counterB].outPointId = outId;
				in.bones[counterB].id = counterB;
				in.bones[counterB].parId = boneParId;
				

				

				//std::cout << "ended with" + std::to_string(in.bones[counterB].mesh.rot.z) << std::endl;
				//std::cout << "ended with" + std::to_string(counterB) << std::endl;

				
			}
		}

		for(int i=0;i< in.bones.size();i++)
			if (in.bones[i].parId != -1)
				AttachBone(in.bones[i], in.bones[in.bones[i].parId], in.bones[i].outPointId);

		
	}
	else
	{
		std::cout << "error";
	}

}

struct AnimationKey
{
	float start, end;
	Pnt3 beginAng, ang;
	Pnt3 locker;
	int type;
	int bone;
	AnimationKey(float istart, float iend, int itype, int ibone, Pnt3 ibang, Pnt3 iang)//, Pnt3 ilocker)
	{
		start = istart;
		end = iend;
		bone = ibone;
		beginAng = ibang;
		ang = iang;
		type = itype;
		//locker = ilocker;
	}
};

struct Animation
{
	float timer;
	float speed;
	std::vector<AnimationKey> keys;
	float duration;
	Animation()
	{
		speed = 1.0f;
	}
};

void saveAnimation(Animation a, std::string wha)
{
	std::ofstream fout;
	fout.open(wha);
	if (fout.is_open())
	{
		fout << std::to_string(a.duration) + "\n";
		fout << std::to_string(a.speed) + "\n";

		for (int i = 0; i < a.keys.size(); i++)
		{
			fout << "N " + std::to_string(a.keys[i].start) + " " + std::to_string(a.keys[i].end) + " " + //time limits
				std::to_string(a.keys[i].type) + " " + //type of animation (position/rotation)
				std::to_string(a.keys[i].bone) + " " + //bone index
				//vector on the begining
				std::to_string(a.keys[i].beginAng.x) + " " + std::to_string(a.keys[i].beginAng.y) + " " + std::to_string(a.keys[i].beginAng.z) + " " +
				//delta vector
				std::to_string(a.keys[i].ang.x) + " " + std::to_string(a.keys[i].ang.y) + " " + std::to_string(a.keys[i].ang.z) + "\n";

		}

	}
	else
	{
		std::cout << "error with saving animation";
	}
}

void loadAnimation(Animation& in, std::string wha)
{
	std::ifstream fin;
	fin.open(wha);

	if (fin.is_open())
	{
		float floatVar;
		
		std::string type;
		Pnt3 vec1,vec2;
		float begin, end;
		int atype, bone;

		fin >> floatVar;
		in.duration = floatVar;

		fin >> floatVar;
		in.speed = floatVar;

		while (!fin.eof())
		{
			type = " ";
			fin >> type;

			if (type == "N")
			{
				fin >> begin;
				fin >> end;

				fin >> atype;
				fin >> bone;

				fin >> vec1.x;
				fin >> vec1.y;
				fin >> vec1.z;

				fin >> vec2.x;
				fin >> vec2.y;
				fin >> vec2.z;

				in.keys.push_back(AnimationKey(begin,end,atype,bone,vec1,vec2));

			}
		}

	}
	else
	{
		std::cout << "error";
	}

}



void playAnimation(Body& body, Animation& anim, float t)
{
	anim.timer += t*anim.speed;
	if (anim.timer > anim.duration)
		anim.timer = anim.timer - anim.duration;
	for (int i = 0; i < anim.keys.size(); i++)
	{
		if (anim.timer >= anim.keys[i].start && anim.timer <= anim.keys[i].end)
		{
			float proc = 0;
			if(anim.keys[i].end - anim.keys[i].start>0)
				proc = 1-(anim.keys[i].end- anim.timer) / (anim.keys[i].end-anim.keys[i].start);
			if (anim.keys[i].end == anim.keys[i].start)
				proc = 1;

			if (anim.keys[i].type == 0)
			{
				Pnt3 deltaAng = anim.keys[i].ang - anim.keys[i].beginAng;
				Pnt3 start = anim.keys[i].beginAng;
				body.bones[anim.keys[i].bone].ang = start + deltaAng * proc;
			}
			else
			{
				body.bones[anim.keys[i].bone].pos = mul3d(anim.keys[i].ang, proc);
				body.bones[anim.keys[i].bone].pos = sum3d(anim.keys[i].beginAng, body.bones[anim.keys[i].bone].pos);
			}
		}
	}
}


float lookAtX(vecmath::Vec3 p1, vecmath::Vec3 p2)
{
	float dz = p2.z - p1.z;
	float dy = p2.y - p1.y;
	float ang = std::atan((dz / dy));
	if (dy < 0)
		ang = ang - 3.1415f;
	return ang;
}


float lookAtY(vecmath::Vec3 p1, vecmath::Vec3 p2)
{
	float dx = p2.x - p1.x;
	float dz = p2.z - p1.z;
	float ang = std::atan((dz / dx));
	if (dx < 0)
		ang = ang - vecmath::PI;
	return ang;
}



float lookAtZ(vecmath::Vec3 p1, vecmath::Vec3 p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	float ang = std::atan((dx / dy));
	if (dy < 0)
		ang = ang - vecmath::PI;
	return ang;
}

*/