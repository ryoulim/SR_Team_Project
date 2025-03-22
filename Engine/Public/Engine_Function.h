#pragma once

BEGIN(Engine)

template<typename T>
void Safe_Delete(T& Pointer)
{
	if (nullptr != Pointer)
	{
		delete Pointer;
		Pointer = nullptr;
	}
}

template<typename T>
void Safe_Delete_Array(T& Pointer)
{
	if (nullptr != Pointer)
	{
		delete[] Pointer;
		Pointer = nullptr;
	}
}

template<typename T>
unsigned int Safe_AddRef(T& Instance)
{
	unsigned int iRefCnt = { 0 };

	if (nullptr != Instance)
		iRefCnt = Instance->AddRef();

	return iRefCnt;
}

template<typename T>
unsigned int Safe_Release(T& Instance)
{
	unsigned int iRefCnt = { 0 };
	
	if (nullptr != Instance)
	{
		iRefCnt = Instance->Release();
		if (0 == iRefCnt)
			Instance = nullptr;
	}
	return iRefCnt;
}

template<typename Key, typename Value>
class BiMap {
    std::unordered_map<Key, Value> keyToVal;
    std::unordered_map<Value, Key> valToKey;

public:
    void insert(const Key& k, const Value& v) {
        keyToVal[k] = v;
        valToKey[v] = k;
    }

    bool findByKey(const Key& k, Value& outVal) const {
        auto it = keyToVal.find(k);
        if (it == keyToVal.end()) return false;
        outVal = it->second;
        return true;
    }

    bool findByValue(const Value& v, Key& outKey) const {
        auto it = valToKey.find(v);
        if (it == valToKey.end()) return false;
        outKey = it->second;
        return true;
    }

    void eraseByKey(const Key& k) {
        auto it = keyToVal.find(k);
        if (it != keyToVal.end()) {
            valToKey.erase(it->second);
            keyToVal.erase(it);
        }
    }

    void eraseByValue(const Value& v) {
        auto it = valToKey.find(v);
        if (it != valToKey.end()) {
            keyToVal.erase(it->second);
            valToKey.erase(it);
        }
    }
};

END