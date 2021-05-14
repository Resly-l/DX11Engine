#pragma once

// windows
#define NOMINMAX
#include <windows.h>

// math
#include "vector.h"
#include "matrix.h"
#include "bounding_box.h"

// standard library
#include <list>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>
#include <fstream>
#include <filesystem>
#include <utility>
#include <optional>
#include <functional>
#include <memory>

// DirectX
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

typedef DirectX::XMFLOAT2 float2;
typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

#include "imnodes/imnodes.h"

// assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#pragma comment (lib, "assimp-vc142-mt.lib")

// json
#include "json/json.hpp"
using JSON = nlohmann::json;

namespace nlohmann
{
	template <>
	struct adl_serializer<Vector>
	{
		static void to_json(json& j, const Vector& v)
		{
			j["x"] = v.x;
			j["y"] = v.y;
			j["z"] = v.z;
			j["w"] = v.w;
		}

		static void from_json(const json& j, Vector& v)
		{
			v.x = j.at("x");
			v.y = j.at("y");
			v.z = j.at("z");
			v.w = j.at("w");
		}
	};
	template <>
	struct adl_serializer<Matrix>
	{
		static void to_json(json& j, const Matrix& m)
		{
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					j.push_back(m[x][y]);
				}
			}
		}

		static void from_json(const json& j, Matrix& m)
		{
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					m[x][y] = j[size_t(x) * 4 + y];
				}
			}
		}
	};
}