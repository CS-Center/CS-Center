struct scoped_mmap {

	void* ptr;
	int len;

	scoped_mmap();
	scoped_mmap(void*);
	scoped_mmap(int fd, int flen = -1);
  
  void munmap_obj();

	~scoped_mmap();

};
