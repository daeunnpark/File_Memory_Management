



typedef struct addr_in_use{
	void* addr;
	int ref_count;// = 0;

}addr_in_use;

extern addr_in_use addr;

