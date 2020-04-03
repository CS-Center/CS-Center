struct scoped_mmap {

	void* ptr;
	int len;

	scoped_mmap();
	scoped_mmap(void*);

	int mmap_whole(int fd, int flen = -1);

	~scoped_mmap();

};
