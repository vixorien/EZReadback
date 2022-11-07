#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

struct EZColor4
{
	unsigned char RedInt;
	unsigned char GreenInt;
	unsigned char BlueInt;
	unsigned char AlphaInt;

	EZColor4() :
		RedInt{ 0 },
		GreenInt{ 0 },
		BlueInt{ 0 },
		AlphaInt{ 0 }
	{}

	EZColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
		RedInt(r),
		GreenInt(g),
		BlueInt(b),
		AlphaInt(a)
	{}

	DirectX::XMFLOAT4 AsFloat4()
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMVECTOR colorVec = DirectX::XMVectorSet(
			static_cast<float>(RedInt),
			static_cast<float>(GreenInt),
			static_cast<float>(BlueInt),
			static_cast<float>(AlphaInt));
		DirectX::XMStoreFloat4(
			&color, 
			DirectX::XMVectorDivide(colorVec, DirectX::XMVectorReplicate(255.0f)));
		return color;
	}
};

class EZReadback
{
public:

	EZReadback(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		this->device = device;
		this->context = context;
	}

	template<typename T>
	std::vector<T> ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, unsigned int mipLevel = 0, unsigned int arrayIndex = 0);

	template<typename T>
	std::vector<T> ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, unsigned int mipLevel = 0, unsigned int arrayIndex = 0);

private:

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	size_t BitsPerPixel(DXGI_FORMAT format);
};

template<typename T>
std::vector<T> EZReadback::ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, unsigned int mipLevel, unsigned int arrayIndex)
{
	// Eventual return value
	std::vector<T> vec;

	// Grab the description and adjust it
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0; // No binding for staging resource
	desc.MiscFlags &= ~(D3D11_RESOURCE_MISC_GENERATE_MIPS); // No mip gen

	// Create a new texture that is readable by the CPU
	Microsoft::WRL::ComPtr<ID3D11Texture2D> readableTexture;
	HRESULT create = device->CreateTexture2D(&desc, 0, readableTexture.GetAddressOf());
	if (FAILED(create))
		return vec;

	// Copy the resource
	context->CopyResource(readableTexture.Get(), texture.Get());

	// Map the resource
	D3D11_MAPPED_SUBRESOURCE gpu = {};
	unsigned int subresourceIndex = D3D11CalcSubresource(mipLevel, arrayIndex, desc.MipLevels);
	HRESULT map = context->Map(readableTexture.Get(), subresourceIndex, D3D11_MAP_READ, 0, &gpu);
	if (FAILED(map))
		return vec;

	// Resize the vector and read into it
	size_t mipWidth = max(desc.Width >> mipLevel, 1);
	size_t mipHeight = max(desc.Height >> mipLevel, 1);
	vec.resize(mipWidth * mipHeight);
	memcpy(&vec[0], gpu.pData, sizeof(T) * mipWidth * mipHeight);

	// Clean up
	context->Unmap(readableTexture.Get(), subresourceIndex);
	return vec;
}

template<typename T>
std::vector<T> EZReadback::ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, unsigned int mipLevel, unsigned int arrayIndex)
{
	// Grab the underlying resource
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	srv->GetResource((ID3D11Resource**)texture.GetAddressOf());
	
	// Use the other overload to finish the job
	return ReadTexture2D<T>(texture, mipLevel, arrayIndex);
}

// From the DirectX tool kit!
size_t EZReadback::BitsPerPixel(DXGI_FORMAT format)
{
	switch (static_cast<int>(format))
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 128;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 96;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_Y416:
	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		return 64;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
	case DXGI_FORMAT_AYUV:
	case DXGI_FORMAT_Y410:
	case DXGI_FORMAT_YUY2:
		return 32;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
		return 24;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		return 16;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
	case DXGI_FORMAT_NV11:
		return 12;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
		return 8;

	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return 4;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return 8;

	default:
		return 0;
	}
}