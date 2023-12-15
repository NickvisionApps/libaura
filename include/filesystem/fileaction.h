#ifndef FILEACTION_H
#define FILEACTION_H

namespace Nickvision::Aura::Filesystem
{
	/**
	 * @brief Actions that cause a file to change. 
	 */
	enum class FileAction
	{
		Added = 1,
		Removed,
		Modified,
		Renamed
	};
}

#endif //FILEACTION_H