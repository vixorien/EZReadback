#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

// --- EZColor1 -------------------------------------------
//  Represents a 1-component R color, which is an 8-bit 
//  unsigned integer in the 0-255 range. Equivalent to the 
//  DXGI_FORMAT_R8_UNORM color format in D3D.
//  
//  Contains a function to convert to a float in the
//  0.0 - 1.0 range.
// --------------------------------------------------------
struct EZColor1
{
	unsigned char RedInt;

	EZColor1() : RedInt{ 0 } {}

	EZColor1(unsigned char r) : RedInt{ r } {}

	inline float AsFloat1() { return RedInt / 255.0f; }
};

// --- EZColor2 -------------------------------------------
//  Represents a 2-component RG color, where each 
//  component is an 8-bit unsigned integer in the 0-255 
//  range. Equivalent to the DXGI_FORMAT_R8G8_UNORM 
//  color format in D3D.
//  
//  Contains functions to convert to float vectors of
//  various sizes in which channel values are 0.0 - 1.0
// --------------------------------------------------------
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

// --- EZColor3 -------------------------------------------
//  Represents a 3-component RGB color, where each 
//  component is an 8-bit unsigned integer in the 0-255 
//  range. There is no equivalent color format in D3D, but
//  this is included for completeness as it may be useful
//  for custom buffers.
//  
//  Contains functions to convert to float vectors of
//  various sizes in which channel values are 0.0 - 1.0
// --------------------------------------------------------
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

// --- EZColor4 -------------------------------------------
//  Represents a 4-component RGBA color, where each 
//  component is an 8-bit unsigned integer in the 0-255 
//  range. Equivalent to the DXGI_FORMAT_R8G8B8A8_UNORM 
//  color format in D3D.
//  
//  Contains functions to convert to float vectors of
//  various sizes in which channel values are 0.0 - 1.0
// --------------------------------------------------------
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

	// --- Constructor ---------------------------------------
	//  Creates the EZReadback object holding D3D objects
	//
	//  - device: the D3D device for creating resources
	//  - context: the D3D context for copying resources
	// -------------------------------------------------------
	EZReadback(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		this->device = device;
		this->context = context;
	}

	// Read functions that populate a given vector
	template<typename ElementType>
	HRESULT Read(ID3D11Resource* resource, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	HRESULT Read(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel = 0, UINT arrayIndex = 0);

	// Read functions that return a new vector
	template<typename ElementType>
	std::vector<ElementType> Read(ID3D11Resource* resource, UINT mipLevel = 0, UINT arrayIndex = 0);

	template<typename ElementType>
	std::vector<ElementType> Read(ID3D11View* view, UINT mipLevel = 0, UINT arrayIndex = 0);


private:

	// D3D objects for resource creation and manipulation
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// Private helper to perform actual readback from GPU resource
	template<typename ResourceType, typename DescriptionType, typename ElementType>
	HRESULT StageAndCopyResource(
		ResourceType* resource,
		UINT mipLevel,
		UINT arrayIndex,
		std::vector<ElementType>& results);

	// Function overloads based on template type
	inline HRESULT CreateResource(void* desc, ...) { return E_INVALIDARG; }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Buffer>& result) { return device->CreateBuffer(static_cast<D3D11_BUFFER_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture1D>& result) { return device->CreateTexture1D(static_cast<D3D11_TEXTURE1D_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture2D>& result) { return device->CreateTexture2D(static_cast<D3D11_TEXTURE2D_DESC*>(desc), 0, result.GetAddressOf()); }
	inline HRESULT CreateResource(void* desc, Microsoft::WRL::ComPtr<ID3D11Texture3D>& result) { return device->CreateTexture3D(static_cast<D3D11_TEXTURE3D_DESC*>(desc), 0, result.GetAddressOf()); }

	// Helpers for calculating the number of elements in a resource
	template<typename DescriptionType> inline UINT CalcElementCount(DescriptionType* desc, size_t elementSize, UINT mipLevel);
	template<> inline UINT CalcElementCount<D3D11_BUFFER_DESC>(D3D11_BUFFER_DESC* desc, size_t elementSize, UINT mipLevel);
	template<> inline UINT CalcElementCount<D3D11_TEXTURE1D_DESC>(D3D11_TEXTURE1D_DESC* desc, size_t elementSize, UINT mipLevel);
	template<> inline UINT CalcElementCount<D3D11_TEXTURE2D_DESC>(D3D11_TEXTURE2D_DESC* desc, size_t elementSize, UINT mipLevel);
	template<> inline UINT CalcElementCount<D3D11_TEXTURE3D_DESC>(D3D11_TEXTURE3D_DESC* desc, size_t elementSize, UINT mipLevel);

	// Helpers for calculating subresource index of different resource types
	template<typename DescriptionType> inline UINT CalcSubresourceIndex(DescriptionType* desc, UINT mipLevel, UINT arrayIndex);
	template<> inline UINT CalcSubresourceIndex<D3D11_BUFFER_DESC>(D3D11_BUFFER_DESC* desc, UINT mipLevel, UINT arrayIndex);

	// Helper for size of formats
	inline size_t BitsPerPixel(DXGI_FORMAT format);
};


// --- Read functions for resources ----------------------
//  Reads data of the specified ElementType from a 
//  specific subresource of the given resource and 
//  populates the given vector with results.  Note that
//  ID3D11Buffers do not have explicit subresources and
//  will always read the entire resource (ignoring mipLevel
//  and arrayIndex parameters).  Likewise, ID3D11Texture3D 
//  resources do not support arrays, so arrayIndex is ignored.
// 
//  ElementType:
//  - Expected data type to read from the resource.  Note that
//    this must match the size of the actual elements in the
//    resource or the function will fail.
// 
//  Valid resource types are: 
//  - ID3D11Buffer
//  - ID3D11Texture1D
//  - ID3D11Texture2D
//  - ID3D11Texture3D
// 
//  Parameters:
//  - resource: GPU resource to read
//  - results: vector to fill with data
//  - mipLevel: mip level to read (for texture resources)
//  - arrayIndex: array element to read (for 1D/2D textures)
// 
// 	Returns:
//  - Return value is an HRESULT from any failed D3D calls 
//    or S_OK if all D3D calls were successful.
// -------------------------------------------------------
template<typename ElementType>
HRESULT EZReadback::Read(ID3D11Resource* resource, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);
	switch (type)
	{
	case D3D11_RESOURCE_DIMENSION_BUFFER: 
		return StageAndCopyResource<ID3D11Buffer, D3D11_BUFFER_DESC, ElementType>(static_cast<ID3D11Buffer*>(resource), mipLevel, arrayIndex, results);

	case D3D11_RESOURCE_DIMENSION_TEXTURE1D: 
		return StageAndCopyResource<ID3D11Texture1D, D3D11_TEXTURE1D_DESC, ElementType>(static_cast<ID3D11Texture1D*>(resource), mipLevel, arrayIndex, results);

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D: 
		return StageAndCopyResource<ID3D11Texture2D, D3D11_TEXTURE2D_DESC, ElementType>(static_cast<ID3D11Texture2D*>(resource), mipLevel, arrayIndex, results);

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D: 
		return StageAndCopyResource<ID3D11Texture3D, D3D11_TEXTURE3D_DESC, ElementType>(static_cast<ID3D11Texture3D*>(resource), mipLevel, arrayIndex, results);
	}

	return E_INVALIDARG;
}


// --- Read functions for views ---------------------------
//  Reads data of the specified ElementType from a 
//  specific subresource the resource in the given view and 
//  populates the given vector with results.  Note that
//  ID3D11Buffers do not have explicit subresources and
//  will always read the entire resource (ignoring mipLevel
//  and arrayIndex parameters).  Likewise, ID3D11Texture3D 
//  resources do not support arrays, so arrayIndex is ignored.
// 
//  ElementType:
//  - Expected data type to read from the resource.  Note that
//    this must match the size of the actual elements in the
//    resource or the function will fail.
// 
//  Valid view types are: 
//  - ID3D11ShaderResourceView
//  - ID3D11UnorderedAccessView
// 
//  Parameters:
//  - resource: GPU resource to read
//  - results: vector to fill with data
//  - mipLevel: mip level to read (for texture resources)
//  - arrayIndex: array element to read (for 1D/2D textures)
// 
// 	Returns:
//  - Return value is an HRESULT from any failed D3D calls 
//    or S_OK if all D3D calls were successful.
// -------------------------------------------------------
template<typename ElementType>
HRESULT EZReadback::Read(ID3D11View* view, std::vector<ElementType>& results, UINT mipLevel, UINT arrayIndex)
{
	ID3D11Resource* resource;
	view->GetResource(&resource);
	return Read(resource, results, mipLevel, arrayIndex);
}


// --- Read functions for resources ----------------------
//  Reads data of the specified ElementType from a 
//  specific subresource of the given resource and 
//  returns a new vector with the results.  Note that
//  ID3D11Buffers do not have explicit subresources and
//  will always read the entire resource (ignoring mipLevel
//  and arrayIndex parameters).  Likewise, ID3D11Texture3D 
//  resources do not support arrays, so arrayIndex is ignored.
// 
//  ElementType:
//  - Expected data type to read from the resource.  Note that
//    this must match the size of the actual elements in the
//    resource or the function will fail.
// 
//  Valid resource types are: 
//  - ID3D11Buffer
//  - ID3D11Texture1D
//  - ID3D11Texture2D
//  - ID3D11Texture3D
// 
//  Parameters:
//  - resource: GPU resource to read
//  - mipLevel: mip level to read (for texture resources)
//  - arrayIndex: array element to read (for 1D/2D textures)
// 
// 	Returns:
//  - A new vector with the read results.  If the read 
//    fails, the vector will be empty.
// -------------------------------------------------------
template<typename ElementType>
std::vector<ElementType> EZReadback::Read(ID3D11Resource* resource, UINT mipLevel, UINT arrayIndex)
{
	std::vector<ElementType> data;
	Read(resource, data, mipLevel, arrayIndex);
	return data;
}


// --- Read functions for resources -----------------------
//  Reads data of the specified ElementType from a specific
//  subresource of the resource in the given view and 
//  returns a new vector with the results.  Note that
//  ID3D11Buffers do not have explicit subresources and
//  will always read the entire resource (ignoring mipLevel
//  and arrayIndex parameters).  Likewise, ID3D11Texture3D 
//  resources do not support arrays, so arrayIndex is ignored.
// 
//  ElementType:
//  - Expected data type to read from the resource.  Note that
//    this must match the size of the actual elements in the
//    resource or the function will fail.
// 
//  Valid resource types are: 
//  - ID3D11Buffer
//  - ID3D11Texture1D
//  - ID3D11Texture2D
//  - ID3D11Texture3D
// 
//  Parameters:
//  - resource: GPU resource to read
//  - mipLevel: mip level to read (for texture resources)
//  - arrayIndex: array element to read (for 1D/2D textures)
// 
// 	Returns:
//  - A new vector with the read results.  If the read 
//    fails, the vector will be empty.
// --------------------------------------------------------
template<typename ElementType>
std::vector<ElementType> EZReadback::Read(ID3D11View* view, UINT mipLevel, UINT arrayIndex)
{
	std::vector<ElementType> data;
	Read(view, data, mipLevel, arrayIndex);
	return data;
}



// --- StageAndCopyResource -------------------------------
//  Private helper function to create a staging resource
//  for CPU readback, copy data from the specified resource
//  and finally read the data from the GPU.
// --------------------------------------------------------
template<typename ResourceType, typename DescriptionType, typename ElementType>
HRESULT EZReadback::StageAndCopyResource(
	ResourceType* resource, 
	UINT mipLevel,
	UINT arrayIndex,
	std::vector<ElementType>& results)
{
	// Update description for staging resource
	DescriptionType desc;
	resource->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage          = D3D11_USAGE_STAGING;
	desc.BindFlags      = 0; // No binding for staging resource
	desc.MiscFlags      &= ~(D3D11_RESOURCE_MISC_GENERATE_MIPS); // No mip gen

	// Calculate the element count and subresource index
	size_t elementCount = CalcElementCount<DescriptionType>(&desc, sizeof(ElementType), mipLevel);
	UINT subresourceIndex = CalcSubresourceIndex(&desc, mipLevel, arrayIndex);

	// Create a new texture that is readable by the CPU
	Microsoft::WRL::ComPtr<ResourceType> readable;
	HRESULT create = CreateResource(&desc, readable);
	
	// Make sure the resource was created
	if (FAILED(create))
		return create;

	// Copy the resource
	context->CopyResource(readable.Get(), resource);

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

// --- CalcElementCount ----------------------------------
//  Calculates the element count of a subresource based on
//  the description and either the elementSize or mipLevel
// 
//  The default implementation returns zero as a each
//  resource type calculates this in different ways.
// --------------------------------------------------------
template<typename DescriptionType> 
inline UINT EZReadback::CalcElementCount(DescriptionType* desc, size_t elementSize, UINT mipLevel)
{ 
	return 0; 
}

// --- CalcElementCount ----------------------------------
//  Calculates the element count of a buffer
// --------------------------------------------------------
template<> inline UINT EZReadback::CalcElementCount<D3D11_BUFFER_DESC>(D3D11_BUFFER_DESC* desc, size_t elementSize, UINT mipLevel)
{
	return desc->ByteWidth / elementSize;
}

// --- CalcElementCount ----------------------------------
//  Calculates the element count the specified mip of a 1D texture
// --------------------------------------------------------
template<> inline UINT EZReadback::CalcElementCount<D3D11_TEXTURE1D_DESC>(D3D11_TEXTURE1D_DESC* desc, size_t elementSize, UINT mipLevel)
{
	return max(desc->Width >> mipLevel, 1);
}

// --- CalcElementCount ----------------------------------
//  Calculates the element count the specified mip of a 2D texture
// --------------------------------------------------------
template<> inline UINT EZReadback::CalcElementCount<D3D11_TEXTURE2D_DESC>(D3D11_TEXTURE2D_DESC* desc, size_t elementSize, UINT mipLevel)
{
	return max(desc->Width >> mipLevel, 1) * max(desc->Height >> mipLevel, 1);
}

// --- CalcElementCount ----------------------------------
//  Calculates the element count the specified mip of a 3D texture
// --------------------------------------------------------
template<> inline UINT EZReadback::CalcElementCount<D3D11_TEXTURE3D_DESC>(D3D11_TEXTURE3D_DESC* desc, size_t elementSize, UINT mipLevel)
{
	return max(desc->Width >> mipLevel, 1) * max(desc->Height >> mipLevel, 1) * max(desc->Depth >> mipLevel, 1);
}


// --- CalcSubresourceIndex -------------------------------
//  Calculates the subresource index for a resource with
//  the given description.  Buffers are specifically handled
//  with another template specialization below.
// --------------------------------------------------------
template<typename DescriptionType> inline UINT EZReadback::CalcSubresourceIndex(DescriptionType* desc, UINT mipLevel, UINT arrayIndex)
{
	return D3D11CalcSubresource(mipLevel, arrayIndex, desc->MipLevels);
}

// --- CalcSubresourceIndex -------------------------------
//  Buffers do not contain subresources, so this always
//  returns zero for buffers specifically.
// --------------------------------------------------------
template<> inline UINT EZReadback::CalcSubresourceIndex<D3D11_BUFFER_DESC>(D3D11_BUFFER_DESC* desc, UINT mipLevel, UINT arrayIndex)
{
	return 0;
}


// --- BitsPerPixel ---------------------------------------
//  Returns the bits per pixel of the specified format
// --------------------------------------------------------
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