#include <array>
#include <d3d11.h>
#include <string>

using namespace std;

struct SimpleVertex
{
	float pos[3];
	float nrml[3];
	float uv[2];

	SimpleVertex(const std::array<float, 3>& position, const std::array<float, 3>& normal, const std::array<float, 2>& uv_coords)
	{
		for (int i = 0; i < 3; ++i)
		{
			pos[i] = position[i];
			nrml[i] = normal[i];
		}

		uv[0] = uv_coords[0];
		uv[1] = uv_coords[1];
	}
};

bool SetupPipeline(ID3D11Device * device, ID3D11Buffer *& vertexBuffer, ID3D11VertexShader *& vShader, ID3D11PixelShader *& pShader, ID3D11InputLayout *& inputLayout, 
	               ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, ID3D11SamplerState*& samplerState);
bool LoadTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, const char* filename);
bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);

