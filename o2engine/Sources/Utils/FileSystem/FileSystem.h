#pragma once

#include "Utils/Singleton.h"
#include "Utils/String.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Containers/Array.h"
#include "Utils/Containers/Dictionary.h"

namespace o2
{
	class LogStream;

	/** File system, singleton. Contains basic resource path. */
	class FileSystem: Singleton<FileSystem>
	{
		typedef Array<String> StringsArr;
		typedef Dictionary<FileType, StringsArr> ExtnsionsDict;

		LogStream*    mLog;
		ExtnsionsDict mExtensions;
		String        mResourcesPath; /**< Basic application resources path. */

	public:
		FileSystem();
		~FileSystem();

		/** Returns resource path. */
		static const String& GetResourcesPath();

		/** Returns info of paths and files of specified path. */
		static PathInfo GetPathInfo(const String& path);

		/** Returns file info. */
		static FileInfo GetFileInfo(const String& path);

		/** Copying file. */
		static bool CopyFile(const String& source, const String& dest);

		/** Deletion file. */
		static bool DeleteFile(const String& file);

		/** Moving file. */
		static bool MoveFile(const String& source, const String& dest);

		/** Creates folder. */
		static bool CreateDirectory(const String& path, bool recursive = true);

		/** Removes directory. */
		static bool RemoveDirectory(const String& path, bool recursive = true);

		/** Returns true if specified directory exist. */
		static bool IsDirectoryExist(const String& path);

		/** Returns true if specified file exist. */
		static bool IsFileExist(const String& path);

		static String ExtractPathStr(const String& path);
	};
}