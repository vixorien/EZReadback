#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>


struct EZColor1
{
	unsigned char RedInt;

	EZColor1() : RedInt{ 0 } {}

	EZColor1(unsigned char r) : RedInt{ r } {}

	inline float AsFloat1() { return RedInt / 255.0f; }
};

struct EZColor2
{
	unsigned char RedInt;
	unsigned char GreenInt;

	EZColor2() : RedInt{ 0 }, GreenInt{ 0 } {}

	EZColor2(unsigned char r, unsigned char g) :
		RedInt{ r }, GreenInt{ g } {}

	inline float AsFloat1() { return RedInt / 255.0f; }
	inline DirectX::XMFLOAT2 AsFloat2() { return DirectX::XMFLOAT2(RedInt / 255.0f, GreenInt / 255.0f); }
};

struct EZColor3
{
	unsigned char RedInt;
	unsigned char GreenInt;
	unsigned char BlueInt;

	EZColor3() : RedInt{ 0 }, GreenInt{ 0 }, BlueInt{ 0 } {}

	EZColor3(unsigned char r, unsigned char g, unsigned char b) :
		RedInt{ r }, GreenInt{ g }, BlueInt{ b } {}

	inline float AsFloat1() { return RedInt / 255.0f; }
	inline DirectX::XMFLOAT2 AsFloat2() { return DirectX::XMFLOAT2(RedInt / 255.0f, GreenInt / 255.0f); }
	inline DirectX::XMFLOAT3 AsFloat3() { return DirectX::XMFLOAT3(RedInt / 255.0f, GreenInt / 255.0f, BlueInt / 255.0f); }
};

struct EZColor4
{
	unsigned char RedInt;
	unsigned char GreenInt;
	unsigned char BlueInt;
	unsigned char AlphaInt;

	EZColor4() : RedInt{ 0 }, GreenInt{ 0 }, BlueInt{ 0 }, AlphaInt{ 0 } {}

	EZColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
		RedInt{ r }, GreenInt{ g }, BlueInt{ b }, AlphaInt{ a } {}

	inline float AsFloat1() { return RedInt / 255.0f; }
	inline DirectX::XMFLOAT2 AsFloat2() { return DirectX::XMFLOAT2(RedInt / 255.0f, GreenInt / 255.0f); }
	inline DirectX::XMFLOAT3 AsFloat3() { return DirectX::XMFLOAT3(RedInt / 255.0f, GreenInt / 255.0f, BlueInt / 255.0f); }
	inline DirectX::XMFLOAT4 AsFloat4() { return DirectX::XMFLOAT4(RedInt / 255.0f, GreenInt / 255.0f, BlueInt / 255.0f, AlphaInt / 255.0f); }
};

class EZReadback
{

public:

	EZReadback(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		this->device = device;
		this->context = context;
	}

	template<typename ElementType>
	HRESULT ReadBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, std::vector<ElementType>& results);

	template<typename ElementType>
	HRESULT ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11Texture3D> texture, std::vector<ElementType>& results, UINT mipLevel = 0);



	template<typename ElementType>
	HRESULT ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel = 0);



	template<typename ElementType>
	HRESULT ReadBuffer(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results);

	template<typename ElementType>
	HRESULT ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel = 0);


	template<typename ElementType>
	HRESULT ReadBuffer(ID3D11View* view, std::vector<ElementType>& results);

	template<typename ElementType>
	HRESULT ReadTexture1D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture2D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT ReadTexture3D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel = 0);

private:

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// Private helper for actually reading any type of resource
	// - A bit ugly due to all the templating, but it simplifies a lot
	template<typename ResourceType, typename DescriptionType, typename ElementType>
	HRESULT ReadResource(
		Microsoft::WRL::ComPtr<ResourceType> resource,
		DescriptionType* desc,
		size_t elementCount,
		UINT subresourceIndex,
		std::vector<ElementType>& results);

	// Function overloads based on template type
	inline HRESULT CreateResource(void* desc, ...) { return E_INVALIDARG; }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Buffer>& result) { return device->CreateBuffer(static_cast<D3D11_BUFFER_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture1D>& result) { return device->CreateTexture1D(static_cast<D3D11_TEXTURE1D_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture2D>& result) { return device->CreateTexture2D(static_cast<D3D11_TEXTURE2D_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture3D>& result) { return device->CreateTexture3D(static_cast<D3D11_TEXTURE3D_DESC*>(desc), 0, result.GetAddressOf()); }

	// Helper for size of formats
	inline size_t BitsPerPixel(DXGI_FORMAT format);
};

template<typename ElementType>
HRESULT EZReadback::ReadBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, std::vector<ElementType>& results)
{
	// Grab the description and adjust it
	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	// Calculate buffer size
	size_t dataCount = desc.ByteWidth / sizeof(ElementType);

	// Do the readback
	return ReadResource<ID3D11Buffer, D3D11_BUFFER_DESC, ElementType>(buffer, &desc, dataCount, 0, results);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	// Grab the description and adjust it
	D3D11_TEXTURE1D_DESC desc;
	texture->GetDesc(&desc);

	// Validate size of T
	if (sizeof(ElementType) != BitsPerPixel(desc.Format) / 8)
		return E_INVALIDARG;

	// Calculate mip size and subresource
	size_t mipWidth = max(desc.Width >> mipLevel, 1);
	UINT srIndex = D3D11CalcSubresource(mipLevel, arrayIndex, desc.MipLevels);

	// Do the readback
	return ReadResource<ID3D11Texture1D, D3D11_TEXTURE1D_DESC, ElementType>(
		texture, &desc, mipWidth, srIndex, results);

}

template<typename ElementType>
HRESULT EZReadback::ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	// Grab the description and adjust it
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	// Validate size of T
	if (sizeof(ElementType) != BitsPerPixel(desc.Format) / 8)
		return E_INVALIDARG;

	// Calculate mip size and subresource
	size_t mipWidth = max(desc.Width >> mipLevel, 1);
	size_t mipHeight = max(desc.Height >> mipLevel, 1);
	UINT srIndex = D3D11CalcSubresource(mipLevel, arrayIndex, desc.MipLevels);

	// Do the readback
	return ReadResource<ID3D11Texture2D, D3D11_TEXTURE2D_DESC, ElementType>(
		texture, &desc, mipWidth * mipHeight, srIndex, results);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11Texture3D> texture, std::vector<ElementType>& results, UINT mipLevel)
{
	// Grab the description and adjust it
	D3D11_TEXTURE3D_DESC desc;
	texture->GetDesc(&desc);

	// Validate size of T
	if (sizeof(ElementType) != BitsPerPixel(desc.Format) / 8)
		return E_INVALIDARG;

	// Calculate mip size and subresource
	size_t mipWidth = max(desc.Width >> mipLevel, 1);
	size_t mipHeight = max(desc.Height >> mipLevel, 1);
	size_t mipDepth = max(desc.Depth >> mipLevel, 1);
	UINT srIndex = D3D11CalcSubresource(mipLevel, 0, desc.MipLevels);

	// Do the readback
	return ReadResource<ID3D11Texture3D, D3D11_TEXTURE3D_DESC, ElementType>(
		texture, &desc, mipWidth * mipHeight * mipDepth, srIndex, results);
}



template<typename ElementType>
HRESULT EZReadback::ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	return ReadTexture1D<ElementType>(srv.Get(), results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	return ReadTexture2D<ElementType>(srv.Get(), results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::vector<ElementType>& results, UINT mipLevel)
{
	return ReadTexture3D<ElementType>(srv.Get(), results, mipLevel);
}



template<typename ElementType>
HRESULT EZReadback::ReadBuffer(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results)
{
	return ReadBuffer<ElementType>(uav.Get(), results);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture1D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	return ReadTexture1D<ElementType>(uav.Get(), results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture2D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	return ReadTexture2D<ElementType>(uav.Get(), results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture3D(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav, std::vector<ElementType>& results, UINT mipLevel)
{
	return ReadTexture3D<ElementType>(uav.Get(), results, mipLevel);
}



template<typename ElementType>
HRESULT EZReadback::ReadBuffer(ID3D11View* view, std::vector<ElementType>& results)
{
	// Grab the underlying resource
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	view->GetResource((ID3D11Resource**)buffer.GetAddressOf());

	// Use the other overload to finish the job
	return ReadBuffer<ElementType>(buffer, results);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture1D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	// Grab the underlying resource
	Microsoft::WRL::ComPtr<ID3D11Texture1D> texture;
	view->GetResource((ID3D11Resource**)texture.GetAddressOf());

	// Use the other overload to finish the job
	return ReadTexture1D<ElementType>(texture, results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture2D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	// Grab the underlying resource
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	view->GetResource((ID3D11Resource**)texture.GetAddressOf());

	// Use the other overload to finish the job
	return ReadTexture2D<ElementType>(texture, results, mipLevel, arrayIndex);
}

template<typename ElementType>
HRESULT EZReadback::ReadTexture3D(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel)
{
	// Grab the underlying resource
	Microsoft::WRL::ComPtr<ID3D11Texture3D> texture;
	view->GetResource((ID3D11Resource**)texture.GetAddressOf());

	// Use the other overload to finish the job
	return ReadTexture3D<ElementType>(texture, results, mipLevel);
}


template<typename ResourceType, typename DescriptionType, typename ElementType>
HRESULT EZReadback::ReadResource(
	Microsoft::WRL::ComPtr<ResourceType> resource, 
	DescriptionType* desc, 
	size_t elementCount, 
	UINT subresourceIndex,
	std::vector<ElementType>& results)
{
	// Update description for staging resource
	desc->CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc->Usage          = D3D11_USAGE_STAGING;
	desc->BindFlags      = 0; // No binding for staging resource
	desc->MiscFlags      &= ~(D3D11_RESOURCE_MISC_GENERATE_MIPS); // No mip gen

	// Create a new texture that is readable by the CPU
	Microsoft::WRL::ComPtr<ResourceType> readable;
	HRESULT create = CreateResource(desc, readable);
	
	// Make sure the resource was created
	if (FAILED(create))
		return create;

	// Copy the resource
	context->CopyResource(readable.Get(), resource.Get());

	// Map the resource
	D3D11_MAPPED_SUBRESOURCE gpu = {};
	HRESULT map = context->Map(readable.Get(), subresourceIndex, D3D11_MAP_READ, 0, &gpu);
	if (FAILED(map))
		return map;

	// Resize the vector and read into it
	results.resize(elementCount);
	memcpy(&results[0], gpu.pData, sizeof(ElementType) * elementCount);

	// Clean up
	context->Unmap(readable.Get(), subresourceIndex);
	return S_OK;
}




inline size_t EZReadback::BitsPerPixel(DXGI_FORMAT format)
{
	switch (format)
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