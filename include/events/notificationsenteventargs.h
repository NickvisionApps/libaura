#ifndef NOTIFICATIONSENTEVENTARGS_H
#define NOTIFICATIONSENTEVENTARGS_H

#include <string>
#include "eventargs.h"
#include "notificationseverity.h"

namespace Nickvision::Aura::Events
{
	/**
	 * @brief Event args for when a notification is sent
	 */
	class NotificationSentEventArgs : public EventArgs
	{
	public:
		/**
		 * @brief Constructs a NotificationSentEventArgs.
		 * @param message The message of the notification
		 * @param severity The severity of the notification
		 * @param action An additional action for the notification
		 * @param actionParam The parameter of the additional action
		 */
		NotificationSentEventArgs(const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "");
		/**
		 * @brief Gets the message of the notification.
		 * @return The message of the notification
		 */
		const std::string& getMessage() const;
		/**
		 * @brief Gets the severity of the notification.
		 * @return The severity of the notification
		 */
		NotificationSeverity getSeverity() const;
		/**
		 * @brief Gets the addition action of the notification.
		 * @return The additional action of the notification
		 */
		const std::string& getAction() const;
		/**
		 * @brief Gets the parameter of the additional action.
		 * @return The parameter of the additional action
		 */
		const std::string& getActionParam() const;
	protected:
		std::string m_message;
		NotificationSeverity m_severity;
		std::string m_action;
		std::string m_actionParam;
	};
}

#endif //NOTIFICATIONSENTEVENTARGS_H