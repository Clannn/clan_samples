#include "configuration.h"

namespace clan
{
    BoolSetting::BoolSetting(bool &handle, bool value) :
        m_handle{handle},
        m_value{value}
    {}

    void BoolSetting::set()
    {
        m_handle = m_value;
    }

    std::type_index BoolSetting::get_type()
    {
        return typeid(BoolSetting);
    }

    IntSetting::IntSetting(int &handle, int value) :
        m_handle{handle},
        m_value{value}
    {}

    void IntSetting::set()
    {
        m_handle = m_value;
    }

    std::type_index IntSetting::get_type()
    {
        return typeid(IntSetting);
    }

    EmptySetting::EmptySetting()
    {}

    void EmptySetting::set()
    {}

    std::type_index EmptySetting::get_type()
    {
        return typeid(EmptySetting);
    }

    void Configuration::set()
    {
        for(auto pair : m_current_configuration->second){
            for(auto setting : pair.second){
                setting->set();
            }
        }
    }

    bool Configuration::next()
    {
        if(m_configs.size() == 0){
            return false;
        }
        m_current_configuration++;
        if(m_current_configuration == m_configs.end()){
            return false;
        }
        return true;
    }

    void Configuration::reset()
    {
        m_current_configuration = m_configs.begin();
    }

    void Configuration::insert_setting(uint32_t config_index, std::unique_ptr<Setting> setting)
    {
        m_settings.push_back(std::move(setting));
        m_configs[config_index][m_settings.back()->get_type()].push_back(m_settings.back().get());
    }
}   // namespace clan