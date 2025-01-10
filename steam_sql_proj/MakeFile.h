#pragma once

#include <fstream>
#include <filesystem>

#define DATA_FOLDER "../DATA"
#define ERROR_LOG_FOLDER "../ERROR_LOG"


/*
This section is about making files. There is no flexibility here and is not reusable.

Similar to Entity, seemingly odd design decidions which really are mostly done to look good. The OUTPUT_HANDLE and INPUT_HANDLE
which are classes the really do nothing besides just reading from or into files. They make no checks or have any functions.
Just dump data into files and using relative paths defined as macros and a guaranteed file close with destructor.
Meaning when these handles are usually called at the end of functions, the construct and then destrut pretty much right after.
If any new file making using this format is desired, just make more constructors.
*/

namespace ORDO {

	class FILE_MANAGER;

	class OUTPUT_HANDLE {
		friend class FILE_MANAGER;
		std::ofstream output;
		OUTPUT_HANDLE(const std::filesystem::path& path, const nlohmann::json& data);
		OUTPUT_HANDLE(const std::filesystem::path& path, const std::vector<std::string>& data);
	public:
		~OUTPUT_HANDLE() {
			output.close();
		}
	private:
		OUTPUT_HANDLE() = default;
	};

	class INPUT_HANDLE {
		std::ifstream input;
	public:
		INPUT_HANDLE(const std::filesystem::path& file_path, nlohmann::json& into);
		~INPUT_HANDLE() {
			input.close();
		}
	private:
		INPUT_HANDLE() = default;
	};


	/*
	FILE_MANAGER will first and always upton calling check if the foler itself exists using std::filesystem::create_directories.
	That function will not create a file if it already exists thus achieving intended behavior. However FILE_MANAGER should always
	and only be used with the macro paths defined at the top, though there are no hard checks preventing otherwise. Manger contains
	a single member which is a relative path. It is modified during MakeJSON creation.
	*/
	class FILE_MANAGER {
		std::filesystem::path some_path;


		//looks into the current working path assined by the constructor (the constructor should've created one if it was missing)
		//and retrieves its content, if there is any.
		const std::vector<std::string> GetFolderContents()const;


		//in a recursive manner modfies the name parameter if a file with the same name already exists in the folder. Adds copy+number to it
		void AssignNameCopyCheck(const std::vector<std::string>& folder_contents, std::string& name, const std::string& extension, int copy_index = 1)const;


	public:
		FILE_MANAGER(const std::string& path) :some_path(path) {
			if (std::filesystem::create_directories(some_path)) {
				LogMessage("Folder missing. Created new at >> " + some_path.string());
			}
		}
		FILE_MANAGER(const char* path) :some_path(path) {
			if (std::filesystem::create_directories(some_path)) {
				LogMessage("Folder missing. Created new at >> " + some_path.string());
			}
		}


		//creates a JSON file. Lets the users assign a name. Checks if name exists and modifies the name if it does.
		//This and MakeManyJSON are kind of not great because of how the DATA is set up, how it is stored.
		//Strictly data in MDATA which is declared as DATA will always be singular while ACHIEVEMENTS could have
		//multiple entries. The only real difference is that MakeManyJSON assings name automatically using the
		//ACHIEVEMENTS::GetStoredNames function
		void MakeJSON(const std::vector<std::unique_ptr<nlohmann::json>>& data)const;
		void MakeManyJSON(const std::vector<std::unique_ptr<nlohmann::json>>& list, const std::unique_ptr<std::vector<std::string>>& names)const;
	};
}