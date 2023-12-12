#ifndef TASKBAR_ITEM_H
#define TASKBAR_ITEM_H

#include "progressstate.h"
#ifdef _WIN32
#include <windows.h>
#include <atlbase.h>
#include <shlobj_core.h>
#endif

namespace Nickvision::Aura::Taskbar
{
	/**
	 * @brief An item on the taskbar. 
	 */
	class TaskbarItem
	{
	public:
		/**
		 * @brief Constructs a TaskbarItem. 
		 */
		TaskbarItem();
		/**
		 * @brief Deconstructs a TaskbarItem.
		 */
		~TaskbarItem();
		/**
		 * @brieg Gets the state of the progress.
		 * @return ProgressState
		 */
		ProgressState getProgressState() const;
		/**
		 * @brief Sets the state of the progress.
		 * @param state The new ProgressState
		 */
		void setProgressState(ProgressState state);
		/**
		 * @brief Gets the value of the progress.
		 * @return The progress value
		 */
		double getProgress() const;
		/**
		 * @brief Sets the value of the progress. Settings the progress value will set the progress state to normal.
		 * @param progress The new progress value
		 */
		void setProgress(double progress);
		/**
		 * @brief Gets whether or not the taskbar item is shown in an urgent state.
		 * @return True if in urgent state, else false
		 */
		bool getUrgent() const;
		/**
		 * @brief Sets whether or not the taskbar item is shown in an urgent state.
		 * @param urgent True for urgent state, else false
		 */
		void setUrgent(bool urgent);
		/**
		 * @brief Gets whether or not the count is visible on the taskbar item.
		 * @return True if count visible, else false
		 */
		bool getCountVisible() const;
		/**
		 * @brief Sets whether or not the count is visible on the taskbar item.
		 * @param countVisible True for visible count, else false
		 */
		void setCountVisible(bool countVisible);
		/**
		 * @brief Gets the count shown on the taskbar item.
		 * @return The count value
		 */
		long getCount() const;
		/**
		 * @brief Sets the count shown on the taskbar item.
		 * @param count The new count value
		 */
		void setCount(long count);
#ifdef _WIN32
		/**
		 * @brief Connects a taskbar item to the application.
		 * @param hwnd The HWND of the application
		 * @return True if connectin successful, else false
		 */
		bool connect(HWND hwnd);
#endif

	private:
		ProgressState m_progressState;
		double m_progress;
		bool m_urgent;
		bool m_countVisible;
		long m_count;
#ifdef _WIN32
		HWND m_hwnd;
		CComPtr<ITaskbarList3> m_taskbar;
#endif
	};
}

#endif //TASKBAR_ITEM_H