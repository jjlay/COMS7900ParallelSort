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


using namespace std;


//
// Function: listFiles
//

// Source:
// https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/

vector<string> listFiles(string path, int numFiles) {

	// The list of files will be stored in an STL vector to
	// avoid the hassle of memory management. The vector
	// consists of strings
	vector<string> files;

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
		cerr << "Unable to process " << path << endl;
		// Since the connection failed, we will return an
		// empty list back to the calling function
		return files;
	}

	// Read the first directory entry
	dirp = readdir(dp);

	// Just like file I/O, we loop until there
	// are no more entries
	
	int filesProcessed = 0;

	while (dirp != NULL) {
		// The field in the struct dirent that contains the
		// filename is d_name. It is a char*. Since our
		// vector is expecting strings, it is converted.
		// Plus we can use the handy C++ string functions.
		auto s = string(dirp->d_name);

		// The file has to meet two criteria for inclusion:
		//    (1) The filename has to be at least five
		//        characters long, and
		//    (2) The filename has to end with .txt
		if ((s.size() > 4) && (s.substr(s.size()-4) == ".txt") &&
			(filesProcessed < numFiles)) {
			// If the filename meets our criteria, add it to our
			// vector container
			files.push_back(path + s);
			filesProcessed++;
		}

		// Fetch the next entry
		dirp = readdir(dp);
	}

	// All done, so close the directory connection
	closedir(dp);

	cout << "listFiles: There are " << files.size() << " to process" << endl;

	return files;
}


