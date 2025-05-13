#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	// ���ø��� ����� ������������ �ڷ����� ���������� ���� ��
	// ����� �ν��Ͻ�ȭ �ϱ� ���Ͽ� �����δ� Ʋ

	template<typename T>
	void	Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_Release(T& pInstance)
	{
		unsigned int		iRefCnt = 0;

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();

			if (0 == iRefCnt)
				pInstance = nullptr;
		}
		return iRefCnt;
	}

	template<typename T>
	unsigned int Safe_AddRef(T& pInstance)
	{
		unsigned int		iRefCnt = 0;

		if (nullptr != pInstance)		
			iRefCnt = pInstance->AddRef();

		return iRefCnt;
	}

	template<typename T>
	void writeVectorToBinary(std::vector<T>& vec, std::ostream& os) {
		// ������ ��� ������ ���� ���Ͽ� ����
		size_t size = vec.size();
		os.write((char*)(&size), sizeof(size_t));
		// ������ �� ��Ҹ� ���� ���Ͽ� ����
		os.write((char*)(vec.data()), sizeof(T) * size);
	}

	template<typename T>
	void readVectorFromBinary(std::istream& is, std::vector<T>& vec) {
		// ������ ��� ������ ���� ���Ͽ��� �о����
		size_t size;
		is.read((char*)(&size), sizeof(size_t));
		// ������ ũ�⸦ �����Ͽ� �޸𸮸� �Ҵ�
		vec.resize(size);
		// ������ ��Ҹ� ���� ���Ͽ��� �о����
		is.read((char*)(vec.data()), sizeof(T) * size);
	}



}

#endif // Engine_Function_h__
