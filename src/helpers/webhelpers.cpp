#include "helpers/webhelpers.h"
#include <fstream>

namespace Nickvision::Aura
{
	bool WebHelpers::getIsValidWebsite(const std::string& url)
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		CURL* curl{ curl_easy_init() };
		if (!curl)
		{
			return false;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		curl_easy_setopt(curl, CURLOPT_HEADER, false);
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return code == CURLE_OK;
	}

	bool WebHelpers::downloadFile(const std::string& url, const std::filesystem::path& path, const CurlProgressFunction& progress, bool overwrite)
	{
		if (std::filesystem::exists(path) && !overwrite)
		{
			return false;
		}
		curl_global_init(CURL_GLOBAL_DEFAULT);
		CURL* curl{ curl_easy_init() };
		if (!curl)
		{
			return false;
		}
		std::ofstream out{ path, std::ios::binary | std::ios::trunc };
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t nmemb, void* data)
		{
			std::ofstream* stream{ (std::ofstream*)data };
			stream->write(ptr, size * nmemb);
			return size * nmemb;
		});
		if (progress)
		{
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
			curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progress);
			curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, [](void* data, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
			{
				CurlProgressFunction& func{ *((CurlProgressFunction*)data) };
				return func(dltotal, dlnow, ultotal, ulnow);
			});
		}
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return code == CURLE_OK;
	}

	std::string WebHelpers::fetchJsonString(const std::string& url)
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		CURL* curl{ curl_easy_init() };
		if (!curl)
		{
			return "";
		}
		std::stringstream out;
		struct curl_slist* listHttpHeader{ curl_slist_append(nullptr, "Content-Type: application/json") };
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/120.0");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, listHttpHeader);
		curl_easy_setopt(curl, CURLOPT_HEADER, false);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t nmemb, void* data)
		{
			std::stringstream* stream{ (std::stringstream*)data };
			stream->write(ptr, size * nmemb);
			return size * nmemb;
		});
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(listHttpHeader);
		curl_global_cleanup();
		return code == CURLE_OK ? out.str() : "";
	}
}