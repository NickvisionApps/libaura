#ifndef WEBHELPERS_H
#define WEBHELPERS_H

#include <filesystem>
#include <functional>
#include <string>
#include <curl/curl.h>

namespace Nickvision::WebHelpers
{
	typedef std::function<int(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)> CurlProgressFunction;

	/**
	 * @brief Gets whether or not a url points to a valid website.
	 * @param url The url to check
	 * @return True if valid website, else false
	 */
	bool isValidWebsite(const std::string& url) noexcept;
	/**
	 * @brief Downloads a file to disk.
	 * @param url The url of the file to download
	 * @param path The path on disk to save the file
	 * @param progress An optional function to receive progress on the download \n
	 * std::function<int(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)> \n 
	 *	dltotal - The total number of bytes to be downloaded \n 
	 *	dlnow   - The total number of bytes currently downloaded \n 
	 *  ultotal - The total number of bytes to be uploaded (will always be 0) \n 
	 *	ulnow   - The total number of bytes currently uploaded (will always be 0) \n 
	 * @param overwrite Whether or not to overwrite existing files on disk
	 */
	bool downloadFile(const std::string& url, const std::filesystem::path& path, const CurlProgressFunction& progress = {}, bool overwrite = true) noexcept;
	/**
	 * @brief Fetches a json string from a url.
	 * @param url The url of the json string
	 * @return The fetched json string
	 */
	std::string fetchJsonString(const std::string& url) noexcept;
}

#endif //WEBHELPERS_H