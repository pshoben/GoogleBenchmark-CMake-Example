#ifndef LL_TINYMAPT_H
#define LL_TINYMAPT_H

namespace llutils {

struct Kvp16_t { // 16-bit id/offset pair
	unsigned char offset;
	unsigned char id;
};

struct Kvp32_t { // 32-bit id/offset pair
	unsigned short offset;
	unsigned short id;
};

typedef  Kvp32_t Kvp_t ;


template<size_t MaxFields, size_t MaxSize, typename BaseT = Kvp_t>
class TinyMapT {
public:
	BaseT(&buffer)[MaxSize];
	TinyMapT(BaseT(&pbuffer)[MaxSize])
		:buffer(pbuffer) 
	{
		buffer[0].offset = MaxFields; // offset of first value is immediately after the index
	}

	// bytes(0-MaxFields) is array of LLKV16 [numkeys]
	BaseT * get(unsigned short pid) {
		BaseT * kvp = buffer;
		for (auto i = 0; i < MaxFields; i++) {
			if (kvp->id == 0)
				break;
			if (kvp->id == pid) {
				return &(buffer[kvp->offset]);
			}
			kvp++;
		}
		return nullptr;
	}
	
	BaseT * allocate(unsigned short pid, size_t len)
	{
		BaseT* kvp = (BaseT*)buffer;
		for (auto i = 0; i < MaxFields; i++) {
			if (kvp->id == 0) {
				auto next_free_offset = kvp->offset;
				// next free offset is stored against id=0
				if (next_free_offset + len > MaxSize) {
					return nullptr; // allocate failed - not enough space in buffer for the value
				}
				kvp->id = pid;
				if (i < MaxFields - 1) {
					BaseT* next_kvp = kvp + 1;
					next_kvp->offset = next_free_offset + len;
				}
				return &(buffer[kvp->offset]);
			}
			kvp++;
		}
		return nullptr; // allocate failed - too many fields
	}
	/** add a new field without checking if it already exists */
	BaseT * add(unsigned short pid, const char * val, size_t len) {
		BaseT * write_address = allocate(pid, len);
		if (write_address != nullptr) {
			memcpy(write_address, val, len);
		}
		return write_address;
	}

	/** overwrite an existing value (or create if not exists)
	 * expects len to be no greater tha len of existing value  - caller manages type of values
	 * returns address the value was set at */
	BaseT * set(unsigned short pid, const char * val, size_t len) {
		BaseT * write_address = get(pid);
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