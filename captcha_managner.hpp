/**
 * 管理 captcha 到期 自动删除
 * Author:Rainboy
 */
#pragma once
#include <thread>
#include <map>
#include <atomic>
#include <mutex>
#include <utility>

#include "captcha.h"
#include "base64.hpp"

struct captcha_node {
    captcha_node() = default;
    captcha_node(captcha_node&& n )
        : code{std::move(n.code)},time_stamp{n.time_stamp}
    {}
    captcha_node(std::string&& code ,std::time_t time_stamp)
        : code{std::move(code)},time_stamp{time_stamp}
    {}
    void operator=(captcha_node&& n){
        code = std::move(n.code);
        time_stamp = n.time_stamp;
    }

    std::string code;
    std::time_t time_stamp;
};

class captcha_manager {

public:
    static captcha_manager & get() { //单例模式
        static captcha_manager instance;
        return instance;
    }
    auto create_captcha(std::string id)
        ->decltype(make_njnaruto_captcha())
    {
        auto capt = make_njnaruto_captcha();
        {
            std::unique_lock<std::mutex> lock(mtx_);
            //captcha_node n = 
            map_[std::move(id)] = { capt->get_code(), std::time(nullptr) };
            //map_.emplace(std::move(id),std::move(n));
        }
        return capt;
    }

    auto create_captcha_to_pngbase64(std::string_view id){
        auto capt = create_captcha(std::string(id));
        std::string rets = "{";
        rets.append(R"("id": ")");
        rets.append(id);
        rets.append(R"(",code: "data:image/png;base64,)");
        rets.append(Base64::Encode(
                    reinterpret_cast<const char *>( capt->get_image().data())
                    , capt->get_image().size()));
        rets.append(R"("})");
        return rets;
    }

    std::string get_captcha(const std::string& id){
        std::unique_lock<std::mutex> lock(mtx_);
        auto it = map_.find(id);
        if(  it== map_.end())
            return "";
        return it->second.code;
    }

    void del_captcha(const std::string& id){
        std::unique_lock<std::mutex> lock(mtx_);
        auto it = map_.find(id);
        if (it != map_.end())
            map_.erase(it);
    }

    void check_expire(){
        if (map_.empty())
            return;
        auto now = std::time(nullptr);
        std::unique_lock<std::mutex> lock(mtx_);
        for (auto it = map_.begin(); it != map_.end();) {
            if (now - it->second.time_stamp >= max_age_) {
                it = map_.erase(it);
            }
            else {
                ++it;
            }
        }

    }

    void set_max_inactive_interval(int seconds) {
        max_age_ = seconds;
    }

private:
    captcha_manager() = default;
    captcha_manager(const captcha_manager&) = delete;
    captcha_manager(captcha_manager&&) = delete;

    std::map<std::string, captcha_node> map_;
    std::mutex mtx_; 
    int max_age_{3*60}; //default 3 minute
    std::atomic_int64_t id_ = 0;
};
