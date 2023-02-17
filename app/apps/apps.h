#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "platform/application.h"

/*
 *
 *	Clan Samples currently has only one type of apps, "Samples". These apps work from the interface AppInfo.
 *	"Samples" is categorised into namespace samples. The namespace has a method to retrieve
 *  the AppInfo interface version of the app (tests::get_apps, samples::get_apps). 
 *
 */



namespace apps
{
    using CreateFunc = std::function<std::unique_ptr<clan::Application>()>;

     /**
     *  AppInfo - Used by Clan Samples to load a vkb::Applicaiton with the creation function (CreateFunc create).
     */

    class AppInfo
    {
     public:
        AppInfo(const std::string& id, const CreateFunc& create):
            id(id), create(create)
        {}
        std::string id;
        CreateFunc  create;
    };

     /**
     *    SampleInfo - These are individual applications which show different usages and optimizations of the Vulkan API.
     */

    class SampleInfo : public AppInfo
    {
    public:
        SampleInfo(const std::string& id, const CreateFunc& create, const std::string& category, const std::string& author, const std::string& name, const std::string& description, const std::vector<std::string>& tags={}):
            AppInfo(id, create), category(category), author(author), name(name), description(description), tags(tags) {}

        std::string              category;
	    std::string              author;
	    std::string              name;
	    std::string              description;
	    std::vector<std::string> tags;
    };

     /**
     * @brief Get a specific app
     *  
     * @param id - ID of a specific app
     * 
     * @return a AppInfo*
     */
    AppInfo* get_app(const std::string& id);

     /**
      * @brief Get all apps
      * 
      * @return const std::vector<AppInfo*> A list of all apps
      * 
     */
    std::vector<AppInfo*> get_apps();

     /**
      * @brief Get a specific sample
      * 
      * @param id - ID of a specific sample
      * 
      * @return a SampleInfo*
     */
    SampleInfo* get_sample(const std::string& id);

     /**
      * @brief Get all samples
      * 
      * @param categories - If not empty the lists will include samples that match one of the categories requested
      * @param tags - If not empty the lists will include samples that match one of the tages requested
      * 
      * @return std::vector<AppInfo*> A list of samples
     */
    std::vector<AppInfo*> get_samples(const std::vector<std::string>& categories = {}, const std::vector<std::string>& tages = {});
}