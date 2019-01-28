//
// listFiles
//


//
// STL includes
//

#include <vector>


//
// Standard includes
//

#include <string>
#include <dirent.h>
#include <iostream>


//
// Function: listFiles
//

// Source:
// https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/

std::vector<std::string> listFiles(std::string path) {

	// The list of files will be stored in an STL vector to
	// avoid the hassle of memory management. The vector
	// consists of strings
	std::vector<std::string> files;

	// The DIR and dirent types are defined in 
	// dirent.h and should be portable to any
	// version of Linux
	DIR *dp;
	struct dirent *dirp;

	// Open a connection to the path provided. It is
	// almost like reading the contents of a file in
	// C.
	dp = opendir(path.c_str());

	// If the pointer is NULL, then the connection failed.
	if (dp == NULL) {
		std::cerr << "Unable to process " << path << std::endl;
		// Since the connection failed, we will return an
		// empty list back to the calling function
		return files;
	}

	// Read the first directory entry
	dirp = readdir(dp);

	// Just like file I/O, we loop until there
	// are no more entries
	while (dirp != NULL) {
		// The field in the struct dirent that contains the
		// filename is d_name. It is a char*. Since our
		// vector is expecting strings, it is converted.
		// Plus we can use the handy C++ string functions.
		auto s = std::string(dirp->d_name);

		// The file has to meet two criteria for inclusion:
		//    (1) The filename has to be at least five
		//        characters long, and
		//    (2) The filename has to end with .txt
		if ((s.size() > 4) && (s.substr(s.size()-4) == ".txt")) {
			// If the filename meets our criteria, add it to our
			// vector container
			files.push_back(s);
		}

		// Fetch the next entry
		dirp = readdir(dp);
	}

	// All done, so close the directory connection
	closedir(dp);

#ifdef _DEBUG_
	// During debugging, let's print out the files we kept
	std::cout << "There are " << files.size() << " files" << std::endl;

	for (auto f : files)
		std::cout << f << std::endl;
#endif

	return files;
}


