#pragma once
#include "bmf.h"
#include "lodepng.h"
#include "njnaruto_bmf.hpp"

#include <memory>
#include <random>
#include <string_view>
#include <chrono>

class image {
public:
    explicit image(size_t width,size_t height)
        :width_{width},height_{height}
    {
        buff_.resize(width_*height_*4);
    }
    inline size_t pos(size_t x,size_t y) {
        return (x +  y*width_) <<2;
    }

    void set_pixel(RGBA rgba,int x,int y){
        if(x < 0 || x >= width_ || y <0 || y >= height_) return;
        auto p_  = pos(x,y);
        buff_[p_] = rgba.rgba[0];
        buff_[p_+1] = rgba.rgba[1];
        buff_[p_+2] = rgba.rgba[2];
        buff_[p_+3] = rgba.rgba[3];
    }

    unsigned encode() {
        png_.clear();
        return lodepng::encode(png_, buff_, width_, height_);
    }
    void save_file(const char * filename){
        auto error = encode();
        if(!error) lodepng::save_file(png_, filename);
    }
    std::vector<unsigned char> buff_;
    std::vector<unsigned char> png_;
    size_t width_,height_;
};

struct Random {
    std::mt19937 engine;
    std::uniform_int_distribution<int> dis; // in [0,0x7fffffffffffffff]

    int operator()(){ return dis(engine); }
    template<typename U> //产生一定范围内的随机数
    U operator()(U l,U r){ return dis(engine) % ( r-l+1 ) + l; }
    Random(){
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::time_point_cast<std::chrono::milliseconds>( std::chrono::system_clock::now()).time_since_epoch()
                ).count();
        engine.seed(now);
    }
    Random(int l,int r){ dis = std::uniform_int_distribution<int> (l,r); }

    Random create(int l,int r){ return Random(l,r); } //工厂模式
};

template<size_t width,size_t height>
class captcha {
public:
    captcha(const bmf & bmf_) : bmf_{bmf_} {
        make_captcha();
    }
    void set_char(char id,int x,int y){
        auto [exist,Char]= bmf_.get_char(id);
        if( !exist) return;
        int xx = x;
        //std::cout << Char->x << std::endl;
        //std::cout << Char->y << std::endl;
        //std::cout << Char->width << std::endl;
        //std::cout << Char->height<< std::endl;
        for(int j = Char->y ;j < Char->height + Char->y-1;j++){
            for( int i  = Char->x; i <Char->width+Char->x-1 ;i++,xx++){
                auto p = bmf_.get_pixel(i, j);
                //std::cout << std::hex <<p.value << " ";
                //if( p.value !=0 )
                    //std::cout << 1 ;
                //else
                    //std::cout << 0 ;
                image_.set_pixel(bmf_.get_pixel(i, j),xx,y);
            }
            //std::cout  << std::endl;
            xx=x;
            y++;
        }
    }
    void encode(){
        image_.encode();
    }
    void save_to_file(const char * filename = "test.png"){
        image_.save_file(filename);
    }
    
    std::string_view get_rand_str(){
        auto len = bmf_.char_set.length();
        auto rnd = Random();
        for(int i=0;i<=3;++i){
            rand_char[i] = bmf_.char_set[ rnd(0, int(len-1)) ];
        }
        return std::string_view(rand_char,4);
    }

    auto make_captcha(){
        auto str = get_rand_str();
        int x = 0 ,y= 0;
        for (const auto& e : str) {
            set_char(e, x ,y);
            auto [exist,info] = bmf_.get_char(e);
            if( exist)
                x += info->width;
        }
        image_.encode(); //TODO 检查错误
    }
    std::string get_code(){
        return std::string(rand_char,4);
    }

    auto& get_image() const {
        return image_.png_;
    }

private:
    char rand_char[5]{0};
    const bmf &bmf_;
    image image_{width,height};
};

//#define njnaruto_captcha std::captcha<300,100>()

auto make_njnaruto_captcha(){
    return std::make_unique<captcha<300,100>>(njnaruto_bmf);
}

