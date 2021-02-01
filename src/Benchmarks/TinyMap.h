#ifndef LL_TINYMAP_H
#define LL_TINYMAP_H

namespace llutils {

struct LLKVP16 { // 16-bit id/offset pair
	unsigned char offset;
	unsigned char id;
};

struct LLKVP32 { // 16-bit id/offset pair
	unsigned short offset;
	unsigned short id;

};
using LLKVP = LLKVP16;

template<size_t MaxFields, size_t MaxSize>
class LLTinyMap {
public:
	char (&buffer)[MaxSize];
	LLTinyMap(char(&pbuffer)[MaxSize])
		:buffer(pbuffer) { }

	// bytes(0-MaxFields) is array of LLKV16 [numkeys]
	void * get(unsigned short pid) {
		char* p = (char*) &buffer;
		for (auto i = 0; i < MaxFields; i++) {
			LLKVP* kvp = (LLKVP*)p;
			if (kvp->id == 0)
				break;
			if (kvp->id == pid) {
				return &(buffer[kvp->offset]);
			}
			p += sizeof(LLKVP);
		}
		return nullptr;
	}
	
	void* allocate(unsigned short pid, size_t len)
	{
		for (auto i = 0; i < MaxFields; i++) {
			char* p = &(buffer[i * sizeof(LLKVP)]);
			LLKVP* kvp = (LLKVP*)p;
			if (kvp->id == 0) {
				unsigned char next_free_offset = kvp->offset + len;
				// next free offset is stored against id=0
				if (next_free_offset > MaxSize) {
					return nullptr; // allocate failed - not enough space in buffer for the value
				}
				(kvp + 1)->offset = next_free_offset;
				return &(buffer[kvp->offset]);
			}
		}
		return nullptr; // allocate failed - too many fields
	}
	/** add a new field without checking if it already exists */
	void* add(unsigned short pid, const char * val, size_t len) {
		auto write_address = allocate(pid, len);
		if (write_address == nullptr) {
			return nullptr; // alloc failed
		}
		memcpy(write_address, val, len);
		return write_address;
	}

	/** overwrite an existing value (or create if not exists)
	 * expects len to be no greater tha len of existing value  - caller manages type of values
	 * returns address the value was set at */
	void* set(unsigned short  pid, const char * val, size_t len) {
		void* write_address = get(pid);
		if (write_address == nullptr) {
			// doesn't exist, add to the end of the map
			return add(pid, val, len);
		} else {
			memcpy(write_address, val, len);
			return write_address;
		}
	}
};
}

#endif