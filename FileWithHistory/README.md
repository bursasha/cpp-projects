# Your task is to develop class CFile to simulate a binary file.

The interface of the class is described below. There are read/write operations, file seeking, and truncating. Moreover, we require versioning, i.e. the current contents of the file can be stored and subsequently restored. The versioning will support an unlimited number of version (there is an implicit memory limit). Any of the previous versions can by restored in an undo-like manner. The class will not manipulate actual files on the disk (the disk will not be writable). Instead, the data re to be stored in the main memory (the class just simulates a file).

Instances of CFile class can be copied, either by copy constructor, or by assignment operator. The copies shall be identical independent instances, modifications of one instance must not affect the other. On the other hand, the modifications are likely to modify only a portion of the file contents, thus it may be desirable to share some data to save memory. The copies are identical, i.e. the copying also copies all stored versions of the file.

Required class interface:
- constructor: default constructor initializes an empty instance of CFile (size 0 B, file position 0),
- destructor, op=, and copy constructor: must be implemented if the automatically generated variant is not usable,
- write (data, len): method writes bytes a block of bytes (data) of length len bytes into the file. The writing start at the actual file position, the position is advanced by the number of bytes written. The method overwrites/adds the bytes to the file as needed. Return value is the number of bytes written.
- read (data, len): method reads bytes from the actual file position. The bytes are read into buffer data, the method reads up to len bytes. The actual number of bytes read is returned as a return value (may be less than len, based on the file position and file size).Next, the method advances file position by the number of bytes it actually read.
- seek ( pos ): the method changes file position to pos. The modified file position will be used in subsequent read/write operations. Parameter pos must fit into interval 0 to file size (both inclusive). Return value is either true to indicate success, or false to report an error (position outside the valid range),
- truncate (): the method truncates the file to the actual position in the file,
- fileSize (): the method returns the actual size of the file,
- addVersion (): the method adds the current version of the instance (a snapshot) into the list of file versions. Both file contents and file position are archived in the file versions. The versions are archived inside the CFile instance,
- undoVersion (): the method reverts the contents (and file position) of the file to the previous version (archived by a call to addVersion). Any previously saved versions may be recovered, i.e. undoVersion may be called multiple times. The method returns true to indicate success, or false to indicate a problem (no previous version available).

Submit a source file with your implementation of CFile. The class must follow the public interface below. If there is a mismatch in the interface, the compilation will fail. You may extend the interface and add you auxiliary methods and member variables (both public and private, although private are preferred). The submitted file must include both declarations as well as implementation of the classes (the methods may be implemented inline but do not have to). The submitted file shall not contain main, your tests, or any #include definitions. If present, please, keep them in a conditional compile block. Use the template below as a basis for your implementation. If the preprocessor definitions are preserved, the file may be submitted to Progtest.

The class is tested in a limited environment -- both memory and running time is limited. The available memory is big enough to store the records. Do not use STL and std::string. These classes are disabled in the testing environment. This homework is to practice shallow/deep object copying. If STL and string is available, the homework would be degraded.

### Notes:
- The assessment consists of mandatory, optional, and bonus tests. A basic implementation of the copy functionality is required to pass the mandatory tests.
- The optional and bonus tests require memory efficient implementation. You may assume that only parts of the file contents change beweeen versions/copies. The implementation must be enhanced with reference counting and copy-on- write techniques to pass the optional and bonus tests.
- The optional test creates copies of CFile and adds versions of the file contents. However, there are not many of them, thus counted references for the versions may save a lot of memory.
- The bonus test modifies the copied files very often, producing many different versions of the content. Therefore counted references alone do not help. Memory efficiency may be achieved by copy-on-write technique, moreover, copy-on-write must be reasonably fine grained (copy-on-write for an entire file version is too coarse).
- A solution of this problem result may be used for code review if it passes all mandatory and optional tests with 100% results. That is, your solution does not have to pass the bonus test for code review.
