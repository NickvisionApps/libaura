#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <vector>
#include "passwordcontent.h"

namespace Nickvision::Keyring
{
	/**
	 * @brief A random password generator. 
	 */
	class PasswordGenerator
	{
	public:
		/**
		 * @brief Constructs a PasswordGenerator.
		 * @param contentFlags Flags of possible characters in a generated password
		 */
		PasswordGenerator(PasswordContent contentFlags = PasswordContent::Numeric | PasswordContent::Uppercase | PasswordContent::Lowercase | PasswordContent::Special) noexcept;
		/**
		 * @brief Gets the flags of possible characters in a generated password.
		 * @return The flags of possible characters in a generated password
		 */
		PasswordContent getContentFlags() const noexcept;
		/**
		 * @brief Sets the flags of possible characters in a generated password.
		 * @param contentFlags Flags of possible characters in a generated password
	 	 */
		void setContentFlags(PasswordContent contentFlags) noexcept;
		/**
		 * @brief Generates a new password
		 * @param length The length of the generated password
		 * @return The generated password
		 */
		std::string next(size_t length = 16) noexcept;

	private:
		std::vector<char> m_chars;
		PasswordContent m_contentFlags;

	private:
		static std::vector<char> m_numericChars;
		static std::vector<char> m_upperChars;
		static std::vector<char> m_lowerChars;
		static std::vector<char> m_specialChars;
	};
}

#endif //PASSWORDGENERATOR_H