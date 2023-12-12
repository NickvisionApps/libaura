#ifndef PROGRESS_STATE_H
#define PROGRESS_STATE_H

namespace Nickvision::Aura::Taskbar
{
	/**
	 * @brief States of progress on a taskbar button.
	 */
	enum class ProgressState
	{
		NoProgress = 0,
		Indeterminate = 1,
		Normal = 2,
		Error = 4,
		Paused = 8
	};
}

#endif //PROGRESS_STATE_H