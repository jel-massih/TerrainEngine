#include "terrainclass.h"

TerrainClass::TerrainClass()
{
	m_vertices = 0;
	m_heightMap = 0;
	m_texture = 0;
}

TerrainClass::TerrainClass(const TerrainClass& other)
{
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device* device, char* heightMapFilename, WCHAR* textureFilename)
{
	if(!LoadHeightMap(heightMapFilename))
	{
		return false;
	}

	NormalizeHeightMap();

	if(!CalculateNormals())
	{
		return false;
	}

	CalculateTextureCoordinates();

	if(!LoadTexture(device, textureFilename))
	{
		if(!LoadTexture(device, L"default.dds"))
		{
			return false;
		}
	}

	if(!InitializeBuffers(device))
	{
		return false;
	}

	return true;
}

void TerrainClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();

	ShutdownHeightMap();

	return;
}

int TerrainClass::GetVertexCount()
{
	return m_vertexCount;
}

ID3D11ShaderResourceView* TerrainClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	int index, i, j;
	int index1, index2, index3, index4;
	float tu, tv;

	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	m_vertices = new VertexType[m_vertexCount];
	if(!m_vertices)
	{
		return false;
	}

	index = 0;
	
	for(i = 0; i < m_terrainHeight-1; i++)
	{
		for(j=0; j < m_terrainWidth-1; j++)
		{
			index1 = (m_terrainHeight*i) + j;
			index2 = (m_terrainHeight*i) + (j+1);
			index3 = (m_terrainHeight*(i+1)) + j;
			index4 = (m_terrainHeight*(i+1)) + (j+1);

			tv = m_heightMap[index3].tv;

			if(tv == 1.0f) {tv = 0.0f;}

			m_vertices[index].position = XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			m_vertices[index].texture = XMFLOAT2(m_heightMap[index3].tu, tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			index++;

			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			if(tu == 0.0f) {tu = 1.0f;}
			if(tv == 1.0f) {tv = 0.0f;}

			m_vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			m_vertices[index].texture = XMFLOAT2(tu, tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			index++;

			m_vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			m_vertices[index].texture = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			index++;

			m_vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			m_vertices[index].texture = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			index++;

			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			if(tu == 0.0f) {tu = 1.0f;}
			if(tv == 1.0f) {tv = 0.0f;}

			m_vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			m_vertices[index].texture = XMFLOAT2(tu, tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			index++;

			tu = m_heightMap[index2].tu;

			if(tu == 0.0f) {tu = 1.0f;}

			m_vertices[index].position = XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			m_vertices[index].texture = XMFLOAT2(tu, m_heightMap[index2].tv);
			m_vertices[index].normal = XMFLOAT3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			index++;
		}
	}

	return true;
}

void TerrainClass::ShutdownBuffers()
{
	if(m_vertices)
	{
		delete [] m_vertices;
		m_vertices = 0;
	}

	return;
}

void TerrainClass::CopyVertexArray(void* vertexList)
{
	memcpy(vertexList, m_vertices, sizeof(VertexType) * m_vertexCount);
	return;
}

bool TerrainClass::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize;
	unsigned char* bitmapImage;
	unsigned char height;

	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_terrainWidth * m_terrainHeight * 3;

	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		//return false;
	}

	error = fclose(filePtr);
	if( error != 0)
	{
		return false;
	}

	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if(!m_heightMap)
	{
		return false;
	}

	int k=0;
	int index = 0;

	for(int i=0; i<m_terrainHeight; i++)
	{
		for(int j=0; j<m_terrainWidth; j++)
		{
			height = bitmapImage[k];
			index = m_terrainHeight*i + j;

			m_heightMap[index].x = (float)j;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)i;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

bool TerrainClass::CalculateNormals()
{
	int index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;

	normals = new VectorType[(m_terrainHeight-1) * (m_terrainWidth - 1)];
	if(!normals)
	{
		return false;
	}

	for(int i=0; i<m_terrainHeight - 1; i++)
	{
		for(int j=0; j<m_terrainWidth - 1; j++)
		{
			index1 = (i*m_terrainHeight) + j;
			index2 = (i*m_terrainHeight) + (j+1);
			index3 = (i+1) * m_terrainHeight + j;

			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;
		
			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;
		
			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (i * (m_terrainHeight-1)) + j;

			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	for(int i=0; i<m_terrainHeight; i++)
	{
		for(int j=0; j<m_terrainWidth; j++)
		{
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			count = 0;

			if(j-1 >= 0 && i-1 >= 0)
			{
				index = (i-1) * (m_terrainHeight - 1) + (j-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			if(j < m_terrainWidth - 1 && i-1 >= 0)
			{
				index = (i-1) * (m_terrainHeight - 1) + (j);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			if(j-1 >= 0 && i < m_terrainHeight - 1)
			{
				index = i * (m_terrainHeight - 1) + (j-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			if(j < m_terrainWidth-1 && i < m_terrainHeight-1)
			{
				index = i * (m_terrainHeight - 1) + j;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			sum[0] /= (float)count;
			sum[1] /= (float)count;
			sum[2] /= (float)count;

			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			index = i*m_terrainHeight + j;

			m_heightMap[index].nx = sum[0] / length;
			m_heightMap[index].ny = sum[1] / length;
			m_heightMap[index].nz = sum[2] / length;
		}
	}

	delete[] normals;
	normals = 0;

	return true;
}

void TerrainClass::NormalizeHeightMap()
{
	for(int i=0; i<m_terrainHeight; i++)
	{
		for(int j=0; j<m_terrainWidth; j++)
		{
			m_heightMap[(m_terrainHeight * i) + j].y /= 15.0f;
		}
	}

	return;
}

void TerrainClass::ShutdownHeightMap()
{
	if(m_heightMap)
	{
		delete [] m_heightMap;
		m_heightMap = 0;
	}

	return;
}

void TerrainClass::CalculateTextureCoordinates()
{
	int incrementCount, tuCount, tvCount;
	float incrementValue, tuCoord, tvCoord;

	incrementValue = (float)TEXTURE_REPEAT / (float)m_terrainWidth;
	incrementCount = m_terrainWidth / TEXTURE_REPEAT;

	tuCoord = 0.0f;
	tvCoord = 1.0f;

	tuCount = 0;
	tvCount = 0;

	for(int i=0; i<m_terrainHeight; i++)
	{
		for(int j=0; j<m_terrainWidth; j++)
		{
			m_heightMap[m_terrainHeight*i + j].tu = tuCoord;
			m_heightMap[m_terrainHeight*i + j].tv = tvCoord;

			tuCoord += incrementValue;
			tuCount++;

			//If we are at right end of tex, start at beggining
			if(tuCount == incrementCount)
			{
				tuCoord = 0;
				tuCount = 0;
			}
		}

		tvCoord -= incrementValue;
		tvCount++;

		if(tvCount == incrementCount)
		{
			tvCoord = 1.0f;
			tvCount = 0;
		}
	}

	return;
}

bool TerrainClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	m_texture = new TextureClass;
	if(!m_texture)
	{
		return false;
	}

	if(!m_texture->Initialize(device, filename))
	{
		return false;
	}

	return true;
}

void TerrainClass::ReleaseTexture()
{
	if(m_texture)
	{
		m_texture->ShutDown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}