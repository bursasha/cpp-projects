#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief CFile class to handle file operations including read, write, seek, truncate, version control, and undo operations.
 */
class CFile
{
public:
	CFile ()
	: cf_DataVector(), cf_VersionVector(), cf_Position(0)
	{
	}

	CFile (const CFile &source)
	: cf_Position(source.cf_Position)
	{
		cf_DataVector = source.cf_DataVector;
		cf_VersionVector = source.cf_VersionVector;
	}

	CFile &operator = (const CFile &source)
	{
		cf_DataVector = source.cf_DataVector;
		cf_VersionVector = source.cf_VersionVector;
		cf_Position = source.cf_Position;
		
		return *this;
	}
	~CFile ()
	{
		cf_DataVector.clear();
		cf_VersionVector.clear();
		cf_Position = 0;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Set the file position.
	 *
	 * @param offset The position to seek to.
	 * @return True if the seek is successful, false otherwise.
	 */
	bool seek (const uint32_t offset)
	{
		if (offset < 0 || offset >= cf_DataVector.max_size())
			return false;
		cf_Position = offset;
		return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Read bytes from the file.
	 *
	 * @param destination The destination buffer to read into.
	 * @param bytes The number of bytes to read.
	 * @return The number of bytes read.
	 */
	uint32_t read (uint8_t *destination, const uint32_t bytes)
	{
		uint32_t read_bytes = 0;
		for (size_t i = 0; i < bytes && cf_Position < cf_DataVector.size(); i++, read_bytes++)
			destination[i] = cf_DataVector[cf_Position++];
		return read_bytes;
	}

	/**
	 * @brief Write bytes to the file.
	 *
	 * @param source The source buffer to write from.
	 * @param bytes The number of bytes to write.
	 * @return The number of bytes written.
	 */
	uint32_t write (const uint8_t *source, const uint32_t bytes)
	{
		uint32_t old_position = cf_Position;
		for (size_t i = 0; i < bytes; i++, cf_Position++, source++)
		{
			if (cf_Position < cf_DataVector.size())
				cf_DataVector.insert(cf_Position, *source);
			else
				cf_DataVector.push_back(*source);
		}
		return (cf_Position - old_position);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Truncate the file at the current position.
	 */
	void truncate ()
	{
		while (cf_DataVector.size() > cf_Position)
			cf_DataVector.pop_back();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the file size.
	 *
	 * @return The size of the file.
	 */
	uint32_t fileSize () const
	{
		return cf_DataVector.size();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a new version of the file.
	 */
	void addVersion ()
	{
		cf_VersionVector.push_back(CVersion(cf_DataVector, cf_Position));
	}

	/**
	 * @brief Undo the last version of the file.
	 *
	 * @return True if the undo is successful, false otherwise.
	 */
	bool undoVersion ()
	{
		uint32_t array_size = cf_VersionVector.size();
		if (array_size == 0)
			return false;
		
		cf_DataVector = cf_VersionVector[array_size - 1].get_datavector();
		cf_Position = cf_VersionVector[array_size - 1].get_position();
		cf_VersionVector.pop_back();

		return true;
	}

private:
	/**
	 * @brief CVector template class to handle dynamic array operations.
	 */
	template <class T>
	class CVector
	{
	public:
		CVector ()
		: cvec_MaxSize(1), cvec_CurrentSize(0), cvec_Array(new T[cvec_MaxSize])
		{
		}

		~CVector () 
		{
			cvec_MaxSize = 1;
			cvec_CurrentSize = 0;
			delete[] cvec_Array;
			cvec_Array = nullptr;
		}

		// -------------------------------------------------------------------------------------------------------------

		/**
		 * @brief Assignment operator for CVector.
		 *
		 * @param source The source vector to copy from.
		 * @return Reference to the assigned vector.
		 */
		CVector &operator = (const CVector &source)
		{
			cvec_MaxSize = source.cvec_MaxSize;
			cvec_CurrentSize = source.cvec_CurrentSize;
			delete[] cvec_Array;

			cvec_Array = new T[cvec_MaxSize];
			for (size_t i = 0; i < cvec_CurrentSize; i++)
				cvec_Array[i] = source.cvec_Array[i];

			return *this;
		}

		T &operator [] (const uint32_t idx) const
		{
			return cvec_Array[idx];
		}

		// -------------------------------------------------------------------------------------------------------------

		/**
		 * @brief Get the first element of the vector.
		 *
		 * @return Reference to the first element.
		 */
		T &front () const
		{
			return cvec_Array[0];
		}

		/**
		 * @brief Get the last element of the vector.
		 *
		 * @return Reference to the last element.
		 */
		T &back () const
		{
			return cvec_Array[cvec_CurrentSize - 1];
		}

		// -------------------------------------------------------------------------------------------------------------

		/**
		 * @brief Get the current size of the vector.
		 *
		 * @return The size of the vector.
		 */
		uint32_t size () const
		{
			return cvec_CurrentSize;
		}

		/**
		 * @brief Get the maximum size of the vector.
		 *
		 * @return The maximum size of the vector.
		 */
		uint32_t max_size () const
		{
			return cvec_MaxSize;
		}

		/**
		 * @brief Check if the vector is empty.
		 *
		 * @return True if the vector is empty, false otherwise.
		 */
		bool is_empty () const
		{
			return cvec_CurrentSize == 0;
		}

		// -------------------------------------------------------------------------------------------------------------

		/**
		 * @brief Add an element to the end of the vector.
		 *
		 * @param data The data to add.
		 */
		void push_back (const T &data)
		{
			if (cvec_CurrentSize == cvec_MaxSize)
			{
				cvec_MaxSize *= 2;

				T *old_array = cvec_Array;
				cvec_Array = new T[cvec_MaxSize];

				for (size_t i = 0; i < cvec_CurrentSize; i++)
					cvec_Array[i] = old_array[i];

				delete[] old_array;
			}

			cvec_Array[cvec_CurrentSize++] = data;
		}

		/**
		 * @brief Insert an element at a specific index.
		 *
		 * @param idx The index to insert at.
		 * @param data The data to insert.
		 */
		void insert (const uint32_t idx, const T &data)
		{
			cvec_Array[idx] = data;
		}

		/**
		 * @brief Remove the last element from the vector.
		 */
		void pop_back ()
		{
			if (is_empty())
				return;

			cvec_CurrentSize--;
		}

		/**
		 * @brief Clear the vector.
		 */
		void clear ()
		{
			cvec_MaxSize = 1;
			cvec_CurrentSize = 0;
			delete[] cvec_Array;
			cvec_Array = nullptr;
		} 

	private:
		uint32_t cvec_MaxSize;
		uint32_t cvec_CurrentSize;
		T *cvec_Array;

	};

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief CVersion class to handle version control of the file.
	 */
	class CVersion
	{
	public:
		CVersion ()
		: cver_DataVector(), cver_Position()
		{
		}

		CVersion (const CVector <uint8_t> &src_datavector, const uint32_t &src_position)
		: cver_Position(src_position)
		{
			cver_DataVector = src_datavector;
		}

		~CVersion ()
		{
			cver_DataVector.clear();
			cver_Position = 0;
		}

		// -------------------------------------------------------------------------------------------------------------

		/**
		 * @brief Get the data vector of the version.
		 *
		 * @return Reference to the data vector.
		 */
		CVector <uint8_t> &get_datavector ()
		{
			return cver_DataVector;
		}

		/**
		 * @brief Get the position of the version.
		 *
		 * @return Reference to the position.
		 */
		uint32_t &get_position ()
		{
			return cver_Position;
		}

	private:
		CVector <uint8_t> cver_DataVector;
		uint32_t cver_Position;

	};

	CVector <uint8_t> cf_DataVector;
	CVector <CVersion> cf_VersionVector;
	uint32_t cf_Position;

};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Helper function to test writing to the file.
 *
 * @param x The file object.
 * @param data The data to write.
 * @param wrLen The expected number of bytes written.
 * @return True if the test passes, false otherwise.
 */
bool writeTest (CFile &x, const initializer_list<uint8_t> &data, uint32_t wrLen)
{
	return x.write(data.begin(), data.size()) == wrLen;
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Helper function to test reading from the file.
 *
 * @param x The file object.
 * @param data The expected data to read.
 * @param rdLen The expected number of bytes read.
 * @return True if the test passes, false otherwise.
*/
bool readTest (CFile &x, const initializer_list<uint8_t> &data, uint32_t rdLen)
{
	uint8_t tmp[100];
	uint32_t idx = 0;

	if (x.read(tmp, rdLen) != data.size())
		return false;
	for (auto v : data)
		if (tmp[idx++] != v)
			return false;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main ( void )
{
  CFile f0;

  assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
  assert ( f0 . fileSize () == 3 );
  assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 2 ));
  assert ( writeTest ( f0, { 5, 4 }, 2 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 1 ));
  assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
  assert ( f0 . seek ( 3 ));
  f0 . addVersion();
  assert ( f0 . seek ( 6 ));
  assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
  f0 . addVersion();
  assert ( f0 . seek ( 5 ));
  CFile f1 ( f0 );
  f0 . truncate ();
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
  assert ( !f0 . seek ( 100 ));
  assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
  assert ( f1 . seek ( 0 ));
  assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
  assert ( f1 . undoVersion () );
  assert ( f1 . undoVersion () );
  assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
  assert ( !f1 . undoVersion () );

  return EXIT_SUCCESS;
}
