#include "Renderer.h"

#include "Window/Window.h"
#include <exception>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sstream>

std::string GetProjectDirectory()
{
	std::string fullPath(__FILE__);
	size_t found = fullPath.find_last_of("/\\");
	if (found != std::string::npos)
	{
		std::string projectDir = fullPath.substr(0, found);
		found = projectDir.find_last_of("/\\");
		if (found != std::string::npos)
		{
			return projectDir.substr(0, found + 1);
		}
	}

	return "";
}

std::string ReadHLSLFile(const std::string& fileName)
{
	std::string absolutePath = GetProjectDirectory() + "Shaders\\" + fileName;
	std::ifstream file(absolutePath);

	if(!file.is_open())
	{
		std::cerr << "Failed to open file: " << absolutePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

Renderer::Renderer(Window* pWindow) : mWindow(pWindow)
{
	CreateDevice();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateShaders();
	CreateInputLayout();
}

void Renderer::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	if(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, 
								mDevice.GetAddressOf(), nullptr, mDeviceContext.GetAddressOf())))
	{
		std::cout << "Failed to create device" << std::endl;
		abort();
	}

	std::cout << "D3D11 Device created." << std::endl;
}

void Renderer::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = mWindow->GetSize().X;
	swapChainDesc.BufferDesc.Height = mWindow->GetSize().Y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = mWindow->GetWindowNativeHandler();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	// swapChainDesc.Flags = 0;
	// swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
	ComPtr<IDXGIDevice> dxgiDevice;
	mDevice.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());

	ComPtr<IDXGIFactory> dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

	if (FAILED(dxgiFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
	{
		std::cout << "Failed to create swap chain" << std::endl;
		abort();
	}

	std::cout << "Swap chain created." << std::endl;
	
}

void Renderer::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> colorBuffer;
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&colorBuffer));

	if (FAILED(mDevice->CreateRenderTargetView(colorBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf())))
	{
		std::cout << "Failed to create render target view" << std::endl;
		abort();
	}

	std::cout << "Render target view created." << std::endl;
}

void Renderer::CreateShaders()
{
	ComPtr<ID3DBlob> errorBlob;

	const std::string pixelShaderCode = ReadHLSLFile("PixelShader.hlsl");
	D3DCompile(pixelShaderCode.c_str(), pixelShaderCode.length(), nullptr, nullptr, nullptr, 
		"main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, mVertexShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob.Get() != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		std::cerr << static_cast<char*>(errorBlob->GetBufferPointer()) << std::endl;
		abort();
	}

	if(FAILED(mDevice->CreatePixelShader(mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf())))
	{
		std::cerr << "Failed to compile Pixel shader" << std::endl;
		abort();
	}
		
	const std::string vertexShaderCode = ReadHLSLFile("VertexShader.hlsl");
	D3DCompile(vertexShaderCode.c_str(), vertexShaderCode.length(), nullptr, nullptr, nullptr,
		"main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, mVertexShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob.Get() != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		std::cerr << static_cast<char*>(errorBlob->GetBufferPointer()) << std::endl;
		abort();
	}

	if (FAILED(mDevice->CreateVertexShader(mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf())))
	{
		std::cerr << "Failed to compile Vertex shader" << std::endl;
		abort();
	}
}

void Renderer::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(mDevice->CreateInputLayout(inputElementDescs, 2, mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), mInputLayout.GetAddressOf())))
	{
		std::cout << "Failed to create input layout" << std::endl;
		abort();
	}

	std::cout << "Input layout created." << std::endl;
}

void Renderer::SetPipeline()
{
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->IASetInputLayout(mInputLayout.Get());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(mWindow->GetSize().X);
	viewport.Height = static_cast<float>(mWindow->GetSize().Y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	mDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::ClearColor(XMFLOAT3 color)
{
	float clearColor[] = { color.x, color.y, color.z, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), clearColor);
}

void Renderer::Draw(uint32_t indexCount)
{
	mDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void Renderer::Present()
{
	mSwapChain->Present(1, 0);
}
